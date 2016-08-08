#ifndef DDATASAVE_H 
#define DDATASAVE_H

#include "DMultiGrid.h"

class DDataSave {

private:
public:
  DMultiGrid	*fMultiGrid;                // pointer to DMultiGrid to be monitored
 
public:
  DDataSave(){}
  DDataSave(DMultiGrid *);
  virtual ~DDataSave();

  ClassDef(DDataSave,0)                  // class controlon save data to file
};
//*****************************************************************************
#endif
