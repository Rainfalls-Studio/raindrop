#pragma once

#include <Layers/LayerModule.hpp>
#include <Raindrop/Modules/IModule.hpp>
#include <Scheduler/Scheduler.hpp>
#include "Event.hpp"

namespace Event{
    class EventModule : public Raindrop::IModule{
        public:
            class PollStage : public Scheduler::IStage{
                public:
                    PollStage() = default;
                    virtual ~PollStage() = default;

                    virtual const char* name() const override;

                    virtual void initialize(Scheduler::StageInitHelper& helper) override;
                    virtual Scheduler::StageResult execute() override;
                    
                private:
                    std::weak_ptr<EventModule> _event;
            };

            EventModule();
            virtual ~EventModule() = default;

            virtual Raindrop::Result initialize(Raindrop::InitHelper& init) override;
            virtual void shutdown() override;

            template<typename T, typename... Args>
            void trigger(Args&&... args){
                pushEvent(std::make_unique<T>(std::forward<Args>(args)...));
            }

            // will transmit all stacked events to the layers
            void poll();
        
        private:
            std::weak_ptr<Layers::LayerModule> _layers;
            std::shared_ptr<spdlog::logger> _logger;
            std::mutex _mtx;
            
            std::deque<std::unique_ptr<Event>> _events;

            void pushEvent(std::unique_ptr<Event>&& event);
    };
}