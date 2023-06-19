#ifndef __RAINDROP_GRAPHICS_GUI_WIDGETS_BUTTON_HPP__
#define __RAINDROP_GRAPHICS_GUI_WIDGETS_BUTTON_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	class Button : public Item{
		public:
			Button(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager);
			virtual ~Button() = default;

			// Note that update also update the childs
			virtual bool update() override;

			std::string _label;
			ImVec2 _size;

			void getLabel(tinyxml2::XMLElement* element);
			void getSize(tinyxml2::XMLElement* element);
	};
}

#endif