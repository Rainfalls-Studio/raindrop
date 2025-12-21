#pragma once

#include <utility>
#include <functional>
#include <shared_mutex>
#include <mutex>

namespace ResourceRegistry{
    template<typename T>
    class LockedResource {
        public:
            using ValueType = T;
            using Callback = std::function<void(const T&)>;

            LockedResource() requires std::is_default_constructible_v<T> = default;

            explicit LockedResource(T initial) : _value(std::move(initial)) {}

            // Take a safe copy for use without holding locks.
            T snapshot() const {
                std::shared_lock lock(_mtx);
                return _value;
            }

            // Read-only access under shared lock (no escaping references).
            template <class Fn>
            auto read(Fn&& fn) const -> decltype(fn(std::declval<const T&>())) {
                std::shared_lock lock(_mtx);
                return fn(_value);
            }

            // Mutate under exclusive lock. Returns whatever fn returns.
            // If you want "changed?" logic, make fn return bool and use write_if_changed().
            template <class Fn>
            auto write(Fn&& fn) -> decltype(fn(std::declval<T&>())) {
                decltype(fn(_value)) result;
                bool should_notify = false;
                T snap_for_callbacks;

                {
                std::unique_lock lock(_mtx);
                result = fn(_value);
                should_notify = !_callbacks.empty();
                if (should_notify) snap_for_callbacks = _value;
                }

                if (should_notify) notify_(snap_for_callbacks);
                return result;
            }

            // Convenience: only notify if predicate says "changed".
            // Example:
            // cfg.write_if_changed([](Config& c){ c.x=3; return true; });
            template <class Fn>
            auto write_if_changed(Fn&& fn) -> decltype(fn(std::declval<T&>())) {
                decltype(fn(_value)) changed;
                
                T snap_for_callbacks;
                bool should_notify = false;

                {
                    std::unique_lock lock(_mtx);
                    changed = fn(_value);
                    if (static_cast<bool>(changed) && !_callbacks.empty()) {
                        should_notify = true;
                        snap_for_callbacks = _value;
                    }
                }

                if (should_notify) notify_(snap_for_callbacks);
                return changed;
            }

            // Subscribe to change notifications.
            // NOTE: callback may be called from any thread that performs a write().
            void on_change(Callback cb) {
                std::scoped_lock lock(_cb_mtx);
                _callbacks.push_back(std::move(cb));
            }

        private:
            void notify_(const T& snap) {
                // Copy callbacks under lock to avoid reentrancy issues / deadlocks.
                std::vector<Callback> cbs;
                {
                    std::scoped_lock lock(_cb_mtx);
                    cbs = _callbacks;
                }
                for (auto& cb : cbs) cb(snap);
            }

            mutable std::shared_mutex _mtx;
            T _value{};

            mutable std::mutex _cb_mtx;
            std::vector<Callback> _callbacks;
    };
}