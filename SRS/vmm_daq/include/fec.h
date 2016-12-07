#ifndef FEC_H
#define FEC_H


#include "globparameter.h"
#include "hdmi.h"

class FEC: public QObject
{
    Q_OBJECT
public:
    FEC();
    ~FEC();
    friend class Commandline;
    HDMI hdmi[HDMIS_PER_FEC];

    bool SetHDMI(unsigned short hdmi, bool OnOff);
    bool GetHDMI(unsigned short hdmi);

    // global registers
    bool SetReg(const char *reg, bool val);
    bool SetReg(int regnum, bool val);
    bool SetReg(const char *reg, unsigned long val);
    bool SetReg(int regnum, unsigned long val);
    bool SetReg(const char *reg, const char *val);
    bool SetReg(int regnum, const char *val);

    const char *GetReg(const char *reg);
    const char *GetReg(int regnum);
    unsigned long GetRegVal(int regnum);
    unsigned long GetRegVal(const char *reg);

    const char *GetRegName(unsigned short regnum);
    unsigned short GetRegNumber(const char *reg);
    unsigned short GetRegSize();


private:
    bool Set(unsigned short reg, unsigned long val);
    bool CheckAllowedVal(unsigned short reg, const char *val);
    // helper functions
    unsigned long FindVecEntry(unsigned short regval, const char *val);
    bool ConstCharStar_comp(const char *ccs1, const char *ccs2);

    std::vector<bool> hdmi_act;//binary to store which hdmis are activated

    std::vector<const char*> *RegNames;
    std::vector<unsigned long> *Reg;
    char *cchr;
};
#endif // FEC_H
