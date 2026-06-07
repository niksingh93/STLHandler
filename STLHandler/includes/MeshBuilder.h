#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "STLFileHandler.h"
#include "STLMesh.h"
#include "STLHandlerExports.h"

typedef std::map<int, std::vector<int>> Leaf;

class STLHANDLER_API MeshBuilder
{
public:
    static STLMesh* GenerateMeshFromStlData(std::vector<Point>& iStlData, Point* iBB_min = NULL, Point* iBB_max = NULL);
private:
    static void GroupPointsByDistance(const std::vector<int>& iPt_lst, const std::vector<int>& iNeighP_tlst, std::vector<int>& ioP2V, const std::vector<Point>& iStlData);
};

