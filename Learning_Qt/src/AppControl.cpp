#include "AppControl.h"
#include "STLFileHandler.h"
#include "MeshBuilder.h"

void AppControl::LoadSTLFile(std::string ifilePath)
{
	std::cout << ifilePath << std::endl;

    std::vector<Point> stlData;
    Point* Min = new Point();
    Point* Max = new Point();
    stlData = STLFileHandler::ReadFile(ifilePath, Min, Max);

    // Forget existing mesh data
    if (_mesh) delete _mesh;

    //GenerateMeshFromStlData
    _mesh = MeshBuilder::GenerateMeshFromStlData(stlData, Min, Max);

    _graphics->DisplayMesh(_mesh);
}

void AppControl::ExportSTLFile(std::string ifilePath)
{
    STLFileHandler::WriteFile(ifilePath, _mesh);
}

void AppControl::InitializeGraphics(vtkRenderWindow* irenderWindow)
{
    _graphics = new Grapics(irenderWindow);
}

void AppControl::ReoriCamX()
{
    _graphics->ReorientCameraX();
}

void AppControl::ReoriCamY()
{
    _graphics->ReorientCameraY();
}

void AppControl::ReoriCamZ()
{
    _graphics->ReorientCameraZ();
}

void AppControl::ToggleEdgeDisplay(bool idisplay)
{
    _graphics->DisplayEdges(idisplay);
}

void AppControl::ToggleNormalDisplay(bool idisplay)
{
    _graphics->DisplayNormalsByColor(idisplay);
}