#pragma once

// Include the common precompiled header
#include "stdafx.h"

// Include the required headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
