#include "PlanetComponent.hpp"
#include "../Editor/ImGuiHelper.hpp"

namespace Planet{
    void PlanetComponent::RegisterComponent(Editor::Editor& editor){
        editor.registerComponent({
            .name = "Planet",
            .add = [](Editor::Entity& e) -> std::expected<void, Raindrop::Error> {
                try{
                    e.emplace<PlanetComponent>();
                } catch (const std::exception& e){
                    return {}; // TODO: setup error codes
                }
                return {};
            },
            .has = [](Editor::Entity& e) -> bool {
                return e.hasAny<PlanetComponent>();
            },
            .drawUI = [](Editor::Entity& e) -> void {
                auto& planet = e.get<PlanetComponent>();

                if (planet.id != INVALID_PLANET_ID){
                    ImGui::TextDisabled("PlanetID : %d", planet.id);
                } else {
                    ImGui::TextDisabled("PlanetID : NONE");
                }

                int maxLOD = planet.maxLOD;
                
                if (ImGui::SliderInt("Max LOD", &maxLOD, 0, 16)){
                    planet.maxLOD = static_cast<uint8_t>(maxLOD);
                }

                ImGui::DragFloat("radius", &planet.radius, 1.f, 0.f, +INFINITY);

                if (auto result = ImGuiHelper::EntitySelector(Editor::Entity(e.scene(), planet.lodAnchor), "LOD Anchor")){
                    planet.lodAnchor = result->getHandle();
                }
            }
        });
    }
}