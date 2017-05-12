#include "guicaller.h"
#include "mainwindow.h"

GUIcaller::GUIcaller()
{

}

GUIcaller::~GUIcaller()
{

}

void GUIcaller::startGUI()
{
    MainWindow w;
    w.setWindowTitle("VMM2 - SRS DCS");

    w.show();
}
