// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME zabaDict

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
#include "DGHelp.h"
#include "DGModule.h"
#include "DModule.h"
#include "DGMultiGrid.h"
#include "DMultiGrid.h"
#include "DMadc32.h"
#include "DGMadc32.h"
#include "DV1718.h"
#include "DGV1718.h"
#include "DGDisplay.h"
#include "DDataSave.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_DGHelp(void *p);
   static void deleteArray_DGHelp(void *p);
   static void destruct_DGHelp(void *p);
   static void streamer_DGHelp(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DGHelp*)
   {
      ::DGHelp *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DGHelp >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DGHelp", ::DGHelp::Class_Version(), "DGHelp.h", 13,
                  typeid(::DGHelp), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DGHelp::Dictionary, isa_proxy, 16,
                  sizeof(::DGHelp) );
      instance.SetDelete(&delete_DGHelp);
      instance.SetDeleteArray(&deleteArray_DGHelp);
      instance.SetDestructor(&destruct_DGHelp);
      instance.SetStreamerFunc(&streamer_DGHelp);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DGHelp*)
   {
      return GenerateInitInstanceLocal((::DGHelp*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DGHelp*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_DGModule(void *p);
   static void deleteArray_DGModule(void *p);
   static void destruct_DGModule(void *p);
   static void streamer_DGModule(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DGModule*)
   {
      ::DGModule *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DGModule >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DGModule", ::DGModule::Class_Version(), "DGModule.h", 26,
                  typeid(::DGModule), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DGModule::Dictionary, isa_proxy, 16,
                  sizeof(::DGModule) );
      instance.SetDelete(&delete_DGModule);
      instance.SetDeleteArray(&deleteArray_DGModule);
      instance.SetDestructor(&destruct_DGModule);
      instance.SetStreamerFunc(&streamer_DGModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DGModule*)
   {
      return GenerateInitInstanceLocal((::DGModule*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DGModule*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DModule(void *p = 0);
   static void *newArray_DModule(Long_t size, void *p);
   static void delete_DModule(void *p);
   static void deleteArray_DModule(void *p);
   static void destruct_DModule(void *p);
   static void streamer_DModule(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DModule*)
   {
      ::DModule *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DModule >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DModule", ::DModule::Class_Version(), "DModule.h", 23,
                  typeid(::DModule), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DModule::Dictionary, isa_proxy, 16,
                  sizeof(::DModule) );
      instance.SetNew(&new_DModule);
      instance.SetNewArray(&newArray_DModule);
      instance.SetDelete(&delete_DModule);
      instance.SetDeleteArray(&deleteArray_DModule);
      instance.SetDestructor(&destruct_DModule);
      instance.SetStreamerFunc(&streamer_DModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DModule*)
   {
      return GenerateInitInstanceLocal((::DModule*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DModule*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DMultiGrid(void *p = 0);
   static void *newArray_DMultiGrid(Long_t size, void *p);
   static void delete_DMultiGrid(void *p);
   static void deleteArray_DMultiGrid(void *p);
   static void destruct_DMultiGrid(void *p);
   static void streamer_DMultiGrid(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DMultiGrid*)
   {
      ::DMultiGrid *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DMultiGrid >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DMultiGrid", ::DMultiGrid::Class_Version(), "DMultiGrid.h", 14,
                  typeid(::DMultiGrid), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DMultiGrid::Dictionary, isa_proxy, 16,
                  sizeof(::DMultiGrid) );
      instance.SetNew(&new_DMultiGrid);
      instance.SetNewArray(&newArray_DMultiGrid);
      instance.SetDelete(&delete_DMultiGrid);
      instance.SetDeleteArray(&deleteArray_DMultiGrid);
      instance.SetDestructor(&destruct_DMultiGrid);
      instance.SetStreamerFunc(&streamer_DMultiGrid);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DMultiGrid*)
   {
      return GenerateInitInstanceLocal((::DMultiGrid*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DMultiGrid*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DGMultiGrid(void *p = 0);
   static void *newArray_DGMultiGrid(Long_t size, void *p);
   static void delete_DGMultiGrid(void *p);
   static void deleteArray_DGMultiGrid(void *p);
   static void destruct_DGMultiGrid(void *p);
   static void streamer_DGMultiGrid(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DGMultiGrid*)
   {
      ::DGMultiGrid *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DGMultiGrid >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DGMultiGrid", ::DGMultiGrid::Class_Version(), "DGMultiGrid.h", 26,
                  typeid(::DGMultiGrid), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DGMultiGrid::Dictionary, isa_proxy, 16,
                  sizeof(::DGMultiGrid) );
      instance.SetNew(&new_DGMultiGrid);
      instance.SetNewArray(&newArray_DGMultiGrid);
      instance.SetDelete(&delete_DGMultiGrid);
      instance.SetDeleteArray(&deleteArray_DGMultiGrid);
      instance.SetDestructor(&destruct_DGMultiGrid);
      instance.SetStreamerFunc(&streamer_DGMultiGrid);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DGMultiGrid*)
   {
      return GenerateInitInstanceLocal((::DGMultiGrid*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DGMultiGrid*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_DGMadc32(void *p);
   static void deleteArray_DGMadc32(void *p);
   static void destruct_DGMadc32(void *p);
   static void streamer_DGMadc32(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DGMadc32*)
   {
      ::DGMadc32 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DGMadc32 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DGMadc32", ::DGMadc32::Class_Version(), "DGMadc32.h", 8,
                  typeid(::DGMadc32), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DGMadc32::Dictionary, isa_proxy, 16,
                  sizeof(::DGMadc32) );
      instance.SetDelete(&delete_DGMadc32);
      instance.SetDeleteArray(&deleteArray_DGMadc32);
      instance.SetDestructor(&destruct_DGMadc32);
      instance.SetStreamerFunc(&streamer_DGMadc32);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DGMadc32*)
   {
      return GenerateInitInstanceLocal((::DGMadc32*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DGMadc32*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DMadc32(void *p = 0);
   static void *newArray_DMadc32(Long_t size, void *p);
   static void delete_DMadc32(void *p);
   static void deleteArray_DMadc32(void *p);
   static void destruct_DMadc32(void *p);
   static void streamer_DMadc32(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DMadc32*)
   {
      ::DMadc32 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DMadc32 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DMadc32", ::DMadc32::Class_Version(), "DMadc32.h", 10,
                  typeid(::DMadc32), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DMadc32::Dictionary, isa_proxy, 16,
                  sizeof(::DMadc32) );
      instance.SetNew(&new_DMadc32);
      instance.SetNewArray(&newArray_DMadc32);
      instance.SetDelete(&delete_DMadc32);
      instance.SetDeleteArray(&deleteArray_DMadc32);
      instance.SetDestructor(&destruct_DMadc32);
      instance.SetStreamerFunc(&streamer_DMadc32);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DMadc32*)
   {
      return GenerateInitInstanceLocal((::DMadc32*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DMadc32*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DV1718(void *p = 0);
   static void *newArray_DV1718(Long_t size, void *p);
   static void delete_DV1718(void *p);
   static void deleteArray_DV1718(void *p);
   static void destruct_DV1718(void *p);
   static void streamer_DV1718(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DV1718*)
   {
      ::DV1718 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DV1718 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DV1718", ::DV1718::Class_Version(), "DV1718.h", 10,
                  typeid(::DV1718), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DV1718::Dictionary, isa_proxy, 16,
                  sizeof(::DV1718) );
      instance.SetNew(&new_DV1718);
      instance.SetNewArray(&newArray_DV1718);
      instance.SetDelete(&delete_DV1718);
      instance.SetDeleteArray(&deleteArray_DV1718);
      instance.SetDestructor(&destruct_DV1718);
      instance.SetStreamerFunc(&streamer_DV1718);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DV1718*)
   {
      return GenerateInitInstanceLocal((::DV1718*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DV1718*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_DGV1718(void *p);
   static void deleteArray_DGV1718(void *p);
   static void destruct_DGV1718(void *p);
   static void streamer_DGV1718(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DGV1718*)
   {
      ::DGV1718 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DGV1718 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DGV1718", ::DGV1718::Class_Version(), "DGV1718.h", 7,
                  typeid(::DGV1718), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DGV1718::Dictionary, isa_proxy, 16,
                  sizeof(::DGV1718) );
      instance.SetDelete(&delete_DGV1718);
      instance.SetDeleteArray(&deleteArray_DGV1718);
      instance.SetDestructor(&destruct_DGV1718);
      instance.SetStreamerFunc(&streamer_DGV1718);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DGV1718*)
   {
      return GenerateInitInstanceLocal((::DGV1718*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DGV1718*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DGDisplay(void *p = 0);
   static void *newArray_DGDisplay(Long_t size, void *p);
   static void delete_DGDisplay(void *p);
   static void deleteArray_DGDisplay(void *p);
   static void destruct_DGDisplay(void *p);
   static void streamer_DGDisplay(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DGDisplay*)
   {
      ::DGDisplay *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DGDisplay >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DGDisplay", ::DGDisplay::Class_Version(), "DGDisplay.h", 15,
                  typeid(::DGDisplay), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DGDisplay::Dictionary, isa_proxy, 16,
                  sizeof(::DGDisplay) );
      instance.SetNew(&new_DGDisplay);
      instance.SetNewArray(&newArray_DGDisplay);
      instance.SetDelete(&delete_DGDisplay);
      instance.SetDeleteArray(&deleteArray_DGDisplay);
      instance.SetDestructor(&destruct_DGDisplay);
      instance.SetStreamerFunc(&streamer_DGDisplay);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DGDisplay*)
   {
      return GenerateInitInstanceLocal((::DGDisplay*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DGDisplay*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DDataSave(void *p = 0);
   static void *newArray_DDataSave(Long_t size, void *p);
   static void delete_DDataSave(void *p);
   static void deleteArray_DDataSave(void *p);
   static void destruct_DDataSave(void *p);
   static void streamer_DDataSave(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DDataSave*)
   {
      ::DDataSave *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DDataSave >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DDataSave", ::DDataSave::Class_Version(), "DDataSave.h", 6,
                  typeid(::DDataSave), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DDataSave::Dictionary, isa_proxy, 16,
                  sizeof(::DDataSave) );
      instance.SetNew(&new_DDataSave);
      instance.SetNewArray(&newArray_DDataSave);
      instance.SetDelete(&delete_DDataSave);
      instance.SetDeleteArray(&deleteArray_DDataSave);
      instance.SetDestructor(&destruct_DDataSave);
      instance.SetStreamerFunc(&streamer_DDataSave);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DDataSave*)
   {
      return GenerateInitInstanceLocal((::DDataSave*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DDataSave*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr DGHelp::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DGHelp::Class_Name()
{
   return "DGHelp";
}

//______________________________________________________________________________
const char *DGHelp::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DGHelp*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DGHelp::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DGHelp*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DGHelp::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DGHelp*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DGHelp::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DGHelp*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DGModule::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DGModule::Class_Name()
{
   return "DGModule";
}

//______________________________________________________________________________
const char *DGModule::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DGModule*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DGModule::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DGModule*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DGModule::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DGModule*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DGModule::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DGModule*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DModule::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DModule::Class_Name()
{
   return "DModule";
}

//______________________________________________________________________________
const char *DModule::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DModule*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DModule::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DModule*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DModule::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DModule*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DModule::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DModule*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DMultiGrid::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DMultiGrid::Class_Name()
{
   return "DMultiGrid";
}

//______________________________________________________________________________
const char *DMultiGrid::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DMultiGrid*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DMultiGrid::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DMultiGrid*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DMultiGrid::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DMultiGrid*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DMultiGrid::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DMultiGrid*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DGMultiGrid::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DGMultiGrid::Class_Name()
{
   return "DGMultiGrid";
}

//______________________________________________________________________________
const char *DGMultiGrid::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DGMultiGrid*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DGMultiGrid::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DGMultiGrid*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DGMultiGrid::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DGMultiGrid*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DGMultiGrid::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DGMultiGrid*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DGMadc32::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DGMadc32::Class_Name()
{
   return "DGMadc32";
}

//______________________________________________________________________________
const char *DGMadc32::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DGMadc32*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DGMadc32::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DGMadc32*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DGMadc32::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DGMadc32*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DGMadc32::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DGMadc32*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DMadc32::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DMadc32::Class_Name()
{
   return "DMadc32";
}

//______________________________________________________________________________
const char *DMadc32::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DMadc32*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DMadc32::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DMadc32*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DMadc32::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DMadc32*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DMadc32::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DMadc32*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DV1718::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DV1718::Class_Name()
{
   return "DV1718";
}

//______________________________________________________________________________
const char *DV1718::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DV1718*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DV1718::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DV1718*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DV1718::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DV1718*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DV1718::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DV1718*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DGV1718::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DGV1718::Class_Name()
{
   return "DGV1718";
}

//______________________________________________________________________________
const char *DGV1718::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DGV1718*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DGV1718::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DGV1718*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DGV1718::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DGV1718*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DGV1718::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DGV1718*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DGDisplay::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DGDisplay::Class_Name()
{
   return "DGDisplay";
}

//______________________________________________________________________________
const char *DGDisplay::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DGDisplay*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DGDisplay::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DGDisplay*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DGDisplay::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DGDisplay*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DGDisplay::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DGDisplay*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DDataSave::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DDataSave::Class_Name()
{
   return "DDataSave";
}

//______________________________________________________________________________
const char *DDataSave::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DDataSave*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DDataSave::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DDataSave*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DDataSave::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DDataSave*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DDataSave::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DDataSave*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void DGHelp::Streamer(TBuffer &R__b)
{
   // Stream an object of class DGHelp.

   TGTransientFrame::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_DGHelp(void *p) {
      delete ((::DGHelp*)p);
   }
   static void deleteArray_DGHelp(void *p) {
      delete [] ((::DGHelp*)p);
   }
   static void destruct_DGHelp(void *p) {
      typedef ::DGHelp current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DGHelp(TBuffer &buf, void *obj) {
      ((::DGHelp*)obj)->::DGHelp::Streamer(buf);
   }
} // end of namespace ROOT for class ::DGHelp

//______________________________________________________________________________
void DGModule::Streamer(TBuffer &R__b)
{
   // Stream an object of class DGModule.

   TGMainFrame::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_DGModule(void *p) {
      delete ((::DGModule*)p);
   }
   static void deleteArray_DGModule(void *p) {
      delete [] ((::DGModule*)p);
   }
   static void destruct_DGModule(void *p) {
      typedef ::DGModule current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DGModule(TBuffer &buf, void *obj) {
      ((::DGModule*)obj)->::DGModule::Streamer(buf);
   }
} // end of namespace ROOT for class ::DGModule

//______________________________________________________________________________
void DModule::Streamer(TBuffer &R__b)
{
   // Stream an object of class DModule.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b.ReadStaticArray((char*)fGenTypeDescription);
      R__b.ReadStaticArray((char*)fActTypeDescription);
      R__b.ReadStaticArray((char*)fModuleDescription);
      R__b >> fAdBase;
      R__b >> fDummy;
      R__b.CheckByteCount(R__s, R__c, DModule::IsA());
   } else {
      R__c = R__b.WriteVersion(DModule::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b.WriteArray(fGenTypeDescription, 80);
      R__b.WriteArray(fActTypeDescription, 80);
      R__b.WriteArray(fModuleDescription, 80);
      R__b << fAdBase;
      R__b << fDummy;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DModule(void *p) {
      return  p ? new(p) ::DModule : new ::DModule;
   }
   static void *newArray_DModule(Long_t nElements, void *p) {
      return p ? new(p) ::DModule[nElements] : new ::DModule[nElements];
   }
   // Wrapper around operator delete
   static void delete_DModule(void *p) {
      delete ((::DModule*)p);
   }
   static void deleteArray_DModule(void *p) {
      delete [] ((::DModule*)p);
   }
   static void destruct_DModule(void *p) {
      typedef ::DModule current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DModule(TBuffer &buf, void *obj) {
      ((::DModule*)obj)->::DModule::Streamer(buf);
   }
} // end of namespace ROOT for class ::DModule

//______________________________________________________________________________
void DMultiGrid::Streamer(TBuffer &R__b)
{
   // Stream an object of class DMultiGrid.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> fModuleList;
      R__b >> m_Timerout;
      R__b >> m_Status;
      R__b >> m_Display;
      R__b >> m_HistogramSave;
      R__b >> m_DataSave;
      R__b >> m_AcqStatusEntry1;
      R__b >> m_AcqStatusEntry2;
      R__b >> m_AcqStatusEntry3;
      R__b >> m_AcqStatusEntry4;
      R__b >> m_StopAfterSecEntry;
      R__b >> m_StopAfterFileEntry;
      R__b >> m_StopAfterEventsEntry;
      R__b >> m_SaveFileSizeCB;
      R__b >> m_SaveFileSizeEntry;
      R__b >> m_SaveFileTimeCB;
      R__b >> m_SaveFileTimeEntry;
      R__b >> m_nrFile;
      R__b >> m_PrevEvent;
      R__b >> m_PrevSize;
      m_StartAcqTime.Streamer(R__b);
      m_EndAcqTime.Streamer(R__b);
      R__b >> m_ElapsedAcqTime;
      R__b >> m_PrevAcqTime;
      R__b >> m_NrOfSavedFiles;
      R__b.ReadStaticArray((char*)m_FileName);
      R__b >> fDMadc32;
      R__b >> fDV1718;
      R__b.CheckByteCount(R__s, R__c, DMultiGrid::IsA());
   } else {
      R__c = R__b.WriteVersion(DMultiGrid::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << fModuleList;
      R__b << m_Timerout;
      R__b << m_Status;
      R__b << m_Display;
      R__b << m_HistogramSave;
      R__b << m_DataSave;
      R__b << m_AcqStatusEntry1;
      R__b << m_AcqStatusEntry2;
      R__b << m_AcqStatusEntry3;
      R__b << m_AcqStatusEntry4;
      R__b << m_StopAfterSecEntry;
      R__b << m_StopAfterFileEntry;
      R__b << m_StopAfterEventsEntry;
      R__b << m_SaveFileSizeCB;
      R__b << m_SaveFileSizeEntry;
      R__b << m_SaveFileTimeCB;
      R__b << m_SaveFileTimeEntry;
      R__b << m_nrFile;
      R__b << m_PrevEvent;
      R__b << m_PrevSize;
      m_StartAcqTime.Streamer(R__b);
      m_EndAcqTime.Streamer(R__b);
      R__b << m_ElapsedAcqTime;
      R__b << m_PrevAcqTime;
      R__b << m_NrOfSavedFiles;
      R__b.WriteArray(m_FileName, 256);
      R__b << fDMadc32;
      R__b << fDV1718;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DMultiGrid(void *p) {
      return  p ? new(p) ::DMultiGrid : new ::DMultiGrid;
   }
   static void *newArray_DMultiGrid(Long_t nElements, void *p) {
      return p ? new(p) ::DMultiGrid[nElements] : new ::DMultiGrid[nElements];
   }
   // Wrapper around operator delete
   static void delete_DMultiGrid(void *p) {
      delete ((::DMultiGrid*)p);
   }
   static void deleteArray_DMultiGrid(void *p) {
      delete [] ((::DMultiGrid*)p);
   }
   static void destruct_DMultiGrid(void *p) {
      typedef ::DMultiGrid current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DMultiGrid(TBuffer &buf, void *obj) {
      ((::DMultiGrid*)obj)->::DMultiGrid::Streamer(buf);
   }
} // end of namespace ROOT for class ::DMultiGrid

//______________________________________________________________________________
void DGMultiGrid::Streamer(TBuffer &R__b)
{
   // Stream an object of class DGMultiGrid.

   TGMainFrame::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DGMultiGrid(void *p) {
      return  p ? new(p) ::DGMultiGrid : new ::DGMultiGrid;
   }
   static void *newArray_DGMultiGrid(Long_t nElements, void *p) {
      return p ? new(p) ::DGMultiGrid[nElements] : new ::DGMultiGrid[nElements];
   }
   // Wrapper around operator delete
   static void delete_DGMultiGrid(void *p) {
      delete ((::DGMultiGrid*)p);
   }
   static void deleteArray_DGMultiGrid(void *p) {
      delete [] ((::DGMultiGrid*)p);
   }
   static void destruct_DGMultiGrid(void *p) {
      typedef ::DGMultiGrid current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DGMultiGrid(TBuffer &buf, void *obj) {
      ((::DGMultiGrid*)obj)->::DGMultiGrid::Streamer(buf);
   }
} // end of namespace ROOT for class ::DGMultiGrid

//______________________________________________________________________________
void DGMadc32::Streamer(TBuffer &R__b)
{
   // Stream an object of class DGMadc32.

   DGModule::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_DGMadc32(void *p) {
      delete ((::DGMadc32*)p);
   }
   static void deleteArray_DGMadc32(void *p) {
      delete [] ((::DGMadc32*)p);
   }
   static void destruct_DGMadc32(void *p) {
      typedef ::DGMadc32 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DGMadc32(TBuffer &buf, void *obj) {
      ((::DGMadc32*)obj)->::DGMadc32::Streamer(buf);
   }
} // end of namespace ROOT for class ::DGMadc32

//______________________________________________________________________________
void DMadc32::Streamer(TBuffer &R__b)
{
   // Stream an object of class DMadc32.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      DModule::Streamer(R__b);
      void *ptr_m_VMEBridge = (void*)&m_VMEBridge;
      R__b >> *reinterpret_cast<Int_t*>(ptr_m_VMEBridge);
      R__b >> m_Link;
      R__b >> m_Device;
      R__b >> m_Handle;
      R__b >> m_BaseAddress;
      R__b >> m_ModuleID;
      R__b.ReadStaticArray((unsigned int*)m_Buffer);
      R__b.ReadStaticArray((unsigned int*)m_localBuffer);
      R__b >> m_BufferPos;
      R__b >> m_BufferSize;
      R__b >> m_dataSizeByte;
      R__b >> m_Events;
      R__b >> m_IgnoreThreshold;
      R__b.ReadStaticArray((int*)m_ThresholdOn);
      R__b.ReadStaticArray((int*)m_ThresholdValue);
      R__b.ReadStaticArray((int*)m_ThresholdCache);
      R__b >> m_GateGenerator;
      R__b >> m_GateDelay;
      R__b >> m_GateWidth;
      R__b >> m_GateOutput;
      R__b >> m_ADCResolution;
      R__b.CheckByteCount(R__s, R__c, DMadc32::IsA());
   } else {
      R__c = R__b.WriteVersion(DMadc32::IsA(), kTRUE);
      DModule::Streamer(R__b);
      R__b << (Int_t)m_VMEBridge;
      R__b << m_Link;
      R__b << m_Device;
      R__b << m_Handle;
      R__b << m_BaseAddress;
      R__b << m_ModuleID;
      R__b.WriteArray(m_Buffer, 849346560);
      R__b.WriteArray(m_localBuffer, 10485760);
      R__b << m_BufferPos;
      R__b << m_BufferSize;
      R__b << m_dataSizeByte;
      R__b << m_Events;
      R__b << m_IgnoreThreshold;
      R__b.WriteArray(m_ThresholdOn, 32);
      R__b.WriteArray(m_ThresholdValue, 32);
      R__b.WriteArray(m_ThresholdCache, 32);
      R__b << m_GateGenerator;
      R__b << m_GateDelay;
      R__b << m_GateWidth;
      R__b << m_GateOutput;
      R__b << m_ADCResolution;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DMadc32(void *p) {
      return  p ? new(p) ::DMadc32 : new ::DMadc32;
   }
   static void *newArray_DMadc32(Long_t nElements, void *p) {
      return p ? new(p) ::DMadc32[nElements] : new ::DMadc32[nElements];
   }
   // Wrapper around operator delete
   static void delete_DMadc32(void *p) {
      delete ((::DMadc32*)p);
   }
   static void deleteArray_DMadc32(void *p) {
      delete [] ((::DMadc32*)p);
   }
   static void destruct_DMadc32(void *p) {
      typedef ::DMadc32 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DMadc32(TBuffer &buf, void *obj) {
      ((::DMadc32*)obj)->::DMadc32::Streamer(buf);
   }
} // end of namespace ROOT for class ::DMadc32

//______________________________________________________________________________
void DV1718::Streamer(TBuffer &R__b)
{
   // Stream an object of class DV1718.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      DModule::Streamer(R__b);
      R__b >> m_VMEBridge;
      R__b >> m_Input;
      R__b >> m_EndCounts;
      R__b >> m_CountOutput;
      R__b >> m_Value;
      R__b.CheckByteCount(R__s, R__c, DV1718::IsA());
   } else {
      R__c = R__b.WriteVersion(DV1718::IsA(), kTRUE);
      DModule::Streamer(R__b);
      R__b << m_VMEBridge;
      R__b << m_Input;
      R__b << m_EndCounts;
      R__b << m_CountOutput;
      R__b << m_Value;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DV1718(void *p) {
      return  p ? new(p) ::DV1718 : new ::DV1718;
   }
   static void *newArray_DV1718(Long_t nElements, void *p) {
      return p ? new(p) ::DV1718[nElements] : new ::DV1718[nElements];
   }
   // Wrapper around operator delete
   static void delete_DV1718(void *p) {
      delete ((::DV1718*)p);
   }
   static void deleteArray_DV1718(void *p) {
      delete [] ((::DV1718*)p);
   }
   static void destruct_DV1718(void *p) {
      typedef ::DV1718 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DV1718(TBuffer &buf, void *obj) {
      ((::DV1718*)obj)->::DV1718::Streamer(buf);
   }
} // end of namespace ROOT for class ::DV1718

//______________________________________________________________________________
void DGV1718::Streamer(TBuffer &R__b)
{
   // Stream an object of class DGV1718.

   DGModule::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_DGV1718(void *p) {
      delete ((::DGV1718*)p);
   }
   static void deleteArray_DGV1718(void *p) {
      delete [] ((::DGV1718*)p);
   }
   static void destruct_DGV1718(void *p) {
      typedef ::DGV1718 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DGV1718(TBuffer &buf, void *obj) {
      ((::DGV1718*)obj)->::DGV1718::Streamer(buf);
   }
} // end of namespace ROOT for class ::DGV1718

//______________________________________________________________________________
void DGDisplay::Streamer(TBuffer &R__b)
{
   // Stream an object of class DGDisplay.

   ::Error("DGDisplay::Streamer", "version id <=0 in ClassDef, dummy Streamer() called"); if (R__b.IsReading()) { }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DGDisplay(void *p) {
      return  p ? new(p) ::DGDisplay : new ::DGDisplay;
   }
   static void *newArray_DGDisplay(Long_t nElements, void *p) {
      return p ? new(p) ::DGDisplay[nElements] : new ::DGDisplay[nElements];
   }
   // Wrapper around operator delete
   static void delete_DGDisplay(void *p) {
      delete ((::DGDisplay*)p);
   }
   static void deleteArray_DGDisplay(void *p) {
      delete [] ((::DGDisplay*)p);
   }
   static void destruct_DGDisplay(void *p) {
      typedef ::DGDisplay current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DGDisplay(TBuffer &buf, void *obj) {
      ((::DGDisplay*)obj)->::DGDisplay::Streamer(buf);
   }
} // end of namespace ROOT for class ::DGDisplay

//______________________________________________________________________________
void DDataSave::Streamer(TBuffer &R__b)
{
   // Stream an object of class DDataSave.

   ::Error("DDataSave::Streamer", "version id <=0 in ClassDef, dummy Streamer() called"); if (R__b.IsReading()) { }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DDataSave(void *p) {
      return  p ? new(p) ::DDataSave : new ::DDataSave;
   }
   static void *newArray_DDataSave(Long_t nElements, void *p) {
      return p ? new(p) ::DDataSave[nElements] : new ::DDataSave[nElements];
   }
   // Wrapper around operator delete
   static void delete_DDataSave(void *p) {
      delete ((::DDataSave*)p);
   }
   static void deleteArray_DDataSave(void *p) {
      delete [] ((::DDataSave*)p);
   }
   static void destruct_DDataSave(void *p) {
      typedef ::DDataSave current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DDataSave(TBuffer &buf, void *obj) {
      ((::DDataSave*)obj)->::DDataSave::Streamer(buf);
   }
} // end of namespace ROOT for class ::DDataSave

namespace {
  void TriggerDictionaryInitialization_zabaDict_Impl() {
    static const char* headers[] = {
"DGHelp.h",
"DGModule.h",
"DModule.h",
"DGMultiGrid.h",
"DMultiGrid.h",
"DMadc32.h",
"DGMadc32.h",
"DV1718.h",
"DGV1718.h",
"DGDisplay.h",
"DDataSave.h",
0
    };
    static const char* includePaths[] = {
"/opt/root/include",
"/home/admin/hg/dg_epool/Madc32/zaba/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Help Text Viewer)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$DGHelp.h")))  DGHelp;
class __attribute__((annotate(R"ATTRDUMP(GUI for DModule)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$DGModule.h")))  DGModule;
class __attribute__((annotate(R"ATTRDUMP(Parent to all VME module classes)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$DGModule.h")))  DModule;
class __attribute__((annotate(R"ATTRDUMP(MultiGrid readout)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$DGMultiGrid.h")))  DMultiGrid;
class __attribute__((annotate(R"ATTRDUMP(GUI for DTrigger)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$DGMultiGrid.h")))  DGMultiGrid;
class __attribute__((annotate(R"ATTRDUMP(GUI for DMadc32)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$DGMultiGrid.h")))  DGMadc32;
class __attribute__((annotate(R"ATTRDUMP(Mesytec Pulse Sensitive Digitizer MADC-32)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$DGMultiGrid.h")))  DMadc32;
class __attribute__((annotate(R"ATTRDUMP(CANE Bridge VME-USB)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$DGMultiGrid.h")))  DV1718;
class __attribute__((annotate(R"ATTRDUMP(GUI for DMadc32)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$DGMultiGrid.h")))  DGV1718;
class __attribute__((annotate(R"ATTRDUMP(GUI for Online Display monitoring)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$DGMultiGrid.h")))  DGDisplay;
class __attribute__((annotate(R"ATTRDUMP(class controlon save data to file)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$DGMultiGrid.h")))  DDataSave;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef LINUX
  #define LINUX 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "DGHelp.h"
#include "DGModule.h"
#include "DModule.h"
#include "DGMultiGrid.h"
#include "DMultiGrid.h"
#include "DMadc32.h"
#include "DGMadc32.h"
#include "DV1718.h"
#include "DGV1718.h"
#include "DGDisplay.h"
#include "DDataSave.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"DDataSave", payloadCode, "@",
"DGDisplay", payloadCode, "@",
"DGHelp", payloadCode, "@",
"DGMadc32", payloadCode, "@",
"DGModule", payloadCode, "@",
"DGMultiGrid", payloadCode, "@",
"DGV1718", payloadCode, "@",
"DMadc32", payloadCode, "@",
"DModule", payloadCode, "@",
"DMultiGrid", payloadCode, "@",
"DV1718", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("zabaDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_zabaDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_zabaDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_zabaDict() {
  TriggerDictionaryInitialization_zabaDict_Impl();
}
