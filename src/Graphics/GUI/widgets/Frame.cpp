#include <Raindrop/Graphics/GUI/Widgets/Frame.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	Frame::Frame(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getName(element);
		getOpenValue(element);
		getFlags(element);
	}

	bool Frame::update(){
		ImGui::PushID(this);
		if (ImGui::Begin(_name.c_str(), _open, _flags)){
			auto it = _childs.begin();
			while (it != _childs.end()){
				if (it->get()->update()){
					_childs.erase(it);
				}
				it++;
			}
		}
		ImGui::End();
		ImGui::PopID();
		return _open == nullptr ? false : !(*_open);
	}

	void Frame::getName(tinyxml2::XMLElement* element){
		const char* name;
		if (element->QueryStringAttribute("name", &name) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read frame name");
		}
		_name = _registry.formatString(name);
	}

	void Frame::getOpenValue(tinyxml2::XMLElement* element){
		const char* openStr;
		if (element->QueryStringAttribute("open", &openStr) == tinyxml2::XML_NO_ATTRIBUTE){
			return;
		}
		_open = &_registry[openStr].as<bool>(true);
	}

	void Frame::getFlags(tinyxml2::XMLElement* element){
		const char* flagsStr;
		if (element->QueryStringAttribute("flags", &flagsStr) == tinyxml2::XML_NO_ATTRIBUTE){
			return;
		}
		_flags = strToFlags(flagsStr);
	}
}