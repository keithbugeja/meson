//----------------------------------------------------------------------------------------------
//	Meson::Common::Random::TRandomNonUniform
//	Templated class for non-uniform random generators using the inverse method.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

// Core Includes
#include "Types.h"
#include "Namespaces.h"
#include "MesonException.h"

#include "Maths.h"
#include "Statistics.h"
#include "RandomUniform.h"

// namespace Meson.Common.Random
Meson_Common_Random_BEGIN

//----------------------------------------------------------------------------------------------
// Base functor for inverse method function
//----------------------------------------------------------------------------------------------
class IRandomNonUniformFunction {
public:
	virtual double operator() ( RandomUniform &p_random ) = 0;
};

//----------------------------------------------------------------------------------------------
// Template for inverse-method pseudorandom number generator
//----------------------------------------------------------------------------------------------
template <typename TNonUniformFunction>
class TRandomNonUniform : public RandomUniform
{
	TNonUniformFunction m_nonUniformFunction;

public:
	double NextNonUniform( void ) {
		return m_nonUniformFunction( *this );	
	}
};

Meson_Common_Random_END