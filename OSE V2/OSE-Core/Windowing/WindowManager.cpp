#include "../../stdafx.h"
#include "WindowManager.h"

namespace ose::windowing
{
	WindowManager::WindowManager() {}

	WindowManager::~WindowManager() {}

	void WindowManager::setEngineReferences(RenderingEngine * rendering_engine)
	{
		if(rendering_engine) {
			this->rendering_engine_ = rendering_engine;
		}
	}

	void WindowManager::framebufferSizeCallbackImpl(int width, int height)
	{
		std::cerr << "FRAMEBUFFER RESIZED: " << width << "x" << height << std::endl;
		this->rendering_engine_->set_framebuffer_size(width, height);
		//rendering_engine_;
		//Must update user interface frame buffer size before updating rendering engine
		//		userInterface->setFramebufferSize(width, height);				//Update the user interface to store the new dimensions
		//		renderingEngine->framebufferSizeCallbackImpl(width, height);	//Update the rendering engines framebuffer
		//	gui->framebufferResized(width, height);
		//		renderingEngine->threadsafe_postShouldUpdateUI();
	}

	void WindowManager::windowPosCallbackImpl(int x, int y) {}
	void WindowManager::cursorPosCallbackImpl(double xPos, double yPos) {}
	void WindowManager::mouseButtonCallbackImpl(int button, int action, int mods) {}
	void WindowManager::mouseScrollCallbackImpl(double xOffset, double yOffset) {}
	void WindowManager::keyCallbackImpl(int key, int scancode, int action, int mods) {}
	void WindowManager::charCallbackImpl(unsigned int codePoint) {}
}
