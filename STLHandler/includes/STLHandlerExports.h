#pragma once

#ifdef STLHANDLER_EXPORTS
#define STLHANDLER_API __declspec(dllexport)
#else
#define STLHANDLER_API __declspec(dllimport)
#endif