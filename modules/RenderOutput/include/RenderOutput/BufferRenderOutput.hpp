#pragma once

#include <VkBootstrap.h>
#include <vulkan/vulkan.hpp>

#include "IRenderOutput.hpp"

namespace Render{
    class BufferRenderOutput : public IRenderOutput{
        public:
            enum class ErrorCode{
                FAILED_ATTACHMENT_IMAGE_CREATION,
                FAILED_ATTACHMENT_IMAGE_VIEW_CREATION,
                FAILED_FRAMEBUFFER_CREATION,
                FAILED_RENDER_PASS_CREATION
            };

            static std::error_category& error_category();

            static inline std::error_code make_error_code(ErrorCode e){
                return {static_cast<int>(e), error_category()};
            }

            static inline std::error_code FailedAttachmentImageCreationError() {return make_error_code(ErrorCode::FAILED_ATTACHMENT_IMAGE_CREATION);}
            static inline std::error_code FailedAttachmentImageViewCreationError() {return make_error_code(ErrorCode::FAILED_ATTACHMENT_IMAGE_VIEW_CREATION);}
            static inline std::error_code FailedFramebufferCreationError() {return make_error_code(ErrorCode::FAILED_FRAMEBUFFER_CREATION);}
            static inline std::error_code FailedRenderPassCreationError() {return make_error_code(ErrorCode::FAILED_RENDER_PASS_CREATION);}

            struct AttachmentDescription{
                std::string name;
                vk::Format format;
                vk::ImageUsageFlags usage;
                vk::ClearValue clearValue;
                vk::AttachmentLoadOp load = vk::AttachmentLoadOp::eClear;
                vk::AttachmentStoreOp store = vk::AttachmentStoreOp::eStore;
                vk::ImageLayout initialLayout = vk::ImageLayout::eUndefined;
                vk::ImageLayout finalLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
            };

            struct Info{
                vk::Extent2D extent;
                std::vector<AttachmentDescription> colorAttachments;
                std::optional<AttachmentDescription> depthAttachment = {};
                vk::SampleCountFlagBits sampleCount = vk::SampleCountFlagBits::e1;
                uint32_t bufferCount = 1;
            };

            BufferRenderOutput(std::shared_ptr<RenderCoreModule> core, const Info& info);
            virtual ~BufferRenderOutput();

            virtual std::expected<void, Raindrop::Error> initialize(Raindrop::Engine& engine) override;
            virtual void shutdown() override;

            virtual std::expected<vk::Semaphore, Raindrop::Error> acquire(vk::Fence fence, uint64_t timeout = UINT64_MAX) override;
            virtual std::expected<void, Raindrop::Error> present(vk::Semaphore finishedSemaphore = {}) override;

            virtual uint32_t getCurrentBufferIndex() const override;
            virtual uint32_t getBufferCount() const override;

            virtual void begin(vk::CommandBuffer cmd, vk::SubpassContents subpassContents = vk::SubpassContents::eInline) override;
            virtual void end(vk::CommandBuffer cmd) override;

            virtual vk::Image currentColorImage(uint32_t attachment) const override;
            virtual vk::ImageView currentColorImageView(uint32_t attachment) const override;
            virtual vk::Image currentDepthStencilImage() const override;
            virtual vk::ImageView currentDepthStencilImageView() const override;
            
            virtual vk::Extent2D extent() const override;
            virtual vk::RenderPass renderPass() const override;
            virtual float scale() const override;

            void resize(vk::Extent2D newExtent);
            virtual bool wasAcquired() const override;

            virtual uint32_t colorAttachmentCount() const override;
            virtual bool hasDepthAttachment() const override;

            virtual uint64_t epoch() const override;
        
        private:
            std::shared_ptr<RenderCoreModule> _core;

            struct Attachment{
                vk::ImageView imageView;
                vk::Image image;
                VmaAllocation allocation;
            };

            struct Buffer{
                vk::Framebuffer framebuffer;
                bool inUse;
                
                uint64_t epoch = 0;
                std::vector<Attachment> colorAttachments;
                std::optional<Attachment> depthAttachment;
            };

            std::vector<AttachmentDescription> _colorDescriptions;
            std::optional<AttachmentDescription> _depthDescription;

            std::vector<vk::ClearValue> _clearValues;

            std::vector<Buffer> _buffers;
            vk::SampleCountFlagBits _sampleCount;
            uint32_t _bufferCount;
            uint32_t _wantedBufferCount;
            uint32_t _currentIndex = 0;
            vk::RenderPass _renderPass;
            vk::Extent2D _extent;
            vk::Extent2D _wantedExtent;
            uint64_t _epoch = 0;
            bool _acquired = false;

            void createBuffers();
            std::expected<void, Raindrop::Error> createRenderPass();
            std::expected<void, Raindrop::Error> createBuffer(Buffer& buffer);
            std::expected<std::pair<vk::Image, VmaAllocation>, Raindrop::Error> createAttachmentImage(const AttachmentDescription& description);
            std::expected<vk::ImageView, Raindrop::Error> createAttachmentImageView(const AttachmentDescription& description, vk::Image image, bool isDepth);
            std::expected<vk::DeviceMemory, Raindrop::Error> allocateAttachmentMemory(const AttachmentDescription& buffer, vk::Image image);
            std::expected<vk::Framebuffer, Raindrop::Error> createFramebuffer(const Buffer& buffer);

            void destroyBuffers();
            void destroyBuffer(Buffer& buffer);
            void destroyAttachment(Attachment& attachment);
    };
}