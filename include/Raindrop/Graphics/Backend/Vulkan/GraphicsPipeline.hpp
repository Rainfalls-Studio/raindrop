#pragma once

#include <Raindrop/pch.pch>

#include "PipelineLayout.hpp"
#include "ShaderModule.hpp"
#include <Raindrop/Graphics/Core/Render/RenderPass.hpp>

namespace Raindrop::Graphics{
	class Context;

	namespace Core::Pipeline{
		class GraphicsPipeline{
			private:
				struct BuildInfo{
					struct VertexInputData{
						std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions;
						std::vector<VkVertexInputAttributeDescription> vertexAttributeDescription;
					} vertexInputData;

					struct ViewportData{
						std::vector<VkViewport> viewports;
						std::vector<VkRect2D> scissors;
					} viewportData;

					struct MultisampleData{
						static constexpr std::size_t SAMPLE_MASK_SIZE = VK_SAMPLE_COUNT_64_BIT / (sizeof(VkSampleMask) * 8);
						VkSampleMask mask[SAMPLE_MASK_SIZE];
					} multisampleData;

					struct ColorBlendData{
						std::vector<VkPipelineColorBlendAttachmentState> attachments;
					} colorBlendData;

					struct DynamicStateData{
						std::vector<VkDynamicState> dynamicStates;
					} dynamicStateData;

					struct ShaderStageData{
						std::list<std::string> entryPoints;
						std::list<std::shared_ptr<ShaderModule>> modules;
						std::vector<VkSpecializationInfo> specializations;
					} shaderStageData;

					VkPipelineCreateFlags flags;
					std::shared_ptr<RenderPass> renderPass;
					std::shared_ptr<PipelineLayout> layout;
					std::uint32_t subpass;

					std::vector<VkPipelineShaderStageCreateInfo> stages;
					std::optional<VkPipelineVertexInputStateCreateInfo> vertexInputState;
					std::optional<VkPipelineInputAssemblyStateCreateInfo> inputAssemblyState;
					std::optional<VkPipelineTessellationStateCreateInfo> tessellationState;
					std::optional<VkPipelineViewportStateCreateInfo> viewportState;
					std::optional<VkPipelineRasterizationStateCreateInfo> rasterizationState;
					std::optional<VkPipelineMultisampleStateCreateInfo> multisampleState;
					std::optional<VkPipelineDepthStencilStateCreateInfo> depthStencilState;
					std::optional<VkPipelineColorBlendStateCreateInfo> colorBlendState;
					std::optional<VkPipelineDynamicStateCreateInfo> dynamicState;
				};

			public:

				class Viewport{
					friend class GraphicsPipeline;
					public:
						Viewport(VkViewport& viewport) noexcept;

						Viewport(const Viewport&) = delete;
						Viewport& operator=(const Viewport&) = delete;

						Viewport(Viewport&& other) noexcept;
						Viewport& operator=(Viewport&& other) noexcept;

						friend void swap(Viewport& A, Viewport& B) noexcept;

						Viewport& setX(const float& x) noexcept;
						Viewport& setY(const float& y) noexcept;
						Viewport& setPosition(const float& x, const float& y) noexcept;
						Viewport& setWidth(const float& width) noexcept;
						Viewport& setHeight(const float& height) noexcept;
						Viewport& setSize(const float& width, const float& height) noexcept;
						Viewport& setMinDepth(const float& minDepth) noexcept;
						Viewport& setMaxDepth(const float& maxDepth) noexcept;
						Viewport& setDepthBounds(const float& min, const float& max) noexcept;

						VkViewport& get() noexcept;
						
					private:
						VkViewport* _viewport;
				};


				class Scissor{
					friend class GraphicsPipeline;
					public:
						Scissor(VkRect2D& rect) noexcept;

						Scissor(const Scissor&) = delete;
						Scissor& operator=(const Scissor&) = delete;

						Scissor(Scissor&& other) noexcept;
						Scissor& operator=(Scissor&& other) noexcept;

						friend void swap(Scissor& A, Scissor& B) noexcept;

						Scissor& setX(const std::int32_t& x) noexcept;
						Scissor& setY(const std::int32_t& y) noexcept;
						Scissor& setPosition(const std::int32_t& x, const std::int32_t& y) noexcept;
						Scissor& setWidth(const std::uint32_t& width) noexcept;
						Scissor& setHeight(const std::uint32_t& height) noexcept;
						Scissor& setSize(const std::uint32_t& width, const std::uint32_t& height) noexcept;

