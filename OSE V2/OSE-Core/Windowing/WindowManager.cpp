#include "stdafx.h"
#include "WindowManager.h"
#include "OSE-Core/Rendering/RenderingEngine.h"
#include "OSE-Core/Input/InputManager.h"

namespace ose
{
	WindowManager::WindowManager() {}

	WindowManager::~WindowManager() {}

	void WindowManager::SetEngineReferences(RenderingEngine * rendering_engine, InputManager * input_manager)
	{
		if(rendering_engine) {
			this->rendering_engine_ = rendering_engine;
		}
		if(input_manager) {
			this->input_manager_= input_manager;
		}
	}

	void WindowManager::FramebufferSizeCallbackImpl(int width, int height)
	{
		DEBUG_LOG("Framebuffer resized to", width, "x", height);
		this->rendering_engine_->SetFramebufferSize(width, height);
	}
	
	void WindowManager::InputCallbackImpl(EInputType type, bool triggered)
	{
		input_manager_->SetInputType(type, triggered);
	}

	//void WindowManager::WindowPosCallbackImpl(int x, int y) {}
	//void WindowManager::CursorPosCallbackImpl(double xPos, double yPos) {}
	//void WindowManager::MouseButtonCallbackImpl(int button, int action, int mods) {}
	//void WindowManager::MouseScrollCallbackImpl(double xOffset, double yOffset) {}
	//void WindowManager::KeyCallbackImpl(int key, int scancode, int action, int mods) {}
	//void WindowManager::CharCallbackImpl(unsigned int codePoint) {}
}
