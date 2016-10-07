#include "DAcquisition.h"
#include <sys/time.h>  
 
ClassImp(DAcquisition)

using namespace std;
extern std::string g_Path;

//*****************************************************************************
 DAcquisition::DAcquisition(std::string s) : TObject(), m_Gnuplot("/usr/bin/gnuplot"), m_AcqName(s) {
  // Create objects corresponding to VME modules, and store the pointers 
  // to these objects on the object array *fModuleList.

  fLog = new std::ofstream(string(m_AcqName)+string("log.txt"), std::ios::out|std::ios::app);

  if (!fLog) {
    std::cout << "[ERROR] log file cannot be opened, exit" << std::endl;
    exit(0);
  }

  m_StartAcqTime = std::time(NULL);
  std::string s1 = ctime(&m_StartAcqTime);
  *fLog << s1.substr(0, s1.length()-1) << " ----- szaba has started" << endl;
  *fLog << "\t\t\t" << " ----- constructing modules:" << endl;
  std::cout<<"[MESSAGE] szaba has started\n";
  std::cout<<"[MESSAGE] constructing modules:\n";
  
  fModuleList = new TObjArray();

  //fModuleList->Add(fDV1718     = new DV1718(   (char*)"VME_USB_Bridge",  0x00000000));
  //fModuleList->Add(fDMadc32    = new DMadc32(  (char*)"Mesytec MADC-32", 0xd0000000));
  fModuleList->Add(fDCAEN1740D  = new DCAEN1740D((char*)"CAEN 1740D",      0x32100000));
  //fModuleList->Add(fDCAEN1740  = new DCAEN1740((char*)"CAEN1740D",        0));

  std::cout << "[MESSAGE] number of modules:      " << fModuleList->GetLast()+1 << std::endl;
  *fLog << "\t\t\t" << " ----- number of modules:      " << fModuleList->GetLast()+1 << std::endl;

  m_Status 		= 0;
  m_Timerout 		= 1;
  m_Display 		= 1;
  m_AcqStatusEntry1	= 0;
  m_AcqStatusEntry2	= 0;
  m_AcqStatusEntry3	= 0;
  m_AcqStatusEntry4	= 0;
  m_StopAfterSecEntry	= 0;
  m_StopAfterFileEntry	= 0;
  m_StopAfterEventsEntry= 0;
  m_SaveFileSizeCB	= 0;
  m_SaveFileSizeEntry	= 100; //in MB
  m_SaveFileTimeCB	= 0;
  m_SaveFileTimeEntry   = 3600;
  m_PrevEvent		= 0.;
  m_PrevSize		= 0.;

  m_HistogramSave	= 0;
  m_DataSave		= 0;

  m_ElapsedAcqTime	= 0;
  m_ElapsedTimeMS	= 0;
  m_PrevTimeMS		= 0;
  m_CurrentTimeMS	= 0;;

  m_NrOfSavedFiles	= 0;
  m_EmptyBuffer		= 0;
  m_StopCondition	= 0;

  m_FileName = string("data");
  m_Path = g_Path;
  m_DataPath = m_Path + string("/data");
  m_ConfigPath = g_Path + string("/zabarc"); 

  LoadConfig((char*)m_ConfigPath.c_str());
  SaveConfig((char*)m_ConfigPath.c_str());

}
//-----------------------------------------------------------------------------
   DAcquisition::~DAcquisition() {
    m_TimeNow = std::time(NULL);
    std::string s1 = string(ctime(&m_TimeNow));
    *fLog << "\t\t\t ----- destroing modules" << endl;    std::cout<<"[MESSAGE] destroing modules:\n";

    SaveConfig((char*)m_ConfigPath.c_str());
    fModuleList->Delete();
    delete fModuleList;
    *fLog << s1.substr(0, s1.length()-1) << " ----- szaba has ended\n" << endl;
    delete fLog;
    std::cout<<"[MESSAGE] szaba has ended\n";
    exit(0);
    //gApplication->Terminate(0);
}
//-----------------------------------------------------------------------------
 void DAcquisition::ResetModules() {
  // Call ResetModule for every VME module of fModuleList

  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->ResetModule();
  delete iter;
}
//-----------------------------------------------------------------------------
 void DAcquisition::InitModules() {
  // Setup these parameters of VME modules which are fixed, i.e. not
  // supposed to be changed by the end-user, and thus are not accesible
  // via GUI.

  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->InitModule();
 
  delete iter;
}

