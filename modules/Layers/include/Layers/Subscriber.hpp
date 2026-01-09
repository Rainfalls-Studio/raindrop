#pragma once

#include <functional>

#include "Transmission.hpp"
#include "Result.hpp"

namespace Layers{
    class SubscriberBase{
        public:
            SubscriberBase(float priority) : _priority{priority}{}
            virtual ~SubscriberBase() = default;
            
            virtual Result call(const Transmission& tr) = 0;

            inline float priority() const noexcept{
                return _priority;
            }

        protected:
            float _priority;
    };

    template<typename T>
    class Subscriber : public SubscriberBase{
        public:
            using Callback = std::function<Result(const T&)>;
            inline Subscriber(const Callback& callback, float priority = 0.f) :
            SubscriberBase{priority},
            _callback(callback){}

            virtual ~Subscriber() = default;

            virtual Result call(const Transmission& tr) final override{
                if (const T* ptr = dynamic_cast<const T*>(&tr)){
                    return _callback(*ptr);
                }
                return Result::Continue();
            }
        
        private:
            std::function<Result(const T&)> _callback;
    };
}