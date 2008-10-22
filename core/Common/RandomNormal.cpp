//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::RandomMersenne
//	Templated class for statistic functions.
//	http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/CODES/mt19937ar.c
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "RandomNormal.h"

using namespace Meson::Common::Maths;

const uint RandomNormal::Period = 0xFFFFFFFF;
const uint RandomNormal::m_unMagicNumbers[2] = { 0x0, MATRIX_A };
const double RandomNormal::m_dfPeriodInverse = 1.0 / RandomNormal::Period;

RandomNormal::RandomNormal( void ) 
{
	Seed(0x000C1CC0); 
}

void RandomNormal::Seed( uint p_unSeed )
{
	m_unSeed = m_unStateVector[0] = p_unSeed & Period;

	for (m_sStateVectorIndex = 1; m_sStateVectorIndex < N; ++m_sStateVectorIndex) {
		m_unStateVector[m_sStateVectorIndex] = (69069 * m_unStateVector[m_sStateVectorIndex - 1]) & Period;
	}
}

void RandomNormal::Reset( void ) 
{ 
	Seed( m_unSeed ); 
}

uint RandomNormal::Next( void ) 
{
	uint unResult;

	if (m_sStateVectorIndex >= N)
	{
		short sVectorSpaceIndex = 0;

		for (; sVectorSpaceIndex < N - M; ++sVectorSpaceIndex)
		{
			unResult = (m_unStateVector[sVectorSpaceIndex] & UPPER_MASK) | (m_unStateVector[sVectorSpaceIndex + 1] & LOWER_MASK);
			m_unStateVector[sVectorSpaceIndex] = m_unStateVector[sVectorSpaceIndex + M] ^ (unResult >> 1) ^ m_unMagicNumbers[unResult & 0x1];
		}

		for(;sVectorSpaceIndex < N - 1; ++sVectorSpaceIndex)
		{
			unResult = (m_unStateVector[sVectorSpaceIndex] & UPPER_MASK) | (m_unStateVector[sVectorSpaceIndex + 1] & LOWER_MASK);
			m_unStateVector[sVectorSpaceIndex] = m_unStateVector[sVectorSpaceIndex+(M - N)] ^ (unResult >> 1) ^ m_unMagicNumbers[unResult & 0x1];
		}

		unResult = (m_unStateVector[N - 1] & UPPER_MASK) | (m_unStateVector[0] & LOWER_MASK);
		m_unStateVector[N - 1] = m_unStateVector[M - 1] ^ (unResult >> 1) ^ m_unMagicNumbers[unResult & 0x1];

		m_sStateVectorIndex = 0;
	}

	unResult = m_unStateVector[m_sStateVectorIndex++];
	unResult ^= unResult >> 11;
	unResult ^= (unResult << 7) & TEMPERING_MASK_B;
	unResult ^= (unResult << 15) & TEMPERING_MASK_C;
	unResult ^= unResult >> 15;

	return unResult;
}

uint RandomNormal::Next( uint p_unRangeMax ) 
{
	return (uint)(m_dfPeriodInverse * p_unRangeMax * Next());
}

uint RandomNormal::Next( uint p_unRangeMin, uint p_unRangeMax ) 
{
	return (uint)(m_dfPeriodInverse * (p_unRangeMax - p_unRangeMin) * Next()) + p_unRangeMin;
}

float RandomNormal::NextFloat( void ) 
{
	return (float)(Next() * m_dfPeriodInverse);
}

double RandomNormal::NextDouble( void ) 
{
	return (double)(Next() * m_dfPeriodInverse);
}
