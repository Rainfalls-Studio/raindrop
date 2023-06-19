#include <Raindrop/Graphics/GUI/Widgets/Spacing.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	Spacing::Spacing(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){

	}
			
	bool Spacing::update(){
		ImGui::Spacing();
		return false;
	}
}