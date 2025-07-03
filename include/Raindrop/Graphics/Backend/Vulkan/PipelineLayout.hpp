#pragma once

#include "../PipelineLayout.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class PipelineLayout : public Backend::PipelineLayout{
        public:
            PipelineLayout(Context& context, const Description& description);
            virtual ~PipelineLayout() override;

            VkPipelineLayout get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkPipelineLayout _PipelineLayout;
    };
}