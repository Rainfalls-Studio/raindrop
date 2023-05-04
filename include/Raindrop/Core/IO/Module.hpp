#ifndef __RAINDROP_CORE_IO_MODULE_HPP__
#define __RAINDROP_CORE_IO_MODULE_HPP__

#include <Core/IO/DLLReader.hpp>

namespace Raindrop::Core::IO{
	class RAINDROP_API Module{
		public:
			Module(const std::filesystem::path& path);
			~Module();

			bool isOpen() const;
			const std::filesystem::path& filepath() const;
			void* getFnc(const char* name) const;
		
		private:
			DLLReader _dll;
			bool _loaded;

			bool init();
			void shutdown();
	};
}

#endif