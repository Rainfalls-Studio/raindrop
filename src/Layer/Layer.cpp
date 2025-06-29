#include "Raindrop/Layer/Layer.hpp"
#include <stdexcept>

namespace Raindrop::Layer{
    Layer::Layer(){}

    std::shared_ptr<Property> Layer::getProperty(std::type_index type){
        auto it = _properties.find(type);

        if (it == _properties.end()){
            throw std::out_of_range("Property not found");
        }

        return it->second;
    }

    void Layer::addProperty(std::type_index type, std::shared_ptr<Property> property){
        _properties[type] = std::move(property);
    }


    void Layer::removeProperty(std::type_index type){
        _properties.erase(type);
    }
}