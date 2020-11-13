#pragma once

#include "OSE-Core/Windowing/WindowManager.h"
#include "OSE-Core/Rendering/VideoMode.h"

struct GLFWwindow;
double glfwGetTime();

namespace ose::windowing
{
	class WindowManagerGLFW : public WindowManager
	{
	public:
		WindowManagerGLFW();
		virtual ~WindowManagerGLFW() noexcept;
		WindowManagerGLFW(WindowManagerGLFW &) = delete;
		WindowManagerGLFW(WindowManagerGLFW &&) = delete;
		WindowManagerGLFW & operator=(WindowManagerGLFW &) = delete;
		WindowManagerGLFW & operator=(WindowManagerGLFW &&) = delete;

		int GetFramebufferWidth() const { return fbwidth_; }
		int GetFramebufferHeight() const { return fbheight_; }
		int GetWindowWidth() const { return wwidth_; }
		int GetWindowHeight() const { return wheight_; }

		std::vector<VideoMode> GetAvailableVideoModes();

		int	SetMouseVisibility(int value);

		void SetWindowSize(int width, int height);
		void SetWindowPos(int x, int y);

		void SetTitle(std::string const & title);

		void SetNumSamples(int numSamples);

		GLFWwindow const & GetWindow() const { return *window_; }
		void NewWindow(int windowMode, int video_mode);

		void Update();

		double GetTimeSeconds() const {return glfwGetTime();}
	private:
		int InitWindowingToolkit() const;

		GLFWwindow * window_;

		int fbwidth_, fbheight_;	// framebuffer width & height
		int wwidth_, wheight_;		// window width & height

		static void FramebufferSizeCallback(GLFWwindow * window, int width, int height);
		//static void WindowPosCallback(GLFWwindow * window, int x, int y);
		//static void CursorPosCallback(GLFWwindow * window, double xPos, double yPos);
		static void MouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
		//static void MouseScrollCallback(GLFWwindow * window, double xOffset, double yOffset);
		static void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);	//Static function which receives key callbacks from window
		//static void CharCallback(GLFWwindow * window, unsigned int codePoint);
	};
}

