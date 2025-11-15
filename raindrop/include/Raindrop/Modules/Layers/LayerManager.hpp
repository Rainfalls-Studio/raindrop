#pragma once

#include <memory>
#include "Layer.hpp"
#include "Context.hpp"

namespace Raindrop::Layers{
    class LayerManager{
        public:
            LayerManager();

            Layer* operator->(){
                return _root.get();
            }

        private:
            Context _ctx;
            std::shared_ptr<Layer> _root;

            void setupContext();
            void createRoot();
    };
}