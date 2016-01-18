/*****************************************************************************
*
* Implementation of the abstract class for digitizer
*
*****************************************************************************/

#include <iostream>
#include <string>
#include <exception>

#include "Digitizer.h"
#include "CAEN5740.h"
#include "SimCard.h"

using namespace std;

ClassImp(Digitizer)

//===============================================================================
Int_t Digitizer::mDigitizers = 0;

Digitizer* Digitizer::Factory(const string& type) throw(Digitizer::BadDigitizerCreation) {
   if(type == "SIM")           { return new SimCard(10);     } 
   if(type == "CAEN5740")      { return new CAEN5740(1);     } 
 throw BadDigitizerCreation(type);
}

//===============================================================================
string Digitizer::Trim(string const & source, char const* delims){

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

//===============================================================================
void Digitizer::Refresh() {
  cerr << "DEBUG Digitizer::Refresh(), NO IMPLEMENTATION FOR THIS DEVICE, you have to immplement it in the derived class" << endl;
 }
//===============================================================================
void Digitizer::Reset() {
  cerr << "DEBUG Digitizer::Reset(), NO IMPLEMENTATION FOR THIS DEVICE, you have to immplement it in the derived class" << endl;
 }

//===============================================================================
void Digitizer::PrintInfo() {
  cerr << "DEBUG Digitizer::PrintInfo(), NO IMPLEMENTATION FOR THIS DEVICE, you have to immplement it in the derived class" << endl;
 }

//===============================================================================
void Digitizer::DoEnable(bool) {
  cerr << "DEBUG Digitizer::DoEnable(), NO IMPLEMENTATION FOR THIS DEVICE, you have to immplement it in the derived class" << endl;
 }

//===============================================================================
Bool_t Digitizer::IsEnabled() const {
  cerr << "DEBUG Digitizer::IsEnabled(), NO IMPLEMENTATION FOR THIS DEVICE, you have to immplement it in the derived class" << endl;
  return false;
 }

//===============================================================================

Int_t Digitizer::Initialize() {
  cerr << "DEBUG Digitizer::Initialize(), NO IMPLEMENTATION FOR THIS DEVICE, you have to immplement it in the derived class" << endl;
  return 0;
 }


//===============================================================================

Int_t Digitizer::Configure() {
  cerr << "DEBUG Digitizer::Configure(), NO IMPLEMENTATION FOR THIS DEVICE, you have to immplement it in the derived class" << endl;
  return 0;
 }

//===============================================================================

void Digitizer::StartAcq() {
  cerr << "DEBUG Digitizer::StartAcq(), NO IMPLEMENTATION FOR THIS DEVICE, you have to immplement it in the derived class" << endl;
 }

//===============================================================================

void Digitizer::StopAcq() {
  cerr << "DEBUG Digitizer::StopAcq(), NO IMPLEMENTATION FOR THIS DEVICE, you have to immplement it in the derived class" << endl;
 }


//===============================================================================

void Digitizer::Close() {
  cerr << "DEBUG Digitizer::Close(), NO IMPLEMENTATION FOR THIS DEVICE, you have to immplement it in the derived class" << endl;
 }




