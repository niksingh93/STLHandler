#include <iostream>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkTriangle.h>
#include <vtkPolyData.h>
#include <vtkAxesActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkFloatArray.h>
#include <vtkCellData.h>
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
	_actor->GetProperty()->LightingOff();
	//_actor->GetProperty()->GetColor(_defaultColor);
	_renderer->AddActor(_actor);
	_renderer->ResetCamera();
	
	_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	_actor->SetMapper(_mapper);

	// Orientation axis rendered at TopRight corner 
	auto axes = vtkSmartPointer<vtkAxesActor>::New();
	axes->SetTotalLength(1.5, 1.5, 1.5);

	axes->SetShaftTypeToCylinder();
	axes->SetCylinderRadius(0.04);

	axes->SetConeRadius(0.60);
	axes->SetSphereRadius(0.2);

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
	auto normals = vtkSmartPointer<vtkFloatArray>::New();
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

		// Add normal
		auto fnormal = imesh->GetFace(fid).GetNormal();
		fnormal.normalize();
		normals->SetNumberOfComponents(3);
		normals->SetName("Normals");
		normals->InsertNextTuple3(fnormal.x(), fnormal.y(), fnormal.z());
	}

	auto polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(points);
	polyData->SetPolys(triangles);
	polyData->GetCellData()->SetNormals(normals);


	_mapper->SetInputData(polyData);

	_actor->SetMapper(_mapper);
	_actor->GetProperty()->SetInterpolationToFlat();

	//_actor->GetProperty()->EdgeVisibilityOn();
	_renderer->ResetCamera();
	_renderer->GetRenderWindow()->Render();

	
}

void Grapics::DisplayEdges(bool idisplay)
{
	if (_renderer == NULL || _actor == NULL) return;

	if (idisplay)
		_actor->GetProperty()->EdgeVisibilityOn();
	else
		_actor->GetProperty()->EdgeVisibilityOff();

	_renderer->GetRenderWindow()->Render();
}

void Grapics::DisplayNormalsByColor(bool idisplay)
{
	if (_renderer == NULL || _actor == NULL) return;
		
	if (idisplay)
	{
		if (!_actor->GetProperty())
			_actor->SetProperty(vtkSmartPointer<vtkProperty>::New());

		if (!_actor->GetBackfaceProperty())
			_actor->SetBackfaceProperty(vtkSmartPointer<vtkProperty>::New());

		// Copy of default properties [automatically deletes old defauls]
		_defaultFaceProperty = vtkSmartPointer<vtkProperty>::New();
		_defaultFaceProperty->DeepCopy(_actor->GetProperty());

		_defaultbackFaceProperty = vtkSmartPointer<vtkProperty>::New();
		_defaultbackFaceProperty->DeepCopy(_actor->GetBackfaceProperty());

		// Highlight front face in Green and back face in red
		_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
		_actor->GetBackfaceProperty()->SetColor(1.0, 0.0, 0.0);
	}
	else
	{
		double defaultColor[3];
		_defaultFaceProperty->GetColor(defaultColor);
		_actor->GetProperty()->SetColor(defaultColor);

		double defaultbackColor[3];
		_defaultbackFaceProperty->GetColor(defaultbackColor);
		_actor->GetBackfaceProperty()->SetColor(defaultbackColor);
	}

	_renderer->GetRenderWindow()->Render();
}

void Grapics::ReorientCameraX()
{
	if (!_renderer) return;

	auto camera = _renderer->GetActiveCamera();

	camera->SetPosition(1, 0, 0);
	camera->SetFocalPoint(0, 0, 0);
	camera->SetViewUp(0, 0, 1);

	_renderer->ResetCamera();
	_renderer->GetRenderWindow()->Render();
}

void Grapics::ReorientCameraY()
{
	if (!_renderer) return;

	auto camera = _renderer->GetActiveCamera();

	camera->SetPosition(0, 1, 0);
	camera->SetFocalPoint(0, 0, 0);
	camera->SetViewUp(1, 0, 0);

	_renderer->ResetCamera();
	_renderer->GetRenderWindow()->Render();
}

void Grapics::ReorientCameraZ()
{
	if (!_renderer) return;

	auto camera = _renderer->GetActiveCamera();

	camera->SetPosition(0, 0, 1);
	camera->SetFocalPoint(0, 0, 0);
	camera->SetViewUp(0, 1, 0);

	_renderer->ResetCamera();
	_renderer->GetRenderWindow()->Render();
}