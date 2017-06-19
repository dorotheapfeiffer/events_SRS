#include "daq_config_handler.h"

DAQ_config_handler::DAQ_config_handler(MainWindow *top, QObject *parent) : root1{top}, QObject(parent)
{
    getcwd(ExecPath,sizeof(ExecPath));
}

bool DAQ_config_handler::LoadDAQConf(const char* filename){
    //add config path before file name
    std::string fname = ExecPath; fname+="/../"; fname+=CONFIG_DIR; fname+="/"; fname+=filename;
    return LoadDAQConfig(fname);
}

bool DAQ_config_handler::WriteDAQConf(const char* filename){
    std::string fname = ExecPath; fname+="/../"; fname+=CONFIG_DIR; fname+="/"; fname+=filename;
    return WriteDAQConfig(fname);
}

bool DAQ_config_handler::LoadDAQConfig(std::string fname){ //load the DAQ configuration from file
    std::string empty = "";
    std::ifstream f; f.open(fname,std::ifstream::in);
    if(!f.is_open()) {std::cout<< "file "<<fname<<" not found"<<std::endl;return false;}
    while (!f.eof() ){
        std::string line, val, word;
        const char *a;
        std::getline (f,line);
        std::istringstream iss(line);
        iss >> word;
        if (word[0] == '/' && word[1] == '/'){;}//do nothing

        else if (word == "daq"){
            iss >> val;
            std::string daq = val.substr(0, val.size()-1);
            iss >> word;
            if (word == "fecs:"){
                iss >> val;
                unsigned short fecs = atoi(val.c_str());
                for (unsigned short i = 0; i < fecs; i++){
                    std::getline (f,line);
                    std::istringstream iss(line);
                    iss >> word; if(word!= "fec") {std::cout  << "Syntax error in file, looking for \"fec\" in line "<< line << std::endl;return false;}
                    iss >> val; std::string fecnrst = val.substr(0, val.size()-1); unsigned short fecnr = atoi(fecnrst.c_str()); if(fecnr != i) {std::cout  << "Syntax error in file, expecting "<< i << " instead of " << fecnr << " in line "<< line << std::endl;return false;}
                    root1->daq[0].SetFEC(fecnr,true); std::cout << "Setting fec " << fecnr << " active" << std::endl;
                    iss >> word; if(word!= "hdmis:") {std::cout  << "Syntax error in file, looking for \"hdmis:\" in line "<< line << std::endl;return false;}
                    iss >> val; unsigned short hdmis = atoi(val.c_str());
                    for (unsigned short j = 0; j < hdmis; j++){
                        std::getline (f,line);
                        std::istringstream iss(line);
                        iss >> word; if(word!= "hdmi") {std::cout  << "Syntax error in file, looking for \"hdmi\" (" << j+1 << "th hdmi, you have set "<<hdmis<<" hdmis) in line "<< line << std::endl;return false;}
                        iss >> val; std::string hdminrst = val.substr(0, val.size()-1);
                        unsigned short hdminr = atoi(hdminrst.c_str()); root1->daq[0].fec[fecnr].SetHDMI(hdminr,true); std::cout << "Setting hdmi " << hdminr << " on fec " << fecnr << " active" << std::endl;
                        iss >> word; if(word!= "hybrids:") {std::cout  << "Syntax error in file, looking for \"hybrids:\" in line "<< line << std::endl;return false;}
                        iss >> val; unsigned short hybrids = atoi(val.c_str());
                        for (unsigned short k = 0; k < hybrids; k++){
                            std::getline (f,line);
                            std::istringstream iss(line);
                            iss >> word; if(word!= "hybrid") {std::cout  << "Syntax error in file, looking for \"hybrid\" in line "<< line << std::endl;return false;}
                            iss >> val; std::string hybridnrst = val.substr(0, val.size()-1); unsigned short hybridnr = atoi(hybridnrst.c_str()); if(hybridnr != k) {std::cout  << "Syntax error in file, expecting "<< k << " instead of " << hybridnr << " in line "<< line << std::endl;return false;}
                            root1->daq[0].fec[fecnr].hdmi[hdminr].SetHybrid(k,true);
                            iss >> val; if(val[0]!= 'x' && val[0] != 'y') {std::cout  << "Syntax error in file, looking for \"x\" or \"y\" in line "<< line << ", got " << val << std::endl;return false;}
                            else if (val == "x"){ root1->daq[0].fec[fecnr].hdmi[hdminr].hybrid[hybridnr].SetPosX(true);}
                            else if (val == "y"){ root1->daq[0].fec[fecnr].hdmi[hdminr].hybrid[hybridnr].SetPosX(false);}
                            iss >> val; std::string posnrst = val.substr(0, val.size()-1); root1->daq[0].fec[i].hdmi[hdminr].hybrid[k].SetPosNo(atoi(posnrst.c_str()));
                            iss >> word; if(word!= "vmms:") {std::cout  << "Syntax error in file, looking for \"vmms:\" in line "<< line << std::endl;return false;}
                            iss >> val; unsigned short vmms = atoi(val.c_str());
                            for (unsigned short l = 0; l < vmms; l++){
                                root1->daq[0].fec[fecnr].hdmi[hdminr].hybrid[hybridnr].SetVMM(l,true);
                            }
                        }
                    }
                }
            }
            else {std::cout << "Syntax error in file, looking for \"fecs\" after daq "<< daq << std::endl;return false;}
//            for (unsigned short i=0; i < DAQS_PER_GUIWINDOW; i++){
//                if (root1->daq_act[i]){
//                    for (unsigned short j=0; j < FECS_PER_DAQ; j++){
//                        if (root1->daq[i].GetFEC(j)){
//                            for (unsigned short k=0; k < HDMIS_PER_FEC; k++){
//                                if(root1->daq[i].fec[j].GetHDMI(k)){
//                                    for (unsigned short l=0; l < HYBRIDS_PER_HDMI; l++){
//                                        if (root1->daq[i].fec[j].hdmi[k].GetHybrid(l)){
//                                            for (unsigned short m=0; m < VMMS_PER_HYBRID; m++){
//                                                if (root1->daq[i].fec[j].hdmi[k].hybrid[l].GetVMM(m)){
//                                                    std::cout << "vmm " << m << " on hybrid " << l << "(pos " << root1->daq[i].fec[j].hdmi[k].hybrid[l].GetPosNo()<< ", " <<root1->daq[i].fec[j].hdmi[k].hybrid[l].GetPosX() << ") on hmdi "<< k << " on fec " << j << " on daq " << i << " is active" << std::endl;
//                                                }
//                                            }
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//             }
        }
        else if (word == empty){ // for empty line at end of file
            iss >> val;
            if (val == empty){
                break;
            }
        }
        else {
            for (unsigned short i = 0; i < root1->daq[0].GetRegSize();i++){
                 if ( word == root1->daq[0].GetRegName(i) ){
                     iss >> val;
                     a = val.c_str();
                     const char * reg = word.c_str();
                     root1->daq[0].SetReg(reg, a);
                 }
            }
        }
        if( (f.fail()) ) {return false;}
    }
    f.close(); //TODO:UpdateGUI:root1->updateConfigState();
    return true;
}

