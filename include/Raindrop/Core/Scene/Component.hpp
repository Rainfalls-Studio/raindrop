#ifndef __RAINDROP_CORE_SCENE_COMPONENT_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENT_HPP__

#include <Raindrop/Core/Scene/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Raindrop::Core::Scene{
	class Component{
		public:
			Component() = default;
			virtual ~Component() = default;

			virtual void UI(){};
	};

	namespace Components{
		class Tag : public Component{
			public:
				Tag() = default;
				virtual ~Tag() override = default;

				std::string name = "Entity";
		};

		class Transform : public Component{
			public:
				Transform() = default;
				virtual ~Transform() override = default;
				glm::vec3 translation;
				glm::vec3 scale;
				glm::vec3 rotation;

				virtual void UI() override{
					if (ImGui::TreeNode("Translation")){
						ImGui::DragFloat3("translation", reinterpret_cast<float*>(&translation));
						ImGui::DragFloat3("scale", reinterpret_cast<float*>(&scale));
						ImGui::DragFloat3("rotation", reinterpret_cast<float*>(&rotation));
						ImGui::TreePop();
					}
				}

		};

		class Hierachy : public Component{
			public:
				Hierachy() = default;

				virtual ~Hierachy() override = default;

				std::list<EntityID> childs;
				EntityID parent = INVALID_ENTITY_ID;
		};

		class Camera : public Component{
			public:
				static constexpr int ORTHOGRAPHIC_MODE = 0;
				static constexpr int PERSPECTIVE_MODE = 1;

				Camera(){

				}

				virtual ~Camera() override = default;

				glm::mat4 projection{1.f};
				glm::mat4 view{1.f};
				glm::mat4 viewProjection{1.f};

				virtual void UI() override{

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

				void update(const Transform& transform){
					glm::mat4 transformMat = glm::translate(glm::mat4(1.f), transform.translation) * glm::rotate(glm::mat4(1.f), glm::pi<float>(), transform.rotation);
					view = glm::inverse(transformMat);
					viewProjection = projection * view;
				}
		};
	}
}

#endif