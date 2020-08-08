// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "stdafx.h"

#include "CefApp.h"
#include "SimpleHandler.h"

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_stream_resource_handler.h"

#include "OSE-Core/File System/FileSystemUtil.h"
#include "OSE-Core/Windowing/WindowManager.h"

using namespace ose::editor;

namespace {

	// When using the Views framework this object provides the delegate
	// implementation for the CefWindow that hosts the Views-based browser.
	class SimpleWindowDelegate : public CefWindowDelegate {
	public:
		explicit SimpleWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
			: browser_view_(browser_view) {}

		void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE {
			// Add the browser view and show the window.
			window->AddChildView(browser_view_);
			window->Show();

			// Give keyboard focus to the browser view.
			browser_view_->RequestFocus();
		}

		void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE {
			browser_view_ = NULL;
		}

		bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE {
			// Allow the window to close if the browser says it's OK.
			CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
			if (browser)
				return browser->GetHost()->TryCloseBrowser();
			return true;
		}

		CefSize GetPreferredSize(CefRefPtr<CefView> view) OVERRIDE {
			return CefSize(800, 600);
		}

	private:
		CefRefPtr<CefBrowserView> browser_view_;

		IMPLEMENT_REFCOUNTING(SimpleWindowDelegate);
		DISALLOW_COPY_AND_ASSIGN(SimpleWindowDelegate);
	};

}  // namespace

SimpleApp::SimpleApp() {}

void SimpleApp::OnContextInitialized() {
	CEF_REQUIRE_UI_THREAD();

	// Create the controller object
	controller_ = std::make_unique<Controller>();
	controller_->LoadProject("OSE-TestProject");
	controller_->LoadScene("scene1");
	controller_->SetActiveScene("scene1");

	CefRefPtr<CefCommandLine> command_line =
		CefCommandLine::GetGlobalCommandLine();

	// SimpleHandler implements browser-level callbacks.
	CefRefPtr<SimpleHandler> handler(new SimpleHandler(false, *controller_));

	// Specify CEF browser settings here.
	CefBrowserSettings browser_settings;

	std::string url = "about:blank";

	// Information used when creating the native window.
	CefWindowInfo window_info;

#if defined(OS_WIN)
	// On Windows we need to specify certain flags that will be passed to
	// CreateWindowEx().
	window_info.SetAsPopup(NULL, "OSE");
#endif

	// Pass the controller window to cef
#ifdef OS_WIN
	HWND hwnd = controller_->GetWindowManager().GetHWND();
	if(hwnd != NULL)
		window_info.SetAsWindowless(hwnd);
#endif
	auto browser = CefBrowserHost::CreateBrowserSync(window_info, handler, "about:blank", browser_settings, NULL, NULL);

	// Navigate to the test html file
	// TODO - Use relative url and copy html files to output directory
	//browser->GetMainFrame()->LoadURL(R"(file:\\\D:\James\Documents\Visual Studio 2017\Projects\OSE V2\OSE Editor Application\Editor\View\Html\test.html)");
	browser->GetMainFrame()->LoadURL(R"(D:\James\Repos\Webpage\OSE Editor Design\index.html)");
}
