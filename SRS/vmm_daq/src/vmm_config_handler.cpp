#include "vmm_config_handler.h"

VMM_config_handler::VMM_config_handler(MainWindow *top, QObject *parent) : root1{top}, QObject(parent)
{
    getcwd(ExecPath,sizeof(ExecPath));
}

bool VMM_config_handler::LoadAllVMMConf(const char* filename){
    return GenericAllVMMConf(1,filename);
}

bool VMM_config_handler::WriteAllVMMConf(const char* filename){
    return GenericAllVMMConf(0,filename);
}

bool VMM_config_handler::LoadSingleVMMConf(const char* filename, unsigned short daq, unsigned short fec, unsigned short hdmi, unsigned short hybrid, unsigned short vmm){
    return GenericSingleVMMConf(1, filename, daq, fec, hdmi, hybrid, vmm);
}

bool VMM_config_handler::LoadSingleVMMConf(const char* filename){//exact file name must be given!
    //add config path before file name
    std::string fname = ExecPath; fname+="/../"; fname+=CONFIG_DIR; fname+="/"; fname+=filename;
    return LoadVMMConfig(fname);
}

bool VMM_config_handler::WriteSingleVMMConf(const char* filename, unsigned short daq, unsigned short fec, unsigned short hdmi, unsigned short hybrid, unsigned short vmm){
    return GenericSingleVMMConf(0, filename, daq, fec, hdmi, hybrid, vmm);
}

