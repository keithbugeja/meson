#pragma once

#include "Set.h"
#include "HashMap.h"

Meson_Common_Collections_BEGIN

Meson::Common::Memory::MemoryManager *GetHashSetMemoryManager(void);

template<class TElement>
class THashSet
	: public TSet<TElement>
{
private: // private variables
	THashMap<TElement, char> m_hashMap;

public:
	THashSet(void)
		: m_hashMap()
	{
	}

	THashSet(const TImmutableSet<TElement>& p_set)
		: m_hashMap()
	{
		InsertSet(p_set);
	}

	THashSet(const TSet<TElement>& p_set)
		: m_hashMap()
	{
		InsertSet(p_set);
	}

	THashSet(const THashSet& p_set)
		: m_hashMap(p_set.m_hashMap)
	{
	}

	~THashSet(void)
	{
	}

	void* operator new(size_t p_unSize)
	{
		return GetHashSetMemoryManager()->AllocatePooled(p_unSize);
	}

	void operator delete(void* p_pObject, size_t p_unSize)
	{
		::operator delete(p_pObject);
	}							

	bool ContainsElement(const TElement& p_element) const
	{
		return m_hashMap.ContainsKey(p_element);
	}

	bool Intersects(const TImmutableSet<TElement>& p_immutableSet) const
	{
		// no intersection if either set is empty
		size_t unSizeThis = Size();
		if (unSizeThis == 0)
			return false;

		size_t unSizeOther = p_immutableSet.Size();
		if (unSizeOther == 0)
			return false;

		// test elements of smaller set for membership against larger set
		if (unSizeOther < unSizeThis)
		{
			// other set is smaller
			TEnumerator<TElement>::Ptr penumElements
				= ((TImmutableSet<TElement>&) p_immutableSet).CreateEnumerator();
			while (penumElements->HasMoreElements())
			{
				TElement& element = penumElements->NextElement();
				if (ContainsElement(element))
					return true;
			}
		}
		else
		{
			// this set is smaller
			TEnumerator<TElement>::Ptr penumElements
				= ((THashSet<TElement>*) this)->CreateEnumerator();
			while (penumElements->HasMoreElements())
			{
				TElement& element = penumElements->NextElement();
				if (p_immutableSet.ContainsElement(element))
					return true;
			}
		}

		// if this point reached, no intersection
		return false;
	}

	size_t Size(void) const
	{
		return m_hashMap.Size();
	}

	TEnumerator<TElement>& GetEnumerator(bool p_bReset = true)
	{
		return m_hashMap.GetKeyEnumerator(p_bReset);
	}

	inline typename TEnumerator<TElement>::Ptr CreateEnumerator(void)
	{
		return m_hashMap.CreateKeyEnumerator();
	}

	void Clear(void)
	{
		m_hashMap.Clear();
	}

	void Insert(const TElement& p_element)
	{
		m_hashMap.Insert(p_element, '\0');
	}

	void InsertSet(const TImmutableSet<TElement>& p_set)
	{
		for(TEnumerator<TElement> &enumerator
				= ((TImmutableSet<TElement>&) p_set).GetEnumerator();
			enumerator.HasMoreElements();)
			m_hashMap.Insert(enumerator.NextElement(), '\0');
	}

	void InsertEnumerator(TEnumerator<TElement>& p_enumerator)
	{
		for(p_enumerator.Reset();
			p_enumerator.HasMoreElements();)
			m_hashMap.Insert(p_enumerator.NextElement(), '\0');
	}

	void Remove(const TElement& p_element)
	{
		m_hashMap.RemoveKey(p_element);
	}

	void RemoveSet(const TImmutableSet<TElement>& p_set)
	{
		for(TEnumerator<TElement>& enumerator
			= ((TImmutableSet<TElement>&) p_set).GetEnumerator();
			enumerator.HasMoreElements();)
			m_hashMap.RemoveKey(enumerator.NextElement());
	}

	void RemoveEnumerator(TEnumerator<TElement>& p_enumerator)
	{
		for(p_enumerator.Reset();
			p_enumerator.HasMoreElements();)
			m_hashMap.RemoveKey(p_enumerator.NextElement());
	}

	THashSet<TElement>& operator=(const TImmutableSet<TElement>& p_set)
	{
		if (&p_set == this) return *this;

		Clear();
		InsertList(p_set);
		return *this;
	}

	THashSet<TElement>& operator=(const TSet<TElement>& p_set)
	{
		if (&p_set == this) return *this;

		Clear();
		InsertList(p_set);
		return *this;
	}

	THashSet<TElement>& operator=(const THashSet<TElement>& p_set)
	{
		if (&p_set == this) return *this;

		Clear();
		InsertList(p_set);
		return *this;
	}
};

Meson_Common_Collections_END
