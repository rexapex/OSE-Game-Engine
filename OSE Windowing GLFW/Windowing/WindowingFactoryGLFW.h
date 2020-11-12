#pragma once
#include "OSE-Core/Windowing/WindowingFactory.h"

namespace ose
{
	class WindowManager;
}

namespace ose::windowing
{
	class WindowingFactoryGLFW : public WindowingFactory
	{
	public:
		constexpr WindowingFactoryGLFW() {}
		virtual ~WindowingFactoryGLFW() {}
		WindowingFactoryGLFW(WindowingFactoryGLFW &) = delete;
		WindowingFactoryGLFW & operator=(WindowingFactoryGLFW &) = delete;
		WindowingFactoryGLFW(WindowingFactoryGLFW &&) = default;
		WindowingFactoryGLFW & operator=(WindowingFactoryGLFW &&) = default;

		virtual uptr<WindowManager> NewWindowManager();
	};
}
