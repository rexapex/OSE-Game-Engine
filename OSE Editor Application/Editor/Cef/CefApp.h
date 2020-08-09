// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include "include/cef_app.h"
#include "Editor/Controller/Controller.h"

class SimpleHandler;

// Implement application-level callbacks for the browser process.
class SimpleApp : public CefApp, public CefBrowserProcessHandler
{
public:
	SimpleApp();
	~SimpleApp();

	// CefApp methods:
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
		OVERRIDE
	{
		return this;
	}

	// CefBrowserProcessHandler methods:
	virtual void OnContextInitialized() OVERRIDE;

	// Start the app (controller)
	void StartApp();

	// Update the cef app
	void Update();

	// Invalidate the current gui texture
	void Invalidate(int fbwidth, int fbheight);

private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(SimpleApp);

	// Game object used to simplify interface between OSE and editor
	std::unique_ptr<ose::editor::Controller> controller_;

	// Cef browser created on initialisation
	CefRefPtr<CefBrowser> browser_;

	// Cef browser handler object
	CefRefPtr<SimpleHandler> handler_;
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
