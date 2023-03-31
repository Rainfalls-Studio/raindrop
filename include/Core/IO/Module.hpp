#ifndef __RAINDROP_CORE_IO_MODULE_HPP__
#define __RAINDROP_CORE_IO_MODULE_HPP__

#include "Core/IO/DLLReader.hpp"

namespace Raindrop::Core::IO{
	class Module{
		public:
			Module(const char* path);
			~Module();

			bool isOpen() const;
			const char* filepath() const;
			void* getFnc(const char* name) const;
		
		private:
			DLLReader _dll;
			bool _loaded;

			bool init();
			void shutdown();
	};
}

#endif