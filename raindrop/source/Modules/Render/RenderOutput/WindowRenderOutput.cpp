#include "Raindrop/Modules/Render/RenderOutput/WindowRenderOutput.hpp"
#include "Raindrop/Modules/Render/RenderOutput/RenderOutputModule.hpp"
#include <spdlog/spdlog.h>

// this will make the window use render pass
// The absence of this define will make the window use only copy mechanism to render
#define RENDERPASS

namespace Raindrop::Render{
    WindowRenderOutput::Swapchain::Swapchain(RenderCoreModule& core_, vk::SwapchainKHR swapchain_) :
        core{core_},
        swapchain{swapchain_}
    {}

    WindowRenderOutput::Swapchain::~Swapchain(){
        auto device = core.deviceManager().device();

        Queue& presentQueue = core.presentQueue();
        Queue& graphicsQueue = core.graphicsQueue();

        {
            auto result = vk::Result::eErrorUnknown;

            if ((result = presentQueue->waitIdle()) != vk::Result::eSuccess){
                spdlog::warn("Failed to wait present queue idle : {}", vk::to_string(result));
            }

            if ((result = graphicsQueue->waitIdle()) != vk::Result::eSuccess){
                spdlog::warn("Failed to wait graphics queue idle : {}", vk::to_string(result));

            }
        }

        for (auto& frame : frames){
            device.destroySemaphore(frame.imageAvailable);
            device.destroyImageView(frame.imageView);
            device.destroyFramebuffer(frame.framebuffer);

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

    std::expected<void, Error> WindowRenderOutput::initialize(Engine& engine){
        _core = engine.getModuleManager().getModuleAs<RenderCoreModule>("RenderCore").get();

        spdlog::info("Initializing window render output...");

        return createSurface()
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
        const auto& caps = _support.capabilities;

        if (caps.currentExtent.width != UINT32_MAX){
            _extent = caps.currentExtent;
        } else {
		    _extent.width = std::clamp(wanted.width, _support.capabilities.minImageExtent.width, _support.capabilities.maxImageExtent.width);
		    _extent.height = std::clamp(wanted.height, _support.capabilities.minImageExtent.height, _support.capabilities.maxImageExtent.height);
        }
    }

    void WindowRenderOutput::shutdown(){
        spdlog::trace("Destroying swapchain...");
        _swapchain.reset();

        auto& deviceManager = _core->deviceManager();

        if (_renderPass){
            deviceManager.device().destroyRenderPass(_renderPass);
        }

        if (_surface){
            deviceManager.instance().destroySurfaceKHR(_surface);
        }
    }

    std::expected<void, Error> WindowRenderOutput::createSurface(){
        auto window = _window.lock();

        if (!window){
            spdlog::error("Window is not valid");
            return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "The window is not valid"));
        }

        spdlog::info("Creating a vulkan surface...");
        auto result = window->createSurface(_core->deviceManager().instance());
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
        auto device = _core->deviceManager().device();

        if (auto result = device.waitIdle(); result != vk::Result::eSuccess){
            spdlog::warn("Failed to wait vulkan device idle : {} ", vk::to_string(result));
        }

        if (!window){
            spdlog::error("Window is not valid");
        }

        {
            if (auto result = getSupport(); !result.has_value()){
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
            .setImageExtent(_extent)
            .setImageArrayLayers(1)
            .setPreTransform(_support.capabilities.currentTransform);
        
        #ifdef RENDERPASS
            info.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
        #else
            info.setImageUsage(vk::ImageUsageFlagBits::eTransferDst);
        #endif

        Queue& presentQueue = _core->presentQueue();
        Queue& graphicsQueue = _core->graphicsQueue();

        if (auto result = presentQueue->waitIdle(); result != vk::Result::eSuccess){
            spdlog::warn("Failed to wait present queue idle : {}", vk::to_string(result));
        }

        if (auto result = graphicsQueue->waitIdle(); result != vk::Result::eSuccess){
            spdlog::warn("Failed to wait graphics queue idle : {}", vk::to_string(result));
        }

        uint32_t families[] = {presentQueue.familyIndex(), graphicsQueue.familyIndex()};

        if (families[0] != families[1]){
            info.setQueueFamilyIndexCount(2)
                .setPQueueFamilyIndices(families)
                .setImageSharingMode(vk::SharingMode::eConcurrent);
        } else {
            info.setImageSharingMode(vk::SharingMode::eExclusive);
        }


        vk::SwapchainKHR newSwapchain;

        {
            auto result = device.createSwapchainKHR(info);

            if (result.result != vk::Result::eSuccess){
                spdlog::error("Failed to create swapchain : {}", vk::to_string(result.result));
                return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to create swapchain"));

            }

            newSwapchain = result.value;
        }

        _swapchain = std::make_unique<Swapchain>(*_core, newSwapchain);
        _rebuildPending = false;

        _currentFrame = 0;
        _currentImage = 0;
        _epoch++;

        return getSwapchainImages()
            .and_then([this]{return createImageViews();})
            .and_then([this]{return createFramebuffers();})
            .and_then([this]{return createSyncObjects();});
    }

    std::expected<void, Error> WindowRenderOutput::getSupport(){
        auto core = *_core;

        auto physicalDevice = core.deviceManager().physicalDevice();

        _support.presentModes = physicalDevice.getSurfacePresentModesKHR(_surface).value;
        _support.formats = physicalDevice.getSurfaceFormatsKHR(_surface).value;
        _support.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(_surface).value;

        if (_support.supported()){
            return {};
        } else {
            return std::unexpected(Error(RenderOutputModule::MissingSurfaceSupportError()));
        }
    }

    std::expected<void, Error> WindowRenderOutput::getSwapchainImages(){
        std::vector<vk::Image> images;

        auto device = _core->deviceManager().device();

        {
            auto result = device.getSwapchainImagesKHR(_swapchain->swapchain);
            
            if (result.result != vk::Result::eSuccess){
                spdlog::error("Failed to get swapchain images : {}", vk::to_string(result.result));
                return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to get swapchain images"));
            }

            images = result.value;
        }   
        
        _swapchain->frames.resize(images.size());
        _swapchain->images = images;

        return {};
    }

    std::expected<void, Error> WindowRenderOutput::createImageViews(){
        auto device = _core->deviceManager().device();

        for (size_t i=0; i<_frameCount; i++){
            auto& frame = _swapchain->frames[i];
            auto& image = _swapchain->images[i];

            vk::ImageViewCreateInfo info{
                vk::ImageViewCreateFlags(0),
                image,
                vk::ImageViewType::e2D,
                _surfaceFormat.format,
                vk::ComponentMapping(),
                vk::ImageSubresourceRange{
                    vk::ImageAspectFlagBits::eColor,
                    0, 1,
                    0, 1
                }
            };

            auto result = device.createImageView(info);

            if (result.result != vk::Result::eSuccess){
                spdlog::error("Failed to create frame image view : {}", vk::to_string(result.result));
                return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to create image view"));
            }

            frame.imageView = result.value;
        }

        return {};
    }


    std::expected<void, Error> WindowRenderOutput::createFramebuffers(){
        auto device = _core->deviceManager().device();

        for (size_t i=0; i<_frameCount; i++){
            auto& frame = _swapchain->frames[i];

            vk::FramebufferCreateInfo info{
                vk::FramebufferCreateFlagBits(0),
                _renderPass,
                1, &frame.imageView,
                _extent.width, _extent.height, 1
            };

            auto result = device.createFramebuffer(info);

            if (result.result != vk::Result::eSuccess){
                spdlog::error("Failed to create framebuffer : {}", vk::to_string(result.result));
                return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to create framebuffer"));
            }

            frame.framebuffer = result.value;
        }

        return {};
    }


    std::expected<void, Error> WindowRenderOutput::createSyncObjects(){
        auto device = _core->deviceManager().device();

		vk::SemaphoreCreateInfo semaphoreInfo;

		for (auto &f : _swapchain->frames) {
            auto result = device.createSemaphore(semaphoreInfo);

            if (result.result != vk::Result::eSuccess){
                spdlog::error("Failed to create vulkan swapchain sync object : {}", vk::to_string(result.result));
                return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "failed to create vulkan swapchain sync object"));

            }
            
            f.imageAvailable = result.value;
		}

        return {};
    }

