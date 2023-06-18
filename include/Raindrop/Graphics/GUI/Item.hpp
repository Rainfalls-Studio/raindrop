#ifndef __RAINDROP_GRAPHICS_GUI_ITEM_HPP__
#define __RAINDROP_GRAPHICS_GUI_ITEM_HPP__

#include <Raindrop/common.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>

namespace Raindrop::Graphics::GUI{
	class Item{
		public:
			Item(tinyxml2::XMLElement* element, Core::Registry::Registry& registry) : _registry{registry}{
				for (tinyxml2::XMLElement* child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()){
					try{
						_childs.push_back(Item::create(child, _registry));
					} catch (const std::exception& e){
						CLOG(ERROR, "Engine.Graphics.GUI") << e.what();
						continue;
					}
				}
			};
			virtual ~Item() = default;

			// Note that update also update the childs
			virtual bool update() = 0;
			static std::unique_ptr<Item> create(tinyxml2::XMLElement* element, Core::Registry::Registry& registry);

			Core::Registry::Registry& _registry;
			std::list<std::unique_ptr<Item>> _childs;

			static ImGuiWindowFlags strToFlags(const std::string& str);
	};
}

#endif