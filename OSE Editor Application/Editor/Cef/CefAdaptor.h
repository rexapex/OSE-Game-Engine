#pragma once

class SimpleApp;

namespace ose
{
	class CefAdaptor
	{
	public:
		CefAdaptor(SimpleApp * cef_app);

		void Update();

	private:
		SimpleApp * cef_app_;
	};
}
