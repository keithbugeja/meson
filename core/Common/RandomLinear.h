//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::RandomLinear
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

#include "IRandom.h"

// namespace Meson.Common.Math
Meson_Common_Maths_BEGIN

class RandomLinear : public IRandom
{
public: 
	static const uint Period;

private:
	static const double m_dfPeriodInverse;

	uint m_unSeed;
	ulonglong m_ullValue;

public:    
	RandomLinear( void );
	
	void Reset( void ); 
	void Seed( uint p_unSeed );

	uint Next( uint p_unRangeMax );
	uint Next( uint p_unRangeMin, uint p_unRangeMax );
	uint Next( void );
	
	float NextFloat( void );
	
	double NextDouble( void );
};

Meson_Common_Maths_END