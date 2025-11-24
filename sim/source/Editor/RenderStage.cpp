#include "RenderStage.hpp"

using Raindrop::Components::Tag;
using Raindrop::Components::Hierarchy;

namespace Editor{

    static bool open = true;

    RenderStage::RenderStage(const std::shared_ptr<Editor>& editor) : _editor{editor}{

    }

    const char* RenderStage::name() const{
        return "Editor - Render Stage";
    }

    void RenderStage::shutdown(){

    }

    Raindrop::Scheduler::StageResult RenderStage::execute(){
        auto editor = _editor.lock();
        if (!editor){
            return Raindrop::Scheduler::StageResult::Skip("No editor");
        }

        dockspace();

        auto scenes = editor->scenes();

        if (ImGui::Begin("Scenes")){

            if (scenes.empty()){
                static constexpr const char* NO_SCENE_TEXT = "No Scene";

                auto windowSize = ImGui::GetWindowSize();
                auto textWidth = ImGui::CalcTextSize(NO_SCENE_TEXT);

                ImGui::SetCursorPos({
                    (windowSize.x - textWidth.x) * 0.5f,
                    (windowSize.y - textWidth.y) * 0.5f
                });

                ImGui::Text(NO_SCENE_TEXT);
            } else {
                for (auto& scene : scenes){
                    if (ImGui::Selectable("Scene - {}", open)){
                        open = !open;
                    }
                }
            }
        }

        ImGui::End();

        for (auto scene : scenes){
            renderScene(*scene, *editor);
        }

        componentPanel(*editor);

        return Raindrop::Scheduler::StageResult::Continue();
    }

    void RenderStage::dockspace(){
        constexpr ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoBackground;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
        

        ImGui::Begin("DockspaceHost", nullptr, window_flags);

        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        ImGui::End();
    }

    void RenderStage::renderEntity(Entity entity, Editor& editor){
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        ImGui::PushID(static_cast<int>(entity.getHandle()));

        ImGuiTreeNodeFlags tree_flags = 
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | // Standard opening mode as we are likely to want to add selection afterwards
            ImGuiTreeNodeFlags_NavLeftJumpsBackHere |  // Left arrow support
            ImGuiTreeNodeFlags_SpanFullWidth;         // Span full width for easier mouse reach
            // ImGuiTreeNodeFlags_DrawLinesToNodes;      // Always draw hierarchy outlines

        if (entity == editor.selected())
            tree_flags |= ImGuiTreeNodeFlags_Selected;

        Hierarchy* hierarchy = nullptr;

        if (entity.hasAny<Hierarchy>()){
            hierarchy = &entity.get<Hierarchy>();

            if (hierarchy->children.empty()){
                tree_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
            }
        }

        auto name = getEntityName(entity);

        bool open = ImGui::TreeNodeEx("", tree_flags, "%s", name.c_str());

        if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
            editor.select(entity);
        }

