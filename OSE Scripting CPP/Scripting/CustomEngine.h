#pragma once
#include "OSE-Core/Scripting/ScriptingEngine.h"

namespace ose::scripting
{
	class CustomEngine
	{
	protected:
		CustomEngine();

	public:
		virtual ~CustomEngine();

		virtual void Init() {}
		virtual void Update() {}
	};
}
