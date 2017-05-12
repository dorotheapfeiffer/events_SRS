//DGHelp.h
#ifndef DGHELP_H 
#define DGHELP_H

#include <iostream>

#include <TObjArray.h>
#include <TGFrame.h>
#include <TGTextView.h>
#include <TGButton.h>

//*****************************************************************************
class DGHelp : public TGTransientFrame {

private:
  TObjArray     *fMemList;           // list containing all members
  TGTextView    *fTextView;
  TGLayoutHints *fL0;
  TGLayoutHints *fL1;
  TGTextButton  *fCloseButton;

public:
  DGHelp(char * filename);
  DGHelp(const DGHelp &){}

  ~DGHelp();
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

//#ifdef COMPIL
  ClassDef(DGHelp,0)                        // Help Text Viewer
//#endif
};
//*****************************************************************************

#endif