//-----------------------------------------------------------------------------
void DAcquisition::BuildEvent(){

  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->GetEvent();
  delete iter;
   	
 }
//-----------------------------------------------------------------------------
bool DAcquisition::CheckCondition(){
  // stops acquisition after certain number of events set by the user
  if(m_StopAfterEventsEntry){
     if( m_AcqStatusEntry2 >= m_StopAfterEventsEntry ){
	 m_StopCondition = 3;    
         return kFALSE;
       }
     }

  // stops acquisition after certain number of seconds set by the user
  if(m_StopAfterSecEntry){
     //std::cout << "m_ElapsedAcqTime: " << m_ElapsedAcqTime << " m_StopAfterSecEntry: " << m_StopAfterSecEntry << std::endl;
     if( std::difftime(std::time(NULL), m_StartAcqTime) >= (UInt_t)m_StopAfterSecEntry ){
	 m_StopCondition = 2;    
         return kFALSE;
       }
     }

  // stops acquisition after certain number of saved files set by the user
  if(m_StopAfterFileEntry){
     //std::cout << "m_NrOvedFiles " << m_NrOfSavedFiles << " m_StopAfterFileEntry: " << m_StopAfterFileEntry << std::endl;
     if( m_NrOfSavedFiles >= m_StopAfterFileEntry ){
	 m_StopCondition = 1;    
         return kFALSE;
       }
     }

return kTRUE;

}
//=============================================================================
 void DAcquisition::EmptyBuffer(){

  if(m_StopCondition == 1) 
     return;
  else{
     m_EmptyBuffer = 1;
     DataSave();
  }
	 
 }
//=============================================================================
 void DAcquisition::GnuplotOnline(){

  static UInt_t a_gnuplot_prev = GetTimeMS();	 
  UInt_t a_gnuplot_esaplsed = GetTimeMS() - a_gnuplot_prev;
  
  if (a_gnuplot_esaplsed < 100)
      return;

  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->GnuplotOnline( m_Gnuplot );
 
  delete iter;
  a_gnuplot_prev = GetTimeMS();
  
	 
 }

//=============================================================================
 void DAcquisition::ShowData(DGDisplay *d) {

  m_CurrentTimeMS = GetTimeMS();
  m_ElapsedTimeMS = m_CurrentTimeMS - m_PrevTimeMS;
  
  if (m_ElapsedTimeMS < 1000)
      return;

  std::cout << "[MESSAGE] ACQ time: " << std::difftime(std::time(NULL), m_StartAcqTime) << " s.\n"; 

  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->ShowData(d, this);
 
  delete iter;
  m_PrevTimeMS = m_CurrentTimeMS;
}

//=============================================================================
void DAcquisition::StartAcq(){

  char mbstr[100];
  if (std::strftime(mbstr, sizeof(mbstr), "%Y_%m_%d_%H%M", std::localtime(&m_StartAcqTime)) ) 
  SetFileTime(string(mbstr));
 
 
  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->Log(*fLog);

  std::string s1 = string(ctime(&m_StartAcqTime));

  *fLog     << s1.substr(0, s1.length()-1) << " ----- Start acquisition" << std::endl;
  *fLog     << s1.substr(0, s1.length()-1) << " ----- Stop after " << m_StopAfterSecEntry << ", 0 means forever" << std::endl;
  *fLog     << s1.substr(0, s1.length()-1) << " ----- Stop after " << m_StopAfterFileEntry << ", 0 means forever" << std::endl;
  *fLog     << s1.substr(0, s1.length()-1) << " ----- Stop after " << m_StopAfterEventsEntry << ", 0 means forever" << std::endl;
  std::cout << "[MESSAGE] " << s1.substr(0, s1.length()-1) << " ----- Start acquisition ----- " << std::endl;
  std::cout << "[MESSAGE] " << s1.substr(0, s1.length()-1) << " ----- Stop after " << m_StopAfterSecEntry << ", 0 means forever" << std::endl;
  std::cout << "[MESSAGE] " << s1.substr(0, s1.length()-1) << " ----- Stop after " << m_StopAfterFileEntry << ", 0 means forever" << std::endl;
  std::cout << "[MESSAGE] " << s1.substr(0, s1.length()-1) << " ----- Stop after " << m_StopAfterEventsEntry << ", 0 means forever" << std::endl;

  iter->Reset();
  m_StartAcqTime = std::time(NULL);
  m_PrevTimeMS = GetTimeMS();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->StartAcq();
 
  delete iter;
}
//-----------------------------------------------------------------------------
void DAcquisition::StopAcq(){
  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->StopAcq();

  EmptyBuffer();

  m_TimeNow = std::time(NULL);
  std::string s1 = string(ctime(&m_TimeNow));
  *fLog << s1.substr(0, s1.length()-1) << " ----- Stop and quit acquisition" << std::endl;
  std::cout << "[MESSAGE] " << s1.substr(0, s1.length()-1) << " ----- Stop and quit acquisition ----- " << std::endl;
  delete iter;
}
//-----------------------------------------------------------------------------

 void DAcquisition::DataSave() {

  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->DataSave(this);

  delete iter; 
}

