#pragma once

#include <memory>

namespace Raindrop{
    class Engine{
        public:
            Engine();

            static std::shared_ptr<Engine> Create();
        private:
            
    };
}