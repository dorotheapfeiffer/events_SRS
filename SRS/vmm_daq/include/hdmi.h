#ifndef HDMI_H
#define HDMI_H

#include "globparameter.h"
#include "hybrid.h"

class HDMI: public QObject
{
    Q_OBJECT
public:
    HDMI();
    ~HDMI();
    friend class Commandline;
    Hybrid hybrid[HYBRIDS_PER_HDMI];

    bool SetHybrid(unsigned short hybrid, bool OnOff);
    bool GetHybrid(unsigned short hybrid);

private:
    std::vector<bool> hybrid_act;//binary to store which hybrids are activated
};

#endif // HDMI_H
