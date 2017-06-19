// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME mySpyDict

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
#include "mySpy.h"
#include "ATrack.h"
#include "AEvent.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_mySpy(void *p = 0);
   static void *newArray_mySpy(Long_t size, void *p);
   static void delete_mySpy(void *p);
   static void deleteArray_mySpy(void *p);
   static void destruct_mySpy(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::mySpy*)
   {
      ::mySpy *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::mySpy >(0);
      static ::ROOT::TGenericClassInfo 
         instance("mySpy", ::mySpy::Class_Version(), "mySpy.h", 21,
                  typeid(::mySpy), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::mySpy::Dictionary, isa_proxy, 4,
                  sizeof(::mySpy) );
      instance.SetNew(&new_mySpy);
      instance.SetNewArray(&newArray_mySpy);
      instance.SetDelete(&delete_mySpy);
      instance.SetDeleteArray(&deleteArray_mySpy);
      instance.SetDestructor(&destruct_mySpy);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::mySpy*)
   {
      return GenerateInitInstanceLocal((::mySpy*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::mySpy*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AEvent(void *p = 0);
   static void *newArray_AEvent(Long_t size, void *p);
   static void delete_AEvent(void *p);
   static void deleteArray_AEvent(void *p);
   static void destruct_AEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AEvent*)
   {
      ::AEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("AEvent", ::AEvent::Class_Version(), "AEvent.h", 20,
                  typeid(::AEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AEvent::Dictionary, isa_proxy, 4,
                  sizeof(::AEvent) );
      instance.SetNew(&new_AEvent);
      instance.SetNewArray(&newArray_AEvent);
      instance.SetDelete(&delete_AEvent);
      instance.SetDeleteArray(&deleteArray_AEvent);
      instance.SetDestructor(&destruct_AEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AEvent*)
   {
      return GenerateInitInstanceLocal((::AEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::AEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ATrack(void *p = 0);
   static void *newArray_ATrack(Long_t size, void *p);
   static void delete_ATrack(void *p);
   static void deleteArray_ATrack(void *p);
   static void destruct_ATrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ATrack*)
   {
      ::ATrack *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ATrack >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ATrack", ::ATrack::Class_Version(), "ATrack.h", 16,
                  typeid(::ATrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ATrack::Dictionary, isa_proxy, 4,
                  sizeof(::ATrack) );
      instance.SetNew(&new_ATrack);
      instance.SetNewArray(&newArray_ATrack);
      instance.SetDelete(&delete_ATrack);
      instance.SetDeleteArray(&deleteArray_ATrack);
      instance.SetDestructor(&destruct_ATrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ATrack*)
   {
      return GenerateInitInstanceLocal((::ATrack*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ATrack*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr mySpy::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *mySpy::Class_Name()
{
   return "mySpy";
}

//______________________________________________________________________________
const char *mySpy::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mySpy*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int mySpy::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mySpy*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *mySpy::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mySpy*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *mySpy::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mySpy*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr AEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *AEvent::Class_Name()
{
   return "AEvent";
}

//______________________________________________________________________________
const char *AEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int AEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ATrack::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ATrack::Class_Name()
{
   return "ATrack";
}

//______________________________________________________________________________
const char *ATrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ATrack*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ATrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ATrack*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ATrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ATrack*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ATrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ATrack*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void mySpy::Streamer(TBuffer &R__b)
{
   // Stream an object of class mySpy.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(mySpy::Class(),this);
   } else {
      R__b.WriteClassBuffer(mySpy::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_mySpy(void *p) {
      return  p ? new(p) ::mySpy : new ::mySpy;
   }
   static void *newArray_mySpy(Long_t nElements, void *p) {
      return p ? new(p) ::mySpy[nElements] : new ::mySpy[nElements];
   }
   // Wrapper around operator delete
   static void delete_mySpy(void *p) {
      delete ((::mySpy*)p);
   }
   static void deleteArray_mySpy(void *p) {
      delete [] ((::mySpy*)p);
   }
   static void destruct_mySpy(void *p) {
      typedef ::mySpy current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::mySpy

//______________________________________________________________________________
void AEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class AEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(AEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_AEvent(void *p) {
      return  p ? new(p) ::AEvent : new ::AEvent;
   }
   static void *newArray_AEvent(Long_t nElements, void *p) {
      return p ? new(p) ::AEvent[nElements] : new ::AEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_AEvent(void *p) {
      delete ((::AEvent*)p);
   }
   static void deleteArray_AEvent(void *p) {
      delete [] ((::AEvent*)p);
   }
   static void destruct_AEvent(void *p) {
      typedef ::AEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AEvent

//______________________________________________________________________________
void ATrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class ATrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ATrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(ATrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ATrack(void *p) {
      return  p ? new(p) ::ATrack : new ::ATrack;
   }
   static void *newArray_ATrack(Long_t nElements, void *p) {
      return p ? new(p) ::ATrack[nElements] : new ::ATrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_ATrack(void *p) {
      delete ((::ATrack*)p);
   }
   static void deleteArray_ATrack(void *p) {
      delete [] ((::ATrack*)p);
   }
   static void destruct_ATrack(void *p) {
      typedef ::ATrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ATrack

namespace ROOT {
   static TClass *vectorlEATrackmUgR_Dictionary();
   static void vectorlEATrackmUgR_TClassManip(TClass*);
   static void *new_vectorlEATrackmUgR(void *p = 0);
   static void *newArray_vectorlEATrackmUgR(Long_t size, void *p);
   static void delete_vectorlEATrackmUgR(void *p);
   static void deleteArray_vectorlEATrackmUgR(void *p);
   static void destruct_vectorlEATrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ATrack*>*)
   {
      vector<ATrack*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ATrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ATrack*>", -2, "vector", 210,
                  typeid(vector<ATrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEATrackmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ATrack*>) );
      instance.SetNew(&new_vectorlEATrackmUgR);
      instance.SetNewArray(&newArray_vectorlEATrackmUgR);
      instance.SetDelete(&delete_vectorlEATrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEATrackmUgR);
      instance.SetDestructor(&destruct_vectorlEATrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ATrack*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<ATrack*>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEATrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<ATrack*>*)0x0)->GetClass();
      vectorlEATrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEATrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEATrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ATrack*> : new vector<ATrack*>;
   }
   static void *newArray_vectorlEATrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ATrack*>[nElements] : new vector<ATrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEATrackmUgR(void *p) {
      delete ((vector<ATrack*>*)p);
   }
   static void deleteArray_vectorlEATrackmUgR(void *p) {
      delete [] ((vector<ATrack*>*)p);
   }
   static void destruct_vectorlEATrackmUgR(void *p) {
      typedef vector<ATrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<ATrack*>

namespace {
  void TriggerDictionaryInitialization_mySpyDict_Impl() {
    static const char* headers[] = {
"mySpy.h",
"ATrack.h",
"AEvent.h",
0
    };
    static const char* includePaths[] = {
"../core",
"/home/dlab/Downloads/root-6.05.02/include",
"/home/dlab/dg_epool/Digitizers/online/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(online client)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$mySpy.h")))  mySpy;
class __attribute__((annotate(R"ATTRDUMP(simple event)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$mySpy.h")))  AEvent;
class __attribute__((annotate(R"ATTRDUMP(simple track)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$mySpy.h")))  ATrack;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "mySpy.h"
#include "ATrack.h"
#include "AEvent.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"AEvent", payloadCode, "@",
"ATrack", payloadCode, "@",
"mySpy", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("mySpyDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_mySpyDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_mySpyDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_mySpyDict() {
  TriggerDictionaryInitialization_mySpyDict_Impl();
}
