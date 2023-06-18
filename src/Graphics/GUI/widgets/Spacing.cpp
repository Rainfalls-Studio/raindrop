#include <Raindrop/Graphics/GUI/Widgets/Spacing.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	Spacing::Spacing(tinyxml2::XMLElement* element, Core::Registry::Registry& registry) : Item{element, registry}{

	}
			
	bool Spacing::update(){
		ImGui::Spacing();
		return false;
	}
}