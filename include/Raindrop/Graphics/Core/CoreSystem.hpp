#pragma once

#include "Raindrop/Graphics/System/IRenderSystem.hpp"
#include "Raindrop/Window/Window.hpp"
#include <vulkan/vulkan.hpp>
#include <spdlog/logger.h>
#include <VkBootstrap.h>

namespace Raindrop::Graphics::Core{
    class CoreSystem : public System::IRenderSystem{
        public:
            virtual ~CoreSystem() = default;
            
            virtual void initialize(GraphicsEngine& engine) override;
            virtual void shutdown() override;

            virtual const char* name() const override;

            vk::Instance getInstance() const;
            vk::PhysicalDevice getPhysicalDevice() const;
            vk::Device getDevice() const;

        private:
            std::shared_ptr<spdlog::logger> _logger;

            vk::Instance _instance;
            vk::PhysicalDevice _physicalDevice;
            vk::Device _device;

            vkb::Instance _vkbInstance;
            vkb::PhysicalDevice _vkbPhysicalDevice;
            vkb::Device _vkbDevice;

            struct InitData{
                Window::Window* window = nullptr;
                vk::SurfaceKHR surface = VK_NULL_HANDLE;
            };
            
            void createLogger();
            void createInstance(InitData& data);
            void findPhysicalDevice(InitData& data);
            void createDevice(InitData& data);
    };
}