						VkRect2D& get() noexcept;
					
					private:
						VkRect2D* _rect;
				};


				class ColorAttachment{
					friend class GraphicsPipeline;
					public:
						ColorAttachment(VkPipelineColorBlendAttachmentState& description) noexcept;

						ColorAttachment& enableBlending(const bool& enable = true) noexcept;
						ColorAttachment& setSrcColorBlendFactor(const VkBlendFactor& srcColorBlendFactor) noexcept;
						ColorAttachment& setDstColorBlendFactor(const VkBlendFactor& dstColorBlendFactor) noexcept;
						ColorAttachment& setColorBlendOp(const VkBlendOp& colorBlendOp) noexcept;
						ColorAttachment& setSrcAlphaBlendFactor(const VkBlendFactor& srcAlphaBlendFactor) noexcept;
						ColorAttachment& setDstAlphaBlendFactor(const VkBlendFactor& dstAlphaBlendFactor) noexcept;
						ColorAttachment& setAlphaBlendOp(const VkBlendOp& alphaBlendOp) noexcept;
						ColorAttachment& setWriteMask(const VkColorComponentFlags& writeMask) noexcept;

						VkPipelineColorBlendAttachmentState& get() noexcept;

					private:
						VkPipelineColorBlendAttachmentState& _description;
				};


				class StencilOpState{
					friend class GraphicsPipeline;
					public:
						StencilOpState(VkStencilOpState& state) noexcept;

						StencilOpState& setFailOp(const VkStencilOp& failOp) noexcept;
						StencilOpState& setPassOp(const VkStencilOp& passOp) noexcept;
						StencilOpState& setDepthFailOp(const VkStencilOp& depthFailOp) noexcept;
						StencilOpState& setCompareOp(const VkCompareOp& compareOp) noexcept;
						StencilOpState& setCompareMask(const std::uint32_t& compareMask) noexcept;
						StencilOpState& setWriteMask(const std::uint32_t& writeMask) noexcept;
						StencilOpState& setReference(const std::uint32_t& reference) noexcept;

						VkStencilOpState& get() noexcept;

					private:
						VkStencilOpState& _state;
				};


				class VertexInputState{
					friend class GraphicsPipeline;
					public:
						VertexInputState() noexcept;
						VertexInputState(VkPipelineVertexInputStateCreateInfo& info, BuildInfo::VertexInputData& data) noexcept;

						VertexInputState& setFlags(const VkPipelineVertexInputStateCreateFlags& flags);
						VertexInputState& addBindingDescriptions(const std::initializer_list<VkVertexInputBindingDescription>& descriptions);
						VertexInputState& addAttributeDescriptions(const std::initializer_list<VkVertexInputAttributeDescription>& descriptions);
						VertexInputState& setLayout(const VertexLayout& layout);

						VkPipelineVertexInputStateCreateInfo& get();

					private:
						VkPipelineVertexInputStateCreateInfo* _info;
						BuildInfo::VertexInputData* _data;

						VkPipelineVertexInputStateCreateInfo& getInfo();
						BuildInfo::VertexInputData& getData();
				};


				class InputAssemblyState{
					friend class GraphicsPipeline;
					public:
						InputAssemblyState() noexcept;
						InputAssemblyState(VkPipelineInputAssemblyStateCreateInfo& info) noexcept;

						InputAssemblyState& setFlags(const VkPipelineInputAssemblyStateCreateFlags& flags);
						InputAssemblyState& setPrimitiveTopology(const VkPrimitiveTopology& topology);
						InputAssemblyState& enablePrimitiveRestart(const bool& enable = true);

						VkPipelineInputAssemblyStateCreateInfo& get();

					private:
						VkPipelineInputAssemblyStateCreateInfo* _info;

						VkPipelineInputAssemblyStateCreateInfo& getInfo();
				};


				class TessellationState{
					friend class GraphicsPipeline;
					public:
						TessellationState() noexcept;
						TessellationState(VkPipelineTessellationStateCreateInfo& info) noexcept;
						
						TessellationState& setFlags(const VkPipelineTessellationStateCreateFlags& flags);
						TessellationState& setPatchControlPoints(const std::uint32_t& points);

						VkPipelineTessellationStateCreateInfo& get();
					
