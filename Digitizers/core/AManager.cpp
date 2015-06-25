#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <exception>
#include <usb.h>

#include <TApplication.h>
#include <TDatime.h>
#include <TGFileDialog.h>

#include "Digitizer.h"
//#include "Acqiris.h"
//#include "Spectrum.h"
#include "MainFrame.h"
#include "AManager.h"
#include "ARingBuffer.h"


//#include "ADQAPI.h"
//#include "AcqirisImport.h"
//#include "AcqirisD1Import.h" // Import for Agilent Acqiris Digitizers


#define _CRT_SECURE_NO_WARNINGS

extern int gDEBUG_MANAGER;

using namespace std;

ClassImp(AManager)

AManager::AManager() {
     
   #ifdef DEBUG
   if(gDEBUG_MANAGER > 0) cout << "DEBUG [AManager::AManager] " << endl;
   #endif

   // initialization all private variables

   mWindow		= 0;	// pointer to main window
   mControlFrame	= 0;	// pointer to control window 
   

   mAcq			= 0;	// pointer to acqusition 
   mDataSave		= 0;	// pointer to dataSave thread
   mOnlineServer	= 0;	// pointer to onlilne server thread

   mTimeout		= 0;
   mSegments		= 1;

   mDisplayEvents 	= kFALSE;

   mAcqLoops		= 0;
   mFileSize		= 80;  // in MB
   mFileTime		= 3600;  // in sec
   mFileNr		= 0;
   mMaxRange 		= 2000;
   mMaxNrSamples	= 1024;
   mMaxEvents		= 0;
   mMaxFiles		= 0;
   mEventNr		= 0;
   

   FindDevices();

      // Create Digitizer devices
     try{
        for(UInt_t i = 0; i < mFindDev.size(); i++) {  
             if( 
                // !mFindDev.at(i).find(string("Sim"))  
                 !mFindDev.at(i).find(string("CAEN5740")) 
              )
               {
               #ifdef DEBUG
               if(gDEBUG_MANAGER > 2) cout << "DEBUG [AManager::AManager()] Create digitizer object: " << mFindDev.at(i) << endl;
               #endif
               mAcqDev.push_back( Digitizer::Factory(mFindDev.at(i)) );
               }
           }
        }
     catch(Digitizer::BadDigitizerCreation e) {
        cout << e.what() << endl;
        }

   if(mAcqDev.size() == 0){
     cout << endl << endl << endl;
     cout << "+-----------------------------------------------------------------+" << endl;
     cout << "| Program did not find any connected digitizers.                  |" << endl;
     cout << "| Are digitizers connected?                                       |" << endl;
     cout << "| Then try to reboot the computer while digitizers are pluged in. |" << endl;
     cout << "+-----------------------------------------------------------------+" << endl;
     cout << endl << endl << endl;
     gApplication->Terminate();
    } 

   // Initialize function set the handle to the card
   for(UInt_t i = 0; i < mAcqDev.size(); i++) {
      #ifdef DEBUG
      if(gDEBUG_MANAGER > 2) cout << "DEBUG [AManager::AManager()] Initialize: " << mAcqDev[i]->GetName() << endl;               
      #endif
      mAcqDev[i]->Initialize();
      }

  
   // Read configuration file and configure cards
   mWorkingDir = gSystem->WorkingDirectory();
   mConfigureFileName = TString(mWorkingDir) + "/acqrc" ;
     
   // for(int i = 0; i < mAcqDev.size(); i++) {
   //    mAcqDev[i]->PrintInfo();
   //    }

   // Read configuration file
   LoadConfiguration();

}
//==============================================================================

void AManager::FindDevices() {

 size_t found;

   #ifdef DEBUG
   if(gDEBUG_MANAGER > 2) cout << "DEBUG [AManager::FindDevies] Searching for spectrum digitizers" << endl;
   #endif

   struct usb_bus *bus;
   struct usb_device *dev;
   
   usb_init();
   usb_find_busses();
   usb_find_devices();
   for(bus = usb_busses; bus; bus = bus->next){
      for(dev = bus->devices; dev; dev = dev->next){

          if(dev->descriptor.idVendor == 0x21e1){
 
              mFindDev.push_back(string("CAEN5740"));

		//printf("Trying devices %s %s \n", bus->dirname, dev->filename); 
		//printf("\tID_VENDOR  = 0x%04x\n", dev->descriptor.idVendor); 
		//printf("\tID_PRODUCT = 0x%04x\n", dev->descriptor.idProduct); 
 	    }
         }
      }
   


 //----------- simulation cards ------------ 
/*
 int sCount = 1;
  for(int i = 0; i < sCount; i++){
      mFindDev.push_back(string("SIM"));
     }
*/

#ifdef DEBUG
 if(gDEBUG_MANAGER > 2){ cout << "DEBUG [AManager::FindDevices] found " << mFindDev.size() << " cards" << endl;
      for(UInt_t i = 0; i < mFindDev.size(); i++) {    // show it
	  cout << "\t"<< i << " " << mFindDev.at(i) << endl;
          }
 }
#endif

}

