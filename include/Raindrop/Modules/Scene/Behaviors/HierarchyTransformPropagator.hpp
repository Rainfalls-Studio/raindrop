#pragma once

#include "Raindrop/Modules/Scene/IBehavior.hpp"

namespace Raindrop::Behaviors{
    class HierarchyTransformPropagator : public Scene::IBehavior{
        public:
            virtual ~HierarchyTransformPropagator() override = default;

            virtual void initialize(Engine& engine, Scene::Scene& scene) override;
            void preUpdate();
        
        private:
            Scene::Scene* _scene;
            Scheduler::Subscription _preUpdateSubscription;

    };
}