#include "AppControl.h"
#include "STLFileHandler.h"
#include "MeshBuilder.h"
#include <string>
#include <future>
#include "Error.h"

void AppControl::LoadSTLFile(std::string ifilePath)
{
	std::cout << ifilePath << std::endl;

    std::vector<Point> stlData;
    Point* Min = new Point();
    Point* Max = new Point();

    auto ReadFileThread = std::async(std::launch::async, STLFileHandler::ReadFile, ifilePath, Min, Max);
    stlData = ReadFileThread.get();
    //stlData = STLFileHandler::ReadFile(ifilePath, Min, Max);
    

    // Forget existing mesh data
    if (_mesh) delete _mesh;

    //GenerateMeshFromStlData
    auto MeshGenerationThread = std::async(std::launch::async, MeshBuilder::GenerateMeshFromStlData, std::ref(stlData), Min, Max);
    _mesh = MeshGenerationThread.get();
    //_mesh = MeshBuilder::GenerateMeshFromStlData(stlData, Min, Max);

    // Forget existing mesh data
    if (_topoDiagostics) delete _topoDiagostics;
    if (_geoDiagnostics) delete _geoDiagnostics;

    //Generate Diagnostic tools
    _topoDiagostics = new STLMeshDiagnosticTopo(_mesh);
    _geoDiagnostics = new STLMeshDiagnosticGeo(_mesh);

    _graphics->DisplayMesh(_mesh);
}

void AppControl::ExportSTLFile(std::string ifilePath)
{
    try 
    {
        STLFileHandler::WriteFile(ifilePath, _mesh);
    }
    catch (Error* err)
    {
        //std::cout << err->GetMsg() << std::endl;
        RaiseError(err->GetMsg());
    }
    
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

void AppControl::ResetView() {
    _graphics->ResetView();
}

void AppControl::ToggleEdgeDisplay(bool idisplay)
{
    _graphics->DisplayEdges(idisplay);
}

void AppControl::ToggleNormalDisplay(bool idisplay)
{
    _graphics->DisplayNormalsByColor(idisplay);
}

void AppControl::DetectNoiseShells()
{
    if (_mesh == NULL || _topoDiagostics == NULL) return;

    std::vector<std::vector<int>> IndepedentFaceList;
    _topoDiagostics->DetectNoiseShells(IndepedentFaceList);

    int numOfShells = IndepedentFaceList.size();
    if (numOfShells == 1)
    {
        RaiseInfo("No Noise Shells detected.");
        return;
    }

    // Shell with most number of triangles
    int primaryShellID = 0;
    for (int i = 0; i < IndepedentFaceList.size(); i++)
    {
        if (IndepedentFaceList[i].size() > IndepedentFaceList[primaryShellID].size())
            primaryShellID = i;
    }

    _graphics->DisplayNoiseShells(IndepedentFaceList, primaryShellID, _mesh->GetNumFaces());

    // Pop-Up for user
    RaiseWarning(std::to_string(numOfShells - 1) + " Nosie Shells detected.");

}

void AppControl::DetectInvertedNormals()
{
    if (_mesh == NULL || _geoDiagnostics == NULL) return;

    std::vector<int> FacesWithInvertedNormals;
    FacesWithInvertedNormals = _geoDiagnostics->DetectInvertedNormals();

    if (FacesWithInvertedNormals.size() == 0)
    {
        RaiseInfo("Normal Orientations are consistent.");
        return;
    }  

    _graphics->DisplayInvertedNormals(FacesWithInvertedNormals, _mesh->GetNumFaces());

    // Pop-Up for user
    RaiseWarning(std::to_string(FacesWithInvertedNormals.size() - 1) + " Faces with inverted normals detected.");
}


