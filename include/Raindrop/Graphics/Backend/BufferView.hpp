#pragma once

#include <memory>
#include "Buffer.hpp"
#include "Format.hpp"
#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class BufferView{
        public:
            using Size = uint32_t;
            static constexpr Size WHOLE_SIZE = ~static_cast<Size>(0);

            struct Description{
                std::shared_ptr<Buffer> buffer;
                Format format;
                Size offset;
                Size range;
            };
            
            virtual ~BufferView() = default;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}