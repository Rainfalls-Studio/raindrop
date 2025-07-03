#pragma once

#include "../GraphicsPipeline.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class GraphicsPipeline : public Backend::GraphicsPipeline{
        public:
            GraphicsPipeline(Context& context, const Description& description);
            virtual ~GraphicsPipeline() override;

            VkPipeline get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkPipeline _GraphicsPipeline;
    };
}