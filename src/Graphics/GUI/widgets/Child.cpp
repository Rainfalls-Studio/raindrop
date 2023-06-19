#include <Raindrop/Graphics/GUI/Widgets/Child.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	Child::Child(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getLabel(element);
		getSize(element);
		getBorder(element);
		getFlags(element);
	}

	bool Child::update(){
		if (ImGui::BeginChild(_id.c_str(), _size, _border, _flags)){
			auto it = _childs.begin();
			while (it != _childs.end()){
				if (it->get()->update()){
					_childs.erase(it);
				}
				it++;
			}
		}
		ImGui::EndChild();
		return false;
	}

	void Child::getLabel(tinyxml2::XMLElement* element){
		const char* id;
		if (element->QueryStringAttribute("id", &id) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read child id");
		}
		_id = _registry.formatString(id);
	}

	void Child::getSize(tinyxml2::XMLElement* element){
		float w;
		if (element->QueryFloatAttribute("w", &w) == tinyxml2::XML_SUCCESS){
			_size.x = w;
		}

		float h;
		if (element->QueryFloatAttribute("h", &h) == tinyxml2::XML_SUCCESS){
			_size.y = h;
		}
	}

	void Child::getBorder(tinyxml2::XMLElement* element){
		bool border;
		if (element->QueryBoolAttribute("border", &border) == tinyxml2::XML_SUCCESS){
			_border = border;
		}
	}

	void Child::getFlags(tinyxml2::XMLElement* element){
		const char* flagsStr;
		if (element->QueryStringAttribute("flags", &flagsStr) == tinyxml2::XML_SUCCESS){
			_flags = strToFlags(flagsStr);
		}
	}
}