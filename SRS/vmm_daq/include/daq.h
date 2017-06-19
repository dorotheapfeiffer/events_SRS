#ifndef DAQ_H
#define DAQ_H


#include <QObject>
#include <iostream>
#include <cstring>
#include <iterator>
#include <vector>
#include <sstream>
#include <iomanip>

#include "globparameter.h"
#include "fec.h"

class DAQ: public QObject
{
    Q_OBJECT
public:
    DAQ();
    ~DAQ();
    friend class Commandline;
    FEC fec[FECS_PER_DAQ];

    bool SetFEC(unsigned short FEC, bool OnOff);
    bool GetFEC(unsigned short FEC);

    // global registers
    bool SetReg(const char *reg, bool val);
    bool SetReg(int regnum, bool val);
    bool SetReg(const char *reg, int val);
    bool SetReg(int regnum, int val);
    bool SetReg(const char *reg, const char *val);
    bool SetReg(int regnum, const char *val);

    const char *GetReg(const char *reg);
    const char *GetReg(int regnum);
    unsigned short GetRegVal(int regnum);
    unsigned short GetRegVal(const char *reg);

    const char *GetRegName(unsigned short regnum);
    unsigned short GetRegNumber(const char *reg);
    unsigned short GetRegSize();

private:
    bool SetText(unsigned short reg, const char * text);
    bool Set(unsigned short reg, unsigned short val);
    bool CheckAllowedVal(unsigned short reg, const char *val);
    // helper functions
    unsigned short FindVecEntry(unsigned short regval, const char *val);
    bool ConstCharStar_comp(const char *ccs1, const char *ccs2);
    std::vector<const char*> *RegNames;
    std::vector<unsigned short> *Reg;
    std::vector<std::string> *RegText;
    std::vector<std::vector<const char*> > *RegVals;

    std::vector<bool> fec_act;//binary to store which fecs are activated
    char *cchr;
};

#endif // DAQ_H
