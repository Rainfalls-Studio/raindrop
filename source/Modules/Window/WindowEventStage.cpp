#include "Raindrop/Modules/Window/WindowEventStage.hpp"
#include "Raindrop/Engine.hpp"
#include "Raindrop/Modules/Window/WindowModule.hpp"

namespace Raindrop::Window{
    void EventStage::initialize(Scheduler::StageInitHelper& helper){
        auto& modules = helper.engine().getModuleManager();
        std::weak_ptr<WindowModule> windowModule = modules.getModuleAs<WindowModule>("Window");

        if (!windowModule.expired()){
            helper.registerHook(Scheduler::Hook{
                Scheduler::Phase::EVENT,
                "window events",
                [windowModule] -> Scheduler::HookResult {
                    if (auto lock = windowModule.lock()) lock->event();
                    return Scheduler::HookResult::Continue();
                }
            });
    }
    }
}