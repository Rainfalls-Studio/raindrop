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

#include "Modules/Render/RenderCoreModule.hpp"
#include "Modules/Render/RenderOutputModule.hpp"
#include "Modules/Render/RenderGraphModule.hpp"
#include "Modules/Render/RenderSchedulerModule.hpp"
#include "Modules/Render/RenderOutputs/WindowRenderOutput.hpp"
#include "Modules/Render/Passes/BlitToRenderOutput.hpp"

#include "Modules/Filesystem/FilesystemModule.hpp"
#include "Modules/Filesystem/Providers/FolderProvider.hpp"