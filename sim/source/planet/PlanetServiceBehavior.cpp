#include "PlanetServiceBehavior.hpp"

namespace Planet{
    ServiceBehavior::ServiceBehavior(){

    }

    ServiceBehavior::~ServiceBehavior(){

    }

    void ServiceBehavior::initialize(Raindrop::Engine& engine, Raindrop::Scene::Scene& scene){
        _renderCore = engine.getModuleManager().getModuleAs<Raindrop::Render::RenderCoreModule>("RenderCore");
    }

    void ServiceBehavior::shutdown(){
        _planets.clear();
    }

    PlanetID ServiceBehavior::createPlanet(){
        PlanetID id = INVALID_PLANET_ID;

        if (_freeIDs.empty()){
            id = static_cast<PlanetID>(_planets.size());
            _planets.push_back({});
        } else {
            id = _freeIDs.front();
            _freeIDs.pop_front();
        }

        return id;
    }

    RuntimePlanet* ServiceBehavior::getRuntimeData(PlanetID id){
        auto& data = _planets[id];
        return data.valid ? &data : nullptr;
    }

    void ServiceBehavior::destroyPlanet(PlanetID id){
        auto& data = _planets[id];
        data.valid = false;
        _freeIDs.push_back(id);
    }



    // ----------------------------------------------------------------------------------
    //                                  Render Data
    // ----------------------------------------------------------------------------------



    PlanetRenderData::PlanetRenderData(std::shared_ptr<Raindrop::Render::RenderCoreModule> renderCore, const Config& conf) : _renderCore(renderCore){

    }

    PlanetRenderData::~PlanetRenderData(){

    }

    std::pair<vk::Buffer, VmaAllocation> PlanetRenderData::createStaginBuffer(size_t size){
        auto allocator = _renderCore->allocator();

        vk::BufferCreateInfo bufferInfo{
            {},
            static_cast<vk::DeviceSize>(size),
            vk::BufferUsageFlagBits::eTransferSrc,
            vk::SharingMode::eExclusive
        };

        VmaAllocationCreateInfo allocationInfo{
            {},
            VMA_MEMORY_USAGE_CPU_TO_GPU,
            {},
            {},
            {},
            {},
            {},
            1.f
        };

        VkBuffer buffer;
        VmaAllocation allocation;

        if (VkResult result = vmaCreateBuffer(
            allocator,
            reinterpret_cast<VkBufferCreateInfo*>(&bufferInfo),
            &allocationInfo,
            &buffer,
            &allocation,
            nullptr
        ); result != VK_SUCCESS){
            throw std::runtime_error("Failed to allocate planet stagin buffer : " + vk::to_string(vk::Result(result)));
        }

        return std::make_pair(vk::Buffer(buffer), allocation);
    }

    vk::CommandBuffer PlanetRenderData::createSingleUseBuffer(){
        return VK_NULL_HANDLE;
    }

    void PlanetRenderData::submitSingleUseBuffer(vk::Buffer buffer [[__maybe_unused__]]){

    }

    std::array<vk::VertexInputAttributeDescription, 2> PlanetRenderData::Vertex::attributes(uint32_t binding){
        return {
            vk::VertexInputAttributeDescription{
                0,
                binding,
                vk::Format::eR32G32Sfloat,
                offsetof(Vertex, position)
            },
            vk::VertexInputAttributeDescription{
                1,
                binding,
                vk::Format::eR32G32Sfloat,
                offsetof(Vertex, uv)
            }
        };
    }

    vk::VertexInputBindingDescription PlanetRenderData::Vertex::description(uint32_t binding){
        return vk::VertexInputBindingDescription{
            binding,
            sizeof(Vertex),
            vk::VertexInputRate::eVertex
        };
    }


    std::array<vk::VertexInputAttributeDescription, 3> PlanetRenderData::Instance::attributes(uint32_t binding){
        return {
            vk::VertexInputAttributeDescription{
                0,
                binding,
                vk::Format::eR32Uint,
                offsetof(Instance, face)
            },
            vk::VertexInputAttributeDescription{
                1,
                binding,
                vk::Format::eR32G32Sfloat,
                offsetof(Instance, offsetUV)
            },
            vk::VertexInputAttributeDescription{
                2,
                binding,
                vk::Format::eR32Sfloat,
                offsetof(Instance, sizeUV)
            }
        };
    }

    vk::VertexInputBindingDescription PlanetRenderData::Instance::description(uint32_t binding){
        return vk::VertexInputBindingDescription{
            binding,
            sizeof(Instance),
            vk::VertexInputRate::eInstance
        };
    }
}