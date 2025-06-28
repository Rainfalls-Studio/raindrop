#pragma once

#include "../RenderPass.hpp"

#include <vector>
#include <memory>

namespace Raindrop::Graphics::Backend::Vulkan{
    struct Context;
    
    namespace Builder{
        class RenderPassBuilder{
            private:
                struct SubpassData{
                    VkAttachmentReference depth;
                    std::vector<VkAttachmentReference> colorAttachments;
                    std::vector<VkAttachmentReference> inputAttachments;
                    std::vector<uint32_t> preserveAttachments;
                };

            public:
                class AttachmentDescription{
                    public:
                        AttachmentDescription(VkAttachmentDescription& description, const uint32_t& index) noexcept;

                        AttachmentDescription& setFlags(const VkAttachmentDescriptionFlags& flags) noexcept;
                        AttachmentDescription& setFormat(const VkFormat& format) noexcept;
                        AttachmentDescription& setLoadOperation(const VkAttachmentLoadOp& operation) noexcept;
                        AttachmentDescription& setStoreOperation(const VkAttachmentStoreOp& operation) noexcept;
                        AttachmentDescription& setStencilLoadOperation(const VkAttachmentLoadOp& operation) noexcept;
                        AttachmentDescription& setStencilStoreOperation(const VkAttachmentStoreOp& operation) noexcept;
                        AttachmentDescription& setInitialLayout(const VkImageLayout& layout) noexcept;
                        AttachmentDescription& setFinalLayout(const VkImageLayout& layout) noexcept;

                        VkAttachmentDescription& get() noexcept;
                        const uint32_t& getIndex() const noexcept;

                    private:
                        VkAttachmentDescription* _description;
                        uint32_t _index;
                };

                class SubpassDescription{
                    friend class RenderPass;
                    public:

                        static SubpassDescription External;

                        SubpassDescription(VkSubpassDescription& description, SubpassData& data, const uint32_t& index) noexcept;
                        
                        SubpassDescription& setFlags(const VkSubpassDescriptionFlags& flags) noexcept;
                        SubpassDescription& setDepthAttachment(const AttachmentDescription& attachment, VkImageLayout layout);
                        SubpassDescription& addColorAttachment(const AttachmentDescription& attachment, VkImageLayout layout);
                        SubpassDescription& addInputAttachment(const AttachmentDescription& attachment, VkImageLayout layout);
                        SubpassDescription& addPreserveAttachment(const AttachmentDescription& attachment);

                        VkSubpassDescription& get() noexcept;
                        const uint32_t& getIndex() const noexcept;

                    private:
                        SubpassDescription();

                        VkSubpassDescription* _description;
                        SubpassData* _data;
                        uint32_t _index;
                };

                class Dependency{
                    friend class RenderPass;
                    public:
                        Dependency(VkSubpassDependency& dependency) noexcept;

                        Dependency& setFlags(const VkDependencyFlags& flags) noexcept;
                        Dependency& setSrcSubpass(const SubpassDescription& subpass) noexcept;
                        Dependency& setDstSubpass(const SubpassDescription& subpass) noexcept;
                        Dependency& setSrcStage(const VkPipelineStageFlags& stage) noexcept;
                        Dependency& setDstStage(const VkPipelineStageFlags& stage) noexcept;
                        Dependency& setSrcAccess(const VkAccessFlags& access) noexcept;
                        Dependency& setDstAccess(const VkAccessFlags& access) noexcept;

                        VkSubpassDependency& get() noexcept;

                    private:
                        VkSubpassDependency* _dependency;
                };

                RenderPassBuilder(Context& context);
                ~RenderPassBuilder();

                std::shared_ptr<RenderPass> create();

                void setCreateFlags(const VkRenderPassCreateFlags& flags);

                AttachmentDescription addAttachment();
                SubpassDescription addSubpass();
                Dependency addDependency();



            private:
                Context* _context;

                VkRenderPassCreateFlags _flags;
                std::vector<VkAttachmentDescription> _attachmentDescriptions;
                std::vector<VkSubpassDescription> _subpassDescriptions;
                std::vector<VkSubpassDependency> _subpassDependencies;
                std::vector<SubpassData> _subpassData;

                VkRenderPassCreateInfo generateInfo() const;
        };
    }
}