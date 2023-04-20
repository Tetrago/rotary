#pragma once

#include <cstdlib>
#include <format>
#include <iostream>

#ifndef NDEBUG
#define VTK_ASSERT(expr, ...) if(!(expr)) { std::cerr << __func__ << ": " << std::format(__VA_ARGS__) << std::endl; std::abort(); }
#else
#define VTK_ASSERT(expr, ...)
#endif
