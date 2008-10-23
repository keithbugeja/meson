//----------------------------------------------------------------------------------------------
//	Meson::Common::Random::RandomUniform
//	Provides pseudo-random number generation over a uniform probability distribution
//  using the mersenne twister algorithm. Note that this generator is used as a base
//  for all other generators.
//	Reference: http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/CODES/mt19937ar.c
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

// namespace Meson.Common.Random
Meson_Common_Random_BEGIN

// Period parameters
#define MERSENNE_N				624
#define MERSENNE_M				397
#define MERSENNE_MATRIX_A		0x9908b0dfU
#define	MERSENNE_UPPER_MASK		0x80000000U
#define	MERSENNE_LOWER_MASK		0x7fffffffU
#define MERSENNE_INVERSE_RNG	2.3283064370807974e-010

// Tempering parameters
#define MERSENNE_TEMPERING_MASK_B 0x9d2c5680
#define	MERSENNE_TEMPERING_MASK_C 0xefc60000

class RandomUniform
{
protected:
	static const uint m_unMagicNumbers[2]; 

	// State vector & index
	uint  m_unStateVector[MERSENNE_N];
	short m_sStateVectorIndex;

	// Operating parameters
	uint m_unSeed;

public:    
	RandomUniform( void );
	
	virtual void Reset( void ); 
	virtual void Seed( uint p_unSeed );

	virtual uint Next( uint p_unRangeMax );
	virtual uint Next( uint p_unRangeMin, uint p_unRangeMax );
	virtual uint Next( void );
	
	virtual float NextFloat( void );
	
	virtual double NextDouble( void );
};

Meson_Common_Random_END