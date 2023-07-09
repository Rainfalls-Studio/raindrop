#include <Raindrop/Core/Scene/Components/Camera.hpp>
#include <Raindrop/Core/Scene/Components/Transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Raindrop::Core::Scene::Components{
	Camera::Camera(){

	}

	Camera::~Camera(){

	}

	void Camera::UI(EngineContext& context){

		if (ImGui::TreeNode("Camera")){
			ImGuiStorage* storage = ImGui::GetStateStorage();
			int* mode = storage->GetIntRef(ImGui::GetID("mode"));

			ImGui::RadioButton("orthographic", mode, ORTHOGRAPHIC_MODE);
			ImGui::RadioButton("perspective", mode, PERSPECTIVE_MODE);

			if (*mode == ORTHOGRAPHIC_MODE){
				float* _left = storage->GetFloatRef(ImGui::GetID("left"));
				float* _right = storage->GetFloatRef(ImGui::GetID("right"));
				float* _top = storage->GetFloatRef(ImGui::GetID("top"));
				float* _bottom = storage->GetFloatRef(ImGui::GetID("bottom"));
				float* _near = storage->GetFloatRef(ImGui::GetID("near"));
				float* _far = storage->GetFloatRef(ImGui::GetID("far"));

				bool change = false;
				change |= ImGui::InputFloat("left", _left);
				change |= ImGui::InputFloat("right", _right);
				change |= ImGui::InputFloat("top", _top);
				change |= ImGui::InputFloat("bottom", _bottom);
				change |= ImGui::InputFloat("near", _near);
				change |= ImGui::InputFloat("far", _far);

				if (change){
					projection = glm::ortho(*_left, *_right, *_bottom, *_top, *_near, *_far);
				}
				
			} else if (*mode == PERSPECTIVE_MODE){

				float* _fov = storage->GetFloatRef(ImGui::GetID("fov"));
				float* _width = storage->GetFloatRef(ImGui::GetID("width"));
				float* _height = storage->GetFloatRef(ImGui::GetID("height"));
				float* _near = storage->GetFloatRef(ImGui::GetID("near"));
				float* _far = storage->GetFloatRef(ImGui::GetID("far"));

				bool change = false;
				change |= ImGui::InputFloat("fov", _fov);
				change |= ImGui::InputFloat("width", _width);
				change |= ImGui::InputFloat("height", _height);
				change |= ImGui::InputFloat("near", _near);
				change |= ImGui::InputFloat("far", _far);

				if (change){
					projection = glm::perspective(*_fov, *_width / *_height, *_near, *_far);
				}
			}

			ImGui::TreePop();
		}
	}

	void Camera::update(const Transform& transform){
		glm::mat4 rotationMatrix = glm::eulerAngleXYZ(transform.rotation.x, transform.rotation.y, transform.rotation.z);
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), transform.translation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), transform.scale);

		glm::mat4 transformMat = translationMatrix * rotationMatrix * scaleMatrix;
		view = glm::inverse(transformMat);
		viewProjection = projection * view;
	}
}