#pragma once

#include <Raindrop/Modules/IModule.hpp>
#include <Window/WindowModule.hpp>
#include <VkBootstrap.h>

#include "DeviceManager.hpp"

#include <vk_mem_alloc.h>

namespace Raindrop{
    class Engine;
}

namespace Render{
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

    class RenderCoreModule : public Raindrop::IModule{
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

            virtual Raindrop::Result initialize(Raindrop::InitHelper& helper) override;
            virtual void shutdown() override;

            DeviceManager& deviceManager() noexcept;

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

            Raindrop::Engine* _engine;
            std::shared_ptr<Window::WindowModule> _windowModule;
            std::shared_ptr<spdlog::logger> _logger;

            DeviceManager _deviceManager;

            VmaAllocator _allocator;

            Queue _graphicsQueue;
            Queue _computeQueue;
            Queue _transferQueue;
            Queue _presentQueue;

            std::expected<void, Raindrop::Error> findQueues();
            std::expected<void, Raindrop::Error> createVmaAllocator();
    };
}