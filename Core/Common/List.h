//----------------------------------------------------------------------------------------------
//	Meson::Common::Collections::TList<TElement>
//	The templated list container defines a standard interface for all list implementations.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella 	08/08/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include "ImmutableList.h"
#include "Memory.h"
#include "InputStream.h"
#include "OutputStream.h"

Meson_Common_Collections_BEGIN

//------------------------------------------------------------------------------------------
/// Returns a pointer to the memory manager for all List heap instances.
/// \returns a pointer to the memory manager for all List heap instances.
//------------------------------------------------------------------------------------------
Meson::Common::Memory::MemoryManager* GetListMemoryManager(void);

//----------------------------------------------------------------------------------------------
/// Common interface for templated list containers. The interface extends TImmutableList by
/// providing list management methods.
//----------------------------------------------------------------------------------------------
template<class TElement>
class TList
	: public TImmutableList<TElement>
{
public: // public methods
	//------------------------------------------------------------------------------------------
	/// Assigns the given list into this this via the assignment operator.
	/// \param p_list Reference to a source list to assign.
	/// \returns a reference to this list.
	//------------------------------------------------------------------------------------------
	virtual TList<TElement>& operator=(const TList<TElement>& p_list) = 0;

	//------------------------------------------------------------------------------------------
	/// Clears the contents of the list.
	//------------------------------------------------------------------------------------------
	virtual void Clear(void) = 0;

	//------------------------------------------------------------------------------------------
	/// Modifies an element safely via a given index. An exception is thrown if the index is out
	/// of range.
	/// \param p_unIndex Index of the element to access.
	/// \param p_element The new element value.
	//------------------------------------------------------------------------------------------
	virtual void SetElement(size_t p_unIndex, const TElement& p_element) = 0;

	//------------------------------------------------------------------------------------------
	/// Appends the given element to the list.
	/// \param p_element Reference to an element to append.
	//------------------------------------------------------------------------------------------
	virtual void Add(const TElement& p_element) = 0;

	//------------------------------------------------------------------------------------------
	/// Appends the given list to this list.
	/// \param p_list Reference to a list to append.
	//------------------------------------------------------------------------------------------
	virtual void AddList(const TImmutableList<TElement>& p_list) = 0;

	//------------------------------------------------------------------------------------------
	/// Inserts the given element to the list at the given index.
	/// \param p_unIndex List index for the element to be inserted.
	/// \param p_elemet Reference to a element to insert.
	//------------------------------------------------------------------------------------------
	virtual void Insert(size_t p_unIndex,
		const TElement& p_element) = 0;

	//------------------------------------------------------------------------------------------
	/// Inserts the given list at the given index.
	/// \param p_unIndex List index where the given list is to be inserted.
	/// \param p_list Reference to a list to insert.
	//------------------------------------------------------------------------------------------
	virtual void InsertList(size_t p_unIndex,
		const TImmutableList<TElement>& p_list) = 0;

	//------------------------------------------------------------------------------------------
	/// Inserts the elements of the given enumerator at the given index.
	/// \param p_unIndex List index where the given list is to be inserted.
	/// \param p_enumerator Reference to the enumerator to be processed.
	//------------------------------------------------------------------------------------------
	virtual void InsertEnumerator(size_t p_unIndex,
		TEnumerator<TElement>& p_enumerator) = 0;

	//------------------------------------------------------------------------------------------
	/// Removes the element at the given index.
	/// \param p_unIndex Index of the element to be removed.
	//------------------------------------------------------------------------------------------
	virtual void RemoveAt(size_t p_unIndex) = 0;

	//------------------------------------------------------------------------------------------
	/// Removes all instances of the given element matched via the equality operator from the
	/// list.
	/// \param p_element Element matching the instances to be removed.
	//------------------------------------------------------------------------------------------
	virtual void Remove(const TElement& p_element) = 0;

	//------------------------------------------------------------------------------------------
	/// Removes a range of elements given by a starting index and an element count.
	/// \param p_unIndex Starting index for the element range to be removed.
	/// \param p_unLength Count of elements to be removed from the starting index onwards.
	//------------------------------------------------------------------------------------------
	virtual void RemoveRange(size_t p_unIndex, size_t p_unLength) = 0;

	//------------------------------------------------------------------------------------------
	/// Reverses the order of the elements within the list using the assignment operator to
	/// swap the elements across.
	//------------------------------------------------------------------------------------------
	virtual void Reverse(void) = 0;

	//------------------------------------------------------------------------------------------
	/// Returns an immutable version of the list.
	/// \returns an immutable version of the list.
	//------------------------------------------------------------------------------------------
	virtual TImmutableList<TElement>& AsImmutableList(void) const = 0;
};

Meson_Common_Collections_END

//------------------------------------------------------------------------------------------
/// Tests the given list references for equality via the global equality operator.
/// \param p_list1 First list to compare.
/// \param p_list2 Second list to compare.
/// \returns True if lists match by size and pairwise elements, or False otherwise.
//------------------------------------------------------------------------------------------
template<class TElement>
bool operator==(
	const Meson::Common::Collections::TImmutableList<TElement>& p_list1,
	const Meson::Common::Collections::TImmutableList<TElement>& p_list2)
{
	// list sizes must match
	if (p_list1.Size() != p_list2.Size()) return false;

	// lists equal if both empty
	if (p_list1.Size() == 0) return true;

	// otherwise, fail equality test on first mismatching element pair
	Meson::Common::Collections::TEnumerator<TElement>& enumerator1
		= ((Meson::Common::Collections::TImmutableList<TElement>&) p_list1).GetEnumerator();
	Meson::Common::Collections::TEnumerator<TElement>& enumerator2
		= ((Meson::Common::Collections::TImmutableList<TElement>&) p_list2).GetEnumerator();
	for (; enumerator1.HasMoreElements();)
		if (!(enumerator1.NextElement() == enumerator2.NextElement()))
			return false;

	// if no test fails, lists considered equal
	return true;
};

