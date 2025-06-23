#pragma once

#include "../Window.hpp"
#include <memory>
#include <type_traits>

namespace Raindrop::Window{
	template<typename T, typename... Args>
	inline std::shared_ptr<T> Window::addProperty(Args&&... args){
		static_assert(std::is_base_of_v<Property, T>, "The property type must be derived from Property");
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	inline std::shared_ptr<T> Window::addProperty(){
		static_assert(std::is_base_of_v<Property, T>, "The property type must be derived from Property");
		return std::make_shared<T>();
	}

	template<typename T>
	inline std::shared_ptr<T> Window::getProperty(){
		static_assert(std::is_base_of_v<Property, T>, "The property type must be derived from Property");
		return std::dynamic_pointer_cast<T>(_properties.find(typeid(T).hash_code())->second.lock());
	}

	template<typename T>
	inline const std::shared_ptr<T> Window::getProperty() const{
		static_assert(std::is_base_of_v<Property, T>, "The property type must be derived from Property");
		return std::dynamic_pointer_cast<const T>(_properties.find(typeid(T).hash_code())->second.lock());
	}

	template<typename T>
	inline bool Window::hasProperty() const noexcept{
		static_assert(std::is_base_of_v<Property, T>, "The property type must be derived from Property");
		return _properties.find(typeid(T).hash_code()) != _properties.end();
	}

	template<typename T>
	inline void Window::removeProperty() noexcept{
		static_assert(std::is_base_of_v<Property, T>, "The property type must be derived from Property");
		_properties.erase(typeid(T).hash_code());
	}
}