// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME coreDict

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
#include "ABuildEvent.h"
#include "ADataSave.h"
#include "ADataSave2.h"
#include "AEvent.h"
#include "AManager.h"
#include "ATrack.h"
#include "ARingBuffer.h"
#include "AHelp.h"
#include "ControlAcq.h"
#include "OnlineServer.h"
#include "Digitizer.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void streamer_AManager(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AManager*)
   {
      ::AManager *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AManager >(0);
      static ::ROOT::TGenericClassInfo 
         instance("AManager", ::AManager::Class_Version(), "AManager.h", 35,
                  typeid(::AManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AManager::Dictionary, isa_proxy, 16,
                  sizeof(::AManager) );
      instance.SetStreamerFunc(&streamer_AManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AManager*)
   {
      return GenerateInitInstanceLocal((::AManager*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::AManager*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ABuildEvent(void *p = 0);
   static void *newArray_ABuildEvent(Long_t size, void *p);
   static void delete_ABuildEvent(void *p);
   static void deleteArray_ABuildEvent(void *p);
   static void destruct_ABuildEvent(void *p);
   static void streamer_ABuildEvent(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ABuildEvent*)
   {
      ::ABuildEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ABuildEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ABuildEvent", ::ABuildEvent::Class_Version(), "ABuildEvent.h", 19,
                  typeid(::ABuildEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ABuildEvent::Dictionary, isa_proxy, 16,
                  sizeof(::ABuildEvent) );
      instance.SetNew(&new_ABuildEvent);
      instance.SetNewArray(&newArray_ABuildEvent);
      instance.SetDelete(&delete_ABuildEvent);
      instance.SetDeleteArray(&deleteArray_ABuildEvent);
      instance.SetDestructor(&destruct_ABuildEvent);
      instance.SetStreamerFunc(&streamer_ABuildEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ABuildEvent*)
   {
      return GenerateInitInstanceLocal((::ABuildEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ABuildEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ADataSave(void *p = 0);
   static void *newArray_ADataSave(Long_t size, void *p);
   static void delete_ADataSave(void *p);
   static void deleteArray_ADataSave(void *p);
   static void destruct_ADataSave(void *p);
   static void streamer_ADataSave(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ADataSave*)
   {
      ::ADataSave *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ADataSave >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ADataSave", ::ADataSave::Class_Version(), "ADataSave.h", 21,
                  typeid(::ADataSave), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ADataSave::Dictionary, isa_proxy, 16,
                  sizeof(::ADataSave) );
      instance.SetNew(&new_ADataSave);
      instance.SetNewArray(&newArray_ADataSave);
      instance.SetDelete(&delete_ADataSave);
      instance.SetDeleteArray(&deleteArray_ADataSave);
      instance.SetDestructor(&destruct_ADataSave);
      instance.SetStreamerFunc(&streamer_ADataSave);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ADataSave*)
   {
      return GenerateInitInstanceLocal((::ADataSave*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ADataSave*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ADataSave2(void *p = 0);
   static void *newArray_ADataSave2(Long_t size, void *p);
   static void delete_ADataSave2(void *p);
   static void deleteArray_ADataSave2(void *p);
   static void destruct_ADataSave2(void *p);
   static void streamer_ADataSave2(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ADataSave2*)
   {
      ::ADataSave2 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ADataSave2 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ADataSave2", ::ADataSave2::Class_Version(), "ADataSave2.h", 20,
                  typeid(::ADataSave2), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ADataSave2::Dictionary, isa_proxy, 16,
                  sizeof(::ADataSave2) );
      instance.SetNew(&new_ADataSave2);
      instance.SetNewArray(&newArray_ADataSave2);
      instance.SetDelete(&delete_ADataSave2);
      instance.SetDeleteArray(&deleteArray_ADataSave2);
      instance.SetDestructor(&destruct_ADataSave2);
      instance.SetStreamerFunc(&streamer_ADataSave2);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ADataSave2*)
   {
      return GenerateInitInstanceLocal((::ADataSave2*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ADataSave2*)0x0); R__UseDummy(_R__UNIQUE_(Init));
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

namespace ROOT {
   static void *new_ControlAcq(void *p = 0);
   static void *newArray_ControlAcq(Long_t size, void *p);
   static void delete_ControlAcq(void *p);
   static void deleteArray_ControlAcq(void *p);
   static void destruct_ControlAcq(void *p);
   static void streamer_ControlAcq(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ControlAcq*)
   {
      ::ControlAcq *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ControlAcq >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ControlAcq", ::ControlAcq::Class_Version(), "ControlAcq.h", 15,
                  typeid(::ControlAcq), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ControlAcq::Dictionary, isa_proxy, 16,
                  sizeof(::ControlAcq) );
      instance.SetNew(&new_ControlAcq);
      instance.SetNewArray(&newArray_ControlAcq);
      instance.SetDelete(&delete_ControlAcq);
      instance.SetDeleteArray(&deleteArray_ControlAcq);
      instance.SetDestructor(&destruct_ControlAcq);
      instance.SetStreamerFunc(&streamer_ControlAcq);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ControlAcq*)
   {
      return GenerateInitInstanceLocal((::ControlAcq*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ControlAcq*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Digitizer(void *p = 0);
   static void *newArray_Digitizer(Long_t size, void *p);
   static void delete_Digitizer(void *p);
   static void deleteArray_Digitizer(void *p);
   static void destruct_Digitizer(void *p);
   static void streamer_Digitizer(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Digitizer*)
   {
      ::Digitizer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Digitizer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Digitizer", ::Digitizer::Class_Version(), "Digitizer.h", 23,
                  typeid(::Digitizer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Digitizer::Dictionary, isa_proxy, 16,
                  sizeof(::Digitizer) );
      instance.SetNew(&new_Digitizer);
      instance.SetNewArray(&newArray_Digitizer);
      instance.SetDelete(&delete_Digitizer);
      instance.SetDeleteArray(&deleteArray_Digitizer);
      instance.SetDestructor(&destruct_Digitizer);
      instance.SetStreamerFunc(&streamer_Digitizer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Digitizer*)
   {
      return GenerateInitInstanceLocal((::Digitizer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Digitizer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ARingBuffer(void *p = 0);
   static void *newArray_ARingBuffer(Long_t size, void *p);
   static void delete_ARingBuffer(void *p);
   static void deleteArray_ARingBuffer(void *p);
   static void destruct_ARingBuffer(void *p);
   static void streamer_ARingBuffer(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ARingBuffer*)
   {
      ::ARingBuffer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ARingBuffer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ARingBuffer", ::ARingBuffer::Class_Version(), "ARingBuffer.h", 25,
                  typeid(::ARingBuffer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ARingBuffer::Dictionary, isa_proxy, 16,
                  sizeof(::ARingBuffer) );
      instance.SetNew(&new_ARingBuffer);
      instance.SetNewArray(&newArray_ARingBuffer);
      instance.SetDelete(&delete_ARingBuffer);
      instance.SetDeleteArray(&deleteArray_ARingBuffer);
      instance.SetDestructor(&destruct_ARingBuffer);
      instance.SetStreamerFunc(&streamer_ARingBuffer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ARingBuffer*)
   {
      return GenerateInitInstanceLocal((::ARingBuffer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ARingBuffer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_AHelp(void *p);
   static void deleteArray_AHelp(void *p);
   static void destruct_AHelp(void *p);
   static void streamer_AHelp(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AHelp*)
   {
      ::AHelp *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AHelp >(0);
      static ::ROOT::TGenericClassInfo 
         instance("AHelp", ::AHelp::Class_Version(), "AHelp.h", 11,
                  typeid(::AHelp), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AHelp::Dictionary, isa_proxy, 16,
                  sizeof(::AHelp) );
      instance.SetDelete(&delete_AHelp);
      instance.SetDeleteArray(&deleteArray_AHelp);
      instance.SetDestructor(&destruct_AHelp);
      instance.SetStreamerFunc(&streamer_AHelp);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AHelp*)
   {
      return GenerateInitInstanceLocal((::AHelp*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::AHelp*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_OnlineServer(void *p = 0);
   static void *newArray_OnlineServer(Long_t size, void *p);
   static void delete_OnlineServer(void *p);
   static void deleteArray_OnlineServer(void *p);
   static void destruct_OnlineServer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::OnlineServer*)
   {
      ::OnlineServer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::OnlineServer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("OnlineServer", ::OnlineServer::Class_Version(), "OnlineServer.h", 23,
                  typeid(::OnlineServer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::OnlineServer::Dictionary, isa_proxy, 4,
                  sizeof(::OnlineServer) );
      instance.SetNew(&new_OnlineServer);
      instance.SetNewArray(&newArray_OnlineServer);
      instance.SetDelete(&delete_OnlineServer);
      instance.SetDeleteArray(&deleteArray_OnlineServer);
      instance.SetDestructor(&destruct_OnlineServer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::OnlineServer*)
   {
      return GenerateInitInstanceLocal((::OnlineServer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::OnlineServer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr AManager::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *AManager::Class_Name()
{
   return "AManager";
}

//______________________________________________________________________________
const char *AManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AManager*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int AManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AManager*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AManager*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AManager*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ABuildEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ABuildEvent::Class_Name()
{
   return "ABuildEvent";
}

//______________________________________________________________________________
const char *ABuildEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ABuildEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ABuildEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ABuildEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ABuildEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ABuildEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ABuildEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ABuildEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ADataSave::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ADataSave::Class_Name()
{
   return "ADataSave";
}

//______________________________________________________________________________
const char *ADataSave::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ADataSave*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ADataSave::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ADataSave*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ADataSave::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ADataSave*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ADataSave::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ADataSave*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ADataSave2::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ADataSave2::Class_Name()
{
   return "ADataSave2";
}

//______________________________________________________________________________
const char *ADataSave2::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ADataSave2*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ADataSave2::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ADataSave2*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ADataSave2::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ADataSave2*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ADataSave2::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ADataSave2*)0x0)->GetClass(); }
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
atomic_TClass_ptr ControlAcq::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ControlAcq::Class_Name()
{
   return "ControlAcq";
}

//______________________________________________________________________________
const char *ControlAcq::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ControlAcq*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ControlAcq::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ControlAcq*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ControlAcq::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ControlAcq*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ControlAcq::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ControlAcq*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Digitizer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Digitizer::Class_Name()
{
   return "Digitizer";
}

//______________________________________________________________________________
const char *Digitizer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Digitizer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Digitizer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Digitizer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Digitizer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Digitizer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Digitizer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Digitizer*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ARingBuffer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ARingBuffer::Class_Name()
{
   return "ARingBuffer";
}

//______________________________________________________________________________
const char *ARingBuffer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ARingBuffer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ARingBuffer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ARingBuffer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ARingBuffer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ARingBuffer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ARingBuffer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ARingBuffer*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr AHelp::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *AHelp::Class_Name()
{
   return "AHelp";
}

//______________________________________________________________________________
const char *AHelp::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AHelp*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int AHelp::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AHelp*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AHelp::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AHelp*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AHelp::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AHelp*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr OnlineServer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *OnlineServer::Class_Name()
{
   return "OnlineServer";
}

//______________________________________________________________________________
const char *OnlineServer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::OnlineServer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int OnlineServer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::OnlineServer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *OnlineServer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::OnlineServer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *OnlineServer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::OnlineServer*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void AManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class AManager.

   TQObject::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around a custom streamer member function.
   static void streamer_AManager(TBuffer &buf, void *obj) {
      ((::AManager*)obj)->::AManager::Streamer(buf);
   }
} // end of namespace ROOT for class ::AManager

//______________________________________________________________________________
void ABuildEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class ABuildEvent.

   TQObject::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ABuildEvent(void *p) {
      return  p ? new(p) ::ABuildEvent : new ::ABuildEvent;
   }
   static void *newArray_ABuildEvent(Long_t nElements, void *p) {
      return p ? new(p) ::ABuildEvent[nElements] : new ::ABuildEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_ABuildEvent(void *p) {
      delete ((::ABuildEvent*)p);
   }
   static void deleteArray_ABuildEvent(void *p) {
      delete [] ((::ABuildEvent*)p);
   }
   static void destruct_ABuildEvent(void *p) {
      typedef ::ABuildEvent current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ABuildEvent(TBuffer &buf, void *obj) {
      ((::ABuildEvent*)obj)->::ABuildEvent::Streamer(buf);
   }
} // end of namespace ROOT for class ::ABuildEvent

//______________________________________________________________________________
void ADataSave::Streamer(TBuffer &R__b)
{
   // Stream an object of class ADataSave.

   TObject::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ADataSave(void *p) {
      return  p ? new(p) ::ADataSave : new ::ADataSave;
   }
   static void *newArray_ADataSave(Long_t nElements, void *p) {
      return p ? new(p) ::ADataSave[nElements] : new ::ADataSave[nElements];
   }
   // Wrapper around operator delete
   static void delete_ADataSave(void *p) {
      delete ((::ADataSave*)p);
   }
   static void deleteArray_ADataSave(void *p) {
      delete [] ((::ADataSave*)p);
   }
   static void destruct_ADataSave(void *p) {
      typedef ::ADataSave current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ADataSave(TBuffer &buf, void *obj) {
      ((::ADataSave*)obj)->::ADataSave::Streamer(buf);
   }
} // end of namespace ROOT for class ::ADataSave

//______________________________________________________________________________
void ADataSave2::Streamer(TBuffer &R__b)
{
   // Stream an object of class ADataSave2.

   TObject::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ADataSave2(void *p) {
      return  p ? new(p) ::ADataSave2 : new ::ADataSave2;
   }
   static void *newArray_ADataSave2(Long_t nElements, void *p) {
      return p ? new(p) ::ADataSave2[nElements] : new ::ADataSave2[nElements];
   }
   // Wrapper around operator delete
   static void delete_ADataSave2(void *p) {
      delete ((::ADataSave2*)p);
   }
   static void deleteArray_ADataSave2(void *p) {
      delete [] ((::ADataSave2*)p);
   }
   static void destruct_ADataSave2(void *p) {
      typedef ::ADataSave2 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ADataSave2(TBuffer &buf, void *obj) {
      ((::ADataSave2*)obj)->::ADataSave2::Streamer(buf);
   }
} // end of namespace ROOT for class ::ADataSave2

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
void ControlAcq::Streamer(TBuffer &R__b)
{
   // Stream an object of class ControlAcq.

   TQObject::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ControlAcq(void *p) {
      return  p ? new(p) ::ControlAcq : new ::ControlAcq;
   }
   static void *newArray_ControlAcq(Long_t nElements, void *p) {
      return p ? new(p) ::ControlAcq[nElements] : new ::ControlAcq[nElements];
   }
   // Wrapper around operator delete
   static void delete_ControlAcq(void *p) {
      delete ((::ControlAcq*)p);
   }
   static void deleteArray_ControlAcq(void *p) {
      delete [] ((::ControlAcq*)p);
   }
   static void destruct_ControlAcq(void *p) {
      typedef ::ControlAcq current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ControlAcq(TBuffer &buf, void *obj) {
      ((::ControlAcq*)obj)->::ControlAcq::Streamer(buf);
   }
} // end of namespace ROOT for class ::ControlAcq

//______________________________________________________________________________
void Digitizer::Streamer(TBuffer &R__b)
{
   // Stream an object of class Digitizer.

   TObject::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Digitizer(void *p) {
      return  p ? new(p) ::Digitizer : new ::Digitizer;
   }
   static void *newArray_Digitizer(Long_t nElements, void *p) {
      return p ? new(p) ::Digitizer[nElements] : new ::Digitizer[nElements];
   }
   // Wrapper around operator delete
   static void delete_Digitizer(void *p) {
      delete ((::Digitizer*)p);
   }
   static void deleteArray_Digitizer(void *p) {
      delete [] ((::Digitizer*)p);
   }
   static void destruct_Digitizer(void *p) {
      typedef ::Digitizer current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Digitizer(TBuffer &buf, void *obj) {
      ((::Digitizer*)obj)->::Digitizer::Streamer(buf);
   }
} // end of namespace ROOT for class ::Digitizer

//______________________________________________________________________________
void ARingBuffer::Streamer(TBuffer &R__b)
{
   // Stream an object of class ARingBuffer.

   TObject::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ARingBuffer(void *p) {
      return  p ? new(p) ::ARingBuffer : new ::ARingBuffer;
   }
   static void *newArray_ARingBuffer(Long_t nElements, void *p) {
      return p ? new(p) ::ARingBuffer[nElements] : new ::ARingBuffer[nElements];
   }
   // Wrapper around operator delete
   static void delete_ARingBuffer(void *p) {
      delete ((::ARingBuffer*)p);
   }
   static void deleteArray_ARingBuffer(void *p) {
      delete [] ((::ARingBuffer*)p);
   }
   static void destruct_ARingBuffer(void *p) {
      typedef ::ARingBuffer current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ARingBuffer(TBuffer &buf, void *obj) {
      ((::ARingBuffer*)obj)->::ARingBuffer::Streamer(buf);
   }
} // end of namespace ROOT for class ::ARingBuffer

//______________________________________________________________________________
void AHelp::Streamer(TBuffer &R__b)
{
   // Stream an object of class AHelp.

   TGTransientFrame::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_AHelp(void *p) {
      delete ((::AHelp*)p);
   }
   static void deleteArray_AHelp(void *p) {
      delete [] ((::AHelp*)p);
   }
   static void destruct_AHelp(void *p) {
      typedef ::AHelp current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AHelp(TBuffer &buf, void *obj) {
      ((::AHelp*)obj)->::AHelp::Streamer(buf);
   }
} // end of namespace ROOT for class ::AHelp

//______________________________________________________________________________
void OnlineServer::Streamer(TBuffer &R__b)
{
   // Stream an object of class OnlineServer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(OnlineServer::Class(),this);
   } else {
      R__b.WriteClassBuffer(OnlineServer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_OnlineServer(void *p) {
      return  p ? new(p) ::OnlineServer : new ::OnlineServer;
   }
   static void *newArray_OnlineServer(Long_t nElements, void *p) {
      return p ? new(p) ::OnlineServer[nElements] : new ::OnlineServer[nElements];
   }
   // Wrapper around operator delete
   static void delete_OnlineServer(void *p) {
      delete ((::OnlineServer*)p);
   }
   static void deleteArray_OnlineServer(void *p) {
      delete [] ((::OnlineServer*)p);
   }
   static void destruct_OnlineServer(void *p) {
      typedef ::OnlineServer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::OnlineServer

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
  void TriggerDictionaryInitialization_coreDict_Impl() {
    static const char* headers[] = {
"ABuildEvent.h",
"ADataSave.h",
"ADataSave2.h",
"AEvent.h",
"AManager.h",
"ATrack.h",
"ARingBuffer.h",
"AHelp.h",
"ControlAcq.h",
"OnlineServer.h",
"Digitizer.h",
0
    };
    static const char* includePaths[] = {
"../gui",
"/home/dlab/Downloads/root-6.05.02/include",
"/home/dlab/dg_epool/Digitizers/core/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$AManager.h")))  AManager;
class __attribute__((annotate("$clingAutoload$ABuildEvent.h")))  ABuildEvent;
class __attribute__((annotate(R"ATTRDUMP(ADataSave class)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$ADataSave.h")))  ADataSave;
class __attribute__((annotate(R"ATTRDUMP(ADataSave2 class)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$ADataSave2.h")))  ADataSave2;
class __attribute__((annotate(R"ATTRDUMP(simple event)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$ADataSave.h")))  AEvent;
class __attribute__((annotate(R"ATTRDUMP(simple track)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$ADataSave.h")))  ATrack;
class __attribute__((annotate("$clingAutoload$AManager.h")))  ControlAcq;
class __attribute__((annotate("$clingAutoload$AManager.h")))  Digitizer;
class __attribute__((annotate("$clingAutoload$ADataSave.h")))  ARingBuffer;
class __attribute__((annotate(R"ATTRDUMP(Help Text Viewer)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$AHelp.h")))  AHelp;
class __attribute__((annotate("$clingAutoload$AManager.h")))  OnlineServer;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "ABuildEvent.h"
#include "ADataSave.h"
#include "ADataSave2.h"
#include "AEvent.h"
#include "AManager.h"
#include "ATrack.h"
#include "ARingBuffer.h"
#include "AHelp.h"
#include "ControlAcq.h"
#include "OnlineServer.h"
#include "Digitizer.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ABuildEvent", payloadCode, "@",
"ADataSave", payloadCode, "@",
"ADataSave2", payloadCode, "@",
"AEvent", payloadCode, "@",
"AHelp", payloadCode, "@",
"AManager", payloadCode, "@",
"ARingBuffer", payloadCode, "@",
"ATrack", payloadCode, "@",
"ControlAcq", payloadCode, "@",
"Digitizer", payloadCode, "@",
"OnlineServer", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("coreDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_coreDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_coreDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_coreDict() {
  TriggerDictionaryInitialization_coreDict_Impl();
}
