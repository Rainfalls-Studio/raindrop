#include "Event/EventModule.hpp"
#include <Layers/LayerModule.hpp>
#include <Layers/LayerManager.hpp>
#include <Raindrop/Modules/InitHelper.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

extern "C" RAINDROP_EXPORT Raindrop::IModule* CreateModule(){
	return new Event::EventModule();
}

extern "C" RAINDROP_EXPORT void DestroyModule(Raindrop::IModule* module){
	delete module;
}

namespace Event{
    EventModule::EventModule(){}

    Raindrop::Result EventModule::initialize(Raindrop::InitHelper& init){
        _layers = init.getDependencyAs<Layers::LayerModule>("Layers");
        _logger = init.logger();
        return Raindrop::Result::Success();
    }

    void EventModule::shutdown(){
        
    }

    void EventModule::poll(){
        std::lock_guard lk(_mtx);

        auto layers = _layers.lock();
        if (!layers) return;

        while (!_events.empty()){
            auto event = std::move(_events.front());
            _events.pop_front();
            
            layers->manager()->get()->transmit(*event);
        }
        _events.clear();
    }

    void EventModule::pushEvent(std::unique_ptr<Event>&& event){
        std::lock_guard lk(_mtx);
        _events.push_back(std::move(event));
    }

    // ------------- Poll stage ----------------------------------------------------------

    const char* EventModule::PollStage::name() const{
        return "Event - Pool Stage";
    }

    void EventModule::PollStage::initialize(Scheduler::StageInitHelper& helper){
        _event = helper.modules().getModuleAs<EventModule>("Event");
    }

    Scheduler::StageResult EventModule::PollStage::execute(){
        auto event = _event.lock();
        if (!event) return Scheduler::StageResult::Skip("No event module");

        event->poll();
        return Scheduler::StageResult::Continue();
    }
}