#pragma once
#include <eigen3/Eigen/Dense>
#include <vector>
#include "Definitions.h"

class Vertex
{
public:
	Vertex() {};
	Vertex(Point& ipoint);

	void AddFace(int iFaceID);

	std::vector<int> GetNeighbouringFaces();
	Point GetPoint();
	double GetX();
	double GetY();
	double GetZ();

private:
	Point _pt;
	std::vector<int> _neighFaces;

	// Can Add Edge data [List of edges]
};

inline std::vector<int> Vertex::GetNeighbouringFaces() { return _neighFaces;}
inline Point Vertex::GetPoint() { return _pt; }
inline double Vertex::GetX() { return _pt[0]; }
inline double Vertex::GetY() { return _pt[1]; }
inline double Vertex::GetZ() { return _pt[2]; }


