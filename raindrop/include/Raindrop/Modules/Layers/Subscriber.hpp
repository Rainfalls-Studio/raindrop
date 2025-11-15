#pragma once

#include <typeindex>
#include <functional>

#include "Transmission.hpp"
#include "Result.hpp"

namespace Raindrop::Layers{
    class SubscriberBase{
        public:
            SubscriberBase(float priority) : _priority{priority}{}
            virtual ~SubscriberBase() = default;
            
            virtual Result call(const TransmissionBase& tr) = 0;

            inline float priority() const noexcept{
                return _priority;
            }

        protected:
            float _priority;
    };

    template<typename T>
    class Subscriber : SubscriberBase{
        public:
            Subscriber(float priority) : SubscriberBase{priority}{}

            virtual ~Subscriber() = default;

            virtual Result call(const TransmissionBase& tr) final override{
                if (tr.type() != typeid(T)) return Result::Continue();
                return _callback(static_cast<const T&>(tr));
            }
        
        private:
            std::function<Result(const T&)> _callback;
    };
}