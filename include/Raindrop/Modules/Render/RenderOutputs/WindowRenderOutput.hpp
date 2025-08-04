#pragma once

#include "../IRenderOutput.hpp"
#include "Raindrop/Modules/Window/Window.hpp"
#include <VkBootstrap.h>
#include <vulkan/vulkan.hpp>

namespace Raindrop::Render{
    class WindowRenderOutput : public IRenderOutput{
        public:
            WindowRenderOutput(Window::SharedWindow window);
            virtual ~WindowRenderOutput() = default;

            virtual void initialize(RenderCoreModule& core) override;
            virtual void shutdown() override;

            virtual std::expected<bool, Error> acquire(uint64_t timeout = UINT64_MAX) override;
            virtual std::expected<PreRenderResult, Error> preRender(uint64_t timeout = UINT64_MAX) override;
            virtual std::expected<void, Error> present(vk::Semaphore waitSemaphore) override;

            virtual uint32_t getCurrentBufferIndex() const override;
            virtual uint32_t getBufferCount() const override;

            virtual void begin(vk::CommandBuffer cmd, vk::SubpassContents subpassContents = vk::SubpassContents::eInline) override;
            virtual void end(vk::CommandBuffer cmd) override;

            void invalidate();
        
        private:
            RenderCoreModule* _core;
            Window::WeakWindow _window;

            struct Swapchain{
                struct Frame{
                    vk::Framebuffer framebuffer = VK_NULL_HANDLE;
                    vk::Semaphore imageAvailable = VK_NULL_HANDLE;
                    vk::Semaphore imageFinished = VK_NULL_HANDLE;
                    vk::Fence inFlightFence = VK_NULL_HANDLE;
                    vk::Fence imageInFlight = VK_NULL_HANDLE;
                    vk::Image image = VK_NULL_HANDLE;
                    vk::ImageView imageView = VK_NULL_HANDLE;
                };
                
                RenderCoreModule& core;
                vk::SwapchainKHR swapchain;
                std::vector<Frame> frames;
                
                Swapchain(RenderCoreModule& core, vk::SwapchainKHR swapchain);
                ~Swapchain();
            };

            struct SwapchainSupport{
                vk::SurfaceCapabilitiesKHR capabilities;
                vk::ColorSpaceKHR colorSpace;
                std::vector<vk::SurfaceFormatKHR> formats;
                std::vector<vk::PresentModeKHR> presentModes;

                inline bool supported() const noexcept {
                    return !formats.empty() && !presentModes.empty();
                }
            };
            
            std::unique_ptr<Swapchain> _swapchain;
            vk::SurfaceKHR _surface;
            vk::RenderPass _renderPass;
            vk::SurfaceFormatKHR _surfaceFormat;
            vk::PresentModeKHR _presentMode;
			vk::ClearColorValue _clearColor;
            vk::Extent2D _extent;
            SwapchainSupport _support;
            uint32_t _frameCount;
            uint32_t _currentFrame;

            bool _rebuildPending;

            vk::SurfaceFormatKHR _wantedSurfaceFormat;
            vk::PresentModeKHR _wantedPresentMode;
            uint32_t _wantedFrameCount;

            void findPresentMode();
            void findSurfaceFormat();
            void findFrameCount();
            void findExtent(const vk::Extent2D& wanted);

            std::expected<void, Error> createSurface();
            std::expected<void, Error> rebuildSwapchain();
            std::expected<void, Error> createRenderPass();
            std::expected<void, Error> getSupport();
            std::expected<void, Error> getSwapchainImages();
            std::expected<void, Error> createImageViews();
            std::expected<void, Error> createFramebuffers();
            std::expected<void, Error> createSyncObjects();
    };
}