#pragma once

#include "HookResult.hpp"
#include <functional>

namespace Raindrop::Scheduler{
    enum class Phase {
        PRE_EVENT,
        EVENT,
        POST_EVENT,
        PRE_UPDATE,
        UPDATE,
        POST_UPDATE,
        PRE_RENDER_SETUP,
        PRE_RENDER,
        RENDER,
        PRE_GUI,
        GUI,
        POST_GUI,
        POST_RENDER,
    };

    struct Hook {
        Phase phase;
        std::string name;
        std::function<HookResult()> fn;
    };
}