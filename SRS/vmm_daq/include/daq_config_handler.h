#ifndef DAQ_CONFIG_HANDLER_H
#define DAQ_CONFIG_HANDLER_H
#include "globparameter.h"
#include <unistd.h>

#include <QObject>
#include <fstream>
#include <iostream>


class MainWindow;

class DAQ_config_handler : public QObject
{
    Q_OBJECT
public:
    explicit DAQ_config_handler(MainWindow *top, QObject *parent = 0);
    ~DAQ_config_handler();
    friend class Commandline;
    bool LoadDAQConf(const char* filename); // only one DAQ implemented
    bool WriteDAQConf(const char* filename);
private:
    char ExecPath[256];
    MainWindow *root1;
    bool LoadDAQConfig(std::string fname);
    bool WriteDAQConfig(std::string fname);
};

#endif // DAQ_CONFIG_HANDLER_H

#ifndef _MAINWINDOW_HPP
#include "mainwindow.h"
#endif
