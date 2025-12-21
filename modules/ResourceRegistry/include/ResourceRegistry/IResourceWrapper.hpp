#pragma once

#include <typeindex>
#include <utility>

#include "detail.hpp"

namespace ResourceRegistry{
    class IResourceWrapper{
        public:
            IResourceWrapper() = default;
            virtual ~IResourceWrapper() = default;

            virtual std::type_index wrapperType() const noexcept = 0;
            virtual std::type_index valueType() const noexcept = 0;

            virtual void* rawPtr() noexcept = 0;
            virtual const void* rawPtr() const noexcept = 0;

    };

    // Used to create wrappers without needing to make them depend of IResourceWrapper
    template<typename Wrapper>
    class ResourceWrapperModel final : IResourceWrapper {
        public:
            using WrapperType = Wrapper;


            template<class... Args>
            explicit ResourceWrapperModel(Args&&... args) : _wrapper(std::forward<Args>(args)...) {}

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