#ifndef __RAINDROP_CORE_REGISTRY_REGISTRY_HPP__
#define __RAINDROP_CORE_REGISTRY_REGISTRY_HPP__

#include <Raindrop/Core/Registry/common.hpp>

namespace Raindrop::Core::Registry{
	class Registry{
		public:
			Registry() = default;
			~Registry() = default;

			template<typename T = std::wstring>
			T& operator[](const std::wstring& name){
				std::wistringstream stream = _nameToValue[name];
				T value;
				stream >> value;
				return value;
			}

		private:
			std::unordered_map<std::wstring, std::wstring> _nameToValue;
	};
}

#endif