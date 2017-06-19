#include "daq.h"

DAQ::DAQ():
    RegNames ( new std::vector<const char*> (11) ),
    Reg ( new std::vector<unsigned short> (3) ),
    RegText ( new std::vector<std::string> (11) ),
    RegVals ( new std::vector<std::vector<const char*> > (3, std::vector<const char*>() ) ),
    fec_act (FECS_PER_DAQ),
    cchr ( new char ) //need for returning const char * in GetReg functions
{
    //fec_act[0] = 1;

    (*RegNames)[0] ="ignore16";                (*Reg)[0] = 0;   (*RegVals)[0]={"0", "1", "false", "true"};
    (*RegNames)[1] ="debug";                   (*Reg)[1] = 0;   (*RegVals)[1]={"0", "1", "false", "true"};
    (*RegNames)[2] ="run_count";               (*Reg)[2] = 20;  // lets allow 16 bit, so unsigned short is ok

    (*RegNames)[3] ="mapping_file";            (*RegText)[3] = "mini2_map.txt";
    (*RegNames)[4] ="output_filename";         (*RegText)[4] = "binary_dump.txt";
    (*RegNames)[5] ="config_filename";         (*RegText)[5] = "";
    (*RegNames)[6] ="vmm_id_list";             (*RegText)[6] = "";
    (*RegNames)[7] ="ip_list";                 (*RegText)[7] = "";
    (*RegNames)[8] ="config_version";          (*RegText)[8] = "";
    (*RegNames)[9] ="comment";                 (*RegText)[9] = "None";
    (*RegNames)[10] ="output_path";            (*RegText)[10] = "";
}


bool DAQ::SetFEC(unsigned short FEC, bool OnOff){
    if (FEC < FECS_PER_DAQ) {fec_act[FEC] = OnOff; return true;}
    else {return false;}
}

bool DAQ::GetFEC(unsigned short FEC){
    if (FEC < FECS_PER_DAQ) {return fec_act[FEC];}
    else {return false;}
}

bool DAQ::Set(unsigned short reg, unsigned short val){

    if (reg < (*Reg).size() ) { (*Reg)[reg] = val; std::cout << "Register " << reg << " set to " << val << " ." << std::endl; return true;}
    else {std::cout << "ERROR register " << reg << " does not exist." << std::endl;return false;}
}

bool DAQ::SetText(unsigned short reg, const char * text){
    std::stringstream str;
    str << text;
    if (reg < (*RegNames).size() &&  reg >= (*Reg).size() ) { (*RegText)[reg] = str.str(); std::cout << "Text register " << reg << " set to " << text << " ." << std::endl; return true;}
    else {std::cout << "ERROR register " << reg << " does not exist." << std::endl;return false;}
}

bool DAQ::SetReg(const char *reg, bool val){ //set a register, name and bool given
    const char *chr =  val ? "true" : "false";// convert bool to const char * to check if in allowed value list
    for (unsigned short i = 0; i < (*RegNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegNames)[i])){
            if (CheckAllowedVal(i, chr)){
                Set(i,val); return true;
            }
        }
    }
    return false;
}

bool DAQ::SetReg(int regnum, bool val){
    const char *chr =  val ? "1" : "0";// convert bool to const char * to check if in allowed value list
    if (CheckAllowedVal(regnum, chr)){
        Set(regnum,val); return true;
    }
    return false;
}

bool DAQ::SetReg(const char *reg, int val){
    std::stringstream str1;str1 << val;const char * chr = str1.str().c_str(); // convert int to const char * to check if in allowed value list
    for (unsigned short i = 0; i < (*RegNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegNames)[i])){
            if (CheckAllowedVal(i, chr) && i < (*Reg).size()){ //numbers only allowed for registers 0-2
                Set(i,val); return true;
            }
        }
    }
    return false;
}

bool DAQ::SetReg(int regnum, int val){
    std::stringstream str1;str1 << val;const char *chr = str1.str().c_str(); // convert int to const char * to check if in allowed value list
    if (CheckAllowedVal(regnum, chr) && abs(regnum) < (*Reg).size()){ //numbers only allowed for registers 0-2
        Set(regnum,val); return true;
    }
    return false;
}

bool DAQ::SetReg(const char *reg, const char *val){
    for (unsigned short i = 0; i < (*RegNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegNames)[i])){
            if (CheckAllowedVal(i, val)){
                if (i < (*Reg).size()){Set(i,FindVecEntry(i,val)); return true;} // numbers
                else if (i < (*RegNames).size() &&  i >= (*Reg).size() ) {SetText(i,val); return true;} //text
            }
        }
    }
    return false;
}

