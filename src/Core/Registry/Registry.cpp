#include <Raindrop/Core/Registry/Registry.hpp>

namespace Raindrop::Core::Registry{
	Registry& Registry::get(std::string name){
		size_t pos = name.find_first_of(".");
		if (pos != std::string::npos){
			std::string registry = name.substr(0, pos);
			return _nameToRegistry[registry].get(name.substr(pos+1));
		} else {
			return _nameToRegistry[name];
		}
	}

	const Registry& Registry::get(std::string name) const{
		size_t pos = name.find_first_of(".");
		if (pos != std::string::npos){
			std::string registry = name.substr(0, pos+1);
			return _nameToRegistry.find(name)->second.get(name.substr(pos+1));
		} else {
			return _nameToRegistry.find(name)->second;
		}
	}

	void Registry::remove(std::string name){
		size_t pos = name.find_first_of(".");
		if (pos != std::string::npos){
			std::string registry = name.substr(0, pos+1);
			_nameToRegistry[name].remove(name.substr(pos+1));
		} else {
			_nameToRegistry.erase(name);
		}
	}

	std::string Registry::formatString(std::string str){
		std::regex variableRegex("\\$\\{(.+?)\\}");

		std::smatch match;
		while (std::regex_search(str, match, variableRegex)){
			std::string variableName = match[1].str();
			str = std::regex_replace(str, variableRegex, get(variableName).toString());
		}
		
		return str;
	}

	std::string Registry::toString() const{
		if (_value.type() == typeid(std::string)) return std::any_cast<std::string>(_value);
		if (_value.type() == typeid(const char*)) return std::any_cast<const char*>(_value);
		if (_value.type() == typeid(int)) return std::to_string(std::any_cast<int>(_value));
		if (_value.type() == typeid(long long int)) return std::to_string(std::any_cast<long long int>(_value));
		if (_value.type() == typeid(long int)) return std::to_string(std::any_cast<long int>(_value));
		if (_value.type() == typeid(unsigned long int)) return std::to_string(std::any_cast<unsigned long int>(_value));
		if (_value.type() == typeid(unsigned int)) return std::to_string(std::any_cast<unsigned int>(_value));
		if (_value.type() == typeid(float)) return std::to_string(std::any_cast<float>(_value));
		if (_value.type() == typeid(unsigned long long int)) return std::to_string(std::any_cast<unsigned long long int>(_value));
		if (_value.type() == typeid(long double)) return std::to_string(std::any_cast<long double>(_value));
		if (_value.type() == typeid(double)) return std::to_string(std::any_cast<double>(_value));
		if (_value.type() == typeid(void*)) return std::to_string((unsigned long long)std::any_cast<void*>(_value));
		if (_value.type() == typeid(bool)) return std::any_cast<bool>(_value) == true ? "true" : "false";

		if (_value.type() == typeid(glm::vec2)){
			glm::vec2 vec = std::any_cast<glm::vec2>(_value);
			char str[100];
			std::sprintf(str, "(%f, %f)", vec.x, vec.y);
			return str;
		}

		if (_value.type() == typeid(glm::vec3)){
			glm::vec3 vec = std::any_cast<glm::vec3>(_value);
			char str[100];
			std::sprintf(str, "(%f, %f, %f)", vec.x, vec.y, vec.z);
			return str;
		}

		if (_value.type() == typeid(glm::vec4)){
			glm::vec4 vec = std::any_cast<glm::vec4>(_value);
			char str[100];
			std::sprintf(str, "(%f, %f, %f, %f)", vec.x, vec.y, vec.z, vec.w);
			return str;
		}
		
		return "";
	}
}