#include "vmm.h"

VMM::VMM():
    RegGlobNames ( new std::vector<const char*> (34) ),
    RegChanNames ( new std::vector<const char*> (10) ),
    RegGlob ( new std::vector<unsigned short> (34) ),
    RegChan ( new std::vector<std::vector<unsigned short> > (10, std::vector<unsigned short>(VMM_CHANNELS) ) ),
    RegGlobVals ( new std::vector<std::vector<const char*> > (34, std::vector<const char*>() ) ),
    RegChanVals ( new std::vector<std::vector<const char*> > (10, std::vector<const char*>() ) ),
    IsIntOrBool ( new std::vector<const char*> () ),
    st (1024)      
{
    // generate const char * vector for allowed values of registers that hold a number e.g. threshold 0 to 1023
    std::vector<unsigned short> s(1024);
    std::iota(s.begin(), s.end(), 0);

    for (unsigned short i = 0; i< s.size(); i++)
    {
        std::stringstream ss;
        ss <<s[i];
        st[i] = ss.str() ;
    }

    // set the size of the registers that hold values and fill them
    (*RegGlobVals)[31].resize(1024);
    (*RegGlobVals)[32].resize(1024);
    for (unsigned short i = 0; i< 1024; i++)
    {
        (*RegGlobVals)[31][i]=st[i].c_str();
        (*RegGlobVals)[32][i]=st[i].c_str();
    }
    (*RegChanVals)[7].resize(32);
    for (unsigned short i = 0; i< 32; i++)
    {
        (*RegChanVals)[7][i]=st[i].c_str();
    }
    (*RegChanVals)[5].resize(16);
    (*RegChanVals)[8].resize(16);
    for (unsigned short i = 0; i< 16; i++)
    {
        (*RegChanVals)[5][i]=st[i].c_str();
        (*RegChanVals)[8][i]=st[i].c_str();
    }
    (*RegChanVals)[9].resize(8);
    for (unsigned short i = 0; i< 8; i++)
    {
        (*RegChanVals)[9][i]=st[i].c_str();
    }
    //set defaults, definde names and allowed values

    //global
    (*RegGlobNames)[0] ="glob_polarity";           (*RegGlob)[0] = 0;   (*RegGlobVals)[0]={"wires", "strips"};
    (*RegGlobNames)[1] ="glob_leakage_current";    (*RegGlob)[1] = 1;   (*RegGlobVals)[1]={"0", "1", "true", "false"};
    (*RegGlobNames)[2] ="analog_tristates";        (*RegGlob)[2] = 0;   (*RegGlobVals)[2]={"0", "1", "true", "false"};
    (*RegGlobNames)[3] ="double_leakage";          (*RegGlob)[3] = 0;   (*RegGlobVals)[3]={"0", "1", "true", "false"};
    (*RegGlobNames)[4] ="gain";                    (*RegGlob)[4] = 2;   (*RegGlobVals)[4]={"0.5", "1.0", "3.0", "4.5", "6.0", "9.0", "12.0", "16.0"};//mV/fC
    (*RegGlobNames)[5] ="peak_time";               (*RegGlob)[5] = 0;   (*RegGlobVals)[5]={"200", "100", "50", "25"}; //ns
    (*RegGlobNames)[6] ="neighbor_trigger";        (*RegGlob)[6] = 0;   (*RegGlobVals)[6]={"0", "1", "true", "false"};
    (*RegGlobNames)[7] ="tac_slope";               (*RegGlob)[7] = 0;   (*RegGlobVals)[7]={"125", "250", "500", "1000"}; //ns
    (*RegGlobNames)[8] ="disable_at_peak";         (*RegGlob)[8] = 0;   (*RegGlobVals)[8]={"0", "1", "true", "false"};
    (*RegGlobNames)[9] ="art";                     (*RegGlob)[9] = 1;   (*RegGlobVals)[9]={"threshold", "peak"};
    (*RegGlobNames)[10]="art_mode";                (*RegGlob)[10] = 0;  (*RegGlobVals)[10]={"0", "1", "true", "false"};
    (*RegGlobNames)[11]="dual_clock_art";          (*RegGlob)[11] = 0;  (*RegGlobVals)[11]={"0", "1", "true", "false"};
    (*RegGlobNames)[12]="out_buffer_mo";           (*RegGlob)[12] = 0;  (*RegGlobVals)[12]={"0", "1", "true", "false"};
    (*RegGlobNames)[13]="out_buffer_pdo";          (*RegGlob)[13] = 0;  (*RegGlobVals)[13]={"0", "1", "true", "false"};
    (*RegGlobNames)[14]="out_buffer_tdo";          (*RegGlob)[14] = 0;  (*RegGlobVals)[14]={"0", "1", "true", "false"};
    (*RegGlobNames)[15]="channel_monitor";         (*RegGlob)[15] = 0;  (*RegGlobVals)[15]={"0", "1", "true", "false"};
    (*RegGlobNames)[16]="monitoring_control";      (*RegGlob)[16] = 1;  (*RegGlobVals)[16]={"0", "1", "true", "false"};
    (*RegGlobNames)[17]="monitor_pdo_out";         (*RegGlob)[17] = 0;  (*RegGlobVals)[17]={"0", "1", "true", "false"};
    (*RegGlobNames)[18]="adcs";                    (*RegGlob)[18] = 1;  (*RegGlobVals)[18]={"0", "1", "true", "false"};
    (*RegGlobNames)[19]="sub_hysteresis";          (*RegGlob)[19] = 0;  (*RegGlobVals)[19]={"0", "1", "true", "false"};
    (*RegGlobNames)[20]="direct_time";             (*RegGlob)[20] = 0;  (*RegGlobVals)[20]={"0", "1", "true", "false"};
    (*RegGlobNames)[21]="direct_time_mode";        (*RegGlob)[21] = 1;  (*RegGlobVals)[21]={"TtP", "ToT", "PtP", "PtT"};
    (*RegGlobNames)[22]="direct_time_mode0";       (*RegGlob)[22] = 0;  (*RegGlobVals)[22]={"0", "1", "true", "false"};
    (*RegGlobNames)[23]="direct_time_mode1";       (*RegGlob)[23] = 1;  (*RegGlobVals)[23]={"0", "1", "true", "false"};
    (*RegGlobNames)[24]="conv_mode_8bit";          (*RegGlob)[24] = 1;  (*RegGlobVals)[24]={"0", "1", "true", "false"};
    (*RegGlobNames)[25]="enable_6bit";             (*RegGlob)[25] = 0;  (*RegGlobVals)[25]={"0", "1", "true", "false"};
    (*RegGlobNames)[26]="adc_10bit";               (*RegGlob)[26] = 0;  (*RegGlobVals)[26]={"200ns", "+60ns"};
    (*RegGlobNames)[27]="adc_8bit";                (*RegGlob)[27] = 0;  (*RegGlobVals)[27]={"100ns", "+60ns"};
    (*RegGlobNames)[28]="adc_6bit";                (*RegGlob)[28] = 0;  (*RegGlobVals)[28]={"low", "middle", "up"};
    (*RegGlobNames)[29]="dual_clock_data";         (*RegGlob)[29] = 0;  (*RegGlobVals)[29]={"0", "1", "true", "false"};
    (*RegGlobNames)[30]="dual_clock_6bit";         (*RegGlob)[30] = 0;  (*RegGlobVals)[30]={"0", "1", "true", "false"};
    (*RegGlobNames)[31]="threshold_dac";           (*RegGlob)[31] = 200;    //(*RegGlobVals)[31] filled before
    (*RegGlobNames)[32]="test_pulse_dac";          (*RegGlob)[32] = 300;    //(*RegGlobVals)[32] filled before
    (*RegGlobNames)[33]="UseMapping";              (*RegGlob)[33] = 0;  (*RegGlobVals)[33]={"0", "1", "true", "false"};

   //channels
   for (unsigned short i = 0; i < VMM_CHANNELS; i++)
   {
       (*RegChan)[0][i] = 0;
       (*RegChan)[1][i] = 0;
       (*RegChan)[2][i] = 0;
       (*RegChan)[3][i] = 0;
       (*RegChan)[4][i] = 0;
       (*RegChan)[5][i] = 0;
       (*RegChan)[6][i] = 0;
       (*RegChan)[7][i] = 0;
       (*RegChan)[8][i] = 0;
       (*RegChan)[9][i] = 0;
   }
   (*RegChanNames)[0]="polarity";       (*RegChanVals)[0]={"wires", "strips"};
   (*RegChanNames)[1]="capacitance";    (*RegChanVals)[1]={"0", "1", "true", "false"};
   (*RegChanNames)[2]="leakage_current";(*RegChanVals)[2]={"0", "1", "true", "false"};
   (*RegChanNames)[3]="test_pulse";     (*RegChanVals)[3]={"0", "1", "true", "false"};
   (*RegChanNames)[4]="masked";         (*RegChanVals)[4]={"0", "1", "true", "false"};
   (*RegChanNames)[5]="thl_adjust";     //(*RegChanVals)[5] filled before
   (*RegChanNames)[6]="monitor";        (*RegChanVals)[6]={"0", "1", "true", "false"};
   (*RegChanNames)[7]="s10bitADC";      //(*RegChanVals)[7] filled before
   (*RegChanNames)[8]="s8bitADC";       //(*RegChanVals)[8] filled before
   (*RegChanNames)[9]="s6bitADC";       //(*RegChanVals)[9] filled before

   *IsIntOrBool = (*RegGlobVals)[32];
   IsIntOrBool->insert(IsIntOrBool->end(), (*RegGlobVals)[1].begin(), (*RegGlobVals)[1].end());
   IsIntOrBool->insert(IsIntOrBool->end(), (*RegGlobVals)[5].begin(), (*RegGlobVals)[5].end());
   IsIntOrBool->insert(IsIntOrBool->end(), (*RegGlobVals)[7].begin(), (*RegGlobVals)[7].end());

}

