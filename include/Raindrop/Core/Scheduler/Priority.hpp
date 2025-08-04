#pragma once

#include <limits>

namespace Raindrop::Scheduler{

    class Priority{
        public:
            enum Phase{
                TOP,
                PRE_EVENT,
                EVENT,
                POST_EVENT,
                PRE_UPDATE,
                UPDATE,
                POST_UPDATE,
                PRE_RENDER,
                RENDER,
                POST_RENDER,
                PRESENT,
                BOTTOM
            };

            inline Priority(Phase phase) noexcept{
                set(phase);
            }
            
            inline Priority(float value) noexcept{
                set(value);
            }

            Priority(const Priority&) = default;
            Priority(Priority&&) = default;

            Priority& operator=(const Priority&) = default;
            Priority& operator=(Priority&&) = default;

            inline float get() const noexcept{
                return _value;
            }

            inline void set(float value) noexcept{
                _value = value;
            }

            inline void set(Phase phase) noexcept{
                _value = static_cast<float>(phase);
            }


            constexpr inline Priority& operator=(float value) noexcept{
                set(value);
                return *this;
            }

            constexpr inline Priority& operator=(Phase phase) noexcept{
                set(phase);
                return *this;
            }

            constexpr bool operator>(const Priority& other) const noexcept{
                return _value > other._value;
            }

            constexpr bool operator<(const Priority& other) const noexcept{
                return _value < other._value;
            }

        private:
            float _value;
    };
}