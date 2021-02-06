#pragma once

#ifndef WindowManagerClassName
#	error Error: Derived window manager class is undefined
#endif

namespace ose::windowing
{
	WindowManagerClassName::WindowManagerClassName()
	{
		if(InitWindowingToolkit() == -1)
			fprintf(stderr, "Error: %s\n", "Failed to initialise GLFW");
	}

	WindowManagerClassName::~WindowManagerClassName()
	{
		if(window_)
		{
			glfwDestroyWindow(window_);
		}
	}

	static void errorCallback(int error, char const * description)	//Prints error message description to stderr
	{
		fprintf(stderr, "Error: %s\n", description);
	}

	std::vector<VideoMode> WindowManagerClassName::GetAvailableVideoModes()
	{
		int count;
		GLFWvidmode const * modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
		std::vector<VideoMode> video_modes;

		for(int i = 0; i < count; i++)
		{
			video_modes.emplace_back(modes[i].width, modes[i].height, modes[i].refreshRate);
		}

		return video_modes;
	}

	void WindowManagerClassName::NewWindow(int window_mode, int video_mode)
	{
		GLFWwindow * window;

		GLFWvidmode const * mode;

		int num_video_modes;
		GLFWvidmode const * modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &num_video_modes);

		if(video_mode >= 0 && video_mode < num_video_modes)
		{
			mode = &modes[video_mode];
		}
		else
		{
			mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		}

		std::cerr << "Resolution: " << mode->width << "x" << mode->height << std::endl;

		switch(window_mode)
		{
		case 0:	//Fullscreen window
		{
			window = glfwCreateWindow(mode->width, mode->height, "Origami Sheep Engine", glfwGetPrimaryMonitor(), NULL);
			break;
		}
		case 1:	//Windowed mode
		{
			window = glfwCreateWindow(mode->width, mode->height, "Origami Sheep Engine", NULL, NULL);
			break;
		}
		case 2:	//Borderless windowed mode
		{
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
			window = glfwCreateWindow(mode->width, mode->height, "Origami Sheep Engine", glfwGetPrimaryMonitor(), NULL);
			break;
		}
		default: //Default to fullscreen window
		{
			window = glfwCreateWindow(mode->width, mode->height, "Origami Sheep Engine", glfwGetPrimaryMonitor(), NULL);
			break;
		}
		}

		//if there is a window already, destroy it
		if(window_)
		{
			glfwDestroyWindow(window_);
			window_ = nullptr;
		}

		if(!window)
		{
			fprintf(stderr, "Error: %s\n", "Failed to create GLFW window");
		}
		else
		{
			glfwGetFramebufferSize(window, &fbwidth_, &fbheight_);
			glfwGetWindowSize(window, &wwidth_, &wheight_);
			glfwSetWindowUserPointer(window, this);
			glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
			glfwSetKeyCallback(window, KeyCallback);
			//glfwSetWindowPosCallback(window, windowPosCallback);
			//glfwSetCursorPosCallback(window, cursorPosCallback);
			glfwSetMouseButtonCallback(window, MouseButtonCallback);
			//glfwSetScrollCallback(window, mouseScrollCallback);
			//glfwSetCharCallback(window, charCallback);

			std::cerr << "Created GLFW Window" << std::endl;

			glfwMakeContextCurrent(window);
			if(!glfwGetCurrentContext())
			{
				std::cerr << "Failed to make context current" << std::endl;
			}
			else
			{
				std::cerr << "Set window to be default render context" << std::endl;
			}

			window_ = window;
		}
	}

	void WindowManagerClassName::SetTitle(std::string const & title)
	{
		glfwSetWindowTitle(window_, title.c_str());
	}

	int WindowManagerClassName::SetMouseVisibility(int value)
	{
		if(value == 0)
			glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else if(value == 1)
			glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		else if(value == 2)
			glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		return 0;
	}

	void WindowManagerClassName::SetWindowSize(int width, int height)
	{
		glfwSetWindowSize(window_, width, height);
	}

	void WindowManagerClassName::SetWindowPos(int x, int y)
	{
		glfwSetWindowPos(window_, x, y);
	}

	void WindowManagerClassName::SetNumSamples(int numSamples)
	{

	}

	void WindowManagerClassName::FramebufferSizeCallback(GLFWwindow * window, int width, int height)
	{
		WindowManagerClassName * windowManager = reinterpret_cast<WindowManagerClassName *>(glfwGetWindowUserPointer(window));
		windowManager->fbwidth_ = width;
		windowManager->fbheight_ = height;
		windowManager->FramebufferSizeCallbackImpl(width, height);
	}

	/*void WindowManagerClassName::WindowPosCallback(GLFWwindow * window, int x, int y)
	{
		WindowManagerGLFW * windowManager = reinterpret_cast<WindowManagerGLFW *>(glfwGetWindowUserPointer(window));	//Get the window user pointer
		windowManager->WindowPosCallbackImpl(x, y);
	}

	void WindowManagerClassName::CursorPosCallback(GLFWwindow * window, double xPos, double yPos)
	{
		WindowManagerGLFW * windowManager = reinterpret_cast<WindowManagerGLFW *>(glfwGetWindowUserPointer(window));	//Get the window user pointer
		windowManager->CursorPosCallbackImpl(xPos, yPos);		//Forward the callback to the member implementation method
	}*/

	void WindowManagerClassName::MouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
	{
		WindowManagerClassName * window_manager = reinterpret_cast<WindowManagerClassName *>(glfwGetWindowUserPointer(window));
		EInputType type { static_cast<EInputType>(button + 1000) };
		if(action == GLFW_PRESS)
			window_manager->InputCallbackImpl(type, true);
		else if(action == GLFW_RELEASE)
			window_manager->InputCallbackImpl(type, false);
		// NOTE - Ignore GLFW_REPEAT action
	}

	/*void WindowManagerClassName::MouseScrollCallback(GLFWwindow * window, double xOffset, double yOffset)
	{
		WindowManagerGLFW * windowManager = reinterpret_cast<WindowManagerGLFW *>(glfwGetWindowUserPointer(window));	//Get the window user pointer
		windowManager->MouseScrollCallbackImpl(xOffset, yOffset);		//Forward the callback to the member implementation method
	}*/

	void WindowManagerClassName::KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)	//Receives input from the window
	{
		WindowManagerClassName * window_manager = reinterpret_cast<WindowManagerClassName *>(glfwGetWindowUserPointer(window));
		EInputType type { static_cast<EInputType>(key) };
		if(action == GLFW_PRESS)
			window_manager->InputCallbackImpl(type, true);
		else if(action == GLFW_RELEASE)
			window_manager->InputCallbackImpl(type, false);
		// NOTE - Ignore GLFW_REPEAT action
	}

	/*void WindowManagerClassName::CharCallback(GLFWwindow * window, unsigned int codePoint)
	{
		WindowManagerGLFW * windowManager = reinterpret_cast<WindowManagerGLFW *>(glfwGetWindowUserPointer(window));	//Get the window user pointer
		windowManager->CharCallbackImpl(codePoint);								//Forward the callback to the member implementation method
	}*/
}
