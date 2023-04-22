#ifndef __RAINDROP_CORE_IO_DLL_READFER_HPP__
#define __RAINDROP_CORE_IO_DLL_READFER_HPP__

#include <common.hpp>

namespace Raindrop::Core::IO{
	class RAINDROP_API DLLReader{
		public:
			DLLReader(const char* filepath);
			~DLLReader();

			bool isOpen() const;
			const char* filepath() const;
			void* getProc(const char* name) const;
		
		private:
			const char* _filepath;
			void* _dll;
	};
}

#endif