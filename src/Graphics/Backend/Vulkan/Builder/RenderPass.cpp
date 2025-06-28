#include "Raindrop/Graphics/Backend/Vulkan/Builder/RenderPass.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/Context.hpp"

namespace Raindrop::Graphics::Backend::Vulkan::Builder{
	RenderPassBuilder::AttachmentDescription::AttachmentDescription(VkAttachmentDescription& description, const std::uint32_t& index) noexcept :
        _description{&description},
        _index{index}
    {}

    RenderPassBuilder::AttachmentDescription& RenderPassBuilder::AttachmentDescription::setFlags(const VkAttachmentDescriptionFlags& flags) noexcept{
        _description->flags = flags;
        return *this;
    }

    RenderPassBuilder::AttachmentDescription& RenderPassBuilder::AttachmentDescription::setFormat(const VkFormat& format) noexcept{
        _description->format = format;
        return *this;
    }

    RenderPassBuilder::AttachmentDescription& RenderPassBuilder::AttachmentDescription::setLoadOperation(const VkAttachmentLoadOp& operation) noexcept{
        _description->loadOp = operation;
        return *this;
    }

    RenderPassBuilder::AttachmentDescription& RenderPassBuilder::AttachmentDescription::setStoreOperation(const VkAttachmentStoreOp& operation) noexcept{
        _description->storeOp = operation;
        return *this;
    }

    RenderPassBuilder::AttachmentDescription& RenderPassBuilder::AttachmentDescription::setStencilLoadOperation(const VkAttachmentLoadOp& operation) noexcept{
        _description->stencilLoadOp = operation;
        return *this;
    }

    RenderPassBuilder::AttachmentDescription& RenderPassBuilder::AttachmentDescription::setStencilStoreOperation(const VkAttachmentStoreOp& operation) noexcept{
        _description->stencilStoreOp = operation;
        return *this;
    }

    RenderPassBuilder::AttachmentDescription& RenderPassBuilder::AttachmentDescription::setInitialLayout(const VkImageLayout& layout) noexcept{
        _description->initialLayout = layout;
        return *this;
    }

    RenderPassBuilder::AttachmentDescription& RenderPassBuilder::AttachmentDescription::setFinalLayout(const VkImageLayout& layout) noexcept{
        _description->finalLayout = layout;
        return *this;
    }


    VkAttachmentDescription& RenderPassBuilder::AttachmentDescription::get() noexcept{
        return *_description;
    }

    const std::uint32_t& RenderPassBuilder::AttachmentDescription::getIndex() const noexcept{
        return _index;
    }


    // ==================================== SUBPASS DESCRIPTION

    RenderPassBuilder::SubpassDescription RenderPassBuilder::SubpassDescription::External;

    RenderPassBuilder::SubpassDescription::SubpassDescription() : 
        _description{nullptr},
        _data{nullptr},
        _index{0}
    {}

    RenderPassBuilder::SubpassDescription::SubpassDescription(VkSubpassDescription& description, SubpassData& data, const std::uint32_t& index) noexcept :
        _description{&description},
        _data{&data},
        _index{index}
    {}
                    
    RenderPassBuilder::SubpassDescription& RenderPassBuilder::SubpassDescription::setFlags(const VkSubpassDescriptionFlags& flags) noexcept{
        _description->flags = flags;
        return *this;
    }

    RenderPassBuilder::SubpassDescription& RenderPassBuilder::SubpassDescription::setDepthAttachment(const AttachmentDescription& attachment, VkImageLayout layout){
        _data->depth = VkAttachmentReference{
            .attachment = attachment.getIndex(),
            .layout = layout
        };

        _description->pDepthStencilAttachment = &_data->depth;
        return *this;
    }

    RenderPassBuilder::SubpassDescription& RenderPassBuilder::SubpassDescription::addColorAttachment(const AttachmentDescription& attachment, VkImageLayout layout){
        _data->colorAttachments.push_back(VkAttachmentReference{
            .attachment = attachment.getIndex(),
            .layout = layout
        });

        _description->pColorAttachments = _data->colorAttachments.data();
        _description->colorAttachmentCount = static_cast<uint32_t>(_data->colorAttachments.size());

        return *this;
    }

    RenderPassBuilder::SubpassDescription& RenderPassBuilder::SubpassDescription::addInputAttachment(const AttachmentDescription& attachment, VkImageLayout layout){
        _data->inputAttachments.push_back(VkAttachmentReference{
            .attachment = attachment.getIndex(),
            .layout = layout
        });

        _description->pInputAttachments = _data->inputAttachments.data();
        _description->inputAttachmentCount = static_cast<uint32_t>(_data->inputAttachments.size());

        return *this;
    }

    RenderPassBuilder::SubpassDescription& RenderPassBuilder::SubpassDescription::addPreserveAttachment(const AttachmentDescription& attachment){
        _data->preserveAttachments.push_back(attachment.getIndex());

        _description->pPreserveAttachments = _data->preserveAttachments.data();
        _description->preserveAttachmentCount = static_cast<uint32_t>(_data->preserveAttachments.size());

        return *this;
    }

    VkSubpassDescription& RenderPassBuilder::SubpassDescription::get() noexcept{
        return *_description;
    }

