#include "Scheduler/StageInitHelper.hpp"
#include "Scheduler/Loop.hpp"
#include <Raindrop/Engine.hpp>

namespace Raindrop::Scheduler{
    StageInitHelper::StageInitHelper(Engine& engine, const std::shared_ptr<LoopData>& data) :
        _engine{engine},
        _loopData(data)
    {}

    Engine& StageInitHelper::engine(){
        return _engine;
    }

    Loop StageInitHelper::loop(){
        return Loop(_loopData);
    }

    Modules::Manager& StageInitHelper::modules() const{
        return _engine.getModuleManager();
    }

}