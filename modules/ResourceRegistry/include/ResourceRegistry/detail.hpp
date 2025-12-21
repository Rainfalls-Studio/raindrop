#pragma once

#include <string>
#include <typeindex>

namespace ResourceRegistry::detail{

    template<typename Wrapper>
    concept HasValueType = requires { typename Wrapper::ValueType; };

    template <class Wrapper>
    static std::type_index wrapperTypeIndex() noexcept {
        return std::type_index(typeid(Wrapper));
    }

    template <class Wrapper>
    static std::type_index valueTypeIndex() noexcept {
        if constexpr (HasValueType<Wrapper>) {
            return std::type_index(typeid(typename Wrapper::value_type));
        } else {
            return std::type_index(typeid(void));
        }
    }

    inline std::string type_name(std::type_index ti) {
      // Minimal: compiler-specific demangling omitted.
      return ti.name();
    }
}