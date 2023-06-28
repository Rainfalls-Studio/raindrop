#include <Raindrop/Graphics/GUI/Widgets/DragVec2.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	DragVec2::DragVec2(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getVariable(element);
		getLabel(element);
		getSpeed(element);
		getMin(element);
		getMax(element);
		getFormat(element);
		getFlags(element);
	}
			
	bool DragVec2::update(){
		ImGui::DragFloat2(_label.c_str(), reinterpret_cast<float*>(_variable), _speed, _min, _max, _format.c_str(), _flags);
		return false;
	}

	void DragVec2::getVariable(tinyxml2::XMLElement* element){
		const char* variable;
		if (element->QueryStringAttribute("variable", &variable) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read DragVec2 variable");
		}
		_variable = &_registry[variable].as<glm::vec2>({0.f, 0.f});
	}

	void DragVec2::getLabel(tinyxml2::XMLElement* element){
		const char* label;
		if (element->QueryStringAttribute("label", &label) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read DragVec2 label");
		}
		_label = _registry.formatString(label);
	}

	void DragVec2::getSpeed(tinyxml2::XMLElement* element){
		float speed;
		if (element->QueryFloatAttribute("speed", &speed) != tinyxml2::XML_SUCCESS){
			return;
		}
		_speed = speed;
	}

	void DragVec2::getMin(tinyxml2::XMLElement* element){
		float min;
		if (element->QueryFloatAttribute("min", &min) != tinyxml2::XML_SUCCESS){
			return;
		}
		_min = min;
	}

	void DragVec2::getMax(tinyxml2::XMLElement* element){
		float max;
		if (element->QueryFloatAttribute("max", &max) != tinyxml2::XML_SUCCESS){
			return;
		}
		_max = max;
	}

	void DragVec2::getFormat(tinyxml2::XMLElement* element){
		const char* format;
		if (element->QueryStringAttribute("format", &format) != tinyxml2::XML_SUCCESS){
			return;
		}
		_format = format;
	}

	void DragVec2::getFlags(tinyxml2::XMLElement* element){
		const char* flags;
		if (element->QueryStringAttribute("flags", &flags) != tinyxml2::XML_SUCCESS){
			return;
		}
		_flags = strToFlags(flags);
	}
}