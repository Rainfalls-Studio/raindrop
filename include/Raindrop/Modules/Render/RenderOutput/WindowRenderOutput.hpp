#pragma once

#include <VkBootstrap.h>
#include <vulkan/vulkan.hpp>

#include "IRenderOutput.hpp"
#include "Raindrop/Core/Store/WriteLease.hpp"
#include "Raindrop/Modules/Window/Window.hpp"
#include "RenderOutputResource.hpp"

namespace Raindrop::Render{
    class WindowRenderOutput : public IRenderOutput{
        public:
            WindowRenderOutput(Window::SharedWindow window);
            virtual ~WindowRenderOutput() = default;

            virtual void initialize(Engine& engine) override;
            virtual void shutdown() override;

            virtual std::expected<bool, Error> acquire(uint64_t timeout = UINT64_MAX) override;
            virtual std::expected<void, Error> preRender(uint64_t timeout = UINT64_MAX) override;
            virtual std::expected<void, Error> postRender() override;

            virtual uint32_t getCurrentBufferIndex() const override;
            virtual uint32_t getBufferCount() const override;

            virtual void begin(vk::CommandBuffer cmd, vk::SubpassContents subpassContents = vk::SubpassContents::eInline) override;
            virtual void end(vk::CommandBuffer cmd) override;

            virtual vk::Image image() const override;
            virtual vk::Extent2D extent() const override;

            void invalidate();
            
            Store::ResourcePtr<RenderOutputResource> resources();
        
        private:
            RenderCoreModule* _core;
            Window::WeakWindow _window;

            struct Swapchain{
                struct Frame{
                    vk::Semaphore imageAvailable = VK_NULL_HANDLE;
                    vk::Fence fence = VK_NULL_HANDLE;
                };

                std::vector<vk::Image> images; 
                
                RenderCoreModule& core;
                vk::SwapchainKHR swapchain;
                std::vector<Frame> frames;

                Store::ResourcePtr<RenderOutputResource> resources;
                
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
            SwapchainSupport _support;
            uint32_t _frameCount;

            uint32_t _currentFrame;
            uint32_t _currentImage;

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
            std::expected<void, Error> getSupport();
            std::expected<void, Error> getSwapchainImages();
            std::expected<void, Error> createSyncObjects();
            std::expected<void, Error> createSwapchainResources();
    };
}