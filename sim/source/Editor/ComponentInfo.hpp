#pragma once

#include <functional>
#include <Raindrop/Raindrop.hpp>

namespace Editor{
    struct ComponentInfo{
        using AddFnc = std::expected<void, Raindrop::Error>(Raindrop::Scene::Entity&);
        using RemoveFnc = std::expected<void, Raindrop::Error>(Raindrop::Scene::Entity&);
        using HasFnc = bool(Raindrop::Scene::Entity&);
        using UiFnc = void(Raindrop::Scene::Entity&);

        std::string name;

        std::function<AddFnc> add;
        std::function<RemoveFnc> remove; // optional
        std::function<HasFnc> has;
        std::function<UiFnc> drawUI;
    };
}