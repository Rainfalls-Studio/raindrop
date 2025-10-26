#include "Raindrop/Modules/Window/WindowEventStage.hpp"
#include "Raindrop/Engine.hpp"
#include "Raindrop/Modules/Window/WindowModule.hpp"

namespace Raindrop::Window{
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