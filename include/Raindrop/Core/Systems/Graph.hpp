#pragma once

#include <memory>
#include <list>
#include <unordered_map>

#include "InitNode.hpp"

namespace Raindrop::Systems{
    struct RichNode{
        std::shared_ptr<InitNode> node;
        std::list<std::string> dependencies = {};
        std::list<std::string> dependents = {};
    };

    using NodeGraph = std::unordered_map<std::string, RichNode>;
}