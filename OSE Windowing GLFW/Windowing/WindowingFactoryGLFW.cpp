#include "pch.h"
#include "WindowingFactoryGLFW.h"
#include "WindowManagerGLFW.h"

namespace ose::windowing
{
	uptr<WindowManager> WindowingFactoryGLFW::NewWindowManager()
	{
		return ose::make_unique<WindowManagerGLFW>();
	}
}