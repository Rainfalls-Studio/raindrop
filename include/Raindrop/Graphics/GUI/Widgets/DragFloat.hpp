#ifndef __RAINDROP_GRAPHICS_GUI_WIDGETS_DRAG_FLOAT_HPP__
#define __RAINDROP_GRAPHICS_GUI_WIDGETS_DRAG_FLOAT_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Widgets{
	class DragFloat : public Item{
		public:
			DragFloat(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager);
			virtual ~DragFloat() = default;
			
			virtual bool update() override;

			float* _variable = nullptr;
			std::string _label;
			float _speed = 1.f;
			float _min = 0.f;
			float _max = 0.f;
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