#include "Vertex.h"
#include "pch.h"

Vertex::Vertex(Point& ipoint) : _pt(ipoint) {}

void Vertex::AddFace(int iFaceID)
{
	_neighFaces.push_back(iFaceID);
}