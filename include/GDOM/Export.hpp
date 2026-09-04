#pragma once

#if defined(_WIN32)

#ifdef ZHULIS_GDOM_EXPORTING
#define GDOM_API __declspec(dllexport)
#else
#define GDOM_API __declspec(dllimport)
#endif

#else

#define GDOM_API __attribute__((visibility("default")))

#endif