#pragma once

#include <vector>
#include <string>
#include "Definitions.h"
#include "STLHandlerExports.h"
#include "STLMesh.h"

class STLHANDLER_API  STLFileHandler
{
public:
    static std::vector<Point> ReadFile(std::string iFile, Point* oMin = NULL, Point* oMax = NULL);

    static void WriteFile(std::string iFile, STLMesh* imesh);
};