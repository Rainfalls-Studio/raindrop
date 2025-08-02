#include "Raindrop/Modules/Render/RenderOutputs/WindowRenderOutput.hpp"
#include "Raindrop/Modules/Render/RenderOutputModule.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop::Render{
    WindowRenderOutput::Swapchain::Swapchain(RenderCoreModule& core_, vk::SwapchainKHR swapchain_) :
        core{core_},
        swapchain{swapchain_}
    {}

    WindowRenderOutput::Swapchain::~Swapchain(){
        auto device = core.device();

        for (auto& frame : frames){
            device.destroyFramebuffer(frame.framebuffer);
            device.destroySemaphore(frame.imageAvailable);
            device.destroySemaphore(frame.imageFinished);
            device.destroyFence(frame.inFlightFence);
            device.destroyFence(frame.imageInFlight);
            device.destroyImageView(frame.imageView);

            // The image is owned by the swapchain
        }

        if (swapchain){
            device.destroySwapchainKHR(swapchain);
        }
    }

    WindowRenderOutput::WindowRenderOutput(Window::SharedWindow window) : _window{window}{
        _wantedPresentMode = vk::PresentModeKHR::eFifo;
        _wantedSurfaceFormat = {vk::Format::eR8G8B8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear};
        _wantedFrameCount = 2;
    }

    void WindowRenderOutput::initialize(RenderCoreModule& core){
        _core = &core;

        spdlog::info("Initializing window render output...");

        auto result = 
            createSurface()
            .and_then([this]{return getSupport();})
            .transform([this]{findSurfaceFormat();})
            .and_then([this]{return createRenderPass();})
            .and_then([this]{return rebuildSwapchain();});
    }

    void WindowRenderOutput::findSurfaceFormat(){
		for (auto& f : _support.formats){
			if (f == _wantedSurfaceFormat){
				_surfaceFormat = f;
				return;
			}
		}

		_surfaceFormat = _support.formats[0];

        spdlog::warn("The wanted surface format is not available ({}, {}). Falling back to : ({}, {})", 
            vk::to_string(_wantedSurfaceFormat.format),
            vk::to_string(_wantedSurfaceFormat.colorSpace),
            vk::to_string(_surfaceFormat.format),
            vk::to_string(_surfaceFormat.colorSpace)
        );

        return;
    }

    void WindowRenderOutput::findPresentMode(){
		for (auto& p : _support.presentModes){
			if (p == _wantedPresentMode){
				_presentMode = p;
				return;
			}
		}

        _presentMode = _support.presentModes[0];

        spdlog::warn("The wanted present mode is not available {}. Falling back to : {}", 
            vk::to_string(_wantedPresentMode),
            vk::to_string(_presentMode)
        );

        return;
    } 

    void WindowRenderOutput::findFrameCount(){
		auto& capabilities = _support.capabilities;
		_frameCount = std::clamp(_wantedFrameCount, capabilities.minImageCount, capabilities.maxImageCount);

        if (_frameCount != _wantedFrameCount){
            spdlog::warn("The wanted frame count is not available {}. Falling back to : {}",
                _wantedFrameCount,
                _frameCount
            );
        }
    }

    void WindowRenderOutput::findExtent(const vk::Extent2D& wanted){
		_extent.width = std::clamp(wanted.width, _support.capabilities.minImageExtent.width, _support.capabilities.maxImageExtent.width);
		_extent.height = std::clamp(wanted.height, _support.capabilities.minImageExtent.height, _support.capabilities.maxImageExtent.height);
    }

    std::expected<void, Error> WindowRenderOutput::createRenderPass(){
        
		vk::AttachmentDescription colorAttachment;
        colorAttachment
            .setFormat(_surfaceFormat.format)
            .setSamples(vk::SampleCountFlagBits::e1)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eStore)
            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setInitialLayout(vk::ImageLayout::eUndefined)
            .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

		vk::AttachmentReference colorAttachmentRef;
        colorAttachmentRef
            .setAttachment(0)
            .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

		vk::SubpassDescription subpass;
        subpass
            .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
            .setColorAttachmentCount(1)
            .setPColorAttachments(&colorAttachmentRef);
		
		vk::SubpassDependency dependency;
        dependency
            .setSrcSubpass(vk::SubpassExternal)
            .setSrcAccessMask(vk::AccessFlagBits::eNone)
            .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
            .setDstSubpass(0)
            .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
            .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

		vk::RenderPassCreateInfo renderPassInfo;
        renderPassInfo
            .setAttachmentCount(1)
            .setPAttachments(&colorAttachment)
            .setSubpassCount(1)
            .setPSubpasses(&subpass)
            .setDependencyCount(1)
            .setPDependencies(&dependency);
        
        try{
            _renderPass = _core->device().createRenderPass(renderPassInfo);
        } catch (vk::Error& e){
            spdlog::error("Failed to create swapchain's render pass : {}", e.what());
            return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to create render pass : {}", e.what()));
        }

        return {};
    }
    
    void WindowRenderOutput::shutdown(){
        _swapchain.reset();

        if (_surface){
            _core->instance().destroySurfaceKHR(_surface);
        }
    }

    std::expected<void, Error> WindowRenderOutput::createSurface(){
        auto window = _window.lock();

        if (!window){
            spdlog::error("Window is not valid");
            // return std::unexpected(Error::InvalidState);
        }

        spdlog::info("Creating a vulkan surface...");
        auto result = window->createSurface(_core->instance());
        if (!result){
            auto& error = result.error();
            spdlog::error("Failed to create surface : {} : {}", error.message(), error.reason());
            return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to create surface : {} : {}", error.message(), error.reason()));
        }

        _surface = result.value();
        return {};
    }

    std::expected<void, Error> WindowRenderOutput::rebuildSwapchain(){
        auto window = _window.lock();
        auto device = _core->device();

        device.waitIdle();

        if (!window){
            spdlog::error("Window is not valid");
        }

        {
            auto result = getSupport();
            if (!result){
                return std::unexpected(result.error());
            }
        }

        auto resolution = window->getResolution();

        findPresentMode();
        findSurfaceFormat();
        findFrameCount();
        findExtent({resolution.x, resolution.y});

        vk::SwapchainCreateInfoKHR info;
        info.setSurface(_surface)
            .setOldSwapchain(_swapchain ? _swapchain->swapchain : nullptr)
            .setPresentMode(_presentMode)
            .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
            .setClipped(vk::True)
            .setImageFormat(_surfaceFormat.format)
            .setMinImageCount(_frameCount)
            .setImageExtent({resolution.x, resolution.y})
            .setImageArrayLayers(1)
            .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
            .setPreTransform(_support.capabilities.currentTransform);

        vk::SwapchainKHR newSwapchain;
        try{
            newSwapchain = device.createSwapchainKHR(info);
        } catch (vk::Error& e){
            spdlog::error("Failed to create swapchain : {}", e.what());
            return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to create swapchain : {}", e.what()));
        }

        _swapchain = std::make_unique<Swapchain>(*_core, newSwapchain);

        return getSwapchainImages()
            .and_then([this]{return createImageViews();})
            .and_then([this]{return createFramebuffers();})
            .and_then([this]{return createSyncObjects();});
    }

    std::expected<void, Error> WindowRenderOutput::getSupport(){
        auto core = *_core;

        _support.presentModes = core.physicalDevice().getSurfacePresentModesKHR(_surface);
        _support.formats = core.physicalDevice().getSurfaceFormatsKHR(_surface);
        _support.capabilities = core.physicalDevice().getSurfaceCapabilitiesKHR(_surface);

        if (_support.supported()){
            return {};
        } else {
            return std::unexpected(Error(RenderOutputModule::MissingSurfaceSupportError()));
        }
    }

    std::expected<void, Error> WindowRenderOutput::getSwapchainImages(){
        std::vector<vk::Image> images;

        try{
            images = _core->device().getSwapchainImagesKHR(_swapchain->swapchain);
        } catch (vk::Error& e){
            spdlog::error("Failed to get swapchain images : {}", e.what());
            return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to get swapchain images : {}", e.what()));
        }
        
        _swapchain->frames.resize(images.size());

        for (size_t i=0; i<images.size(); i++){
            auto& frame = _swapchain->frames[i];
            frame.image = images[i];
        }

        return {};
    }

    std::expected<void, Error> WindowRenderOutput::createImageViews(){
        for (auto &f : _swapchain->frames){
			vk::ImageViewCreateInfo viewInfo;
            viewInfo
                .setImage(f.image)
                .setViewType(vk::ImageViewType::e2D)
                .setFormat(_surfaceFormat.format)
                .setSubresourceRange(vk::ImageSubresourceRange(
                    vk::ImageAspectFlagBits::eColor,
                    0,
                    1,
                    0,
                    1
                ));

            try{
                f.imageView = _core->device().createImageView(viewInfo);
            } catch (vk::Error& e){
                spdlog::error("failed to create swapchain image view : {}", e.what());
                return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to create swapchain image view : {}", e.what()));
            }
		}

        return {};
    }

    std::expected<void, Error> WindowRenderOutput::createFramebuffers(){
		for (auto &f : _swapchain->frames){
			vk::FramebufferCreateInfo info;
			info.renderPass = _renderPass;
			info.attachmentCount = 1;
			info.pAttachments = &f.imageView;
			info.width = _extent.width;
			info.height = _extent.height;
			info.layers = 1;

            try {
                f.framebuffer = _core->device().createFramebuffer(info);
            } catch (vk::Error& e){
                spdlog::error("Failed to create vulkan swapchain framebuffer : {}", e.what());
                return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "failed to create vulkan swapchain framebuffer : {}", e.what()));
            }
		}

        return {};
    }

    std::expected<void, Error> WindowRenderOutput::createSyncObjects(){
        auto device = _core->device();

		vk::SemaphoreCreateInfo semaphoreInfo;
		vk::FenceCreateInfo fenceInfo = {};
        fenceInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

		for (auto &f : _swapchain->frames) {
            try{
                f.imageAvailable = device.createSemaphore(semaphoreInfo);
                f.imageFinished = device.createSemaphore(semaphoreInfo);
                f.inFlightFence = device.createFence(fenceInfo);
            } catch (const vk::Error& e){
                spdlog::error("Failed to create vulkan swapchain sync object : {}", e.what());
                return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "failed to create vulkan swapchain sync object : {}", e.what()));
            }
		}

        return {};
    }
}