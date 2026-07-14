#pragma once
//#include <QApplication>
#include <QMainWindow>
//#include <QSurfaceFormat>
#include <qaction.h>
#include <qlabel.h>
#include <qwidget.h>
#include <qdockwidget.h>
#include <QVTKOpenGLNativeWidget.h>
#include "AppControl.h"

class MainWindow : public QMainWindow
{
public:
    MainWindow();

private:
    void OnButtonClicked();

    void OnOpenClicked();

    void OnSaveClicked();

    void OnReoriCamXClicked();
    void OnReoriCamYClicked();
    void OnReoriCamZClicked();
    void OnResetViewClicked();

    void OnEdgeDisplayToggled(bool active);

    void OnNormalDisplayToggled(bool active);

    void OnDetectNoiseShellsClicked();

    void OnDetectInvertedNormalsClicked();


    void CreateActions();

    void CreateToolbar();

    void CreateMenus();

    void CreateToolDockWidget();

    void CreateStatisticsWidget();

    void DefineActionBehaviour();

    void UpdateMeshStatistics();

    // Action List
    QAction* _openAction = NULL;
    QAction* _saveAction = NULL;
    QAction* _toggleEdgeDisplay = NULL;
    QAction* _toggleNormalDisplay = NULL;

    
    QAction* _reorientCameraX = NULL;
    QAction* _reorientCameraY = NULL;
    QAction* _reorientCameraZ = NULL;
    QAction* _resetCameraAction = NULL;

    QAction* _detectNoiseShells = NULL;
    QAction* _detectInvertedNormals = NULL;


    // Widgets
    QVTKOpenGLNativeWidget* _vtkWidget = NULL;
    QWidget* _centerWidget = NULL;
    QWidget* _statsWidget = NULL;
    QDockWidget* _toolDock = NULL;
    

    // Labels
    QLabel* _volumeLabel;
    QLabel* _surfaceAreaLabel;
    QLabel* _triNumLabel;
    QLabel* _vertexNumLabel;

    // AppControl
    AppControl* _AppControl = NULL;
};