//-----------------------------------------------------------------------------

 ULong_t DAcquisition::GetTimeMS() {
    long time_ms;
    struct timeval t1;
    struct timezone tz;
    gettimeofday(&t1, &tz);
    time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
    return time_ms;

 }
//-----------------------------------------------------------------------------
 void DAcquisition::LoadConfig(char *filename) {

  std::ifstream inpfile(filename, std::ios::in);
  if (!inpfile) {
    std::cout << "DAcquisition: cannot open file " << filename << std::endl;
    return;
  }

  std::cout << "[MESSAGE] Load configuration file from " << filename << endl;
  *fLog << "\t\t\t ----- Load configuration file from " << filename << endl;
  Int_t temp;
  string line;
  string name;
  string value;
  string inSection;
  int posEqual;
  int lineNr = 0;

   while (getline(inpfile,line)) {
    lineNr++;
    if ( !line.length()) continue;

    if (line[0] == '#') continue;
    if (line[0] == ';') continue;

    if (line[0] == '[') {
      inSection = Trim(line.substr(1,line.find(']')-1));
      continue;
      }

    posEqual=line.find(' ');
    name  = Trim(line.substr(0,posEqual));
    value = Trim(line.substr(posEqual+1));

     if( inSection == string("GENERAL SETTINGS")){
       if( name == string("StopAfterSec") ){
         temp = atoi( value.c_str() );
         if(temp < 0) m_StopAfterSecEntry = 100;
         else { m_StopAfterSecEntry = temp; m_StopAfterFileEntry = 0; m_SaveFileSizeEntry = 0;}
         }
       else if( name == string("StopAfterFile") ){
         temp = atoi( value.c_str() );
         if(temp < 0) m_StopAfterFileEntry = 0;
         else { m_StopAfterFileEntry = temp; m_StopAfterEventsEntry = 0; m_SaveFileSizeEntry = 0;}
         }
       else if( name == string("StopAfterEvents") ){
         temp = atoi( value.c_str() );
         if(temp < 0) m_StopAfterEventsEntry = 0;
         else { m_StopAfterEventsEntry = temp; m_StopAfterFileEntry = 0; m_StopAfterSecEntry = 0;}
         }
       else if( name == string("SaveFileSize") ){
         temp = atoi( value.c_str() );
         if(temp < 0) m_SaveFileSizeEntry = 99;
         else m_SaveFileSizeEntry = temp;
         }
       else if( name == string("SaveFileTime") ){
         temp = atoi( value.c_str() );
         if(temp < 0) m_SaveFileTimeEntry = 3600;
         else m_SaveFileTimeEntry = temp;
         }
       else if( name == string("DataPath") ){
         if(value.length()) SetDataPath(value);
         }
       else if( name == string("FileName") ){
         if(value.length()) SetFileName(value); 
         else SetFileName(string("test"));
	  
        }

     }

    } // end while loop

 inpfile.clear();
 inpfile.seekg (0);

  

  std::cout << "[MESSAGE] path to szaba:          " << g_Path       << std::endl;
  std::cout << "[MESSAGE] configuration file:     " << m_ConfigPath << std::endl;
  std::cout << "[MESSAGE] stored data path:       " << m_DataPath   << std::endl;
  std::cout << "[MESSAGE] name added to filename: " << m_FileName   << std::endl;


  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) {
       ((DModule*) elem)->LoadConfig(inpfile);
       inpfile.clear();
       inpfile.seekg (0);
       }

  delete iter;
