#include "PlanetUpdateBehavior.hpp"
#include "PlanetComponent.hpp"

using Raindrop::Components::Transform;
using Raindrop::Scene::Scene;

namespace Planet{
    UpdateBehavior::UpdateBehavior(){

    }

    void UpdateBehavior::initialize(Raindrop::Engine& engine, Scene& scene){
        _scene = &scene;
    }

    void UpdateBehavior::shutdown(){

    }

    void UpdateBehavior::execute(){
        auto view = _scene->registry().view<PlanetComponent, Transform>();

        for (auto e : view){
            auto [planet, transform] = view.get(e);

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

            // travers and construct LOD
            
        }
    }
}