bool VMM::SetGlobReg(unsigned short reg, unsigned short val){

    if (reg < RegGlob->size() ) { (*RegGlob)[reg] = val;
        //std::cout << "Global register " << reg << " set to " << val << " ." << std::endl;
        return true;
    }
    else {
        std::cout << "ERROR register " << reg << " does not exist." << std::endl;
        return false;
    }
}

bool VMM::SetChanReg(unsigned short reg, unsigned short chan, unsigned short val){

    if (reg < 10) {
        if (chan < VMM_CHANNELS) {(*RegChan)[reg][chan] = val;
            //std::cout << "Channel register " << reg << " for channel " << chan << " set to " << val << " ." << std::endl;
            return true;
        }
        else {
            std::cout << "ERROR channel " << chan << " does not exist. Give value from 0 to 63!" << std::endl;
            return false;
        }
    }
    else {
        std::cout << "ERROR register " << reg << " does not exist." << std::endl;
        return false;
    }
}

bool VMM::CheckAllowedGlobVal(unsigned short reg, const char *val){
    bool found = false;
    // need to compare the strings at the const char * addresses
    for (unsigned short i = 0; i < (*RegGlobVals)[reg].size(); i++ )
    {
        if(ConstCharStar_comp(val,(*RegGlobVals)[reg][i])){found = true;}
    }
    //if (found){std::cout << "Value " << val << " found in global register " << reg << " which is " << (*RegGlobNames)[reg] << std::endl;}
    //else {std::cout << "ERROR: Value " << val << " not found in global register " << reg << " which is " << (*RegGlobNames)[reg] << std::endl;}
    return found;
}

