//----------------------------------------------------------------------------------------------
//	Meson::Common::Random::RandomGeneric
//	Provides both uniform and non-uniform pseudo-random number generation. This generator does
//  not incur the indirection penalty of templated non-uniform generators.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "Maths.h"
#include "Statistics.h"
#include "RandomGeneric.h"

using namespace Meson::Common::Random;
using namespace Meson::Common::Maths;

//----------------------------------------------------------------------------------------------
///	Returns a random number using the exponential probability distribution using 
/// the inverse-method.
/// \returns Pseudo-random number in exponential distribution
//----------------------------------------------------------------------------------------------
double RandomGeneric::NextExponential( void )
{
	double dfRandom = NextDouble();

	if (dfRandom < Math::Epsilon )
		return Math::Maximum;

	return Statistics::InverseExponentialCDF( dfRandom, 1.0 );
}
	
//----------------------------------------------------------------------------------------------
/// Returns a random number using the normal probability distribution using the 
/// the Marsaglia generator.
/// \returns Pseudo-random number in normal distribution
//----------------------------------------------------------------------------------------------
double RandomGeneric::NextNormal( void ) 
{
	// Marsaglia Polar
	// S = U^2 + V^2 < 1
	// X = U * Sqrt( -2 * Ln(S) / S );

	// Use cached value?
	if (m_bUseLast)
	{
		m_bUseLast = false;
		return m_dfLastValue;
	}

	double dfRand1, dfRand2, dfNormSqr;

	do
	{
		dfRand1 = 2 * NextDouble() - 1;
		dfRand2 = 2 * NextDouble() - 1;
	} while ( (dfNormSqr = dfRand1 * dfRand1 + dfRand2 * dfRand2) > 1 ) ;

	dfNormSqr = Math::Sqrt( -2 * Math::Ln( dfNormSqr ) / dfNormSqr );
	
	// Cache next value
	m_dfLastValue = dfRand1 * dfNormSqr;
	m_bUseLast = true;

	return dfRand2 * dfNormSqr;
}
