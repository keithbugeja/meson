#pragma once

#include "Types.h"
#include "HashMap.h"

#include "VstTypes.h"

Meson_Vistas_BEGIN
	//----------------------------------------------------------------------------------------------
	/** Type used in Vistas' Object type system.
	 */ 
	//----------------------------------------------------------------------------------------------
	typedef Meson::Common::Text::String VistasType;

	//----------------------------------------------------------------------------------------------
	/** This macro is used to enable runtime type information within objects derived from the
		VistasObject base class. The P_TYPE parameter should be set to the name of the class
		within which the macro is called, while P_BASE should be set to the name of the base 
		class from which the class inherits. Note that this system does not handle multiple 
		inheritance.
	 */
	//----------------------------------------------------------------------------------------------
	#define VISTAS_IMPLEMENT_RTTI(P_TYPE, P_BASE) \
	public:																				\
	static VistasType& GetStaticType(void)												\
	{																					\
		static VistasType type = #P_TYPE;												\
		return type;																	\
	}																					\
																						\
	const VistasType& GetBaseType(void) const											\
	{																					\
		return P_BASE::GetInstanceType();												\
	}																					\
																						\
	const VistasType& GetInstanceType(void) const										\
	{																					\
		return GetStaticType();															\
	}																					\
																						\
	bool IsExact(const VistasType& p_type) const										\
	{																					\
		return (P_TYPE::GetInstanceType().Compare(p_type) == 0);						\
	}																					\
																						\
	bool IsDerived(const VistasType& p_type) const										\
	{																					\
		if (P_TYPE::IsExact(p_type))													\
			return true;																\
																						\
		return P_BASE::IsDerived(p_type);												\
	}																					\
																						\
	bool IsExactTypeOf(const VistasObject* p_pObject) const								\
	{																					\
		return (P_TYPE::GetInstanceType().Compare(p_pObject->GetInstanceType()) == 0);	\
	}																					\
																						\
	bool IsDerivedTypeOf(const VistasObject* p_pObject) const							\
	{																					\
		if (P_TYPE::IsExactTypeOf(p_pObject))											\
			return true;																\
																						\
		return P_BASE::IsDerivedTypeOf(p_pObject);										\
	}																					
																						
	//----------------------------------------------------------------------------------------------
	/** All objects desiring to make use of the Vistas runtime typing, should derive from 
		the VistasObject base class and initialise it through the VISTAS_DECLARE_RTTI macro.
	 */
	//----------------------------------------------------------------------------------------------
	typedef Meson::Common::Collections::TEnumerator<void*>& UserDataEnumerator;

	class VistasObject : public Meson::Common::TReferenceCounter<VistasObject>
	{
	protected:
		VistasObject();

		explicit VistasObject(bool p_bAutogenerateId);
		explicit VistasObject(const Meson::Common::Text::String& p_strId);

	public:		
		virtual ~VistasObject();

		const Meson::Common::Text::String& GetId(void) const;
		void SetId(const Meson::Common::Text::String& p_strId);

		static  const VistasType& GetStaticType(void);
		virtual const VistasType& GetBaseType(void) const;
		virtual const VistasType& GetInstanceType(void) const;

		virtual bool IsDerived(const VistasType& p_type) const;
		virtual bool IsDerivedTypeOf(const VistasObject* p_pObject) const;

		virtual bool IsExact(const VistasType& p_type) const;
		virtual bool IsExactTypeOf(const VistasObject* p_pObject) const;

		virtual Meson::Common::TPointer<VistasObject> GetObjectPtr(void);
		virtual Meson::Common::TPointer<VistasObject> Clone(bool p_bShallow = true);
		virtual Meson::Common::TPointer<VistasObject> CreateInstance(void);

		void SetUserData(void* p_pData);
		void SetUserData(uint p_uiSlot, void* p_pData);
		
		void* GetUserData(void) const;
		void* GetUserData(uint p_uiSlot) const;

		uint GetUserDataCount(void) const;
		UserDataEnumerator GetUserDataEnumerator();

	protected:
		Meson::Common::Text::String m_strId;
		Meson::Common::Collections::THashMap<int, void*> m_userDataMap;
	};

	//----------------------------------------------------------------------------------------------
	/** Global casting operations for VistasObject-derived classes.
	 */ 
	//----------------------------------------------------------------------------------------------
	template <class TType> TType* StaticCast(VistasObject* p_pObject) 
	{ 
		return(TType*)p_pObject; 
	}

	template <class TType> const TType* StaticCast(const VistasObject* p_pObject) 
	{ 
		return(const TType*)p_pObject; 
	}

	template <class TType> TType* DynamicCast(VistasObject* p_pObject) 
	{
		if (p_pObject == NULL)
			return NULL;

		return p_pObject->IsDerived(TType::GetStaticType()) ? (TType*)p_pObject : NULL; 
	}

	template <class TType> const TType* DynamicCast(const VistasObject* p_pObject) 
	{
		if (p_pObject == NULL)
			return NULL;

		return p_pObject->IsDerived(TType::GetStaticType()) ? (const TType*)p_pObject : NULL; 
	}

	typedef Meson::Common::TPointer<VistasObject> VistasObjectPtr;
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END

