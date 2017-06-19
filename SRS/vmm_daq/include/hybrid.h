#ifndef HYBRID_H
#define HYBRID_H

#include "globparameter.h"
#include "vmm.h"

class Hybrid: public QObject
{
    Q_OBJECT
public:
    Hybrid();
    ~Hybrid();
    friend class Commandline;
    VMM vmm[VMMS_PER_HYBRID];

    bool SetVMM(unsigned short vmm, bool OnOff);
    void SetART(bool OnOff);
    bool GetVMM(unsigned short vmm);
    bool GetART();

    void SetPosX(bool);
    bool SetPosNo(unsigned short);
    bool GetPosX();
    unsigned short GetPosNo();

private:
    std::vector<bool> vmm_act;//binary to store which vmms are activated
    bool ART; // binary to store if ART is active

    bool PosX; // hybrid is reading x (true) or y (false)
    unsigned short PosNo; // hybrid is number ? on its axis
};

#endif // HYBRID_H