bool DAQ::SetReg(int regnum, const char *val){
    if (CheckAllowedVal(regnum, val)){
        if (abs(regnum) < (*Reg).size()){Set(regnum,FindVecEntry(regnum,val)); return true;} // numbers
        else if (abs(regnum) < (*RegNames).size() &&  abs(regnum) >= (*Reg).size() ) {SetText(regnum,val); return true;} //text
    }
    return false;
}

const char * DAQ::GetReg(const char *reg){
    for (unsigned short i = 0; i < (*RegNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegNames)[i])){
            const char *chr = GetReg((int)i);
            return chr;
        }
    }
    return "ERROR";
}

const char * DAQ::GetReg(int regnum){
    if (regnum == 0){// give the name of the value
        if ((*Reg)[0] == 0 || (*Reg)[0] == 2 ) return "false";
        else if ((*Reg)[0] == 1 || (*Reg)[0] == 3) return "true";
        else return "ERROR";
    }
    else if (abs(regnum) < (*Reg).size() && regnum != 4){ //need to convert number to const char *
        std::stringstream str1;str1 << (*Reg)[regnum];
        std::strcpy(cchr,str1.str().c_str());
        return cchr;
    }
    else if (abs(regnum) < (*RegNames).size() &&  abs(regnum) >= (*Reg).size() ) { //directly return text
        std::strcpy(cchr,(RegText->at(regnum)).c_str());
        return cchr;
    }
    else return "ERROR";
}

unsigned short DAQ::GetRegVal(int regnum){
    if (abs(regnum) < (*Reg).size() ){ //only reg 0-2
        return (*Reg)[regnum];
    }
    else return -1;
}

unsigned short DAQ::GetRegVal(const char *reg){
    for (unsigned short i = 0; i < (*RegNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegNames)[i])){
            return GetRegVal((int)i);
        }
    }
    return -1;
}

const char * DAQ::GetRegName(unsigned short regnum){
    if(regnum < (*RegNames).size()){return (*RegNames)[regnum];}
    return "failure";
}

unsigned short DAQ::GetRegNumber(const char *reg){
    unsigned short regnum = -1;
    for (unsigned short i = 0; i < (*RegNames).size(); i++)
    {
        if (reg == (*RegNames)[i]) {regnum = i;}
    }
    return regnum;
}

unsigned short DAQ::GetRegSize(){
    return RegNames->size();
}

bool DAQ::CheckAllowedVal(unsigned short reg, const char *val){
    bool found = false;
    std::stringstream strValue;
    strValue << val;
    int intValue;
    strValue >> intValue;
    if (reg == 0 || reg == 1){ //ignore16 or debug
        if (ConstCharStar_comp(val,"0") || ConstCharStar_comp(val,"1") || ConstCharStar_comp(val,"true") || ConstCharStar_comp(val,"false") ) found = true;
    }
    if (reg == 2 ){ // 16 bit values allowed for run_count
        if (intValue < 65536)found = true;
    }
    else if (reg < (*RegNames).size() && reg != 0 && reg != 1 && reg != 2 ){ //others are text, so can be anything
        found = true;
    }
    return found;
}

unsigned short DAQ::FindVecEntry(unsigned short regnum, const char *val){
    unsigned short pos = -1;

    if (regnum == 0 || regnum == 1) {// need to compare the strings at the const char * addresses
        if (ConstCharStar_comp(val,"0")) pos = 0;
        else if (ConstCharStar_comp(val,"1")) pos = 1;
        else if (ConstCharStar_comp(val,"false")) pos = 2;
        else if (ConstCharStar_comp(val,"true")) pos = 3;
    }
    else {
        std::stringstream strValue;
        strValue << val;
        unsigned short intValue;
        strValue >> intValue;
        pos = intValue;
    }
    return pos;
}

bool DAQ::ConstCharStar_comp(const char *ccs1, const char *ccs2){
    std::stringstream str1;
    str1 << ccs1;
    std::stringstream str2;
    str2 << ccs2;
    if (str1.str() == str2.str()) {return true;}
    else {return false;}
}

DAQ::~DAQ(){
    if( RegNames != NULL ){
        delete RegNames;}
    RegNames = NULL;

    if( Reg != NULL ){
        delete Reg;}
    Reg = NULL;

    if( RegText != NULL ){
        delete RegText;}
    RegText = NULL;

    if( RegVals != NULL ){
        delete RegVals;}
    RegVals = NULL;

    if( cchr != NULL ){
        delete cchr;}
    cchr = NULL;
}