//==============================================================================

void AManager::StartAcquisition(MainFrame::mSTATE aState){

   cout << "DEBUG AManager::StartAcquisition " << GetMode() << endl;
// SetMode(aState);
   mWindow->Refresh();

// if(aState == MainFrame::sOSCI || aState == MainFrame::sACQUISITION){
   ControlAcq aAcq(1);
   aAcq.Start();
   cout << "DEBUG AManager::StopAcquisition " << GetMode() << endl;
    
// }
}

//==============================================================================

void AManager::StopAcquisition(MainFrame::mSTATE aState){
 cout << "DEBUG AManager::StopAcquisition " << aState << endl;
 SetMode(aState);
 mWindow->Refresh();
 delete mAcq;
}

//==============================================================================
void AManager::ConfigureFrame(Bool_t create){

 if(create)
   mControlFrame = new ControlFrame(mWindow);
 else
   mControlFrame = NULL;
}
//==============================================================================

void AManager::CreateGui() {
#ifdef DEBUG
 if(gDEBUG_MANAGER > 0) cout << "DEBUG [AManager::CreateGui]" << endl;
#endif
	mWindow = new MainFrame(gClient->GetRoot(),800,600);
}


//=========================================================================
void AManager::LoadConfiguration(){
#ifdef DEBUG
 if(gDEBUG_MANAGER > 0) cout << "DEBUG [AManager::LoadConfiguration]" << endl;
#endif
  ReadConfigFile( mConfigureFileName );
//  for(UInt_t i = 0; i < mAcqDev.size(); i++) {
//      mAcqDev[i]->Configure();
//      mAcqDev[i]->Refresh();
//     }

}
//=========================================================================
void AManager::SaveConfiguration(){

}
//=========================================================================

Int_t AManager::ReadConfigFile(const char *filename) {
ifstream fin(filename, ios::in);
if(!fin){
  cout << "ERROR could not open the file " << filename << ". The default parameters will be used"<< endl;
  return -1;
  }

  //First reading configuration for the general acq settings and then for each card in the system
  Int_t temp;
  string line;
  string name;
  string value;
  string inSection;
  int posEqual;
  int lineNr = 0;
  while (getline(fin,line)) {
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
       if( name == string("SoftTimeout") ){ 
         temp = atoi( value.c_str() );
         if(temp <= 0) mTimeout = 0;
         else mTimeout = temp;
         }
       else if( name == string("NrLoops") ){ 
         temp = atoi( value.c_str() );
         if(temp <= 0) mAcqLoops = 0;
         else { mAcqLoops = temp; mFileNr = 0;}
         }    
       else if( name == string("NrFiles") ){ 
         temp = atoi( value.c_str() );
         if(temp <= 0) mFileNr = 0;
         else { mFileNr = temp; mAcqLoops = 0;}
         }    
       else if( name == string("FileSize") ){ 
         temp = atoi( value.c_str() );
         if(temp <= 0) mFileSize = 4;
         else mFileSize = temp;
         }    
     }
/*
    if( inSection == string("ACQ SETTINGS")){
       if( name == string("ACQmode") ){ 
          mAcqMode   = atoi( value.c_str() ); 
          if(mAcqMode != 1 && mAcqMode != 2){
             cout << "ERROR in configuration file: value not allowed, line nr " << lineNr
                  << "\t[" << inSection << "] " << name << " = " << value << endl;
             cout << "       ACQmode set to 1 [standard single mode]" << endl;     
            mAcqMode = 1;
            }
         }
       else if(name == string("NumberOfSegments")){ 
          mSegments = atoi( value.c_str() ); 
          if(mSegments < 1){
             cout << "ERROR in configuration file: value not allowed, line nr " << lineNr
                  << "\t[" << inSection << "] " << name << " = " << value << endl;
             cout << "       NumberOfSegments set to 1" << endl;     
             mSegments = 1;
            }
          else if(mSegments > 1000){
             cout << "ERROR in configuration file: value not allowed, line nr " << lineNr
                  << "\t[" << inSection << "] " << name << " = " << value << endl;
             cout << "       NumberOfSegments set to 1" << endl;     
             mSegments = 1;
            }
          else if(mSegments != 1 && mAcqMode == 1){
             cout << "WARRNING in configuration file: value does not any effect, line nr " << lineNr
                  << "\t[" << inSection << "] " << name << " = " << value << endl;
             cout << "       NumberOfSegments set to 1" << endl;     
             mSegments = 1;
            }
          else {
           } 
             

           }
       else if(name == string("NumberOfFiles")){ mFileNr = atoi(value.c_str()); }
       else if(name == string("NumberOfLoops")){ mAcqLoops = atoi(value.c_str()); }
       else if(name == string("FileName"))     { mDataFileName = value.c_str(); }
       else if(name == string("FileSize"))     { mFileSize  = atoi(value.c_str()); }
       else cout << "ERROR: Parameter not recognized, line nr " << lineNr
                 << "\t[" << inSection << "] >" << name << "< = >" << value <<"<"<< endl;
      }
*/

    } // end while loop

   #ifdef DEBUG
	cout << "DEBUG [AManager::ReadConfigFile] "	<< endl;
	cout << "\t[GENERAL SETTINGS]"			<< endl;
	cout << "SoftTimeout "	<< mTimeout	<< endl;
	cout << "NrLoops "	<< mAcqLoops	<< endl;
	cout << "NrFiles " 	<< mFileNr 	<< endl;
	cout << "FileSize "	<< mFileSize	<< endl;
   #endif
   // now each card
