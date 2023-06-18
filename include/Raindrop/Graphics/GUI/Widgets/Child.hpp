#ifndef __RAINDROP_GRAPHICS_GUI_WIDGETS_CHILD_HPP__
#define __RAINDROP_GRAPHICS_GUI_WIDGETS_CHILD_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	class Child : public Item{
		public:
			Child(tinyxml2::XMLElement* element, Core::Registry::Registry& registry);
			virtual ~Child() = default;

			// Note that update also update the childs
			virtual bool update() override;

			std::string _id;
			ImVec2 _size = ImVec2(0, 0);
			bool _border = false;
			int _flags = 0;

			void getLabel(tinyxml2::XMLElement* element);
			void getSize(tinyxml2::XMLElement* element);
			void getBorder(tinyxml2::XMLElement* element);
			void getFlags(tinyxml2::XMLElement* element);
	};
}

#endif