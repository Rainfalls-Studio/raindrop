#ifndef __RAINDROP_GRAPHICS_GUI_WIDGETS_TEXT_HPP__
#define __RAINDROP_GRAPHICS_GUI_WIDGETS_TEXT_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	class Text : public Item{
		public:
			Text(tinyxml2::XMLElement* element, Core::Registry::Registry& registry);
			virtual ~Text() = default;

			// Note that update also update the childs
			virtual bool update() override;

			std::string _unformatedText;
			void getText(tinyxml2::XMLElement* element);
	};
}

#endif