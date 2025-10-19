#pragma once

#include "Engine.hpp"

#include "Core/Modules/InitHelper.hpp"
#include "Core/Layers/Layer.hpp"

// builtin Modules

#include "Modules/Scene/SceneModule.hpp"
#include "Modules/Scene/SceneTrait.hpp"
#include "Modules/Scene/Entity.hpp"
#include "Modules/Scene/Behaviors/HierarchyAttacherBehavior.hpp"
#include "Modules/Scene/Behaviors/TagAttacherBehavior.hpp"
#include "Modules/Scene/Behaviors/TransformAttacherBehavior.hpp"
#include "Modules/Scene/Behaviors/HierarchyTransformPropagator.hpp"
#include "Modules/Scene/Components/Hierarchy.hpp"
#include "Modules/Scene/Components/Tag.hpp"
#include "Modules/Scene/Components/Transform.hpp"

#include "Modules/Event/EventModule.hpp"

#include "Modules/Window/WindowModule.hpp"
#include "Modules/Window/WindowEvents.hpp"
#include "Modules/Window/WindowEventStage.hpp"

#include "Modules/Render/Core/RenderCoreModule.hpp"

#include "Modules/Render/RenderGraph/RenderGraphModule.hpp"
#include "Modules/Render/RenderGraph/RenderGraphRecordStage.hpp"
#include "Modules/Render/RenderGraph/RenderGraphRenderStage.hpp"

#include "Modules/Render/RenderOutput/RenderOutputModule.hpp"
#include "Modules/Render/RenderOutput/PresentRenderOutputStage.hpp"
#include "Modules/Render/RenderOutput/WindowRenderOutput.hpp"
#include "Modules/Render/RenderOutput/BlitToRenderOutputPass.hpp"

#include "Modules/Render/ImGui/ImGuiModule.hpp"
#include "Modules/Render/ImGui/ImGuiStage.hpp"

#include "Modules/Filesystem/FilesystemModule.hpp"
#include "Modules/Filesystem/Providers/FolderProvider.hpp"

#include "Modules/Asset/AssetModule.hpp"
#include "Modules/Asset/Factory.hpp"