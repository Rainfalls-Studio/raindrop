#include "Raindrop/Layer/Layer.hpp"

namespace Raindrop::Layer{
    Layer::Layer() :
        _manager{nullptr},
        _handle{INVALID_LAYER_HANDLE}
    {}

    Layer::Layer(Manager* manager, LayerHandle handle) :
        _manager{manager},
        _handle{handle}
    {}

    Layer::Layer(const Layer& other) :
        _manager{other._manager},
        _handle{other._handle}
    {}

    Layer::Layer(Layer&& other) :
        _manager{std::move(other._manager)},
        _handle{std::move(other._handle)}
    {}

    Layer& Layer::operator=(Layer&& other){
        _manager = other._manager;
        _handle = other._handle;
        return *this;
    }

    Layer& Layer::operator=(const Layer& other){
        _manager = std::move(other._manager);
        _handle = std::move(other._handle);
        return *this;
    }

    bool Layer::isValid() const noexcept{
        return _handle != INVALID_LAYER_HANDLE && _manager != nullptr;
    }

    LayerHandle Layer::getHandle() const noexcept{
        return _handle;
    }

    Manager* Layer::getManager() const noexcept{
        return _manager;
    }
}