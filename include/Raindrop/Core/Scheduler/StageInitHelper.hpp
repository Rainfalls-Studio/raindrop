#pragma once

#include <memory>
#include "Raindrop/Core/Modules/Manager.hpp"

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Scheduler{
    class Loop;
    struct LoopData;
    struct Hook;

    class StageInitHelper{
        public:
            StageInitHelper(Engine& engine, const std::shared_ptr<LoopData>& data);

            Engine& engine();
            Loop loop();

            Modules::Manager& modules() const;

        private:
            Engine& _engine;
            const std::shared_ptr<LoopData>& _loopData;
    };
}