#ifndef __RAINDROP_GRAPHICS_GUI_ITEM_HPP__
#define __RAINDROP_GRAPHICS_GUI_ITEM_HPP__

#include <Raindrop/common.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>
#include <Raindrop/Core/Event/EventManager.hpp>

namespace Raindrop::Graphics::GUI{
	class Item{
		public:
			Item(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& eventManager) : _registry{registry}, _eventManager{eventManager}{
				for (tinyxml2::XMLElement* child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()){
					try{
						_childs.push_back(Item::create(child, _registry, _eventManager));
					} catch (const std::exception& e){
						CLOG(ERROR, "Engine.Graphics.GUI") << e.what();
						continue;
					}
				}
			};
			virtual ~Item() = default;

			// Note that update also update the childs
			virtual bool update() = 0;
			static std::unique_ptr<Item> create(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& eventManager);

			Core::Registry::Registry& _registry;
			Core::Event::EventManager& _eventManager;
			std::list<std::unique_ptr<Item>> _childs;

			static ImGuiWindowFlags strToFlags(const std::string& str);
	};
}

#endif