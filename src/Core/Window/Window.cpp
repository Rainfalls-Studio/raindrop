#include "Window.hpp"
#include "../Debug/logger.hpp"
#include "../Debug/profiler.hpp"

namespace Raindrop::Core::Window{
	Window::Window(const char* filepath) : _dll{filepath}{
		RAINDROP_profile_function();
		_loaded = false;

		if (!_dll.isOpen()){
			RAINDROP_log(ERROR, IO, "failed to load \"%s\" DLL window file", filepath);
			return;
		}

		read(filepath);
		if (!_loaded) return;
		_loaded = _initFnc();
	}

	Window::~Window(){
		RAINDROP_profile_function();
		if (_loaded) _shutdownFnc();
	}

	void Window::read(const char* filepath){
		RAINDROP_profile_function();
		_loaded = true;

		_initFnc = (InitFnc)_dll.getProc("initialize");
		_shutdownFnc = (ShutdownFnc)_dll.getProc("shutdown");
		_setTitleFnc = (SetTitleFnc)_dll.getProc("getTitle");
		_setSizeFnc = (SetSizeFnc)_dll.getProc("setSize");
		_setPositionFnc = (SetPositionFnc)_dll.getProc("setPosition");
		_getTitleFnc = (GetTitleFnc)_dll.getProc("getTitle");
		_getSizeFnc = (GetSizeFnc)_dll.getProc("getSize");
		_getPositionFnc = (GetPositionFnc)_dll.getProc("getPosition");
		_getAPIFnc = (GetAPIFnc)_dll.getProc("getAPI");

		_loaded = 
			_initFnc &&
			_shutdownFnc &&
			_setTitleFnc &&
			_setSizeFnc &&
			_setPositionFnc &&
			_getTitleFnc &&
			_getSizeFnc &&
			_getPositionFnc &&
			_getAPIFnc;
	}

	bool Window::loaded() const{
		return _loaded;
	}

	void Window::setTitle(const String& title){
		RAINDROP_profile_function();
		setTitle(title.str());
	}

	void Window::setTitle(const char* title){
		RAINDROP_profile_function();
		RAINDROP_assert(_setTitleFnc != nullptr);
		_setTitleFnc(title);
	}

	void Window::setSize(const Maths::vec2<uint32>& size){
		RAINDROP_profile_function();
		RAINDROP_assert(_setPositionFnc != nullptr);
		_setPositionFnc(size);
	}

	void Window::setPosition(const Maths::vec2<uint32>& position){
		RAINDROP_profile_function();
		RAINDROP_assert(_getPositionFnc != nullptr);
		_setPositionFnc(position);
	}

	const char* Window::getTitle() const{
		RAINDROP_profile_function();
		RAINDROP_assert(_getTitleFnc != nullptr);
		return _getTitleFnc();
	}

	Maths::vec2<uint32> Window::getSize() const{
		RAINDROP_profile_function();
		RAINDROP_assert(_getSizeFnc != nullptr);
		return _getSizeFnc();
	}

	Maths::vec2<uint32> Window::getPosition() const{
		RAINDROP_profile_function();
		RAINDROP_assert(_getPositionFnc != nullptr);
		return _getPositionFnc();
	}

	const char* Window::getAPI() const{
		RAINDROP_profile_function();
		RAINDROP_assert(_getAPIFnc != nullptr);
		return _getAPIFnc();
	}
}