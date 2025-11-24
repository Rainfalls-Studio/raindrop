#include "PlanetUpdateBehavior.hpp"
#include "PlanetComponent.hpp"

using Raindrop::Components::Transform;
using Raindrop::Scene::Scene;
using Raindrop::Behaviors::FrameSnapshotService;

namespace Planet{
    UpdateBehavior::UpdateBehavior(){

    }

    void UpdateBehavior::initialize(Raindrop::Engine& engine, Scene& scene){
        _scene = &scene;
        _snapshotService = scene.getBehaviorIndex<FrameSnapshotService>();
        _planetService = scene.getBehaviorIndex<ServiceBehavior>();

        if (_snapshotService != FrameSnapshotService::INVALID_SLOT_ID){
            auto snapshotService = scene.getBehavior<FrameSnapshotService>(_snapshotService);
            _planetSlot = snapshotService->registerSlot<RenderDataPayload>();
        }
    }

    void UpdateBehavior::shutdown(){}

    void UpdateBehavior::execute(){
        auto frameSnapshot = _scene->getBehavior<FrameSnapshotService>(_snapshotService);
        if (!frameSnapshot) return;

        auto service = _scene->getBehavior<ServiceBehavior>(_planetService);
        if (!service) return;

        if (_planetSlot == FrameSnapshotService::INVALID_SLOT_ID) return;


        auto view = _scene->registry().view<PlanetComponent, Transform>();

        for (auto e : view){
            auto [planet, transform] = view.get(e);

            if (planet.id == INVALID_PLANET_ID){
                planet.id = service->createPlanet();
            }


            auto instance = frameSnapshot->writeSlot<RenderDataPayload>(_planetSlot);

            instance->transform = transform.worldTransform;
            instance->radius = planet.radius;
            instance->id = planet.id;
            

            auto chunks = frameSnapshot->write<ChunkDataPayload>(6);

            instance->chunks = chunks;
            instance->chunkCount = 6;

            for (uint32_t i=0; i<6; i++){
                auto chunk = chunks[i];

                switch (i){
                    case 0:{
                        chunk->face = Face::NX;
                        chunk->center = {-planet.radius, 0.f, 0.f};
                        break;
                    }
                    case 1:{
                        chunk->face = Face::PX;
                        chunk->center = {planet.radius, 0.f, 0.f};
                        break;
                    }
                    case 2:{
                        chunk->face = Face::NY;
                        chunk->center = {0.f, -planet.radius, 0.f};
                        break;
                    }
                    case 3:{
                        chunk->face = Face::PY;
                        chunk->center = {0.f, planet.radius, 0.f};
                        break;
                    }
                    case 4:{
                        chunk->face = Face::NZ;
                        chunk->center = {0.f, 0.f, -planet.radius};
                        break;
                    }
                    case 5:{
                        chunk->face = Face::PZ;
                        chunk->center = {0.f, 0.f, planet.radius};
                        break;
                    }
                }

                chunk->instanceIndex = i;
                chunk->uvMin = glm::vec2(0.f);
                chunk->uvScale = 1.f;

                chunk->neighorLOD[0] = 0;
                chunk->neighorLOD[1] = 0;
                chunk->neighorLOD[2] = 0;
                chunk->neighorLOD[3] = 0;
            }

            glm::vec3 lodAnchorOrigin = glm::vec3(0.f);
            {
                auto anchor = planet.lodAnchor;

                if (anchor == Raindrop::Scene::INVALID_ENTITY_HANDLE)
                    continue;

                if (!_scene->hasAllComponents<Transform>(anchor)){
                    continue;
                }

                lodAnchorOrigin = _scene->getComponent<Transform>(anchor).translation;
            }

            if (planet.dirty){
                // reconstruct lod tree
                planet.dirty = false;
            }

            
        }
    }

    const char* UpdateBehavior::name() const{
        return "Planet - Update";
    }
}