#pragma once

#ifdef ERRORLIB_EXPORTS
#define ERRORLIB_API __declspec(dllexport)
#else
#define ERRORLIB_API __declspec(dllimport)
#endif // ERRORLIB_EXPORTS
