//----------------------------------------------------------------------------------------------
//	Meson::Common::Collections::TSet<TElement>
//	The templated set container defines a standard interface for all set implementations.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella 	09/08/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include "ImmutableSet.h"
#include "Memory.h"
#include "InputStream.h"
#include "OutputStream.h"

Meson_Common_Collections_BEGIN

//------------------------------------------------------------------------------------------
/// Returns a pointer to the memory manager for all Set heap instances.
/// \returns a pointer to the memory manager for all Set heap instances.
//------------------------------------------------------------------------------------------
Meson::Common::Memory::MemoryManager* GetSetMemoryManager(void);

//----------------------------------------------------------------------------------------------
/// Common interface for templated set containers. The interface extends TImmutableSet by
/// providing set management methods.
//----------------------------------------------------------------------------------------------
template<class TElement>
class TSet
	: public TImmutableSet<TElement>
{
public:
	//------------------------------------------------------------------------------------------
	/// Clears the contents of the set.
	//------------------------------------------------------------------------------------------
	virtual void Clear(void) = 0;

	virtual void Insert(const TElement& p_element) = 0;
	virtual void InsertSet(const TImmutableSet<TElement>& p_set) = 0;
	virtual void InsertEnumerator(TEnumerator<TElement>& p_enumerator) = 0;
	virtual void Remove(const TElement& p_element) = 0;
	virtual void RemoveSet(const TImmutableSet<TElement>& p_set) = 0;
	virtual void RemoveEnumerator(TEnumerator<TElement>& p_enumerator) = 0;
};

Meson_Common_Collections_END

template<class TElement>
bool operator==(
	const Meson::Common::Collections::TImmutableSet<TElement>& p_set1,
	const Meson::Common::Collections::TImmutableSet<TElement>& p_set2)
{
	if (p_set1.Size() != p_set2.Size()) return false;

	if (p_set1.Size() == 0) return true;

	Meson::Common::Collections::TEnumerator<TElement>& enumerator1
		= ((Meson::Common::Collections::TImmutableSet<TElement>&) p_set1).GetEnumerator();
	for (; enumerator1.HasMoreElements();)
		if (!p_set2.ContainsElement(enumerator1.NextElement()))
			return false;
	return true;
};

template<class TElement>
bool operator!=(
	const Meson::Common::Collections::TImmutableSet<TElement>& p_set1,
	const Meson::Common::Collections::TImmutableSet<TElement>& p_set2)
{
	return !(p_set1 == p_set2);
};
template<class TElement>
Meson::Common::Collections::TSet<TElement>& operator+=(
	Meson::Common::Collections::TSet<TElement>& p_set,
	const TElement& p_element)
{
	p_set.Insert(p_element);
	return p_set;
};

template<class TElement>
Meson::Common::Collections::TSet<TElement>& operator+=(
	Meson::Common::Collections::TSet<TElement>& p_set,
	const Meson::Common::Collections::TImmutableSet<TElement>& p_setToAdd)
{
	p_set.InsertSet(p_setToAdd);
	return p_set;
};

template<class TElement>
Meson::Common::Collections::TSet<TElement>& operator-=(
	Meson::Common::Collections::TSet<TElement>& p_set,
	const TElement& p_element)
{
	p_set.Remove(p_element);
	return p_set;
};

template<class TElement>
Meson::Common::Collections::TSet<TElement>& operator-=(
	Meson::Common::Collections::TSet<TElement>& p_set,
	const Meson::Common::Collections::TImmutableSet<TElement>& p_setToRemove)
{
	Meson::Common::Collections::TEnumerator<TElement> &enumerator
		= ((Meson::Common::Collections::TSet<TElement>&)
			p_setToRemove).GetEnumerator();
	for (; enumerator.HasMoreElements();)
		p_set.Remove(enumerator.NextElement());
	return p_set;
};

//template<class TElement>
//Meson::Common::Collections::TSet<TElement> &operator&=(
//	Meson::Common::Collections::TSet<TElement> &p_set,
//	const Meson::Common::Collections::TImmutableSet<TElement> &p_setToAnd)
//{
//	TTreeSet<TElement> setTemp;
//
//	for(Meson::Common::Collections::TEnumerator<TElement> &enumerator
//		= ((Meson::Common::Collections::TImmutableSet<TElement> &) p_setToAnd).GetEnumerator();
//		enumerator.HasMoreElements();)
//	{
//		TElement &element = enumerator.NextElement();
//		if (p_set.ContainsElement(element))
//			setTemp.Insert(element);
//	}
//
//	p_set.Clear();
//	p_set.InsertSet(setTemp);
//	return p_set;
//};

template<class TElement>
Meson::Common::Collections::TSet<TElement>& operator|=(
	Meson::Common::Collections::TSet<TElement>& p_set,
	const Meson::Common::Collections::TImmutableSet<TElement>& p_setToOr)
{
	p_set.InsertSet(p_setToOr);
	return p_set;
};

template<class TElement>
Meson::Common::IO::IOutputStream& operator<<(
	Meson::Common::IO::IOutputStream& p_outputStream,
	const Meson::Common::Collections::TImmutableSet<TElement>& p_set)
{
	p_outputStream << (uint) p_set.Size();

	TEnumerator<TElement> &enumerator
		= ((Meson::Common::Collections::TSet<TElement>&)
			p_set).GetEnumerator();

	for (; enumerator.HasMoreElements();)
		p_outputStream << enumerator.NextElement();
	
	return p_outputStream;
}

template<class TElement>
Meson::Common::IO::IInputStream& operator>>(
	Meson::Common::IO::IInputStream& p_inputStream,
	Meson::Common::Collections::TSet<TElement>& p_set)
{
	uint unSize;
	p_inputStream >> unSize;

	TElement element;
	while (unSize-- > 0)
	{
		p_inputStream >> element;
		p_set.Add(element);
	}

	return p_inputStream;
}

Meson_Common_Collections_BEGIN

template<typename TPointer>
void FreePointers(Meson::Common::Collections::TSet<TPointer>& p_setPointers)
{
	TEnumerator<TPointer>& enumPointers = p_setPointers.GetEnumerator();
	for (TPointer pointer = NULL; enumPointers.HasMoreElements(); pointer = enumPointers.NextElement())
		delete pointer;
};

Meson_Common_Collections_END