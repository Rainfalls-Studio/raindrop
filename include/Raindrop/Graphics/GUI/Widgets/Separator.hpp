#ifndef __RAINDROP_GRAPHICS_GUI_WIDGETS_SEPARATOR_HPP__
#define __RAINDROP_GRAPHICS_GUI_WIDGETS_SEPARATOR_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	class Separator : public Item{
		public:
			Separator(tinyxml2::XMLElement* element, Core::Registry::Registry& registry);
			virtual ~Separator() = default;

			// Note that update also update the childs
			virtual bool update() override;
	};
}

#endif