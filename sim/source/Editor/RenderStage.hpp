#pragma once

#include "Editor.hpp"

namespace Editor{
    class RenderStage : public Raindrop::Scheduler::IStage{
        public:
            RenderStage(const std::shared_ptr<Editor>& editor);
            virtual ~RenderStage() override = default;

            virtual const char* name() const override;

            virtual void shutdown() override;
            virtual Raindrop::Scheduler::StageResult execute() override;
        
        private:
            std::weak_ptr<Editor> _editor;
            ImGuiTextFilter _sceneGraphFilter;

            void dockspace();
            void renderScene(Raindrop::Scene::Scene& scene, Editor& editor);
            void renderEntity(Entity entity, Editor& editor);

            void componentPanel(Editor& editor);
            std::string getEntityName(Entity entity);
    };
}