#include <Raindrop/Graphics/factory/ShaderFactory.hpp>
#include <Raindrop/Graphics/Shader.hpp>

namespace Raindrop::Graphics::Factory{

	static std::vector<char> readFile(const std::filesystem::path &filepath){
		std::ifstream file(filepath, std::ios::ate | std::ios::binary);

		if (!file.is_open()){
			std::stringstream err;
			err << "Failed to open : " << filepath << " shader file";
			throw std::runtime_error(err.str());
		}
		
		size_t fileSize = file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	ShaderFactory::ShaderFactory(std::shared_ptr<Device> device, VkAllocationCallbacks* allocationCallbacks) : _device{device}, _allocationCallbacks{allocationCallbacks}{

	}

	ShaderFactory::~ShaderFactory(){
		_shaders.clear();
	}

	std::shared_ptr<Core::Asset::Asset> ShaderFactory::createAsset(const std::filesystem::path& path){
		auto extension = path.extension();

		std::shared_ptr<Shader> shader;
		if (Core::iequals(extension, ".spv")) shader = loadSPV(path);

		if (shader){
			_shaders.push_back(shader);
			return shader;
		}

		std::stringstream err;
		err << "Failed to reconize " << extension << " extesion (" << path << ")";
		throw std::runtime_error(err.str());
	}

	void ShaderFactory::destroyAsset(std::shared_ptr<Core::Asset::Asset> asset){
		std::shared_ptr<Shader> shader = std::static_pointer_cast<Shader>(asset);
		_shaders.remove(shader);
	}

	std::shared_ptr<Shader> ShaderFactory::loadSPV(const std::filesystem::path& path){
		std::vector<char> code = readFile(path);
		return std::make_unique<Shader>(_device, code, _allocationCallbacks);
	}
}