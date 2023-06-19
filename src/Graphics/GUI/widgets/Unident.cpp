#include <Raindrop/Graphics/GUI/Widgets/Unindent.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	Unindent::Unindent(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getIndent(element);
	}

	bool Unindent::update(){
		ImGui::Unindent(_indent);
		return false;
	}

	void Unindent::getIndent(tinyxml2::XMLElement* element){
		float indent;
		if (element->QueryFloatAttribute("indent", &indent) == tinyxml2::XML_SUCCESS){
			_indent = indent;
		}
	}
}