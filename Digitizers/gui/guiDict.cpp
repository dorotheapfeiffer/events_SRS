// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME guiDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "ADisplay.h"
#include "MainFrame.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_ADisplay(void *p = 0);
   static void *newArray_ADisplay(Long_t size, void *p);
   static void delete_ADisplay(void *p);
   static void deleteArray_ADisplay(void *p);
   static void destruct_ADisplay(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ADisplay*)
   {
      ::ADisplay *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ADisplay >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ADisplay", ::ADisplay::Class_Version(), "ADisplay.h", 34,
                  typeid(::ADisplay), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ADisplay::Dictionary, isa_proxy, 4,
                  sizeof(::ADisplay) );
      instance.SetNew(&new_ADisplay);
      instance.SetNewArray(&newArray_ADisplay);
      instance.SetDelete(&delete_ADisplay);
      instance.SetDeleteArray(&deleteArray_ADisplay);
      instance.SetDestructor(&destruct_ADisplay);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ADisplay*)
   {
      return GenerateInitInstanceLocal((::ADisplay*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ADisplay*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_MainFrame(void *p);
   static void deleteArray_MainFrame(void *p);
   static void destruct_MainFrame(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MainFrame*)
   {
      ::MainFrame *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MainFrame >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MainFrame", ::MainFrame::Class_Version(), "MainFrame.h", 26,
                  typeid(::MainFrame), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MainFrame::Dictionary, isa_proxy, 4,
                  sizeof(::MainFrame) );
      instance.SetDelete(&delete_MainFrame);
      instance.SetDeleteArray(&deleteArray_MainFrame);
      instance.SetDestructor(&destruct_MainFrame);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MainFrame*)
   {
      return GenerateInitInstanceLocal((::MainFrame*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MainFrame*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ADisplay::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ADisplay::Class_Name()
{
   return "ADisplay";
}

//______________________________________________________________________________
const char *ADisplay::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ADisplay*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ADisplay::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ADisplay*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ADisplay::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ADisplay*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ADisplay::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ADisplay*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MainFrame::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MainFrame::Class_Name()
{
   return "MainFrame";
}

//______________________________________________________________________________
const char *MainFrame::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MainFrame*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MainFrame::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MainFrame*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MainFrame::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MainFrame*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MainFrame::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MainFrame*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ADisplay::Streamer(TBuffer &R__b)
{
   // Stream an object of class ADisplay.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ADisplay::Class(),this);
   } else {
      R__b.WriteClassBuffer(ADisplay::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ADisplay(void *p) {
      return  p ? new(p) ::ADisplay : new ::ADisplay;
   }
   static void *newArray_ADisplay(Long_t nElements, void *p) {
      return p ? new(p) ::ADisplay[nElements] : new ::ADisplay[nElements];
   }
   // Wrapper around operator delete
   static void delete_ADisplay(void *p) {
      delete ((::ADisplay*)p);
   }
   static void deleteArray_ADisplay(void *p) {
      delete [] ((::ADisplay*)p);
   }
   static void destruct_ADisplay(void *p) {
      typedef ::ADisplay current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ADisplay

//______________________________________________________________________________
void MainFrame::Streamer(TBuffer &R__b)
{
   // Stream an object of class MainFrame.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MainFrame::Class(),this);
   } else {
      R__b.WriteClassBuffer(MainFrame::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_MainFrame(void *p) {
      delete ((::MainFrame*)p);
   }
   static void deleteArray_MainFrame(void *p) {
      delete [] ((::MainFrame*)p);
   }
   static void destruct_MainFrame(void *p) {
      typedef ::MainFrame current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MainFrame

namespace {
  void TriggerDictionaryInitialization_guiDict_Impl() {
    static const char* headers[] = {
"ADisplay.h",
"MainFrame.h",
0
    };
    static const char* includePaths[] = {
"../core",
"/home/dlab/Downloads/root-6.05.02/include",
"/home/dlab/dg_epool/Digitizers/gui/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$ADisplay.h")))  ADisplay;
class __attribute__((annotate("$clingAutoload$MainFrame.h")))  MainFrame;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "ADisplay.h"
#include "MainFrame.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ADisplay", payloadCode, "@",
"MainFrame", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("guiDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_guiDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_guiDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_guiDict() {
  TriggerDictionaryInitialization_guiDict_Impl();
}
