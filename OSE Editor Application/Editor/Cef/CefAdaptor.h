#pragma once

class SimpleApp;

namespace ose
{
	class CefAdaptor
	{
	public:
		CefAdaptor(SimpleApp * cef_app);

		void Update();

		void SetFramebufferSize(int width, int height);

	private:
		SimpleApp * cef_app_;
	};
}
