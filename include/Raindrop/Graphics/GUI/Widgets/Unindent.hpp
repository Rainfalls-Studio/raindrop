#ifndef __RAINDROP_GRAPHICS_GUI_WIDGETS_UNINDENT_HPP__
#define __RAINDROP_GRAPHICS_GUI_WIDGETS_UNINDENT_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	class Unindent : public Item{
		public:
			Unindent(tinyxml2::XMLElement* element, Core::Registry::Registry& registry);
			virtual ~Unindent() = default;

			virtual bool update() override;

			float _indent = 0.f;

			void getIndent(tinyxml2::XMLElement* element);
	};
}

#endif