bool VMM_config_handler::WriteSingleVMMConf(const char* filename){//exact file name must be given!
    //need to extract daq,fec,... from file name
    std::stringstream str1; str1 << filename;
    std::string str(str1.str());
    //daq
    std::string daq_str = str.substr ((str.find("daq")+3),str.find("_",str.find("daq")+3)-(str.find("daq")+3));
    unsigned short daq =atoi(daq_str.c_str()); if(!root1->daq_act[daq]) {std::cout << "ERROR, daq " << daq << " does not exist "<< std::endl; return false;}
    //fec
    std::string fec_str = str.substr ((str.find("fec")+3),str.find("_",str.find("fec")+3)-(str.find("fec")+3));
    unsigned short fec =atoi(fec_str.c_str());if(!root1->daq[daq].GetFEC(fec)) {std::cout << "ERROR, fec " << fec << " does not exist "<< std::endl; return false;}
    //hdmi
    std::string hdmi_str = str.substr ((str.find("hdmi")+4),str.find("_",str.find("hdmi")+4)-(str.find("hdmi")+4));
    unsigned short hdmi =atoi(hdmi_str.c_str());if(!root1->daq[daq].fec[fec].GetHDMI(hdmi)) {std::cout << "ERROR, hdmi " << hdmi << " does not exist "<< std::endl; return false;}
    //hybrid
    std::string hybrid_str = str.substr ((str.find("hybrid")+6),str.find("_",str.find("hybrid")+6)-(str.find("hybrid")+6));
    unsigned short hybrid =atoi(hybrid_str.c_str());if(!root1->daq[daq].fec[fec].hdmi[hdmi].GetHybrid(hybrid)) {std::cout << "ERROR, hybrid " << hybrid << " does not exist "<< std::endl; return false;}
    //vmm
    std::string vmm_str = str.substr ((str.find("vmm")+3),str.find("_",str.find("vmm")+3)-(str.find("vmm")+3));
    unsigned short vmm =atoi(vmm_str.c_str());if(!root1->daq[daq].fec[fec].hdmi[hdmi].hybrid[hybrid].GetVMM(vmm)) {std::cout << "ERROR, vmm " << vmm << " does not exist "<< std::endl; return false;}
    //add config path before file name
    std::string fname = ExecPath; fname+="/../"; fname+=CONFIG_DIR; fname+="/"; fname+=filename;

    return WriteVMMConfig(fname,daq,fec,hdmi,hybrid,vmm);
}
bool VMM_config_handler::GenericAllVMMConf(bool load, const char* filename){
    for (unsigned short i=0; i < DAQS_PER_GUIWINDOW; i++){
        if (root1->daq_act[i]){
            for (unsigned short j=0; j < FECS_PER_DAQ; j++){
                if (root1->daq[i].GetFEC(j)){
                    for (unsigned short k=0; k < HDMIS_PER_FEC; k++){
                        if(root1->daq[i].fec[j].GetHDMI(k)){
                            for (unsigned short l=0; l < HYBRIDS_PER_HDMI; l++){
                                if (root1->daq[i].fec[j].hdmi[k].GetHybrid(l)){
                                    for (unsigned short m=0; m < VMMS_PER_HYBRID; m++){
                                        if (root1->daq[i].fec[j].hdmi[k].hybrid[l].GetVMM(m)){
                                            if (load) std::cout <<"Loading vmm configuraten \""<<filename<<"\" for daq"<<i<<" fec"<<j<<" hdmi"<<k<<" hybrid"<<l<<" vmm"<<m<<std::endl;
                                            else std::cout <<"Writing vmm configuraten \""<<filename<<"\" for daq"<<i<<" fec"<<j<<" hdmi"<<k<<" hybrid"<<l<<" vmm"<<m<<std::endl;
                                            std::ostringstream oss;
                                            std::string fname = ExecPath; fname+="/../"; fname+=CONFIG_DIR; fname+="/"; fname+=filename;
                                            oss << i;
                                            fname+="_daq";
                                            fname+=oss.str();
                                            oss.str("");oss.clear();oss << j;
                                            fname+="_fec";
                                            fname+=oss.str();
                                            oss.str("");oss.clear();oss << k;
                                            fname+="_hdmi";
                                            fname+=oss.str();
                                            oss.str("");oss.clear();oss << l;
                                            fname+="_hybrid";
                                            fname+=oss.str();
                                            oss.str("");oss.clear();oss << m;
                                            fname+="_vmm";
                                            fname+=oss.str();
                                            fname+=".txt"; //build file name with path and extension
                                            if (load){
                                                if( !LoadVMMConfig(fname) ) {std::cout<<"Error"<<std::endl;return false;}
                                            }
                                            else{
                                                if( !WriteVMMConfig(fname,i,j,k,l,m) ) {std::cout<<"Error"<<std::endl;return false;}
                                            }

                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool VMM_config_handler::GenericSingleVMMConf(bool load, const char* filename, unsigned short daq, unsigned short fec, unsigned short hdmi, unsigned short hybrid, unsigned short vmm){
    if (load) std::cout <<"Loading vmm configuraten \""<<filename<<"\" for daq"<<daq<<" fec"<<fec<<" hdmi"<<hdmi<<" hybrid"<<hybrid<<" vmm"<<vmm<<std::endl;
    else std::cout <<"Loading vmm configuraten \""<<filename<<"\" for daq"<<daq<<" fec"<<fec<<" hdmi"<<hdmi<<" hybrid"<<hybrid<<" vmm"<<vmm<<std::endl;
    std::ostringstream oss;
    std::string fname = ExecPath; fname+="/../"; fname+=CONFIG_DIR; fname+="/"; fname+=filename;
    oss << daq;
    fname+="_daq";
    fname+=oss.str();
    oss.str("");oss.clear();oss << fec;
    fname+="_fec";
    fname+=oss.str();
    oss.str("");oss.clear();oss << hdmi;
    fname+="_hdmi";
    fname+=oss.str();
    oss.str("");oss.clear();oss << hybrid;
    fname+="_hybrid";
    fname+=oss.str();
    oss.str("");oss.clear();oss << vmm;
    fname+="_vmm";
    fname+=oss.str();
    fname+=".txt"; //build file name with path and extension
    std::cout<< " file name: "  << fname << std::endl;
    if (load) return LoadVMMConfig(fname);
    else return WriteVMMConfig(fname,daq,fec,hdmi,hybrid,vmm);
}

bool VMM_config_handler::LoadVMMConfig(std::string fname){ //load the VMM configuration from file
    std::string channel = "channel";
    std::string empty = "";
    unsigned short daq = 0, fec = 0, hdmi = 0,hybrid = 0, vmms = 0;
    std::ifstream f; f.open(fname,std::ifstream::in);
    if(!f.is_open()) {std::cout<< "file "<<fname<<" not found"<<std::endl;return false;}
    while (!f.eof() ){
        std::string s, val, chanreg, chanval;
        const char *a, *b;
        f >> s >> val;
        if (s == empty && val == empty) break; // for empty line at end of file
        else if (s == "daq") {daq = atoi(val.c_str());}
        else if (s == "fec") {fec = atoi(val.c_str());}
        else if (s == "hdmi") {hdmi = atoi(val.c_str());}
        else if (s == "hybrid") {hybrid = atoi(val.c_str());}
        else if (s == "vmm") {vmms = atoi(val.c_str());}
        else if (s == channel) {
            unsigned short chan = atoi(val.c_str());
            f >> chanreg >> chanval;
            a = chanreg.c_str(); b= chanval.c_str();
            if (!root1->daq[daq].fec[fec].hdmi[hdmi].hybrid[hybrid].vmm[vmms].SetReg(a,chan,b)) return false;
        }
        else {
            a = s.c_str(); b = val.c_str();
            if (!root1->daq[daq].fec[fec].hdmi[hdmi].hybrid[hybrid].vmm[vmms].SetReg(a,b)) return false;
        }
        if( (f.fail()) ) {return false;}
    }
    f.close(); //TODO:UpdateGUI:root1->updateConfigState();
    return true;
}

bool VMM_config_handler::WriteVMMConfig(std::string fname, unsigned short daq, unsigned short fec, unsigned short hdmi, unsigned short hybrid, unsigned short vmm){
    std::ofstream f; f.open(fname,std::ofstream::out);
    if(!f.is_open()) {return false;}
    f << "daq " << daq << std::endl;
    f << "fec " << fec << std::endl;
    f << "hdmi " << hdmi << std::endl;
    f << "hybrid " << hybrid << std::endl;
    f << "vmm " << vmm << std::endl;
    f << "\n";
    for(unsigned short j=0;j<root1->daq[daq].fec[fec].hdmi[hdmi].hybrid[hybrid].vmm[vmm].GetRegGlobSize() ;j++){
        f << root1->daq[daq].fec[fec].hdmi[hdmi].hybrid[hybrid].vmm[vmm].GetRegGlobName(j) << " " << root1->daq[daq].fec[fec].hdmi[hdmi].hybrid[hybrid].vmm[vmm].GetReg(j) << std::endl;
        if(f.fail()) {return false;}
    }
    f << "\n";
    for(unsigned short k=0;k<VMM_CHANNELS;k++){
        for (unsigned short j = 0; j < root1->daq[daq].fec[fec].hdmi[hdmi].hybrid[hybrid].vmm[vmm].GetRegChanSize(); j++){
            f << "channel " << k << " " << root1->daq[daq].fec[fec].hdmi[hdmi].hybrid[hybrid].vmm[vmm].GetRegChanName(j) << " " << root1->daq[daq].fec[fec].hdmi[hdmi].hybrid[hybrid].vmm[vmm].GetReg(j,k) << std::endl;
            if(f.fail()) {return false;}
        }
        if(k<63) f << "\n"; // no empty line at end
    }

    f.close();
    return true;
}

VMM_config_handler::~VMM_config_handler(){

}
