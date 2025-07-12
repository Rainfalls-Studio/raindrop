#pragma once

#include <functional>
#include <list>
#include <typeindex>
#include <memory>
#include <spdlog/spdlog.h>

namespace Raindrop{
	class Engine;

	namespace Event{
		class Event;

		class Manager{
			public:
				struct CallbackInfo{
					std::function<bool(const Event&)> callback;
				};

				struct EventData{
					std::list<CallbackInfo> callbacks;
				};

				Manager(Engine& engine) noexcept;
				~Manager();
				
				void subscribe(const std::type_index& id, const CallbackInfo& info);
				// void unsubscribe(const std::type_index& id, const Listener* listener);

				void trigger(const std::type_index& id, const Event& event);


				template<typename EventType>
				void subscribe(std::function<bool(const EventType&)> callback);

				template<typename EventType, typename ClassType>
				void subscribe(ClassType& instance, bool (ClassType::*memberFunction)(const EventType&));

				template<typename EventType, typename ClassType>
				void subscribe(ClassType* instance, bool (ClassType::*memberFunction)(const EventType&));

				template<typename T>
				void trigger(const T& event);

			private:
				Engine& _engine;
				std::shared_ptr<spdlog::logger> _logger;
				std::unordered_map<std::type_index, EventData> _events;

				void _createLogger();
		};
	}
}

#include "inl/Manager.inl"