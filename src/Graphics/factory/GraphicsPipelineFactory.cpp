#include <Raindrop/Graphics/factory/GraphicsPipelineFactory.hpp>
#include <Raindrop/Core/Asset/AssetManager.hpp>
#include <Raindrop/Graphics/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Shader.hpp>
#include <Raindrop/Graphics/builders/GraphicsPipelineBuilder.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

struct CaseInsensitiveHash {
	std::size_t operator()(const std::wstring& str) const {
		std::wstring lowercaseStr;
		lowercaseStr.reserve(str.length());
		for (wchar_t ch : str) {
			lowercaseStr.push_back(std::tolower(ch, std::locale()));
		}
		return std::hash<std::wstring>()(lowercaseStr);
	}
};

struct CaseInsensitiveEqual {
	bool operator()(const std::wstring& str1, const std::wstring& str2) const {
		return std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(),
			[](wchar_t ch1, wchar_t ch2) {
				return std::tolower(ch1, std::locale()) == std::tolower(ch2, std::locale());
			}
		);
	}
};

namespace Raindrop::Graphics::Factory{
	GraphicsPipelineFactory::GraphicsPipelineFactory(GraphicsContext& context) : _context{context}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.Pipeline");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		CLOG(INFO, "Engine.Graphics.Pipeline") << "Creating graphics pipeline asset factory...";
	}

	GraphicsPipelineFactory::~GraphicsPipelineFactory(){
		CLOG(INFO, "Engine.Graphics.Pipeline") << "Destroying graphics pipeline asset factory...";
	}

	std::shared_ptr<Core::Asset::Asset> GraphicsPipelineFactory::createAsset(const std::filesystem::path& path){
		CLOG(INFO, "Engine.Graphics.Pipeline") << "Loading a new graphics pipeline from " << path << " ...";

		std::shared_ptr<GraphicsPipeline> pipeline;
		Format format = getFormat(path);

		switch (format){
			case XML: pipeline = loadFromXML(path); break;
		}

		if (pipeline){
			_pipelines[pipeline->name()] = pipeline;
			return pipeline;
		}

		CLOG(ERROR, "Engine.Graphics.Pipeline") << "Failed to load " << path << " pipeline, unknown graphics pipeline extension";
		throw std::runtime_error("failed to reconise extension");
	}

	void GraphicsPipelineFactory::destroyAsset(std::shared_ptr<Core::Asset::Asset> asset){
		CLOG(INFO, "Engine.Graphics.Pipeline") << "Destroying graphics pipeline...";
		asset.reset();
	}

	void GraphicsPipelineFactory::registerExtensions(const std::shared_ptr<GraphicsPipelineFactory>& factory){
		auto& assetManager = _context->assetManager;

		assetManager.linkFactory(".gfxpipe", factory);
		assetManager.linkFactory(".gfxpipeline", factory);
	}

	GraphicsPipelineFactory::Format GraphicsPipelineFactory::getFormat(const std::filesystem::path& path){
		std::wregex extensionRegex(L"\\.([a-zA-Z0-9]+)");
		std::wstring strPath = path.wstring();

		std::wsmatch extensionsMatch;
		auto searchStart = strPath.cbegin();

		while (std::regex_search(searchStart, strPath.cend(), extensionsMatch, extensionRegex)) {
			std::wstring extension = extensionsMatch[1].str();
			Format format = getExtentionFormat(extension);
			if (format != NONE) return format;
			searchStart = extensionsMatch.suffix().first;
		}
		return NONE;
	}

	class GraphicsPipelineFactory::ExtensionToFormat{
		public:
			ExtensionToFormat(){
				hashMap[L"xml"] = XML;
			}

			std::unordered_map<std::wstring, GraphicsPipelineFactory::Format, CaseInsensitiveHash, CaseInsensitiveEqual> hashMap;
	};

	GraphicsPipelineFactory::Format GraphicsPipelineFactory::getExtentionFormat(const std::wstring extension){
		static ExtensionToFormat map;
		return map.hashMap[extension];
	}

	std::shared_ptr<GraphicsPipeline> GraphicsPipelineFactory::loadFromXML(const std::filesystem::path& path){
		auto& assetManager = _context->assetManager;
		auto& registry = _context->registry;

		CLOG(INFO, "Engine.Graphics.Pipeline") << "Loading " << path << " xml graphics pipeline";

		tinyxml2::XMLDocument document;
		if (!document.LoadFile(path.generic_u8string().c_str()) == tinyxml2::XML_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.Pipeline") << "Failed to open " << path << ". Reason : " << document.ErrorStr();
			throw std::runtime_error("Failed to open graphics pipline xml file");
		}

		Builders::GraphicsPipelineBuilder builder;
		auto node = document.FirstChildElement("GraphicsPipeline");

		if (!node){
			CLOG(ERROR, "Engine.Graphics.Pipeline") << "The file " << path << " does not contain a GraphicsPipeline element, cannot load pipeline";
			throw std::runtime_error("Cannot find GraphicsPipeline element");
		}

		getShadersXML(node->FirstChildElement("Shaders"), builder);
		getTargetXML(node->FirstChildElement("Target"), builder);
		getRasterisationInfoXML(node->FirstChildElement("Rasterization"), builder);
		getMultisampleInfoXML(node->FirstChildElement("Multisample"), builder);
		getAssemnlyInfoXML(node->FirstChildElement("Assembly"), builder);
		getDepthStencilInfoXML(node->FirstChildElement("DepthStencil"), builder);
		getColorBlendInfoXML(node->FirstChildElement("ColorBlend"), builder);
		getTessellationInfoXML(node->FirstChildElement("Tessellation"), builder);

		return builder.build(_context);
	}

	void GraphicsPipelineFactory::getShadersXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder){
		if (!element){
			CLOG(ERROR, "Engine.Graphics.Pipeline") << "Cannot create a pipeline without any shader, Failed to find shaders node";
			throw std::runtime_error("Cannot create a pipeline without any shader");
		}

		for (auto c = element->FirstChildElement("Shader"); c!=nullptr; c=c->NextSiblingElement("Shader")){
			std::shared_ptr<Shader> shader;
			try{
				shader = getShaderXML(c);
			} catch (const std::exception &){
				continue;
			}
			builder.addShader(shader);
		}
	}

	std::shared_ptr<Shader> GraphicsPipelineFactory::getShaderXML(tinyxml2::XMLElement* shaderElement){
		auto& assetManager = _context->assetManager;
		auto& registry = _context->registry;

		const char* source = shaderElement->Attribute("source");
		if (!source){
			CLOG(WARNING, "Engine.Graphics.Pipeline") << "The xml shader node does node contain a \"source\" attribute. This might result in errors.";
			throw std::runtime_error("the shader node does not contain \"source\" attribute.");
		}

		return assetManager.loadOrGet<Shader>(registry.formatString(source)).lock();
	}

	void GraphicsPipelineFactory::getTargetXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder){
		if (!element){
			CLOG(ERROR, "Engine.Graphics.Pipeline") << "Cannot create a pipeline without a target";
			throw std::runtime_error("Cannot create a pipeline without a target");
		}

		const char* id;
		if (element->QueryStringAttribute("id", &id) != tinyxml2::XML_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.Pipeline") << "Cannot create a pipeline without a target id";
			throw std::runtime_error("Cannot create a pipeline without a target id");
		}

		try{
			const Target* target = static_cast<const Target*>(_context.gRegistry.get(id));
			builder.setRenderPass(target->renderPass());
			builder.setAttachmentCount(target->attachmentCount());
		} catch (const std::exception &e){
			CLOG(ERROR, "Engine.Graphics.Pipeline") << "Cannot create a pipeline without a valid target";
			throw std::runtime_error("Cannot create a pipeline without a valid target");
		}
	}

	void GraphicsPipelineFactory::getNameXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder){
		if (!element){
			CLOG(ERROR, "Engine.Graphics.Pipeline") << "Cannot create a graphics pipeline without a name";
			throw std::runtime_error("Cannot create a graphics pipeline without a name");
		}

		const char* name;
		if (element->QueryStringAttribute("name", &name) != tinyxml2::XML_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.Pipeline") << "Cannot find name attribute ";
		}

		builder.setName(name);
	}

	void GraphicsPipelineFactory::getRasterisationInfoXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder){
		if (!element) return;
		auto& info = builder.rasterizationInfo();

		if (auto depthElement = element->FirstChildElement("DepthClamp"))
			info.depthClampEnable = depthElement->BoolAttribute("Enable", info.depthClampEnable);
		
		if (auto rasterizationDiscard = element->FirstChildElement("RasterizerDiscard"))
			info.rasterizerDiscardEnable = rasterizationDiscard->BoolAttribute("Enable", info.rasterizerDiscardEnable);
		
		if (auto polygonMode = element->FirstChildElement("PolygonMode")){
			const char* mode;
			if (polygonMode->QueryStringAttribute("Mode", &mode) == tinyxml2::XML_SUCCESS)
				info.polygonMode = strToVkEnumT(_context.context.registry.formatString(mode), info.polygonMode);
		}
		
		if (auto lineWith = element->FirstChildElement("LineWidth"))
			info.lineWidth = lineWith->FloatAttribute("Width", info.lineWidth);
		
		if (auto cullMode = element->FirstChildElement("CullMode")){
			const char* mode;
			if (cullMode->QueryStringAttribute("Mode", &mode) == tinyxml2::XML_SUCCESS)
				info.cullMode = strToVkEnumT(_context.context.registry.formatString(mode), info.cullMode);
		}
		
		if (auto frontFace = element->FirstChildElement("FrontFace")){
			const char* mode;
			if (frontFace->QueryStringAttribute("FrontFace", &mode) == tinyxml2::XML_SUCCESS)
				info.frontFace = strToVkEnumT(_context.context.registry.formatString(mode), info.frontFace);
		}

		if (auto depthBias = element->FirstChildElement("DepthBias"))
			info.depthBiasEnable = depthBias->BoolAttribute("Enable", info.depthBiasEnable);

		if (auto depthBiasConstantFactor = element->FirstChildElement("DepthBiasConstantFactor"))
			info.depthBiasConstantFactor = depthBiasConstantFactor->FloatAttribute("Factor", info.depthBiasConstantFactor);
		
		if (auto depthBiasClamp = element->FirstChildElement("DepthBiasClamp"))
			info.depthBiasClamp = depthBiasClamp->FloatAttribute("Clamp", info.depthBiasClamp);
		
		if (auto depthBiasSlopeFactor = element->FirstChildElement("depthBiasSlopeFactor"))
			info.depthBiasSlopeFactor = depthBiasSlopeFactor->FloatAttribute("Clamp", info.depthBiasSlopeFactor);
	}

	void GraphicsPipelineFactory::getMultisampleInfoXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder){
		if (!element) return;
		auto& info = builder.multisampleInfo();

		if (auto sampleShading = element->FirstChildElement("SampleShading"))
			info.sampleShadingEnable = sampleShading->BoolAttribute("Enable", info.sampleShadingEnable);
		
		if (auto rasterizationSamples = element->FirstChildElement("RasterizationSamples")){
			const char* mode;
			if (rasterizationSamples->QueryStringAttribute("Samples", &mode) == tinyxml2::XML_SUCCESS)
				info.rasterizationSamples = strToVkEnumT(_context.context.registry.formatString(mode), info.rasterizationSamples);
		}
		
		if (auto minSampleShading = element->FirstChildElement("MinSampleShading"))
			info.minSampleShading = element->FloatAttribute("Minumum", info.minSampleShading);
		
		if (auto alphaToCoverageEnable = element->FirstChildElement("AlphaToCoverage"))
			info.alphaToCoverageEnable = alphaToCoverageEnable->BoolAttribute("Enable", info.alphaToCoverageEnable);
		
		if (auto alphaToOneEnable = element->FirstChildElement("AlphaToOne"))
			info.alphaToOneEnable = alphaToOneEnable->BoolAttribute("Enable", info.alphaToOneEnable);
	}

	void GraphicsPipelineFactory::getAssemnlyInfoXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder){
		if (!element) return;
		auto& info = builder.inputAssemblyInfo();

		if (auto topology = element->FirstChildElement("Topology")){
			const char* mode;
			if (topology->QueryStringAttribute("Topology", &mode) == tinyxml2::XML_SUCCESS)
				info.topology = strToVkEnumT(_context.context.registry.formatString(mode), info.topology);
		}

		if (auto primitiveRestartEnable = element->FirstChildElement("PrimitiveRestart"))
			info.primitiveRestartEnable = primitiveRestartEnable->BoolAttribute("Enable", info.primitiveRestartEnable);
	}

	void GraphicsPipelineFactory::getDepthStencilInfoXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder){
		if (!element) return;
		auto& info = builder.depthStencilInfo();

		if (auto depthTestEnable = element->FirstChildElement("DepthTest"))
			info.depthTestEnable = depthTestEnable->BoolAttribute("Enable", info.depthTestEnable);
		
		if (auto depthWriteEnable = element->FirstChildElement("DepthWrite"))
			info.depthWriteEnable = depthWriteEnable->BoolAttribute("Enable", info.depthWriteEnable);

		if (auto compareOp = element->FirstChildElement("DepthCompare")){
			const char* mode;
			if (compareOp->QueryStringAttribute("Operation", &mode) == tinyxml2::XML_SUCCESS)
				info.depthCompareOp = strToVkEnumT(_context.context.registry.formatString(mode), info.depthCompareOp);
		}

		if (auto depthBoundTestEnable = element->FirstChildElement("DepthBoundTest"))
			info.depthBoundsTestEnable = depthBoundTestEnable->BoolAttribute("Enable", info.depthBoundsTestEnable);
		
		if (auto depthBounds = element->FirstChildElement("DepthBounds")){
			info.minDepthBounds = depthBounds->FloatAttribute("min", info.minDepthBounds);
			info.maxDepthBounds = depthBounds->FloatAttribute("max", info.maxDepthBounds);
		}

		if (auto stencilTestEnable = element->FirstChildElement("StencilTest"))
			info.stencilTestEnable = stencilTestEnable->BoolAttribute("Enable", info.stencilTestEnable);
		
		getDepthStencilOpStateXML(element->FirstChildElement("Front"), info.front);
		getDepthStencilOpStateXML(element->FirstChildElement("Back"), info.back);
	}

	void GraphicsPipelineFactory::getDepthStencilOpStateXML(tinyxml2::XMLElement* element, VkStencilOpState& op){
		if (!element) return;

		if (auto failOp = element->FirstChildElement("Fail")){
			const char* mode;
			if (failOp->QueryStringAttribute("Operation", &mode) == tinyxml2::XML_SUCCESS)
				op.failOp = strToVkEnumT(_context.context.registry.formatString(mode), op.failOp);
		}

		if (auto passOp = element->FirstChildElement("Pass")){
			const char* mode;
			if (passOp->QueryStringAttribute("Operation", &mode) == tinyxml2::XML_SUCCESS)
				op.passOp = strToVkEnumT(_context.context.registry.formatString(mode), op.passOp);
		}

		if (auto depthFailOp = element->FirstChildElement("DepthFail")){
			const char* mode;
			if (depthFailOp->QueryStringAttribute("Operation", &mode) == tinyxml2::XML_SUCCESS)
				op.depthFailOp = strToVkEnumT(_context.context.registry.formatString(mode), op.depthFailOp);
		}

		if (auto compare = element->FirstChildElement("Compare")){
			const char* mode;
			if (compare->QueryStringAttribute("Operation", &mode) == tinyxml2::XML_SUCCESS)
				op.compareOp = strToVkEnumT(_context.context.registry.formatString(mode), op.compareOp);
			
			if (compare->QueryStringAttribute("Mask", &mode) == tinyxml2::XML_SUCCESS)
				op.compareMask = static_cast<uint32_t>(std::bitset<32>(mode).to_ulong());
		}

		if (auto writeMask = element->FirstChildElement("Write")){
			const char* mode;
			if (writeMask->QueryStringAttribute("Mask", &mode) == tinyxml2::XML_SUCCESS)
				op.writeMask = static_cast<uint32_t>(std::bitset<32>(mode).to_ulong());
		}

		if (auto reference = element->FirstChildElement("Reference")){
			const char* mode;
			if (reference->QueryStringAttribute("Mask", &mode) == tinyxml2::XML_SUCCESS)
				op.writeMask = static_cast<uint32_t>(std::bitset<32>(mode).to_ulong());
		}
	}

	void GraphicsPipelineFactory::getColorBlendInfoXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder){
		if (!element) return;
		auto& info = builder.colorBlendInfo();
		
		if (auto logicOpEnabled = element->FirstChildElement("Logic")){
			info.logicOpEnable = element->BoolAttribute("Enable", info.logicOpEnable);

			const char* op;
			if (logicOpEnabled->QueryStringAttribute("Operation", &op) == tinyxml2::XML_SUCCESS)
				info.logicOp = strToVkEnumT(op, info.logicOp);
		}

		if (auto blendConstants = element->FirstChildElement("BlendConstants")){
			info.blendConstants[0] = element->FloatAttribute("r", info.blendConstants[0]);
			info.blendConstants[1] = element->FloatAttribute("g", info.blendConstants[1]);
			info.blendConstants[2] = element->FloatAttribute("b", info.blendConstants[2]);
			info.blendConstants[3] = element->FloatAttribute("a", info.blendConstants[3]);
		}

		for (auto attachment = element->FirstChildElement("Attachment"); attachment!=nullptr; attachment = attachment->NextSiblingElement("Attachment")){
			int id = attachment->IntAttribute("ID", -1);
			if (id == -1){
				CLOG(WARNING, "Engine.Graphics.Pipeline") << "Cannot setup a non indentified attachment";
				continue;
			}

			auto& info = builder.attachmentState(id);

			if (auto blendEnable = attachment->FirstChildElement("Blend"))
				info.blendEnable = blendEnable->BoolAttribute("Enable", info.blendEnable);
			
			if (auto srcColorBlendFactor = attachment->FirstChildElement("SrcColorBlend")){
				const char* factor;
				if (srcColorBlendFactor->QueryStringAttribute("Factor", &factor) == tinyxml2::XML_SUCCESS)
					info.srcColorBlendFactor = strToVkEnumT(_context.context.registry.formatString(factor), info.srcColorBlendFactor);
			}

			if (auto dstColorBlendFactor = attachment->FirstChildElement("DstColorBlend")){
				const char* factor;
				if (dstColorBlendFactor->QueryStringAttribute("Factor", &factor) == tinyxml2::XML_SUCCESS)
					info.dstColorBlendFactor = strToVkEnumT(_context.context.registry.formatString(factor), info.dstColorBlendFactor);
			}

			if (auto colorBlendOp = attachment->FirstChildElement("ColorBlend")){
				const char* factor;
				if (colorBlendOp->QueryStringAttribute("Operation", &factor) == tinyxml2::XML_SUCCESS)
					info.colorBlendOp = strToVkEnumT(_context.context.registry.formatString(factor), info.colorBlendOp);
			}

			if (auto srcAlphaBlendFactor = attachment->FirstChildElement("SrcAlphaBlend")){
				const char* factor;
				if (srcAlphaBlendFactor->QueryStringAttribute("Factor", &factor) == tinyxml2::XML_SUCCESS)
					info.srcAlphaBlendFactor = strToVkEnumT(_context.context.registry.formatString(factor), info.srcAlphaBlendFactor);
			}

			if (auto dstAlphaBlendFactor = attachment->FirstChildElement("DstAlphaBlend")){
				const char* factor;
				if (dstAlphaBlendFactor->QueryStringAttribute("Factor", &factor) == tinyxml2::XML_SUCCESS)
					info.dstAlphaBlendFactor = strToVkEnumT(_context.context.registry.formatString(factor), info.dstAlphaBlendFactor);
			}

			if (auto alphaBlendOp = attachment->FirstChildElement("AlphaBlend")){
				const char* factor;
				if (alphaBlendOp->QueryStringAttribute("Operation", &factor) == tinyxml2::XML_SUCCESS)
					info.alphaBlendOp = strToVkEnumT(_context.context.registry.formatString(factor), info.alphaBlendOp);
			}

			if (auto colorWriteMask = element->FirstChildElement("ColorWrite")){
				info.colorWriteMask = 0x0;
				if (colorWriteMask->BoolAttribute("r", false)) info.colorWriteMask |= VK_COLOR_COMPONENT_R_BIT;
				if (colorWriteMask->BoolAttribute("g", false)) info.colorWriteMask |= VK_COLOR_COMPONENT_G_BIT;
				if (colorWriteMask->BoolAttribute("b", false)) info.colorWriteMask |= VK_COLOR_COMPONENT_B_BIT;
				if (colorWriteMask->BoolAttribute("a", false)) info.colorWriteMask |= VK_COLOR_COMPONENT_A_BIT;
			}
		}
	}

	void GraphicsPipelineFactory::getTessellationInfoXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder){
		if (!element) return;
		auto& info = builder.tessellationInfo();

		if (auto patchControlPoint = element->FirstChildElement("PatchControlPoints"))
			info.patchControlPoints = patchControlPoint->IntAttribute("Count", info.patchControlPoints);
	}
}