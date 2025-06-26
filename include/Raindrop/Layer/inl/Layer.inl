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
        addProperty(std::static_pointer_cast<Property>(property));
    }
}