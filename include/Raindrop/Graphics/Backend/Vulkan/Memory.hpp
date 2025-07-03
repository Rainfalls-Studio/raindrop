#pragma once

#include <vulkan/vulkan.h>
#include "Context.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/translation.hpp"
namespace Raindrop::Graphics::Backend::Vulkan{
    enum class MemoryProperty{
        GPU_ONLY,
        CPU_TO_GPU,
        GPU_TO_CPU,
        CPU_ONLY
    };

    class Memory{
        public:
            Memory(Context& context);

        private:
            Context& _context;
            VkDeviceMemory _memory;
    };
}