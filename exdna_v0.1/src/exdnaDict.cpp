// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME exdnaDict

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
#include "MainWindow.h"
#include "/home/dlab/dg_epool/Digitizers/exdna_v0.1/app/UserClass.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_MainWindow(void *p = 0);
   static void *newArray_MainWindow(Long_t size, void *p);
   static void delete_MainWindow(void *p);
   static void deleteArray_MainWindow(void *p);
   static void destruct_MainWindow(void *p);
   static void streamer_MainWindow(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MainWindow*)
   {
      ::MainWindow *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MainWindow >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MainWindow", ::MainWindow::Class_Version(), "MainWindow.h", 39,
                  typeid(::MainWindow), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MainWindow::Dictionary, isa_proxy, 16,
                  sizeof(::MainWindow) );
      instance.SetNew(&new_MainWindow);
      instance.SetNewArray(&newArray_MainWindow);
      instance.SetDelete(&delete_MainWindow);
      instance.SetDeleteArray(&deleteArray_MainWindow);
      instance.SetDestructor(&destruct_MainWindow);
      instance.SetStreamerFunc(&streamer_MainWindow);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MainWindow*)
   {
      return GenerateInitInstanceLocal((::MainWindow*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MainWindow*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_UserClass(void *p);
   static void deleteArray_UserClass(void *p);
   static void destruct_UserClass(void *p);
   static void streamer_UserClass(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::UserClass*)
   {
      ::UserClass *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::UserClass >(0);
      static ::ROOT::TGenericClassInfo 
         instance("UserClass", ::UserClass::Class_Version(), "invalid", 14,
                  typeid(::UserClass), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::UserClass::Dictionary, isa_proxy, 16,
                  sizeof(::UserClass) );
      instance.SetDelete(&delete_UserClass);
      instance.SetDeleteArray(&deleteArray_UserClass);
      instance.SetDestructor(&destruct_UserClass);
      instance.SetStreamerFunc(&streamer_UserClass);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::UserClass*)
   {
      return GenerateInitInstanceLocal((::UserClass*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::UserClass*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr MainWindow::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MainWindow::Class_Name()
{
   return "MainWindow";
}

//______________________________________________________________________________
const char *MainWindow::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MainWindow*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MainWindow::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MainWindow*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MainWindow::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MainWindow*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MainWindow::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MainWindow*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr UserClass::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *UserClass::Class_Name()
{
   return "UserClass";
}

//______________________________________________________________________________
const char *UserClass::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::UserClass*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int UserClass::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::UserClass*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *UserClass::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::UserClass*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *UserClass::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::UserClass*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void MainWindow::Streamer(TBuffer &R__b)
{
   // Stream an object of class MainWindow.

   TGMainFrame::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MainWindow(void *p) {
      return  p ? new(p) ::MainWindow : new ::MainWindow;
   }
   static void *newArray_MainWindow(Long_t nElements, void *p) {
      return p ? new(p) ::MainWindow[nElements] : new ::MainWindow[nElements];
   }
   // Wrapper around operator delete
   static void delete_MainWindow(void *p) {
      delete ((::MainWindow*)p);
   }
   static void deleteArray_MainWindow(void *p) {
      delete [] ((::MainWindow*)p);
   }
   static void destruct_MainWindow(void *p) {
      typedef ::MainWindow current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_MainWindow(TBuffer &buf, void *obj) {
      ((::MainWindow*)obj)->::MainWindow::Streamer(buf);
   }
} // end of namespace ROOT for class ::MainWindow

//______________________________________________________________________________
void UserClass::Streamer(TBuffer &R__b)
{
   // Stream an object of class UserClass.

   TObject::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_UserClass(void *p) {
      delete ((::UserClass*)p);
   }
   static void deleteArray_UserClass(void *p) {
      delete [] ((::UserClass*)p);
   }
   static void destruct_UserClass(void *p) {
      typedef ::UserClass current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_UserClass(TBuffer &buf, void *obj) {
      ((::UserClass*)obj)->::UserClass::Streamer(buf);
   }
} // end of namespace ROOT for class ::UserClass

namespace {
  void TriggerDictionaryInitialization_exdnaDict_Impl() {
    static const char* headers[] = {
"MainWindow.h",
"/home/dlab/dg_epool/Digitizers/exdna_v0.1/app/UserClass.h",
0
    };
    static const char* includePaths[] = {
"/home/dlab/Downloads/root-6.05.02/include",
"/home/dlab/dg_epool/Digitizers/exdna_v0.1/src/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$MainWindow.h")))  MainWindow;
class __attribute__((annotate("$clingAutoload$/home/dlab/dg_epool/Digitizers/exdna_v0.1/app/UserClass.h")))  UserClass;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "MainWindow.h"
#include "/home/dlab/dg_epool/Digitizers/exdna_v0.1/app/UserClass.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"MainWindow", payloadCode, "@",
"UserClass", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("exdnaDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_exdnaDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_exdnaDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_exdnaDict() {
  TriggerDictionaryInitialization_exdnaDict_Impl();
}
