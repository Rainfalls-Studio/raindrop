#pragma once

#include "../Shader.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class Shader : public Backend::Shader{
        public:
            Shader(Context& context, const Description& description);
            virtual ~Shader() override;

            VkShaderModule get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkShaderModule _shader;
    };
}