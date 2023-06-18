#include <Raindrop/Graphics/GUI/Widgets/Separator.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	Separator::Separator(tinyxml2::XMLElement* element, Core::Registry::Registry& registry) : Item(element, registry){

	}

	bool Separator::update(){
		ImGui::Separator();
		return false;
	}
}