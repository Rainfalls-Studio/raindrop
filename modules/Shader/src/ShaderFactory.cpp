#include "Raindrop/Modules/Render/Shaders/ShaderFactory.hpp"
#include "Raindrop/Modules/Render/Shaders/GLSLShader.hpp"
#include "Raindrop/Engine.hpp"

namespace Raindrop::Render{
    ShaderFactory::ShaderFactory(Engine& engine){
        auto& modules = engine.getModuleManager();

        _renderCore = modules.getModuleAs<RenderCoreModule>("RenderCore");
        _filesystem = modules.getModuleAs<Filesystem::FilesystemModule>("Filesystem");
    }

    ShaderFactory::~ShaderFactory(){
        _shaders.clear();
    }

    std::shared_ptr<Asset::Asset> ShaderFactory::getOrCreate(const Filesystem::Path& path){
        const auto& ext = path.extension();

        if (auto it=_shaders.find(path); it != _shaders.end()){
            auto shader = it->second.lock();
            
            if (shader){
                return shader;
            }
        }

        if (ext == ".glsl" || ext == ".vert" || ext == ".frag" || ext == ".geom" || ext == ".comp" || ext == ".tesc" || ext == ".tese"){
            auto shader =  std::make_shared<GLSLShader>(_filesystem, _renderCore, path);
            _shaders[path] = shader;
            return shader;
        }

        throw std::runtime_error("Unknown extension !");
    }

	std::string ShaderFactory::getName() const noexcept{
        return "Shader factory";
    }
}