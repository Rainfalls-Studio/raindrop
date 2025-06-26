#pragma once

#include <typeindex>
#include <memory>
#include <unordered_map>
#include "Property.hpp"

namespace Raindrop::Layer{
    class Layer{
        public:
            Layer();

            std::shared_ptr<Property> getProperty(std::type_index type);

            template<typename T>
            inline std::shared_ptr<T> getProperty();

            void addProperty(std::type_index type, std::shared_ptr<Property> property);

            template<typename T>
            void addProperty(std::shared_ptr<T> property);

        private:
            std::unordered_map<std::type_index, std::weak_ptr<Property>> _properties;
    };
}

#include "inl/Layer.inl"