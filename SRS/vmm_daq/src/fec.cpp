#include "fec.h"

FEC::FEC():
    hdmi_act (HDMIS_PER_FEC),
    RegNames ( new std::vector<const char*> (14) ),
    Reg ( new std::vector<unsigned long> (14) ),
    cchr ( new char ) //need for returning const char * in GetReg functions
{
    //hdmi_act[0] = 1;


    (*RegNames)[0] ="tp_delay";                (*Reg)[0] = 81;      //32 bit //max 50000 by gui?
    (*RegNames)[1] ="trigger_period";          (*Reg)[1] = 262142;  //32 bit //max 7FFFFFFF = 31 bit?
    (*RegNames)[2] ="acq_sync";                (*Reg)[2] = 100;     //32 bit
    (*RegNames)[3] ="acq_window";              (*Reg)[3] = 4096;    //32 bit
    (*RegNames)[4] ="run_mode";                (*Reg)[4] = 0;       //{"0", "1", "pulser" (=0), "external" (=1)};
    (*RegNames)[5] ="bcid_reset";              (*Reg)[5] = 0;       //max 65535 (16 bit)
    (*RegNames)[6] ="fec_port";                (*Reg)[6] = 6007;    //32 bit
    (*RegNames)[7] ="daq_port";                (*Reg)[7] = 6006;    //32 bit
    (*RegNames)[8] ="vmmasic_port";            (*Reg)[8] = 6603;    //32 bit
    (*RegNames)[9] ="vmmapp_port";             (*Reg)[9] = 6600;    //32 bit
    (*RegNames)[10]="s6_port";                 (*Reg)[10] = 6602;   //32 bit
    (*RegNames)[11]="evbld_mode";              (*Reg)[11] = 0;   //{"Frame_Cnt", "Global_Frame_Cnt", "Timestamp+Frame_Cnt" }
    (*RegNames)[12]="evbld_info";              (*Reg)[12] = 0;   //{"HINFO+Datalength", "Trigger_Cnt+Datalength", "Trigger_Cnt", "Trigger_Timestamp+Datalength", "Trigger_Timestamp", "Trigger_Cnt+Trigger_Timestamp"}
    (*RegNames)[13]="timeStampHighRes";        (*Reg)[13] = 0;   //{"0", "1", "false", "true"};

}

bool FEC::SetHDMI(unsigned short hdmi, bool OnOff){
    if (hdmi < HDMIS_PER_FEC) {hdmi_act[hdmi] = OnOff; return true;}
    else {return false;}
}

bool FEC::GetHDMI(unsigned short hdmi){
    if (hdmi < HDMIS_PER_FEC) {return hdmi_act[hdmi];}
    else {return false;}
}

bool FEC::Set(unsigned short reg, unsigned long val){

    if (reg < Reg->size() ) { (*Reg)[reg] = val; std::cout << "Register " << reg << " set to " << val << " ." << std::endl; return true;}
    else {std::cout << "ERROR register " << reg << " does not exist." << std::endl;return false;}
}

bool FEC::CheckAllowedVal(unsigned short reg, const char *val){
    bool found = false;

    std::stringstream strValue;
    strValue << val;
    unsigned long intValue;
    strValue >> intValue;
    std::cout << "intValue: " << intValue << std::endl;
    if (reg == 4){ //run_mode
        if (ConstCharStar_comp(val,"0") || ConstCharStar_comp(val,"1") || ConstCharStar_comp(val,"pulser") || ConstCharStar_comp(val,"external") ) found = true;
    }
    if (reg == 5 ){ // 16 bit values allowed for bcid_reset
        if (intValue < 65536)found = true;
    }
    else if (reg < Reg->size() && reg != 4 && reg != 5  && reg != 11 && reg != 12 && reg != 13){ //others are 32 bit
        if (intValue < 4294967296)found = true;
    }
    if (reg == 11){ //evbld_mode
        if (ConstCharStar_comp(val,"Frame_Cnt") || ConstCharStar_comp(val,"Global_Frame_Cnt") || ConstCharStar_comp(val,"Timestamp+Frame_Cnt")) found = true;
    }
    if (reg == 12){ //evbld_info
        if (ConstCharStar_comp(val,"HINFO+Datalength") || ConstCharStar_comp(val,"Trigger_Cnt+Datalength") || ConstCharStar_comp(val,"Trigger_Cnt") || ConstCharStar_comp(val,"Trigger_Timestamp+Datalength") || ConstCharStar_comp(val,"Trigger_Timestamp") || ConstCharStar_comp(val,"Trigger_Cnt+Trigger_Timestamp") ) found = true;
    }
    if (reg == 13){ //timeStampHighRes
        if (ConstCharStar_comp(val,"0") || ConstCharStar_comp(val,"1") || ConstCharStar_comp(val,"false") || ConstCharStar_comp(val,"true") ) found = true;
    }
    return found;
}

