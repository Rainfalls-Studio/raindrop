#pragma once

#include <memory>
#include <functional>
#include "Priority.hpp"

namespace Raindrop::Scheduler{
    using Callback = std::function<void()>;

    class SubscriptionData{
        public:
            constexpr inline SubscriptionData(Callback callback, Priority priority) noexcept : _callback(callback), _priority(priority){}

            constexpr inline Callback getCallback() const noexcept{
                return  _callback;
            }

            constexpr inline Priority getPriority() const noexcept{
                return _priority;
            }

        private:
            Callback _callback;
            Priority _priority;
    };

    using SharedSubscriptionData = std::shared_ptr<SubscriptionData>;
    using WeakSubscriptionData = std::weak_ptr<SubscriptionData>;
}