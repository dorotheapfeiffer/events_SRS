#ifndef FEC_CONFIG_HANDLER_H
#define FEC_CONFIG_HANDLER_H

#include <unistd.h>

#include <QObject>
#include <fstream>

class MainWindow;

class FEC_config_handler : public QObject
{
    Q_OBJECT
public:
    explicit FEC_config_handler(MainWindow *top, QObject *parent = 0);
    ~FEC_config_handler();
    bool LoadAllFECConf(const char* filename);
    bool WriteAllFECConf(const char* filename);
    bool LoadSingleFECConf(const char* filename, unsigned short daq, unsigned short fec);
    bool WriteSingleFECConf(const char* filename, unsigned short daq, unsigned short fec);
    bool LoadSingleFECConf(const char* filename);
    bool WriteSingleFECConf(const char* filename);
private:
    char ExecPath[256];
    MainWindow *root1;
    bool GenericAllFECConf(bool load, const char* filename);
    bool GenericSingleFECConf(bool load, const char* filename, unsigned short daq, unsigned short fec);
    bool LoadFECConfig(std::string fname);
    bool WriteFECConfig(std::string filename, unsigned short daq, unsigned short fec);
};

#endif // FEC_CONFIG_HANDLER_H

#ifndef _MAINWINDOW_HPP
#include "mainwindow.h"
#endif
