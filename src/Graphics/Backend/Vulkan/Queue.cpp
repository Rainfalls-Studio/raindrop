#include "Raindrop/Graphics/Backend/Vulkan/Queue.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/CommandList.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/Semaphore.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/CommandList.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/PipelineStage.hpp"
#include "Raindrop/Graphics/Backend/API.hpp"
#include "vulkan/vulkan_core.h"
#include <list>
#include <memory>

namespace Raindrop::Graphics::Backend::Vulkan{
    
    Queue::Queue(Context& context, VkQueue queue) :
        _context{context},
        _queue{queue}
    {}

    Queue::~Queue(){}

    VkQueue Queue::get() const noexcept{
        return _queue;
    }

    void* Queue::getHandle() const noexcept{
        return static_cast<void*>(_queue);
    }

    API Queue::getAPI() const noexcept{
        return API::VULKAN;
    }

    void Queue::waitIdle() const{
        vkQueueWaitIdle(_queue);
    }

    void Queue::submit(const std::vector<Submission>& submissions, std::shared_ptr<Fence> fence){
        assert(!submissions.empty() && "Submissions must not be empty");
        assert(fence ? fence->getAPI() == API::VULKAN : true && "Fence must be a Vulkan fence");
        
        std::vector<VkSubmitInfo> vkSubmitInfos;
        std::vector<std::vector<VkCommandBuffer>> commandBuffersStorage;
        std::vector<std::vector<VkSemaphore>> waitSemaphoresStorage;
        std::vector<std::vector<VkPipelineStageFlags>> waitStagesStorage;
        std::vector<std::vector<VkSemaphore>> signalSemaphoresStorage;

        vkSubmitInfos.reserve(submissions.size());
        commandBuffersStorage.reserve(submissions.size());
        waitSemaphoresStorage.reserve(submissions.size());
        waitStagesStorage.reserve(submissions.size());
        signalSemaphoresStorage.reserve(submissions.size());

        for (const auto& submission : submissions) {
            // Extract handles
            auto& cmdBufs = commandBuffersStorage.emplace_back();
            for (const auto& cmd : submission.commands) {

                assert(cmd && "Command buffer must not be null");
                assert(cmd->getAPI() == API::VULKAN && "Command buffer API must be Vulkan");

                auto vulkanCmd = std::static_pointer_cast<CommandList>(cmd);
                cmdBufs.push_back(vulkanCmd->get());
            }

            auto& waitSems = waitSemaphoresStorage.emplace_back();
            for (const auto& sem : submission.waitSemaphores) {

                assert(sem && "Semaphore must not be null");
                assert(sem->getAPI() == API::VULKAN && "Semaphore API must be Vulkan");

                auto vulkanSem = std::static_pointer_cast<Semaphore>(sem);
                waitSems.push_back(vulkanSem->get());
            }

            auto& waitStages = waitStagesStorage.emplace_back();
            for (const auto& stage : submission.waitStage) {
                waitStages.push_back(toVulkan<VkPipelineStageFlags>(stage));
            }

            auto& signalSems = signalSemaphoresStorage.emplace_back();
            for (const auto& sem : submission.signalSemaphores) {
                
                assert(sem && "Semaphore must not be null");
                assert(sem->getAPI() == API::VULKAN && "Semaphore API must be Vulkan");

                auto vulkanSem = std::static_pointer_cast<Semaphore>(sem);
                signalSems.push_back(vulkanSem->get());
            }

            vkSubmitInfos.push_back(VkSubmitInfo{
                .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .pNext = nullptr,
                .waitSemaphoreCount = static_cast<uint32_t>(waitSems.size()),
                .pWaitSemaphores = waitSems.data(),
                .pWaitDstStageMask = waitStages.data(),
                .commandBufferCount = static_cast<uint32_t>(cmdBufs.size()),
                .pCommandBuffers = cmdBufs.data(),
                .signalSemaphoreCount = static_cast<uint32_t>(signalSems.size()),
                .pSignalSemaphores = signalSems.data(),
            });
        }

        vkQueueSubmit(_queue, static_cast<uint32_t>(vkSubmitInfos.size()), vkSubmitInfos.data(), VK_NULL_HANDLE);
    }
}