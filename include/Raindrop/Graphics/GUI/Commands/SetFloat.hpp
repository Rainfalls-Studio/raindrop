#ifndef __RAINDROP_GRAPHICS_GUI_COMMANDS_SET_FLOAT_HPP__
#define __RAINDROP_GRAPHICS_GUI_COMMANDS_SET_FLOAT_HPP__

#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI::Commands{
	class SetFloat : public Item{
		public:
			SetFloat(tinyxml2::XMLElement* element, Core::Registry::Registry& registry, Core::Event::EventManager& _eventManager);
			virtual ~SetFloat() = default;

			virtual bool update() override;

			std::string _variable;
			float _value;

			void getVariable(tinyxml2::XMLElement* element);
			void getValue(tinyxml2::XMLElement* element);
	};
}

#endif