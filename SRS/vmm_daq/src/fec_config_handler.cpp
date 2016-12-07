#include "fec_config_handler.h"

FEC_config_handler::FEC_config_handler(MainWindow *top, QObject *parent) : root1{top}, QObject(parent)
{
    getcwd(ExecPath,sizeof(ExecPath));
}

bool FEC_config_handler::LoadAllFECConf(const char* filename){
    return GenericAllFECConf(1,filename);
}

bool FEC_config_handler::WriteAllFECConf(const char* filename){
    return GenericAllFECConf(0,filename);
}

bool FEC_config_handler::LoadSingleFECConf(const char* filename, unsigned short daq, unsigned short fec){
    return GenericSingleFECConf(1, filename, daq, fec);
}

bool FEC_config_handler::LoadSingleFECConf(const char* filename){//exact file name must be given!
    //add config path before file name
    std::string fname = ExecPath; fname+="/../"; fname+=CONFIG_DIR; fname+="/"; fname+=filename;
    return LoadFECConfig(fname);
}

bool FEC_config_handler::WriteSingleFECConf(const char* filename, unsigned short daq, unsigned short fec){
    return GenericSingleFECConf(0, filename, daq, fec);
}

bool FEC_config_handler::WriteSingleFECConf(const char* filename){//exact file name must be given!
    //need to extract daq,fec from file name
    std::stringstream str1; str1 << filename;
    std::string str(str1.str());
    //daq
    std::string daq_str = str.substr ((str.find("daq")+3),str.find("_",str.find("daq")+3)-(str.find("daq")+3));
    unsigned short daq =atoi(daq_str.c_str()); if(!root1->daq_act[daq]) {std::cout << "ERROR, daq " << daq << " does not exist "<< std::endl; return false;}
    //fec
    std::string fec_str = str.substr ((str.find("fec")+3),str.find("_",str.find("fec")+3)-(str.find("fec")+3));
    unsigned short fec =atoi(fec_str.c_str());if(!root1->daq[daq].GetFEC(fec)) {std::cout << "ERROR, fec " << fec << " does not exist "<< std::endl; return false;}
    //add config path before file name
    std::string fname = ExecPath; fname+="/../"; fname+=CONFIG_DIR; fname+="/"; fname+=filename;

    return WriteFECConfig(fname,daq,fec);
}

bool FEC_config_handler::GenericAllFECConf(bool load, const char* filename){
    for (unsigned short i=0; i < DAQS_PER_GUIWINDOW; i++){
        if (root1->daq_act[i]){
            for (unsigned short j=0; j < FECS_PER_DAQ; j++){
                if (root1->daq[i].GetFEC(j)){
                    if (load) std::cout <<"Loading FEC configuraten \""<<filename<<"\" for daq"<<i<<" fec"<<j<<std::endl;
                    else std::cout <<"Writing FEC configuraten \""<<filename<<"\" for daq"<<i<<" fec"<<j<<std::endl;
                    std::ostringstream oss;
                    std::string fname = ExecPath; fname+="/../"; fname+=CONFIG_DIR; fname+="/"; fname+=filename;
                    oss << i;
                    fname+="_daq";
                    fname+=oss.str();
                    oss.str("");oss.clear();oss << j;
                    fname+="_fec";
                    fname+=oss.str();
                    fname+=".txt"; //build file name with path and extension
                    if (load){
                        if( !LoadFECConfig(fname) ) {std::cout<<"Error"<<std::endl;return false;}
                    }
                    else{
                        if( !WriteFECConfig(fname,i,j) ) {std::cout<<"Error"<<std::endl;return false;}
                    }
                }
            }
        }
    }
    return true;
}

bool FEC_config_handler::GenericSingleFECConf(bool load, const char* filename, unsigned short daq, unsigned short fec){
    if (load) std::cout <<"Loading FEC configuraten \""<<filename<<"\" for daq"<<daq<<" fec"<<fec<<std::endl;
    else std::cout <<"Writing FEC configuraten \""<<filename<<"\" for daq"<<daq<<" fec"<<fec<<std::endl;
    std::ostringstream oss;
    std::string fname = ExecPath; fname+="/../"; fname+=CONFIG_DIR; fname+="/"; fname+=filename;
    oss << daq;
    fname+="_daq";
    fname+=oss.str();
    oss.str("");oss.clear();oss << fec;
    fname+="_fec";
    fname+=oss.str();
    fname+=oss.str();
    fname+=".txt"; //build file name with path and extension
    std::cout<< " file name: "  << fname << std::endl;
    if (load) return LoadFECConfig(fname);
    else return WriteFECConfig(fname,daq,fec);
}

bool FEC_config_handler::LoadFECConfig(std::string fname){ //load the FEC configuration from file
    std::string empty = "";
    unsigned short daq = 0, fec = 0;
    std::ifstream f; f.open(fname,std::ifstream::in);
    if(!f.is_open()) {std::cout<< "file "<<fname<<" not found"<<std::endl;return false;}
    while (!f.eof() ){
        std::string s, val;
        const char *a, *b;
        f >> s >> val;
        if (s == empty && val == empty) break; // for empty line at end of file
        else if (s == "daq") {daq = atoi(val.c_str());}
        else if (s == "fec") {fec = atoi(val.c_str());}
        else {
            a = s.c_str(); b = val.c_str();
            if (!root1->daq[daq].fec[fec].SetReg(a,b)) return false;
        }
        if( (f.fail()) ) {return false;}
    }
    f.close(); //TODO:UpdateGUI:root1->updateConfigState();
    return true;
}

bool FEC_config_handler::WriteFECConfig(std::string fname, unsigned short daq, unsigned short fec){
    std::ofstream f; f.open(fname,std::ofstream::out);
    if(!f.is_open()) {return false;}
    f << "daq " << daq << std::endl;
    f << "fec " << fec << std::endl;
    f << "\n";
    for(unsigned short j=0;j<root1->daq[daq].fec[fec].GetRegSize() ;j++){
        f << root1->daq[daq].fec[fec].GetRegName(j) << " " << root1->daq[daq].fec[fec].GetReg(j) << std::endl;
        if(f.fail()) {return false;}
    }

    f.close();
    return true;
}

FEC_config_handler::~FEC_config_handler(){

}
