#include <Raindrop/Graphics/GUI/Commands/Set.hpp>

namespace Raindrop::Graphics::GUI::Commands{
	Set::Set(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getVariable(element);
		getValue(element);
	}

	bool Set::update(){
		_registry[_registry.formatString(_variable)] = _registry.formatString(_value);
		return false;
	}

	void Set::getVariable(tinyxml2::XMLElement* element){
		const char* variable;
		if (element->QueryStringAttribute("variable", &variable) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read Set variable");
		}
		_variable = _registry.formatString(variable);
	}

	void Set::getValue(tinyxml2::XMLElement* element){
		const char* value;
		if (element->QueryStringAttribute("value", &value) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read Set value");
		}
		_value = _registry.formatString(value);
	}
}