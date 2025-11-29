#include "Shader/GLSLShader.hpp"
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>

namespace Raindrop::Render{
    GLSLShader::GLSLShader(
        std::shared_ptr<Filesystem::FilesystemModule> filesystem,
        std::shared_ptr<RenderCoreModule> core,
        const Filesystem::Path& path,
        const std::string& entryPoint
    ) : Shader(
        filesystem,
        core,
        path,
        entryPoint
    ) {}

    // https://github.com/KhronosGroup/glslang/issues/2207
    static TBuiltInResource InitResources(){
        TBuiltInResource Resources;

        Resources.maxLights                                 = 32;
        Resources.maxClipPlanes                             = 6;
        Resources.maxTextureUnits                           = 32;
        Resources.maxTextureCoords                          = 32;
        Resources.maxVertexAttribs                          = 64;
        Resources.maxVertexUniformComponents                = 4096;
        Resources.maxVaryingFloats                          = 64;
        Resources.maxVertexTextureImageUnits                = 32;
        Resources.maxCombinedTextureImageUnits              = 80;
        Resources.maxTextureImageUnits                      = 32;
        Resources.maxFragmentUniformComponents              = 4096;
        Resources.maxDrawBuffers                            = 32;
        Resources.maxVertexUniformVectors                   = 128;
        Resources.maxVaryingVectors                         = 8;
        Resources.maxFragmentUniformVectors                 = 16;
        Resources.maxVertexOutputVectors                    = 16;
        Resources.maxFragmentInputVectors                   = 15;
        Resources.minProgramTexelOffset                     = -8;
        Resources.maxProgramTexelOffset                     = 7;
        Resources.maxClipDistances                          = 8;
        Resources.maxComputeWorkGroupCountX                 = 65535;
        Resources.maxComputeWorkGroupCountY                 = 65535;
        Resources.maxComputeWorkGroupCountZ                 = 65535;
        Resources.maxComputeWorkGroupSizeX                  = 1024;
        Resources.maxComputeWorkGroupSizeY                  = 1024;
        Resources.maxComputeWorkGroupSizeZ                  = 64;
        Resources.maxComputeUniformComponents               = 1024;
        Resources.maxComputeTextureImageUnits               = 16;
        Resources.maxComputeImageUniforms                   = 8;
        Resources.maxComputeAtomicCounters                  = 8;
        Resources.maxComputeAtomicCounterBuffers            = 1;
        Resources.maxVaryingComponents                      = 60;
        Resources.maxVertexOutputComponents                 = 64;
        Resources.maxGeometryInputComponents                = 64;
        Resources.maxGeometryOutputComponents               = 128;
        Resources.maxFragmentInputComponents                = 128;
        Resources.maxImageUnits                             = 8;
        Resources.maxCombinedImageUnitsAndFragmentOutputs   = 8;
        Resources.maxCombinedShaderOutputResources          = 8;
        Resources.maxImageSamples                           = 0;
        Resources.maxVertexImageUniforms                    = 0;
        Resources.maxTessControlImageUniforms               = 0;
        Resources.maxTessEvaluationImageUniforms            = 0;
        Resources.maxGeometryImageUniforms                  = 0;
        Resources.maxFragmentImageUniforms                  = 8;
        Resources.maxCombinedImageUniforms                  = 8;
        Resources.maxGeometryTextureImageUnits              = 16;
        Resources.maxGeometryOutputVertices                 = 256;
        Resources.maxGeometryTotalOutputComponents          = 1024;
        Resources.maxGeometryUniformComponents              = 1024;
        Resources.maxGeometryVaryingComponents              = 64;
        Resources.maxTessControlInputComponents             = 128;
        Resources.maxTessControlOutputComponents            = 128;
        Resources.maxTessControlTextureImageUnits           = 16;
        Resources.maxTessControlUniformComponents           = 1024;
        Resources.maxTessControlTotalOutputComponents       = 4096;
        Resources.maxTessEvaluationInputComponents          = 128;
        Resources.maxTessEvaluationOutputComponents         = 128;
        Resources.maxTessEvaluationTextureImageUnits        = 16;
        Resources.maxTessEvaluationUniformComponents        = 1024;
        Resources.maxTessPatchComponents                    = 120;
        Resources.maxPatchVertices                          = 32;
        Resources.maxTessGenLevel                           = 64;
        Resources.maxViewports                              = 16;
        Resources.maxVertexAtomicCounters                   = 0;
        Resources.maxTessControlAtomicCounters              = 0;
        Resources.maxTessEvaluationAtomicCounters           = 0;
        Resources.maxGeometryAtomicCounters                 = 0;
        Resources.maxFragmentAtomicCounters                 = 8;
        Resources.maxCombinedAtomicCounters                 = 8;
        Resources.maxAtomicCounterBindings                  = 1;
        Resources.maxVertexAtomicCounterBuffers             = 0;
        Resources.maxTessControlAtomicCounterBuffers        = 0;
        Resources.maxTessEvaluationAtomicCounterBuffers     = 0;
        Resources.maxGeometryAtomicCounterBuffers           = 0;
        Resources.maxFragmentAtomicCounterBuffers           = 1;
        Resources.maxCombinedAtomicCounterBuffers           = 1;
        Resources.maxAtomicCounterBufferSize                = 16384;
        Resources.maxTransformFeedbackBuffers               = 4;
        Resources.maxTransformFeedbackInterleavedComponents = 64;
        Resources.maxCullDistances                          = 8;
        Resources.maxCombinedClipAndCullDistances           = 8;
        Resources.maxSamples                                = 4;
        Resources.maxMeshOutputVerticesNV                   = 256;
        Resources.maxMeshOutputPrimitivesNV                 = 512;
        Resources.maxMeshWorkGroupSizeX_NV                  = 32;
        Resources.maxMeshWorkGroupSizeY_NV                  = 1;
        Resources.maxMeshWorkGroupSizeZ_NV                  = 1;
        Resources.maxTaskWorkGroupSizeX_NV                  = 32;
        Resources.maxTaskWorkGroupSizeY_NV                  = 1;
        Resources.maxTaskWorkGroupSizeZ_NV                  = 1;
        Resources.maxMeshViewCountNV                        = 4;

        Resources.limits.nonInductiveForLoops                 = 1;
        Resources.limits.whileLoops                           = 1;
        Resources.limits.doWhileLoops                         = 1;
        Resources.limits.generalUniformIndexing               = 1;
        Resources.limits.generalAttributeMatrixVectorIndexing = 1;
        Resources.limits.generalVaryingIndexing               = 1;
        Resources.limits.generalSamplerIndexing               = 1;
        Resources.limits.generalVariableIndexing              = 1;
        Resources.limits.generalConstantMatrixVectorIndexing  = 1;

        return Resources;
    }

