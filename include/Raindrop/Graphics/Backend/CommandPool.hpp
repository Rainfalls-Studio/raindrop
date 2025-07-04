#pragma once

#include "API.hpp"
#include "QueueFamily.hpp"
#include "Raindrop/Core/Utils/Flags.hpp"
#include "Raindrop/Graphics/Backend/CommandList.hpp"

namespace Raindrop::Graphics::Backend{
    class CommandPool{
        public:
            enum class FlagBits{
                NONE = 0,
                TRANSIENT = 1 << 0,
                RESET = 1 << 1,
                __SIZE__
            };

            RAINDROP_FLAG_CLASS(Flags, FlagBits)

            struct AllocationInfo{
                uint32_t count;
            };

            struct Description{
                std::shared_ptr<QueueFamily> family;
                Flags flags;
            };

            virtual ~CommandPool() = default;

            virtual void reset() const = 0;
            virtual void trim() const = 0;

            virtual void free(const std::vector<std::shared_ptr<CommandList>>& commands) = 0;
            virtual std::vector<std::shared_ptr<CommandList>> allocate(uint32_t count, CommandList::Level level) const = 0;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}