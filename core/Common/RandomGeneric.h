//----------------------------------------------------------------------------------------------
//	Meson::Common::Random::RandomGeneric
//	Provides both uniform and non-uniform pseudo-random number generation. This generator does
//  not incur the indirection penalty of templated non-uniform generators.
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

#include "RandomUniform.h"

// namespace Meson.Common.Math
Meson_Common_Random_BEGIN
class RandomGeneric : public RandomUniform
{
protected:
	bool m_bUseLast;
	double m_dfLastValue;

public:
	double RandomGeneric::NextExponential( void );
	double RandomGeneric::NextNormal( void );
};

Meson_Common_Random_END