bool VMM::CheckAllowedChanVal(unsigned short reg, const char *val){
    bool found = false;
    // need to compare the strings at the const char * addresses
    for (unsigned short i = 0; i < (*RegChanVals)[reg].size(); i++ )
    {
        if(ConstCharStar_comp(val,(*RegChanVals)[reg][i])){found = true;}
    }
    //if (found){std::cout << "Value " << val << " found in global register " << reg << " which is " << (*RegChanNames)[reg] << std::endl;}
    //else {std::cout << "ERROR: Value " << val << " not found in global register " << reg << " which is " << (*RegChanNames)[reg] << std::endl;}
    return found;
}

bool VMM::SetReg(const char *reg, bool val){ //set a global register, name and bool given
    const char *chr =  val ? "true" : "false";// convert bool to const char * to check if in allowed value list
    for (unsigned short i = 0; i < (*RegGlobNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegGlobNames)[i])){
            if (CheckAllowedGlobVal(i, chr)){
                SetGlobReg(i,val); return true;
            }
        }
    }
    return false;
}

bool VMM::SetReg(int regnum, bool val){
    const char *chr =  val ? "true" : "false";// convert bool to const char * to check if in allowed value list
    if (CheckAllowedGlobVal(regnum, chr)){
        SetGlobReg(regnum,val); return true;
    }
    return false;
}

bool VMM::SetReg(const char *reg, int val){
    std::stringstream str1;str1 << val;const char * chr = str1.str().c_str(); // convert int to const char * to check if in allowed value list
    for (unsigned short i = 0; i < (*RegGlobNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegGlobNames)[i])){
            if (CheckAllowedGlobVal(i, chr)){
                SetGlobReg(i,val); return true;
            }
        }
    }
    return false;
}

bool VMM::SetReg(int regnum, int val){
    std::stringstream str1;str1 << val;const char *chr = str1.str().c_str(); // convert int to const char * to check if in allowed value list
    if (CheckAllowedGlobVal(regnum, chr)){
        SetGlobReg(regnum,val); return true;
    }
    return false;
}

