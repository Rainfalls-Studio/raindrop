#ifndef __RAINDROP_GRAPHICS_GUI_WIDGETS_INDENT_HPP__
#define __RAINDROP_GRAPHICS_GUI_WIDGETS_INDENT_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	class Indent : public Item{
		public:
			Indent(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager);
			virtual ~Indent() = default;

			virtual bool update() override;

			float _indent = 0.f;

			void getIndent(tinyxml2::XMLElement* element);
	};
}

#endif