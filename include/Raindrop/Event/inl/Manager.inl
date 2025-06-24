#include "../Manager.hpp"

#include <iostream>

namespace Raindrop::Event{
	template<typename EventType>
	void Manager::subscribe(std::function<bool(const EventType&)> callback){
		static_assert(std::is_base_of<Event, EventType>::value, "The event type has to be derived from Events::Event");

		subscribe(
			typeid(EventType),
			CallbackInfo{
				[callback](const Event& event) -> bool {
					return callback(static_cast<const EventType&>(event));
				}
			}
		);
	}

	template<typename EventType, typename ClassType>
	void Manager::subscribe(ClassType& instance, bool (ClassType::*memberFunction)(const EventType&)) {
		subscribe<EventType>([&instance, memberFunction](const EventType& event) -> bool {
			return (instance.*memberFunction)(event);
		});
	}

	template<typename EventType, typename ClassType>
	void Manager::subscribe(ClassType* instance, bool (ClassType::*memberFunction)(const EventType&)) {
		subscribe<EventType>([instance, memberFunction](const EventType& event) -> bool {
			return (instance->*memberFunction)(event);
		});
	}

	template<typename T>
	void Manager::trigger(const T& event){
		static_assert(std::is_base_of<Event, T>::value, "The event type has to be derived from Events::Event");
		trigger(typeid(T), static_cast<const Event&>(event));
	}
}