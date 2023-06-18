#include <Raindrop/Graphics/GUI/Commands/SetInt.hpp>

namespace Raindrop::Graphics::GUI::Commands{
	SetInt::SetInt(tinyxml2::XMLElement* element, Core::Registry::Registry& registry) : Item(element, registry){
		getVariable(element);
		getValue(element);
	}

	bool SetInt::update(){
		_registry[_registry.formatString(_variable)] = _value;
		return false;
	}

	void SetInt::getVariable(tinyxml2::XMLElement* element){
		const char* variable;
		if (element->QueryStringAttribute("variable", &variable) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read SetInt variable");
		}
		_variable = _registry.formatString(variable);
	}

	void SetInt::getValue(tinyxml2::XMLElement* element){
		int value;
		if (element->QueryIntAttribute("value", &value) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read SetInt value");
		}
		_value = value;
	}
}