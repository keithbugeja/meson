#pragma once

#include "Set.h"
#include "TreeMap.h"

Meson_Common_Collections_BEGIN

Meson::Common::Memory::MemoryManager* GetTreeSetMemoryManager(void);

template<class TElement>
class TTreeSet
	: public TSet<TElement>
{
private: // private variables
	TTreeMap<TElement, char> m_treeMap;

public:
	TTreeSet(void)
		: m_treeMap()
	{
	}

	TTreeSet(const TImmutableSet<TElement>& p_set)
		: m_treeMap()
	{
		InsertSet(p_set);
	}

	TTreeSet(const TSet<TElement>& p_set)
		: m_treeMap()
	{
		InsertSet(p_set);
	}

	TTreeSet(const TTreeSet& p_set)
		: m_treeMap(p_set.m_treeMap)
	{
	}

	~TTreeSet(void)
	{
	}

	void* operator new(size_t p_unSize)
	{
		return GetTreeSetMemoryManager()->AllocateAdhoc(p_unSize);
	}

	void operator delete(void* p_pObject, size_t p_unSize)
	{
		::operator delete(p_pObject);
	}							

	bool ContainsElement(const TElement& p_element) const
	{
		return m_treeMap.ContainsKey(p_element);
	}

	// generic implementation with no type knowledge of other set
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
				= ((TTreeSet<TElement>*) this)->CreateEnumerator();
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

	// specialised treeset intersection
	bool Intersects(const TTreeSet<TElement>& p_treeSet) const
	{
		// do linear search on sorted element assumption

		// get enumerators for both sets
		TEnumerator<TElement>::Ptr penumElementsThis
			= ((TTreeSet<TElement>)* this).CreateEnumerator();
		TEnumerator<TElement>::Ptr penumElementsOther
			= ((TTreeSet<TElement>&) p_treeSet).CreateEnumerator();

		// do intersection if either set empty
		if (!penumElementsThis->HasMoreElements())
			return false;
		if (!penumElementsOther->HasMoreElements())
			return false;

		// get first elements from set enumerators
		TElement elementThis = penumElementsThis->NextElement();
		TElement elementOther = penumElementsThis->NextElement();

		// iterate as long as either enumerator has elements
		while (penumElementsThis->HasMoreElements() && penumElementsOther->HasMoreElements())
		{
			// if first set element less than second, advance first iterator
			if (elementThis < elementOther)
				elementThis = penumElementsThis->NextElement();
			// otherwise if second set element less than first, advance second iterator
			else if (elementOther < elementThis)
				elementOther = penumElementsThis->NextElement();
			// otherwise there is a match
			else
				return true;
		}

		// if this point reached, no intersection
		return false;
	}

	size_t Size(void) const
	{
		return m_treeMap.Size();
	}

	inline TEnumerator<TElement>& GetEnumerator(bool p_bReset = true)
	{
		return m_treeMap.GetKeyEnumerator(p_bReset);
	}

	inline typename TEnumerator<TElement>::Ptr CreateEnumerator(void)
	{
		return m_treeMap.CreateKeyEnumerator();
	}

	void Clear(void)
	{
		m_treeMap.Clear();
	}

	void Insert(const TElement& p_element)
	{
		m_treeMap.Insert(p_element, '\0');
	}

	void InsertSet(const TImmutableSet<TElement>& p_set)
	{
		for(TEnumerator<TElement> &enumerator
				= ((TImmutableSet<TElement>&) p_set).GetEnumerator();
			enumerator.HasMoreElements();)
			m_treeMap.Insert(enumerator.NextElement(), '\0');
	}

	void InsertEnumerator(TEnumerator<TElement>& p_enumerator)
	{
		for(p_enumerator.Reset();
			p_enumerator.HasMoreElements();)
			m_treeMap.Insert(p_enumerator.NextElement(), '\0');
	}

	void Remove(const TElement& p_element)
	{
		m_treeMap.RemoveKey(p_element);
	}

	void RemoveSet(const TImmutableSet<TElement>& p_set)
	{
		for(TEnumerator<TElement>& enumerator
			= ((TImmutableSet<TElement>&) p_set).GetEnumerator();
			enumerator.HasMoreElements();)
			m_treeMap.RemoveKey(enumerator.NextElement());
	}

	void RemoveEnumerator(TEnumerator<TElement>& p_enumerator)
	{
		for(p_enumerator.Reset();
			p_enumerator.HasMoreElements();)
			m_treeMap.RemoveKey(p_enumerator.NextElement());
	}

	bool operator==(const TImmutableSet<TElement>& p_set) const
	{
		if (this == &p_set) return true;
		if (m_treeMap.Size() != p_set.Size()) return false;
		for (TEnumerator<TElement>& enumElements = ((TTreeMap<TElement, char> &) m_treeMap).GetKeyEnumerator();
			enumElements.HasMoreElements();)
		{
			if (!p_set.ContainsElement(enumElements.NextElement()))
				return false;
		}
		return true;
	}

	TTreeSet<TElement>& operator=(const TImmutableSet<TElement>& p_set)
	{
		if (&p_set == this) return *this;

		Clear();
		InsertSet(p_set);
		return *this;
	}

	TTreeSet<TElement>& operator=(const TSet<TElement>& p_set)
	{
		if (&p_set == this) return *this;

		Clear();
		InsertSet(p_set);
		return *this;
	}

	TTreeSet<TElement>& operator=(const TTreeSet<TElement>& p_set)
	{
		if (&p_set == this) return *this;

		Clear();
		InsertSet(p_set);
		return *this;
	}
};

Meson_Common_Collections_END
