#include "Raindrop/Core/Scheduler/StageInitHelper.hpp"
#include "Raindrop/Core/Scheduler/Loop.hpp"


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

    void StageInitHelper::registerHook(const Hook& hook){
        _loopData->hooks.push_back(hook);
    }
}