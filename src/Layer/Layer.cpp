#include "Raindrop/Layer/Layer.hpp"

namespace Raindrop::Layer{
    Layer::Layer(){}

    std::shared_ptr<Property> Layer::getProperty(std::type_index type){
        auto it = _properties.find(type);

        if (it == _properties.end()){
            return nullptr;
        }

        auto prop = it->second.lock();

        if (!prop){
            _properties.erase(it);
            return nullptr;
        }

        return prop;
    }

    void Layer::addProperty(std::type_index type, std::shared_ptr<Property> property){
        _properties[type] = property;
    }
}