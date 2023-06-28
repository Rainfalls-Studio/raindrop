#ifndef __RAINDROP_GRAPHICS_GUI_WIDGETS_DRAG_INT_HPP__
#define __RAINDROP_GRAPHICS_GUI_WIDGETS_DRAG_INT_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	class DragInt : public Item{
		public:
			DragInt(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager);
			virtual ~DragInt() = default;
			
			virtual bool update() override;

			int* _variable = nullptr;
			std::string _label;
			float _speed = 1.f;
			int _min = 0.f;
			int _max = 0.f;
			std::string _format = "%d";
			ImGuiSliderFlags _flags = 0;

			void getVariable(tinyxml2::XMLElement* element);
			void getLabel(tinyxml2::XMLElement* element);
			void getSpeed(tinyxml2::XMLElement* element);
			void getMin(tinyxml2::XMLElement* element);
			void getMax(tinyxml2::XMLElement* element);
			void getFormat(tinyxml2::XMLElement* element);
			void getFlags(tinyxml2::XMLElement* element);
	};
}

#endif