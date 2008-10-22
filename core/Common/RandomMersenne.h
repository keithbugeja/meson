//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::RandomMersenne
//	Templated class for statistic functions.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

// Core Includes
#include "Namespaces.h"
#include "MesonException.h"
#include "Maths.h"

#include "IRandom.h"

// namespace Meson.Common.Math
Meson_Common_Maths_BEGIN

class RandomMersenne : public IRandom
{
private:
	// Period parameters
	static const uint N = 624,
					  M = 397,
					  MATRIX_A = 0x9908b0dfU,
					  UPPER_MASK = 0x80000000U,
					  LOWER_MASK = 0x7fffffffU;

	// Tempering parameters
	static const uint TEMPERING_MASK_B = 0x9d2c5680, 
					  TEMPERING_MASK_C = 0xefc60000;

	static const uint m_unMagicNumbers[2]; 

	// State vector & index
	uint  m_unStateVector[N];
	short m_sStateVectorIndex;

	// Operating parameters
	static const double m_dfPeriodInverse;
	uint m_unSeed;

public:
	static const uint Period;

public:    
	RandomMersenne( void );
void Reset( void ); 
	void Seed( uint p_unSeed );

	uint Next( uint p_unRangeMax );
	uint Next( uint p_unRangeMin, uint p_unRangeMax );
	uint Next( void );
	
	float NextFloat( void );
	
	double NextDouble( void );
};

typedef RandomMersenne Random;

Meson_Common_Maths_END