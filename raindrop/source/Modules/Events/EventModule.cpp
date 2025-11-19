#include "Raindrop/Modules/Event/EventModule.hpp"
#include <Raindrop/Modules/Layers/LayerModule.hpp>
#include <Raindrop/Modules/Layers/LayerManager.hpp>
#include "Raindrop/Core/Modules/InitHelper.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Event{
    EventModule::EventModule(){}

    Modules::Result EventModule::initialize(Modules::InitHelper& init){
        _layers = init.getDependencyAs<Layers::LayerModule>("Layer");
        return Modules::Result::Success();
    }


    void EventModule::_createLogger(){
        _logger = spdlog::get("Raindrop::Event");
        if (!_logger){
            _logger = spdlog::stdout_color_mt("Raindrop::Event");
        }
    }

    void EventModule::shutdown(){
        
    }

    std::string EventModule::name() const noexcept{
        return "Event";
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
    }

    inline Modules::DependencyList EventModule::dependencies() const noexcept{
        return {
            Modules::Dependency("Layer")
        };
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