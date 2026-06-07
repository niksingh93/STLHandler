#include <iostream>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkTriangle.h>
#include <vtkPolyData.h>
#include <vtkAxesActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include "pch.h"
#include "Grapics.h"

Grapics::Grapics(vtkRenderWindow* irenderWindow)
{
	_renderer = vtkSmartPointer<vtkRenderer>::New();
	irenderWindow->AddRenderer(_renderer);

	_renderer->GradientBackgroundOn(); // Enable gradient
	_renderer->SetBackground(0.68, 0.85, 0.90); // Light Blue
	_renderer->SetBackground2(1, 1, 1);

	_actor = vtkSmartPointer<vtkActor>::New();
	_renderer->AddActor(_actor);
	_renderer->ResetCamera();

	_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	_actor->SetMapper(_mapper);

	// Orientation axis rendered at TopRight corner 
	auto axes = vtkSmartPointer<vtkAxesActor>::New();
	_orientationWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	_orientationWidget->SetOrientationMarker(axes);

	_orientationWidget->SetInteractor(irenderWindow->GetInteractor());
	_orientationWidget->SetViewport(0.80, 0.80,1.0, 1.0);
	_orientationWidget->EnabledOn();
	_orientationWidget->InteractiveOff();
}

void Grapics::DisplayMesh(STLMesh* imesh)
{
	//std::cout << imesh->GetNumVertices() << std::endl;

	auto points = vtkSmartPointer<vtkPoints>::New();
	auto triangles = vtkSmartPointer<vtkCellArray>::New();

	// Add vertices
	points->SetNumberOfPoints(imesh->GetNumVertices());
	for (int vid = 0; vid < imesh->GetNumVertices(); vid++)
	{
		const Point& p = imesh->GetVertex(vid).GetPoint();
		points->SetPoint(vid, p.x(), p.y(), p.z());
	}

	// Add faces
	for (int fid = 0; fid < imesh->GetNumFaces(); fid++)
	{
		std::vector<int> vertexIds = imesh->GetFace(fid).GetVertexIds();
		if (vertexIds.size() != 3)
			continue;

		auto triangle = vtkSmartPointer<vtkTriangle>::New();
		triangle->GetPointIds()->SetId(0, vertexIds[0]);
		triangle->GetPointIds()->SetId(1, vertexIds[1]);
		triangle->GetPointIds()->SetId(2, vertexIds[2]);

		triangles->InsertNextCell(triangle);
	}

	auto polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(points);
	polyData->SetPolys(triangles);

	_mapper->SetInputData(polyData);

	_actor->SetMapper(_mapper);
	_renderer->ResetCamera();
	_renderer->GetRenderWindow()->Render();
}

void Grapics::ReorientCameraX()
{
	auto camera = _renderer->GetActiveCamera();

	camera->SetPosition(1, 0, 0);
	camera->SetFocalPoint(0, 0, 0);
	camera->SetViewUp(0, 0, 1);

	_renderer->ResetCamera();
	_renderer->GetRenderWindow()->Render();
}

void Grapics::ReorientCameraY()
{
	auto camera = _renderer->GetActiveCamera();

	camera->SetPosition(0, 1, 0);
	camera->SetFocalPoint(0, 0, 0);
	camera->SetViewUp(1, 0, 0);

	_renderer->ResetCamera();
	_renderer->GetRenderWindow()->Render();
}

void Grapics::ReorientCameraZ()
{
	auto camera = _renderer->GetActiveCamera();

	camera->SetPosition(0, 0, 1);
	camera->SetFocalPoint(0, 0, 0);
	camera->SetViewUp(0, 1, 0);

	_renderer->ResetCamera();
	_renderer->GetRenderWindow()->Render();
}