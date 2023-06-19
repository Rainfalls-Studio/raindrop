#include <Raindrop/Core/Event/EventManager.hpp>

namespace Raindrop::Core::Event{
	EventManager::EventManager(){
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Core.EventManager");
    	customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");
		CLOG(INFO, "Engine.Core.EventManager") << "New Event Manager";

		_keyEvents = std::make_unique<KeyEvents>();	
		_mouseEvents = std::make_unique<MouseEvents>();	
	}
	
	EventManager::~EventManager() {
		CLOG(INFO, "Engine.Core.EventManager") << "Destroying Event Manager";
	}

	void EventManager::subscribe(const std::string& name, Callback callback){
		CLOG(TRACE, "Engine.Core.EventManager") << "New subscriber to \"" << name << "\" event";
		_nameToCallbacksMap[name].push_back(callback);
	}

	void EventManager::trigger(const std::string& name){
		auto& callbacks = _nameToCallbacksMap[name];
		for (auto &c : callbacks){
			c();
		}
	}
	
	KeyEvents& EventManager::keyEvents(){
		return *_keyEvents;
	}

	MouseEvents& EventManager::mouseEvents(){
		return *_mouseEvents;
	}
}