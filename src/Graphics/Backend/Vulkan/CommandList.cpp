#include "Raindrop/Graphics/Backend/Vulkan/CommandList.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/ComputeDomain.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/SyncDomain.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/TransferDomain.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/GraphicsDomain.hpp"
#include "vulkan/vulkan_core.h"
#include <memory>
#include <stdexcept>

namespace Raindrop::Graphics::Backend::Vulkan{
    CommandList::CommandList(Context& context, VkCommandBuffer commandBuffer) :
        _context{context},
        _commandBuffer(commandBuffer)
    {}
    
    CommandList::~CommandList(){

    }

    VkCommandBuffer CommandList::get() const noexcept{
        return _commandBuffer;
    }

    void CommandList::execute(const std::vector<std::shared_ptr<Backend::CommandList>>& commands){
        assert(commands.size() > 0 && "commands must be non-empty");

        std::vector<VkCommandBuffer> buffers(commands.size());
        for (uint32_t i=0; i<buffers.size(); i++){
            assert(commands[i] && "All commands must be valid commands");
            assert(commands[i]->getAPI() == API::VULKAN && "All commands must be from the vulkan API");

            buffers[i] = std::static_pointer_cast<CommandList>(commands[i])->get();
        }

        vkCmdExecuteCommands(
            _commandBuffer,
            static_cast<uint32_t>(buffers.size()),
            buffers.data()
        );
    }

    std::shared_ptr<CommandList::Domain> CommandList::getDomain(const std::type_index& type) const{
        if (type == typeid(Backend::GraphicsDomain)) return std::make_shared<GraphicsDomain>(_commandBuffer);
        if (type == typeid(Backend::ComputeDomain)) return std::make_shared<ComputeDomain>(_commandBuffer);
        if (type == typeid(Backend::TransferDomain)) return std::make_shared<TransferDomain>(_commandBuffer);
        if (type == typeid(Backend::SyncDomain)) return std::make_shared<SyncDomain>(_commandBuffer);

        throw std::runtime_error("Unknown domain type");
    }

    void* CommandList::getHandle() const noexcept{
        return static_cast<void*>(_commandBuffer);
    }

    API CommandList::getAPI() const noexcept{
        return API::VULKAN;
    }
}