#pragma once

#include "../Layer.hpp"
#include <memory>
#include <type_traits>

namespace Raindrop::Layer{
    template<typename T>
    inline std::shared_ptr<T> Layer::getProperty(){
        static_assert(std::is_base_of<Property, T>::value, "T must be derived from Raindrop::Layer::Property");
        return std::static_pointer_cast<T>(getProperty(typeid(T)));
    }

    template<typename T>
    void Layer::addProperty(std::shared_ptr<T> property){
        static_assert(std::is_base_of<Property, T>::value, "T must be derived from Raindrop::Layer::Property");
        addProperty(typeid(T), std::static_pointer_cast<Property>(property));
    }

    template<typename T>
    void Layer::removeProperty(){
        removeProperty(typeid(T));
    }

    template<typename T>
    std::shared_ptr<T> Layer::emplaceProperty(){
        static_assert(std::is_base_of<Property, T>::value, "T must be derived from Raindrop::Layer::Property");

        std::shared_ptr<T> property = std::make_shared<T>();
        addProperty(property);

        return property;
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> Layer::emplaceProperty(Args... args){
        static_assert(std::is_base_of<Property, T>::value, "T must be derived from Raindrop::Layer::Property");

        std::shared_ptr<T> property = std::make_shared<T>(std::forward<Args>(args)...);
        addProperty(property);

        return property;
    }
}