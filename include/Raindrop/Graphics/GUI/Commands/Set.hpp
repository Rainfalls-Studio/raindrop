#ifndef __RAINDROP_GRAPHICS_GUI_COMMANDS_SET_HPP__
#define __RAINDROP_GRAPHICS_GUI_COMMANDS_SET_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Commands{
	class Set : public Item{
		public:
			Set(tinyxml2::XMLElement* element, Core::Registry::Registry& registry);
			virtual ~Set() = default;

			virtual bool update() override;

			std::string _variable;
			std::string _value;

			void getVariable(tinyxml2::XMLElement* element);
			void getValue(tinyxml2::XMLElement* element);
	};
}

#endif