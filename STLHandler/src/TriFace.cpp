#include "Vertex.h"
#include "STLMesh.h"
#include "TriFace.h"
#include <eigen3/Eigen/Dense>
#include "pch.h"

TriFace::TriFace(std::vector<int> iVertexIdx, STLMesh* iMeshPtr) : _meshPtr(iMeshPtr) , _area(0)
{
	if (iVertexIdx.size() != 3) return;

	_vertexIdx = iVertexIdx;

	// Evaluate normal and area
	auto v0 = _meshPtr->GetVertex(_vertexIdx[0]).GetPoint();
	auto v1 = _meshPtr->GetVertex(_vertexIdx[1]).GetPoint();
	auto v2 = _meshPtr->GetVertex(_vertexIdx[2]).GetPoint();

	_centroid = (v0 + v1 + v2) / 3;
	_normal = (v1 - v0).cross(v2 - v1);
	_area = _normal.norm() / 2;
	_normal.normalize();
}

Vertex TriFace::GetVertex(int idx) { return _meshPtr->GetVertex(_vertexIdx[idx]); }

void TriFace::FillAdjacentFaceList(int idx)
{
	_AdjacentFaceList.push_back(idx);
}

