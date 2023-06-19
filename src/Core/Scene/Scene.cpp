#include <Raindrop/Core/Scene/Scene.hpp>
#include <Raindrop/Core/Scene/Entity.hpp>
#include <Raindrop/Core/Scene/ComponentRegistry.hpp>
#include <Raindrop/Core/Scene/ComponentManager.hpp>
#include <Raindrop/Core/Scene/EntityManager.hpp>
#include <Raindrop/Core/Scene/EntityComponentsRegistry.hpp>
#include <Raindrop/Core/Scene/Component.hpp>

namespace Raindrop::Core::Scene{
	Scene::Scene(uint32_t entityCount, uint32_t componentCount){
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Core.Scene");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		CLOG(INFO, "Engine.Core.Scene") << "Creating new scene...";
		_componentRegistry = std::make_unique<ComponentRegistry>(componentCount);
		_entityComponentsRegistry = std::make_unique<EntityComponentsRegistry>(entityCount, componentCount);
		_entityManager = std::make_unique<EntityManager>(entityCount);
		CLOG(INFO, "Engine.Core.Scene") << "Scene created with success !";

		registerComponent<Components::Tag>(entityCount);
		registerComponent<Components::Transform>(entityCount);
		registerComponent<Components::Hierachy>(entityCount);
		registerComponent<Components::Camera>(1);

		_root = createEntity();
	}

	Scene::~Scene(){
		CLOG(INFO, "Engine.Core.Scene") << "Destroying scene...";
		_componentRegistry.reset();
		_entityComponentsRegistry.reset();
		_entityManager.reset();
		CLOG(INFO, "Engine.Core.Scene") << "Scene destroyed with success !";
	}

	uint32_t Scene::maxEntityCount() const{
		return _entityManager->size();
	}

	uint32_t Scene::maxComponentCount() const{
		return _componentRegistry->size();
	}

	uint32_t Scene::usedEntityCount() const{
		return _entityManager->usedSize();
	}

	uint32_t Scene::usedComponentCount() const{
		return _componentRegistry->usedSize();
	}

	ComponentID Scene::registerComponent(uint32_t componentSize, size_t typeID, uint32_t maxCount, ConstructorPtr constructor, DestructorPtr destructor){
		return _componentRegistry->registerComponent(componentSize, typeID, maxCount, constructor, destructor);
	}
	
	void Scene::unregisterComponent(ComponentID id){
		_componentRegistry->unregisterComponent(id);
	}

	ComponentID Scene::getComponentID(size_t typeID){
		return _componentRegistry->getComponentID(typeID);
	}
	
	EntityID Scene::createEntity(){
		EntityID entity = _entityManager->createEntity();
		createComponent<Components::Transform>(entity);
		createComponent<Components::Hierachy>(entity);
		createComponent<Components::Tag>(entity);

		return entity;
	}

	void Scene::destroyEntity(EntityID ID){
		for (uint32_t i=0; i<_entityComponentsRegistry->componentCount(); i++){
			auto& component = _entityComponentsRegistry->get(ID, i);
			if (component != INVALID_COMPONENT_HANDLE_ID){
				_componentRegistry->destroyHandle(i, component, ID);
			}
			component = INVALID_COMPONENT_HANDLE_ID;
		}
		
		_entityManager->destroyEntity(ID);
	}

	bool Scene::hasComponent(EntityID entity, ComponentID component){
		ComponentHandleID handleID = _entityComponentsRegistry->get(entity, component);
		return handleID != INVALID_COMPONENT_HANDLE_ID;
	}

	void* Scene::getComponent(EntityID entity, ComponentID component){
		ComponentHandleID handleID = _entityComponentsRegistry->get(entity, component);
		if (handleID == INVALID_COMPONENT_HANDLE_ID){
			throw std::runtime_error("the entity does not have this component");
		}
		return _componentRegistry->getComponent(component, handleID);
	}

	void* Scene::createComponent(EntityID entity, ComponentID component){
		ComponentHandleID handle = _componentRegistry->createHandle(component, entity);
		_entityComponentsRegistry->set(entity, component, handle);
		void* componentPtr = getComponent(entity, component);
		return componentPtr;
	}

