#pragma once

#include "List.h"
#include "ArrayList.h"
#include "LinkedList.h"
#include "Vector3.h"

#include "GrvNamespaces.h"

#ifdef GRAVITAS_EXPORTS
	#define GRAVITAS_API __declspec(dllexport)
#else
	#define GRAVITAS_API __declspec(dllimport)
#endif

Meson_Gravitas_BEGIN

// real type use 'r' for hungarian notation

//------------------------------------------------------------------------------------------
/// Real numeric type definition. The physics engine may be recompiled with a different
/// numeric precision by altering this definition.
//------------------------------------------------------------------------------------------
typedef double Real;

//------------------------------------------------------------------------------------------
/// Abstract list of vectors defined over the Real field.
//------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TList< Meson::Common::Maths::TVector3<Real> > VectorList;

//------------------------------------------------------------------------------------------
/// Array list of vectors defined over the Real field.
//------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TArrayList< Meson::Common::Maths::TVector3<Real> > VectorArrayList;

//------------------------------------------------------------------------------------------
/// Linked list of vectors defined over the Real field.
//------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TLinkedList< Meson::Common::Maths::TVector3<Real> > VectorLinkedList;

Meson_Gravitas_END
