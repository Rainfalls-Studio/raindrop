#include "Core/IO/DLLReader.hpp"
#include "Core/Debug/profiler.hpp"
#include "Core/Debug/logger.hpp"

#if defined(RAINDROP_WINDOWS)
	#include <windows.h>

	#define WIN_ERROR 0
	#undef ERROR
#endif

namespace Raindrop::Core::IO{
	#ifdef RAINDROP_WINDOWS
		DLLReader::DLLReader(const char* filepath) : _filepath{filepath}{
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

		const char* DLLReader::filepath() const{
			return _filepath;
		}

		void* DLLReader::getProc(const char* name) const{
			RAINDROP_profile_function();
			void* proc = (void*)GetProcAddress((::HMODULE)_dll, (LPCSTR)name);

			if (proc == nullptr){
				::DWORD error_code = ::GetLastError();
				::LPSTR error_message = NULL;

				::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					error_code,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPSTR)&error_message,
					0,
					NULL);
				
				RAINDROP_log(ERROR, IO, "failed to get process named \"%s\" in \"%s\" : %s", name, _filepath, error_message);
				::LocalFree(error_message);
			}
			return proc;
		}

	#else
		DLLReader::DLLReader(const char* filepath) : _filepath{filepath}{
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