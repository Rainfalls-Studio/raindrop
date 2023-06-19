#ifndef __RAINDROP_GRAPHICS_GUI_INTERPRETER_HPP__
#define __RAINDROP_GRAPHICS_GUI_INTERPRETER_HPP__

#include <Raindrop/Graphics/GUI/common.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>
#include <Raindrop/Core/Event/EventManager.hpp>

namespace Raindrop::Graphics::GUI{
	class Interpreter{
		public:
			Interpreter(Core::Registry::Registry& registry, Core::Event::EventManager& eventManager);
			~Interpreter(); 

			void parse(const std::filesystem::path& path, Item* root = nullptr);

			void update();
			void clear();

		private:
			Core::Registry::Registry& _registry;
			Core::Event::EventManager& _eventManager;
			std::list<std::unique_ptr<Item>> _items;

			void subscribeEvents();
	};
}

#endif