#include <Raindrop/Graphics/GUI/Widgets/SameLine.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	SameLine::SameLine(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getOffset(element);
		getSpacing(element);
	}

	bool SameLine::update(){
		ImGui::SameLine(_offset, _spacing);
		return false;
	}
	
	void SameLine::getOffset(tinyxml2::XMLElement* element){
		float offset;
		if (element->QueryFloatAttribute("offset", &offset) == tinyxml2::XML_SUCCESS){
			_offset = offset;
		}
	}

	void SameLine::getSpacing(tinyxml2::XMLElement* element){
		float spacing;
		if (element->QueryFloatAttribute("spacing", &spacing) == tinyxml2::XML_SUCCESS){
			_spacing = spacing;
		}
	}
}