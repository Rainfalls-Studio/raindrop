#ifndef __RAINDROP_GRAPHICS_GUI_WIDGETS_SAME_LINE_HPP__
#define __RAINDROP_GRAPHICS_GUI_WIDGETS_SAME_LINE_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	class SameLine : public Item{
		public:
			SameLine(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager);
			virtual ~SameLine() = default;

			// Note that update also update the childs
			virtual bool update() override;

			float _offset = 0.0f;
			float _spacing = -1.f;

			void getOffset(tinyxml2::XMLElement* element);
			void getSpacing(tinyxml2::XMLElement* element);
	};
}

#endif