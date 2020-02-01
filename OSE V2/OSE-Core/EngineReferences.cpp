#pragma once
#include "stdafx.h"
#include "EngineReferences.h"
#include "OSE Rendering GL/Rendering/RenderingFactoryGL.h"
#include "OSE Project XML/Project/ProjectLoaderFactoryXML.h"
#include "OSE Resources SOIL/Resources/Texture/TextureLoaderFactorySOIL.h"
#include "OSE Resources Tilemap TXT/Resources/Tilemap/TilemapLoaderFactoryTXT.h"
#include "OSE Resources Mesh ASSIMP/Resources/Mesh/MeshLoaderFactoryASSIMP.h"
#include "OSE Windowing GLFW/Windowing/WindowingFactoryGLFW.h"
#include "OSE Scripting CPP/Scripting/ScriptingFactoryCPP.h"

namespace ose
{
	// Create array of windowing factories
	extern std::unique_ptr<WindowingFactory> const WindowingFactories[] { std::make_unique<ose::windowing::WindowingFactoryGLFW>() };

	// Create array of project loader factories
	extern std::unique_ptr<ProjectLoaderFactory> const ProjectLoaderFactories[] { std::make_unique<ose::project::ProjectLoaderFactoryXML>() };

	// Create array of texture loader factories
	extern std::unique_ptr<TextureLoaderFactory> const TextureLoaderFactories[] { std::make_unique<ose::resources::TextureLoaderFactorySOIL>() };

	// Create array of tilemap loader factories
	extern std::unique_ptr<TilemapLoaderFactory> const TilemapLoaderFactories[] { std::make_unique<ose::resources::TilemapLoaderFactoryTXT>() };

	// Create array of mesh loader factories
	extern std::unique_ptr<MeshLoaderFactory> const MeshLoaderFactories[] { std::make_unique<ose::resources::MeshLoaderFactoryASSIMP>() };

	// Create array of rendering factories
	extern std::unique_ptr<RenderingFactory> const RenderingFactories[] { std::make_unique<ose::rendering::RenderingFactoryGL>() };

	// Create array of scripting factories
	extern std::unique_ptr<ScriptingFactory> const ScriptingFactories[] { std::make_unique<ose::scripting::ScriptingFactoryCPP>() };
}