// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME uTPCViewerDict

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
#include "GEM0HitsAnalysis.h"
#include "uTPCAnalysis.h"
#include "uTPCViewer.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_uTPCViewer(void *p);
   static void deleteArray_uTPCViewer(void *p);
   static void destruct_uTPCViewer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::uTPCViewer*)
   {
      ::uTPCViewer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::uTPCViewer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("uTPCViewer", ::uTPCViewer::Class_Version(), "uTPCViewer.h", 22,
                  typeid(::uTPCViewer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::uTPCViewer::Dictionary, isa_proxy, 4,
                  sizeof(::uTPCViewer) );
      instance.SetDelete(&delete_uTPCViewer);
      instance.SetDeleteArray(&deleteArray_uTPCViewer);
      instance.SetDestructor(&destruct_uTPCViewer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::uTPCViewer*)
   {
      return GenerateInitInstanceLocal((::uTPCViewer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::uTPCViewer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_uTPCRawData(void *p = 0);
   static void *newArray_uTPCRawData(Long_t size, void *p);
   static void delete_uTPCRawData(void *p);
   static void deleteArray_uTPCRawData(void *p);
   static void destruct_uTPCRawData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::uTPCRawData*)
   {
      ::uTPCRawData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::uTPCRawData >(0);
      static ::ROOT::TGenericClassInfo 
         instance("uTPCRawData", ::uTPCRawData::Class_Version(), "uTPCAnalysis.h", 48,
                  typeid(::uTPCRawData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::uTPCRawData::Dictionary, isa_proxy, 4,
                  sizeof(::uTPCRawData) );
      instance.SetNew(&new_uTPCRawData);
      instance.SetNewArray(&newArray_uTPCRawData);
      instance.SetDelete(&delete_uTPCRawData);
      instance.SetDeleteArray(&deleteArray_uTPCRawData);
      instance.SetDestructor(&destruct_uTPCRawData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::uTPCRawData*)
   {
      return GenerateInitInstanceLocal((::uTPCRawData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::uTPCRawData*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *uTPCEvent_Dictionary();
   static void uTPCEvent_TClassManip(TClass*);
   static void *new_uTPCEvent(void *p = 0);
   static void *newArray_uTPCEvent(Long_t size, void *p);
   static void delete_uTPCEvent(void *p);
   static void deleteArray_uTPCEvent(void *p);
   static void destruct_uTPCEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::uTPCEvent*)
   {
      ::uTPCEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::uTPCEvent));
      static ::ROOT::TGenericClassInfo 
         instance("uTPCEvent", "uTPCAnalysis.h", 206,
                  typeid(::uTPCEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &uTPCEvent_Dictionary, isa_proxy, 4,
                  sizeof(::uTPCEvent) );
      instance.SetNew(&new_uTPCEvent);
      instance.SetNewArray(&newArray_uTPCEvent);
      instance.SetDelete(&delete_uTPCEvent);
      instance.SetDeleteArray(&deleteArray_uTPCEvent);
      instance.SetDestructor(&destruct_uTPCEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::uTPCEvent*)
   {
      return GenerateInitInstanceLocal((::uTPCEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::uTPCEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *uTPCEvent_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::uTPCEvent*)0x0)->GetClass();
      uTPCEvent_TClassManip(theClass);
   return theClass;
   }

   static void uTPCEvent_TClassManip(TClass* ){
   }

} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr uTPCViewer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *uTPCViewer::Class_Name()
{
   return "uTPCViewer";
}

//______________________________________________________________________________
const char *uTPCViewer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::uTPCViewer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int uTPCViewer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::uTPCViewer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *uTPCViewer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::uTPCViewer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *uTPCViewer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::uTPCViewer*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr uTPCRawData::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *uTPCRawData::Class_Name()
{
   return "uTPCRawData";
}

//______________________________________________________________________________
const char *uTPCRawData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::uTPCRawData*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int uTPCRawData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::uTPCRawData*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *uTPCRawData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::uTPCRawData*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *uTPCRawData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::uTPCRawData*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void uTPCViewer::Streamer(TBuffer &R__b)
{
   // Stream an object of class uTPCViewer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(uTPCViewer::Class(),this);
   } else {
      R__b.WriteClassBuffer(uTPCViewer::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_uTPCViewer(void *p) {
      delete ((::uTPCViewer*)p);
   }
   static void deleteArray_uTPCViewer(void *p) {
      delete [] ((::uTPCViewer*)p);
   }
   static void destruct_uTPCViewer(void *p) {
      typedef ::uTPCViewer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::uTPCViewer

//______________________________________________________________________________
void uTPCRawData::Streamer(TBuffer &R__b)
{
   // Stream an object of class uTPCRawData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(uTPCRawData::Class(),this);
   } else {
      R__b.WriteClassBuffer(uTPCRawData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_uTPCRawData(void *p) {
      return  p ? new(p) ::uTPCRawData : new ::uTPCRawData;
   }
   static void *newArray_uTPCRawData(Long_t nElements, void *p) {
      return p ? new(p) ::uTPCRawData[nElements] : new ::uTPCRawData[nElements];
   }
   // Wrapper around operator delete
   static void delete_uTPCRawData(void *p) {
      delete ((::uTPCRawData*)p);
   }
   static void deleteArray_uTPCRawData(void *p) {
      delete [] ((::uTPCRawData*)p);
   }
   static void destruct_uTPCRawData(void *p) {
      typedef ::uTPCRawData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::uTPCRawData

namespace ROOT {
   // Wrappers around operator new
   static void *new_uTPCEvent(void *p) {
      return  p ? new(p) ::uTPCEvent : new ::uTPCEvent;
   }
   static void *newArray_uTPCEvent(Long_t nElements, void *p) {
      return p ? new(p) ::uTPCEvent[nElements] : new ::uTPCEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_uTPCEvent(void *p) {
      delete ((::uTPCEvent*)p);
   }
   static void deleteArray_uTPCEvent(void *p) {
      delete [] ((::uTPCEvent*)p);
   }
   static void destruct_uTPCEvent(void *p) {
      typedef ::uTPCEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::uTPCEvent

namespace ROOT {
   static TClass *vectorlEunsignedsPintgR_Dictionary();
   static void vectorlEunsignedsPintgR_TClassManip(TClass*);
   static void *new_vectorlEunsignedsPintgR(void *p = 0);
   static void *newArray_vectorlEunsignedsPintgR(Long_t size, void *p);
   static void delete_vectorlEunsignedsPintgR(void *p);
   static void deleteArray_vectorlEunsignedsPintgR(void *p);
   static void destruct_vectorlEunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<unsigned int>*)
   {
      vector<unsigned int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<unsigned int>", -2, "vector", 457,
                  typeid(vector<unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<unsigned int>) );
      instance.SetNew(&new_vectorlEunsignedsPintgR);
      instance.SetNewArray(&newArray_vectorlEunsignedsPintgR);
      instance.SetDelete(&delete_vectorlEunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEunsignedsPintgR);
      instance.SetDestructor(&destruct_vectorlEunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<unsigned int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<unsigned int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<unsigned int>*)0x0)->GetClass();
      vectorlEunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned int> : new vector<unsigned int>;
   }
   static void *newArray_vectorlEunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned int>[nElements] : new vector<unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEunsignedsPintgR(void *p) {
      delete ((vector<unsigned int>*)p);
   }
   static void deleteArray_vectorlEunsignedsPintgR(void *p) {
      delete [] ((vector<unsigned int>*)p);
   }
   static void destruct_vectorlEunsignedsPintgR(void *p) {
      typedef vector<unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<unsigned int>

namespace {
  void TriggerDictionaryInitialization_uTPCViewerDict_Impl() {
    static const char* headers[] = {
"GEM0HitsAnalysis.h",
"uTPCAnalysis.h",
"uTPCViewer.h",
0
    };
    static const char* includePaths[] = {
"/Applications/root/include",
"/Users/dpfeiffe/programming/ESS/dg_epool/SRS/uTPCViewer/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "uTPCViewerDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$uTPCViewer.h")))  uTPCViewer;
class __attribute__((annotate("$clingAutoload$uTPCAnalysis.h")))  uTPCRawData;
class __attribute__((annotate("$clingAutoload$uTPCAnalysis.h")))  uTPCEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "uTPCViewerDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "GEM0HitsAnalysis.h"
#include "uTPCAnalysis.h"
#include "uTPCViewer.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"uTPCEvent", payloadCode, "@",
"uTPCRawData", payloadCode, "@",
"uTPCViewer", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("uTPCViewerDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_uTPCViewerDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_uTPCViewerDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_uTPCViewerDict() {
  TriggerDictionaryInitialization_uTPCViewerDict_Impl();
}
