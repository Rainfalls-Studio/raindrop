#pragma once

#include <memory>
#include "Layer.hpp"
#include "Context.hpp"

namespace Layers{
    class LayerManager{
        public:
            LayerManager(std::shared_ptr<spdlog::logger> logger);

            inline Layer* operator->(){
                return _root.get();
            }

            inline std::shared_ptr<Layer> get(){
                return _root;
            }

        private:
            Context _ctx;
            std::shared_ptr<Layer> _root;

            void setupContext(std::shared_ptr<spdlog::logger> logger);
            void createRoot();
    };
}