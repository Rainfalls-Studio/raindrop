#include <Raindrop/Graphics/factory/ShaderFactory.hpp>
#include <Raindrop/Graphics/Shader.hpp>

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
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.Shader");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");
	}

	ShaderFactory::~ShaderFactory(){
		_shaders.clear();
	}

	std::shared_ptr<Core::Asset::Asset> ShaderFactory::createAsset(const std::filesystem::path& path){
		auto extension = path.extension();
		CLOG(INFO, "Engine.Graphics.Shader") << "Loading a new shader : " << path;

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
		CLOG(INFO, "Engine.Graphics.Shader") << "Destroying shader";
		_shaders.remove(shader);
	}

	std::shared_ptr<Shader> ShaderFactory::loadSPV(const std::filesystem::path& path){
		std::vector<char> code = readFile(path);
		VkShaderStageFlagBits stage = getStage(path);
		CLOG(INFO, "Engine.Graphics.Shader") << "Loading " << string_VkShaderStageFlagBits(stage) << " shader from " << path;
		return std::make_unique<Shader>(_device, code, stage, _allocationCallbacks);
	}

	VkShaderStageFlagBits ShaderFactory::getStage(const std::filesystem::path& path){
		std::wregex extensionRegex(L"\\.([a-zA-Z0-9]+)");
		std::wstring strPath = path.wstring();

		std::wsmatch extensionsMatch;
		auto searchStart = strPath.cbegin();

		while (std::regex_search(searchStart, strPath.cend(), extensionsMatch, extensionRegex)) {
			std::wstring extension = extensionsMatch[1].str();
			VkShaderStageFlagBits flagBits = getExtentionStage(extension);
			if (flagBits != 0) return flagBits;
			searchStart = extensionsMatch.suffix().first;
		}
	}

	class ExtensionToFlag{
		public:
			ExtensionToFlag(){
				hashMap[L"vert"] = VK_SHADER_STAGE_VERTEX_BIT;
				hashMap[L"vertex"] = VK_SHADER_STAGE_VERTEX_BIT;
				hashMap[L"vs"] = VK_SHADER_STAGE_VERTEX_BIT;
				hashMap[L"vertexshader"] = VK_SHADER_STAGE_VERTEX_BIT;
				hashMap[L"vp"] = VK_SHADER_STAGE_VERTEX_BIT;
				
				hashMap[L"frag"] = VK_SHADER_STAGE_FRAGMENT_BIT;
				hashMap[L"fragment"] = VK_SHADER_STAGE_FRAGMENT_BIT;
				hashMap[L"fs"] = VK_SHADER_STAGE_FRAGMENT_BIT;
				hashMap[L"fragshader"] = VK_SHADER_STAGE_FRAGMENT_BIT;
				hashMap[L"fp"] = VK_SHADER_STAGE_FRAGMENT_BIT;

				hashMap[L"geom"] = VK_SHADER_STAGE_GEOMETRY_BIT;
				hashMap[L"geometry"] = VK_SHADER_STAGE_GEOMETRY_BIT;
				hashMap[L"gs"] = VK_SHADER_STAGE_GEOMETRY_BIT;
				hashMap[L"geometryshader"] = VK_SHADER_STAGE_GEOMETRY_BIT;
				hashMap[L"gp"] = VK_SHADER_STAGE_GEOMETRY_BIT;
			}

			std::unordered_map<std::wstring, VkShaderStageFlagBits, CaseInsensitiveHash, CaseInsensitiveEqual> hashMap;
	};

	VkShaderStageFlagBits ShaderFactory::getExtentionStage(const std::wstring extension){
		static ExtensionToFlag map;
		return map.hashMap[extension];
	}
}