#pragma once
#include "OSE-Core/Rendering/VideoMode.h"
#include "OSE-Core/Rendering/RenderingEngine.h"

namespace ose::windowing
{
	using namespace rendering;

	class WindowManager
	{
	public:
		WindowManager();
		virtual ~WindowManager() noexcept;
		WindowManager(WindowManager &) = delete;
		WindowManager(WindowManager &&) = delete;
		WindowManager & operator=(WindowManager &) = delete;
		WindowManager & operator=(WindowManager &&) = delete;

		void setEngineReferences(RenderingEngine * rendering_engine);

		virtual std::vector<VideoMode> getAvailableVideoModes() = 0;

		//window callbacks
		void framebufferSizeCallbackImpl(int width, int height);
		void windowPosCallbackImpl(int x, int y);
		void cursorPosCallbackImpl(double xPos, double yPos);
		void mouseButtonCallbackImpl(int button, int action, int mods);
		void mouseScrollCallbackImpl(double xOffset, double yOffset);
		void keyCallbackImpl(int key, int scancode, int action, int mods);
		void charCallbackImpl(unsigned int codePoint);

		virtual const int getFramebufferWidth() const = 0;
		virtual const int getFramebufferHeight() const = 0;
		virtual const int getWindowWidth() const = 0;
		virtual const int getWindowHeight() const = 0;

		virtual int	setMouseVisibility(int value) = 0;

		virtual void setWindowSize(int width, int height) = 0;
		virtual void setWindowPos(int x, int y) = 0;

		virtual void setTitle(const std::string & title) = 0;

		virtual void setNumSamples(int numSamples) = 0;

		virtual void createWindow(int windowMode, int video_mode = -1) = 0;

		virtual void update() = 0;

		virtual double getTimeSeconds() const = 0;
	private:
		virtual int	initWindowingToolkit() const = 0;

		// pointers should not be de-allocated by this class
		RenderingEngine * rendering_engine_ { nullptr };
	};
}

