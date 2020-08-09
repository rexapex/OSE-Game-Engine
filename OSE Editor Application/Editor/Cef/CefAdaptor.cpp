#include "stdafx.h"
#include "CefAdaptor.h"
#include "CefApp.h"

namespace ose
{
	CefAdaptor::CefAdaptor(SimpleApp * cef_app) : cef_app_(cef_app)
	{
	
	}

	void CefAdaptor::Update()
	{
		cef_app_->Update();
	}

	void CefAdaptor::SetFramebufferSize(int width, int height)
	{
		cef_app_->Invalidate(width, height);
	}
}
