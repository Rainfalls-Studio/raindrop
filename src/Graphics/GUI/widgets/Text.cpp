#include <Raindrop/Graphics/GUI/Widgets/Text.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	Text::Text(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getText(element);
	}

	bool Text::update(){
		std::string text = _registry.formatString(_unformatedText);
		ImGui::Text("%s", text.c_str());
		return false;
	}
	
	void Text::getText(tinyxml2::XMLElement* element){
		const char* text;
		if (element->QueryStringAttribute("text", &text) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("cannot find \"text\" attribute");
		}
		_unformatedText = text;
	}
}