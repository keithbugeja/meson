//----------------------------------------------------------------------------------------------
//	Meson::Common::Collections::TEnumerator<TElement>
//	This is a templated interface that provides enumeration services over various list and
//	map container implementations.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella 	17/07/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "Pointer.h"

Meson_Common_Collections_BEGIN

//----------------------------------------------------------------------------------------------
//	This is a templated interface that provides enumeration services over various list and
//	map container implementations.
//----------------------------------------------------------------------------------------------
template<class TElement>
class TEnumerator
	: public Meson::Common::TReferenceCounter< TEnumerator<TElement> >
{
public: // public methods
	virtual ~TEnumerator(void) {}
	virtual bool HasMoreElements(void) = 0;
	virtual void Reset(void) = 0;
	virtual TElement& NextElement(void) = 0;
	virtual size_t Size(void) const = 0;
	virtual size_t Available(void) const = 0;

	typedef Meson::Common::TPointer< TEnumerator > Ptr;
};

Meson_Common_Collections_END
