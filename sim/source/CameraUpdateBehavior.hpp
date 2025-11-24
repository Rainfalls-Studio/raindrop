#pragma once

#include <Raindrop/Raindrop.hpp>

class CameraUpdateBehavior : public Raindrop::Scene::IBehavior{
    public:
        CameraUpdateBehavior(){}

        virtual void initialize(Raindrop::Engine& engine, Raindrop::Scene::Scene& scene) override{
            _scene = &scene;
            auto eventManager = engine.getModuleManager().getModuleAs<Raindrop::Event::EventModule>("Event");
            // eventManager->getManager().subscribe<Raindrop::Window::>()
            _snapshotService = scene.getBehaviorIndex<Raindrop::Behaviors::FrameSnapshotService>();

            if (_snapshotService != Raindrop::Scene::INVALID_BEHAVIOR_ID){
                auto snapshotService = scene.getBehavior<Raindrop::Behaviors::FrameSnapshotService>(_snapshotService);

                _cameraSlot = snapshotService->registerSlot<Raindrop::Components::Camera::RenderData>();
            }
        }

        inline virtual void execute() override{
            using Raindrop::Components::Camera;
            using Raindrop::Components::Transform;

            auto service = _scene->getBehavior<Raindrop::Behaviors::FrameSnapshotService>(_snapshotService);
            if (!service) return;

            auto cameras = _scene->registry().view<Camera, Transform>();

            for (auto entity : cameras){
                const auto& [camera, transform] = cameras.get(entity);
                
                auto renderData = service->writeSlot<Camera::RenderData>(_cameraSlot);
                renderData->viewProjection = camera.getProjectionMatrix() * glm::inverse(transform.worldTransform);
            }
        }

        virtual const char* name() const override{
            return "Camera - Update";
        }
    
    private:
        Raindrop::Scene::Scene* _scene;
        Raindrop::Scene::BehaviorID _snapshotService;
        Raindrop::Behaviors::FrameSnapshotService::SlotID _cameraSlot;

};