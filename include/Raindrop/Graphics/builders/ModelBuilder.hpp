#ifndef __RAINDROP_GRAPHICS_BUILDERS_MODEL_BUILDER_HPP__
#define __RAINDROP_GRAPHICS_BUILDERS_MODEL_BUILDER_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics::Builders{
	class ModelBuilder{
		public:
			ModelBuilder(const std::filesystem::path& path);
			ModelBuilder();

			~ModelBuilder();

			std::vector<Vertex>& vertices();
			std::vector<uint32_t>& indices();
			
			std::shared_ptr<Model> build(GraphicsContext& context);

		private:
		
			std::vector<Vertex> _vertices;
			std::vector<uint32_t> _indices;
	};
}

#endif