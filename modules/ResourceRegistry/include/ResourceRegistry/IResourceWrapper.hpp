#pragma once

#include <typeindex>

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
}