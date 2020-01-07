// NOTE - No #pragma once or include guards
// If a script file includes another script file, macros should be redefined s.t. NAME is set correctly

#include "ControlScript.h"

#ifdef CAT
#	undef CAT
#endif
#ifdef XCAT
#	undef XCAT
#endif
#ifdef NAME
#	undef NAME
#endif
#ifdef CONTROL
#	undef CONTROL
#endif

#define CAT(a, b) a ## b
#define XCAT(a, b) CAT(a, b)

#define TOSTR(x) #x
#define STR(x) TOSTR(x)

#define CONTROL(code) \
	class XCAT(NAME, Control) : public ose::scripting::ControlScript { \
	public: \
		XCAT(NAME, Control)() : ose::scripting::ControlScript() {} \
		std::string GetControlTypeName() const override { return STR(NAME); } \
		code \
	};\
	std::unique_ptr<ose::scripting::ControlScript> XCAT(New, XCAT(NAME, Control))() { \
		return std::make_unique<XCAT(NAME, Control)>(); \
	} \
	auto XCAT(NAME, AddFactoryToMap) = [] { \
		ControlScript::GetSetControlScriptFactory(STR(New) STR(NAME) STR(Control), XCAT(New, XCAT(NAME, Control))); \
		return 0; \
	}();

// Prevents needing to include these in every control script
using namespace ose;
using namespace ose::scripting;

