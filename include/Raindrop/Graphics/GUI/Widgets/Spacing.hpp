#ifndef __RAINDROP_GRAPHICS_GUI_WIDGETS_SPACING_HPP__
#define __RAINDROP_GRAPHICS_GUI_WIDGETS_SPACING_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	class Spacing : public Item{
		public:
			Spacing(tinyxml2::XMLElement* element, Core::Registry::Registry& registry);
			virtual ~Spacing() = default;

			virtual bool update() override;
	};
}

#endif