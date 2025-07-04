#pragma once

namespace Raindrop{
    class System{
        public:
            virtual ~System() = default;
            
            virtual void onFrame() {}
    };
}