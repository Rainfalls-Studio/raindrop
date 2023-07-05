#include <Raindrop/Core/Scene/Components/Model.hpp>
#include <Raindrop/Graphics/Model.hpp>

namespace Raindrop::Core::Scene::Components{
	void Model::UI(EngineContext& context){
		ImGui::PushID(this);

		ImGuiStorage* storage = ImGui::GetStateStorage();

		if (ImGui::TreeNode("Model")){
			static char buff[255] = "";

			if (ImGui::InputText("model", buff, sizeof(buff), ImGuiInputTextFlags_EnterReturnsTrue)){
				try{
					_model = context.assetManager.loadOrGet<Graphics::Model>(buff);
				} catch (const std::exception& e){
					printf("%s\n", e.what());
				}
			}

			ImGui::TreePop();
		}
		ImGui::PopID();
	}
	
}