// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dataDict

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
#include "ReadData.h"
#include "ReadDataFile.h"
#include "ReadDataNet.h"
#include "Files.h"
#include "AEvent.h"
#include "ATrack.h"
#include "ASignal.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void streamer_Files(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Files*)
   {
      ::Files *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Files >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Files", ::Files::Class_Version(), "Files.h", 9,
                  typeid(::Files), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Files::Dictionary, isa_proxy, 16,
                  sizeof(::Files) );
      instance.SetStreamerFunc(&streamer_Files);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Files*)
   {
      return GenerateInitInstanceLocal((::Files*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Files*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ASignal(void *p = 0);
   static void *newArray_ASignal(Long_t size, void *p);
   static void delete_ASignal(void *p);
   static void deleteArray_ASignal(void *p);
   static void destruct_ASignal(void *p);
   static void directoryAutoAdd_ASignal(void *obj, TDirectory *dir);
   static Long64_t merge_ASignal(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ASignal*)
   {
      ::ASignal *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ASignal >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ASignal", ::ASignal::Class_Version(), "ASignal.h", 19,
                  typeid(::ASignal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ASignal::Dictionary, isa_proxy, 4,
                  sizeof(::ASignal) );
      instance.SetNew(&new_ASignal);
      instance.SetNewArray(&newArray_ASignal);
      instance.SetDelete(&delete_ASignal);
      instance.SetDeleteArray(&deleteArray_ASignal);
      instance.SetDestructor(&destruct_ASignal);
      instance.SetDirectoryAutoAdd(&directoryAutoAdd_ASignal);
      instance.SetMerge(&merge_ASignal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ASignal*)
   {
      return GenerateInitInstanceLocal((::ASignal*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ASignal*)0x0); R__UseDummy(_R__UNIQUE_(Init));
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
         instance("AEvent", ::AEvent::Class_Version(), "AEvent.h", 19,
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

namespace ROOT {
   static void *new_ReadData(void *p = 0);
   static void *newArray_ReadData(Long_t size, void *p);
   static void delete_ReadData(void *p);
   static void deleteArray_ReadData(void *p);
   static void destruct_ReadData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ReadData*)
   {
      ::ReadData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ReadData >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ReadData", ::ReadData::Class_Version(), "ReadData.h", 21,
                  typeid(::ReadData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ReadData::Dictionary, isa_proxy, 4,
                  sizeof(::ReadData) );
      instance.SetNew(&new_ReadData);
      instance.SetNewArray(&newArray_ReadData);
      instance.SetDelete(&delete_ReadData);
      instance.SetDeleteArray(&deleteArray_ReadData);
      instance.SetDestructor(&destruct_ReadData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ReadData*)
   {
      return GenerateInitInstanceLocal((::ReadData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ReadData*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ReadDataFile*)
   {
      ::ReadDataFile *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ReadDataFile >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ReadDataFile", ::ReadDataFile::Class_Version(), "ReadDataFile.h", 6,
                  typeid(::ReadDataFile), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ReadDataFile::Dictionary, isa_proxy, 4,
                  sizeof(::ReadDataFile) );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ReadDataFile*)
   {
      return GenerateInitInstanceLocal((::ReadDataFile*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ReadDataFile*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ReadDataNet*)
   {
      ::ReadDataNet *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ReadDataNet >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ReadDataNet", ::ReadDataNet::Class_Version(), "ReadDataNet.h", 7,
                  typeid(::ReadDataNet), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ReadDataNet::Dictionary, isa_proxy, 4,
                  sizeof(::ReadDataNet) );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ReadDataNet*)
   {
      return GenerateInitInstanceLocal((::ReadDataNet*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ReadDataNet*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Files::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Files::Class_Name()
{
   return "Files";
}

//______________________________________________________________________________
const char *Files::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Files*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Files::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Files*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Files::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Files*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Files::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Files*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ASignal::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ASignal::Class_Name()
{
   return "ASignal";
}

//______________________________________________________________________________
const char *ASignal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ASignal*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ASignal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ASignal*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ASignal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ASignal*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ASignal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ASignal*)0x0)->GetClass(); }
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
atomic_TClass_ptr ReadData::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ReadData::Class_Name()
{
   return "ReadData";
}

//______________________________________________________________________________
const char *ReadData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ReadData*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ReadData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ReadData*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ReadData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ReadData*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ReadData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ReadData*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ReadDataFile::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ReadDataFile::Class_Name()
{
   return "ReadDataFile";
}

//______________________________________________________________________________
const char *ReadDataFile::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ReadDataFile*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ReadDataFile::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ReadDataFile*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ReadDataFile::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ReadDataFile*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ReadDataFile::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ReadDataFile*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ReadDataNet::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ReadDataNet::Class_Name()
{
   return "ReadDataNet";
}

//______________________________________________________________________________
const char *ReadDataNet::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ReadDataNet*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ReadDataNet::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ReadDataNet*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ReadDataNet::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ReadDataNet*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ReadDataNet::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ReadDataNet*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Files::Streamer(TBuffer &R__b)
{
   // Stream an object of class Files.

   TObject::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around a custom streamer member function.
   static void streamer_Files(TBuffer &buf, void *obj) {
      ((::Files*)obj)->::Files::Streamer(buf);
   }
} // end of namespace ROOT for class ::Files

//______________________________________________________________________________
void ASignal::Streamer(TBuffer &R__b)
{
   // Stream an object of class ASignal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ASignal::Class(),this);
   } else {
      R__b.WriteClassBuffer(ASignal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ASignal(void *p) {
      return  p ? new(p) ::ASignal : new ::ASignal;
   }
   static void *newArray_ASignal(Long_t nElements, void *p) {
      return p ? new(p) ::ASignal[nElements] : new ::ASignal[nElements];
   }
   // Wrapper around operator delete
   static void delete_ASignal(void *p) {
      delete ((::ASignal*)p);
   }
   static void deleteArray_ASignal(void *p) {
      delete [] ((::ASignal*)p);
   }
   static void destruct_ASignal(void *p) {
      typedef ::ASignal current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the directory auto add.
   static void directoryAutoAdd_ASignal(void *p, TDirectory *dir) {
      ((::ASignal*)p)->DirectoryAutoAdd(dir);
   }
   // Wrapper around the merge function.
   static Long64_t  merge_ASignal(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::ASignal*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::ASignal

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

//______________________________________________________________________________
void ReadData::Streamer(TBuffer &R__b)
{
   // Stream an object of class ReadData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ReadData::Class(),this);
   } else {
      R__b.WriteClassBuffer(ReadData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ReadData(void *p) {
      return  p ? new(p) ::ReadData : new ::ReadData;
   }
   static void *newArray_ReadData(Long_t nElements, void *p) {
      return p ? new(p) ::ReadData[nElements] : new ::ReadData[nElements];
   }
   // Wrapper around operator delete
   static void delete_ReadData(void *p) {
      delete ((::ReadData*)p);
   }
   static void deleteArray_ReadData(void *p) {
      delete [] ((::ReadData*)p);
   }
   static void destruct_ReadData(void *p) {
      typedef ::ReadData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ReadData

//______________________________________________________________________________
void ReadDataFile::Streamer(TBuffer &R__b)
{
   // Stream an object of class ReadDataFile.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ReadDataFile::Class(),this);
   } else {
      R__b.WriteClassBuffer(ReadDataFile::Class(),this);
   }
}

namespace ROOT {
} // end of namespace ROOT for class ::ReadDataFile

//______________________________________________________________________________
void ReadDataNet::Streamer(TBuffer &R__b)
{
   // Stream an object of class ReadDataNet.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ReadDataNet::Class(),this);
   } else {
      R__b.WriteClassBuffer(ReadDataNet::Class(),this);
   }
}

namespace ROOT {
} // end of namespace ROOT for class ::ReadDataNet

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
  void TriggerDictionaryInitialization_dataDict_Impl() {
    static const char* headers[] = {
"ReadData.h",
"ReadDataFile.h",
"ReadDataNet.h",
"Files.h",
"AEvent.h",
"ATrack.h",
"ASignal.h",
0
    };
    static const char* includePaths[] = {
"/home/dlab/Downloads/root-6.05.02/include",
"/home/dlab/Documents/programC/backup/exdna.dev.07Mar2016_noworking/lib/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$Files.h")))  Files;
class __attribute__((annotate(R"ATTRDUMP(extended TH1F class for signal processing)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$ASignal.h")))  ASignal;
class __attribute__((annotate(R"ATTRDUMP(simple event)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$ReadData.h")))  AEvent;
class __attribute__((annotate(R"ATTRDUMP(simple track)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$ReadData.h")))  ATrack;
class __attribute__((annotate("$clingAutoload$ReadData.h")))  ReadData;
class __attribute__((annotate("$clingAutoload$ReadDataFile.h")))  ReadDataFile;
class __attribute__((annotate("$clingAutoload$ReadDataNet.h")))  ReadDataNet;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "ReadData.h"
#include "ReadDataFile.h"
#include "ReadDataNet.h"
#include "Files.h"
#include "AEvent.h"
#include "ATrack.h"
#include "ASignal.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"AEvent", payloadCode, "@",
"ASignal", payloadCode, "@",
"ATrack", payloadCode, "@",
"Files", payloadCode, "@",
"ReadData", payloadCode, "@",
"ReadDataFile", payloadCode, "@",
"ReadDataNet", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("dataDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_dataDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_dataDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_dataDict() {
  TriggerDictionaryInitialization_dataDict_Impl();
}
