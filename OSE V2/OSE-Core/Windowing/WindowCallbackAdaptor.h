#pragma once
#include "OSE-Core/Input/EInputType.h"

namespace ose
{
	class WindowCallbackAdaptor
	{
	public:
		virtual void OnFramebufferResize(int width, int height) = 0;
		virtual void OnInputChange(EInputType type, bool triggered) = 0;
		virtual void OnCursorPosChange(double x, double y) = 0;
	};
}
