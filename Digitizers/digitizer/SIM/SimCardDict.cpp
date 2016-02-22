// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME SimCardDict

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
#include "SimCard.h"
#include "SimCardDlg.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_SimCard(void *p);
   static void deleteArray_SimCard(void *p);
   static void destruct_SimCard(void *p);
   static void streamer_SimCard(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SimCard*)
   {
      ::SimCard *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SimCard >(0);
      static ::ROOT::TGenericClassInfo 
         instance("SimCard", ::SimCard::Class_Version(), "SimCard.h", 28,
                  typeid(::SimCard), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::SimCard::Dictionary, isa_proxy, 16,
                  sizeof(::SimCard) );
      instance.SetDelete(&delete_SimCard);
      instance.SetDeleteArray(&deleteArray_SimCard);
      instance.SetDestructor(&destruct_SimCard);
      instance.SetStreamerFunc(&streamer_SimCard);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SimCard*)
   {
      return GenerateInitInstanceLocal((::SimCard*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SimCard*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_SimCardDlg(void *p = 0);
   static void *newArray_SimCardDlg(Long_t size, void *p);
   static void delete_SimCardDlg(void *p);
   static void deleteArray_SimCardDlg(void *p);
   static void destruct_SimCardDlg(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SimCardDlg*)
   {
      ::SimCardDlg *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SimCardDlg >(0);
      static ::ROOT::TGenericClassInfo 
         instance("SimCardDlg", ::SimCardDlg::Class_Version(), "SimCardDlg.h", 22,
                  typeid(::SimCardDlg), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::SimCardDlg::Dictionary, isa_proxy, 4,
                  sizeof(::SimCardDlg) );
      instance.SetNew(&new_SimCardDlg);
      instance.SetNewArray(&newArray_SimCardDlg);
      instance.SetDelete(&delete_SimCardDlg);
      instance.SetDeleteArray(&deleteArray_SimCardDlg);
      instance.SetDestructor(&destruct_SimCardDlg);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SimCardDlg*)
   {
      return GenerateInitInstanceLocal((::SimCardDlg*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SimCardDlg*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr SimCard::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *SimCard::Class_Name()
{
   return "SimCard";
}

//______________________________________________________________________________
const char *SimCard::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SimCard*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int SimCard::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SimCard*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SimCard::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SimCard*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SimCard::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SimCard*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr SimCardDlg::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *SimCardDlg::Class_Name()
{
   return "SimCardDlg";
}

//______________________________________________________________________________
const char *SimCardDlg::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SimCardDlg*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int SimCardDlg::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SimCardDlg*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SimCardDlg::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SimCardDlg*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SimCardDlg::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SimCardDlg*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void SimCard::Streamer(TBuffer &R__b)
{
   // Stream an object of class SimCard.

   Digitizer::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_SimCard(void *p) {
      delete ((::SimCard*)p);
   }
   static void deleteArray_SimCard(void *p) {
      delete [] ((::SimCard*)p);
   }
   static void destruct_SimCard(void *p) {
      typedef ::SimCard current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_SimCard(TBuffer &buf, void *obj) {
      ((::SimCard*)obj)->::SimCard::Streamer(buf);
   }
} // end of namespace ROOT for class ::SimCard

//______________________________________________________________________________
void SimCardDlg::Streamer(TBuffer &R__b)
{
   // Stream an object of class SimCardDlg.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(SimCardDlg::Class(),this);
   } else {
      R__b.WriteClassBuffer(SimCardDlg::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_SimCardDlg(void *p) {
      return  p ? new(p) ::SimCardDlg : new ::SimCardDlg;
   }
   static void *newArray_SimCardDlg(Long_t nElements, void *p) {
      return p ? new(p) ::SimCardDlg[nElements] : new ::SimCardDlg[nElements];
   }
   // Wrapper around operator delete
   static void delete_SimCardDlg(void *p) {
      delete ((::SimCardDlg*)p);
   }
   static void deleteArray_SimCardDlg(void *p) {
      delete [] ((::SimCardDlg*)p);
   }
   static void destruct_SimCardDlg(void *p) {
      typedef ::SimCardDlg current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SimCardDlg

namespace {
  void TriggerDictionaryInitialization_SimCardDict_Impl() {
    static const char* headers[] = {
"SimCard.h",
"SimCardDlg.h",
0
    };
    static const char* includePaths[] = {
"../../core",
"/home/dlab/Downloads/root-6.05.02/include",
"/home/dlab/dg_epool/Digitizers/digitizer/SIM/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$SimCard.h")))  SimCard;
class __attribute__((annotate("$clingAutoload$SimCard.h")))  SimCardDlg;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "SimCard.h"
#include "SimCardDlg.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"SimCard", payloadCode, "@",
"SimCardDlg", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SimCardDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SimCardDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SimCardDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SimCardDict() {
  TriggerDictionaryInitialization_SimCardDict_Impl();
}
