#include "hybrid.h"

Hybrid::Hybrid(): vmm_act (VMMS_PER_HYBRID)
{
    vmm_act[0] = 1;
    vmm_act[1] = 1;
    PosX = true;
    PosNo = -1;
    ART = 0;
}

bool Hybrid::SetVMM(unsigned short vmm, bool OnOff){
    if (vmm < VMMS_PER_HYBRID) {vmm_act[vmm] = OnOff; return true;}
    else {return false;}
}

void Hybrid::SetART(bool OnOff){
    ART = OnOff;
}

bool Hybrid::GetVMM(unsigned short vmm){
    if (vmm < VMMS_PER_HYBRID) {return vmm_act[vmm];}
    else {return false;}
}

bool Hybrid::GetART(){
    return ART;
}

void Hybrid::SetPosX(bool IsX){
    PosX = IsX;
}

bool Hybrid::SetPosNo(unsigned short PosNr){
    if(false) return false; //TODO: check if this number is already given to another hybrid on the same axis
    else {PosNo=PosNr;return true;}
}

bool Hybrid::GetPosX(){
    return PosX;
}

unsigned short Hybrid::GetPosNo(){
    return PosNo;
}

Hybrid::~Hybrid()
{

}
