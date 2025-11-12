#include "ImGuiHelper.hpp"

namespace ImGuiHelper{
    using Raindrop::Scene::Entity;

    std::optional<Entity> EntitySelector(const Entity& entity, const char* label, bool dropSource, bool dropTarget){
        std::optional<Entity> result;

        auto id = entity.getHandle();
        ImGui::PushID(static_cast<int>(id));

        ImGui::TextUnformatted(label);
        ImGui::SameLine();

        std::string name;
        const bool entityValid = entity.isValid();

        if (entityValid){
            if (entity.hasAny<Raindrop::Components::Tag>()){
                name = entity.get<Raindrop::Components::Tag>().tag;
            } else {
                name = "Unnamed entity (#" + std::to_string(id) + ")";
            }
        } else {
            name = "None";
        }

        if (ImGui::Button(name.c_str())){
            // clicked = true;
        }

        if (dropSource && entityValid && ImGui::BeginDragDropSource()){
            ImGui::SetDragDropPayload("ENTITY", &entity, sizeof(Entity));
            ImGui::Text("Entity %s", name.c_str());
            ImGui::EndDragDropSource();
        }

        if (dropTarget && ImGui::BeginDragDropTarget()){
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY")) {
                Entity dropped = *reinterpret_cast<Entity*>(payload->Data);
                result = dropped;
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::PopID();
        return result;
    }
}