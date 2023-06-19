#include <Raindrop/Graphics/GUI/Commands/Open.hpp>

namespace Raindrop::Graphics::GUI::Commands{
	Open::Open(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager) : Item(element, registry, _eventManager){
		getUnformatedPath(element);
	}

	bool Open::update(){
		_registry["Engine.GUI.OpenPath"] = _registry.formatString(_unformatedPath);
		_registry["Engine.GUI.OpenRoot"] = (void*)nullptr;
		_eventManager.trigger("Engine.GUI.Open");
		return false;
	}

	void Open::getUnformatedPath(tinyxml2::XMLElement* element){
		const char* path;
		if (element->QueryStringAttribute("path", &path) != tinyxml2::XML_SUCCESS){
			throw std::runtime_error("failed to read Set value");
		}
		_unformatedPath = path;
	}
}