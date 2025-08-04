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
        spdlog::trace("Destroying swapchain...");
        _swapchain.reset();

        if (_renderPass){
            _core->device().destroyRenderPass(_renderPass);
        }

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
        
        Queue& presentQueue = _core->presentQueue();
        Queue& graphicsQueue = _core->graphicsQueue();

        presentQueue->waitIdle();
        graphicsQueue->waitIdle();

        uint32_t families[] = {presentQueue.familyIndex(), graphicsQueue.familyIndex()};

        if (families[0] != families[1]){
            info.setQueueFamilyIndexCount(2)
                .setPQueueFamilyIndices(families)
                .setImageSharingMode(vk::SharingMode::eConcurrent);
        } else {
            info.setImageSharingMode(vk::SharingMode::eExclusive);
        }


        vk::SwapchainKHR newSwapchain;
        try{
            newSwapchain = device.createSwapchainKHR(info);
        } catch (vk::Error& e){
            spdlog::error("Failed to create swapchain : {}", e.what());
            return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to create swapchain : {}", e.what()));
        }

        _swapchain = std::make_unique<Swapchain>(*_core, newSwapchain);
        _rebuildPending = false;

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

    void WindowRenderOutput::invalidate(){
        _rebuildPending = true;
    }

    std::expected<bool, Error> WindowRenderOutput::acquire(uint64_t timeout){
        if (_window.expired()){
            spdlog::error("Window is not valid");
            return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "The window is not valid"));
        }

        if (_rebuildPending){
            auto rebuildResult = rebuildSwapchain();

            if (!rebuildResult){
                return std::unexpected(rebuildResult.error());
            }
        }

        auto device = _core->device();
        auto& frame = _swapchain->frames[_currentFrame];

        auto result = device.acquireNextImageKHR(_swapchain->swapchain, timeout, frame.imageAvailable, VK_NULL_HANDLE, &_currentFrame);

        switch (result){
            case vk::Result::eSuboptimalKHR: invalidate(); [[fallthrough]];
            case vk::Result::eSuccess: break;

            case vk::Result::eErrorOutOfDateKHR: invalidate(); [[fallthrough]];
            case vk::Result::eTimeout: [[fallthrough]];
            case vk::Result::eNotReady: return false;

            default:{
                spdlog::error("Failed to acquire swapchain image : {}", vk::to_string(result));
                return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed acquire swapchain image : {}", vk::to_string(result)));
            }
        }

        result = device.waitForFences(frame.inFlightFence, vk::True, timeout);
        if (result != vk::Result::eSuccess && result != vk::Result::eTimeout){
            spdlog::error("Failed to wait for in-flight fence : {}", vk::to_string(result));
            return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to wait for in-flight fence : {}", vk::to_string(result)));
        }

        return true;
    }

    std::expected<WindowRenderOutput::PreRenderResult, Error> WindowRenderOutput::preRender(uint64_t timeout){
        auto& frame = _swapchain->frames[_currentFrame];
        auto device = _core->device();

        if (frame.imageInFlight != nullptr){
            auto result = device.waitForFences(frame.imageInFlight, vk::True, timeout);

            if (result != vk::Result::eSuccess && result != vk::Result::eTimeout){
                spdlog::error("Failed to wait for in-flight fence : {}", vk::to_string(result));
                return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to wait for in-flight fence : {}", vk::to_string(result)));
            }
        }

        frame.imageInFlight = frame.inFlightFence;

        device.resetFences(frame.inFlightFence);
        PreRenderResult result;

        result.wait = frame.imageAvailable;
        result.waitStageFlags = vk::PipelineStageFlagBits::eColorAttachmentOutput;

        return result;
    }

    std::expected<void, Error> WindowRenderOutput::present(vk::Semaphore waitSemaphore){
        vk::PresentInfoKHR info;
        info.setSwapchains(_swapchain->swapchain)
            .setSwapchainCount(1)
            .setImageIndices(_currentFrame);

        if (waitSemaphore != nullptr){
            info.setWaitSemaphoreCount(1)
                .setWaitSemaphores(waitSemaphore);
        }
        
        auto result = _core->presentQueue()->presentKHR(info);

        switch (result){
            case vk::Result::eErrorOutOfDateKHR: [[fallthrough]];
            case vk::Result::eSuboptimalKHR: invalidate(); [[fallthrough]];
            case vk::Result::eSuccess: break;
            default: {
                spdlog::error("Failed to present swapchain image : {}", vk::to_string(result));
                return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to present swapchain image : {}", vk::to_string(result)));
            }
        }

        return {};
    }

    uint32_t WindowRenderOutput::getCurrentBufferIndex() const{
        return _currentFrame;
    }

    uint32_t WindowRenderOutput::getBufferCount() const{
        return _frameCount;
    }

    void WindowRenderOutput::begin(vk::CommandBuffer cmd, vk::SubpassContents subpassContents){

        vk::RenderPassBeginInfo info;

        vk::ClearValue clearValue = _clearColor;

        info.setRenderPass(_renderPass)
            .setFramebuffer(_swapchain->frames[_currentFrame].framebuffer)
            .setRenderArea(vk::Rect2D({0, 0}, _extent))
            .setClearValueCount(1)
            .setClearValues(clearValue);

        cmd.beginRenderPass(info, subpassContents);


		vk::Viewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(_extent.width);
		viewport.height = static_cast<float>(_extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
        
		vk::Rect2D scissor{{0, 0}, _extent};
        
        cmd.setViewport(0, viewport);
        cmd.setScissor(0, scissor);
    }

    void WindowRenderOutput::end(vk::CommandBuffer cmd){
        cmd.endRenderPass();
    }
}