bool FEC::SetReg(const char *reg, bool val){ //set a register, name and bool given
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

bool FEC::SetReg(int regnum, bool val){
    const char *chr =  val ? "1" : "0";// convert bool to const char * to check if in allowed value list
    if (CheckAllowedVal(regnum, chr)){
        Set(regnum,val); return true;
    }
    return false;
}

bool FEC::SetReg(const char *reg, unsigned long val){
    std::stringstream str1;str1 << val;const char * chr = str1.str().c_str(); // convert int to const char * to check if in allowed value list
    for (unsigned short i = 0; i < (*RegNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegNames)[i])){
            if (CheckAllowedVal(i, chr)){
                Set(i,val); return true;
            }
        }
    }
    return false;
}

bool FEC::SetReg(int regnum, unsigned long val){
    std::stringstream str1;str1 << val;const char *chr = str1.str().c_str(); // convert int to const char * to check if in allowed value list
    if (CheckAllowedVal(regnum, chr)){
        Set(regnum,val); return true;
    }
    return false;
}

bool FEC::SetReg(const char *reg, const char *val){
    for (unsigned short i = 0; i < (*RegNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegNames)[i])){
            if (CheckAllowedVal(i, val)){
                Set(i,FindVecEntry(i,val)); return true;
            }
        }
    }
    return false;
}

bool FEC::SetReg(int regnum, const char *val){
    if (CheckAllowedVal(regnum, val)){
        Set(regnum,FindVecEntry(regnum,val)); return true;
    }
    return false;
}

unsigned long FEC::FindVecEntry(unsigned short regnum, const char *val){
    unsigned long pos = 4294967296;

    if (regnum == 4) {// need to compare the strings at the const char * addresses
        if (ConstCharStar_comp(val,"0")) pos = 0;
        else if (ConstCharStar_comp(val,"1")) pos = 1;
        else if (ConstCharStar_comp(val,"pulser")) pos = 2;
        else if (ConstCharStar_comp(val,"external")) pos = 3;
    }
    else if (regnum == 11) {// need to compare the strings at the const char * addresses
        if (ConstCharStar_comp(val,"Frame_Cnt")) pos = 0;
        else if (ConstCharStar_comp(val,"Global_Frame_Cnt")) pos = 1;
        else if (ConstCharStar_comp(val,"Timestamp+Frame_Cnt")) pos = 2;
    }
    else if (regnum == 12) {// need to compare the strings at the const char * addresses
        if (ConstCharStar_comp(val,"HINFO+Datalength")) pos = 0;
        else if (ConstCharStar_comp(val,"Trigger_Cnt+Datalength")) pos = 1;
        else if (ConstCharStar_comp(val,"Trigger_Cnt")) pos = 2;
        else if (ConstCharStar_comp(val,"Trigger_Timestamp+Datalength")) pos = 3;
        else if (ConstCharStar_comp(val,"Trigger_Timestamp")) pos = 4;
        else if (ConstCharStar_comp(val,"Trigger_Cnt+Trigger_Timestamp")) pos = 5;
    }
    else if (regnum == 13) {// need to compare the strings at the const char * addresses
        if (ConstCharStar_comp(val,"0")) pos = 0;
        else if (ConstCharStar_comp(val,"1")) pos = 1;
        else if (ConstCharStar_comp(val,"false")) pos = 2;
        else if (ConstCharStar_comp(val,"true")) pos = 3;
    }
    else {
        std::stringstream strValue;
        strValue << val;
        unsigned long intValue;
        strValue >> intValue;
        pos = intValue;
    }
    return pos;
}

