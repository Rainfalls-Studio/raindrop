#ifndef __RAINDROP_GRAPHICS_GUI_WIDGETS_GROUP_HPP__
#define __RAINDROP_GRAPHICS_GUI_WIDGETS_GROUP_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	class Group : public Item{
		public:
			Group(tinyxml2::XMLElement* element, Core::Registry::Registry& registry);
			virtual ~Group() = default;

			// Note that update also update the childs
			virtual bool update() override;
	};
}

#endif