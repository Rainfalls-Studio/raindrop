#include "Core/IO/Module.hpp"
#include "Core/Debug/profiler.hpp"
#include "Core/Debug/logger.hpp"

#define RAINDROP_submodule_get(fnc) __RAINDROP_##fnc##_getContext
#define RAINDROP_submodule_set(fnc) __RAINDROP_##fnc##_setContext
#define str(a) #a
#define to_str(a) str(a)

#define RAINDROP_set_context(type, submodule){																						\
	using __FncType = void(*)(type*); 																								\
	__FncType __fnc = nullptr;		    																							\
	try{																															\
		__fnc = (__FncType)_dll.getProc(to_str(RAINDROP_submodule_set(submodule)));													\
	} catch (const std::exception &){																								\
		throw std::runtime_error("missing module function, cannot find " + std::string(to_str(RAINDROP_submodule_set(submodule))));	\
	}																																\
	__fnc(RAINDROP_submodule_get(submodule)());																						\
}

namespace Raindrop::Core::IO{
	RAINDROP_API Module::Module(const std::filesystem::path& path) : _dll(path){
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

		try{
			InitModuleFnc initModule = (InitModuleFnc)_dll.getProc("initialize");
			return initModule();
		} catch (const std::exception& e){
			RAINDROP_log(INFO, IO, "the module does not have a initialize function");
			return true;
		}
	}

	RAINDROP_API void Module::shutdown(){
		RAINDROP_profile_function();
		using ShutdownModuleFnc = bool(*)(void);

		try{
			ShutdownModuleFnc shutdownModule = (ShutdownModuleFnc)_dll.getProc("shutdown");
			shutdownModule();
		} catch (const std::exception& e){
			RAINDROP_log(INFO, IO, "the module does not have a shutdown function");
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

	RAINDROP_API const std::filesystem::path& Module::filepath() const{
		RAINDROP_profile_function();
		return _dll.filepath();
	}

	RAINDROP_API void* Module::getFnc(const char* name) const{
		RAINDROP_profile_function();
		return _dll.getProc(name);
	}
}