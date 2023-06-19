#include <Raindrop/Graphics/GUI/Commands/SetBool.hpp>

namespace Raindrop::Graphics::GUI::Commands{
	SetBool::SetBool(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getVariable(element);
		getValue(element);
	}

	bool SetBool::update(){
		_registry[_registry.formatString(_variable)] = _value;
		return false;
	}

	void SetBool::getVariable(tinyxml2::XMLElement* element){
		const char* variable;
		if (element->QueryStringAttribute("variable", &variable) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read SetFloat variable");
		}
		_variable = _registry.formatString(variable);
	}

	void SetBool::getValue(tinyxml2::XMLElement* element){
		bool value;
		if (element->QueryBoolAttribute("value", &value) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read SetBool value");
		}
		_value = value;
	}
}