bool DAQ_config_handler::WriteDAQConfig(std::string fname){
    std::ofstream f; f.open(fname,std::ofstream::out);
    if(!f.is_open()) {return false;}
    // write conneted hardware and vmm position map
    f << "//Conneted hardware and vmm position map. No comment in the following lines, take care of syntax!" << std::endl;
    for (unsigned short i = 0; i < DAQS_PER_GUIWINDOW; i++){
        if (root1->daq_act[i]) {
            unsigned short countFECS = 0;
            for (unsigned short j = 0; j < FECS_PER_DAQ; j++){
                if (root1->daq[i].GetFEC(j)){
                    countFECS ++;
                }
            }
            f << "daq " << i << ": fecs: "<< countFECS << std::endl;
            for (unsigned short j = 0; j < countFECS; j++){
                unsigned short countHDMIS = 0;
                bool HDMI_act[HDMIS_PER_FEC] = {false};
                for (unsigned short k = 0; k < HDMIS_PER_FEC; k++){
                    if (root1->daq[i].fec[j].GetHDMI(k)){
                        countHDMIS++;
                        HDMI_act[k] = true;
                    }
                }
                f << "\tfec "<< j << ": hdmis: " << countHDMIS << std::endl;
                for (unsigned short k = 0; k < HDMIS_PER_FEC; k++){
                    if (HDMI_act[k]){
                        unsigned short countHybrids = 0;
                        for (unsigned short l = 0; l < HYBRIDS_PER_HDMI; l++){
                            if (root1->daq[i].fec[j].hdmi[k].GetHybrid(l)){
                                countHybrids++;
                            }
                        }
                        f << "\t\thdmi "<< k << ": hybrids: " << countHybrids<< std::endl;
                        for (unsigned short l = 0; l < countHybrids; l++){
                            unsigned short countVMMS = 0;
                            for (unsigned short m = 0; m < VMMS_PER_HYBRID; m++){
                                if (root1->daq[i].fec[j].hdmi[k].hybrid[l].GetVMM(m)){
                                    countVMMS++;
                                }
                            }
                            if (root1->daq[i].fec[j].hdmi[k].hybrid[l].GetPosX()){
                                f << "\t\t\thybrid "<< l << ": x " << root1->daq[i].fec[j].hdmi[k].hybrid[l].GetPosNo() << ", vmms: "<< countVMMS << std::endl;
                                std::cout << "DAQ " << i << ", FEC " << j << ", HDMI " << k << ", hybrid " << l << " at position " << root1->daq[i].fec[j].hdmi[k].hybrid[l].GetPosNo() << " on x axis has " << countVMMS << " active VMMS" << std::endl;
                            }
                            else {
                                f << "\t\t\thybrid "<< l << ": y " << root1->daq[i].fec[j].hdmi[k].hybrid[l].GetPosNo() << ", vmms: "<< countVMMS << std::endl;
                                std::cout << "DAQ " << i << ", FEC " << j << ", HDMI " << k << ", hybrid " << l << " at position " << root1->daq[i].fec[j].hdmi[k].hybrid[l].GetPosNo() << " on y axis has " << countVMMS << " active VMMS" << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }
    // daq general settings
    f << "// daq general settings" << std::endl;
    for(unsigned short i=0;i<root1->daq[0].GetRegSize() ;i++){
        const char *chr = root1->daq[0].GetReg(root1->daq[0].GetRegName(i));
        std::stringstream str;
        str << chr;
        f << root1->daq[0].GetRegName(i) << " " << str.str() << std::endl;
        std::cout << "Writing reg " << root1->daq[0].GetRegName(i) << " value: " << chr << std::endl;
        if(f.fail()) {return false;}
    }
    f << "\n";
    f.close();
    return true;
}

DAQ_config_handler::~DAQ_config_handler(){

}
