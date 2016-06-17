#include "DMultiGrid.h"

ClassImp(DMultiGrid)

using namespace std;

//*****************************************************************************
 DMultiGrid::DMultiGrid() : TObject() {
  // Create objects corresponding to VME modules, and store the pointers 
  // to these objects on the object array *fModuleList.

  std::cout<<"constructing DMultiGrid\n";
  
  fModuleList = new TObjArray();

  fModuleList->Add(fDMadc32  = new DMadc32((char*)"MultiGrid",   0xd0000000));
  fModuleList->Add(fDV1718  = new DV1718((char*)"VME_USB_Bridge",   0x00000000));

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

  m_StartAcqTime.Set();
  m_EndAcqTime.Set();

  m_ElapsedAcqTime	= 0;
  m_PrevAcqTime		= 0;
  m_NrOfSavedFiles	= 0;

  LoadConfig("zabarc");
  SaveConfig("zabarc");

  printf("%d modules\n",fModuleList->GetLast()+1);
}
//-----------------------------------------------------------------------------
   DMultiGrid::~DMultiGrid() {
    std::cout<<"destroying DMultiGrid\n";
    SaveConfig("zabarc");
    fModuleList->Delete();
    delete fModuleList;
    exit(0);
    //gApplication->Terminate(0);
}
//-----------------------------------------------------------------------------
 void DMultiGrid::ResetModules() {
  // Call ResetModule for every VME module of fModuleList

  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->ResetModule();
}
//-----------------------------------------------------------------------------
 void DMultiGrid::InitModules() {
  // Setup these parameters of VME modules which are fixed, i.e. not
  // supposed to be changed by the end-user, and thus are not accesible
  // via GUI.

  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->InitModule();
 
}
//-----------------------------------------------------------------------------

 void DMultiGrid::ShowData(DGDisplay *d) {

  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->ShowData(d);
 
}

//-----------------------------------------------------------------------------
void DMultiGrid::StartAcq(){
  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->StartAcq();
 
}
//-----------------------------------------------------------------------------
void DMultiGrid::StopAcq(){
  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->StopAcq();
 
}
//-----------------------------------------------------------------------------

 void DMultiGrid::DataSave() {

  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->DataSave(this);
 
}

//-----------------------------------------------------------------------------

 void DMultiGrid::LoadConfig(char *filename) {

  std::ifstream inpfile(filename, std::ios::in);
  if (!inpfile) {
    std::cout << "DMultiGrid: cannot open file " << filename << std::endl;
    return;
  }

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
         if(temp <= 0) m_StopAfterSecEntry = 100;
         else { m_StopAfterSecEntry = temp; m_StopAfterFileEntry = 0; m_SaveFileSizeEntry = 0;}
         }
       else if( name == string("StopAfterFile") ){
         temp = atoi( value.c_str() );
         if(temp <= 0) m_StopAfterFileEntry = 0;
         else { m_StopAfterFileEntry = temp; m_StopAfterEventsEntry = 0; m_SaveFileSizeEntry = 0;}
         }
       else if( name == string("StopAfterEvents") ){
         temp = atoi( value.c_str() );
         if(temp <= 0) m_StopAfterEventsEntry = 0;
         else { m_StopAfterEventsEntry = temp; m_StopAfterFileEntry = 0; m_StopAfterSecEntry = 0;}
         }
       else if( name == string("SaveFileSize") ){
         temp = atoi( value.c_str() );
         if(temp <= 0) m_SaveFileSizeEntry = 100;
         else m_SaveFileSizeEntry = temp;
         }
       else if( name == string("SaveFileTime") ){
         temp = atoi( value.c_str() );
         if(temp <= 0) m_SaveFileTimeEntry = 3600;
         else m_SaveFileTimeEntry = temp;
         }

     }

    } // end while loop

inpfile.clear();
inpfile.seekg (0);

  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) {
       ((DModule*) elem)->LoadConfig(inpfile);
       inpfile.clear();
       inpfile.seekg (0);
       }

inpfile.close();

}
//-----------------------------------------------------------------------------
 void DMultiGrid::SaveConfig(char *filename) {

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

  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->SaveConfig(fout);

fout.close();





}
//-----------------------------------------------------------------------------
 void DMultiGrid::ReadVME() {
  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->ReadVME();
}
//*****************************************************************************

std::string DMultiGrid::Trim(std::string const& source, char const* delims )
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


