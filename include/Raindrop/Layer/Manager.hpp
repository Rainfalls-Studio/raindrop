#pragma once

#include <entt/entt.hpp>

namespace Raindrop::Layer{
    using LayerHandle = uint32_t;
    static constexpr LayerHandle INVALID_LAYER_HANDLE = ~static_cast<LayerHandle>(0);

    // Wrapper
    class Layer;

    class Manager{
        public:
            Manager();
            ~Manager();

            Layer createLayer();
            Layer getLayer(LayerHandle handle);

            entt::basic_registry<LayerHandle>& getRegistry() noexcept;
            
        private:
            entt::basic_registry<LayerHandle> _layers;
    };
}