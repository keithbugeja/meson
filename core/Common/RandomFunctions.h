//----------------------------------------------------------------------------------------------
//	RandomFunctions.h
//	Contains various random number generating functions.
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
#include "RandomNonUniform.h"
#include "RandomAcceptReject.h"

// namespace Meson.Common.Random
Meson_Common_Random_BEGIN

//----------------------------------------------------------------------------------------------
/// RandomBoxMullerFunction
///	Generates random variates over the normal distribution using Box-Muller[58].
//----------------------------------------------------------------------------------------------
class RandomBoxMullerFunction : public IRandomNonUniformFunction
{
public:
	double operator() ( RandomUniform &p_random )
	{
		double dfU1 = p_random.NextDouble(),
			   dfU2 = p_random.NextDouble();

		return Meson::Common::Maths::Math::Sqrt( -2 * Meson::Common::Maths::Math::Ln( dfU1 )) * Meson::Common::Maths::Math::Cos( Meson::Common::Maths::Math::PiTwo * dfU2 );
	}
};

typedef TRandomNonUniform<RandomBoxMullerFunction> RandomBoxMuller;

//----------------------------------------------------------------------------------------------
/// RandomMarsagliaFunction
/// Generates random variates over the normal distribution using the Marsaglia polar method.
//----------------------------------------------------------------------------------------------
class RandomMarsagliaFunction : public IRandomNonUniformFunction
{
protected:
	bool m_bUseLast;
	double m_dfLastValue;

public:
	double operator() ( RandomUniform &p_random )
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
			dfRand1 = 2 * p_random.NextDouble() - 1;
			dfRand2 = 2 * p_random.NextDouble() - 1;
		} while ( (dfNormSqr = dfRand1 * dfRand1 + dfRand2 * dfRand2) > 1 ) ;

		dfNormSqr = Meson::Common::Maths::Math::Sqrt( -2 * Meson::Common::Maths::Math::Ln( dfNormSqr ) / dfNormSqr );
		
		// Cache next value
		m_dfLastValue = dfRand1 * dfNormSqr;
		m_bUseLast = true;

		return dfRand2 * dfNormSqr;
	}
};

typedef TRandomNonUniform<RandomMarsagliaFunction> RandomMarsaglia;

//----------------------------------------------------------------------------------------------
/// RandomInverseErfFunction
/// Generates random variates over the distribution using the inverse-method. The inverse
/// of the Normal CDF is given by the inverse error function.
//----------------------------------------------------------------------------------------------
class RandomInverseErfFunction : public IRandomNonUniformFunction
{
public:
	double operator() ( RandomUniform &p_random ) {
		return Meson::Common::Maths::Statistics::InverseErf( p_random.NextDouble() );
	}
};

typedef TRandomNonUniform<RandomInverseErfFunction> RandomInverseErf;

//----------------------------------------------------------------------------------------------
/// RandomExponentialFunction
/// Generates random variates over the exponential distribution using the inverse-method.
//----------------------------------------------------------------------------------------------
class RandomExponentialFunction : public IRandomNonUniformFunction
{
public:
	double operator() ( RandomUniform &p_random )
	{
		double dfRandom = p_random.NextDouble();
	
		if (dfRandom < Meson::Common::Maths::Math::Epsilon )
			return Meson::Common::Maths::Math::Maximum;

		return Meson::Common::Maths::Statistics::InverseExponentialCDF( dfRandom, 1.0 );
	}
};

typedef TRandomNonUniform<RandomExponentialFunction> RandomExponential;

Meson_Common_Random_END