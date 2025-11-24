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
        
        _planets[id].valid = true;
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

    PlanetRenderData ServiceBehavior::constructPlanetRenderData(){
        return PlanetRenderData{_renderCore, PlanetRenderData::Config{5, 5, 16}};
    }

    const char* ServiceBehavior::name() const{
        return "Planet - Service";
    }


    // ----------------------------------------------------------------------------------
    //                                  Render Data
    // ----------------------------------------------------------------------------------



    PlanetRenderData::PlanetRenderData(std::shared_ptr<Raindrop::Render::RenderCoreModule> renderCore, const Config& conf) : _renderCore(renderCore){
        createVertexBuffer(conf);
        createIndexBuffer(conf);
        createInstanceBuffer(conf);
    }

    PlanetRenderData::~PlanetRenderData(){
        if (!_renderCore) return;
        auto allocator = _renderCore->allocator();

        if (grid.index.buffer){
            vmaDestroyBuffer(
                allocator,
                grid.index.buffer,
                grid.index.allocation
            );
        }

        if (grid.vertex.buffer){
            vmaDestroyBuffer(
                allocator,
                grid.vertex.buffer,
                grid.vertex.allocation
            );
        }

        if (instances.buffer){
            vmaUnmapMemory(
                allocator,
                instances.allocation
            );
            
            vmaDestroyBuffer(
                allocator,
                instances.buffer,
                instances.allocation
            );
        }
    }


    void PlanetRenderData::createVertexBuffer(const Config& config){
        std::vector<Vertex> vertices;

        const uint32_t xcells = config.xcells;
        const uint32_t ycells = config.ycells;

        vertices.reserve((xcells + 1) * (ycells + 1));
        
        // generating the geometry
        for (uint32_t y = 0; y <= ycells; ++y) {
            for (uint32_t x = 0; x <= xcells; ++x) {
                Vertex v{
                    .position = glm::vec3(
                        static_cast<float>(x) * (1.f / float(xcells)) - 0.5f,
                        static_cast<float>(y) * (1.f / float(xcells)) - 0.5f,
                        0.0f
                    ),

                    .uv = glm::vec2(
                        static_cast<float>(x) / static_cast<float>(xcells),
                        static_cast<float>(y) / static_cast<float>(ycells)
                    )
                };

                vertices.push_back(v);
            }
        }

        // buffer creation
        auto allocator = _renderCore->allocator();
        size_t size = vertices.size() * sizeof(Vertex);

        vk::BufferCreateInfo info(
            {},
            static_cast<vk::DeviceSize>(size),
            vk::BufferUsageFlagBits::eVertexBuffer,
            vk::SharingMode::eExclusive,
            {}, {}
        );

        VmaAllocationCreateInfo allocationInfo{
            {},
            VMA_MEMORY_USAGE_CPU_TO_GPU, // host visible buffer. I don't want to make staging system right now
            0,
            0,
            0,
            nullptr,
            nullptr,
            0.1f
        };

        VkBuffer buffer;
        auto& allocation = grid.vertex.allocation;
        
        vmaCreateBuffer(
            allocator,
            reinterpret_cast<VkBufferCreateInfo*>(&info),
            &allocationInfo,
            &buffer,
            &allocation,
            nullptr
        );

        grid.vertex.buffer = buffer;

        // submit the data
        void* data;

        vmaMapMemory(
            allocator,
            allocation,
            &data
        );

        std::memcpy(data, vertices.data(), size);

        vmaUnmapMemory(
            allocator,
            allocation
        );

        vmaFlushAllocation(
            allocator,
            allocation,
            0,
            size
        );

    }

    void PlanetRenderData::createIndexBuffer(const Config& config){
        std::vector<uint32_t> indices;

        const uint32_t xcells = config.xcells;
        const uint32_t ycells = config.ycells;

        indices.reserve(xcells * ycells * 6);

        for (uint32_t y = 0; y < ycells; ++y) {
            for (uint32_t x = 0; x < xcells; ++x) {
                uint32_t i0 = y * (xcells + 1) + x;
                uint32_t i1 = i0 + 1;
                uint32_t i2 = i0 + (xcells + 1);
                uint32_t i3 = i2 + 1;

                // triangle 1
                indices.push_back(i0);
                indices.push_back(i2);
                indices.push_back(i1);

                // triangle 2
                indices.push_back(i1);
                indices.push_back(i2);
                indices.push_back(i3);
            }
        }

        auto allocator = _renderCore->allocator();

        size_t size = indices.size() * sizeof(uint32_t);
        grid.index.count = static_cast<uint32_t>(indices.size());

        vk::BufferCreateInfo info(
            {},
            static_cast<vk::DeviceSize>(size),
            vk::BufferUsageFlagBits::eIndexBuffer,
            vk::SharingMode::eExclusive,
            {}, {}
        );


        VmaAllocationCreateInfo allocationInfo{
            {},
            VMA_MEMORY_USAGE_CPU_TO_GPU, // host visible buffer. I don't want to make staging system right now
            0,
            0,
            0,
            nullptr,
            nullptr,
            0.1f
        };

        VkBuffer buffer;
        auto& allocation = grid.index.allocation;
        
        vmaCreateBuffer(
            allocator,
            reinterpret_cast<VkBufferCreateInfo*>(&info),
            &allocationInfo,
            &buffer,
            &allocation,
            nullptr
        );

        grid.index.buffer = buffer;

        void* data;

        vmaMapMemory(
            allocator,
            allocation,
            &data
        );

        std::memcpy(data, indices.data(), size);

        vmaUnmapMemory(
            allocator,
            allocation
        );

        vmaFlushAllocation(
            allocator,
            allocation,
            0,
            size
        );
    }

    void PlanetRenderData::createInstanceBuffer(const Config& config){
        
        auto allocator = _renderCore->allocator();

        instances.maxCount = 6 * (1 << (2*config.maxLod));

        vk::BufferCreateInfo info(
            {},
            static_cast<vk::DeviceSize>(sizeof(Instance) * instances.maxCount),
            vk::BufferUsageFlagBits::eVertexBuffer,
            vk::SharingMode::eExclusive,
            {}, {}
        );


        VmaAllocationCreateInfo allocationInfo{
            {},
            VMA_MEMORY_USAGE_CPU_TO_GPU, // host visible buffer
            0,
            0,
            0,
            nullptr,
            nullptr,
            0.1f
        };

        VkBuffer buffer;
        auto& allocation = instances.allocation;
        
        vmaCreateBuffer(
            allocator,
            reinterpret_cast<VkBufferCreateInfo*>(&info),
            &allocationInfo,
            &buffer,
            &allocation,
            nullptr
        );

        instances.buffer = buffer;

        vmaMapMemory(
            allocator,
            allocation,
            &instances.mapped
        );
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

    std::vector<vk::VertexInputAttributeDescription> PlanetRenderData::Vertex::attributes(uint32_t binding){
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


    std::vector<vk::VertexInputAttributeDescription> PlanetRenderData::Instance::attributes(uint32_t startLocation, uint32_t binding){
        return {
            vk::VertexInputAttributeDescription{
                startLocation + 0,
                binding,
                vk::Format::eR32Uint,
                offsetof(Instance, face)
            },
            vk::VertexInputAttributeDescription{
                startLocation + 1,
                binding,
                vk::Format::eR32G32Sfloat,
                offsetof(Instance, offsetUV)
            },
            vk::VertexInputAttributeDescription{
                startLocation + 2,
                binding,
                vk::Format::eR32Sfloat,
                offsetof(Instance, sizeUV)
            },vk::VertexInputAttributeDescription{
                startLocation + 3,
                binding,
                vk::Format::eR32Uint,
                offsetof(Instance, packedNeighbors)
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