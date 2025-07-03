#pragma once

#include "../QueryPool.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class QueryPool : public Backend::QueryPool{
        public:
            QueryPool(Context& context, const Description& description);
            virtual ~QueryPool() override;

            VkQueryPool get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkQueryPool _queryPool;
    };
}