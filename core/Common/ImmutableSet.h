//----------------------------------------------------------------------------------------------
//	Meson::Common::Collections::TImmutableSet<TElement>
//	The templated immutable set container defines a standard interface for all read-only set
//  implementations.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella 	09/08/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "Collection.h"
#include "Pointer.h"

Meson_Common_Collections_BEGIN

//----------------------------------------------------------------------------------------------
/// Common interface for read-only, templated, set containers.
//----------------------------------------------------------------------------------------------
template<class TElement>
class TImmutableSet
	: public TCollection<TElement>
{
public:
	//------------------------------------------------------------------------------------------
	/// Destroys the set.
	//------------------------------------------------------------------------------------------
	virtual ~TImmutableSet(void) { }

	//------------------------------------------------------------------------------------------
	/// Returns true of the given element is in the set, or false otherwise.
	/// \returns true of the given element is in the set, or false otherwise.
	//------------------------------------------------------------------------------------------
	virtual bool ContainsElement(const TElement& p_element) const = 0; 

	//------------------------------------------------------------------------------------------
	/// Returns true of the given set intersects this set, or false otherwise.
	/// \returns true of the given set intersects this set, or false otherwise.
	//------------------------------------------------------------------------------------------
	virtual bool Intersects(const TImmutableSet& p_immutableSet) const = 0; 
};

Meson_Common_Collections_END
