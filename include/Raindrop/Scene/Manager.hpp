#pragma once

#include "Raindrop/Core/Utils/Registry.hpp"
#include <cstdint>
#include <memory>
#include "Scene.hpp"
#include "../Engine.hpp"

namespace Raindrop::Scene{
    class Manager : public Core::Utils::Registry<uint32_t, std::shared_ptr<Scene>>{
        public:
            Manager(Engine& engine);

        private:
            Engine& _engine;
    };

    using SceneID = Manager::ID;
    static constexpr SceneID INVALID_SCENE_ID = Manager::INVALID_ID;
}