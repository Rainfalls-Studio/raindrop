#pragma once

#include <Raindrop/Modules/IModule.hpp>
#include "Raindrop/Engine.hpp"
#include "Scene.hpp"

namespace Raindrop::Scene{
    class SceneTrait;
    class SceneModule : public IModule{
        public:
            SceneModule() = default;
            virtual ~SceneModule() override = default;

            virtual Result initialize(InitHelper& helper) override;
        
        private:
            Engine* _engine;
    };
}