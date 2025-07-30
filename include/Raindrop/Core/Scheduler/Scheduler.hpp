#pragma once

#include <set>
#include "SubscriptionData.hpp"
#include "Subscription.hpp"
#include "Priority.hpp"

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Scheduler{
    class Scheduler{
        public:
            Scheduler(Engine& engine);

            Subscription subscribe(const Callback& callback, Priority priority);

            void trigger();

        private:
            Engine& _engine;

            struct SubscriptionWrapper{
                WeakSubscriptionData sub;
                const Priority priority;

                bool operator<(const SubscriptionWrapper& other) const noexcept;
            };

            std::set<SubscriptionWrapper> _subscriptions;
    };
}