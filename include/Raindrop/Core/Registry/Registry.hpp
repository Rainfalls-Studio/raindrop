#ifndef __RAINDROP_CORE_REGISTRY_REGISTRY_HPP__
#define __RAINDROP_CORE_REGISTRY_REGISTRY_HPP__

#include <Raindrop/Core/Registry/common.hpp>

namespace Raindrop::Core::Registry{
	class Registry{
		public:
			Registry() = default;
			Registry(const std::string& val) : _value{val}{}
			~Registry() = default;

			std::string& value();
			const std::string& value() const;

			Registry& get(std::string name);
			const Registry& get(std::string name) const;

			Registry& operator[](const std::string& name){
				return get(name);
			}

			const Registry& operator[](const std::string& name) const{
				return get(name);
			}

			void remove(std::string name);
			std::string formatString(std::string str);

			Registry& operator=(const std::string& value){
				_value = value;
				return *this;
			}

			operator const std::string&(){
				return _value;
			}

		private:
			std::unordered_map<std::string, Registry> _nameToRegistry;
			std::string _value;
	};
}

#endif