inpfile.close();

}
//-----------------------------------------------------------------------------
 void DAcquisition::SaveConfig(char *filename) {

 ofstream fout(filename, ios::out | ios::trunc);
 if(!fout) {
   cout << "ERROR [ WriteConfigFile ] could not open the file... " << filename << endl;
   return ;
   }

  TDatime dtime;

  fout << "# Configuration file for ZABA data acquisition system" << std::endl;
  fout << "# the file was created:  " << dtime.GetYear() << "." << dtime.GetMonth() << "." << dtime.GetDay() << "\t"
     << dtime.GetHour() << ":" << dtime.GetMinute() << ":"<< dtime.GetSecond() << std::endl;
  fout << "# Line starts with a '#' is a comment line" << std::endl;
  fout << "#" << std::endl;
  fout << "#" << std::endl;
  fout << "[GENERAL SETTINGS]"            << std::endl;
  fout << "# Stop acquisition after certain number of seconds, default 0 means acq will runn forever" << std::endl;
  fout << "# Keep in mind that from StopAfterSec, StopAfterFile, StopAfterEvents ONLY ONE CAN BE DIFFERENT THAN 0!" << std::endl;
  fout << "StopAfterSec " <<  m_StopAfterSecEntry << std::endl;
  fout << " " << std::endl;
  fout << "# Stop acquisition after certain number of files, default 0 means acq will run forever, size of the file you have to define in annother variable" << std::endl;
  fout << "StopAfterFile " << m_StopAfterFileEntry << std::endl;
  fout << " " << std::endl;
  fout << "# Stop acquisition after certain number of Events, default 0 means acq will runn forever" << std::endl;
  fout << "StopAfterEvents " << m_StopAfterEventsEntry << std::endl;
  fout << " " << std::endl;

  fout << "# Save each file after reach the file size" << std::endl;
  fout << "SaveFileSize " << m_SaveFileSizeEntry << std::endl;
  fout << " " << std::endl;
  fout << "# Save each file after certain number of seconds" << std::endl;
  fout << "SaveFileTime " << m_SaveFileTimeEntry << std::endl;
  fout << " " << std::endl;

  fout << "# Default path where the data is stored, if you do not set this parameter the path will be ./data" << std::endl;
  fout << "DataPath " << m_DataPath << std::endl;
  fout << " " << std::endl;

  fout << "# Default file name, in the GUI version program will ask the user to set correct file name" << std::endl;
  fout << "# The format of file name is: YYYY_MM_DD_hhmm_xxxxxx_nr.bin" << std::endl;
  fout << "# where xxxxxx is the file name and nr file number" << std::endl;
  fout << "# if you put the field of FileName empty then by default the name will be test" << std::endl;
  fout << "# e.g is 2016 July 14, Thursday, 19:56 and you set filename measurement1" << std::endl;
  fout << "#     the file name will be: 2016_07_14_1956_measurement1_000.bin" << std::endl;
  fout << "#     then only the number will be increased, so 001, 002, 003, 004" << std::endl;

  fout << "FileName " << m_FileName << std::endl;
  fout << " " << std::endl;

  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->SaveConfig(fout);

  delete iter;
fout.close();





}
//-----------------------------------------------------------------------------
 void DAcquisition::ReadVME() {
  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->ReadVME();

  delete iter;
}
//*****************************************************************************

std::string DAcquisition::Trim(std::string const& source, char const* delims )
{
 string result(source);
 string::size_type index = result.find_last_not_of(delims);
 if(index != string::npos)
    result.erase(++index);

  index = result.find_first_not_of(delims);
  if(index != string::npos)
    result.erase(0, index);
  else
    result.erase();
  return result;
}


