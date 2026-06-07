#include "MainWindow.h"
#include <QPushButton>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QDockWidget>
#include <QIcon>
#include <QFileDialog>
#include <qstatusbar.h>
#include <qtoolbutton.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkNew.h>

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>

#include<string>
#include <iostream>

// Used with qdebug and Qdir commands in create action
//#include <QDir>
//#include <QDebug>

MainWindow::MainWindow()
{

    // Resize application window
    resize(800, 600);

    //_centerWidget = new QWidget(this);
    //setCentralWidget(_centerWidget);

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    QVTKOpenGLNativeWidget* vtkWidget = new QVTKOpenGLNativeWidget(this); 
    vtkWidget->setRenderWindow(renderWindow);

    // Object of Connenter class
    _AppControl = new AppControl();
    _AppControl->InitializeGraphics(renderWindow);

    //vtkNew<vtkRenderer> renderer;
    //renderWindow->AddRenderer(renderer);
    //renderer->GradientBackgroundOn(); // Enable gradient
    //renderer->SetBackground(0.68, 0.85, 0.90); // Light Blue
    //renderer->SetBackground2(1, 1, 1); 

    //vtkNew<vtkSphereSource> sphere;
    //sphere->Update();

    //vtkNew<vtkPolyDataMapper> mapper;
    //mapper->SetInputConnection(sphere->GetOutputPort());

    //vtkNew<vtkActor> actor;
    //actor->SetMapper(mapper);

    //renderer->AddActor(actor);
    //renderer->ResetCamera();

    setCentralWidget(vtkWidget);

    // Creates all actions
    CreateActions();

    // Creates ToolBar and adds actions
    CreateToolbar();
    
    // Creates MenuBar and DockWidgets
    CreateMenus();

    // Button click function
    DefineActionBehaviour();
}

#pragma region CreateAndConnectActions

void MainWindow::CreateActions()
{
    // Actions List
    _openAction = new QAction(QIcon("icons/Open.png"), "Open", this);
    _saveAction = new QAction(QIcon("icons/Save.png"), "Save", this);

    // Camera Actions
    _reorientCameraX = new QAction("X", this); //QIcon("icons/Open.png"), 
    _reorientCameraY = new QAction("Y", this); //QIcon("icons/Open.png"), 
    _reorientCameraZ = new QAction("Z", this); //QIcon("icons/Open.png"), 

    /*
    QIcon icon("icons/Open.png");

    qDebug() << icon.isNull();
    qDebug() << QDir::currentPath();
    */
}

void MainWindow::DefineActionBehaviour()
{
    connect(_openAction, &QAction::triggered,
        this, &MainWindow::OnOpenClicked);

    connect(_saveAction, &QAction::triggered,
        this, &MainWindow::OnSaveClicked);

    connect(_reorientCameraX, &QAction::triggered,
        this, &MainWindow::OnReoriCamXClicked);

    connect(_reorientCameraY, &QAction::triggered,
        this, &MainWindow::OnReoriCamYClicked);

    connect(_reorientCameraZ, &QAction::triggered,
        this, &MainWindow::OnReoriCamZClicked);
}

#pragma endregion

#pragma region Display
void MainWindow::CreateToolbar()
{
    // Toolbar
    QToolBar* toolbar = addToolBar("Main Toolbar");
    toolbar->setToolButtonStyle( Qt::ToolButtonIconOnly );
    toolbar->setIconSize(QSize(24, 24));

    toolbar->addAction(_openAction);
    toolbar->addAction(_saveAction);

    // Creates empty space
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Preferred);

    toolbar->addWidget(spacer);

    // Creates camera controls
    QWidget* cameraControl = new QWidget();

    QHBoxLayout* layout = new QHBoxLayout(cameraControl);
    layout->setContentsMargins(0, 0, 0, 0);

    QToolButton* xBtn = new QToolButton();
    QToolButton* yBtn = new QToolButton();
    QToolButton* zBtn = new QToolButton();

    xBtn->setDefaultAction(_reorientCameraX);
    yBtn->setDefaultAction(_reorientCameraY);
    zBtn->setDefaultAction(_reorientCameraZ);

    layout->addWidget(xBtn);
    layout->addWidget(yBtn);
    layout->addWidget(zBtn);

    toolbar->addWidget(cameraControl);
}

