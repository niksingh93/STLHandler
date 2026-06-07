// Learning_Qt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QSurfaceFormat::setDefaultFormat(
        QVTKOpenGLNativeWidget::defaultFormat()
    );

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}