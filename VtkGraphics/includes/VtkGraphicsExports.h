#pragma once

#ifdef VTKGRAPHICS_EXPORTS
#define VTKGRAPHICS_API __declspec(dllexport)
#else
#define VTKGRAPHICS_API __declspec(dllimport)
#endif