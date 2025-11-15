#pragma once

#include "Engine.hpp"
#include "Core/Modules/InitHelper.hpp"

// builtin Modules

#include "Modules/Scene/SceneModule.hpp"
#include "Modules/Scene/Entity.hpp"
#include "Modules/Scene/PhaseExecutionStage.hpp"

#include "Modules/Scene/Behaviors/HierarchyAttacherBehavior.hpp"
#include "Modules/Scene/Behaviors/TagAttacherBehavior.hpp"
#include "Modules/Scene/Behaviors/TransformAttacherBehavior.hpp"
#include "Modules/Scene/Behaviors/HierarchyTransformPropagator.hpp"
#include "Modules/Scene/Behaviors/FrameSnapshotServiceBehavior.hpp"

#include "Modules/Scene/Components/Hierarchy.hpp"
#include "Modules/Scene/Components/Tag.hpp"
#include "Modules/Scene/Components/Transform.hpp"
#include "Modules/Scene/Components/Camera.hpp"

#include "Modules/Event/EventModule.hpp"

#include "Modules/Window/WindowModule.hpp"
#include "Modules/Window/WindowEvents.hpp"
#include "Modules/Window/WindowEventStage.hpp"

#include "Modules/Render/Core/RenderCoreModule.hpp"
#include "Modules/Render/Core/RenderCommandContext.hpp"

#include "Modules/Render/Shaders/ShaderFactory.hpp"
#include "Modules/Render/Shaders/Shader.hpp"

#include "Modules/Render/RenderGraph/RenderGraphModule.hpp"
#include "Modules/Render/RenderGraph/RenderGraphRenderStage.hpp"
#include "Modules/Render/RenderGraph/IRenderStep.hpp"
#include "Modules/Render/RenderGraph/RenderSequencePass.hpp"

#include "Modules/Render/RenderOutput/RenderOutputModule.hpp"
#include "Modules/Render/RenderOutput/WindowRenderOutput.hpp"
#include "Modules/Render/RenderOutput/BufferRenderOutput.hpp"
#include "Modules/Render/RenderOutput/RenderOutputBeginRenderPass.hpp"
#include "Modules/Render/RenderOutput/RenderOutputEndRenderPass.hpp"

#include "Modules/ImGui/ImGuiModule.hpp"
#include "Modules/ImGui/ImGuiContext.hpp"

#include "Modules/Filesystem/FilesystemModule.hpp"
#include "Modules/Filesystem/Providers/FolderProvider.hpp"

#include "Modules/Asset/AssetModule.hpp"
#include "Modules/Asset/Factory.hpp"

#include "Modules/Layers/LayerModule.hpp"