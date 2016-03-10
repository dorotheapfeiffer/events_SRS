// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME CAEN5740Dict

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
#include "CAEN5740.h"
#include "CAEN5740Dlg.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_CAEN5740(void *p);
   static void deleteArray_CAEN5740(void *p);
   static void destruct_CAEN5740(void *p);
   static void streamer_CAEN5740(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CAEN5740*)
   {
      ::CAEN5740 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CAEN5740 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("CAEN5740", ::CAEN5740::Class_Version(), "CAEN5740.h", 17,
                  typeid(::CAEN5740), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CAEN5740::Dictionary, isa_proxy, 16,
                  sizeof(::CAEN5740) );
      instance.SetDelete(&delete_CAEN5740);
      instance.SetDeleteArray(&deleteArray_CAEN5740);
      instance.SetDestructor(&destruct_CAEN5740);
      instance.SetStreamerFunc(&streamer_CAEN5740);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CAEN5740*)
   {
      return GenerateInitInstanceLocal((::CAEN5740*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::CAEN5740*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CAEN5740Dlg(void *p = 0);
   static void *newArray_CAEN5740Dlg(Long_t size, void *p);
   static void delete_CAEN5740Dlg(void *p);
   static void deleteArray_CAEN5740Dlg(void *p);
   static void destruct_CAEN5740Dlg(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CAEN5740Dlg*)
   {
      ::CAEN5740Dlg *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CAEN5740Dlg >(0);
      static ::ROOT::TGenericClassInfo 
         instance("CAEN5740Dlg", ::CAEN5740Dlg::Class_Version(), "CAEN5740Dlg.h", 23,
                  typeid(::CAEN5740Dlg), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CAEN5740Dlg::Dictionary, isa_proxy, 4,
                  sizeof(::CAEN5740Dlg) );
      instance.SetNew(&new_CAEN5740Dlg);
      instance.SetNewArray(&newArray_CAEN5740Dlg);
      instance.SetDelete(&delete_CAEN5740Dlg);
      instance.SetDeleteArray(&deleteArray_CAEN5740Dlg);
      instance.SetDestructor(&destruct_CAEN5740Dlg);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CAEN5740Dlg*)
   {
      return GenerateInitInstanceLocal((::CAEN5740Dlg*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::CAEN5740Dlg*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CAEN5740::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CAEN5740::Class_Name()
{
   return "CAEN5740";
}

//______________________________________________________________________________
const char *CAEN5740::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CAEN5740*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CAEN5740::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CAEN5740*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CAEN5740::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CAEN5740*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CAEN5740::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CAEN5740*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CAEN5740Dlg::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CAEN5740Dlg::Class_Name()
{
   return "CAEN5740Dlg";
}

//______________________________________________________________________________
const char *CAEN5740Dlg::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CAEN5740Dlg*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CAEN5740Dlg::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CAEN5740Dlg*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CAEN5740Dlg::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CAEN5740Dlg*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CAEN5740Dlg::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CAEN5740Dlg*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CAEN5740::Streamer(TBuffer &R__b)
{
   // Stream an object of class CAEN5740.

   Digitizer::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CAEN5740(void *p) {
      delete ((::CAEN5740*)p);
   }
   static void deleteArray_CAEN5740(void *p) {
      delete [] ((::CAEN5740*)p);
   }
   static void destruct_CAEN5740(void *p) {
      typedef ::CAEN5740 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CAEN5740(TBuffer &buf, void *obj) {
      ((::CAEN5740*)obj)->::CAEN5740::Streamer(buf);
   }
} // end of namespace ROOT for class ::CAEN5740

//______________________________________________________________________________
void CAEN5740Dlg::Streamer(TBuffer &R__b)
{
   // Stream an object of class CAEN5740Dlg.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CAEN5740Dlg::Class(),this);
   } else {
      R__b.WriteClassBuffer(CAEN5740Dlg::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CAEN5740Dlg(void *p) {
      return  p ? new(p) ::CAEN5740Dlg : new ::CAEN5740Dlg;
   }
   static void *newArray_CAEN5740Dlg(Long_t nElements, void *p) {
      return p ? new(p) ::CAEN5740Dlg[nElements] : new ::CAEN5740Dlg[nElements];
   }
   // Wrapper around operator delete
   static void delete_CAEN5740Dlg(void *p) {
      delete ((::CAEN5740Dlg*)p);
   }
   static void deleteArray_CAEN5740Dlg(void *p) {
      delete [] ((::CAEN5740Dlg*)p);
   }
   static void destruct_CAEN5740Dlg(void *p) {
      typedef ::CAEN5740Dlg current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CAEN5740Dlg

namespace {
  void TriggerDictionaryInitialization_CAEN5740Dict_Impl() {
    static const char* headers[] = {
"CAEN5740.h",
"CAEN5740Dlg.h",
0
    };
    static const char* includePaths[] = {
"./",
"../../core",
"/home/dlab/Downloads/root-6.05.02/include",
"/home/dlab/dg_epool/Digitizers/digitizer/CAEN5740/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$CAEN5740.h")))  CAEN5740;
class __attribute__((annotate("$clingAutoload$CAEN5740.h")))  CAEN5740Dlg;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "CAEN5740.h"
#include "CAEN5740Dlg.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"CAEN5740", payloadCode, "@",
"CAEN5740Dlg", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CAEN5740Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CAEN5740Dict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CAEN5740Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CAEN5740Dict() {
  TriggerDictionaryInitialization_CAEN5740Dict_Impl();
}
