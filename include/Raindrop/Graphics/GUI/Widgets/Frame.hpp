#ifndef __RAINDROP_GRAPHICS_GUI_WIDGETS_FRAME_HPP__
#define __RAINDROP_GRAPHICS_GUI_WIDGETS_FRAME_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	class Frame : public Item{
		public:
			Frame(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager);
			virtual ~Frame() = default;

			// Note that update also update the childs
			virtual bool update() override;

			std::string _name;
			bool* _open = nullptr;
			int _flags = 0;

			void getName(tinyxml2::XMLElement* element);
			void getOpenValue(tinyxml2::XMLElement* element);
			void getFlags(tinyxml2::XMLElement* element);
	};
}

#endif