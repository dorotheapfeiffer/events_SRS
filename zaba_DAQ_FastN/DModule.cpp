///////////////////////////////////////////////////////////////////////////////
// DModule is a parent class for DScaler, DLogic, DDisc, etc. 
///////////////////////////////////////////////////////////////////////////////

#include "DModule.h"
#include "CAENVMElib.h"

  ClassImp(DModule)

//*****************************************************************************
 DModule::DModule(Char_t *gtdesc, Char_t *atdesc, Char_t *mdesc, UInt_t adoff) 
       : TObject() {
  fAdBase=adoff;
  strcpy(fGenTypeDescription, gtdesc);
  strcpy(fActTypeDescription, atdesc);
  strcpy(fModuleDescription,  mdesc);

  m_VMEBridge = cvV1718;
  m_Handle = -1;

}
//-----------------------------------------------------------------------------
 DModule::~DModule() {
}
//-----------------------------------------------------------------------------
 void DModule::PrintAddress() {
  std::cout<<"address="<<fAdBase<<"n";
}
//-----------------------------------------------------------------------------
void ShowBuffer(char * buffer, int nchar) {
  int i;
  for (i=0; i<nchar; i++) printf("(%02x)",buffer[i]);
  if (i > 0) printf("n");
}
//-----------------------------------------------------------------------------
 Int_t DModule::Access(Char_t *flag, Int_t adoff, Int_t len, void *buf) {
  // Access to a VME module via the server which runs on VME. 
  // flag   - "r" for reading, "w" for writing
  // adoff  - address within the module
  // len    - number of bytes of data to be read or written
  // buf    - pointer to the data buffer
  //
  // swapping is OK for FIC, check for others. 

  Long_t addr,llen;
  TMessage command;
  static TSocket *soc;
  char tempbuf[2048];

  // Define the socket during the first access. 
  // For this pick the last item in the list of sockets. 

  if (!soc)  soc = (TSocket*) gROOT->GetListOfSockets()->Last();
  if (!soc) {
    printf("Access: no socketn");
    exit(-1);
  }

  command.SetWhat(0x01020304);

  addr = fAdBase+adoff;
  llen = (Long_t) len;
  //  addr = htonl(addr);
  //  llen = htonl(llen);
  command.WriteFastArray(&addr,1);
  command.WriteFastArray(&llen,1);

  if (strcmp(flag,"r")==0) {
    if (soc->Send(command) != 12) {
      printf("Access: Send failedn");
      exit(-1);
    }
    soc->RecvRaw(tempbuf,len);
    swab(tempbuf,buf,len);
  }

  if (strcmp(flag,"w")==0) {
    swab(buf,tempbuf,len);
    command.WriteFastArray((Char_t *) tempbuf,len);
    if (soc->Send(command) != len+12) {
      printf("Access: Send failedn");
      exit(-1);
    }
    soc->RecvRaw(tempbuf,2);
    //    ShowBuffer(tempbuf,len);
  }
  return 0;
}
//-----------------------------------------------------------------------------

 Int_t DModule::BitAccess(Int_t adoff, Int_t bioff) {
  // Read a bit from VME.
  // Since VME can only be accessed word-wise, the bit access works
  // by copying a word from VME and accessing the required bit of 
  // this word. 
  // adoff  - address of the 16-bit word in the module
  // bioff  - bit address (0-15) 

  UShort_t temp;
  Access((char*)"r", adoff, 2, &temp);
  return ((temp>>bioff) & 0x1);
}
//-----------------------------------------------------------------------------
 void DModule::BitAccess(Int_t adoff, Int_t bioff, Int_t val) {
  // Set a VME bit to val.
  // Since VME can only be accessed word-wise, the bit access works
  // by copying a word from VME and accessing the required bit of 
  // this word. The modified word is subsequently written to VME. 
  // adoff  - address of the 16-bit word in the module
  // bioff  - bit address (0-15) 
  // val    - value to which the bit should be set

  //UShort_t temp;

  //Access((char*)"r", adoff, 2, &temp);
  //setbit(&temp, bioff, val);
  //Access((char*)"w", adoff, 2, &temp);
}
//*****************************************************************************
std::string DModule::Trim(std::string const& source, char const* delims )
{
 std::string result(source);
 std::string::size_type index = result.find_last_not_of(delims);
 if(index != std::string::npos)
    result.erase(++index);

  index = result.find_first_not_of(delims);
  if(index != std::string::npos)
    result.erase(0, index);
  else
    result.erase();
  return result;
}





