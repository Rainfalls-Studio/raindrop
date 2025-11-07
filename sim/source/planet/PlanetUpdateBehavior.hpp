#pragma once

#include <Raindrop/Raindrop.hpp>

namespace Planet{
    class UpdateBehavior : public Raindrop::Scene::IBehavior{
        public:
            UpdateBehavior();

            virtual void initialize(Raindrop::Engine& engine, Raindrop::Scene::Scene& scene) override;
            virtual void shutdown() override;

            virtual void execute() override;

        private:
            Raindrop::Scene::Scene* _scene;
        
    };
}