	void Scene::destroyComponent(EntityID entity, ComponentID component){
		ComponentHandleID& handle = _entityComponentsRegistry->get(entity, component);
		_componentRegistry->destroyHandle(component, handle, entity);
		handle = INVALID_COMPONENT_HANDLE_ID;
	}

	bool Scene::componentRegistred(size_t typeID){
		return _componentRegistry->registred(typeID);
	}

	EntityID Scene::root() const{
		return _root;
	}

	std::list<EntityID>& Scene::componentEntities(ComponentID component){
		return _componentRegistry->getManager(component)->entities();
	}
	
	bool drawEntityNode(Entity entity, EntityID& selection){
		const ImGuiStyle& style = ImGui::GetStyle();
		ImGuiStorage* storage = ImGui::GetStateStorage();

		bool* isTextEditActive = storage->GetBoolRef(ImGui::GetID("textEdit"));
		bool* opened = storage->GetBoolRef(ImGui::GetID("opened"));
		static char inputBuffer[256];
	
		if (ImGui::ArrowButton("##expend_button", *opened ? ImGuiDir_Down : ImGuiDir_Right)){
			*opened = !(*opened);
		}
		ImGui::SameLine();

		ImVec2 pos = ImGui::GetCursorPos();
		ImGui::SetCursorPosX(0);
		if (ImGui::InvisibleButton("##button", ImVec2(ImGui::GetWindowWidth(), 15))){
			selection = entity.id();
		}
		ImGui::SetCursorPos(pos);

		if (ImGui::IsItemHovered()){
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
				*isTextEditActive = true;
			}
		}

		if (*isTextEditActive){
			ImGui::SetKeyboardFocusHere();
			if (ImGui::InputText("##editText", inputBuffer, sizeof(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)){
				*isTextEditActive = false;
				entity.tag().name = std::string(inputBuffer);
			}
		} else {
			ImGui::Text("%s", entity.tag().name.c_str());
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				strncpy(inputBuffer, entity.tag().name.c_str(), sizeof(inputBuffer));
				*isTextEditActive = true;
			}
		}

		return *opened;
	}

	EntityID drawEntity(Entity entity, EntityID& selectedEntity){
		ImGui::PushID(entity.id());
		EntityID selected = INVALID_ENTITY_ID;

		if (drawEntityNode(entity, selected)){
			ImGui::Indent(15);
			for (auto c : entity){
				EntityID newSelection = drawEntity(c, selected);
				if (newSelection != INVALID_ENTITY_ID){
					selected = newSelection;
				}
			}
			ImGui::Indent(-15);
		}
		
		ImGui::PopID();
		return selected == INVALID_ENTITY_ID ? selectedEntity : selected;
	}

	void entityButtons(Entity entity){
		if (ImGui::Button("Add childs")){
			entity.createChild();
		}
	}

	EntityID Scene::UI(EntityID selectedEntity){
		if (selectedEntity == INVALID_ENTITY_ID) ImGui::BeginDisabled();
		entityButtons(Entity(selectedEntity, this));
		if (selectedEntity == INVALID_ENTITY_ID) ImGui::EndDisabled();

		Entity base = Entity(root(), this);
		return drawEntity(base, selectedEntity);
	}

	#define draw_component(component) if (entity.hasComponent<component>()) {entity.getComponent<component>().UI();}
	#define add_component(component) if (!entity.hasComponent<component>()) {if (ImGui::MenuItem(#component)){entity.createComponent<component>();}}

	void Scene::componentsUI(EntityID id){
		Entity entity = Entity(id, this);
		ImGui::Text("LUID: 0x%x", entity.id());
		ImGui::Text("GUID: none");

		if (ImGui::Button("add component")){
			ImGui::OpenPopup("add component");
		}

		if (ImGui::BeginPopup("add component")){
			add_component(Components::Transform);
			add_component(Components::Camera);
			ImGui::EndPopup();
		}

		draw_component(Components::Transform);
		draw_component(Components::Camera);
	}
}