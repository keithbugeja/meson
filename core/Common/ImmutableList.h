//----------------------------------------------------------------------------------------------
//	Meson::Common::Collections::TImmutableList<TElement>
//	The templated immutable list container defines a standard interface for all read-only list
//  implementations.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella 	08/08/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include "Collection.h"

Meson_Common_Collections_BEGIN

//----------------------------------------------------------------------------------------------
/// Common interface for read-only, templated, list containers.
//----------------------------------------------------------------------------------------------
template<class TElement>
class TImmutableList
	: public TCollection<TElement>
{
public: // public methods

	//------------------------------------------------------------------------------------------
	/// Destroys the list.
	//------------------------------------------------------------------------------------------
	virtual ~TImmutableList(void) { }

	//------------------------------------------------------------------------------------------
	/// Accesses an element safely via the given index using the indexing operator. An exception
	/// is thrown if the index is out of range.
	/// \param p_unIndex Index of the element to access.
	/// \returns the indexed element.
	//------------------------------------------------------------------------------------------
	virtual TElement& operator[](size_t p_unIndex) const = 0;

	//------------------------------------------------------------------------------------------
	/// Accesses an element via the given index using the functor operator. No range checking is
	/// performed on the index as the operator is intended for fast element access.
	/// \param p_unIndex Index of the element to access.
	/// \returns the indexed element.
	//------------------------------------------------------------------------------------------
	virtual TElement& operator()(size_t p_unIndex) const = 0;

	//------------------------------------------------------------------------------------------
	/// Accesses an element safely via a given index. An exception is thrown if the index is out
	/// of range.
	/// \param p_unIndex Index of the element to access.
	/// \returns the indexed element.
	//------------------------------------------------------------------------------------------
	virtual TElement& GetElement(size_t p_unIndex) const = 0;

	//------------------------------------------------------------------------------------------
	/// Returns the index of the given element if found in the list, or -1 otherwise.
	/// \returns the index of the given element if found in the list, or -1 otherwise.
	//------------------------------------------------------------------------------------------
	virtual int IndexOf(const TElement& p_element) const = 0;
};

Meson_Common_Collections_END
