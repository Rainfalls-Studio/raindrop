#pragma once

#include "SubscriptionData.hpp"

namespace Raindrop::Scheduler{
    class Subscription{
        public:
            constexpr inline Subscription() noexcept : _data{}{}
            constexpr inline Subscription(SharedSubscriptionData data) : _data{data}{}

            Subscription(const Subscription&) = delete;
            Subscription(Subscription&&) = default;

            Subscription& operator=(const Subscription&) = delete;
            Subscription& operator=(Subscription&&) = default;

            inline void unsubscribe(){
                _data.reset();
            }

        private:
            SharedSubscriptionData _data;
    };
}