#include "stdafx.h"
#include "WindowManager.h"
#include "WindowCallbackAdaptor.h"

namespace ose
{
	WindowManager::WindowManager() {}

	WindowManager::~WindowManager() {}

	void WindowManager::SetEngineReferences(WindowCallbackAdaptor * adaptor)
	{
		if(adaptor)
			this->callback_adaptor_ = adaptor;
	}

	void WindowManager::FramebufferSizeCallbackImpl(int width, int height)
	{
		DEBUG_LOG("Framebuffer resized to", width, "x", height);
		this->callback_adaptor_->OnFramebufferResize(width, height);
	}
	
	void WindowManager::InputCallbackImpl(EInputType type, bool triggered)
	{
		callback_adaptor_->OnInputChange(type, triggered);
	}

	//void WindowManager::WindowPosCallbackImpl(int x, int y) {}
	
	void WindowManager::CursorPosCallbackImpl(double xPos, double yPos)
	{
		callback_adaptor_->OnMousePosChange(xPos, yPos);
	}

	//void WindowManager::MouseButtonCallbackImpl(int button, int action, int mods) {}
	//void WindowManager::MouseScrollCallbackImpl(double xOffset, double yOffset) {}
	//void WindowManager::KeyCallbackImpl(int key, int scancode, int action, int mods) {}
	//void WindowManager::CharCallbackImpl(unsigned int codePoint) {}
}
