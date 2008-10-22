//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::IRandomGenerator
//	Templated class for statistic functions.
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

// namespace Meson.Common.Math
Meson_Common_Maths_BEGIN

class IRandom
{
public:
	static const uint Period;

	virtual void Reset( void ) = 0;
	virtual void Seed( uint p_nSeed ) = 0;
	virtual float NextFloat( void ) = 0;
	virtual double NextDouble( void ) = 0;
	virtual uint Next( void ) = 0;
	virtual uint Next( uint p_unRangeMax ) = 0;
	virtual uint Next( uint p_unRangeMin, uint p_unRangeMax ) = 0;
};

Meson_Common_Maths_END