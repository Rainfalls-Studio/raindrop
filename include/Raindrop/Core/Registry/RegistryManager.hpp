#ifndef __RAINDROP_CORE_REGISTRY_REGISTRY_MANAGER_HPP__
#define __RAINDROP_CORE_REGISTRY_REGISTRY_MANAGER_HPP__

#include <Raindrop/Core/Registry/common.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>

namespace Raindrop::Core::Registry{
	class RegistryManager{
		public:
			RegistryManager();
			~RegistryManager();

			Registry& operator[](const size_t id);
			Registry& operator[](const void* ptr);
			Registry& operator[](const std::wstring& name);

			Registry& get(const size_t id = 0);
			Registry& get(const void* ptr = nullptr);
			Registry& get(const std::wstring& name = L"\0");

			void removeRegister(const size_t id);
			void removeRegister(const void* ptr);
			void removeRegister(const std::wstring& name);

		private:
			std::unordered_map<std::wstring, std::unique_ptr<Registry>> _registries;
	};
}

#endif