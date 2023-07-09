#include <Raindrop/Core/Scene/Components/Transform.hpp>

namespace Raindrop::Core::Scene::Components{
	void Transform::UI(EngineContext& context){
		if (ImGui::TreeNode("Translation")){
			ImGui::DragFloat3("translation", reinterpret_cast<float*>(&translation));
			ImGui::DragFloat3("scale", reinterpret_cast<float*>(&scale), 0.01);
			ImGui::DragFloat3("rotation", reinterpret_cast<float*>(&rotation), 0.01);
			ImGui::TreePop();
		}
	}
}