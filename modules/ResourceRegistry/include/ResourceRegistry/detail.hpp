#pragma once

#include <string>
#include <typeindex>
#include <utility>
#include "IResourceWrapper.hpp"

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


    // Used to create wrappers without needing to make them depend of IResourceWrapper
    template<typename Wrapper>
    class WrapperModel final : IResourceWrapper {
        public:
            using WrapperType = Wrapper;


            template<class... Args>
            explicit WrapperModel(Args&&... args) : _wrapper(std::forward<Args>(args)...) {}

            std::type_index wrapperType() const noexcept override {
                return detail::wrapperTypeIndex<Wrapper>();
            }

            std::type_index valueType() const noexcept override {
                return detail::valueTypeIndex<Wrapper>();
            }

            void* rawPtr() noexcept override { return &_wrapper; }
            const void* rawPtr() const noexcept override { return &_wrapper; }

            Wrapper& get() noexcept { return _wrapper; }
            const Wrapper& get() const noexcept { return _wrapper; }
        
        private:
            Wrapper _wrapper;
    };
}