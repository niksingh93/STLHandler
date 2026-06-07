#include <stdexcept>
#include <map>
#include <cmath>
#include <iostream>
#include "STLMesh.h"
#include "pch.h"

STLMesh::STLMesh(std::vector<Point>& iStlData, 
				std::vector<int>& iP2V, 
				Point* BBmin, 
				Point* BBmax)
{
	// Fills _faces and _vertices
	FillVertexAndFaceData(iStlData, iP2V);

	//Evaluate Adjacent Faces
	EvaluateAdjacentFaces();

	// Bounding Box variables
	if (BBmin) _BBmin = *BBmin;
	if (BBmax) _BBmax = *BBmax;

	// Evaluate Volume (Can be parallelized; use reduction variable)
	double vol = 0;
	_surfarea = 0;
	for (auto face : _faces) 
	{
		Point p0 = face.GetVertex(0).GetPoint();
		Point p1 = face.GetVertex(1).GetPoint();
		Point p2 = face.GetVertex(2).GetPoint();

		vol += p0.dot(p1.cross(p2));
		_surfarea += face.Area();
	}
	_vol = std::abs(vol) / 6;

}

void STLMesh::FillVertexAndFaceData(std::vector<Point>& iStlData, std::vector<int>& iP2V)
{
	// Number of Vertices
	int NumVertices = *std::max_element(iP2V.begin(), iP2V.end()) + 1;
	std::vector<bool> IsVertexCreated(NumVertices, false);
	_vertices.resize(NumVertices);
	_faces.resize(iP2V.size() / 3);

	// Fill Face and Vertex data
	for (int i = 0; i < iP2V.size() / 3; i++)
	{
		// Index of first point
		int idx = 3 * i;

		// If vertex not created: Create vertex
		if (!IsVertexCreated[iP2V[idx]])
		{
			_vertices[iP2V[idx]] = Vertex(iStlData[idx]);
			IsVertexCreated[iP2V[idx]] = true;
		}

		if (!IsVertexCreated[iP2V[idx + 1]])
		{
			_vertices[iP2V[idx + 1]] = Vertex(iStlData[idx + 1]);
			IsVertexCreated[iP2V[idx + 1]] = true;
		}

		if (!IsVertexCreated[iP2V[idx + 2]])
		{
			_vertices[iP2V[idx + 2]] = Vertex(iStlData[idx + 2]);
			IsVertexCreated[iP2V[idx + 2]] = true;
		}

		// Create Face only after vertex is defined!!
		_faces[i] = TriFace({ iP2V[idx] , iP2V[idx + 1] , iP2V[idx + 2] }, this);

		// Add face to vertex neighbour
		_vertices[iP2V[idx]].AddFace(i);
		_vertices[iP2V[idx + 1]].AddFace(i);
		_vertices[iP2V[idx + 2]].AddFace(i);
	}
}

void STLMesh::EvaluateAdjacentFaces()
{
	// Can be parallelized, evaluation at each face is independent of others
	for (int f_id =0; f_id < _faces.size(); f_id++)
	{
		// All faces which share atleast a vertex with _faces[f_id]
		std::map<int, std::vector<int>> SurroundingFaces;
		for (int j = 0; j < 3; j++)
		{
			// adjFace : All faces containing vertex face.GetVertex(j)
			std::vector<int> adjFace = _faces[f_id].GetVertex(j).GetNeighbouringFaces();

			for (const auto& afidx : adjFace)
			{
				SurroundingFaces[afidx].push_back(j);
			}
		}

		for (const auto& cf : SurroundingFaces)
		{
			if (cf.second.size() < 2 || cf.first == f_id)
				continue;

			// Adding faces which share 2 vertices 
			_faces[f_id].FillAdjacentFaceList(cf.first);
		}

	}
}

Vertex STLMesh::GetVertex(int iVertexId)
{
	if (iVertexId < 0 || iVertexId >= _vertices.size())
		throw std::out_of_range("GetVertex: index out of range");

	return _vertices[iVertexId];
}

TriFace STLMesh::GetFace(int iFaceId)
{
	if (iFaceId < 0 || iFaceId >= _faces.size())
		throw std::out_of_range("GetFace: index out of range");

	return _faces[iFaceId];
}
