#ifndef DDATASAVE_H 
#define DDATASAVE_H

#include "DAcquisition.h"

class DDataSave {

private:
public:
  DAcquisition	*fMultiGrid;                // pointer to DAcquisition to be monitored
 
public:
  DDataSave(){}
  DDataSave(DAcquisition *);
  virtual ~DDataSave();

  ClassDef(DDataSave,0)                  // class controlon save data to file
};
//*****************************************************************************
#endif
