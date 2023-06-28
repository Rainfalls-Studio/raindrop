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
			_pipelines.push_back(pipeline);
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

		Builders::PipelineBuilder builder;

		// builder.setDevice();

		{
			std::list<std::shared_ptr<Shader>> shaders = getShadersXML(document.FirstChildElement("shaders"));
			for (auto &s : shaders){
				builder.addShader(s);
			}
		}

		VkRenderPass renderPass = registry["Engine.Graphics.Swapchain.RenderPass"].as<VkRenderPass>();
		builder.setRenderPass(renderPass);

		return builder.build(_context);
	}

	std::list<std::shared_ptr<Shader>> GraphicsPipelineFactory::getShadersXML(tinyxml2::XMLElement* element){
		std::list<std::shared_ptr<Shader>> shaders;
		for (tinyxml2::XMLElement* child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()){
			if (strcmp(child->Name(), "shader") == 0){
				shaders.push_back(getShaderXML(child));
			}
		}
		return shaders;
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
}