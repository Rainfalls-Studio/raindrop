#pragma once

#include <memory>
#include <Raindrop/Modules/Manager.hpp>

namespace Raindrop{
    class Engine;
}

namespace Scheduler{
    class Loop;
    struct LoopData;
    struct Hook;

    class StageInitHelper{
        public:
            StageInitHelper(Raindrop::Engine& engine, const std::shared_ptr<LoopData>& data);

            Raindrop::Engine& engine();
            Loop loop();

            Raindrop::ModuleManager& modules() const;

        private:
            Raindrop::Engine& _engine;
            const std::shared_ptr<LoopData>& _loopData;
    };
}