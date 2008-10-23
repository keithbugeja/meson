#pragma once

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvKineticProperties.h"

Meson_Gravitas_Kinetics_BEGIN

//----------------------------------------------------------------------------------------------
/// \class IVectorTimeFunction
/// A functor interface representing a function mapping from the time domain to a vector range.
/// Implementations of the functor interface are used to represent vector quantities dependent
/// on time.
//----------------------------------------------------------------------------------------------
class IVectorTimeFunction
{
public:
	virtual ~IVectorTimeFunction(void) {};

	//----------------------------------------------------------------------------------------------
	/// Maps the given time input to a vector output.
	/// \param p_rTime Time input into the functor.
	/// \returns A vector quantity for the given time value.
	//----------------------------------------------------------------------------------------------
	virtual const Meson::Common::Maths::TVector3<Real> &operator()(Real p_rTime) = 0;
};

Meson_Gravitas_Kinetics_END
