#ifndef __RAINDROP_GRAPHICS_GUI_INTERPRETER_HPP__
#define __RAINDROP_GRAPHICS_GUI_INTERPRETER_HPP__

#include <Raindrop/Graphics/GUI/common.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>

namespace Raindrop::Graphics::GUI{
	class Interpreter{
		public:
			Interpreter(Core::Registry::Registry& registry);
			~Interpreter(); 

			void parse(const std::filesystem::path& path);

			void update();
			void clear();

		private:
			Core::Registry::Registry& _registry;
			std::list<std::unique_ptr<Item>> _items;
	};
}

#endif