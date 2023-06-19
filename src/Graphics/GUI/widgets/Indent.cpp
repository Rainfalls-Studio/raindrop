#include <Raindrop/Graphics/GUI/Widgets/Indent.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	Indent::Indent(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getIndent(element);
	}

	bool Indent::update(){
		ImGui::Indent(_indent);
		return false;
	}

	void Indent::getIndent(tinyxml2::XMLElement* element){
		float indent;
		if (element->QueryFloatAttribute("indent", &indent) == tinyxml2::XML_SUCCESS){
			_indent = indent;
		}
	}
}