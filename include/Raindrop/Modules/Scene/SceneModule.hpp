#pragma once

#include "Raindrop/Core/Modules/IModule.hpp"
#include "Raindrop/Engine.hpp"
#include "Scene.hpp"

namespace Raindrop::Scene{
    class SceneTrait;
    class SceneModule : public Modules::IModule{
        public:
            SceneModule() = default;
            virtual ~SceneModule() override = default;

            virtual Modules::Result initialize(Modules::InitHelper& helper) override;
            virtual Name name() const noexcept override;

            SceneTrait& emplaceTrait(Layers::Layer layer);
        
        private:
            Engine* _engine;
    };
}