    std::expected<void, Error> WindowRenderOutput::createRenderPass(){

        vk::RenderPassCreateInfo info;

        auto device = _core->deviceManager().device();

        vk::AttachmentDescription colorAttachment(
            vk::AttachmentDescriptionFlags(),
            _surfaceFormat.format,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::ePresentSrcKHR
        );

        vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);

        vk::SubpassDescription subpass(
            vk::SubpassDescriptionFlags(),
            vk::PipelineBindPoint::eGraphics,
            0, nullptr,
            1, &colorAttachmentRef
        );

        vk::SubpassDependency dependency(
            VK_SUBPASS_EXTERNAL,
            0,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            {},
            vk::AccessFlagBits::eColorAttachmentWrite
        );

        info.setAttachments({colorAttachment})
            .setSubpasses({subpass})
            .setDependencies({dependency});

        auto result = device.createRenderPass(info);

        if (result.result != vk::Result::eSuccess){
            spdlog::error("Failed to create render pass : {}", vk::to_string(result.result));
            return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to create render pass"));
        }

        _renderPass = result.value;
        return {};
    }

    void WindowRenderOutput::invalidate(){
        spdlog::info("swapchain invalidated");
        _rebuildPending = true;
    }

    std::expected<vk::Semaphore, Error> WindowRenderOutput::acquire(vk::Fence fence, uint64_t timeout){
        if (_window.expired()){
            spdlog::error("Window is not valid");
            return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "The window is not valid"));
        }

        _acquired = false;

        if (_rebuildPending){
            auto rebuildResult = rebuildSwapchain();

            if (!rebuildResult){
                return std::unexpected(rebuildResult.error());
            }
            return nullptr;
        }

        auto device = _core->deviceManager().device();
        auto& frame = _swapchain->frames[_currentFrame];

        if (fence != nullptr){
            auto result = device.waitForFences(fence, vk::True, timeout);

            if (result != vk::Result::eSuccess && result != vk::Result::eTimeout){
                spdlog::error("Failed to wait for in-flight fence : {}", vk::to_string(result));
                return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed to wait for in-flight fence : {}", vk::to_string(result)));
            }

            // The framegraph already resets the fence
            // device.resetFences(fence);
        }

        auto result = device.acquireNextImageKHR(_swapchain->swapchain, timeout, frame.imageAvailable, VK_NULL_HANDLE, &_currentImage);

        switch (result){
            case vk::Result::eSuboptimalKHR: invalidate(); [[fallthrough]];
            case vk::Result::eSuccess: break;

            case vk::Result::eErrorOutOfDateKHR: invalidate(); [[fallthrough]];
            case vk::Result::eTimeout: [[fallthrough]];
            case vk::Result::eNotReady: return nullptr;

            default:{
                spdlog::error("Failed to acquire swapchain image : {}", vk::to_string(result));
                return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "Failed acquire swapchain image : {}", vk::to_string(result)));
            }
        }

        _acquired = true;
        return frame.imageAvailable;
    }

    std::expected<void, Error> WindowRenderOutput::present(vk::Semaphore finishedSemaphore){

        auto lock = _window.lock();
        if (!lock){
            return std::unexpected(Error(RenderOutputModule::FailedObjectCreationError(), "The window is not valid"));
        }

        // Check for window resize before presenting
        if (lock->resized()){
            invalidate();
            return {};
        }

        _acquired = false;

        vk::PresentInfoKHR info{};
        info.setSwapchains(_swapchain->swapchain)
            .setSwapchainCount(1)
            .setImageIndices(_currentImage);

        
        if (finishedSemaphore){
            info.setWaitSemaphores({finishedSemaphore});
        }
        
        vk::Result result;
        
        result = _core->presentQueue()->presentKHR(info);
        _currentFrame = (_currentFrame + 1) % _frameCount;
        

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

    void WindowRenderOutput::begin(vk::CommandBuffer cmd, vk::SubpassContents contents){
        #ifdef RENDERPASS
            auto& frame = _swapchain->frames[_currentFrame];

            static Time::TimePoint start = Time::now();
            Time::Duration a(Time::now() - start);

            float s = static_cast<float>(a.as<Time::milliseconds>().count()) / 1000.f;

            vk::ClearValue clear{
                vk::ClearColorValue(
                    sin(s) * 0.5 + 0.5, cos(s) * 0.5 + 0.5, 0.f, 0.f
                )
            };

            vk::RenderPassBeginInfo info{
                _renderPass,
                frame.framebuffer,
                {
                    {0, 0},
                    _extent
                },
                1, &clear
            };

            cmd.beginRenderPass(info, contents);

        #else
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
        #endif
    }

    void WindowRenderOutput::end(vk::CommandBuffer cmd){
        #ifdef RENDERPASS
            cmd.endRenderPass();
        #else
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
        #endif
    }


    vk::Image WindowRenderOutput::currentColorImage(uint32_t) const{
        return _swapchain->images[_currentImage];
    }

    vk::ImageView WindowRenderOutput::currentColorImageView(uint32_t) const{
        return _swapchain->frames[_currentFrame].imageView;
    }

    vk::Image WindowRenderOutput::currentDepthStencilImage() const{
        return {};
    }

    vk::ImageView WindowRenderOutput::currentDepthStencilImageView() const{
        return {};
    }

    vk::Extent2D WindowRenderOutput::extent() const{
        return _extent;
    }

    Window::WeakWindow WindowRenderOutput::window() const{
        return _window;
    }

    vk::RenderPass WindowRenderOutput::renderPass() const{
        return _renderPass;
    }

    float WindowRenderOutput::scale() const{
        if (auto lock = _window.lock()){
            return lock->scale();
        }
        return 1.f;
    }

    bool WindowRenderOutput::wasAcquired() const{
        return _acquired;
    }

    uint32_t WindowRenderOutput::colorAttachmentCount() const{
        return 1;
    }

    bool WindowRenderOutput::hasDepthAttachment() const{
        return false;
    }

    uint64_t WindowRenderOutput::epoch() const{
        return _epoch;
    }
}