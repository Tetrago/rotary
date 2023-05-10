#pragma once

#ifdef ROTARY_SHARED
#ifdef _WIN32

#ifdef ROTARY_BUILD
#define ROTARY_API __declspec(dllexport)
#else
#define ROTARY_API __declspec(dllimport)
#endif

#else

#ifdef ROTARY_BUILD
#define ROTARY_API __attribute__((visibility("default")))
#elseif
#define ROTARY_API
#endif

#endif
#else
#define ROTARY_API
#endif
