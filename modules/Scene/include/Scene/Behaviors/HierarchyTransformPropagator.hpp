#pragma once

#include "Scene/IBehavior.hpp"

namespace Raindrop::Behaviors{
    class HierarchyTransformPropagator : public Scene::IBehavior{
        public:
            virtual ~HierarchyTransformPropagator() override = default;

            virtual void initialize(Engine& engine, Scene::Scene& scene) override;
            virtual void execute() override;

            virtual const char* name() const override;
        
        private:
            Scene::Scene* _scene;

    };
}