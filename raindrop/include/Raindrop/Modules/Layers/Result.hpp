#pragma once

// #include "Layer.hpp"
#include <memory>

namespace Raindrop::Layers{
    class Layer;

    struct CompletedStatus{};

    struct RerouteStatus{
        std::weak_ptr<Layer> target;
    };

    struct Result{
        enum Type {
            CONTINUE,
            CONSUME,
            REROUTE,
            STOP_SIBLINGS
        } type = CONTINUE;

        std::variant<
            RerouteStatus
        > status;

        inline RerouteStatus& getReroute(){
            return std::get<RerouteStatus>(status);
        }

        static inline constexpr Result Continue(){
            return Result{Result::CONTINUE, {}};
        } 
    
        static inline constexpr Result Consume(){
            return Result{Result::CONSUME, {}};
        }

        static inline constexpr Result Reroute(const std::weak_ptr<Layer>& target){
            return Result{Result::REROUTE, RerouteStatus(target)};
        }

        static inline constexpr Result StopSiblings(){
            return Result{Result::STOP_SIBLINGS, {}};
        }
    };
}