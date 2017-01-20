// Mainframe macro generated from application: /Applications/root/bin/root.exe
// By ROOT version 6.06/04 on 2016-11-14 21:52:28

#ifndef ROOT_TGDockableFrame
#include "TGDockableFrame.h"
#endif
#ifndef ROOT_TGMenu
#include "TGMenu.h"
#endif
#ifndef ROOT_TGMdiDecorFrame
#include "TGMdiDecorFrame.h"
#endif
#ifndef ROOT_TG3DLine
#include "TG3DLine.h"
#endif
#ifndef ROOT_TGMdiFrame
#include "TGMdiFrame.h"
#endif
#ifndef ROOT_TGMdiMainFrame
#include "TGMdiMainFrame.h"
#endif
#ifndef ROOT_TGMdiMenu
#include "TGMdiMenu.h"
#endif
#ifndef ROOT_TGListBox
#include "TGListBox.h"
#endif
#ifndef ROOT_TGNumberEntry
#include "TGNumberEntry.h"
#endif
#ifndef ROOT_TGHtml
#include "TGHtml.h"
#endif
#ifndef ROOT_TGScrollBar
#include "TGScrollBar.h"
#endif
#ifndef ROOT_TGComboBox
#include "TGComboBox.h"
#endif
#ifndef ROOT_TGuiBldHintsEditor
#include "TGuiBldHintsEditor.h"
#endif
#ifndef ROOT_TGuiBldNameFrame
#include "TGuiBldNameFrame.h"
#endif
#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGFileDialog
#include "TGFileDialog.h"
#endif
#ifndef ROOT_TGShutter
#include "TGShutter.h"
#endif
#ifndef ROOT_TGButtonGroup
#include "TGButtonGroup.h"
#endif
#ifndef ROOT_TGCanvas
#include "TGCanvas.h"
#endif
#ifndef ROOT_TGFSContainer
#include "TGFSContainer.h"
#endif
#ifndef ROOT_TGuiBldEditor
#include "TGuiBldEditor.h"
#endif
#ifndef ROOT_TGColorSelect
#include "TGColorSelect.h"
#endif
#ifndef ROOT_TGButton
#include "TGButton.h"
#endif
#ifndef ROOT_TGFSComboBox
#include "TGFSComboBox.h"
#endif
#ifndef ROOT_TGLabel
#include "TGLabel.h"
#endif
#ifndef ROOT_TGView
#include "TGView.h"
#endif
#ifndef ROOT_TRootGuiBuilder
#include "TRootGuiBuilder.h"
#endif
#ifndef ROOT_TGTab
#include "TGTab.h"
#endif
#ifndef ROOT_TGListView
#include "TGListView.h"
#endif
#ifndef ROOT_TGSplitter
#include "TGSplitter.h"
#endif
#ifndef ROOT_TGTextView
#include "TGTextView.h"
#endif
#ifndef ROOT_TGStatusBar
#include "TGStatusBar.h"
#endif
#ifndef ROOT_TGListTree
#include "TGListTree.h"
#endif
#ifndef ROOT_TGuiBldGeometryFrame
#include "TGuiBldGeometryFrame.h"
#endif
#ifndef ROOT_TGToolTip
#include "TGToolTip.h"
#endif
#ifndef ROOT_TGToolBar
#include "TGToolBar.h"
#endif
#ifndef ROOT_TGuiBldDragManager
#include "TGuiBldDragManager.h"
#endif

#include "Riostream.h"

