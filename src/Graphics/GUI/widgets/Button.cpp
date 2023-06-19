#include <Raindrop/Graphics/GUI/Widgets/Button.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	Button::Button(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getLabel(element);
		getSize(element);
	}

	bool Button::update(){
		if (ImGui::Button(_label.c_str(), _size)){
			auto it = _childs.begin();
			while (it != _childs.end()){
				if (it->get()->update()){
					_childs.erase(it);
				}
				it++;
			}
		}
		return false;
	}

	void Button::getLabel(tinyxml2::XMLElement* element){
		const char* label;
		if (element->QueryStringAttribute("label", &label) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read button label");
		}
		_label = _registry.formatString(label);
	}

	void Button::getSize(tinyxml2::XMLElement* element){
		float w;
		if (element->QueryFloatAttribute("w", &w) == tinyxml2::XML_SUCCESS){
			_size.x = w;
		}

		float h;
		if (element->QueryFloatAttribute("h", &h) == tinyxml2::XML_SUCCESS){
			_size.y = h;
		}
	}
}