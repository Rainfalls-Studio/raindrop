#include <Raindrop/Graphics/GUI/Widgets/Dummy.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	Dummy::Dummy(tinyxml2::XMLElement* element, Core::Registry::Registry& registry) : Item{element, registry}{
		getSize(element);
	}
			
	bool Dummy::update(){
		ImGui::Dummy(_size);
		return false;
	}

	void Dummy::getSize(tinyxml2::XMLElement* element){
		float w;
		if (element->QueryFloatAttribute("w", &w) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("cannot find \"w\" attribute");
		}
		_size.x = w;

		float h;
		if (element->QueryFloatAttribute("h", &h) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("cannot find \"h\" attribute");
		}	
		_size.y = h;
	}
}