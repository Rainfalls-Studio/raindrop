#ifndef __RAINDROP_CORE_REGISTRY_REGISTRY_HPP__
#define __RAINDROP_CORE_REGISTRY_REGISTRY_HPP__

#include <Raindrop/Core/Registry/common.hpp>

namespace Raindrop::Core::Registry{
	class Registry{
		public:
			Registry(RegistryManager& manager) : _manager{manager}{};
			~Registry() = default;

			std::wstring& get(const std::wstring& name){
				return _nameToValue[name];
			}

		private:
			RegistryManager& _manager;
			std::unordered_map<std::wstring, std::wstring> _nameToValue;
	};
}

#endif