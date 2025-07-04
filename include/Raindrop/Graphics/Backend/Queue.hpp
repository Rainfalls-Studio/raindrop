#pragma once

#include "Raindrop/Core/Utils/Flags.hpp"
#include "API.hpp"
#include "CommandList.hpp"
#include "PipelineStage.hpp"
#include "Semaphore.hpp"
#include "Fence.hpp"
#include <memory>
#include <vector>

namespace Raindrop::Graphics::Backend{
    class Queue{
        public:
            struct Submission{
                std::vector<std::shared_ptr<CommandList>> commands;
                std::vector<std::shared_ptr<Semaphore>> waitSemaphores;
                std::vector<PipelineStage> waitStage;
                std::vector<std::shared_ptr<Semaphore>> signalSemaphores;
            };

            virtual ~Queue() = default;

            virtual void waitIdle() const = 0;
            virtual void submit(const std::vector<Submission>& submissions, std::shared_ptr<Fence> fence = nullptr) = 0;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}