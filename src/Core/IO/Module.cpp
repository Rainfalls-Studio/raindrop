#include "Core/IO/Module.hpp"
#include "Core/Debug/profiler.hpp"
#include "Core/Debug/logger.hpp"

#define RAINDROP_submodule_get(fnc) __RAINDROP_##fnc##_getContext
#define RAINDROP_submodule_set(fnc) __RAINDROP_##fnc##_setContext
#define str(a) #a
#define to_str(a) str(a)

#define RAINDROP_set_context(type, submodule){																	\
	using __FncType = void(*)(type*);     																		\
	__FncType __fnc = (__FncType)_dll.getProc(to_str(RAINDROP_submodule_set(submodule)));						\
	if (__fnc){																									\
		__fnc(RAINDROP_submodule_get(submodule)());																								\
	} else {																									\
		RAINDROP_log(ERROR, IO, "missing module function : %s", to_str(RAINDROP_submodule_set(submodule)));		\
	}																											\
}

namespace Raindrop::Core::IO{
	RAINDROP_API Module::Module(const char* path) : _dll(path){
		RAINDROP_profile_function();

		RAINDROP_set_context(::Raindrop::Core::Debug::Log::__Logger, log);
		RAINDROP_set_context(::Raindrop::Core::Debug::Profiler::__Profiler, profile);

		_loaded = init();

		if (!_loaded){
			RAINDROP_log(ERROR, IO, "failed to initialize module \"%s\"", _dll.filepath());
		}
	}
	
	RAINDROP_API bool Module::init(){
		RAINDROP_profile_function();
		using InitModuleFnc = bool(*)(void);
		InitModuleFnc initModule = (InitModuleFnc)_dll.getProc("initialize");

		if (initModule){
			return initModule();
		}
		RAINDROP_log(WARNING, IO, "invalid module : missing initialize function \"%s\"", _dll.filepath());
		return false;
	}

	RAINDROP_API void Module::shutdown(){
		RAINDROP_profile_function();
		using ShutdownModuleFnc = bool(*)(void);
		ShutdownModuleFnc shutdownModule = (ShutdownModuleFnc)_dll.getProc("shutdown");

		if (shutdownModule){
			shutdownModule();
		} else {
			RAINDROP_log(WARNING, IO, "invalid module : missing shutdown function \"%s\"", _dll.filepath());
		}
	}

	RAINDROP_API Module::~Module(){
		RAINDROP_profile_function();
		shutdown();
	}

	RAINDROP_API bool Module::isOpen() const{
		RAINDROP_profile_function();
		return _dll.isOpen() && _loaded;
	}

	RAINDROP_API const char* Module::filepath() const{
		RAINDROP_profile_function();
		return _dll.filepath();
	}

	RAINDROP_API void* Module::getFnc(const char* name) const{
		RAINDROP_profile_function();
		return _dll.getProc(name);
	}
}