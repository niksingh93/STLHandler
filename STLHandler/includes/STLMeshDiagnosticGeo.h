#pragma once
#include "STLMesh.h"
class STLMeshDiagnosticGeo
{
public:
	STLMeshDiagnosticGeo(STLMesh* mesh = NULL) : _mesh(mesh) {};

    std::vector<int> DetectDegenerateTriangles();
	std::vector<int> DetectInvertedNormals();
private:
	STLMesh* _mesh;

	bool HaveSameOri(TriFace& iFirstFace, TriFace& iSecondFace);

	void FacesIntersectingRay(
		const Point& point, 
		const Eigen::Vector3d idir,
		const std::vector<int>& topFaces,
		std::vector<int>& oIntersectingFaces,
		std::vector<Point>& oIntersectionPoints) const;
};

