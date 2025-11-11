#pragma once

#include <Raindrop/Raindrop.hpp>

namespace Planet{
    using PlanetID = uint32_t;
    static constexpr PlanetID INVALID_PLANET_ID = ~static_cast<PlanetID>(0);

    struct PlanetRenderData{
        struct Vertex{
            glm::vec2 position;
            glm::vec2 uv;

            static std::array<vk::VertexInputAttributeDescription, 2> attributes(uint32_t binding=0);
            static vk::VertexInputBindingDescription description(uint32_t binding=0);
        };

        using IndexType = uint32_t;
        static constexpr vk::IndexType INDEX_TYPE = vk::IndexType::eUint32;

        struct Instance{
            uint32_t face;
            glm::vec2 offsetUV;
            float sizeUV;

            static std::array<vk::VertexInputAttributeDescription, 3> attributes(uint32_t binding=0);
            static vk::VertexInputBindingDescription description(uint32_t binding=0);
        };

        std::shared_ptr<Raindrop::Render::RenderCoreModule> _renderCore;

        struct {
            struct{
                vk::Buffer buffer;
                VmaAllocation allocation;
                uint32_t count;
            } vertex;

            struct{
                vk::Buffer buffer;
                VmaAllocation allocation;
                uint32_t count;
            } index;
        } grid;

        struct {
            vk::Buffer buffer;
            VmaAllocation allocation;
            uint32_t maxCount;
            uint32_t count;
        } instances;

        vk::DescriptorSet set;

        struct Config{
            uint32_t xcells;
            uint32_t ycells;
        };

        PlanetRenderData(std::shared_ptr<Raindrop::Render::RenderCoreModule> renderCore, const Config& conf);
        ~PlanetRenderData();

        PlanetRenderData(const PlanetRenderData&) = delete;
        PlanetRenderData& operator=(const PlanetRenderData&) = delete;

        PlanetRenderData(PlanetRenderData&&) = default;
        PlanetRenderData& operator=(PlanetRenderData&&) = default;

        std::pair<vk::Buffer, VmaAllocation> createStaginBuffer(size_t size);
        vk::CommandBuffer createSingleUseBuffer();
        void submitSingleUseBuffer(vk::Buffer buffer);

        void createVertexBuffer(const Config& config);
        void createIndexBuffer(const Config& config);
    };

    enum class Face{
        PX, NX,
        PY, NY,
        PZ, NZ
    };

    struct PlanetLODData{
        struct ChunkNode{
            uint8_t lod;
            Face face;
            glm::vec2 center;
            glm::ivec2 coord;

            ChunkNode* parent;
            ChunkNode* children[4];
        };

        ChunkNode faces[6];
    };

    struct RuntimePlanet{
        bool valid = false;
        PlanetLODData lod;
        std::optional<PlanetRenderData> render;
    };

    struct RenderDataPayload{
        Raindrop::Behaviors::Offset chunks;
        uint32_t chunkCount;
        glm::mat4 transform;
        float radius;
        PlanetID id;
    };

    class ServiceBehavior : public Raindrop::Scene::IBehavior{
        public:
            ServiceBehavior();
            ~ServiceBehavior();

            virtual void initialize(Raindrop::Engine& engine, Raindrop::Scene::Scene& scene);
            virtual void shutdown();

            PlanetID createPlanet();
            RuntimePlanet* getRuntimeData(PlanetID id);
            void destroyPlanet(PlanetID id);

            PlanetRenderData constructPlanetRenderData();

        private:
            std::shared_ptr<Raindrop::Render::RenderCoreModule> _renderCore;
            std::vector<RuntimePlanet> _planets;
            std::deque<PlanetID> _freeIDs;
    };
}