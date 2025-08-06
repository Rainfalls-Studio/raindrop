#include "Raindrop/Modules/Scene/Behaviors/HierarchyTransformPropagator.hpp"
#include "Raindrop/Modules/Scene/Components/Hierarchy.hpp"
#include "Raindrop/Modules/Scene/Components/Transform.hpp"
#include <queue>
#include <unordered_set>

namespace Raindrop::Behaviors{
    void HierarchyTransformPropagator::initialize(Engine& engine, Scene::Scene& scene){
        _scene = &scene;
        _preUpdateSubscription = engine.getScheduler().subscribe([this]{preUpdate();}, Scheduler::Priority::PRE_UPDATE);
    }

    void HierarchyTransformPropagator::preUpdate(){
        auto view = _scene->view<Components::Hierarchy, Components::Transform>();

        std::queue<Scene::EntityHandle> toProcess;
        std::unordered_set<Scene::EntityHandle> processed;

        // Find all transform roots
        for (auto [handle, hierarchy, transform] : view.each()){
            if (transform.dirty){
                toProcess.push(handle);
            }
        }

        while (!toProcess.empty()){
            Scene::EntityHandle current = toProcess.front();
            toProcess.pop();

            if (processed.count(current)) continue;

            auto [hierachy, transform] = view.get(current);

            
            bool parentProcessed = true;
            Scene::EntityHandle parent = hierachy.parent;

            // IF the entity has a parent. We must wait till the parent is processed
            if (parent != Scene::INVALID_ENTITY_HANDLE){

                // If the parent doesn't have a transform or hierarchy component. We skip it as it will never be processed
                if (view.contains(parent)){
                    parentProcessed = processed.count(hierachy.parent) > 0;
                } else {
                    // TODO: warning The parent doesn't have a transform or hierarchy component
                }
            }

            // cannot process the entity if its parent is not processed
            if (!parentProcessed){
                toProcess.push(hierachy.parent);
                continue;
            }

            if (parent != Scene::INVALID_ENTITY_HANDLE && view.contains(parent)){
                // We don't both warning as it was already done on the parent process check

                auto& parentTransform = view.get<Components::Transform>(parent);
                transform.worldTransform = parentTransform.worldTransform * transform.localTransform;
            } else {
                transform.worldTransform = transform.localTransform;
            }

            processed.insert(current);

            for (auto& child : hierachy.children){
                if (view.contains(child)){
                    toProcess.push(child);
                } else {
                    // TODO: warning The child doesn't have a transform or hierarchy component
                }
            }
        }
    }
}