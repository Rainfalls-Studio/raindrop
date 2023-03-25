#include "Module.hpp"
#include "../Debug/profiler.hpp"
#include "../Debug/logger.hpp"

namespace Raindrop::Core::IO{
	Module::Module(const char* path) : _dll(path){
		RAINDROP_profile_function();

		using SetLogContextFnc = void(*)(const ::Raindrop::Core::Debug::Log::Logger&);
		SetLogContextFnc setLogContext = (SetLogContextFnc)_dll.getProc("__RAINDROP_log_setContext");
		
		if (setLogContext){
			setLogContext(__RAINDROP_log_getContext());
		} else {
			RAINDROP_log(WARNING, IO, "invalid module : raindrop is not properly included, the use of logs might not work \"%s\"", _dll.filepath());
		}

		_loaded = init();

		if (!_loaded){
			RAINDROP_log(ERROR, IO, "failed to initialize module \"%s\"", _dll.filepath());
		}
	}
	
	bool Module::init(){
		RAINDROP_profile_function();
		using InitModuleFnc = bool(*)(void);
		InitModuleFnc initModule = (InitModuleFnc)_dll.getProc("initialize");

		if (initModule){
			return initModule();
		}
		RAINDROP_log(WARNING, IO, "invalid module : missing initialize function \"%s\"", _dll.filepath());
		return false;
	}

	void Module::shutdown(){
		RAINDROP_profile_function();
		using ShutdownModuleFnc = bool(*)(void);
		ShutdownModuleFnc shutdownModule = (ShutdownModuleFnc)_dll.getProc("shutdown");

		if (shutdownModule){
			shutdownModule();
		} else {
			RAINDROP_log(WARNING, IO, "invalid module : missing shutdown function \"%s\"", _dll.filepath());
		}
	}

	Module::~Module(){
		RAINDROP_profile_function();
		shutdown();
	}

	bool Module::isOpen() const{
		RAINDROP_profile_function();
		return _dll.isOpen() && _loaded;
	}

	const char* Module::filepath() const{
		RAINDROP_profile_function();
		return _dll.filepath();
	}

	void* Module::getFnc(const char* name) const{
		RAINDROP_profile_function();
		return _dll.getProc(name);
	}
}