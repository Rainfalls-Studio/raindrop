#include <Raindrop/Graphics/GUI/Widgets/SameLine.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	SameLine::SameLine(tinyxml2::XMLElement* element, Core::Registry::Registry& registry) : Item(element, registry){
		getOffset(element);
		getSpacing(element);
	}

	bool SameLine::update(){
		auto it = _childs.begin();
		while (it != _childs.end()){
			ImGui::SameLine(_offset, _spacing);
			if (it->get()->update()){
				_childs.erase(it);
			}
			it++;
		}
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