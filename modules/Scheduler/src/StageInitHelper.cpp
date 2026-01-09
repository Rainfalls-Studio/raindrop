#include "Scheduler/StageInitHelper.hpp"
#include "Scheduler/Loop.hpp"
#include <Raindrop/Engine.hpp>

namespace Scheduler{
    StageInitHelper::StageInitHelper(Raindrop::Engine& engine, const std::shared_ptr<LoopData>& data) :
        _engine{engine},
        _loopData(data)
    {}

    Raindrop::Engine& StageInitHelper::engine(){
        return _engine;
    }

    Loop StageInitHelper::loop(){
        return Loop(_loopData);
    }

    Raindrop::ModuleManager& StageInitHelper::modules() const{
        return _engine.getModuleManager();
    }
}