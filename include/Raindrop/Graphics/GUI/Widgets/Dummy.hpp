#ifndef __RAINDROP_GRAPHICS_GUI_WIDGETS_DUMMY_HPP__
#define __RAINDROP_GRAPHICS_GUI_WIDGETS_DUMMY_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	class Dummy : public Item{
		public:
			Dummy(tinyxml2::XMLElement* element, Core::Registry::Registry& registry);
			virtual ~Dummy() = default;
			
			virtual bool update() override;

			ImVec2 _size;

			void getSize(tinyxml2::XMLElement* element);
	};
}

#endif