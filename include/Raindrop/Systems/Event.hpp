#pragma once

#include "../System.hpp"

namespace Raindrop::Systems{
    class Event : public System{
        public:
            Event();
            virtual ~Event() override;

            virtual void onFrame() override;
    };
}