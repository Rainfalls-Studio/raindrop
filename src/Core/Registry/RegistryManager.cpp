#include <Raindrop/Core/Registry/RegistryManager.hpp>

namespace Raindrop::Core::Registry{
	RegistryManager::RegistryManager(){
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Registry");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");
		CLOG(INFO, "Engine.Registry") << "Creating Registry manager ...";
	}

	RegistryManager::~RegistryManager(){
		CLOG(INFO, "Engine.Registry") << "Destroying Registry manager ...";
		_registries.clear();
	}

	Registry& RegistryManager::operator[](const size_t id){
		return get(std::to_wstring(id));
	}

	Registry& RegistryManager::operator[](const void* ptr){
		return get(reinterpret_cast<size_t>(ptr));
	}

	Registry& RegistryManager::operator[](const std::wstring& name){
		return get(name);
	}

	Registry& RegistryManager::get(const size_t id){
		return get(std::to_wstring(id));
	}
	
	Registry& RegistryManager::get(const void* ptr){
		return get(reinterpret_cast<size_t>(ptr));
	}

	Registry& RegistryManager::get(const std::wstring& name){
		auto it = _registries.find(name);
		if (it == _registries.end()){
			CLOG(INFO, "Engine.Registry") << "Creating new registry ...";
			_registries[name] = std::make_unique<Registry>(*this);
			return *_registries[name];
		}
		return *it->second;
	}

	void RegistryManager::removeRegister(const size_t id){
		removeRegister(std::to_wstring(id));
	}

	void RegistryManager::removeRegister(const void* ptr){
		removeRegister(reinterpret_cast<size_t>(ptr));
	}

	void RegistryManager::removeRegister(const std::wstring& name){
		CLOG(INFO, "Engine.Registry") << "Removing registry from registry manager ...";
		_registries.erase(name);
	}
}