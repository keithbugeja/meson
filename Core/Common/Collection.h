//----------------------------------------------------------------------------------------------
//	Meson::Common::Collections::TCollection<TElement>
//	The templated collection container defines a standard interface for all collections.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella 	07/03/2008	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "Enumerator.h"
#include "Pointer.h"

Meson_Common_Collections_BEGIN

//----------------------------------------------------------------------------------------------
/// Common interface a generic container.
//----------------------------------------------------------------------------------------------
template<class TElement>
class TCollection
	: public Meson::Common::TReferenceCounter< TCollection<TElement> >
{
public: // public methods

	//------------------------------------------------------------------------------------------
	/// Destroys the collection.
	//------------------------------------------------------------------------------------------
	virtual ~TCollection(void) { }

	//------------------------------------------------------------------------------------------
	/// Returns the size of the collection.
	/// \returns the size of the collection.
	//------------------------------------------------------------------------------------------
	virtual size_t Size(void) const = 0;

	//------------------------------------------------------------------------------------------
	/// Returns a reference to a TEnumerator for enumerating the elements of the collection.
	/// \returns a reference to a TEnumerator for enumerating the elements of the collection.
	//------------------------------------------------------------------------------------------
	virtual TEnumerator<TElement>& GetEnumerator(bool p_bReset = true) = 0;

	virtual typename TEnumerator<TElement>::Ptr CreateEnumerator(void) = 0;
};

Meson_Common_Collections_END
