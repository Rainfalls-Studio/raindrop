#ifndef __RAINDROP_CORE_EVENT_EVENT_MANAGER_HPP__
#define __RAINDROP_CORE_EVENT_EVENT_MANAGER_HPP__

/**
 * @brief Observer patern event manager
 * 
 */

#include <Raindrop/Core/Event/common.hpp>
#include <Raindrop/Core/Event/KeyEvents.hpp>
#include <Raindrop/Core/Event/MouseEvents.hpp>

namespace Raindrop::Core::Event{

	using Callback = std::function<void()>;

	class EventManager{
		public:
			EventManager();
			~EventManager();

			void subscribe(const std::string& name, Callback callback);
			void trigger(const std::string& name);

			KeyEvents& keyEvents();
			MouseEvents& mouseEvents();

		private:
			std::unordered_map<std::string, std::list<Callback>> _nameToCallbacksMap;
			std::unique_ptr<KeyEvents> _keyEvents;
			std::unique_ptr<MouseEvents> _mouseEvents;
	};
}

#endif