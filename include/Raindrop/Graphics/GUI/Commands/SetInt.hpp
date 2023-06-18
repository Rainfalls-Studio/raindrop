#ifndef __RAINDROP_GRAPHICS_GUI_COMMANDS_SET_INT_HPP__
#define __RAINDROP_GRAPHICS_GUI_COMMANDS_SET_INT_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Commands{
	class SetInt : public Item{
		public:
			SetInt(tinyxml2::XMLElement* element, Core::Registry::Registry& registry);
			virtual ~SetInt() = default;

			virtual bool update() override;

			std::string _variable;
			int _value;

			void getVariable(tinyxml2::XMLElement* element);
			void getValue(tinyxml2::XMLElement* element);
	};
}

#endif