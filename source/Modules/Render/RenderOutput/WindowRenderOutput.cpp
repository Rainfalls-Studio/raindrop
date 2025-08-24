#include "Raindrop/Modules/Render/RenderOutput/WindowRenderOutput.hpp"
#include "Raindrop/Modules/Render/RenderOutput/RenderOutputModule.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop::Render{
    WindowRenderOutput::Swapchain::Swapchain(RenderCoreModule& core_, vk::SwapchainKHR swapchain_) :
        core{core_},
        swapchain{swapchain_}
    {}

    WindowRenderOutput::Swapchain::~Swapchain(){
        auto device = core.device();

        Queue& presentQueue = core.presentQueue();
        Queue& graphicsQueue = core.graphicsQueue();

        presentQueue->waitIdle();
        graphicsQueue->waitIdle();

        for (auto& frame : frames){
            device.destroySemaphore(frame.imageAvailable);

            // the image is owned by the swapchain

            // the fence is owned externaly
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

    void WindowRenderOutput::initialize(Engine& engine){
        _core = engine.getModuleManager().getModuleAs<RenderCoreModule>("RenderCore").get();

        spdlog::info("Initializing window render output...");

        auto result = 
            createSurface()
            .and_then([this]{return getSupport();})
            .transform([this]{findSurfaceFormat();})
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

    void WindowRenderOutput::shutdown(){
        spdlog::trace("Destroying swapchain...");
        _swapchain.reset();

        if (_surface){
            _core->instance().destroySurfaceKHR(_surface);
        }
    }

    std::expected<void, Error> WindowRenderOutput::createSurface(){
        auto window = _window.lock();

        if (!window){
            spdlog::error("Window is not valid");
            return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "The window is not valid"));
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
            .setImageUsage(vk::ImageUsageFlagBits::eTransferDst)
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
            .and_then([this]{return createSyncObjects();})
            .and_then([this]{return createSwapchainResources();});
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
        _swapchain->images = images;

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
                // f.inFlightFence = device.createFence(fenceInfo);
            } catch (const vk::Error& e){
                spdlog::error("Failed to create vulkan swapchain sync object : {}", e.what());
                return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "failed to create vulkan swapchain sync object : {}", e.what()));
            }
		}

        return {};
    }

    std::expected<void, Error> WindowRenderOutput::createSwapchainResources(){
        spdlog::info("Creating window render output resources...");
        if (!_swapchain){
            spdlog::error("Cannot create swapchain resources as there is no swapchain");
            return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "The swapchain as not been created"));
        }

        auto& res = _swapchain->resources;
        res = std::make_shared<Store::Resource<RenderOutputResource>>(_frameCount);

        for (size_t i=0; i<_frameCount; i++){
            auto wl = res->acquire_write(i);
            auto& b = _swapchain->frames[i];

            wl->image = _swapchain->images[i];
            wl->imageAvailableFence = b.fence;
            wl->imageAvailableSemaphore = b.imageAvailable;
            wl->available = false;
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

        if (frame.fence != nullptr){
            auto result = device.waitForFences(frame.fence, vk::True, timeout);

            if (result != vk::Result::eSuccess && result != vk::Result::eTimeout){
                spdlog::error("Failed to wait for in-flight fence : {}", vk::to_string(result));
                return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to wait for in-flight fence : {}", vk::to_string(result)));
            }

            // The framegraph already resets the fence
            // device.resetFences(frame.fence);
        }

        auto result = device.acquireNextImageKHR(_swapchain->swapchain, timeout, frame.imageAvailable, VK_NULL_HANDLE, &_currentImage);

        // no possible reader at the time
        auto wl = _swapchain->resources->acquire_write(_currentFrame);

        wl->available = false;
        wl->image = _swapchain->images[_currentImage];

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

        return true;
    }

    std::expected<void, Error> WindowRenderOutput::preRender(uint64_t timeout [[maybe_unused]]){
        auto& frame = _swapchain->frames[_currentFrame];
        
        auto wl = _swapchain->resources->blocking_acquire_write(_currentFrame);
        wl->imageAvailableFence = frame.fence;
        wl->imageAvailableSemaphore = frame.imageAvailable;
        wl->available = true;

        return {};
    }

    std::expected<void, Error> WindowRenderOutput::postRender(){
        auto wl = _swapchain->resources->blocking_acquire_write(_currentFrame);

        vk::PresentInfoKHR info;
        info.setSwapchains(_swapchain->swapchain)
            .setSwapchainCount(1)
            .setImageIndices(_currentFrame);

        if (wl->renderFinishedSemaphore != nullptr){
            info.setWaitSemaphoreCount(1)
                .setWaitSemaphores(wl->renderFinishedSemaphore);
        }
        
        _currentFrame = (_currentFrame + 1) % _frameCount;
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

    void WindowRenderOutput::begin(vk::CommandBuffer cmd, vk::SubpassContents){
        vk::ImageMemoryBarrier barrier(
            {},  // Presentation engine doesn't write in a way we can sync on
            vk::AccessFlagBits::eTransferWrite,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eTransferDstOptimal,
            vk::QueueFamilyIgnored,
            vk::QueueFamilyIgnored,
            image(),
            {
                vk::ImageAspectFlagBits::eColor,
                0,
                1,
                0,
                1
            }
        );

        cmd.pipelineBarrier(
            vk::PipelineStageFlagBits::eBottomOfPipe,
            vk::PipelineStageFlagBits::eTransfer,
            {},
            {},
            {},
            barrier
        );
    }

    void WindowRenderOutput::end(vk::CommandBuffer cmd){
        vk::ImageMemoryBarrier barrier(
            vk::AccessFlagBits::eTransferWrite,
            {},
            vk::ImageLayout::eTransferDstOptimal,
            vk::ImageLayout::ePresentSrcKHR,
            vk::QueueFamilyIgnored,
            vk::QueueFamilyIgnored,
            image(),
            {
                vk::ImageAspectFlagBits::eColor,
                0,
                1,
                0,
                1
            }
        );

        cmd.pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eBottomOfPipe,
            {},
            {},
            {},
            barrier
        );
    }

    vk::Image WindowRenderOutput::image() const{
        return _swapchain->images[_currentImage];
    }

    vk::Extent2D WindowRenderOutput::extent() const{
        return _extent;
    }

    Store::ResourcePtr<RenderOutputResource> WindowRenderOutput::resources(){
        return _swapchain->resources;
    }
}