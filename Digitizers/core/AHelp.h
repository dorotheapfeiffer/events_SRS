#ifndef AHELP_H 
#define AHELP_H

#include <iostream>

#include <TGFrame.h>
#include <TGTextView.h>
#include <TGButton.h>

//*****************************************************************************
class AHelp : public TGTransientFrame {

private:
  TGTextView    *fTextView;
  TGLayoutHints *fL0;
  TGLayoutHints *fL1;
  TGTextButton  *fCloseButton;

public:
  AHelp(char * filename);
  ~AHelp();
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  ClassDef(AHelp,0)                        // Help Text Viewer
};
//*****************************************************************************

#endif