					private:
						VkPipelineTessellationStateCreateInfo* _info;

						VkPipelineTessellationStateCreateInfo& getInfo();
				};


				class ViewportState{
					friend class GraphicsPipeline;
					public:
						ViewportState() noexcept;
						ViewportState(VkPipelineViewportStateCreateInfo& info, BuildInfo::ViewportData& data) noexcept;

						ViewportState& setFlags(const VkPipelineViewportStateCreateFlags& flags) noexcept;
						Viewport addViewport();
						Scissor addScissor();

						VkPipelineViewportStateCreateInfo& get();

					private:
						VkPipelineViewportStateCreateInfo* _info;
						BuildInfo::ViewportData* _data;

						VkPipelineViewportStateCreateInfo& getInfo();
						BuildInfo::ViewportData& getData();
				};

				
				class RasterizationState{
					friend class GraphicsPipeline;
					public:
						RasterizationState() noexcept;
						RasterizationState(VkPipelineRasterizationStateCreateInfo& info) noexcept;

						RasterizationState& setFlags(const VkPipelineRasterizationStateCreateFlags& flags);
						RasterizationState& enableDepthClamp(const bool& enable = true);
						RasterizationState& enableRasterizerDiscard(const bool& enable = true);
						RasterizationState& setPolygoneMode(const VkPolygonMode& mode);
						RasterizationState& setCullMode(const VkCullModeFlags& mode);
						RasterizationState& setFrontFace(const VkFrontFace& frontFace);
						RasterizationState& enableDepthBias(const bool& enable = true);
						RasterizationState& setDepthBiasConstant(const float& factor);
						RasterizationState& setDepthBiasSlop(const float& factor);
						RasterizationState& setDepthBiasClamp(const float& clamp);
						RasterizationState& setLineWidth(const float& width);

						VkPipelineRasterizationStateCreateInfo& get();

					private:
						VkPipelineRasterizationStateCreateInfo* _info;

						VkPipelineRasterizationStateCreateInfo& getInfo();
				};


				class MultisampleState{
					friend class GraphicsPipeline;
					public:
						MultisampleState() noexcept;
						MultisampleState(VkPipelineMultisampleStateCreateInfo& info, BuildInfo::MultisampleData& data) noexcept;

						MultisampleState& setFlags(VkPipelineMultisampleStateCreateFlags& flags);
						MultisampleState& setRasterizationSamples(const VkSampleCountFlagBits& samples);
						MultisampleState& enableSampleShading(const bool& enable = true);
						MultisampleState& setMinSampleShading(const float& min);
						MultisampleState& setSampleMask(const VkSampleMask mask[BuildInfo::MultisampleData::SAMPLE_MASK_SIZE]);
						MultisampleState& enableAlphaToCoverage(const bool& enable = true);
						MultisampleState& enableAlphaToOne(const bool& enable = true);

						VkPipelineMultisampleStateCreateInfo& get();

					private:
						VkPipelineMultisampleStateCreateInfo* _info;
						BuildInfo::MultisampleData* _data;

						VkPipelineMultisampleStateCreateInfo& getInfo();
						BuildInfo::MultisampleData& getData();
				};


				class DepthStencilState{
					friend class GraphicsPipeline;
					public:
						DepthStencilState() noexcept;
						DepthStencilState(VkPipelineDepthStencilStateCreateInfo& info) noexcept;

						DepthStencilState& setFlags(const VkPipelineDepthStencilStateCreateFlags& flags);
						DepthStencilState& enableDepthTest(const bool& enable = true);
						DepthStencilState& enableDepthWrite(const bool& enable = true);
						DepthStencilState& setDepthCompare(const VkCompareOp& operation);
						DepthStencilState& enableDepthBoundsTest(const bool& enable = true);
						DepthStencilState& enableStencilTest(const bool& enable = true);
						StencilOpState setFrontStencilOperations();
						StencilOpState setBackStencilOperations();
						DepthStencilState& setMinDepthBounds(const float& min);
						DepthStencilState& setMaxDepthBounds(const float& max);
						DepthStencilState& setDepthBounds(const float& min, const float& max);

						VkPipelineDepthStencilStateCreateInfo& get();

					private:
						VkPipelineDepthStencilStateCreateInfo* _info;

						VkPipelineDepthStencilStateCreateInfo& getInfo();
				};
				

