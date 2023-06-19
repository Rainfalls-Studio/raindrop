#include <Raindrop/Graphics/GUI/Widgets/Group.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	Group::Group(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
	}

	bool Group::update(){
		ImGui::BeginGroup();
		auto it = _childs.begin();
		while (it != _childs.end()){
			if (it->get()->update()){
				_childs.erase(it);
			}
			it++;
		}
		ImGui::EndGroup();
		return false;
	}
}