#include "pch.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define WINDOW_MANAGER_EXT_NAME VK
#include "WindowManagerGLFW.h"
#include "WindowManagerGLFW.inl"

namespace ose::windowing
{
	int WindowManagerClassName::InitWindowingToolkit() const
	{
		glfwSetErrorCallback(errorCallback);

		if(!glfwInit())		/**Initialise GLFW*/
			return -1;		/**Return -1 if failed to initialise*/

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		return 0;
	}

	void WindowManagerClassName::Update()
	{
		//swap buffers to update the screen and then poll for new events
		//glfwSwapBuffers(window_);
		glfwPollEvents();

		//check if game should be closed
		if(glfwWindowShouldClose(window_))
		{
			glfwDestroyWindow(window_);
			window_ = nullptr;
			exit(0);
		}

		WindowManagerClassName * window_manager = reinterpret_cast<WindowManagerClassName *>(glfwGetWindowUserPointer(window_));
		double xpos, ypos;
		glfwGetCursorPos(window_, &xpos, &ypos);		//Callback function not called frequently enough to update camera
		window_manager->CursorPosCallbackImpl(xpos, ypos);
	}

	std::vector<char const *> WindowManagerClassName::GetExtensions()
	{
		std::vector<char const *> extensions;
		uint32_t ext_count = 0;
		const char ** exts;
		exts = glfwGetRequiredInstanceExtensions(&ext_count);
		for(size_t i = 0; i < ext_count; ++i)
			extensions.push_back(exts[i]);
		return extensions;
	}
}
