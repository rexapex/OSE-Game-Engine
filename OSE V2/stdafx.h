#pragma once

#include <string>
#include <vector>
#include <map>
#include <array>
#include <unordered_map>

#include <numeric>
#include <algorithm>

#include <fstream>
#include <sstream>
#include <iostream>

#include <memory>
#include <functional>
#include <any>
#include <variant>
#include <initializer_list>

#define GLM_FORCE_RADIANS
#include "OSE-Core/EngineDependencies/glm/glm.hpp"
#include "OSE-Core/EngineDependencies/glm/gtc/quaternion.hpp"
#include "OSE-Core/EngineDependencies/glm/gtx/quaternion.hpp"
#include "OSE-Core/EngineDependencies/glm/gtc/type_ptr.hpp"
#include "OSE-Core/EngineDependencies/glm/gtc/matrix_transform.hpp"

#include "OSE-Core/Types.h"
#include "OSE-Core/Logging.h"

#ifdef _WIN32
#	define WIN32_LEAN_AND_MEAN
#	define NOMINMAX
#	include <Windows.h>
#endif
