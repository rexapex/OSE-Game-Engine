#include "stdafx.h"
#include "WindowManager.h"

namespace ose::windowing
{
	WindowManager::WindowManager() {}

	WindowManager::~WindowManager() {}

	void WindowManager::SetEngineReferences(RenderingEngine * rendering_engine)
	{
		if(rendering_engine) {
			this->rendering_engine_ = rendering_engine;
		}
	}

	void WindowManager::FramebufferSizeCallbackImpl(int width, int height)
	{
		std::cerr << "FRAMEBUFFER RESIZED: " << width << "x" << height << std::endl;
		this->rendering_engine_->SetFramebufferSize(width, height);
		//rendering_engine_;
		//Must update user interface frame buffer size before updating rendering engine
		//		userInterface->setFramebufferSize(width, height);				//Update the user interface to store the new dimensions
		//		renderingEngine->framebufferSizeCallbackImpl(width, height);	//Update the rendering engines framebuffer
		//	gui->framebufferResized(width, height);
		//		renderingEngine->threadsafe_postShouldUpdateUI();
	}

	void WindowManager::WindowPosCallbackImpl(int x, int y) {}
	void WindowManager::CursorPosCallbackImpl(double xPos, double yPos) {}
	void WindowManager::MouseButtonCallbackImpl(int button, int action, int mods) {}
	void WindowManager::MouseScrollCallbackImpl(double xOffset, double yOffset) {}
	void WindowManager::KeyCallbackImpl(int key, int scancode, int action, int mods) {}
	void WindowManager::CharCallbackImpl(unsigned int codePoint) {}
}
