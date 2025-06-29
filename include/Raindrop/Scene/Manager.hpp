#pragma once

#include "Raindrop/Core/Utils/BehaviorRegistry.hpp"
#include "Raindrop/Core/Utils/Registry.hpp"
#include <cstdint>
#include <memory>
#include "Scene.hpp"
#include "../Engine.hpp"

namespace Raindrop::Scene{
    class Manager : public Core::Utils::BehaviorRegistry<uint32_t, std::shared_ptr<Scene>>{
        using Super = BehaviorRegistry<uint32_t, std::shared_ptr<Scene>>;
        public:
            Manager(Engine& engine);


            std::pair<ID, std::shared_ptr<Scene>> createScene();

        private:
            Engine& _engine;
            
    };

    using SceneID = Manager::ID;
    static constexpr SceneID INVALID_SCENE_ID = Manager::INVALID_ID;
}