template<class TElement>
bool operator!=(
	const Meson::Common::Collections::TImmutableList<TElement>& p_list1,
	const Meson::Common::Collections::TImmutableList<TElement>& p_list2)
{
	return !(p_list1 == p_list2);
}

//------------------------------------------------------------------------------------------
/// Appends the given element to the given list via the global addition assignment operator.
/// \param p_list List to which the element is to be appended.
/// \param p_element Element to append.
/// \returns reference to the list to which the element is appended.
//------------------------------------------------------------------------------------------
template<class TElement>
Meson::Common::Collections::TList<TElement>& operator+=(
	Meson::Common::Collections::TList<TElement>& p_list,
	const TElement& p_element)
{
	p_list.Add(p_element);
	return p_list;
};

//------------------------------------------------------------------------------------------
/// Appends the given second list to the given first list via the global addition assignment
/// operator.
/// \param p_list List to which the second list is to be appended.
/// \param p_listToAppend List to append.
/// \returns reference to the list to which the second list is appended.
//------------------------------------------------------------------------------------------
template<class TElement>
Meson::Common::Collections::TList<TElement>& operator+=(
	Meson::Common::Collections::TList<TElement>& p_list,
	const Meson::Common::Collections::TImmutableList<TElement>& p_listToAppend)
{
	p_list.AddList(p_listToAppend);
	return p_list;
};

//------------------------------------------------------------------------------------------
/// Removes the given element from the given list via the global subtraction assignment
/// operator.
/// \param p_list List from which the element is to be removed.
/// \param p_element Element to remove.
/// \returns reference to the list from which the element is removed.
//------------------------------------------------------------------------------------------
template<class TElement>
Meson::Common::Collections::TList<TElement>& operator-=(
	Meson::Common::Collections::TList<TElement>& p_list,
	const TElement& p_element)
{
	p_list.Remove(p_element);
	return p_list;
};

//------------------------------------------------------------------------------------------
/// Removes the elements of the given second list from the given first list via the global
/// subtraction assignment operator.
/// \param p_list List from which the elements of the second list are to be removed.
/// \param p_listToRemove List with the elements to remove.
/// \returns reference to the list from which the elements of the second list are removed.
//------------------------------------------------------------------------------------------
template<class TElement>
Meson::Common::Collections::TList<TElement>& operator-=(
	Meson::Common::Collections::TList<TElement>& p_list,
	const Meson::Common::Collections::TImmutableList<TElement>& p_listToRemove)
{
	Meson::Common::Collections::TEnumerator<TElement>& enumerator
		= ((Meson::Common::Collections::TList<TElement>&)
			p_listToRemove).GetEnumerator();
	for (; enumerator.HasMoreElements();)
		p_list.Remove(enumerator.NextElement());
	return p_list;
};

//------------------------------------------------------------------------------------------
/// Streams the given list into the given IOutputStream via the global left-shift operator.
/// The streaming operator must also be defined for the list element types for this
/// template function to compile and operate correctly.
/// \param p_outputStream Output stream to which to stream the list.
/// \param p_list List to be written to the output stream.
/// \returns reference to the output stream after the list is written to it.
//------------------------------------------------------------------------------------------
template<class TElement>
Meson::Common::IO::IOutputStream& operator<<(
	Meson::Common::IO::IOutputStream& p_outputStream,
	const Meson::Common::Collections::TImmutableList<TElement>& p_list)
{
	// write list size
	p_outputStream << (uint) p_list.Size();

	// write individual elements
	Meson::Common::Collections::TEnumerator<TElement>& enumerator
		= ((Meson::Common::Collections::TList<TElement>&)
			p_list).GetEnumerator();
	for (; enumerator.HasMoreElements();)
		p_outputStream << enumerator.NextElement();

	// return output stream reference
	return p_outputStream;
}

//------------------------------------------------------------------------------------------
/// Reads serialised list data from the given IInputStream into the given list via the
/// global right-shift streaming operator. The streaming operator must also be defined for
/// the list element types for this template function to compile and operate correctly.
/// \param p_inputStream Output stream from which to deserialise the list.
/// \param p_list List to receive the serialised content.
/// \returns reference to the input stream after the list deserialisation.
//------------------------------------------------------------------------------------------
template<class TElement>
Meson::Common::IO::IInputStream& operator>>(
	Meson::Common::IO::IInputStream& p_inputStream,
	Meson::Common::Collections::TList<TElement>& p_list)
{
	// deserialise list size
	uint unSize;
	p_inputStream >> unSize;

	// deserialise elements
	TElement element;
	while (unSize-- > 0)
	{
		p_inputStream >> element;
		p_list.Add(element);
	}

	// return input stream reference
	return p_inputStream;
}

Meson_Common_Collections_BEGIN

template<typename TPointer>
void FreePointers(Meson::Common::Collections::TList<TPointer>& p_listPointers)
{
	TEnumerator<TPointer>& enumPointers = p_listPointers.GetEnumerator();
	for (TPointer pointer = NULL; enumPointers.HasMoreElements();
		pointer = enumPointers.NextElement())
		delete pointer;
};

Meson_Common_Collections_END
