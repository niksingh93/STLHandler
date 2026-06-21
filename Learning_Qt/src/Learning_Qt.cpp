// Learning_Qt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <QSurfaceFormat>
#include <QVTKOpenGLNativeWidget.h>
#include <QApplication>
#include <QIcon>
#include "MainWindow.h"


int main(int argc, char* argv[])
{
    QSurfaceFormat::setDefaultFormat(
        QVTKOpenGLNativeWidget::defaultFormat()
    );

    QApplication app(argc, argv);

    // Set the application metadata name
    QCoreApplication::setApplicationName("STLDiagnostics");
    QCoreApplication::setOrganizationName("Lala's Projects");

    app.setWindowIcon(QIcon("icons/STLDiagnostics_Logo_4.png"));

    MainWindow window;
    window.show();

    return app.exec();
}