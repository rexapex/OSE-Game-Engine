#pragma once

#ifdef EXPORT_TEST_FUNCTIONS
#define UNIT_TEST_EXPORT __declspec(dllexport)
#else
#define UNIT_TEST_EXPORT
#endif

#include <string>
#include <vector>
#include <map>

#include <iostream>
#include <memory>