    vk::ShaderStageFlagBits detectShaderStage(const Filesystem::Path& path) {
        const auto filename = path.filename();

        if (filename.contains(".vert"))  return vk::ShaderStageFlagBits::eVertex;
        if (filename.contains(".frag"))  return vk::ShaderStageFlagBits::eFragment;
        if (filename.contains(".comp"))  return vk::ShaderStageFlagBits::eCompute;
        if (filename.contains(".geom"))  return vk::ShaderStageFlagBits::eGeometry;
        if (filename.contains(".tesc"))  return vk::ShaderStageFlagBits::eTessellationControl;
        if (filename.contains(".tese"))  return vk::ShaderStageFlagBits::eTessellationEvaluation;

        throw std::runtime_error("Unknown shader stage: " + filename);
    }

    std::expected<std::vector<uint32_t>, Error> GLSLShader::getCodeImpl(){

        std::string source;
        {
            auto file = _filesystem->open(_path, Filesystem::OpenFlags::READ);
            if (!file){
                // TODO : change to file read error
                return std::unexpected(Error(FailedShaderModuleCreationError(), "Failed to load file : {}", _path));
            }

            file->seek(0, Filesystem::SeekOrigin::END);
            uint32_t size = static_cast<uint32_t>(file->tell());
            file->seek(0, Filesystem::SeekOrigin::BEGIN);

            source.resize(size);
            file->read(source.data(), size);

            file->close();
        }

        _stage = detectShaderStage(_path);

        EShLanguage shaderStage;
        switch (_stage) {
            case vk::ShaderStageFlagBits::eVertex:   shaderStage = EShLangVertex; break;
            case vk::ShaderStageFlagBits::eFragment: shaderStage = EShLangFragment; break;
            case vk::ShaderStageFlagBits::eCompute:  shaderStage = EShLangCompute; break;
            case vk::ShaderStageFlagBits::eGeometry: shaderStage = EShLangGeometry; break;
            case vk::ShaderStageFlagBits::eTessellationControl:    shaderStage = EShLangTessControl; break;
            case vk::ShaderStageFlagBits::eTessellationEvaluation: shaderStage = EShLangTessEvaluation; break;
            default: throw std::runtime_error("Unsupported shader stage");
        }

        glslang::TShader shader(shaderStage);
        const char* sourceCStr = source.c_str();
        shader.setStrings(&sourceCStr, 1);
        shader.setEntryPoint(_entryPoint.c_str());
        shader.setEnvInput(glslang::EShSourceGlsl, shaderStage, glslang::EShClientVulkan, 100);
        shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_3);
        shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_6);

        TBuiltInResource resources = InitResources();
        EShMessages messages = static_cast<EShMessages>(EShMsgDefault | EShMsgVulkanRules | EShMsgSpvRules);

        if (!shader.parse(&resources, 100, false, messages)) {
            throw std::runtime_error("GLSL parsing failed:\n" + std::string(shader.getInfoLog()));
        }

        glslang::TProgram program;
        program.addShader(&shader);
        if (!program.link(messages)) {
            throw std::runtime_error("Shader linking failed:\n" + std::string(program.getInfoLog()));
        }

        std::vector<uint32_t> spirv;
        glslang::GlslangToSpv(*program.getIntermediate(shaderStage), spirv);

        return spirv;
    }
}