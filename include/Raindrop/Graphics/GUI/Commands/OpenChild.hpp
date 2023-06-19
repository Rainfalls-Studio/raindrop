#ifndef __RAINDROP_GRAPHICS_GUI_COMMANDS_OPEN_CHILD_HPP__
#define __RAINDROP_GRAPHICS_GUI_COMMANDS_OPEN_CHILD_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Commands{
	class OpenChild : public Item{
		public:
			OpenChild(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager);
			virtual ~OpenChild() = default;

			virtual bool update() override;

			std::string _unformatedPath;

			void getUnformatedPath(tinyxml2::XMLElement* element);
	};
}

#endif