#ifndef __RAINDROP_CORE_EVENT_EVENT_MANAGER_HPP__
#define __RAINDROP_CORE_EVENT_EVENT_MANAGER_HPP__

/**
 * @brief Observer patern event manager
 * 
 */

#include <Raindrop/Core/Event/common.hpp>

namespace Raindrop::Core::Event{

	using Callback = std::function<void()>;

	class EventManager{
		public:
			EventManager() = default;
			~EventManager() = default;

			void subscribe(const std::string& name, Callback callback){
				_nameToCallbacksMap[name].push_back(callback);
			}

			void trigger(const std::string& name){
				auto& callbacks = _nameToCallbacksMap[name];
				for (auto &c : callbacks){
					c();
				}
			}

		private:
			std::unordered_map<std::string, std::list<Callback>> _nameToCallbacksMap;
	};
}

#endif