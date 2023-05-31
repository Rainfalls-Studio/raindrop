#include <Raindrop/Core/Registry/Registry.hpp>

namespace Raindrop::Core::Registry{
	std::string& Registry::value(){
		return _value;
	}

	const std::string& Registry::value() const{
		return _value;
	}

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

			str = std::regex_replace(str, variableRegex, get(variableName).value());
		}
		
		return str;
	}
}