#pragma once

#include "Editor.hpp"

namespace ImGuiHelper{
    std::optional<Editor::Entity> EntitySelector(const Editor::Entity& entity, const char* name, bool dropSource = true, bool dropTarget = true);
};
