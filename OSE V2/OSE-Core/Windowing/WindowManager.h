#pragma once
#include "OSE-Core/Rendering/VideoMode.h"
#include "OSE-Core/Input/EInputType.h"

namespace ose
{
	class WindowCallbackAdaptor;

	class WindowManager
	{
	public:
		WindowManager();
		virtual ~WindowManager() noexcept;
		WindowManager(WindowManager &) = delete;
		WindowManager(WindowManager &&) = delete;
		WindowManager & operator=(WindowManager &) = delete;
		WindowManager & operator=(WindowManager &&) = delete;

		void SetEngineReferences(WindowCallbackAdaptor * adaptor);

		virtual std::vector<VideoMode> GetAvailableVideoModes() = 0;

#	ifdef _WIN32
		virtual HWND GetHWND() const = 0;
#	endif

		//window callbacks
		void FramebufferSizeCallbackImpl(int width, int height);
		void InputCallbackImpl(EInputType type, bool triggered);
		//void WindowPosCallbackImpl(int x, int y);
		void CursorPosCallbackImpl(double xPos, double yPos);
		//void MouseButtonCallbackImpl(int button, int action, int mods);
		//void MouseScrollCallbackImpl(double xOffset, double yOffset);
		//void KeyCallbackImpl(int key, int scancode, int action, int mods);
		//void CharCallbackImpl(unsigned int codePoint);

		virtual int GetFramebufferWidth() const = 0;
		virtual int GetFramebufferHeight() const = 0;
		virtual int GetWindowWidth() const = 0;
		virtual int GetWindowHeight() const = 0;

		virtual int	SetMouseVisibility(int value) = 0;

		virtual void SetWindowSize(int width, int height) = 0;
		virtual void SetWindowPos(int x, int y) = 0;

		virtual void SetTitle(std::string const & title) = 0;

		virtual void SetNumSamples(int numSamples) = 0;

		virtual void NewWindow(int windowMode, int video_mode = -1) = 0;

		// Returns true iff the window is closed
		virtual bool Update() = 0;

		virtual double GetTimeSeconds() const = 0;
	private:
		virtual int	InitWindowingToolkit() const = 0;

		// Reference to the adaptor object which ill process window callbacks
		WindowCallbackAdaptor * callback_adaptor_;
	};
}

