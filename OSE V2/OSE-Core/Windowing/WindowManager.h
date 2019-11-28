#pragma once
#include "OSE-Core/Rendering/VideoMode.h"

namespace ose
{
	namespace rendering
	{
		class RenderingEngine;
	}
}

namespace ose::windowing
{
	class WindowManager
	{
	public:
		WindowManager();
		virtual ~WindowManager() noexcept;
		WindowManager(WindowManager &) = delete;
		WindowManager(WindowManager &&) = delete;
		WindowManager & operator=(WindowManager &) = delete;
		WindowManager & operator=(WindowManager &&) = delete;

		void SetEngineReferences(ose::rendering::RenderingEngine * rendering_engine);

		virtual std::vector<VideoMode> GetAvailableVideoModes() = 0;

		//window callbacks
		void FramebufferSizeCallbackImpl(int width, int height);
		void WindowPosCallbackImpl(int x, int y);
		void CursorPosCallbackImpl(double xPos, double yPos);
		void MouseButtonCallbackImpl(int button, int action, int mods);
		void MouseScrollCallbackImpl(double xOffset, double yOffset);
		void KeyCallbackImpl(int key, int scancode, int action, int mods);
		void CharCallbackImpl(unsigned int codePoint);

		virtual const int GetFramebufferWidth() const = 0;
		virtual const int GetFramebufferHeight() const = 0;
		virtual const int GetWindowWidth() const = 0;
		virtual const int GetWindowHeight() const = 0;

		virtual int	SetMouseVisibility(int value) = 0;

		virtual void SetWindowSize(int width, int height) = 0;
		virtual void SetWindowPos(int x, int y) = 0;

		virtual void SetTitle(const std::string & title) = 0;

		virtual void SetNumSamples(int numSamples) = 0;

		virtual void NewWindow(int windowMode, int video_mode = -1) = 0;

		virtual void Update() = 0;

		virtual double GetTimeSeconds() const = 0;
	private:
		virtual int	InitWindowingToolkit() const = 0;

		// pointers should not be de-allocated by this class
		ose::rendering::RenderingEngine * rendering_engine_ { nullptr };
	};
}

