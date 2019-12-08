#include "pch.h"
#include "WindowingFactoryGLFW.h"
#include "WindowManagerGLFW.h"

namespace ose::windowing
{
	std::unique_ptr<WindowManager> WindowingFactoryGLFW::NewWindowManager()
	{
		return std::make_unique<WindowManagerGLFW>();
	}
}