bool VMM::SetReg(const char * reg, double val){
    std::stringstream str1;str1 << std::fixed << std::setprecision(1)<< val;const char * chr = str1.str().c_str(); // convert double to const char * (with precision 1) to check if in allowed value list
    for (unsigned short i = 0; i < (*RegGlobNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegGlobNames)[i])){
            if (CheckAllowedGlobVal(i, chr)){
                SetGlobReg(i,FindVecEntry((*RegGlobVals)[i],chr)); return true;
            }
        }
    }
    return false;
}

bool VMM::SetReg(int regnum, double val){
    std::stringstream str1;str1 << std::fixed << std::setprecision(1)<< val;const char * chr = str1.str().c_str(); // convert double to const char * (with precision 1) to check if in allowed value list
    if (CheckAllowedGlobVal(regnum, chr)){
        SetGlobReg(regnum,FindVecEntry((*RegGlobVals)[regnum],chr)); return true;
    }

}

bool VMM::SetReg(const char *reg, const char *val){
    for (unsigned short i = 0; i < (*RegGlobNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegGlobNames)[i])){
            if (CheckAllowedGlobVal(i, val)){
                SetGlobReg(i,FindVecEntry((*RegGlobVals)[i],val)); return true;
            }
        }
    }
    return false;
}

bool VMM::SetReg(int regnum, const char *val){
    if (CheckAllowedGlobVal(regnum, val)){
        SetGlobReg(regnum,FindVecEntry((*RegGlobVals)[regnum],val)); return true;
    }
    return false;
}

bool VMM::SetReg(const char * reg, unsigned short chan, bool val){ //set a channel register, name and bool given
    const char *chr =  val ? "true" : "false";// convert bool to const char * to check if in allowed value list
    for (unsigned short i = 0; i < (*RegChanNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegChanNames)[i])){
            if (CheckAllowedChanVal(i, chr)){
                SetChanReg(i,chan,val); return true;
            }
        }
    }
    return false;
}

bool VMM::SetReg(int regnum, unsigned short chan, bool val){
    const char *chr =  val ? "true" : "false";// convert bool to const char * to check if in allowed value list
    if (CheckAllowedChanVal(regnum, chr)){
        SetChanReg(regnum,chan,val); return true;
    }
    return false;
}

bool VMM::SetReg(const char *reg, unsigned short chan, int val){
    std::stringstream str1;str1 << val;const char * chr = str1.str().c_str(); // convert int to const char * to check if in allowed value list
    for (unsigned short i = 0; i < (*RegChanNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegChanNames)[i])){
            if (CheckAllowedChanVal(i, chr)){
                SetChanReg(i,chan,val); return true;
            }
        }
    }
    return false;
}

bool VMM::SetReg(int regnum, unsigned short chan, int val){
    std::stringstream str1;str1 << val;const char *chr = str1.str().c_str(); // convert int to const char * to check if in allowed value list
    if (CheckAllowedChanVal(regnum, chr)){
        SetChanReg(regnum,chan,val); return true;
    }
    return false;
}

bool VMM::SetReg(const char *reg, unsigned short chan, const char *val){
    for (unsigned short i = 0; i < (*RegChanNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegChanNames)[i])){
            if (CheckAllowedChanVal(i, val)){
                SetChanReg(i,chan,FindVecEntry((*RegChanVals)[i],val)); return true;
            }
        }
    }
    return false;
}

bool VMM::SetReg(int regnum, unsigned short chan, const char *val){
    if (CheckAllowedChanVal(regnum, val)){
        SetChanReg(regnum,chan,FindVecEntry((*RegChanVals)[regnum],val)); return true;
    }
    return false;
}

unsigned short VMM::FindVecEntry(std::vector<const char*> reg, const char *val){
    unsigned short pos = 65536;
    // need to compare the strings at the const char * addresses
    for (unsigned short i = 0; i < reg.size(); i++ )
    {
        if(ConstCharStar_comp(val,reg[i])){pos = i;}
    }
    return pos;
}

const char * VMM::GetReg(const char *reg){
    for (unsigned short i = 0; i < (*RegGlobNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegGlobNames)[i])){
            return (*RegGlobVals)[i][(*RegGlob)[i]];
        }
    }
   {std::cout << "ERROR: Register not found" << std::endl;return "ERROR";}
}

const char * VMM::GetReg(int regnum){
    return (*RegGlobVals)[regnum][(*RegGlob)[regnum]];
}


