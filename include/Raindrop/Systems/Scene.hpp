#pragma once

#include "../System.hpp"

namespace Raindrop::Systems{
    class Scene : public System{
        public:
            Scene();
            virtual ~Scene() override;

            virtual void onFrame() override;
    };
}