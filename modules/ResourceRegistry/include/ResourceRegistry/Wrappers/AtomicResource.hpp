#pragma once

#include <atomic>

namespace ResourceRegistry{
    template<typename T>
    class AtomicResource {
        public:
            static_assert(std::is_trivially_copyable_v<T>,
                            "AtomicResource<T> requires T to be trivially copyable.");

            // Some platforms don't support atomic for large structs (not lock-free).
            // std::atomic<T> still works (may use a lock internally).
            using ValueType = T;

            AtomicResource() requires std::is_default_constructible_v<T> = default;

            explicit AtomicResource(T initial) : value_(initial) {}

            T load(std::memory_order order = std::memory_order_acquire) const noexcept {
                return value_.load(order);
            }

            void store(T v, std::memory_order order = std::memory_order_release) noexcept {
                value_.store(v, order);
            }

            T exchange(T v, std::memory_order order = std::memory_order_acq_rel) noexcept {
                return value_.exchange(v, order);
            }

            // CAS (compare-exchange) – useful for flags/counters/lock-free patterns.
            bool compare_exchange(T& expected, T desired,
                                    std::memory_order success = std::memory_order_acq_rel,
                                    std::memory_order failure = std::memory_order_acquire) noexcept {
                return value_.compare_exchange_strong(expected, desired, success, failure);
            }

        private:
            std::atomic<T> value_{};
    };
}