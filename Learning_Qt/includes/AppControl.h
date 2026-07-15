#pragma once
#include <iostream>
#include <string>
#include "STLMesh.h"
#include "Grapics.h"
#include "STLMeshDiagnosticTopo.h"
#include "STLMeshDiagnosticGeo.h"

class AppControl
{
public:

	void LoadSTLFile(std::string ifilePath);

	void ExportSTLFile(std::string ifilePath);

	void InitializeGraphics(vtkRenderWindow* irenderWindow);

	double GetMeshVolume();

	double GetMeshSurfaceArea();

	double GetMeshNumTriangles();

	double GetMeshNumVertices();

	void ReoriCamX();
	void ReoriCamY();
	void ReoriCamZ();
	void ResetView();
	void ToggleEdgeDisplay(bool idisplay);
	void ToggleNormalDisplay(bool idisplay);

	void DetectNoiseShells();
	void DetectInvertedNormals();

	std::function<void(const std::string&)> RaiseWarning;

private:
	STLMesh* _mesh = NULL;
	Grapics* _graphics = NULL;

	STLMeshDiagnosticTopo* _topoDiagostics = NULL;
	STLMeshDiagnosticGeo* _geoDiagnostics = NULL;
};

inline double AppControl::GetMeshVolume() { return _mesh->GetVolume(); }
inline double AppControl::GetMeshSurfaceArea() { return _mesh->GetSurfaceArea(); }
inline double AppControl::GetMeshNumTriangles() { return _mesh->GetNumFaces(); }
inline double AppControl::GetMeshNumVertices() { return _mesh->GetNumVertices(); }
