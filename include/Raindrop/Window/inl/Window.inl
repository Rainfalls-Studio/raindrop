#pragma once

#include "../Window.hpp"
#include <memory>
#include <ratio>
#include <stdexcept>
#include <type_traits>

namespace Raindrop::Window{
	template<typename T, typename... Args>
	inline std::shared_ptr<T> Window::addProperty(Args&&... args){
		static_assert(std::is_base_of<Property, T>::value, "The property type must be derived from Property");
		std::shared_ptr<T> property = std::make_shared<T>(*this, std::forward<Args>(args)...);
		addProperty(typeid(T), std::static_pointer_cast<Property>(property));
	}

	template<typename T>
	inline std::shared_ptr<T> Window::addProperty(){
		static_assert(std::is_base_of<Property, T>::value, "The property type must be derived from Property");
		std::shared_ptr<T> property = std::make_shared<T>(*this);
		addProperty(typeid(T), std::static_pointer_cast<Property>(property));
		return property;
	}

	template<typename T>
	inline std::shared_ptr<T> Window::getProperty(){
		static_assert(std::is_base_of<Property, T>::value, "The property type must be derived from Property");
		return std::dynamic_pointer_cast<T>(_properties.find(typeid(T))->second);
	}

	template<typename T>
	inline const std::shared_ptr<T> Window::getProperty() const{
		static_assert(std::is_base_of<Property, T>::value, "The property type must be derived from Property");
		return std::dynamic_pointer_cast<const T>(_properties.find(typeid(T))->second);
	}

	template<typename T>
	inline bool Window::hasProperty() const noexcept{
		static_assert(std::is_base_of<Property, T>::value, "The property type must be derived from Property");
		return _properties.find(typeid(T)) != _properties.end();
	}

	template<typename T>
	inline void Window::removeProperty() noexcept{
		static_assert(std::is_base_of<Property, T>::value, "The property type must be derived from Property");
		_properties.erase(typeid(T));
	}


	template<typename T>
	inline std::unique_ptr<T> Window::getSurfaceProvider(){
		static_assert(std::is_base_of<SurfaceProvider, T>::value, "T type must be derived from SurfaceProvider");

		auto base_ptr = getSurfaceProvider(T::API());
		T* derived_ptr = dynamic_cast<T*>(base_ptr.get());
		if (!derived_ptr) {
			throw std::runtime_error("Unsupported API");
			return nullptr;
		}
		base_ptr.release();  // release ownership from base_ptr
		return std::unique_ptr<T>(derived_ptr);
	}
}