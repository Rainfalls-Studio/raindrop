#pragma once

#include <Raindrop/Raindrop.hpp>

class CameraUpdateBehavior : public Raindrop::Scene::IBehavior{
    public:
        CameraUpdateBehavior();

        virtual void initialize(Raindrop::Engine& engine, Raindrop::Scene::Scene& scene) override{
            auto eventManager = engine.getModuleManager().getModuleAs<Raindrop::Event::EventModule>("Event");
            // eventManager->getManager().subscribe<Raindrop::Window::>()

        }

        inline virtual void execute() override{

        }

};