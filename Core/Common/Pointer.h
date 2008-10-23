//----------------------------------------------------------------------------------------------
//	Meson::Common::TPointer<T>
//	Templated class for smart pointer.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Definitions.h"
#include "Namespaces.h"
#include "Types.h"

#include "DebugSupport.h"

Meson_Common_BEGIN

//----------------------------------------------------------------------------------------------
template <typename TClass> class TPointer
{
public:
	explicit TPointer( TClass* p_pObject )
	{
		m_pObject = p_pObject;
		
		if (m_pObject != NULL) 
			m_pObject->IncrementReferenceCount();
	}

	explicit TPointer(void)
	{
		m_pObject = NULL;
	}

	TPointer( const TPointer& p_pointer )
	{
		m_pObject = p_pointer.m_pObject;
		
		if (m_pObject != NULL)
			m_pObject->IncrementReferenceCount();
	}

	~TPointer(void) 
	{
		if (m_pObject != NULL)
			m_pObject->DecrementReferenceCount();
	}

	operator void*() const { return (void*)m_pObject; }

	operator TClass*() const { return m_pObject; }
	
	TClass& operator*() const 
	{ 
		MESON_ASSERT(m_pObject != NULL, "NULL Pointer exception!");
		return *m_pObject; 
	}
	
	TClass* operator->() const 
	{
		MESON_ASSERT(m_pObject != NULL, "NULL Pointer exception!");
		return m_pObject; 
	}

	TPointer& operator= (TClass* p_pObject)
	{
		if (p_pObject != m_pObject)
		{
			if (p_pObject != NULL) 
				p_pObject->IncrementReferenceCount();

			if (m_pObject != NULL)
				m_pObject->DecrementReferenceCount();

			m_pObject = p_pObject;
		}

		return *this;
	}

	TPointer& operator= (TPointer& p_pointer)
	{
		if (p_pointer.m_pObject != m_pObject)
		{
			if (p_pointer.m_pObject != NULL)
				p_pointer.m_pObject->IncrementReferenceCount();

			if (m_pObject != NULL)
				m_pObject->DecrementReferenceCount();

			m_pObject = p_pointer.m_pObject;
		}

		return *this;
	}

	bool operator!() const
	{
		return m_pObject == NULL;
	}

	bool operator== (TClass* p_pObject) const 
	{ 
		return m_pObject == p_pObject; 
	}

	bool operator!= (TClass* p_pObject) const 
	{ 
		return m_pObject != p_pObject; 
	}

	bool operator< (TClass* p_pObject) const 
	{ 
		return m_pObject < p_pObject; 
	}

	bool operator<= (TClass* p_pObject) const 
	{ 
		return m_pObject <= p_pObject; 
	}

	bool operator> (TClass* p_pObject) const 
	{ 
		return m_pObject > p_pObject; 
	}

	bool operator>= (TClass* p_pObject) const 
	{ 
		return m_pObject >= p_pObject; 
	}

	bool operator== (const TPointer& p_pointer) const 
	{ 
		return p_pointer.m_pObject == m_pObject; 
	}
    
	bool operator!= (const TPointer& p_pointer) const 
	{ 
		return p_pointer.m_pObject != m_pObject; 
	}

	bool operator< (const TPointer& p_pointer) const 
	{ 
		return p_pointer.m_pObject < m_pObject; 
	}

	bool operator<= (const TPointer& p_pointer) const 
	{ 
		return p_pointer.m_pObject <= m_pObject; 
	}

	bool operator> (const TPointer& p_pointer) const 
	{ 
		return p_pointer.m_pObject > m_pObject; 
	}

	bool operator>= (const TPointer& p_pointer) const 
	{ 
		return p_pointer.m_pObject >= m_pObject; 
	}

	inline friend bool operator== (const TClass* p_pObject, const TPointer& p_pointer)
	{
		return p_pObject == p_pointer.m_pObject;
	}

	inline friend bool operator!= (const TClass* p_pObject, const TPointer& p_pointer)
	{
		return p_pObject != p_pointer.m_pObject;
	}

	inline friend bool operator< (const TClass* p_pObject, const TPointer& p_pointer)
	{
		return p_pObject < p_pointer.m_pObject;
	}

	inline friend bool operator<= (const TClass* p_pObject, const TPointer& p_pointer)
	{
		return p_pObject <= p_pointer.m_pObject;
	}

	inline friend bool operator> (const TClass* p_pObject, const TPointer& p_pointer)
	{
		return p_pObject > p_pointer.m_pObject;
	}

	inline friend bool operator>= (const TClass* p_pObject, const TPointer& p_pointer)
	{
		return p_pObject >= p_pointer.m_pObject;
	}

protected:
	TClass* m_pObject;
};

//----------------------------------------------------------------------------------------------
template <typename TClass> class TReferenceCounter
{
protected:
	int m_nReferenceCount;

public:
	TReferenceCounter(void) : m_nReferenceCount(0) { }
	virtual ~TReferenceCounter(void) { }

	void IncrementReferenceCount(void) { m_nReferenceCount++; }
	int GetReferenceCount(void) { return m_nReferenceCount; }
	void DecrementReferenceCount(void) { if(!--m_nReferenceCount) delete this; }
};

Meson_Common_END
