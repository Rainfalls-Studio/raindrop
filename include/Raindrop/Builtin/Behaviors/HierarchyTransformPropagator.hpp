#pragma once

#include "Raindrop/Scene/IBehavior.hpp"

namespace Raindrop::Builtin::Behaviors{
    class HierarchyTransformPropagator : public Scene::IBehavior{
        public:
            virtual ~HierarchyTransformPropagator() override = default;

            virtual void initialize(Scene::Scene& scene) override;

            virtual void preUpdate() override;
        
        private:
            Scene::Scene* _scene;

    };
}