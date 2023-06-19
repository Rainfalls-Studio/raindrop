#include <Raindrop/Graphics/GUI/Widgets/Separator.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	Separator::Separator(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){

	}

	bool Separator::update(){
		ImGui::Separator();
		return false;
	}
}