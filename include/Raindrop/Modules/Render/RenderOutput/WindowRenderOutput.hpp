#pragma once

#include <VkBootstrap.h>
#include <vulkan/vulkan.hpp>

#include "IRenderOutput.hpp"
#include "Raindrop/Modules/Window/Window.hpp"

namespace Raindrop::Render{
    class WindowRenderOutput : public IRenderOutput{
        public:
            WindowRenderOutput(Window::SharedWindow window);
            virtual ~WindowRenderOutput() = default;

            virtual void initialize(Engine& engine) override;
            virtual void shutdown() override;

            virtual std::expected<vk::Semaphore, Error> acquire(vk::Fence fence, uint64_t timeout = UINT64_MAX) override;
            virtual std::expected<void, Error> preRender(uint64_t timeout = UINT64_MAX) override;
            virtual std::expected<void, Error> postRender(vk::Semaphore finishedSemaphore = {}) override;

            virtual uint32_t getCurrentBufferIndex() const override;
            virtual uint32_t getBufferCount() const override;

            virtual void begin(vk::CommandBuffer cmd, vk::SubpassContents subpassContents = vk::SubpassContents::eInline) override;
            virtual void end(vk::CommandBuffer cmd) override;

            virtual vk::Image image() const override;
            virtual vk::Extent2D extent() const override;
            virtual vk::RenderPass renderPass() const override;

            void invalidate();

            Window::WeakWindow window() const;
            
            // virtual Store::ResourcePtr<RenderOutputResource> resources() override;
        
        private:
            RenderCoreModule* _core;
            Window::WeakWindow _window;

            struct Swapchain{
                struct Frame{
                    vk::Semaphore imageAvailable = VK_NULL_HANDLE;
                    vk::Fence fence = VK_NULL_HANDLE;
                    vk::ImageView imageView = VK_NULL_HANDLE;
                    vk::Framebuffer framebuffer = VK_NULL_HANDLE;
                };

                std::vector<vk::Image> images; 
                
                RenderCoreModule& core;
                vk::SwapchainKHR swapchain;
                std::vector<Frame> frames;

                // Store::ResourcePtr<RenderOutputResource> resources;
                
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
            // Store::WriteLease<RenderOutputResource> _resourceWrite;

            vk::SurfaceKHR _surface;
            vk::SurfaceFormatKHR _surfaceFormat;
            vk::PresentModeKHR _presentMode;
			vk::ClearColorValue _clearColor;
            vk::Extent2D _extent;
            vk::RenderPass _renderPass;
            SwapchainSupport _support;
            uint32_t _frameCount;

            uint32_t _currentFrame = 0;
            uint32_t _currentImage = 0;

            bool _rebuildPending = false;

            vk::SurfaceFormatKHR _wantedSurfaceFormat;
            vk::PresentModeKHR _wantedPresentMode;
            uint32_t _wantedFrameCount;

            void findPresentMode();
            void findSurfaceFormat();
            void findFrameCount();
            void findExtent(const vk::Extent2D& wanted);

            std::expected<void, Error> createSurface();
            std::expected<void, Error> rebuildSwapchain();
            std::expected<void, Error> getSupport();
            std::expected<void, Error> getSwapchainImages();
            std::expected<void, Error> createImageViews();
            std::expected<void, Error> createFramebuffers();
            std::expected<void, Error> createSyncObjects();
            std::expected<void, Error> createRenderPass();
    };
}