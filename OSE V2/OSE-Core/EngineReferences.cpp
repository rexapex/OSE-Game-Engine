#pragma once
#include "stdafx.h"
#include "EngineReferences.h"
#include "OSE Rendering GL/Rendering/RenderingFactoryGL.h"
#include "OSE Rendering VK/Rendering/RenderingFactoryVK.h"
#include "OSE Project XML/Project/ProjectLoaderFactoryXML.h"
#include "OSE Resources SOIL/Resources/Texture/TextureLoaderFactorySOIL.h"
#include "OSE Resources Tilemap TXT/Resources/Tilemap/TilemapLoaderFactoryTXT.h"
#include "OSE Resources Mesh ASSIMP/Resources/Mesh/MeshLoaderFactoryASSIMP.h"
#include "OSE Windowing GLFW/Windowing/WindowingFactoryGLFW.h"
#include "OSE Scripting CPP/Scripting/ScriptingFactoryCPP.h"

namespace ose
{
	// Create array of windowing factories
	extern uptr<WindowingFactory> const WindowingFactories[] { ose::make_unique<ose::windowing::WindowingFactoryGLFW>() };

	// Create array of project loader factories
	extern uptr<ProjectLoaderFactory> const ProjectLoaderFactories[] { ose::make_unique<ose::project::ProjectLoaderFactoryXML>() };

	// Create array of texture loader factories
	extern uptr<TextureLoaderFactory> const TextureLoaderFactories[] { ose::make_unique<ose::resources::TextureLoaderFactorySOIL>() };

	// Create array of tilemap loader factories
	extern uptr<TilemapLoaderFactory> const TilemapLoaderFactories[] { ose::make_unique<ose::resources::TilemapLoaderFactoryTXT>() };

	// Create array of mesh loader factories
	extern uptr<MeshLoaderFactory> const MeshLoaderFactories[] { ose::make_unique<ose::resources::MeshLoaderFactoryASSIMP>() };

	// Create array of rendering factories
	extern uptr<RenderingFactory> const RenderingFactories[] { ose::make_unique<ose::rendering::RenderingFactoryVK>()/*ose::make_unique<ose::rendering::RenderingFactoryGL>()*/ };

	// Create array of scripting factories
	extern uptr<ScriptingFactory> const ScriptingFactories[] { ose::make_unique<ose::scripting::ScriptingFactoryCPP>() };
}