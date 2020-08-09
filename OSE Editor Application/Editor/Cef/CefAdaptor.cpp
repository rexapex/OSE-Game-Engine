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
}