				class ColorBlendState{
					friend class GraphicsPipeline;
					public:
						ColorBlendState() noexcept;
						ColorBlendState(VkPipelineColorBlendStateCreateInfo& info, BuildInfo::ColorBlendData& data) noexcept;

						ColorBlendState& setFlags(const VkPipelineColorBlendStateCreateFlags& flags);
						ColorBlendState& enableLogicOperation(const bool& enable = true);
						ColorBlendState& setLogicOperation(const VkLogicOp& operation);
						ColorBlendState& setBlendConstants(const glm::vec4& constants);
						ColorAttachment addColorAttachment();

						VkPipelineColorBlendStateCreateInfo& get();

					private:
						VkPipelineColorBlendStateCreateInfo* _info;
						BuildInfo::ColorBlendData* _data;

						VkPipelineColorBlendStateCreateInfo& getInfo();
						BuildInfo::ColorBlendData& getData();
				};

				class DynamicState{
					public:
						DynamicState() noexcept;
						DynamicState(VkPipelineDynamicStateCreateInfo& info, BuildInfo::DynamicStateData& data) noexcept;

						DynamicState& setFlags(const VkPipelineDynamicStateCreateFlags& flags);
						DynamicState& addDynamicStates(const std::initializer_list<VkDynamicState>& dynamicStates);

						VkPipelineDynamicStateCreateInfo& get();

					private:
						VkPipelineDynamicStateCreateInfo* _info;
						BuildInfo::DynamicStateData* _data;

						VkPipelineDynamicStateCreateInfo& getInfo();
						BuildInfo::DynamicStateData& getData();
				};

				class ShaderStage{
					public:
						ShaderStage() noexcept;
						ShaderStage(VkPipelineShaderStageCreateInfo& info, BuildInfo::ShaderStageData& data) noexcept;

						ShaderStage& setFlags(const VkPipelineStageFlags& flags);
						ShaderStage& setStage(const VkShaderStageFlagBits& stage);
						ShaderStage& setModule(const std::shared_ptr<ShaderModule>& module);
						ShaderStage& setEntryPoint(const std::string& entryPoint);
						ShaderStage& addSpecialization(const VkSpecializationInfo& specialization);

						VkPipelineShaderStageCreateInfo& get();

					private:
						VkPipelineShaderStageCreateInfo* _info;
						BuildInfo::ShaderStageData* _data;

						VkPipelineShaderStageCreateInfo& getInfo();
						BuildInfo::ShaderStageData& getData();
				};

				static std::shared_ptr<GraphicsPipeline> create(Raindrop::Context& context);

				GraphicsPipeline() noexcept;
				~GraphicsPipeline();

				GraphicsPipeline(const GraphicsPipeline&) = delete;
				GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

				GraphicsPipeline(GraphicsPipeline&& other);
				GraphicsPipeline& operator=(GraphicsPipeline&& other);

				friend void swap(GraphicsPipeline& A, GraphicsPipeline& B);

				void prepare(Context& context);
				void initialize();
				void release();

				GraphicsPipeline& setFlags(const VkPipelineCreateFlags& flags);
				GraphicsPipeline& setRenderPass(const std::shared_ptr<RenderPass>& renderPass);
				GraphicsPipeline& setLayout(const std::shared_ptr<PipelineLayout>& layout);
				GraphicsPipeline& setSubpass(const std::uint32_t& subpass);

				ShaderStage addStage();
				VertexInputState getVertexInputState(const bool& disable = false);
				InputAssemblyState getInputAssemplyState(const bool& disable = false);
				TessellationState getTessellationState(const bool& disable = false);
				ViewportState getViewportState(const bool& disable = false);
				RasterizationState getRasterizationState(const bool& disable = false);
				MultisampleState getMultisampleState(const bool& disable = false);
				DepthStencilState getDepthStencilState(const bool& disable = false);
				ColorBlendState getColorBlendState(const bool& disable = false);
				DynamicState getDynamicState(const bool& disable = false);

				void bind(const CommandBuffer& cmd);

			private:
				Context* _context;
				VkPipeline _pipeline;
				std::shared_ptr<RenderPass> _renderPass;
				std::shared_ptr<PipelineLayout> _pipelineLayout;
				std::list<std::shared_ptr<ShaderModule>> _modules;

				std::unique_ptr<BuildInfo> _info;

				BuildInfo& checkBuild();
		};
	}
}

#endif