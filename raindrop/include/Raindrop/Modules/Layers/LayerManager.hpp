#pragma once

#include <memory>
#include "Layer.hpp"
#include "Context.hpp"

namespace Raindrop::Layers{
    class LayerManager{
        public:
            LayerManager();

            inline Layer* operator->(){
                return _root.get();
            }

            inline Layer& get(){
                return *_root;
            }

        private:
            Context _ctx;
            std::shared_ptr<Layer> _root;

            void setupContext();
            void createRoot();
    };
}