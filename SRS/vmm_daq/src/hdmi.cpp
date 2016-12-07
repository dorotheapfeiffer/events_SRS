#include "hdmi.h"

HDMI::HDMI():hybrid_act (HYBRIDS_PER_HDMI)
{
    hybrid_act[0] = 1;
}

bool HDMI::SetHybrid(unsigned short hybrid, bool OnOff){
    if (hybrid < HYBRIDS_PER_HDMI) {hybrid_act[hybrid] = OnOff; return true;}
    else {return false;}
}

bool HDMI::GetHybrid(unsigned short hybrid){
    if (hybrid < HYBRIDS_PER_HDMI) {return hybrid_act[hybrid];}
    else {return false;}
}

HDMI::~HDMI()
{

}
