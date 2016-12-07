#ifndef VMM_H
#define VMM_H

#include <QObject>
#include <iostream>
#include <cstring>
#include <iterator>
#include <vector>
#include <sstream>
#include <iomanip>

#include "globparameter.h"


class VMM: public QObject
{
    Q_OBJECT
public:
    VMM();
    ~VMM();
    friend class Commandline;

    bool CheckAllowedGlobVal(unsigned short reg, const char *val);
    bool CheckAllowedChanVal(unsigned short reg, const char *val);

    // global registers
    bool SetReg(const char *reg, bool val);
    bool SetReg(int regnum, bool val);
    bool SetReg(const char *reg, int val);
    bool SetReg(int regnum, int val);
    bool SetReg(const char *reg, double val);
    bool SetReg(int regnum, double val);
    bool SetReg(const char *reg, const char *val);
    bool SetReg(int regnum, const char *val);
    // channel registers
    bool SetReg(const char *reg, unsigned short chan, bool val);
    bool SetReg(int regnum, unsigned short chan, bool val);
    bool SetReg(const char *reg, unsigned short chan, int val);
    bool SetReg(int regnum, unsigned short chan, int val);
    bool SetReg(const char *reg, unsigned short chan, const char *val);
    bool SetReg(int regnum, unsigned short chan, const char *val);

    const char *GetReg(const char *reg);
    const char *GetReg(int regnum);
    unsigned short GetRegVal(int regnum);
    unsigned short GetRegVal(const char *reg);
    const char *GetReg(const char *reg, unsigned short chan);
    const char *GetReg(int regnum, unsigned short chan);
    unsigned short GetRegVal(int regnum, unsigned short chan);
    unsigned short GetRegVal(const char *reg, unsigned short chan);


    const char *GetRegGlobName(unsigned short regnum);
    const char *GetRegChanName(unsigned short regnum);
    unsigned short GetRegNumber(const char *reg);
    unsigned short GetRegGlobSize();
    unsigned short GetRegChanSize();



private:
    bool SetGlobReg(unsigned short reg, unsigned short val);
    bool SetChanReg(unsigned short reg, unsigned short chan, unsigned short val);
    // helper functions
    unsigned short FindVecEntry(std::vector<const char*> reg, const char *val);
    bool ConstCharStar_comp(const char *ccs1, const char *ccs2);

    std::vector<const char*> *RegGlobNames;
    std::vector<const char*> *RegChanNames;
    std::vector<unsigned short> *RegGlob;
    std::vector<std::vector<unsigned short> > *RegChan;
    std::vector<std::vector<const char*> > *RegGlobVals;
    std::vector<std::vector<const char*> > *RegChanVals;
    std::vector<const char*> *IsIntOrBool;
    std::vector<std::string> st;
};

#endif // VMM_H
