#ifndef __RAINDROP_GRAPHICS_BUILDERS_MODEL_BUILDER_HPP__
#define __RAINDROP_GRAPHICS_BUILDERS_MODEL_BUILDER_HPP__

#include <Raindrop/Graphics/common.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Raindrop::Graphics::Builders{
	class ModelBuilder{
		public:
			ModelBuilder(const std::filesystem::path& path);
			ModelBuilder();

			~ModelBuilder();

			void setRenderer(Renderer* renderer);
			
			std::vector<Vertex>& vertices();
			std::vector<uint32_t>& indices();
			Renderer* renderer();

			std::shared_ptr<Model> build(GraphicsContext& context);

		private:
			void getVertices(aiMesh* mesh);
			void getColors(aiMesh* mesh);
			void getIndices(aiMesh* mesh);

			Renderer* _renderer;

			std::vector<Vertex> _vertices;
			std::vector<uint32_t> _indices;
	};
}

#endif