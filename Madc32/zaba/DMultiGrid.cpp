#include "DMultiGrid.h"

ClassImp(DMultiGrid)


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

  printf("%d modules\n",fModuleList->GetLast()+1);
}
//-----------------------------------------------------------------------------
   DMultiGrid::~DMultiGrid() {
    std::cout<<"destroying DMultiGrid\n";
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

 void DMultiGrid::Load(char *filename) {
/*
  Int_t i, j;
  TString buf;

  std::ifstream inpfile(filename, std::ios::in);
  if (!inpfile) {
    std::cout << "DMultiGrid: cannot open file " << filename << std::endl;
    return;
  }

  while (buf.ReadLine(inpfile)) {

    }
  return;
*/
}
//-----------------------------------------------------------------------------
 void DMultiGrid::Save(char *filename) {

}
//-----------------------------------------------------------------------------
 void DMultiGrid::ReadVME() {
  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->ReadVME();
}
//*****************************************************************************

