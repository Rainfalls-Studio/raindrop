#pragma once

#include "Raindrop/Core/Modules/IModule.hpp"
#include "Raindrop/Modules/Window/WindowModule.hpp"
#include <VkBootstrap.h>

#include <vk_mem_alloc.h>

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Render{
    class Queue{
        friend class RenderCoreModule;
        public:

            enum class Type{
                GRAPHICS,
                COMPUTE,
                TRANSFER
            };

            Queue() = default;

            inline const vk::Queue* operator->() const noexcept{
                return &_queue;
            }

            inline vk::Queue operator*() const noexcept{
                return _queue;
            }

            inline vk::QueueFlags flags() const noexcept{
                return _flags;
            }

            inline uint32_t familyIndex() const noexcept{
                return _family;
            }

        private:
            inline Queue(vk::Device device, vk::Queue queue, vk::QueueFlags flags, uint32_t family) : 
                _device{device},
                _queue{queue},
                _flags{flags},
                _family{family}
            {}

            vk::Device _device;
            vk::Queue _queue;
            vk::QueueFlags _flags;
            uint32_t _family;
    };

    class RenderCoreModule : public Modules::IModule{
        public:
            enum class ErrorCode{
                FAILED_INSTANCE_CREATION,
                MISSING_INSTANCE_EXTENSIONS,
                MISSING_LAYER,
                FAILED_SURFACE_CREATION,
                NO_PRESENT_SUPPORT,
                NO_SUITABLE_PHYSICAL_DEVICE,
                NO_COMPATIBLE_QUEUE_FAMILY,
                FAILED_LOGICAL_DEVICE_CREATION,
                FAILED_ALLOCATOR_CREATION
            };

            static std::error_category& error_category();

            static inline std::error_code make_error_code(ErrorCode e){
                return {static_cast<int>(e), error_category()};
            }

            static inline std::error_code FailedInstanceCreationError() {return make_error_code(ErrorCode::FAILED_INSTANCE_CREATION);}
            static inline std::error_code MissingInstanceExtensionError() {return make_error_code(ErrorCode::MISSING_INSTANCE_EXTENSIONS);}
            static inline std::error_code MissingLayerError() {return make_error_code(ErrorCode::MISSING_LAYER);}
            static inline std::error_code FailedSurfaceCreationError() {return make_error_code(ErrorCode::FAILED_SURFACE_CREATION);}
            static inline std::error_code NoPresentSupportError() {return make_error_code(ErrorCode::NO_PRESENT_SUPPORT);}
            static inline std::error_code NoSuitablePhysicalDeviceError() {return make_error_code(ErrorCode::NO_SUITABLE_PHYSICAL_DEVICE);}
            static inline std::error_code NoCompatibleQueueFamilyError() {return make_error_code(ErrorCode::NO_COMPATIBLE_QUEUE_FAMILY);}
            static inline std::error_code FailedLogicalDeviceCreationError() {return make_error_code(ErrorCode::FAILED_LOGICAL_DEVICE_CREATION);}
            static inline std::error_code FailedAllocatorCreationError() {return make_error_code(ErrorCode::FAILED_ALLOCATOR_CREATION);}

            RenderCoreModule();
            virtual ~RenderCoreModule() = default;

            virtual Modules::Result initialize(Modules::InitHelper& helper) override;
            virtual void shutdown() override;

            virtual Name name() const noexcept override;

            virtual Modules::DependencyList dependencies() const noexcept override;

            virtual Modules::Result dependencyReload(const Name& name) override;
            virtual Modules::Result dependencyShutdown(const Name& name) override;

            

            inline vk::Instance instance() const noexcept{
                return _vkInstance;
            }

            inline vk::Device device() const noexcept{
                return _vkDevice;
            }

            inline vk::PhysicalDevice physicalDevice() const noexcept{
                return _vkPhysicalDevice;
            }

            inline vkb::Instance vkbInstance() const noexcept{
                return _instance;
            }

            inline vkb::Device vkbDevice() const noexcept{
                return _device;
            }

            inline vkb::PhysicalDevice vkbPhysicalDevice() const noexcept{
                return _physicalDevice;
            }

            inline Queue& graphicsQueue() noexcept{
                return _graphicsQueue;
            }

            inline Queue& computeQueue() noexcept{
                return _computeQueue;
            }

            inline Queue& transferQueue() noexcept{
                return _transferQueue;
            }

            inline Queue& presentQueue() noexcept{
                return _presentQueue;
            }

            inline VmaAllocator allocator() const noexcept{
                return _allocator;
            }

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

            VmaAllocator _allocator;

            Queue _graphicsQueue;
            Queue _computeQueue;
            Queue _transferQueue;
            Queue _presentQueue;

            void destroyVulkan();
            Modules::Result initVulkan();

            std::expected<void, Error> createInstance(InitData& init);
            std::expected<void, Error> findPhysicalDevice(InitData& init);
            std::expected<void, Error> createDevice(InitData& init);
            std::expected<void, Error> findQueues();
            std::expected<void, Error> createVmaAllocator();
    };
}