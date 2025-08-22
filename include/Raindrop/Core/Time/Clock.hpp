#pragma once

#include <chrono>

namespace Raindrop::Time{
    using clock = std::chrono::steady_clock;

    using nanoseconds  = std::chrono::nanoseconds;
    using microseconds = std::chrono::microseconds;
    using milliseconds = std::chrono::milliseconds;
    using seconds      = std::chrono::seconds;
    using minutes      = std::chrono::minutes;
    using hours        = std::chrono::hours;
    
    class Duration {
        public:
            using rep   = int64_t;
            using ns    = nanoseconds;

            constexpr Duration() noexcept : value_(0) {}
            constexpr explicit Duration(ns v) : value_(v.count()) {}

            template<typename Rep, typename Period>
            constexpr Duration(std::chrono::duration<Rep, Period> d)
                : value_(std::chrono::duration_cast<ns>(d).count()) {}

            // Conversion to std::chrono::duration
            template<typename DurationT>
            constexpr DurationT as() const {
                return std::chrono::duration_cast<DurationT>(ns(value_));
            }

            // Count in seconds as double
            constexpr double seconds() const {
                return std::chrono::duration<double>(ns(value_)).count();
            }

            constexpr inline static Duration zero() noexcept {
                return Duration();
            }

            constexpr inline static Duration max() noexcept{
                return Duration(std::numeric_limits<rep>::max());
            }

            constexpr inline static Duration min() noexcept{
                return Duration(std::numeric_limits<rep>::min());
            }

            // Operators
            constexpr Duration operator+(Duration other) const { return Duration(ns(value_ + other.value_)); }
            constexpr Duration operator-(Duration other) const { return Duration(ns(value_ - other.value_)); }
            constexpr Duration operator*(double scale) const { return Duration(ns(static_cast<rep>(static_cast<double>(value_) * scale))); }
            constexpr Duration operator/(double scale) const { return Duration(ns(static_cast<rep>(static_cast<double>(value_) / scale))); }

            constexpr bool operator<(Duration other) const { return value_ < other.value_; }
            constexpr bool operator<=(Duration other) const { return value_ <= other.value_; }
            constexpr bool operator>(Duration other) const { return value_ > other.value_; }
            constexpr bool operator>=(Duration other) const { return value_ >= other.value_; }
            constexpr bool operator==(Duration other) const { return value_ == other.value_; }
            constexpr bool operator!=(Duration other) const { return value_ != other.value_; }

        private:
            inline constexpr Duration(rep value) noexcept : value_(value){}

            rep value_;
    };

    class TimePoint{
        public:
            using rep = int64_t;
            using ns = nanoseconds;

            constexpr TimePoint() : _value(0){}
            constexpr explicit TimePoint(ns value) : _value(value.count()) {}

            inline Duration operator-(const TimePoint& other) const {
                return Duration(ns(_value - other._value));
            }
            

        private:
            rep _value;

    };

    inline static TimePoint now(){
        return TimePoint(clock::now().time_since_epoch());
    }
    
    class Clock {
        public:
            static Duration now() {
                return Duration(clock::now().time_since_epoch());
            }
    };

    namespace literals{
        // Literals
        constexpr Duration operator""_ns(unsigned long long v) { return Duration(nanoseconds(v)); }
        constexpr Duration operator""_ns(long double v)         { return Duration(std::chrono::duration<long double, std::nano>(v)); }

        constexpr Duration operator""_us(unsigned long long v) { return Duration(microseconds(v)); }
        constexpr Duration operator""_us(long double v)         { return Duration(std::chrono::duration<long double, std::micro>(v)); }

        constexpr Duration operator""_ms(unsigned long long v) { return Duration(milliseconds(v)); }
        constexpr Duration operator""_ms(long double v)         { return Duration(std::chrono::duration<long double, std::milli>(v)); }

        constexpr Duration operator""_s(unsigned long long v)  { return Duration(seconds(v)); }
        constexpr Duration operator""_s(long double v)         { return Duration(std::chrono::duration<long double>(v)); }
        
        // Frequency literal: 60_Hz -> period of 1/60s
        constexpr Duration operator""_Hz(unsigned long long hz) {
            return Duration(std::chrono::duration<double>(1.0 / static_cast<double>(hz)));
        }
        constexpr Duration operator""_Hz(long double hz) {
            return Duration(std::chrono::duration<double>(1.0l / hz));
        }
    }
}