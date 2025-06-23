#pragma once

#include "Raindrop/Core/Internal/Event/Manager.hpp"
#include "Event.hpp"

namespace Raindrop::Core::Event{
    class Manager{
        public:
            using CallbackInfo = Core::Internal::Event::Manager::CallbackInfo;

            inline Manager();
            inline Manager(std::shared_ptr<Core::Internal::Event::Manager> impl);
            ~Manager() = default;

            static Manager Create();

            inline bool isValid() const noexcept;

            inline void subscribe(const std::type_index& id, const CallbackInfo& info);

            inline void trigger(const std::type_index& id, const Event& event);

			template<typename EventType>
			inline void subscribe(std::function<bool(const EventType&)> callback);

			template<typename EventType, typename ClassType>
			inline void subscribe(ClassType& instance, bool (ClassType::*memberFunction)(const EventType&));

			template<typename EventType, typename ClassType>
			inline void subscribe(ClassType* instance, bool (ClassType::*memberFunction)(const EventType&));

			template<typename T>
			inline void trigger(const T& event);

            inline Core::Internal::Event::Manager* get();

        private:
            inline Internal::Event::Manager& assertGet();

            std::shared_ptr<Internal::Event::Manager> _impl;
    };
}

#include "inl/Manager.inl"