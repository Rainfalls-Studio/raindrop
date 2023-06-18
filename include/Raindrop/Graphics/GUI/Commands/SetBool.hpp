#ifndef __RAINDROP_GRAPHICS_GUI_COMMANDS_SET_BOOL_HPP__
#define __RAINDROP_GRAPHICS_GUI_COMMANDS_SET_BOOL_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Commands{
	class SetBool : public Item{
		public:
			SetBool(tinyxml2::XMLElement* element, Core::Registry::Registry& registry);
			virtual ~SetBool() = default;

			virtual bool update() override;

			std::string _variable;
			bool _value;

			void getVariable(tinyxml2::XMLElement* element);
			void getValue(tinyxml2::XMLElement* element);
	};
}

#endif