void MainWindow::CreateMenus()
{
    // Menu bar
    QMenu* fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(_openAction);
    fileMenu->addAction(_saveAction);

    QMenu* viewMenu = menuBar()->addMenu("View");
    
    if (!_toolDock) CreateToolDockWidget();
    viewMenu->addAction(_toolDock->toggleViewAction());

    if (!_statsWidget) CreateStatisticsWidget();
    //viewMenu->addAction(_statsWidget->toggleViewAction());
}

void MainWindow::CreateToolDockWidget()
{
    if (!_toolDock)
        _toolDock = new QDockWidget("Tools", this);

    _toolDock->setAllowedAreas(
        Qt::LeftDockWidgetArea |
        Qt::RightDockWidgetArea
    );

    //_toolDock->setAutoFillBackground(true);
    _toolDock->setStyleSheet("background-color: rgba(200, 200, 200,150);");

    //---------------------------------
    // Contents inside dock
    //---------------------------------

    QWidget* dockContent = new QWidget();

    QVBoxLayout* layout =
        new QVBoxLayout(dockContent);

    QPushButton* loadButton =
        new QPushButton("Load STL");

    QPushButton* generateButton =
        new QPushButton("Generate Support");

    layout->addWidget(loadButton);
    layout->addWidget(generateButton);

    dockContent->setLayout(layout);

    _toolDock->setWidget(dockContent);

    //---------------------------------
    // Add dock to main window
    //---------------------------------

    addDockWidget(Qt::LeftDockWidgetArea,
        _toolDock);
}

void MainWindow::CreateStatisticsWidget()
{
    //_statsDock = new QDockWidget("Statistics", this);
    

    //QWidget* content = new QWidget();
    _statsWidget = new QWidget();
    _statsWidget->setStyleSheet("background-color: rgba(200, 200, 200,150);");

    QHBoxLayout* layout = new QHBoxLayout(_statsWidget);

    _volumeLabel        = new QLabel("Volume: N/A");
    _surfaceAreaLabel   = new QLabel("Surface Area: N/A");
    _triNumLabel        = new QLabel("Triangles: N/A");
    _vertexNumLabel     = new QLabel("Vertices: N/A");
    

    layout->addWidget(_volumeLabel);
    layout->addWidget(_surfaceAreaLabel);
    layout->addWidget(_triNumLabel);
    layout->addWidget(_vertexNumLabel);


    _statsWidget->setLayout(layout);

    //_statsDock->setWidget(content);
    statusBar()->addPermanentWidget(_statsWidget, 1);
    //addWidget(Qt::BottomDockWidgetArea,_statsDock);
}

#pragma endregion

#pragma region BehaviourDefinition
void MainWindow::OnButtonClicked()
{
    QMessageBox::information(this,
        "Message",
        "Button was clicked!");
}

void MainWindow::OnOpenClicked()
{
    QString filePath =
        QFileDialog::getOpenFileName(
            this,
            "Open",
            "",
            "STL Files (*.stl)"
        );

    // User pressed Cancel
    if (filePath.isEmpty())
        return;

    //qDebug() << filePath;

    _AppControl->LoadSTLFile(filePath.toStdString());

    UpdateMeshStatistics();
}

void MainWindow::OnSaveClicked()
{
    QMessageBox::information(this,
        "Save",
        "Save menu clicked");
}

void MainWindow::OnReoriCamXClicked()
{
    _AppControl->ReoriCamX();
}

void MainWindow::OnReoriCamYClicked()
{
    _AppControl->ReoriCamY();
}

void MainWindow::OnReoriCamZClicked()
{
    _AppControl->ReoriCamZ();
}


#pragma endregion

void MainWindow::UpdateMeshStatistics()
{
    double volume = _AppControl->GetMeshVolume();
    double area = _AppControl->GetMeshSurfaceArea();
    double numTriangles = _AppControl->GetMeshNumTriangles();
    double numVertices = _AppControl->GetMeshNumVertices();

    _volumeLabel->setText(
        QString("Volume: %1 mm").arg(volume) + QChar(0x00B3));

    _surfaceAreaLabel->setText(
        QString("Surface Area: %1 mm").arg(area) + QChar(0x00B2));

    _triNumLabel->setText(
        QString("Triangles: %1").arg(numTriangles));

    _vertexNumLabel->setText(
        QString("Vertices: %1").arg(numVertices));
    
}