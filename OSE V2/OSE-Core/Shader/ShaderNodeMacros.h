#pragma once
#include "EShaderType.h"

// Convert any data into a null-terminated string
#define TO_STRING(x) #x

// Convenience macro for including default methods required by every shader node
// NOTE - All nodes must derive directly from ShaderNode
#define SHADER_NODE( ClassName, ShaderTypes )												\
public:                                                                                     \
	static size_t GetClassType() {															\
		static const std::size_t type { std::hash<std::string>()( TO_STRING(ClassName) ) };	\
		return type;																		\
	}																						\
																							\
	virtual bool IsClassType(const std::size_t classType) const {							\
		return GetClassType() == classType;													\
	}																						\
																							\
	static int32_t GetPossibleShaderTypes() {												\
		return ShaderTypes;																	\
	}																						\
																							\
	virtual bool IsShaderType(int32_t type) const {											\
		return (GetPossibleShaderTypes() & type) == type;									\
	}																						\
private:
