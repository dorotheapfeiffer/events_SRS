// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME vmm3rootDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
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
#include "RootFile.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_RootFile(void *p);
   static void deleteArray_RootFile(void *p);
   static void destruct_RootFile(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RootFile*)
   {
      ::RootFile *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RootFile >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RootFile", ::RootFile::Class_Version(), "RootFile.h", 22,
                  typeid(::RootFile), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RootFile::Dictionary, isa_proxy, 4,
                  sizeof(::RootFile) );
      instance.SetDelete(&delete_RootFile);
      instance.SetDeleteArray(&deleteArray_RootFile);
      instance.SetDestructor(&destruct_RootFile);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RootFile*)
   {
      return GenerateInitInstanceLocal((::RootFile*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::RootFile*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr RootFile::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RootFile::Class_Name()
{
   return "RootFile";
}

//______________________________________________________________________________
const char *RootFile::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RootFile*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RootFile::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RootFile*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RootFile::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RootFile*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RootFile::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RootFile*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void RootFile::Streamer(TBuffer &R__b)
{
   // Stream an object of class RootFile.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RootFile::Class(),this);
   } else {
      R__b.WriteClassBuffer(RootFile::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_RootFile(void *p) {
      delete ((::RootFile*)p);
   }
   static void deleteArray_RootFile(void *p) {
      delete [] ((::RootFile*)p);
   }
   static void destruct_RootFile(void *p) {
      typedef ::RootFile current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RootFile

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 458,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR_Dictionary();
   static void multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR_TClassManip(TClass*);
   static void *new_multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR(void *p = 0);
   static void *newArray_multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR(Long_t size, void *p);
   static void delete_multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR(void *p);
   static void deleteArray_multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR(void *p);
   static void destruct_multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const multimap<double,pair<int,unsigned int> >*)
   {
      multimap<double,pair<int,unsigned int> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(multimap<double,pair<int,unsigned int> >));
      static ::ROOT::TGenericClassInfo 
         instance("multimap<double,pair<int,unsigned int> >", -2, "map", 1531,
                  typeid(multimap<double,pair<int,unsigned int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(multimap<double,pair<int,unsigned int> >) );
      instance.SetNew(&new_multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR);
      instance.SetNewArray(&newArray_multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR);
      instance.SetDelete(&delete_multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR);
      instance.SetDeleteArray(&deleteArray_multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR);
      instance.SetDestructor(&destruct_multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< multimap<double,pair<int,unsigned int> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const multimap<double,pair<int,unsigned int> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const multimap<double,pair<int,unsigned int> >*)0x0)->GetClass();
      multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) multimap<double,pair<int,unsigned int> > : new multimap<double,pair<int,unsigned int> >;
   }
   static void *newArray_multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) multimap<double,pair<int,unsigned int> >[nElements] : new multimap<double,pair<int,unsigned int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR(void *p) {
      delete ((multimap<double,pair<int,unsigned int> >*)p);
   }
   static void deleteArray_multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR(void *p) {
      delete [] ((multimap<double,pair<int,unsigned int> >*)p);
   }
   static void destruct_multimaplEdoublecOpairlEintcOunsignedsPintgRsPgR(void *p) {
      typedef multimap<double,pair<int,unsigned int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class multimap<double,pair<int,unsigned int> >

namespace ROOT {
   static TClass *multimaplEdoublecOfloatgR_Dictionary();
   static void multimaplEdoublecOfloatgR_TClassManip(TClass*);
   static void *new_multimaplEdoublecOfloatgR(void *p = 0);
   static void *newArray_multimaplEdoublecOfloatgR(Long_t size, void *p);
   static void delete_multimaplEdoublecOfloatgR(void *p);
   static void deleteArray_multimaplEdoublecOfloatgR(void *p);
   static void destruct_multimaplEdoublecOfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const multimap<double,float>*)
   {
      multimap<double,float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(multimap<double,float>));
      static ::ROOT::TGenericClassInfo 
         instance("multimap<double,float>", -2, "map", 1531,
                  typeid(multimap<double,float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &multimaplEdoublecOfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(multimap<double,float>) );
      instance.SetNew(&new_multimaplEdoublecOfloatgR);
      instance.SetNewArray(&newArray_multimaplEdoublecOfloatgR);
      instance.SetDelete(&delete_multimaplEdoublecOfloatgR);
      instance.SetDeleteArray(&deleteArray_multimaplEdoublecOfloatgR);
      instance.SetDestructor(&destruct_multimaplEdoublecOfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< multimap<double,float> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const multimap<double,float>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *multimaplEdoublecOfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const multimap<double,float>*)0x0)->GetClass();
      multimaplEdoublecOfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void multimaplEdoublecOfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_multimaplEdoublecOfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) multimap<double,float> : new multimap<double,float>;
   }
   static void *newArray_multimaplEdoublecOfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) multimap<double,float>[nElements] : new multimap<double,float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_multimaplEdoublecOfloatgR(void *p) {
      delete ((multimap<double,float>*)p);
   }
   static void deleteArray_multimaplEdoublecOfloatgR(void *p) {
      delete [] ((multimap<double,float>*)p);
   }
   static void destruct_multimaplEdoublecOfloatgR(void *p) {
      typedef multimap<double,float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class multimap<double,float>

namespace {
  void TriggerDictionaryInitialization_vmm3rootDict_Impl() {
    static const char* headers[] = {
"RootFile.h",
0
    };
    static const char* includePaths[] = {
"/Applications/root/include",
"/Users/dpfeiffe/programming/ESS/dg_epool/SRS/vmm3root/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "vmm3rootDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$RootFile.h")))  RootFile;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "vmm3rootDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "RootFile.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"RootFile", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("vmm3rootDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_vmm3rootDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_vmm3rootDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_vmm3rootDict() {
  TriggerDictionaryInitialization_vmm3rootDict_Impl();
}
