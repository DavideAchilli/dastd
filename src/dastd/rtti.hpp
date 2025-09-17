/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 21-AUG-2023
**/
#pragma once
#include "defs.hpp"
#include "hash_crc32.hpp"

#if DASTD_CPP_VER >= 20

//------------------------------------------------------------------
// Body for RTTI base classes
//------------------------------------------------------------------
#define DASTD_RTTI_BASE(CLASSNAME) \
	public: \
		inline static constexpr uint32_t s_class_id = dastd::const_crc32(#CLASSNAME); \
		virtual const char* class_name() const {return #CLASSNAME;} \
		virtual bool derives_from(uint32_t cls_id) const {return cls_id == CLASSNAME::s_class_id;} \
		template <class CLS> bool derives_from() const {return derives_from(CLS::s_class_id);} \
		virtual uint32_t class_id() const {return CLASSNAME::s_class_id;} \
		template<class CLS> CLS* cast() {if(derives_from(CLS::s_class_id)) return static_cast<CLS*>(this); else return nullptr;} \
		template<class CLS> const CLS* cast_const() const {if(derives_from(CLS::s_class_id)) return static_cast<const CLS*>(this); else return nullptr;} \
	private:

//------------------------------------------------------------------
// Body for RTTI derived classes
//------------------------------------------------------------------
#define DASTD_RTTI_DERIVED(CLASSNAME,BASECLASS) \
	public: \
		inline static constexpr uint32_t s_class_id = dastd::const_crc32(#CLASSNAME); \
		virtual const char* class_name() const override {return #CLASSNAME;} \
		virtual bool derives_from(uint32_t cls_id) const override {if (cls_id == CLASSNAME::s_class_id) return true; return BASECLASS::derives_from(cls_id);} \
		template <class CLS> bool derives_from() const {return derives_from(CLS::s_class_id);} \
		virtual uint32_t class_id() const override {return CLASSNAME::s_class_id;} \
	private:

//------------------------------------------------------------------
// Body for pure virtual
//------------------------------------------------------------------
#define DASTD_RTTI_ABSTRACT \
	public:\
		virtual const char* class_name() const = 0;\
		virtual bool derives_from(uint32_t cls_id) const {DASTD_NOWARN_UNUSED(cls_id); return false;} \
		template <class CLS> bool derives_from() const {return derives_from(CLS::s_class_id);} \
		virtual uint32_t class_id() const = 0;\
		template<class CLS> CLS* cast() {if(derives_from(CLS::s_class_id)) return static_cast<CLS*>(this); else return nullptr;} \
		template<class CLS> const CLS* cast_const() const {if(derives_from(CLS::s_class_id)) return static_cast<const CLS*>(this); else return nullptr;} \
	private:


//------------------------------------------------------------------
// Definition for base classes
//------------------------------------------------------------------
#define DASTD_RTTI_1(CLASSNAME) \
	class CLASSNAME { \
		DASTD_RTTI_BASE(CLASSNAME)

//------------------------------------------------------------------
// Definition for base classes with an uninvolved base class
//------------------------------------------------------------------
#define DASTD_RTTI_1_WITH_UNINVOLVED_BASE(CLASSNAME, BASECLASS) \
	class CLASSNAME: public BASECLASS { \
		DASTD_RTTI_BASE(CLASSNAME)

//------------------------------------------------------------------
// Definition for derived classes
//------------------------------------------------------------------
#define DASTD_RTTI_2(CLASSNAME, BASECLASS) \
	class CLASSNAME: public BASECLASS { \
		DASTD_RTTI_DERIVED(CLASSNAME,BASECLASS)

#endif // DASTD_CPP_VER >= 20
