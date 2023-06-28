#include <Raindrop/Graphics/GUI/Widgets/DragFloat.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	DragFloat::DragFloat(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getVariable(element);
		getLabel(element);
		getSpeed(element);
		getMin(element);
		getMax(element);
		getFormat(element);
		getFlags(element);
	}
			
	bool DragFloat::update(){
		ImGui::DragFloat(_label.c_str(), _variable, _speed, _min, _max, _format.c_str(), _flags);
		return false;
	}

	void DragFloat::getVariable(tinyxml2::XMLElement* element){
		const char* variable;
		if (element->QueryStringAttribute("variable", &variable) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read DragFloat variable");
		}
		_variable = &_registry[variable].as<float>(0.f);
	}

	void DragFloat::getLabel(tinyxml2::XMLElement* element){
		const char* label;
		if (element->QueryStringAttribute("label", &label) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read DragFloat label");
		}
		_label = _registry.formatString(label);
	}

	void DragFloat::getSpeed(tinyxml2::XMLElement* element){
		float speed;
		if (element->QueryFloatAttribute("speed", &speed) != tinyxml2::XML_SUCCESS){
			return;
		}
		_speed = speed;
	}

	void DragFloat::getMin(tinyxml2::XMLElement* element){
		float min;
		if (element->QueryFloatAttribute("min", &min) != tinyxml2::XML_SUCCESS){
			return;
		}
		_min = min;
	}

	void DragFloat::getMax(tinyxml2::XMLElement* element){
		float max;
		if (element->QueryFloatAttribute("max", &max) != tinyxml2::XML_SUCCESS){
			return;
		}
		_max = max;
	}

	void DragFloat::getFormat(tinyxml2::XMLElement* element){
		const char* format;
		if (element->QueryStringAttribute("format", &format) != tinyxml2::XML_SUCCESS){
			return;
		}
		_format = format;
	}

	void DragFloat::getFlags(tinyxml2::XMLElement* element){
		const char* flags;
		if (element->QueryStringAttribute("flags", &flags) != tinyxml2::XML_SUCCESS){
			return;
		}
		_flags = strToFlags(flags);
	}
}