#ifndef __RAINDROP_GRAPHICS_GUI_INTERPRETER_HPP__
#define __RAINDROP_GRAPHICS_GUI_INTERPRETER_HPP__

#include <Raindrop/Graphics/GUI/common.hpp>

namespace Raindrop::Graphics::GUI{
	class Interpreter{
		public:
			Interpreter(Core::EngineContext& context);
			~Interpreter(); 

			void parse(const std::filesystem::path& path, Item* root = nullptr);

			void update();
			void clear();

		private:
			Core::EngineContext& _context;
			std::list<std::unique_ptr<Item>> _items;

			void subscribeEvents();
	};
}

#endif