//----------------------------------------------------------------------------------------------
//	Meson::Common::Random::RandomUniform
//	Provides pseudo-random number generation over a uniform probability distribution
//  using the mersenne twister algorithm.
//	Reference: http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/CODES/mt19937ar.c
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "RandomUniform.h"

using namespace Meson::Common::Random;

//----------------------------------------------------------------------------------------------
/// Constant definition
//----------------------------------------------------------------------------------------------
const uint RandomUniform::m_unMagicNumbers[2] = { 0x0, MERSENNE_MATRIX_A };

//----------------------------------------------------------------------------------------------
/// Constructor
//----------------------------------------------------------------------------------------------
RandomUniform::RandomUniform( void ) 
{
	Seed(0x000C1CC0); 
}

//----------------------------------------------------------------------------------------------
/// Sets the seed of the pseudo-random number generator.
/// \param p_unSeed An unsigned 32-bit integer representing the seed.
//----------------------------------------------------------------------------------------------
void RandomUniform::Seed( uint p_unSeed )
{
	m_unSeed = m_unStateVector[0] = p_unSeed;

	for (m_sStateVectorIndex = 1; m_sStateVectorIndex < MERSENNE_N; ++m_sStateVectorIndex) {
		m_unStateVector[m_sStateVectorIndex] = (69069 * m_unStateVector[m_sStateVectorIndex - 1]);
	}
}

//----------------------------------------------------------------------------------------------
/// Resets the generator to the first entry in sequence.
//----------------------------------------------------------------------------------------------
void RandomUniform::Reset( void ) 
{ 
	Seed( m_unSeed ); 
}

//----------------------------------------------------------------------------------------------
/// Returns the next pseudo-random integer number.
/// \returns Random number
//----------------------------------------------------------------------------------------------
uint RandomUniform::Next( void ) 
{
	uint unResult;

	if (m_sStateVectorIndex >= MERSENNE_N)
	{
		short sVectorSpaceIndex = 0;

		for (; sVectorSpaceIndex < MERSENNE_N - MERSENNE_M; ++sVectorSpaceIndex)
		{
			unResult = (m_unStateVector[sVectorSpaceIndex] & MERSENNE_UPPER_MASK) | (m_unStateVector[sVectorSpaceIndex + 1] & MERSENNE_LOWER_MASK);
			m_unStateVector[sVectorSpaceIndex] = m_unStateVector[sVectorSpaceIndex + MERSENNE_M] ^ (unResult >> 1) ^ m_unMagicNumbers[unResult & 0x1];
		}

		for(;sVectorSpaceIndex < MERSENNE_N - 1; ++sVectorSpaceIndex)
		{
			unResult = (m_unStateVector[sVectorSpaceIndex] & MERSENNE_UPPER_MASK) | (m_unStateVector[sVectorSpaceIndex + 1] & MERSENNE_LOWER_MASK);
			m_unStateVector[sVectorSpaceIndex] = m_unStateVector[sVectorSpaceIndex+(MERSENNE_M - MERSENNE_N)] ^ (unResult >> 1) ^ m_unMagicNumbers[unResult & 0x1];
		}

		unResult = (m_unStateVector[MERSENNE_N - 1] & MERSENNE_UPPER_MASK) | (m_unStateVector[0] & MERSENNE_LOWER_MASK);
		m_unStateVector[MERSENNE_N - 1] = m_unStateVector[MERSENNE_M - 1] ^ (unResult >> 1) ^ m_unMagicNumbers[unResult & 0x1];

		m_sStateVectorIndex = 0;
	}

	unResult = m_unStateVector[m_sStateVectorIndex++];
	unResult ^= unResult >> 11;
	unResult ^= (unResult << 7) & MERSENNE_TEMPERING_MASK_B;
	unResult ^= (unResult << 15) & MERSENNE_TEMPERING_MASK_C;
	unResult ^= unResult >> 15;

	return unResult;
}

//----------------------------------------------------------------------------------------------
/// Returns the next random number less than the maximum specified.
/// \param p_unRangeMax Specifies upper bound for random number
/// \returns Random Number
//----------------------------------------------------------------------------------------------
uint RandomUniform::Next( uint p_unRangeMax ) 
{
	return (uint)(MERSENNE_INVERSE_RNG * p_unRangeMax * Next());
}

//----------------------------------------------------------------------------------------------
/// Returns the next random number in the specified range.
/// \param p_unRangeMax Specifies inclusive lower bound for random number
/// \param p_unRangeMax Specifies exclusive upper bound for random number
/// \returns Random Number
//----------------------------------------------------------------------------------------------
uint RandomUniform::Next( uint p_unRangeMin, uint p_unRangeMax ) 
{
	return (uint)(MERSENNE_INVERSE_RNG * (p_unRangeMax - p_unRangeMin) * Next()) + p_unRangeMin;
}

//----------------------------------------------------------------------------------------------
/// Returns the next single precision random number in the range (0..1].
/// \returns Single precision random number in the range (0..1]
//----------------------------------------------------------------------------------------------
float RandomUniform::NextFloat( void ) 
{
	return (float)(Next() * MERSENNE_INVERSE_RNG);
}

//----------------------------------------------------------------------------------------------
/// Returns the next double precision random number in the range (0..1].
/// \returns Double precision random number in the range (0..1]
//----------------------------------------------------------------------------------------------
double RandomUniform::NextDouble( void ) 
{
	return (double)(Next() * MERSENNE_INVERSE_RNG);
}