        if (hierarchy){
            if (!ImGui::GetDragDropPayload() && ImGui::BeginDragDropSource()) {

                ImGui::SetDragDropPayload("ENTITY", &entity, sizeof(Entity));

                ImGui::Text("Entity : %d", entity.getHandle());
                ImGui::EndDragDropSource();
            }
                
            if (ImGui::BeginDragDropTarget()) {

                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY")){
                    assert(payload->DataSize == sizeof(Entity));
                    Entity dragged = *reinterpret_cast<Entity*>(payload->Data);

                    auto& childHierarchy = dragged.get<Hierarchy>();

                    if (dragged != entity){
                        if (!hierarchy->isChildOf(*entity.scene(), dragged.getHandle()) &&
                            !childHierarchy.isChildOf(*entity.scene(), entity))
                        {
                            if (childHierarchy.parent != Raindrop::Scene::INVALID_ENTITY_HANDLE){
                                auto& parentHierarchy = entity.getScene()->getComponent<Hierarchy>(childHierarchy.parent);
                                parentHierarchy.children.erase(dragged.getHandle());
                            }

                            childHierarchy.parent = entity.getHandle();
                            hierarchy->children.insert(dragged.getHandle());

                            if (dragged.hasAny<Raindrop::Components::Transform>()){
                                dragged.get<Raindrop::Components::Transform>().dirty = true;
                            }
                        }
                    }
                }

                // Some processing...
                ImGui::EndDragDropTarget();
            } 
        }

        if (open){
            if (hierarchy){
                for (auto child : hierarchy->children){
                    renderEntity(Entity(entity.scene(), child), editor);
                }
                ImGui::TreePop();
            }
        }

        ImGui::PopID();
    }

    void RenderStage::renderScene(Raindrop::Scene::Scene& scene, Editor& editor){
        if (!open) return;

        if (ImGui::Begin("Scene - {}", &open)){
            if (ImGui::BeginTabBar("tab")){
                if (ImGui::BeginTabItem("Content")){
                    auto& registry = scene.registry();
                    auto entities = registry.view<Raindrop::Scene::EntityHandle>();

                    if (auto size = entities->size(); size == 0){
                        ImGui::Text("No entities in the scene");
                    } else if (size == 1){
                        ImGui::Text("1 entity");
                    } else {
                        ImGui::Text("%ld entities", size);
                    }

                    ImGui::SeparatorText("Content");

                    if (ImGui::InputTextWithHint(
                            "##Filter",
                            "incl,-excl",
                            _sceneGraphFilter.InputBuf,
                            IM_ARRAYSIZE(_sceneGraphFilter.InputBuf),
                            ImGuiInputTextFlags_EscapeClearsAll))
                    {
                        _sceneGraphFilter.Build();
                    }

                    ImVec2 size = ImGui::GetWindowSize();
                    size.y -= ImGui::GetCursorPosY();

                    if (ImGui::BeginTable("##bg", 1, ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp, size)){

                        for (auto entity_id : entities){
                            auto entity = Raindrop::Scene::Entity(&scene, entity_id);
                            
                            auto name = getEntityName(entity);

                            if (entity.get<Hierarchy>().parent == Raindrop::Scene::INVALID_ENTITY_HANDLE){

                                if (_sceneGraphFilter.PassFilter(name.c_str())){
                                    renderEntity(entity, editor);
                                }
                            }
                        }

                        ImGui::EndTable();


                        if (ImGui::BeginDragDropTarget()){
                            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY")){
                                Entity dropped = *static_cast<Entity*>(payload->Data);
                                
                                if (dropped.hasAny<Hierarchy>()){
                                    auto& hierarchy = dropped.get<Hierarchy>();
                                    const bool hasParent = hierarchy.parent != Raindrop::Scene::INVALID_ENTITY_HANDLE;

                                    if (hasParent){
                                        auto& parentHierarchy = scene.getComponent<Hierarchy>(hierarchy.parent);
                                        parentHierarchy.children.erase(dropped.getHandle());
                                    }

                                    if (dropped.hasAny<Raindrop::Components::Transform>()){
                                        dropped.get<Raindrop::Components::Transform>().dirty = true;
                                    }

                                    hierarchy.parent = Raindrop::Scene::INVALID_ENTITY_HANDLE;
                                }
                            }
                            ImGui::EndDragDropTarget();
                        }
                    }
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Behaviors")){
                    // copy
                    auto behaviors = scene.behaviors();

                    if (ImGui::BeginTable("Behaviors", 1)){
                        for (auto behavior : behaviors){
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();

                            ImGui::Text("%s", behavior->name());
                            
                        }
                        ImGui::EndTable();
                    }
                    ImGui::EndTabItem();
                }

            }
            ImGui::EndTabBar();
        }
        
        ImGui::End();
    }

    std::string RenderStage::getEntityName(Entity entity){
        std::string name;

        if (entity.hasAny<Tag>()){
            const auto& tag = entity.get<Tag>();
            name = tag.tag;
        }

        if (name.empty()){
            name = "Unnamed (entity #" + std::to_string(entity.getHandle()) + ")";
        }

        return name;
    }


    void RenderStage::componentPanel(Editor& editor){
        
        if (ImGui::Begin("Components")){
            Entity selected = editor.selected();

            if (!selected){
                static constexpr const char* NO_SELECTION_TEXT = "No Selection";

                auto windowSize = ImGui::GetWindowSize();
                auto textWidth = ImGui::CalcTextSize(NO_SELECTION_TEXT);

                ImGui::SetCursorPos({
                    (windowSize.x - textWidth.x) * 0.5f,
                    (windowSize.y - textWidth.y) * 0.5f
                });

                ImGui::Text(NO_SELECTION_TEXT);
            } else {
                ImGui::Text("id: %08x", selected.getHandle());

                for (auto& component : editor.components()){
                    if (component.has(selected)){
                        if (ImGui::CollapsingHeader(component.name.c_str())){
                            ImGui::PushID(component.name.c_str());

                            component.drawUI(selected);
                            
                            ImGui::PopID();
                        }
                    }
                }
            }
        }
        ImGui::End();
    }

}