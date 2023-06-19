#include <Raindrop/Graphics/GUI/Commands/SetFloat.hpp>

namespace Raindrop::Graphics::GUI::Commands{
	SetFloat::SetFloat(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getVariable(element);
		getValue(element);
	}

	bool SetFloat::update(){
		_registry[_registry.formatString(_variable)] = _value;
		return false;
	}

	void SetFloat::getVariable(tinyxml2::XMLElement* element){
		const char* variable;
		if (element->QueryStringAttribute("variable", &variable) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read SetFloat variable");
		}
		_variable = _registry.formatString(variable);
	}

	void SetFloat::getValue(tinyxml2::XMLElement* element){
		float value;
		if (element->QueryFloatAttribute("value", &value) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read SetFloat value");
		}
		_value = value;
	}
}