#pragma once
#include "OSE-Core/Windowing/WindowingFactory.h"

namespace ose::windowing
{
	class WindowManager;

	class WindowingFactoryGLFW : public WindowingFactory
	{
	public:
		constexpr WindowingFactoryGLFW() {}
		virtual ~WindowingFactoryGLFW() {}
		WindowingFactoryGLFW(WindowingFactoryGLFW &) = delete;
		WindowingFactoryGLFW & operator=(WindowingFactoryGLFW &) = delete;
		WindowingFactoryGLFW(WindowingFactoryGLFW &&) = default;
		WindowingFactoryGLFW & operator=(WindowingFactoryGLFW &&) = default;

		virtual std::unique_ptr<WindowManager> NewWindowManager();
	};
}
