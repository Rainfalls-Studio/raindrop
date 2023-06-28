#include <Raindrop/Graphics/GUI/Widgets/DragVec4.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	DragVec4::DragVec4(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getVariable(element);
		getLabel(element);
		getSpeed(element);
		getMin(element);
		getMax(element);
		getFormat(element);
		getFlags(element);
	}
			
	bool DragVec4::update(){
		ImGui::DragFloat4(_label.c_str(), reinterpret_cast<float*>(_variable), _speed, _min, _max, _format.c_str(), _flags);
		return false;
	}

	void DragVec4::getVariable(tinyxml2::XMLElement* element){
		const char* variable;
		if (element->QueryStringAttribute("variable", &variable) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read DragVec4 variable");
		}
		_variable = &_registry[variable].as<glm::vec4>({0.f, 0.f, 0.f, 0.f});
	}

	void DragVec4::getLabel(tinyxml2::XMLElement* element){
		const char* label;
		if (element->QueryStringAttribute("label", &label) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read DragVec4 label");
		}
		_label = _registry.formatString(label);
	}

	void DragVec4::getSpeed(tinyxml2::XMLElement* element){
		float speed;
		if (element->QueryFloatAttribute("speed", &speed) != tinyxml2::XML_SUCCESS){
			return;
		}
		_speed = speed;
	}

	void DragVec4::getMin(tinyxml2::XMLElement* element){
		float min;
		if (element->QueryFloatAttribute("min", &min) != tinyxml2::XML_SUCCESS){
			return;
		}
		_min = min;
	}

	void DragVec4::getMax(tinyxml2::XMLElement* element){
		float max;
		if (element->QueryFloatAttribute("max", &max) != tinyxml2::XML_SUCCESS){
			return;
		}
		_max = max;
	}

	void DragVec4::getFormat(tinyxml2::XMLElement* element){
		const char* format;
		if (element->QueryStringAttribute("format", &format) != tinyxml2::XML_SUCCESS){
			return;
		}
		_format = format;
	}

	void DragVec4::getFlags(tinyxml2::XMLElement* element){
		const char* flags;
		if (element->QueryStringAttribute("flags", &flags) != tinyxml2::XML_SUCCESS){
			return;
		}
		_flags = strToFlags(flags);
	}
}