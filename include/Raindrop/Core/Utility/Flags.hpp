#pragma once

#include <initializer_list>
#include <type_traits>

namespace Raindrop::Core::Utils{
    template<typename U, int init_value = 0>
    class Flags{
        static_assert(std::is_enum<U>::value, "Flags<U>: U must be an enum");

        public:
            // using enum U;
            using Bits = typename std::underlying_type<U>::type;

            Flags() noexcept = default;

            constexpr Flags(U bit) noexcept{
                _bits |= static_cast<Bits>(bit);
            }

            constexpr Flags(Bits bits) noexcept{
                _bits |= bits;
            }

            constexpr Flags(std::initializer_list<U> bits) noexcept{
                for (const U b : bits){
                    _bits |= static_cast<Bits>(b);
                }
            }

            Flags(const Flags& other) noexcept = default;

            Flags& operator=(const Flags& other) noexcept = default;

            constexpr Flags& operator=(Bits bits) noexcept{
                _bits = bits;
                return *this;
            }

            constexpr Flags& operator=(U bit) noexcept{
                _bits = static_cast<Bits>(bit);
                return *this;
            }
            
            constexpr Flags operator&(const Flags& other) const noexcept{
                return Flags(_bits & other._bits);
            }

            constexpr Flags operator^(const Flags& other) const noexcept{
                return Flags(_bits ^ other._bits);
            }

            constexpr Flags operator|(const Flags& other) const noexcept{
                return Flags(_bits | other._bits);
            }

            constexpr Flags operator&(Bits bits) const noexcept{
                return Flags(_bits & bits);
            }

            constexpr Flags operator^(Bits bits) const noexcept{
                return Flags(_bits ^ bits);
            }

            constexpr Flags operator|(Bits bits) const noexcept{
                return Flags(_bits | bits);
            }

            constexpr Flags operator&(U bit) const noexcept{
                return Flags(_bits & static_cast<Bits>(bit));
            }

            constexpr Flags operator^(U bit) const noexcept{
                return Flags(_bits ^ static_cast<Bits>(bit));
            }

            constexpr Flags operator|(U bit) const noexcept{
                return Flags(_bits | static_cast<Bits>(bit));
            }

            constexpr Flags operator~() const noexcept{
                return Flags(~_bits);
            }

            constexpr Flags operator<<(int a) const noexcept{
                return Flags(_bits << a);
            }

            constexpr Flags operator>>(int a) const noexcept{
                return Flags(_bits >> a);
            }


            constexpr Flags& operator&=(const Flags& other) noexcept{
                _bits &= other._bits;
                return *this;
            }

            constexpr Flags& operator^=(const Flags& other) noexcept{
                _bits ^= other._bits;
                return *this;
            }

            constexpr Flags& operator|=(const Flags& other) noexcept{
                _bits |= other._bits;
                return *this;
            }

            constexpr Flags& operator&=(Bits bits) noexcept{
                _bits &= bits;
                return *this;
            }

            constexpr Flags& operator^=(Bits bits) noexcept{
                _bits ^= bits;
                return *this;
            }

            constexpr Flags& operator|=(Bits bits) noexcept{
                _bits |= bits;
                return *this;
            }

            constexpr Flags& operator&=(U bit) noexcept{
                _bits &= static_cast<Bits>(bit);
                return *this;
            }

            constexpr Flags& operator^=(U bit) noexcept{
                _bits ^= static_cast<Bits>(bit);
                return *this;
            }

            constexpr Flags& operator|=(U bit) noexcept{
                _bits |= static_cast<Bits>(bit);
                return *this;
            }

            constexpr Flags& operator<<=(int a) noexcept{
                _bits <<= a;
                return *this;
            }

            constexpr Flags& operator>>=(int a) noexcept{
                _bits >>= a;
                return *this;
            }

            constexpr operator Bits() noexcept{
                return _bits;
            }
        
        private:
            Bits _bits = static_cast<Bits>(init_value);
    };
}

#define RAINDROP_FLAG_CLASS(name, enumType) \
    class name : public Raindrop::Utils::Flags<enumType>{ \
        public: \
            using Raindrop::Utils::Flags<enumType>::Flags; \
            using enum enumType; \
    };\
    \
    namespace Raindrop::Utils { \
        inline constexpr name::Bits operator|(enumType a, enumType b) noexcept { \
            return static_cast<name::Bits>(a) | static_cast<name::Bits>(b); \
        } \
        inline constexpr name::Bits operator|(name::Bits a, enumType b) noexcept { \
            return a | static_cast<name::Bits>(b); \
        } \
    }

#define RAINDROP_FLAG(name, enumType) \
    class name : public Raindrop::Utils::Flags<enumType>{ \
        public: \
            using Raindrop::Utils::Flags<enumType>::Flags; \
            using enum enumType; \
    };\
    \
    namespace Raindrop::Utils { \
        inline constexpr name::Bits operator|(enumType a, enumType b) noexcept { \
            return static_cast<name::Bits>(a) | static_cast<name::Bits>(b); \
        } \
        inline constexpr name::Bits operator|(name::Bits a, enumType b) noexcept { \
            return a | static_cast<name::Bits>(b); \
        } \
    }
