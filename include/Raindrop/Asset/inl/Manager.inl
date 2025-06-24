#pragma once
#include "../Manager.hpp"
#include "../Factory.hpp"


namespace Raindrop::Asset{
	template<typename Asset, typename T, typename... Args>
	std::shared_ptr<T> Manager::emplaceFactory(Args&&... args){
		static_assert(std::is_base_of<Factory, T>::value, "The factory MUST be a derived class of Factory");
		std::shared_ptr<T> factory = std::make_shared<T>(std::forward<Args>(args)...);
		insertFactory(typeid(Asset), std::static_pointer_cast<Factory>(factory));
		return factory;
	}

	template<typename T>
	std::shared_ptr<T> Manager::loadOrGet(const std::filesystem::path& path){
		static_assert(std::is_base_of<Asset, T>::value, "The asset MUST be a derived class of Asset");
		return std::static_pointer_cast<T>(loadOrGet(typeid(T), path));
	}


	template<typename T>
	inline std::shared_ptr<T> Manager::getFactory(){
		static_assert(std::is_base_of<Factory, T>::value, "The factory MUST be a derived class of Factory");
		return std::static_pointer_cast<T>(getFactory(typeid(T)));
	}
}