void unnamed()
{

   // main frame
   TGMainFrame *fMainFrame739 = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
   fMainFrame739->SetName("fMainFrame739");
   fMainFrame739->SetLayoutBroken(kTRUE);

   // tab widget
   TGTab *fTab560 = new TGTab(fMainFrame739,96,26);

   // container of "Tab1"
   TGCompositeFrame *fCompositeFrame563;
   fCompositeFrame563 = fTab560->AddTab("Tab1");
   fCompositeFrame563->SetLayoutManager(new TGVerticalLayout(fCompositeFrame563));


   // container of "Tab2"
   TGCompositeFrame *fCompositeFrame565;
   fCompositeFrame565 = fTab560->AddTab("Tab2");
   fCompositeFrame565->SetLayoutManager(new TGVerticalLayout(fCompositeFrame565));


   fTab560->SetTab(0);

   fTab560->Resize(fTab560->GetDefaultSize());
   fMainFrame739->AddFrame(fTab560, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTab560->MoveResize(80,160,96,26);

   // tab widget
   TGTab *fTab571 = new TGTab(fMainFrame739,224,275);

   // container of "Tab1"
   TGCompositeFrame *fCompositeFrame574;
   fCompositeFrame574 = fTab571->AddTab("Tab1");
   fCompositeFrame574->SetLayoutManager(new TGVerticalLayout(fCompositeFrame574));

   // vertical frame
   TGVerticalFrame *fVerticalFrame588 = new TGVerticalFrame(fCompositeFrame574,135,135,kVerticalFrame);
   fVerticalFrame588->SetLayoutBroken(kTRUE);

   TGFont *ufont;         // will reflect user font changes
   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   TGGC   *uGC;           // will reflect user GC changes
   // graphics context changes
   GCValues_t valEntry595;
   valEntry595.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valEntry595.fForeground);
   gClient->GetColorByName("#e7e7e7",valEntry595.fBackground);
   valEntry595.fFillStyle = kFillSolid;
   valEntry595.fFont = ufont->GetFontHandle();
   valEntry595.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valEntry595, kTRUE);
   TGTextEntry *fTextEntry595 = new TGTextEntry(fVerticalFrame588, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kOwnBackground);
   fTextEntry595->SetMaxLength(4096);
   fTextEntry595->SetAlignment(kTextLeft);
   fTextEntry595->SetText("fTextEntry595");
   fTextEntry595->Resize(90,fTextEntry595->GetDefaultHeight());
   fVerticalFrame588->AddFrame(fTextEntry595, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextEntry595->MoveResize(45,85,90,21);

   fCompositeFrame574->AddFrame(fVerticalFrame588, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


   // container of "Tab2"
   TGCompositeFrame *fCompositeFrame576;
   fCompositeFrame576 = fTab571->AddTab("Tab2");
   fCompositeFrame576->SetLayoutManager(new TGVerticalLayout(fCompositeFrame576));

   // horizontal frame
   TGHorizontalFrame *fHorizontalFrame581 = new TGHorizontalFrame(fCompositeFrame576,2,2,kHorizontalFrame);
   fHorizontalFrame581->SetLayoutBroken(kTRUE);

   fCompositeFrame576->AddFrame(fHorizontalFrame581, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

   ULong_t ucolor;        // will reflect user color changes
   gClient->GetColorByName("#ffffff",ucolor);

   // combo box
   TGComboBox *fComboBox602 = new TGComboBox(fCompositeFrame576,-1,kHorizontalFrame | kSunkenFrame | kOwnBackground);
   fComboBox602->AddEntry("Entry 1 ",0);
   fComboBox602->AddEntry("Entry 2 ",1);
   fComboBox602->AddEntry("Entry 3 ",2);
   fComboBox602->AddEntry("Entry 4 ",3);
   fComboBox602->AddEntry("Entry 5 ",4);
   fComboBox602->AddEntry("Entry 6 ",5);
   fComboBox602->AddEntry("Entry 7 ",6);
   fComboBox602->Resize(102,21);
   fComboBox602->Select(-1);
   fCompositeFrame576->AddFrame(fComboBox602, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


   fTab571->SetTab(1);

   fTab571->Resize(fTab571->GetDefaultSize());
   fMainFrame739->AddFrame(fTab571, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTab571->MoveResize(194,97,224,275);

   fMainFrame739->SetMWMHints(kMWMDecorAll,
                        kMWMFuncAll,
                        kMWMInputModeless);
   fMainFrame739->MapSubwindows();

   fMainFrame739->Resize(fMainFrame739->GetDefaultSize());
   fMainFrame739->MapWindow();
   fMainFrame739->Resize(490,372);
}  
