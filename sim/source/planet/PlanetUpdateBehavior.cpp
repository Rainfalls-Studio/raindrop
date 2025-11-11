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


            RenderDataPayload* instance;
            frameSnapshot->writeSlot(_planetSlot, instance);

            instance->transform = transform.worldTransform;
            instance->radius = planet.radius;
            instance->id = planet.id;
            
            instance->chunkCount = 0;
            

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
}