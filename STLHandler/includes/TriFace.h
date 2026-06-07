#pragma once
#include "Vertex.h"
#include <vector>
class STLMesh;


class TriFace
{
public:
	TriFace() : _meshPtr(NULL) , _area(0) {};
	TriFace(std::vector<int> iVertexIdx, STLMesh* iMeshPtr);

	void FillAdjacentFaceList(int idx);
	std::vector<int> GetAdjacentFaceList();

	// Get vertex by index [0/1/2]
	std::vector<int> GetVertexIds();
	Vertex GetVertex(int idx);		
	Point GetCentroid();
	Eigen::Vector3d GetNormal();
	double Area();

private:
	STLMesh* _meshPtr;
	std::vector<int> _vertexIdx;
	std::vector<int> _AdjacentFaceList;

	Point _centroid;
	Eigen::Vector3d _normal;
	double _area;

	// Can Add edge list index
};

inline std::vector<int> TriFace::GetAdjacentFaceList() { return _AdjacentFaceList; }

inline std::vector<int> TriFace::GetVertexIds() { return _vertexIdx; }

inline Point TriFace::GetCentroid() { return _centroid; }

inline Eigen::Vector3d TriFace::GetNormal() { return _normal; }

inline double TriFace::Area() { return _area; }