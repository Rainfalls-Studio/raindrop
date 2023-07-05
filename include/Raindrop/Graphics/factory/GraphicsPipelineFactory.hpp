#ifndef __RAINDROP_GRAPHICS_FACTORY_GRAPHICS_PIPELINE_FACTORY_HPP__
#define __RAINDROP_GRAPHICS_FACTORY_GRAPHICS_PIPELINE_FACTORY_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics::Factory{
	class GraphicsPipelineFactory : public Raindrop::Core::Asset::AssetFactory{
		public:
			GraphicsPipelineFactory(GraphicsContext& context);
			virtual ~GraphicsPipelineFactory() override;

			virtual std::shared_ptr<Core::Asset::Asset> createAsset(const std::filesystem::path& path) override;
			virtual void destroyAsset(std::shared_ptr<Core::Asset::Asset> asset) override;

			void registerExtensions(const std::shared_ptr<GraphicsPipelineFactory>& factory);
		
		private:
			class ExtensionToFormat;
			GraphicsContext& _context;
			std::unordered_map<std::string, std::shared_ptr<GraphicsPipeline>> _pipelines;

			enum Format{
				NONE = 0,
				XML,
			};

			Format getFormat(const std::filesystem::path& path);
			Format getExtentionFormat(const std::wstring extension);

			std::shared_ptr<GraphicsPipeline> loadFromXML(const std::filesystem::path& path);

			void getShadersXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder);
			std::shared_ptr<Shader> getShaderXML(tinyxml2::XMLElement* shaderElement);
			void getTargetXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder);
			void getNameXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder);
			void getRasterisationInfoXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder);
			void getMultisampleInfoXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder);
			void getAssemnlyInfoXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder);
			void getDepthStencilInfoXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder);
			void getColorBlendInfoXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder);
			void getTessellationInfoXML(tinyxml2::XMLElement* element, Builders::GraphicsPipelineBuilder& builder);
			void getDepthStencilOpStateXML(tinyxml2::XMLElement* element, VkStencilOpState& op);
	};
}

#endif