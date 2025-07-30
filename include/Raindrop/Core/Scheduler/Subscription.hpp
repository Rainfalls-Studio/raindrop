#pragma once

#include "SubscriptionData.hpp"

namespace Raindrop::Scheduler{
    class Subscription{
        public:
            constexpr Subscription() noexcept : _data{}{}
            constexpr Subscription(SharedSubscriptionData data) : _data{data}{}

            Subscription(const Subscription&) = delete;
            Subscription(Subscription&&) = default;

            Subscription& operator=(const Subscription&) = delete;
            Subscription& operator=(Subscription&&) = default;

        private:
            SharedSubscriptionData _data;
    };
}