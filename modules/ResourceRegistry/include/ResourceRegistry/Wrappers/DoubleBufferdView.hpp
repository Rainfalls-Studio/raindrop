#pragma once

#include <atomic>
#include <type_traits>
#include <utility>

namespace ResourceRegistry{
    template<typename T>
    class DoubleBufferedView {
        public:
            using ValueType = T;

            DoubleBufferedView() requires std::is_default_constructible_v<T> = default;

            explicit DoubleBufferedView(T initial_front)
                : buffers_{std::move(initial_front), buffers_[1]} {}

            // Read current front by const reference (no copy).
            // Safe if consumers don't keep the reference across an arbitrary swap boundary.
            const T& front_ref() const noexcept {
                const uint32_t fi = front_index_.load(std::memory_order_acquire);
                return buffers_[fi];
            }

            // Always safe: returns a copy.
            T front_copy() const {
                return front_ref();
            }

            // Mutable access to back buffer (producer side).
            T& back_ref() noexcept {
                return buffers_[back_index_()];
            }

            // Producer helper: mutate back via a function.
            template <class Fn>
            auto write_back(Fn&& fn) -> decltype(fn(std::declval<T&>())) {
                return fn(back_ref());
            }

            // Publish: make back become front.
            // Readers that fetch front_ref() after this will see the new snapshot.
            void swap() noexcept {
                const uint32_t old_front = front_index_.load(std::memory_order_relaxed);
                const uint32_t new_front = 1u - old_front;
                // We assume producer only wrote to new_front (i.e. old back) before swapping.
                front_index_.store(new_front, std::memory_order_release);
            }

        private:
            uint32_t back_index_() const noexcept {
                const uint32_t fi = front_index_.load(std::memory_order_relaxed);
                return 1u - fi;
            }

            alignas(64) mutable std::atomic<uint32_t> front_index_{0};
            T buffers_[2]{};
    };
}