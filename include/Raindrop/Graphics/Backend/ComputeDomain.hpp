#pragma once

#include "Raindrop/Graphics/Backend/CommandList.hpp"

namespace Raindrop::Graphics::Backend{
    class ComputeDomain : public CommandList::Domain{
        public:
            virtual ~ComputeDomain() override = default;
            
            virtual void dispatch() = 0;
            virtual void distatchIndirect() = 0;
    };
}