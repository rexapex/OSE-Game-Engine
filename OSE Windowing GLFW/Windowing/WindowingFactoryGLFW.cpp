#include "pch.h"
#include "WindowingFactoryGLFW.h"
#include "WindowManagerGLFW_VK.h"
//#include "WindowManagerGLFW_GL.h"

namespace ose::windowing
{
	uptr<WindowManager> WindowingFactoryGLFW::NewWindowManager()
	{
		//return nullptr;
		return ose::make_unique<WindowManagerGLFW_VK>();
	}
}