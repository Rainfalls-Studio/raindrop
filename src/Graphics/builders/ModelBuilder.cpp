#include <Raindrop/Graphics/builders/ModelBuilder.hpp>
#include <Raindrop/Graphics/Model.hpp>

namespace Raindrop::Graphics::Builders{
	ModelBuilder::ModelBuilder(const std::filesystem::path& path){
		
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.Model");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		Assimp::Importer importer;

		unsigned int postProcessSteps = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords;
		const aiScene* scene = importer.ReadFile(path.string(), postProcessSteps);

		// Check if the model was loaded successfully
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			CLOG(ERROR, "Engine.Graphics.Model") << "Failed to load model : " << importer.GetErrorString();
			throw std::runtime_error("Failed to load model");
		}

		aiNode* rootNode = scene->mRootNode;
		aiMesh* mesh = scene->mMeshes[0];

		getVertices(mesh);
		getIndices(mesh);

		importer.FreeScene();
	}

	std::shared_ptr<Model> ModelBuilder::build(GraphicsContext& context){
		return std::make_shared<Model>(context, *this);
	}
	
	void ModelBuilder::getVertices(aiMesh* mesh){
		aiVector3D* vertices = mesh->mVertices;
    	aiVector3D* normals = mesh->mNormals;
    	aiColor4D* colors = mesh->mColors[0];
    	aiVector3D* uvs = mesh->mTextureCoords[0];

		unsigned int numVertices = mesh->mNumVertices;

		_vertices.resize(numVertices);

		for (unsigned int i = 0; i < numVertices; i++){
			auto& vertex = _vertices[i];
			auto& pos = vertices[i];
			auto& color = colors[i];
			auto& normal = normals[i];

			vertex.position = glm::vec3(pos.x, pos.y, pos.z);
			vertex.color = glm::vec4(color.r, color.g, color.b, color.a);
			vertex.normal = glm::vec3(normal.x, normal.y, normal.z);
		}

		if (mesh->HasTextureCoords(0)){
			for (unsigned int i = 0; i < numVertices; i++){
				auto& vertex = _vertices[i];
				auto& uv = uvs[i];

				vertex.uv = glm::vec2(uv.x, uv.y);
			}
		}
	}

	void ModelBuilder::getIndices(aiMesh* mesh){
		aiFace* faces = mesh->mFaces;
		unsigned int numFaces = mesh->mNumFaces;

		_indices.resize(numFaces);

		for (unsigned int i = 0; i < numFaces; i++) {
			aiFace& face = faces[i];
			unsigned int numIndices = face.mNumIndices;

			for (unsigned int j = 0; j < numIndices; j++) {
				_indices[i * j] = face.mIndices[j];
			}
		}
	}

	ModelBuilder::ModelBuilder(){

	}

	ModelBuilder::~ModelBuilder(){

	}

	void ModelBuilder::setRenderer(Renderer* renderer){
		_renderer = renderer;
	}
	
	std::vector<Vertex>& ModelBuilder::vertices(){
		return _vertices;
	}

	std::vector<uint32_t>& ModelBuilder::indices(){
		return _indices;
	}

	Renderer* ModelBuilder::renderer(){
		return _renderer;
	}
}