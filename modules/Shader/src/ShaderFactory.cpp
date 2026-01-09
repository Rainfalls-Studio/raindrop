#include "Shader/ShaderFactory.hpp"
#include "Shader/GLSLShader.hpp"

#include <Raindrop/Engine.hpp>
#include <unordered_set>

namespace Shader{
    ShaderFactory::ShaderFactory(Raindrop::Engine& engine){
        auto& modules = engine.getModuleManager();

        _renderCore = modules.getModuleAs<Render::RenderCoreModule>("RenderCore");
        _filesystem = modules.getModuleAs<Filesystem::FilesystemModule>("Filesystem");
    }

    ShaderFactory::~ShaderFactory(){
        _shaders.clear();
    }

    std::shared_ptr<Asset::Asset> ShaderFactory::getOrCreate(const Filesystem::Path& path){
    
        const std::unordered_set<std::string> validExtensions = {
            ".glsl", ".vert", ".frag", ".geom", ".comp", ".tect", ".tese"
        };

        const auto& ext = path.extension();

        if (auto it=_shaders.find(path); it != _shaders.end()){
            auto shader = it->second.lock();
            
            if (shader){
                return shader;
            }
        }

        if (validExtensions.count(ext)){
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