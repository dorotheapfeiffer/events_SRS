#ifndef VMM_CONFIG_HANDLER_H
#define VMM_CONFIG_HANDLER_H
#include "globparameter.h"
#include <unistd.h>

#include <QObject>
#include <fstream>

class MainWindow;

class VMM_config_handler : public QObject
{
    Q_OBJECT
public:
    explicit VMM_config_handler(MainWindow *top, QObject *parent = 0);
    ~VMM_config_handler();
    friend class Commandline;
    bool LoadAllVMMConf(const char* filename);
    bool WriteAllVMMConf(const char* filename);
    bool LoadSingleVMMConf(const char* filename, unsigned short daq, unsigned short fec, unsigned short hdmi, unsigned short hybrid, unsigned short vmm);
    bool WriteSingleVMMConf(const char* filename, unsigned short daq, unsigned short fec, unsigned short hdmi, unsigned short hybrid, unsigned short vmm);
    bool LoadSingleVMMConf(const char* filename);
    bool WriteSingleVMMConf(const char* filename);

private:
    char ExecPath[256];
    MainWindow *root1;
    bool GenericAllVMMConf(bool load, const char* filename);
    bool GenericSingleVMMConf(bool load, const char* filename, unsigned short daq, unsigned short fec, unsigned short hdmi, unsigned short hybrid, unsigned short vmm);
    bool LoadVMMConfig(std::string fname);
    bool WriteVMMConfig(std::string filename, unsigned short daq, unsigned short fec, unsigned short hdmi, unsigned short hybrid, unsigned short vmm);

signals:

public slots:
};

#endif // VMM_CONFIG_HANDLER_H

#ifndef _MAINWINDOW_HPP
#include "mainwindow.h"
#endif
