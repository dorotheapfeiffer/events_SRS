#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QObject>
#include <iostream>
#include "vmm.h"

class MainWindow;

class Commandline: public QObject
{
    Q_OBJECT
    friend class MainWindow;
    friend class VMM;
    friend class VMM_config_handler;

    public:
        Commandline(MainWindow *top, int value, QObject* parent = 0);
        ~Commandline();
        int StartCommandline();
        int EndCommandline();

    private:
    MainWindow *root1;
    int index;
    bool running;


};

#endif // COMMANDLINE_H

#ifndef _MAINWINDOW_HPP
#include "mainwindow.h"
#endif
