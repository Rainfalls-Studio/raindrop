#include <Raindrop/Graphics/GUI/Widgets/DragInt.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	DragInt::DragInt(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getVariable(element);
		getLabel(element);
		getSpeed(element);
		getMin(element);
		getMax(element);
		getFormat(element);
		getFlags(element);
	}
			
	bool DragInt::update(){
		ImGui::DragInt(_label.c_str(), _variable, _speed, _min, _max, _format.c_str(), _flags);
		return false;
	}

	void DragInt::getVariable(tinyxml2::XMLElement* element){
		const char* variable;
		if (element->QueryStringAttribute("variable", &variable) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read DragInt variable");
		}
		_variable = &_registry[variable].as<int>(0.f);
	}

	void DragInt::getLabel(tinyxml2::XMLElement* element){
		const char* label;
		if (element->QueryStringAttribute("label", &label) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read DragInt label");
		}
		_label = _registry.formatString(label);
	}

	void DragInt::getSpeed(tinyxml2::XMLElement* element){
		float speed;
		if (element->QueryFloatAttribute("speed", &speed) != tinyxml2::XML_SUCCESS){
			return;
		}
		_speed = speed;
	}

	void DragInt::getMin(tinyxml2::XMLElement* element){
		int min;
		if (element->QueryIntAttribute("min", &min) != tinyxml2::XML_SUCCESS){
			return;
		}
		_min = min;
	}

	void DragInt::getMax(tinyxml2::XMLElement* element){
		int max;
		if (element->QueryIntAttribute("max", &max) != tinyxml2::XML_SUCCESS){
			return;
		}
		_max = max;
	}

	void DragInt::getFormat(tinyxml2::XMLElement* element){
		const char* format;
		if (element->QueryStringAttribute("format", &format) != tinyxml2::XML_SUCCESS){
			return;
		}
		_format = format;
	}

	void DragInt::getFlags(tinyxml2::XMLElement* element){
		const char* flags;
		if (element->QueryStringAttribute("flags", &flags) != tinyxml2::XML_SUCCESS){
			return;
		}
		_flags = strToFlags(flags);
	}
}