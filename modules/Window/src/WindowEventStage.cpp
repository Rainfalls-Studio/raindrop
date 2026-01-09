#include "Window/WindowEventStage.hpp"
#include "Window/WindowModule.hpp"
#include <Raindrop/Engine.hpp>

namespace Window{
    void EventStage::initialize(Scheduler::StageInitHelper& helper){
        auto& modules = helper.engine().getModuleManager();
        _windowModule = modules.getModuleAs<WindowModule>("Window");
    }

    const char* EventStage::name() const{
        return "Event";
    }

    Scheduler::StageResult EventStage::execute(){
        if (auto lock = _windowModule.lock()){
            lock->event();
        }
        return Scheduler::StageResult::Continue();
    }

}