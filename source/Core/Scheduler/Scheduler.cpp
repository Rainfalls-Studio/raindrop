#include "Raindrop/Core/Scheduler/Scheduler.hpp"

#include <spdlog/spdlog.h>

namespace Raindrop::Scheduler{
    Scheduler::Scheduler(Engine& engine) : _engine{engine}{}

    Subscription Scheduler::subscribe(const Callback& callback, Priority priority){
        SharedSubscriptionData data = std::make_shared<SubscriptionData>(callback, priority);
        _subscriptions.insert({data, priority});
        return Subscription(data);
    }

    bool Scheduler::SubscriptionWrapper::operator<(const SubscriptionWrapper& other) const noexcept{
        return other.priority > priority;
    }

    void Scheduler::trigger(){
        auto it = _subscriptions.begin();

        while (it != _subscriptions.end()){
            auto ref = it->sub.lock();
            
            if (!ref){
                it = _subscriptions.erase(it);
                continue;
            }

            ref->getCallback()();
            ++it;
        }
    }
}