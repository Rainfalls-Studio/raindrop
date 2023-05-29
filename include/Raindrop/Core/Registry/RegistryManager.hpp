#ifndef __RAINDROP_CORE_REGISTRY_REGISTRY_MANAGER_HPP__
#define __RAINDROP_CORE_REGISTRY_REGISTRY_MANAGER_HPP__

#include <Raindrop/Core/Registry/common.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>

namespace Raindrop::Core::Registry{
	class RegistryManager{
		public:
			RegistryManager(){
				el::Logger* customLogger = el::Loggers::getLogger("Engine.Registry");
				customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");
				CLOG(INFO, "Engine.Registry") << "Creating Registry manager ...";
			}

			~RegistryManager(){
				CLOG(INFO, "Engine.Registry") << "Destroying Registry manager ...";
				_registries.clear();
			}

			Registry& operator[](const uint64_t id){
				return _registries[id];
			}

			Registry& operator[](const void* ptr){
				return (*this)[reinterpret_cast<uint64_t>(ptr)];
			}

			Registry& get(const uint64_t id = 0){
				return _registries[id];
			}

			void removeRegister(uint64_t id){
				CLOG(INFO, "Engine.Registry") << "Remouving registry from registry manager ...";
				_registries.erase(id);
			}

		private:
			std::unordered_map<uint64_t, Registry> _registries;
	};
}

#endif