//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::RadomLinear
//	Templated class for statistic functions.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "RandomLinear.h"

using namespace Meson::Common::Maths;

const uint RandomLinear::Period = 0xFFFFFFFF;
const double RandomLinear::m_dfPeriodInverse = 1.0 / RandomLinear::Period;

RandomLinear::RandomLinear( void ) 
{
	Seed(0x000C1CC0); 
}

void RandomLinear::Reset( void ) 
{ 
	m_ullValue = m_unSeed; 
}

void RandomLinear::Seed( uint p_unSeed ) 
{
	m_ullValue = m_unSeed = p_unSeed;
}

uint RandomLinear::Next( uint p_unRangeMax ) 
{
	return (uint)(m_dfPeriodInverse * p_unRangeMax * Next());
}

uint RandomLinear::Next( uint p_unRangeMin, uint p_unRangeMax ) 
{
	return (uint)(m_dfPeriodInverse * (p_unRangeMax - p_unRangeMin) * Next()) + p_unRangeMin;
}

uint RandomLinear::Next( void ) 
{
	return (uint) (m_ullValue = (279470273U * m_ullValue) % 4294967291U);
}

float RandomLinear::NextFloat( void ) 
{
	return (float)(Next() * m_dfPeriodInverse);
}

double RandomLinear::NextDouble( void ) 
{
	return (double)(Next() * m_dfPeriodInverse);
}