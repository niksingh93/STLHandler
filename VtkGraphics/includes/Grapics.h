#pragma once
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkOrientationMarkerWidget.h>
#include "VtkGraphicsExports.h"
#include "STLMesh.h"

class VTKGRAPHICS_API Grapics
{
public:
	Grapics(vtkRenderWindow* irenderWindow);

	void DisplayMesh(STLMesh* imesh);

	void ReorientCameraX();
	void ReorientCameraY();
	void ReorientCameraZ();

private:
	//vtkRenderWindow* _renderWindow = NULL;
	vtkSmartPointer<vtkRenderer> _renderer = NULL;
	vtkSmartPointer<vtkActor> _actor = NULL;
	vtkSmartPointer<vtkPolyDataMapper> _mapper = NULL;
	vtkSmartPointer<vtkOrientationMarkerWidget> _orientationWidget = NULL;
};

