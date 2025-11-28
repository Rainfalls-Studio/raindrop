#include "Raindrop/Modules/Render/RenderOutput/BufferRenderOutput.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop::Render{

    class BufferRenderOutputErrorCategory : public std::error_category{
        public:
            const char* name() const noexcept override{
                return "Render Core";
            }

            std::string message(int ev) const override{
                using ErrorCode = BufferRenderOutput::ErrorCode;

                switch (static_cast<ErrorCode>(ev)){
                    case ErrorCode::FAILED_ATTACHMENT_IMAGE_CREATION: return "Failed to create attachment image";
                    case ErrorCode::FAILED_ATTACHMENT_IMAGE_VIEW_CREATION: return "Failed to create attachment image view";
                    case ErrorCode::FAILED_FRAMEBUFFER_CREATION: return "Failed to create framebuffer";
                    case ErrorCode::FAILED_RENDER_PASS_CREATION: return "Failed to create render pass";
                    default: return "Unknown system error";
                }
            }
    };

    std::error_category& BufferRenderOutput::error_category(){
        static BufferRenderOutputErrorCategory category;
        return category;
    }

    BufferRenderOutput::BufferRenderOutput(std::shared_ptr<RenderCoreModule> core, const Info& info) :
        _core{core},
        _colorDescriptions(info.colorAttachments),
        _depthDescription(info.depthAttachment),
        _sampleCount(info.sampleCount),
        _wantedBufferCount(info.bufferCount),
        _extent(info.extent)
    {}

    BufferRenderOutput::~BufferRenderOutput(){}

    void BufferRenderOutput::destroyBuffers(){
        for (auto& buffer : _buffers){
            destroyBuffer(buffer);
        }
        _buffers.resize(0);
    }

    void BufferRenderOutput::destroyBuffer(Buffer& buffer){
        auto device = _core->deviceManager().device();

        if (buffer.framebuffer){
            device.destroyFramebuffer(buffer.framebuffer);
        }
        
        for (auto& attachment : buffer.colorAttachments){
            destroyAttachment(attachment);
        }

        if (buffer.depthAttachment.has_value()){
            destroyAttachment(buffer.depthAttachment.value());
        }
    }

    void BufferRenderOutput::destroyAttachment(Attachment& attachment){
        auto device = _core->deviceManager().device();
        auto allocator = _core->allocator();

        device.destroyImageView(attachment.imageView);
        vmaDestroyImage(allocator, attachment.image, attachment.allocation);
    }

    std::expected<void, Error> BufferRenderOutput::initialize(Engine& engine [[__maybe_unused__]]){
        createBuffers();
        return createRenderPass();
    }

    void BufferRenderOutput::shutdown(){
        auto device = _core->deviceManager().device();

        destroyBuffers();
        device.destroyRenderPass(_renderPass);
    }

    void BufferRenderOutput::createBuffers(){
        _buffers.resize(_wantedBufferCount);
        _bufferCount = _wantedBufferCount;

        if (_depthDescription.has_value()){
            _clearValues.push_back(_depthDescription->clearValue);
        }

        for (auto& attachment : _colorDescriptions){
            _clearValues.push_back(attachment.clearValue);
        }

        _epoch++;
    }

    std::expected<void, Error> BufferRenderOutput::createRenderPass(){
        auto device = _core->deviceManager().device();

        std::vector<vk::AttachmentDescription> attachments;

        const bool hasDepth = _depthDescription.has_value();
        const bool hasColors = !_colorDescriptions.empty();

        bool isSampled = false;

        if (_depthDescription){
            attachments.emplace_back(vk::AttachmentDescription{
                {},
                _depthDescription->format,
                _sampleCount,
                _depthDescription->load,
                _depthDescription->store,
                vk::AttachmentLoadOp::eDontCare,
                vk::AttachmentStoreOp::eDontCare,
                _depthDescription->initialLayout,
                _depthDescription->finalLayout
            });

            isSampled |= bool(_depthDescription->usage | vk::ImageUsageFlagBits::eSampled);
        }

        for (auto& attachment : _colorDescriptions){
            attachments.emplace_back(vk::AttachmentDescription{
                {},
                attachment.format,
                _sampleCount,
                attachment.load,
                attachment.store,
                vk::AttachmentLoadOp::eDontCare,
                vk::AttachmentStoreOp::eDontCare,
                attachment.initialLayout,
                attachment.finalLayout
            });

            isSampled |= bool(attachment.usage | vk::ImageUsageFlagBits::eSampled);
        }

        std::vector<vk::AttachmentReference> colorRefs;
        std::optional<vk::AttachmentReference> depthRef;

        uint32_t idx=0;

        if (_depthDescription){
            depthRef = vk::AttachmentReference{
                idx++, // if there is a depth attachment, it's index is 0
                vk::ImageLayout::eDepthStencilAttachmentOptimal
            };
        }

        for (auto& attachment [[maybe_unused]] : _colorDescriptions){
            colorRefs.emplace_back(vk::AttachmentReference{
                idx++,
                vk::ImageLayout::eColorAttachmentOptimal
            });
        }

        vk::SubpassDescription subpass{
            {},
            vk::PipelineBindPoint::eGraphics,
            {},
            colorRefs,
            {},
            &depthRef.value(),
            {}
        };

        vk::SubpassDependency subpassDependency{
            vk::SubpassExternal,
            0,
            {},
            {},
            {},
            {},
            vk::DependencyFlagBits::eByRegion
        };

        if (hasDepth){
            subpassDependency.dstAccessMask |= vk::AccessFlagBits::eDepthStencilAttachmentWrite;
            subpassDependency.dstStageMask |= vk::PipelineStageFlagBits::eEarlyFragmentTests;
        }

        if (hasColors){
            subpassDependency.dstStageMask |= vk::PipelineStageFlagBits::eColorAttachmentOutput;
            subpassDependency.dstAccessMask |= vk::AccessFlagBits::eColorAttachmentWrite;
        }

        if (isSampled){
            subpassDependency.srcStageMask |= vk::PipelineStageFlagBits::eFragmentShader;
            subpassDependency.srcAccessMask |= vk::AccessFlagBits::eShaderRead;
        }

        vk::RenderPassCreateInfo info{
            {},
            attachments,
            subpass,
            subpassDependency
        };

        if (auto result = device.createRenderPass(info); result.result != vk::Result::eSuccess){
            return std::unexpected(Error(FailedRenderPassCreationError(), "Failed to create render pass : {}", vk::to_string(result.result)));
        } else {
            _renderPass = result.value;
        }

        return {};
    }

    std::expected<void, Error> BufferRenderOutput::createBuffer(Buffer& buffer){

        // create color attachments
        buffer.colorAttachments.resize(_colorDescriptions.size());
        for (uint32_t i=0; i<_colorDescriptions.size(); i++){
            auto& attachment = buffer.colorAttachments[i];
            auto& description = _colorDescriptions[i];
            
            // create image
            if (auto result = createAttachmentImage(description); result.has_value()){
                auto [image, allocation] = result.value();

                attachment.image = image;
                attachment.allocation = allocation;

            } else {
                return std::unexpected(result.error());
            }
            
            // create image view
            if (auto result = createAttachmentImageView(description, attachment.image, false); result.has_value()){
                attachment.imageView = result.value();
            } else {
                return std::unexpected(result.error());
            }
        }

        if (_depthDescription.has_value()){
            auto& attachment = buffer.depthAttachment.emplace(Attachment{});
            auto& description = _depthDescription.value();

            if (auto result = createAttachmentImage(description); result.has_value()){
                auto [image, allocation] = result.value();

                attachment.image = image;
                attachment.allocation = allocation;
            } else {
                return std::unexpected(result.error());
            }
            
            // create image view
            if (auto result = createAttachmentImageView(description, attachment.image, true); result.has_value()){
                attachment.imageView = result.value();
            } else {
                return std::unexpected(result.error());
            }
        }

        if (auto result = createFramebuffer(buffer); result.has_value()){
            buffer.framebuffer = result.value();
        } else {
            return std::unexpected(result.error());
        }

        buffer.epoch = _epoch;

        return {};
    }

    std::expected<vk::Framebuffer, Error> BufferRenderOutput::createFramebuffer(const Buffer& buffer){
        
        auto device = _core->deviceManager().device();

        std::vector<vk::ImageView> attachments(buffer.colorAttachments.size() + (buffer.depthAttachment.has_value() ? 1 : 0));

        {
            uint32_t index = 0;

            if (buffer.depthAttachment.has_value()){
                attachments[index++] = buffer.depthAttachment->imageView;
            }

            for (uint32_t i=0; i<buffer.colorAttachments.size(); i++){
                attachments[index++] = buffer.colorAttachments[i].imageView;
            }
        }

        vk::FramebufferCreateInfo info{
            {},
            _renderPass,
            attachments,
            _extent.width,
            _extent.height,
            1,
        };

        auto result = device.createFramebuffer(info);

        if (result.result != vk::Result::eSuccess){
            return std::unexpected(Error(FailedFramebufferCreationError(), "Failed to create framebuffer : {}", vk::to_string(result.result)));
        }

        return result.value;
    }


    std::expected<std::pair<vk::Image, VmaAllocation>, Error> BufferRenderOutput::createAttachmentImage(const AttachmentDescription& description){
        auto allocator = _core->allocator();

        vk::ImageCreateInfo info{
            {},
            vk::ImageType::e2D,
            description.format,
            vk::Extent3D{_extent, 1},
            1,
            1,
            _sampleCount,
            vk::ImageTiling::eOptimal,
            description.usage,
            vk::SharingMode::eExclusive,
            0,
            nullptr,
            description.initialLayout
        };

        VmaAllocationCreateInfo allocCreateInfo{
            {},
            VMA_MEMORY_USAGE_GPU_ONLY,
            {},
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            0,
            VK_NULL_HANDLE,
            nullptr,
            0.f
        };

        VmaAllocationInfo allocInfo;
        VmaAllocation allocation;

        VkImage image;
        vk::Result result = vk::Result(vmaCreateImage(
            allocator,
            reinterpret_cast<const VkImageCreateInfo*>(&info),
            &allocCreateInfo,
            &image,
            &allocation,
            &allocInfo
        ));

        if (result !=  vk::Result::eSuccess){
            return std::unexpected(Error(FailedAttachmentImageCreationError(), "Failed to create attachment image : {}", vk::to_string(result)));
        }

        return std::make_pair(image, allocation);
    }

    std::expected<vk::ImageView, Error> BufferRenderOutput::createAttachmentImageView(const AttachmentDescription& description, vk::Image image, bool isDepth){
        auto device = _core->deviceManager().device();

        vk::ImageViewCreateInfo info{
            {},
            image,
            vk::ImageViewType::e2D,
            description.format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                isDepth ? vk::ImageAspectFlagBits::eDepth : vk::ImageAspectFlagBits::eColor,
                0, 1,
                0, 1
            }
        };

        auto result = device.createImageView(info);

        if (result.result != vk::Result::eSuccess){
            return std::unexpected(Error(FailedAttachmentImageViewCreationError(), "Failed to create attachment image view : {}", vk::to_string(result.result)));
        }

        return result.value;
    }

    std::expected<vk::Semaphore, Error> BufferRenderOutput::acquire(vk::Fence fence [[__maybe_unused__]], uint64_t timeout [[__maybe_unused__]]){
        
        auto& buffer = _buffers[_currentIndex];

        if (buffer.epoch != _epoch){
            destroyBuffer(buffer);

            if (auto result = createBuffer(buffer); !result){
                return std::unexpected(result.error());
            }
        }

        _acquired = true;

        return {};
    }

    std::expected<void, Error> BufferRenderOutput::present(vk::Semaphore finishedSemaphore [[__maybe_unused__]]){
        _currentIndex = (_currentIndex + 1) % _bufferCount;
        return {};
    }

    uint32_t BufferRenderOutput::getCurrentBufferIndex() const{
        return _currentIndex;
    }

    uint32_t BufferRenderOutput::getBufferCount() const{
        return _bufferCount;
    }

    void BufferRenderOutput::begin(vk::CommandBuffer cmd, vk::SubpassContents subpassContents){
        auto& buffer = _buffers[_currentIndex];

        vk::Rect2D renderArea{
            {}, _extent
        };

        vk::Rect2D scissor{
            {}, _extent
        };

        vk::RenderPassBeginInfo info{
            _renderPass,
            buffer.framebuffer,
            renderArea,
            _clearValues
        };

        cmd.beginRenderPass(info, subpassContents);
    }

    void BufferRenderOutput::end(vk::CommandBuffer cmd){
        cmd.endRenderPass();
    }

    
    vk::Image BufferRenderOutput::currentColorImage(uint32_t attachment) const{
        return _buffers[_currentIndex].colorAttachments[attachment].image;
    }

    vk::ImageView BufferRenderOutput::currentColorImageView(uint32_t attachment) const{
        return _buffers[_currentIndex].colorAttachments[attachment].imageView;
    }

    vk::Image BufferRenderOutput::currentDepthStencilImage() const{
        auto& depthAttachment = _buffers[_currentIndex].depthAttachment;

        if (depthAttachment.has_value()){
            return depthAttachment->image;
        }

        return {};
    }

    vk::ImageView BufferRenderOutput::currentDepthStencilImageView() const{
        auto& depthAttachment = _buffers[_currentIndex].depthAttachment;

        if (depthAttachment.has_value()){
            return depthAttachment->imageView;
        }

        return {};
    }


    vk::Extent2D BufferRenderOutput::extent() const{
        return _extent;
    }

    vk::RenderPass BufferRenderOutput::renderPass() const{
        return _renderPass;
    }

    float BufferRenderOutput::scale() const{
        return 1.f;
    }

    void BufferRenderOutput::resize(vk::Extent2D newExtent){
        _extent = newExtent;
        _epoch++;
    }

    bool BufferRenderOutput::wasAcquired() const{
        return _acquired;
    }


    uint32_t BufferRenderOutput::colorAttachmentCount() const{
        return static_cast<uint32_t>(_colorDescriptions.size());
    }

    bool BufferRenderOutput::hasDepthAttachment() const{
        return _depthDescription.has_value();
    }

    uint64_t BufferRenderOutput::epoch() const{
        return _epoch;
    }
}