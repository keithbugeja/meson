//----------------------------------------------------------------------------------------------
//	Meson::Common::Random::TRandomAcceptReject
//  Templated class for accept-reject method-based random generators.
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
// Base functor for distribution function
//----------------------------------------------------------------------------------------------
class IRandomDistributionFunction {
public: 
	virtual double operator() ( double p_dfX ) = 0;
};

//----------------------------------------------------------------------------------------------
// Template for Accept-Reject Pseudorandom number generator
//----------------------------------------------------------------------------------------------
template <typename TDistributionFunction>
class TRandomAcceptReject : public RandomUniform
{
	TDistributionFunction m_customDistributionFunction;

public:
	double NextAcceptReject( void ) 
	{
		double dfRandX, 
			   dfRandY;

		do
		{
			dfRandX = NextDouble();
			dfRandY = NextDouble();
		} while ( m_customDistributionFunction( dfRandX ) < dfRandY );
		
		return dfRandX;
	}
};

Meson_Common_Random_END