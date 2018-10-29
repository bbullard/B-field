// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME MuonAnalysisDictReflexDict

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
#include "/afs/cern.ch/user/b/bbullard/B-field/athena/source/MuonAnalysis/MuonAnalysis/MuonAnalysisDict.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *MuonAODAnalysis_Dictionary();
   static void MuonAODAnalysis_TClassManip(TClass*);
   static void delete_MuonAODAnalysis(void *p);
   static void deleteArray_MuonAODAnalysis(void *p);
   static void destruct_MuonAODAnalysis(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MuonAODAnalysis*)
   {
      ::MuonAODAnalysis *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::MuonAODAnalysis));
      static ::ROOT::TGenericClassInfo 
         instance("MuonAODAnalysis", "MuonAODAnalysis.h", 11,
                  typeid(::MuonAODAnalysis), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &MuonAODAnalysis_Dictionary, isa_proxy, 4,
                  sizeof(::MuonAODAnalysis) );
      instance.SetDelete(&delete_MuonAODAnalysis);
      instance.SetDeleteArray(&deleteArray_MuonAODAnalysis);
      instance.SetDestructor(&destruct_MuonAODAnalysis);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MuonAODAnalysis*)
   {
      return GenerateInitInstanceLocal((::MuonAODAnalysis*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::MuonAODAnalysis*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *MuonAODAnalysis_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::MuonAODAnalysis*)0x0)->GetClass();
      MuonAODAnalysis_TClassManip(theClass);
   return theClass;
   }

   static void MuonAODAnalysis_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *MuonSelection_Dictionary();
   static void MuonSelection_TClassManip(TClass*);
   static void delete_MuonSelection(void *p);
   static void deleteArray_MuonSelection(void *p);
   static void destruct_MuonSelection(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MuonSelection*)
   {
      ::MuonSelection *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::MuonSelection));
      static ::ROOT::TGenericClassInfo 
         instance("MuonSelection", "MuonSelection.h", 12,
                  typeid(::MuonSelection), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &MuonSelection_Dictionary, isa_proxy, 4,
                  sizeof(::MuonSelection) );
      instance.SetDelete(&delete_MuonSelection);
      instance.SetDeleteArray(&deleteArray_MuonSelection);
      instance.SetDestructor(&destruct_MuonSelection);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MuonSelection*)
   {
      return GenerateInitInstanceLocal((::MuonSelection*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::MuonSelection*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *MuonSelection_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::MuonSelection*)0x0)->GetClass();
      MuonSelection_TClassManip(theClass);
   return theClass;
   }

   static void MuonSelection_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_MuonAODAnalysis(void *p) {
      delete ((::MuonAODAnalysis*)p);
   }
   static void deleteArray_MuonAODAnalysis(void *p) {
      delete [] ((::MuonAODAnalysis*)p);
   }
   static void destruct_MuonAODAnalysis(void *p) {
      typedef ::MuonAODAnalysis current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MuonAODAnalysis

namespace ROOT {
   // Wrapper around operator delete
   static void delete_MuonSelection(void *p) {
      delete ((::MuonSelection*)p);
   }
   static void deleteArray_MuonSelection(void *p) {
      delete [] ((::MuonSelection*)p);
   }
   static void destruct_MuonSelection(void *p) {
      typedef ::MuonSelection current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MuonSelection

namespace {
  void TriggerDictionaryInitialization_libMuonAnalysisDict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libMuonAnalysisDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$MuonAnalysis/MuonAODAnalysis.h")))  MuonAODAnalysis;
class __attribute__((annotate("$clingAutoload$MuonAnalysis/MuonSelection.h")))  MuonSelection;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libMuonAnalysisDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef HAVE_PRETTY_FUNCTION
  #define HAVE_PRETTY_FUNCTION 1
#endif
#ifndef HAVE_64_BITS
  #define HAVE_64_BITS 1
#endif
#ifndef __IDENTIFIER_64BIT__
  #define __IDENTIFIER_64BIT__ 1
#endif
#ifndef ATLAS
  #define ATLAS 1
#endif
#ifndef GAUDI_V20_COMPAT
  #define GAUDI_V20_COMPAT 1
#endif
#ifndef ATLAS_GAUDI_V21
  #define ATLAS_GAUDI_V21 1
#endif
#ifndef HAVE_GAUDI_PLUGINSVC
  #define HAVE_GAUDI_PLUGINSVC 1
#endif
#ifndef XAOD_ANALYSIS
  #define XAOD_ANALYSIS 1
#endif
#ifndef ROOTCORE_RELEASE_SERIES
  #define ROOTCORE_RELEASE_SERIES 25
#endif
#ifndef PACKAGE_VERSION
  #define PACKAGE_VERSION "MuonAnalysis-00-00-00"
#endif
#ifndef PACKAGE_VERSION_UQ
  #define PACKAGE_VERSION_UQ MuonAnalysis-00-00-00
#endif
#ifndef EIGEN_DONT_VECTORIZE
  #define EIGEN_DONT_VECTORIZE 1
#endif
#ifndef CLHEP_MAX_MIN_DEFINED
  #define CLHEP_MAX_MIN_DEFINED 1
#endif
#ifndef CLHEP_ABS_DEFINED
  #define CLHEP_ABS_DEFINED 1
#endif
#ifndef CLHEP_SQR_DEFINED
  #define CLHEP_SQR_DEFINED 1
#endif
#ifndef CLHEP_MAX_MIN_DEFINED
  #define CLHEP_MAX_MIN_DEFINED 1
#endif
#ifndef CLHEP_ABS_DEFINED
  #define CLHEP_ABS_DEFINED 1
#endif
#ifndef CLHEP_SQR_DEFINED
  #define CLHEP_SQR_DEFINED 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#ifndef MUONANALYSIS_MUONANALYSIS_DICT_H
#define MUONANALYSIS_MUONANALYSIS_DICT_H

// This file includes all the header files that you need to create
// dictionaries for.

#include <MuonAnalysis/MuonAODAnalysis.h>
#include <MuonAnalysis/MuonSelection.h>

#endif

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"MuonAODAnalysis", payloadCode, "@",
"MuonSelection", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libMuonAnalysisDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libMuonAnalysisDict_Impl, {{"namespace DataVector_detail { template <typename B> class DVLEltBase_init; }", 1},{"namespace DataVector_detail { template <typename T> class RegisterDVLEltBaseInit; }", 1},{"namespace DataVector_detail { template <typename T> class DVLEltBaseInit; }", 1},{"namespace DataVector_detail { template <typename B1, typename B2, typename B3> class VirtBases; }", 1},{"template <typename T> class DataVectorBase;", 1},{"template <typename T, typename BASE> class DataVector;", 1}}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libMuonAnalysisDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libMuonAnalysisDict() {
  TriggerDictionaryInitialization_libMuonAnalysisDict_Impl();
}
