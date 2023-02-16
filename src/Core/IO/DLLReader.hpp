#ifndef __RAINDROP_CORE_IO_DLL_READFER_HPP__
#define __RAINDROP_CORE_IO_DLL_READFER_HPP__

#include "../core.hpp"
#include "../common.hpp"

namespace Raindrop::Core::IO{
	class DLLReader{
		public:
			DLLReader(const char* filepath);
			~DLLReader();

			bool isOpen() const;
			void* getProc(const char* name) const;
		
		private:
			void* _dll;
	};
}

#endif