const char * FEC::GetReg(const char *reg){
    for (unsigned short i = 0; i < (*RegNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegNames)[i])){
            const char *chr = GetReg((int)i);
            return chr;
        }
    }
    return "ERROR";
}

const char * FEC::GetReg(int regnum){
    if (regnum == 4){// give the name of the value
        if ((*Reg)[4] == 0 || (*Reg)[4] == 2 ) return "pulser";
        else if ((*Reg)[4] == 1 || (*Reg)[4] == 3) return "external";
        else return "ERROR";
    }
    else if (abs(regnum) < (*Reg).size() && regnum != 4 && regnum != 11 && regnum != 12 && regnum != 13){ //need to convert number to const char *
        std::stringstream str1;str1 << (*Reg)[regnum];
        std::strcpy(cchr,str1.str().c_str());
        return cchr;
    }
    else if (regnum == 11){
        if ((*Reg)[11] == 0) return "Frame_Cnt";
        else if ((*Reg)[11] == 1) return "Global_Frame_Cnt";
        else if ((*Reg)[11] == 2) return "Timestamp+Frame_Cnt";
        else return "ERROR";
    }
    else if (regnum == 12){
        if ((*Reg)[12] == 0) return "HINFO+Datalength";
        else if ((*Reg)[12] == 1) return "Trigger_Cnt+Datalength";
        else if ((*Reg)[12] == 2) return "Trigger_Cnt";
        else if ((*Reg)[12] == 3) return "Trigger_Timestamp+Datalength";
        else if ((*Reg)[12] == 4) return "Trigger_Timestamp";
        else if ((*Reg)[12] == 5) return "Trigger_Cnt+Trigger_Timestamp";
        else return "ERROR";
    }
    else if (regnum == 13){// give the name of the value
        if ((*Reg)[13] == 0 || (*Reg)[13] == 2 ) return "false";
        else if ((*Reg)[13] == 1 || (*Reg)[13] == 3) return "true";
        else return "ERROR";
    }
    else return "ERROR";
}


unsigned long FEC::GetRegVal(int regnum){
    if (abs(regnum) < (*Reg).size() ){
        return (*Reg)[regnum];
    }
    else return -1;
}

unsigned long FEC::GetRegVal(const char *reg){
    for (unsigned short i = 0; i < (*RegNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegNames)[i])){
            return GetRegVal((int)i);
        }
    }
    return -1;
}

const char * FEC::GetRegName(unsigned short regnum){
    if(regnum < (*RegNames).size()){return (*RegNames)[regnum];}
    return "failure";
}

unsigned short FEC::GetRegNumber(const char *reg){
    unsigned short regnum = -1;
    for (unsigned short i = 0; i < (*RegNames).size(); i++)
    {
        if (reg == (*RegNames)[i]) {regnum = i;}
    }
    return regnum;
}

unsigned short FEC::GetRegSize(){
    return Reg->size();
}

bool FEC::ConstCharStar_comp(const char *ccs1, const char *ccs2){
    std::stringstream str1;
    str1 << ccs1;
    std::stringstream str2;
    str2 << ccs2;
    if (str1.str() == str2.str()) {return true;}
    else {return false;}
}

FEC::~FEC()
{
    if( RegNames != NULL ){
        delete RegNames;}
    RegNames = NULL;

    if( Reg != NULL ){
        delete Reg;}
    Reg = NULL;

    if( cchr != NULL ){
        delete cchr;}
    cchr = NULL;
}
