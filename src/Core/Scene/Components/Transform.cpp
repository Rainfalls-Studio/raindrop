#include <Raindrop/Core/Scene/Components/Transform.hpp>

namespace Raindrop::Core::Scene::Components{
	void Transform::UI(EngineContext& context){
		if (ImGui::TreeNode("Translation")){
			ImGui::DragFloat3("translation", reinterpret_cast<float*>(&translation));
			ImGui::DragFloat3("scale", reinterpret_cast<float*>(&scale));
			ImGui::DragFloat3("rotation", reinterpret_cast<float*>(&rotation));
			ImGui::TreePop();
		}
	}
}