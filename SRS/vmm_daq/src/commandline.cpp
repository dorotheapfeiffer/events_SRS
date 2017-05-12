#include "commandline.h"


Commandline::Commandline(MainWindow*top, int value, QObject *parent): root1{top}, index{value}, QObject(parent)
{

}

int Commandline::StartCommandline()

{
    running=true;
    std::string ein;
    VMM VMM_inst;

    std::cout << "> " << std::flush;
    const char *reg = "glob_polarity";
    const char *reg_gain = "gain";
    const char *val = "wires";
    const char *val2 = "strips";
    double doub = 3.0;
    while(running){
        std::getline(std::cin,ein);
        if((ein.compare("GeneralReset")==0)||(ein.compare("1")==0)) {std::cout << "hello"<< std::endl;}
        else if(ein.compare("quit")==0){root1->close();running=false;break;}
        else if(ein.compare("2")==0){root1->configHandle().WriteConfig("/home/michael/Readout_Software/configs/test4.xml");}
        else if(ein.compare("3")==0){root1->configHandle().LoadConfig("/home/michael/Readout_Software/configs/test4.xml");root1->updateConfigState();}
        else if(ein.compare("4")==0){std::cout << "Reg 12 is "<< root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetRegGlobName(12) << " ."<< std::endl;}
        else if(ein.compare("5")==0){std::cout << " Reg out_buffer_pdo is number " << root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetRegNumber("out_buffer_pdo") << " ." << std::endl;}
        else if(ein.compare("6")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("glob_leakage_current",true); }
        else if(ein.compare("7")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("glob_leakage_current",false); }
        else if(ein.compare("8")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("glob_polarity",true); }
        else if(ein.compare("9")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("bla",true); }
        else if(ein.compare("10")==0){std::cout << " Reg glob_polarity is " <<root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetReg(reg) << std::endl; }
        else if(ein.compare("11")==0){std::cout << " Reg glob_polarit, 10 is " <<root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetReg (reg,10) << std::endl; }
        else if(ein.compare("12")==0){std::cout << " polarity, 10 is " <<root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetReg ("polarity",10) << std::endl; }
        else if(ein.compare("13")==0){std::cout << " Reg 31 is " <<root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetReg (31) << std::endl; }
        else if(ein.compare("14")==0){std::cout << " Reg 0, 10 is " <<root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetReg (0,10) << std::endl; }
        else if(ein.compare("15")==0){std::cout << " Reg 42, 10 is " <<root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetReg (42,10) << std::endl; }
        else if(ein.compare("16")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(reg,val); }
        else if(ein.compare("17")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(reg,val2); }
        else if(ein.compare("18")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("gain",0.5); }
        else if(ein.compare("19")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("gain",3.0); }
        else if(ein.compare("20")==0){std::cout << " Reg gain is " <<root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetReg(reg_gain) << std::endl; }
        else if(ein.compare("21")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(0,"wires"); }
        else if(ein.compare("22")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(0,"strips"); }
        //else if(ein.compare("23")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(4,0.5); }
        //else if(ein.compare("24")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(4,3.0); }
        else if(ein.compare("25")==0){std::cout << " Regnum 22 ("<< root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetRegGlobName(12) <<") is " <<root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetReg(22) << std::endl; }
        else if(ein.compare("26")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].CheckAllowedGlobVal(4,"1.0"); }
        else if(ein.compare("27")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].CheckAllowedGlobVal(31,"0"); }
        else if(ein.compare("28")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].CheckAllowedGlobVal(31,"1"); }
        else if(ein.compare("29")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].CheckAllowedGlobVal(31,"2"); }
        else if(ein.compare("30")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].CheckAllowedGlobVal(0,"wires"); }
        else if(ein.compare("31")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].CheckAllowedGlobVal(0,"dogs"); }
        else if(ein.compare("32")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(1,true); }
        else if(ein.compare("33")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(1,false); }
        else if(ein.compare("34")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(0,true); }
        else if(ein.compare("35")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(31,true); }
        else if(ein.compare("36")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("threshold_dac",2); }
        else if(ein.compare("37")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("threshold_dac",0); }
        else if(ein.compare("38")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("threshold_dac",2220); }
        else if(ein.compare("39")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(31,2); }
        else if(ein.compare("40")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(31,0); }
        else if(ein.compare("41")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(31,2220); }
        else if(ein.compare("42")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(2,2220); }
        else if(ein.compare("43")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(1,12); }
        else if(ein.compare("44")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("gain",2.1); }
        else if(ein.compare("45")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("gain",0.5); }
        else if(ein.compare("46")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("gain",doub); }
        else if(ein.compare("47")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("threshold_dac",2.0); }
        else if(ein.compare("48")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(4,"2.1"); }
        else if(ein.compare("49")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(4,"0.5"); }
        else if(ein.compare("50")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(4,"3.0"); }
        else if(ein.compare("51")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(31,"2.0"); }
        else if(ein.compare("52")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("masked",4,true); }
        else if(ein.compare("53")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("s6bitADC",43,false); }
        else if(ein.compare("54")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("masked",64,false); }
        else if(ein.compare("55")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("thl_adjust",2,4); }
        else if(ein.compare("56")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("thl_adjust",4,1002); }
        else if(ein.compare("57")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("s6bitADC",43,2); }
        else if(ein.compare("58")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("masked",63,23); }
        else if(ein.compare("59")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(9,2,"4"); }
        else if(ein.compare("60")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(9,4,"1002"); }
        else if(ein.compare("61")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(7,43,"2"); }
        else if(ein.compare("62")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg(1,63,"23"); }
        else if(ein.compare("63")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("hund",2,"4"); }
        else if(ein.compare("64")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("s6bitADC",4,"1002"); }
        else if(ein.compare("65")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("s6bitADC",43,"2"); }
        else if(ein.compare("66")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("masked",63,"true"); }
        else if(ein.compare("67")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("s6bitADC",43,"0"); }
        else if(ein.compare("68")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("s6bitADC",43,"1"); }
        else if(ein.compare("69")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("s6bitADC",43,"2"); }
        else if(ein.compare("70")==0){root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].SetReg("s6bitADC",43,"3"); }
        else if(ein.compare("71")==0){std::cout<< "is " << root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetRegVal("s6bitADC",43) << std::endl; }
        else if(ein.compare("72")==0){std::cout<< "is " << root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetRegVal(0,1) << std::endl;}
        else if(ein.compare("73")==0){std::cout<< "is " << root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetRegVal("thl_adjust",2) << std::endl;}
        else if(ein.compare("74")==0){std::cout<< "is " << root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetRegVal("polarity",3) << std::endl;}
        else if(ein.compare("75")==0){std::cout << root1->vmmconfhandl->LoadAllVMMConf("test") << std::endl; }
        else if(ein.compare("77")==0){std::cout << root1->vmmconfhandl->WriteAllVMMConf("test") << std::endl; }
        else if(ein.compare("78")==0){std::cout << root1->daq[0].fec[0].SetReg(11,"Global_Frame_Cnt") << std::endl; }
        else if(ein.compare("79")==0){std::cout << root1->daq[0].fec[0].SetReg(12,"Trigger_Cnt+Trigger_Timestamp") << std::endl; }
        else if(ein.compare("80")==0){std::cout << root1->daq[0].fec[0].SetReg(13,false) << std::endl; }
        else if(ein.compare("81")==0){std::cout << root1->daq[0].fec[0].SetReg(13,(bool)1) << std::endl; }
        else if(ein.compare("82")==0){std::cout << root1->daq[0].fec[0].SetReg(13,true) << std::endl; }
        else if(ein.compare("83")==0){std::cout << root1->daq[0].fec[0].SetReg(11,"pulser") << std::endl; }
        else if(ein.compare("84")==0){std::cout << root1->daq[0].fec[0].GetRegVal(11) << std::endl; }
        else if(ein.compare("85")==0){std::cout << root1->daq[0].fec[0].GetReg(12) << std::endl; }
        else if(ein.compare("86")==0){std::cout << root1->daq[0].fec[0].GetReg("evbld_info") << std::endl; }
        else if(ein.compare("86")==0){std::cout << root1->daq[0].GetReg("ignore16") << std::endl; }
        else if(ein.compare("87")==0){std::cout << root1->daq[0].GetReg("debug") << std::endl; }
        else if(ein.compare("88")==0){std::cout << root1->daq[0].GetReg("run_count") << std::endl; }
        else if(ein.compare("89")==0){std::cout << root1->daq[0].GetReg("mapping_file") << std::endl; }
        else if(ein.compare("90")==0){std::cout << root1->daq[0].GetReg(0) << std::endl; }
        else if(ein.compare("91")==0){std::cout << root1->daq[0].GetReg(1) << std::endl; }
        else if(ein.compare("92")==0){std::cout << root1->daq[0].GetReg(2) << std::endl; }
        else if(ein.compare("93")==0){std::cout << root1->daq[0].GetReg(3) << std::endl; }
        else if(ein.compare("94")==0){std::cout << root1->daq[0].SetReg("ignore16",true) << std::endl; }
        else if(ein.compare("95")==0){std::cout << root1->daq[0].SetReg("ignore16",0) << std::endl; }
        else if(ein.compare("96")==0){std::cout << root1->daq[0].SetReg("ignore16",12) << std::endl; }
        else if(ein.compare("97")==0){std::cout << root1->daq[0].SetReg("run_count",true) << std::endl; }
        else if(ein.compare("98")==0){std::cout << root1->daq[0].SetReg("run_count",22) << std::endl; }
        else if(ein.compare("99")==0){std::cout << root1->daq[0].SetReg("run_count",65537) << std::endl; }
        else if(ein.compare("100")==0){std::cout << root1->daq[0].SetReg("mapping_file",true) << std::endl; }
        else if(ein.compare("101")==0){std::cout << root1->daq[0].SetReg("mapping_file",22) << std::endl; }
        else if(ein.compare("102")==0){std::cout << root1->daq[0].SetReg("mapping_file","true") << std::endl; }
        else if(ein.compare("103")==0){std::cout << root1->daq[0].SetReg("mapping_file","22") << std::endl; }
        else if(ein.compare("104")==0){std::cout << root1->daq[0].SetReg(2,true) << std::endl; }
        else if(ein.compare("105")==0){std::cout << root1->daq[0].SetReg("mapping_file","Hund") << std::endl; }
        else if(ein.compare("106")==0){std::cout << root1->daq[0].SetReg("ignore16","false") << std::endl; }
        else if(ein.compare("107")==0){std::cout << root1->vmmconfhandl->LoadSingleVMMConf("test",0,0,0,0,0) << std::endl; }
        else if(ein.compare("108")==0){std::cout << root1->vmmconfhandl->WriteSingleVMMConf("test",0,0,0,0,0) << std::endl; }
        else if(ein.compare("109")==0){std::cout<< "is " << root1->daq[0].fec[0].hdmi[0].hybrid[0].vmm[0].GetRegVal("s6bitADC",43) << std::endl; }
        else if(ein.compare("110")==0){std::cout << root1->vmmconfhandl->WriteSingleVMMConf("test_daq1100_fec12_hdmi3_hybrid110_vmm0.txt") << std::endl; }
        else if(ein.compare("111")==0){std::cout << root1->vmmconfhandl->WriteSingleVMMConf("test_daq0_fec12_hdmi3_hybrid110_vmm0.txt") << std::endl; }
        else if(ein.compare("112")==0){std::cout << root1->vmmconfhandl->WriteSingleVMMConf("test_daq0_fec0_hdmi3_hybrid110_vmm0.txt") << std::endl; }
        else if(ein.compare("113")==0){std::cout << root1->vmmconfhandl->WriteSingleVMMConf("test_daq0_fec0_hdmi0_hybrid110_vmm0.txt") << std::endl; }
        else if(ein.compare("114")==0){std::cout << root1->vmmconfhandl->WriteSingleVMMConf("test_daq0_fec0_hdmi0_hybrid0_vmm0.txt") << std::endl; }
        else if(ein.compare("115")==0){std::cout << root1->vmmconfhandl->LoadSingleVMMConf("test_daq0_fec0_hdmi0_hybrid0_vmm0.txt") << std::endl; }
        else if(ein.compare("116")==0){std::cout << root1->daqconfhandl->LoadDAQConf("test_daq0.txt") << std::endl; }
        else if(ein.compare("117")==0){std::cout << root1->daqconfhandl->WriteDAQConf("test_daq1.txt") << std::endl; }
        else if(ein.compare("118")==0){std::cout << root1->fecconfhandl->LoadAllFECConf("test") << std::endl; }
        else if(ein.compare("119")==0){std::cout << root1->fecconfhandl->WriteAllFECConf("test") << std::endl; }
        else{std::cout<<"command not found"<<std::endl;}
        std::cout<<"VMMDAQ> "<<std::flush;
    }
//    std::cout << "Teriminating VMMDAQ" << std::endl;
    return 0;
}

int Commandline::EndCommandline()
{
    std::cout << "end called" << std::endl;
    running=false;
    return 0;
}


Commandline::~Commandline(){

}
