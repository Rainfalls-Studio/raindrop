#pragma once

#include "Raindrop/Core/Modules/IModule.hpp"
#include "Raindrop/Modules/Window/WindowModule.hpp"
#include <VkBootstrap.h>

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Render{
    class RenderCoreModule : public Modules::IModule{
        public:
            RenderCoreModule();
            virtual ~RenderCoreModule() = default;

            virtual Modules::Result initialize(Modules::InitHelper& helper) override;
            virtual void shutdown() override;

            virtual Name name() const noexcept override;

            virtual Modules::DependencyList dependencies() const noexcept override;

            virtual Modules::Result dependencyReload(const Name& name) override;
            virtual Modules::Result dependencyShutdown(const Name& name) override;

        private:
            struct InitData{
                std::shared_ptr<Window::Window> window;
                vk::SurfaceKHR surface;
            };

            Engine* _engine;
            std::shared_ptr<Window::WindowModule> _window;

            vkb::Instance _instance;
            vkb::PhysicalDevice _physicalDevice;
            vkb::Device _device;

            vk::Instance _vkInstance;
            vk::PhysicalDevice _vkPhysicalDevice;
            vk::Device _vkDevice;

            void destroyVulkan();
            Modules::Result initVulkan();

            std::expected<void, std::error_code> createInstance(InitData& init);
            std::expected<void, std::error_code> findPhysicalDevice(InitData& init);
            std::expected<void, std::error_code> createDevice(InitData& init);
    };
}