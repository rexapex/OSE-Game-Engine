#pragma once

namespace ose::windowing
{
	class WindowManager;

	class WindowingFactory
	{
	public:
		constexpr WindowingFactory() {}
		virtual ~WindowingFactory() {}
		WindowingFactory(WindowingFactory &) = delete;
		WindowingFactory & operator=(WindowingFactory &) = delete;
		WindowingFactory(WindowingFactory &&) = default;
		WindowingFactory & operator=(WindowingFactory &&) = default;

		virtual std::unique_ptr<WindowManager> NewWindowManager() = 0;
	};
}