    const std::uint32_t& RenderPassBuilder::SubpassDescription::getIndex() const noexcept{
        return _index;
    }

    // ==================================== SUBPASS DEPENDENCY

    RenderPassBuilder::Dependency::Dependency(VkSubpassDependency& dependency) noexcept: 
        _dependency{&dependency}
    {}

    RenderPassBuilder::Dependency& RenderPassBuilder::Dependency::setFlags(const VkDependencyFlags& flags) noexcept{
        _dependency->dependencyFlags = flags;
        return *this;
    }

    RenderPassBuilder::Dependency& RenderPassBuilder::Dependency::setSrcSubpass(const SubpassDescription& subpass) noexcept{
        _dependency->srcSubpass = subpass.getIndex();
        return *this;
    }

    RenderPassBuilder::Dependency& RenderPassBuilder::Dependency::setDstSubpass(const SubpassDescription& subpass) noexcept{
        _dependency->dstAccessMask = subpass.getIndex();
        return *this;
    }

    RenderPassBuilder::Dependency& RenderPassBuilder::Dependency::setSrcStage(const VkPipelineStageFlags& stage) noexcept{
        _dependency->srcStageMask = stage;
        return *this;
    }

    RenderPassBuilder::Dependency& RenderPassBuilder::Dependency::setDstStage(const VkPipelineStageFlags& stage) noexcept{
        _dependency->dstStageMask = stage;
        return *this;
    }

    RenderPassBuilder::Dependency& RenderPassBuilder::Dependency::setSrcAccess(const VkAccessFlags& access) noexcept{
        _dependency->srcAccessMask = access;
        return *this;
    }

    RenderPassBuilder::Dependency& RenderPassBuilder::Dependency::setDstAccess(const VkAccessFlags& access) noexcept{
        _dependency->dstAccessMask = access;
        return *this;
    }

    VkSubpassDependency& RenderPassBuilder::Dependency::get() noexcept{
        return *_dependency;
    }


    RenderPassBuilder::RenderPassBuilder(Context& context) :
        _context{&context},
        _flags{0}
    {}

    RenderPassBuilder::~RenderPassBuilder(){}

    std::shared_ptr<RenderPass> RenderPassBuilder::create(){
        VkRenderPassCreateInfo info = generateInfo();

        auto& device = _context->device;
        VkRenderPass renderPass;

        if (vkCreateRenderPass(device, &info, nullptr, &renderPass) != VK_SUCCESS){
            throw std::runtime_error("Failed to create render pass");
        }

        return std::make_shared<RenderPass>(*_context, renderPass);
    }

	void RenderPassBuilder::setCreateFlags(const VkRenderPassCreateFlags& flags){
        _flags = flags;
	}

    RenderPassBuilder::AttachmentDescription RenderPassBuilder::addAttachment(){
		static constexpr VkAttachmentDescription DEFAULT{
			.flags = 0,
			.format = VK_FORMAT_UNDEFINED,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_UNDEFINED
		};

		std::uint32_t index = static_cast<std::uint32_t>(_attachmentDescriptions.size());
		VkAttachmentDescription& description = _attachmentDescriptions.emplace_back(DEFAULT);

		return AttachmentDescription(description, index);
	}

	RenderPassBuilder::SubpassDescription RenderPassBuilder::addSubpass(){
		static constexpr VkSubpassDescription DEFAULT{
			.flags = 0,
			.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.inputAttachmentCount = 0,
			.pInputAttachments = nullptr,
			.colorAttachmentCount = 0,
			.pColorAttachments = nullptr,
			.pResolveAttachments = nullptr,
			.pDepthStencilAttachment = nullptr,
			.preserveAttachmentCount = 0,
			.pPreserveAttachments = nullptr
		};
		
		std::uint32_t index = static_cast<std::uint32_t>(_subpassDescriptions.size());
		VkSubpassDescription& description = _subpassDescriptions.emplace_back(DEFAULT);
		SubpassData& data = _subpassData.emplace_back();

		return SubpassDescription(description, data, index);
	}

	RenderPassBuilder::Dependency RenderPassBuilder::addDependency(){
		static constexpr VkSubpassDependency DEFAULT{
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = VK_SUBPASS_EXTERNAL,
			.srcStageMask = 0,
			.dstStageMask = 0,
			.srcAccessMask = 0,
			.dstAccessMask = 0,
			.dependencyFlags = 0
		};


		VkSubpassDependency& dependency = _subpassDependencies.emplace_back(DEFAULT);
		return Dependency(dependency);
	}

	VkRenderPassCreateInfo RenderPassBuilder::generateInfo() const{
		return VkRenderPassCreateInfo{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			.pNext = nullptr,
			.flags = _flags,
			.attachmentCount = static_cast<uint32_t>(_attachmentDescriptions.size()),
			.pAttachments = _attachmentDescriptions.data(),
			.subpassCount = static_cast<uint32_t>(_subpassDescriptions.size()),
			.pSubpasses = _subpassDescriptions.data(),
			.dependencyCount = static_cast<uint32_t>(_subpassDependencies.size()),
			.pDependencies = _subpassDependencies.data()
		};
	}
}