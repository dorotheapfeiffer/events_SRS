#include "DDataSave.h"

  ClassImp(DDataSave)

//-----------------------------------------------------------------------------
 DDataSave::DDataSave(DMultiGrid *multigrid) {
  std::cout << "Constructing DDataSave" << std::endl; //ja

  fMultiGrid = multigrid;

}
//-----------------------------------------------------------------------------
 DDataSave::~DDataSave() {

}
//-----------------------------------------------------------------------------