fin.clear();
fin.seekg (0);

for(UInt_t i = 0; i < mAcqDev.size(); i++) {
    mAcqDev[i]->ReadConfigFile(fin);
    fin.clear();
    fin.seekg (0);
   }

fin.close();
return 0;
}
//==============================================================================

Int_t AManager::WriteConfigFile(const char *filename) {

 cout << "DEBUG AManager::WriteConfigFile " << endl;

//if(filename == NULL)
//   filename = mWorkingDir + mConfigureFileName;

ofstream fout(mConfigureFileName, ios::out | ios::trunc);
if(!fout) {
   cout << "ERROR [ WriteConfigFile ] could not open the file... " << filename << endl;
   return 1;
   }
TDatime dtime;

fout << "# Configuration file for the data acquisition system" << endl;
fout << "# the file was created:  " << dtime.GetYear() << "." << dtime.GetMonth() << "." << dtime.GetDay() << "\t"
     << dtime.GetHour() << ":" << dtime.GetMinute() << ":"<< dtime.GetSecond() << endl;
fout << "# Line starts with a '#' is a comment line" << endl;
fout << "#" << endl;
fout << "#" << endl;
fout << "#" << endl;
fout << "#" << endl;
fout << "#" << endl;
fout << "#" << endl;
fout << "[GENERAL SETTINGS]"		<< endl;
fout << "SoftTimeout "	<< mTimeout	<< endl;
fout << "NrLoops "	<< mAcqLoops	<< endl;
fout << "NrFiles " 	<< mFileNr 	<< endl;
fout << "FileSize "	<< mFileSize	<< endl;
fout << "" << endl;
fout << "" << endl;
fout << "" << endl;
for(UInt_t i = 0; i < mAcqDev.size(); i++) {
    mAcqDev[i]->WriteConfigFile(fout);
   }

fout.close();
return 0;


}
//==============================================================================


void AManager::ClearAll() {
 StopAcquisition( MainFrame::sSTOP );
 
 WriteConfigFile(); 

 // It must also stop all other services...

 // close digitizers should be in destructor???:w

 //for(int i = 0; i < aManager->GetNrDigitizers(); i++){
 //    aManager->GetDigitizer(i)->Close(); 
 //    }

 if (mControlFrame) delete mControlFrame;

 vector<Digitizer*>::iterator it;
 for(it = mAcqDev.begin(); it != mAcqDev.end(); ++it) {
      delete *it;
      *it = 0;
      }
 mAcqDev.clear();

 mFindDev.clear();
 
}

//==============================================================================
string AManager::Trim(string const& source, char const* delims )
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

