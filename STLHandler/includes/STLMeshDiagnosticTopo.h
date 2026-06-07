#pragma once
#include <vector>
#include <unordered_map>
#include "STLMesh.h"
class STLMeshDiagnosticTopo
{
public:
	STLMeshDiagnosticTopo(STLMesh* mesh = NULL) : _mesh(mesh) {};

	void DetectNoiseShells(std::vector<std::vector<int>> oIndepedentFaceList);

	void DetectOpenEdges();

private:
	STLMesh* _mesh;
};

