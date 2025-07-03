#pragma once

#include "../BufferView.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class BufferView : public Backend::BufferView{
        public:
            BufferView(Context& context, const Description& description);
            virtual ~BufferView() override;

            VkBufferView get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkBufferView _BufferView;
    };
}