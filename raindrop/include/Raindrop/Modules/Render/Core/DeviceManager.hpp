#pragma once

#include <Raindrop/Core/Error.hpp>
#include <Raindrop/Modules/Window/Window.hpp>
#include <Raindrop/Modules/Window/WindowModule.hpp>
#include <vulkan/vulkan.hpp>
#include <VkBootstrap.h>

namespace Raindrop::Render{
    class DeviceManager{
        public:

            enum class ErrorCode{
                FAILED_INSTANCE_CREATION,
                PHYSICAL_DEVICE_NOT_FOUND,
                FAILED_DEVICE_CREATION
            };

            static std::error_category& error_category();

            static inline std::error_code make_error_code(ErrorCode e){
                return {static_cast<int>(e), error_category()};
            }

            static inline std::error_code FailedInstanceCreationError() {return make_error_code(ErrorCode::FAILED_INSTANCE_CREATION);}
            static inline std::error_code PhysicalDeviceNotFoundError() {return make_error_code(ErrorCode::PHYSICAL_DEVICE_NOT_FOUND);}
            static inline std::error_code FailedDeviceCreationError() {return make_error_code(ErrorCode::FAILED_DEVICE_CREATION);}

            DeviceManager();
            ~DeviceManager();

            std::expected<void, Error> initialize(vk::SurfaceKHR& surface, std::shared_ptr<Window::Window>& window);
            void shutdown();

            vk::Device device() const;
            vk::PhysicalDevice physicalDevice() const;
            vk::Instance instance() const;

            vkb::Instance vkbInstance() const;
            vkb::PhysicalDevice vkbPhysicalDevice() const;
            vkb::Device vkbDevice() const;

        private:
            struct InitData{
                vk::SurfaceKHR& surface;
                std::shared_ptr<Window::Window>& window;
            };

            vkb::Instance _instance;
            vkb::PhysicalDevice _physicalDevice;
            vkb::Device _device;

            vk::Instance _vkInstance;
            vk::PhysicalDevice _vkPhysicalDevice;
            vk::Device _vkDevice;

            std::expected<void, Error> createInstance(InitData& surface);
            std::expected<void, Error> pickPhysicalDevice(InitData& surface);
            std::expected<void, Error> createDevice(InitData& surface);

    };
}