unsigned short VMM::GetRegVal(int regnum){
    for (unsigned short i = 0; i < IsIntOrBool->size(); i++ ){
        if(ConstCharStar_comp((*IsIntOrBool)[i],(*RegGlobVals)[regnum][(*RegGlob)[regnum]])){
            if ((*RegGlobVals)[regnum][(*RegGlob)[regnum]] == "true") {return 1;}
            else if ((*RegGlobVals)[regnum][(*RegGlob)[regnum]] == "false") {return 0;}
            else{
                std::stringstream strValue;
                strValue << (*RegGlobVals)[regnum][(*RegGlob)[regnum]];
                unsigned short intValue;
                strValue >> intValue;
                return intValue;
            }
        }
    }
    return -1;
}

unsigned short VMM::GetRegVal(const char *reg){
    for (unsigned short i = 0; i < (*RegGlobNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegGlobNames)[i])){
            return GetRegVal((int)i);
        }
    }
    return -1;
}

const char * VMM::GetReg(const char *reg, unsigned short chan){
    for (unsigned short i = 0; i < (*RegChanNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegChanNames)[i])){
            return (*RegChanVals)[i][(*RegChan)[i][chan]];
        }
    }
   {std::cout << "ERROR: Register not found" << std::endl;return "ERROR";}
}

const char * VMM::GetReg(int regnum, unsigned short chan){
    return (*RegChanVals)[regnum][(*RegChan)[regnum][chan]];
}

unsigned short VMM::GetRegVal(int regnum, unsigned short chan){
    for (unsigned short i = 0; i < IsIntOrBool->size(); i++ ){
        if(ConstCharStar_comp((*IsIntOrBool)[i],(*RegChanVals)[regnum][(*RegChan)[regnum][chan]])){
            if ((*RegChanVals)[regnum][(*RegChan)[regnum][chan]] == "true") {return 1;}
            else if ((*RegChanVals)[regnum][(*RegChan)[regnum][chan]] == "false") {return 0;}
            else{
                std::stringstream strValue;
                strValue << (*RegChanVals)[regnum][(*RegChan)[regnum][chan]];
                unsigned short intValue;
                strValue >> intValue;
                return intValue;
            }
        }
    }
    return -1;
}

unsigned short VMM::GetRegVal(const char *reg, unsigned short chan){
    for (unsigned short i = 0; i < (*RegChanNames).size(); i++ ){
        if(ConstCharStar_comp(reg,(*RegChanNames)[i])){
            return GetRegVal((int)i, chan);
        }
    }
    return -1;
}


const char * VMM::GetRegGlobName(unsigned short regnum){
    if(regnum > 32){return "failure";}
    return (*RegGlobNames)[regnum];
}

const char * VMM::GetRegChanName(unsigned short regnum){
    if(regnum > 9){return "failure";}
    return (*RegChanNames)[regnum];
}

unsigned short VMM::GetRegNumber(const char *reg){
    unsigned short regnum = -1;
    for (unsigned short i = 0; i < (*RegGlobNames).size(); i++)
    {
        if (reg == (*RegGlobNames)[i]) {regnum = i;}
    }
    for (unsigned short i = 0; i < (*RegChanNames).size(); i++)
    {
        if (reg == (*RegChanNames)[i]) {regnum = i;}
    }

    return regnum;
}

unsigned short VMM::GetRegGlobSize(){
    return RegGlob->size();
}

unsigned short VMM::GetRegChanSize(){
    return RegChan->size();
}

bool VMM::ConstCharStar_comp(const char *ccs1, const char *ccs2){
    std::stringstream str1;
    str1 << ccs1;
    std::stringstream str2;
    str2 << ccs2;
    if (str1.str() == str2.str()) {return true;}
    else {return false;}
}

VMM::~VMM(){
    if( RegGlobNames != NULL ){
        delete RegGlobNames;}
    RegGlobNames = NULL;

    if( RegChanNames != NULL ){
        delete RegChanNames;}
    RegChanNames = NULL;

    if( RegGlob != NULL ){
        delete RegGlob;}
    RegGlob = NULL;

    if( RegChan != NULL ){
        delete RegChan;}
    RegChan = NULL;

    if( RegGlobVals != NULL ){
        delete RegGlobVals;}
    RegGlobVals = NULL;

    if( RegChanVals != NULL ){
        delete RegChanVals;}
    RegChanVals = NULL;

    if( IsIntOrBool != NULL ){
        delete IsIntOrBool;}
    IsIntOrBool = NULL;

}
