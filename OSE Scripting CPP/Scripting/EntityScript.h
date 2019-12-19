#pragma once
#include "CustomEngine.h"

#ifdef CAT
#	undef CAT
#endif
#ifdef XCAT
#	undef XCAT
#endif
#ifdef NAME
#	undef NAME
#endif
#ifdef DATA
#	undef DATA
#endif
#ifdef ENGINE
#	undef ENGINE
#endif

#define CAT(a, b) a ## b
#define XCAT(a, b) CAT(a, b)

#define DATA(code) \
	struct XCAT(NAME, Data) { code };

#define ENGINE(code) \
	class XCAT(NAME, Engine) : public ose::scripting::CustomEngine { \
	public: \
		XCAT(NAME, Engine)() : ose::scripting::CustomEngine() {} \
	private: \
		code \
	};\
	std::unique_ptr<ose::scripting::CustomEngine> XCAT(New, XCAT(NAME, Engine))() { \
		return std::make_unique<XCAT(NAME, Engine)>(); \
	}
