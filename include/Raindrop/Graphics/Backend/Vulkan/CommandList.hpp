#pragma once

#include "../CommandList.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class CommandList : public Backend::CommandList{
        public:
            CommandList(Context& context);
            virtual ~CommandList() override;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
    };
}