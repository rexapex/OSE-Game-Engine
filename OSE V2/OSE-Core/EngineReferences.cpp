#pragma once
#include "stdafx.h"
#include "EngineReferences.h"
#include "OSE Rendering GL/Rendering/RenderingFactoryGL.h"
#include "OSE Project XML/Project/ProjectLoaderFactoryXML.h"
#include "OSE Resources SOIL/Resources/Texture/TextureLoaderFactorySOIL.h"
#include "OSE Resources Tilemap TXT/Resources/Tilemap/TilemapLoaderFactoryTXT.h"
#include "OSE Windowing GLFW/Windowing/WindowingFactoryGLFW.h"

namespace ose
{
	// Create array of windowing factories
	extern std::unique_ptr<ose::windowing::WindowingFactory> const WindowingFactories[] { std::make_unique<ose::windowing::WindowingFactoryGLFW>() };

	// Create array of project loader factories
	extern std::unique_ptr<ose::project::ProjectLoaderFactory> const ProjectLoaderFactories[] { std::make_unique<ose::project::ProjectLoaderFactoryXML>() };

	// Create array of texture loader factories
	extern std::unique_ptr<ose::resources::TextureLoaderFactory> const TextureLoaderFactories[] { std::make_unique<ose::resources::TextureLoaderFactorySOIL>() };

	// Create array of tilemap loader factories
	extern std::unique_ptr<ose::resources::TilemapLoaderFactory> const TilemapLoaderFactories[] { std::make_unique<ose::resources::TilemapLoaderFactoryTXT>() };

	// Create array of rendering factories
	extern std::unique_ptr<ose::rendering::RenderingFactory> const RenderingFactories[] { std::make_unique<ose::rendering::RenderingFactoryGL>() };
}