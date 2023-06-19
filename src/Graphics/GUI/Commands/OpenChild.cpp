#include <Raindrop/Graphics/GUI/Commands/OpenChild.hpp>

namespace Raindrop::Graphics::GUI::Commands{
	OpenChild::OpenChild(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getUnformatedPath(element);
	}

	bool OpenChild::update(){
		_registry["Engine.GUI.OpenPath"] = _registry.formatString(_unformatedPath);
		_registry["Engine.GUI.OpenRoot"] = static_cast<void*>(this);
		_eventManager.trigger("Engine.GUI.Open");
		return false;
	}

	void OpenChild::getUnformatedPath(tinyxml2::XMLElement* element){
		const char* path;
		if (element->QueryStringAttribute("path", &path) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read Set value");
		}
		_unformatedPath = path;
	}
}