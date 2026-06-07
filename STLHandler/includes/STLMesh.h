#pragma once
#include "Vertex.h"
#include "TriFace.h"
#include <vector>
#include "STLHandlerExports.h"


class STLHANDLER_API STLMesh
{
public:
	STLMesh(std::vector<Point>& iStlData, std::vector<int>& iP2V, Point* BBmin = NULL, Point* BBmax = NULL);
	
	double GetVolume();
	double GetSurfaceArea();
	
	Vertex GetVertex(int iVertexId);
	TriFace GetFace(int iFaceId);
	int GetNumVertices();
	int GetNumFaces();
private:

	void FillVertexAndFaceData(std::vector<Point>& iStlData, std::vector<int>& iP2V);
	void EvaluateAdjacentFaces();

	std::vector<Vertex> _vertices;
	std::vector<TriFace> _faces;
	Point _BBmin;
	Point _BBmax;

	double _vol;
	double _surfarea;
};

inline int		STLMesh::GetNumVertices()	{ return _vertices.size(); }
inline int		STLMesh::GetNumFaces()		{ return _faces.size(); }
inline double	STLMesh::GetVolume()		{ return _vol; }
inline double	STLMesh::GetSurfaceArea()	{ return _surfarea; }
