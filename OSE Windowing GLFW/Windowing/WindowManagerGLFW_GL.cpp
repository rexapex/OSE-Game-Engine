#include "pch.h"
//#include <GLFW/glfw3.h>
//#define WINDOW_MANAGER_EXT_NAME GL
//#include "WindowManagerGLFW.h"
//#include "WindowManagerGLFW.inl"
//
//namespace ose::windowing
//{
//	int WindowManagerClassName::InitWindowingToolkit() const
//	{
//		glfwSetErrorCallback(errorCallback);
//
//		if(!glfwInit())		/**Initialise GLFW*/
//			return -1;		/**Return -1 if failed to initialise*/
//
//		return 0;
//	}
//
//	void WindowManagerClassName::Update()
//	{
//		//swap buffers to update the screen and then poll for new events
//		glfwSwapBuffers(window_);
//		glfwPollEvents();
//
//		//check if game should be closed
//		if(glfwWindowShouldClose(window_))
//		{
//			glfwDestroyWindow(window_);
//			window_ = nullptr;
//			exit(0);
//		}
//
//		WindowManagerClassName * window_manager = reinterpret_cast<WindowManagerClassName *>(glfwGetWindowUserPointer(window_));
//		double xpos, ypos;
//		glfwGetCursorPos(window_, &xpos, &ypos);		//Callback function not called frequently enough to update camera
//		window_manager->CursorPosCallbackImpl(xpos, ypos);
//	}
//}
