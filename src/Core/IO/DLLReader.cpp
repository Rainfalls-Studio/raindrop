#include "DLLReader.hpp"
#include "../Debug/profiler.hpp"
#include "../Debug/logger.hpp"

#if defined(RAINDROP_WINDOWS)
	#include <windows.h>

	#define WIN_ERROR 0
	#undef ERROR
#endif

namespace Raindrop::Core::IO{
	#ifdef RAINDROP_WINDOWS
		DLLReader::DLLReader(const char* filepath){
			RAINDROP_profile_function();
			RAINDROP_log(INFO, IO, "loading \"%s\" dll library", filepath);
			_dll = nullptr;
			_dll = (void*)::LoadLibraryA(filepath);

			if (_dll == nullptr){
				RAINDROP_log(ERROR, IO, "failed to load \"%s\" dll file", filepath);
				return;
			}
		}

		DLLReader::~DLLReader(){
			RAINDROP_profile_function();
			if (_dll){
				::FreeLibrary((::HMODULE)_dll);
			}
		}

		bool DLLReader::isOpen() const{
			RAINDROP_profile_function();
			return _dll != nullptr;
		}

		void* DLLReader::getProc(const char* name) const{
			RAINDROP_profile_function();
			return (void*)::GetProcAddress((::HMODULE)_dll, (LPCSTR)name);
		}

	#else
		DLLReader::DLLReader(const char* filepath){
			RAINDROP_profile_function();
			RAINDROP_log(INFO, IO, "loading \"%s\" dll library", filepath);
			_dll = nullptr;
			_dll = dlopen(sDllPath.c_str(), RTLD_LAZY);

			if (_dll == nullptr){
				RAINDROP_log(ERROR, IO, "failed to load \"%s\" dll file", filepath);
				return;
			}
		}

		DLLReader::~DLLReader(){
			RAINDROP_profile_function();
			if (_dll){
				::dlclose((::HMODULE)_dll);
			}
		}

		bool DLLReader::isOpen() const{
			RAINDROP_profile_function();
			return _dll != nullptr;
		}

		void* DLLReader::getProc(const char* name) const{
			RAINDROP_profile_function();
			return (void*)::dlsym((::HMODULE)_dll, (LPCSTR)name);
		}

	#endif
}