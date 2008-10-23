//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::TStatistics<T>
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

#include "RandomUniform.h"

// namespace Meson.Common.Math
Meson_Common_Maths_BEGIN

// Template
template <typename TReal> class TStatistics
{
public:
	//----------------------------------------------------------------------------------------------
	/// Calculates the arithmetic mean of the given distribution.
	///	\param TReal *p_ptDistribution Distribution array
	/// \param int p_nDistributionSize Size of distribution
	/// \returns Arithmetic mean
	//----------------------------------------------------------------------------------------------
	static TReal Mean(TReal *p_ptDistribution, int p_nDistributionSize)
	{
		MESON_ASSERT( p_ptDistribution, "Distribution list is a null pointer!" );

		// Distribution size is zero or less
		if ( p_nDistributionSize <= 0 ) 
			return 0;

		TReal tSum = 0;

		for (int n = 0; n < p_nDistributionSize; n++)
			tSum += p_ptDistribution[n];

		tSum /= p_nDistributionSize;
		return tSum; 
	}

	//----------------------------------------------------------------------------------------------
	/// Calculates the variance of the given distribution.
	///	\param TReal *p_ptDistribution Distribution array
	/// \param int p_nDistributionSize Size of distribution
	/// \returns Variance
	//----------------------------------------------------------------------------------------------
	static TReal Variance(TReal *p_ptDistribution, int p_nDistributionSize)
	{
		MESON_ASSERT( p_ptDistribution, "Distribution list is a null pointer!" );

		// Distribution size is zero or less
		if ( p_nDistributionSize <= 0 ) 
			return 0;

		TReal tMean = Mean( p_ptDistribution, p_nDistributionSize ),
			  tDeviation = 0;

		for (int n = 0; n < p_nDistributionSize; n++)
			tDeviation += p_ptDistribution[n] * p_ptDistribution[n];

		tDeviation -= tMean * tMean * p_nDistributionSize;
		
		return tDeviation / p_nDistributionSize;
	}

	//----------------------------------------------------------------------------------------------
	/// Calculates the standard deviation of the given distribution.
	///	\param TReal *p_ptDistribution Distribution array
	/// \param int p_nDistributionSize Size of distribution
	/// \returns Standard deviation
	//----------------------------------------------------------------------------------------------
	static TReal StandardDeviation(TReal *p_ptDistribution, int p_nDistributionSize) 
	{
		MESON_ASSERT( p_ptDistribution, "Distribution list is a null pointer!" );

		// Distribution size is zero or less
		if ( p_nDistributionSize <= 0 ) 
			return 0;

		TReal tMean = Mean( p_ptDistribution, p_nDistributionSize ),
			  tDeviation = 0;

		for (int n = 0; n < p_nDistributionSize; n++)
			tDeviation += p_ptDistribution[n] * p_ptDistribution[n];

		tDeviation -= tMean * tMean * p_nDistributionSize;
		
		return TMaths<TReal>::Sqrt( tDeviation / p_nDistributionSize );
	}
	//----------------------------------------------------------------------------------------------
	/// Calculates the permutations, for the given number of elements, taken N at a time.
	/// \param p_nElements Number of elements
	/// \param p_nSelection Number of elements to take at a time
	/// \returns Permutation count
	//----------------------------------------------------------------------------------------------
	static int Permutations(int p_nElements, int p_nSelection)
	{
		// P( n, r ) = n! / (n-r)!

		MESON_ASSERT( p_nSelection <= p_nElements, "Number of elements selected is greater than element count!" );
		MESON_ASSERT( p_nElements > 0 && p_nSelection > 0, "Invalid number of elements or selected elements!" );
		
		// TODO: Optimise factorials
		int nNFct = TMaths<TReal>::Factorial(p_nElements),
			nNLessRFct = TMaths<TReal>::Factorial( p_nElements - p_nSelection);

		return ( nNFct / nNLessRFct  );
	}
	
	//----------------------------------------------------------------------------------------------
	/// Calculates the combinations, for the given number of elements, taken N at a time.
	/// \param p_nElements Number of elements
	/// \param p_nSelection Number of elements to take at a time
	/// \returns Combination count
	//----------------------------------------------------------------------------------------------
	static int Combinations(int p_nElements, int p_nSelection)
	{
		// C(n, r) = n! / ((n-r)!r!)

		MESON_ASSERT( p_nSelection <= p_nElements, "Number of elements selected is greater than element count!" );
		MESON_ASSERT( p_nElements > 0 && p_nSelection > 0, "Invalid number of elements or selected elements!" );
		
		// TODO: Optimise factorials
		int nNFct = TMaths<TReal>::Factorial(p_nElements),
			nNLessRFct = TMaths<TReal>::Factorial( p_nElements - p_nSelection),
			nRFct = TMaths<TReal>::Factorial(p_nSelection);

		return ( nNFct / (nNLessRFct * nRFct)  );
	}

	//----------------------------------------------------------------------------------------------
	/// Calculates the entropy for a given symbol probability list.
	/// \param *p_ptSymbolProbability List of symbol probabilities (should add to 1)
	/// \param p_ptSymbolCount Number of symbols in symbol probability list
	/// \param p_nCode Size of code used to represent symbols (default = 2[binary])
	/// \returns Entropy for given symbols
	//----------------------------------------------------------------------------------------------
	static TReal Entropy(TReal *p_ptSymbolProbability, int p_nSymbolCount, int p_nCode = 2)
	{
		MESON_ASSERT( p_ptSymbolProbability, "Symbol probability list is a null pointer!" );

		// Distribution size is zero or less
		if ( p_nSymbolCount <= 0 ) 
			return 0;

		TReal tLength = 0;

		for (int n=0; n < p_nSymbolCount; n++)
			tLength -= p_ptSymbolProbability[n] * TMaths<TReal>::LogBase( p_ptSymbolProbability[n], p_nCode );

		return tLength;
	}

	//----------------------------------------------------------------------------------------------
	/// Calculates the maximum entropy for a given number of symbols.
	/// \param p_ptSymbolCount Number of symbols 
	/// \param p_nCode Size of code used to represent symbols (default = 2[binary])
	/// \returns Maximum entropy for given symbol count
	//----------------------------------------------------------------------------------------------
	static TReal MaximumEntropy(int p_nSymbolCount, int p_nCode = 2)
	{
		if ( p_nSymbolCount <= 0 ) 
			return 0;

		TReal tProbability = ((TReal)1.0 / p_nSymbolCount);
		return p_nSymbolCount * -(tProbability * TMaths<TReal>::LogBase( tProbability, p_nCode ));
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	static TReal InverseErf(TReal p_tX)
	{
		// Inverse Error Function, source: http://functions.wolfram.com/GammaBetaErf/InverseErf/06/01/
		//
		//					   Ck	     Sqrt(Pi)
		// erf^-1(x) = SUM( -------- * ( -------- * x) ^ 2k+1, k=0 to Inf )
		//                  (2k + 1)         2
		//					
		// C0 = 1
		//			 Cm * Ck - 1 - m 
		// Ck = SUM( ---------------, m=0 to k-1)
		//			 (m + 1)(2m + 1)
		//

		// Using Peter J. Acklam's algorithm:
		// http://home.online.no/~pjacklam/notes/invnorm/

		static const long double A1 = -3.969683028665376e+01,
								 A2 =  2.209460984245205e+02,
								 A3 = -2.759285104469687e+02,
								 A4 =  1.383577518672690e+02,
								 A5 = -3.066479806614716e+01,
								 A6 =  2.506628277459239e+00;

		static const long double B1 = -5.447609879822406e+01,
								 B2 =  1.615858368580409e+02,
								 B3 = -1.556989798598866e+02,
								 B4 =  6.680131188771972e+01,
								 B5 = -1.328068155288572e+01;

		static const long double C1 = -7.784894002430293e-03,
								 C2 = -3.223964580411365e-01,
								 C3 = -2.400758277161838e+00,
								 C4 = -2.549732539343734e+00,
								 C5 =  4.374664141464968e+00,
								 C6 =  2.938163982698783e+00;

		static const long double D1 =  7.784695709041462e-03,
								 D2 =  3.224671290700398e-01,
								 D3 =  2.445134137142996e+00,
								 D4 =  3.754408661907416e+00;

		static const long double ldfXLow  = 0.02425,
								 ldfXHigh = 0.97575;

		long double q, r;

		// 0 < X < Low
		if ( p_tX > 0 && p_tX < ldfXLow )
		{
			q = TMaths<TReal>::Sqrt( -2 * TMaths<TReal>::Ln( p_tX ) );
			return (TReal)(((((C1 * q + C2) * q + C3) * q + C4) * q + C5) * q + C6) / ((((D1 * q + D2) * q + D3) * q + D4) * q + 1);
		}
		
		// Low <= X <= High
		if ( p_tX >= ldfXLow && p_tX <= ldfXHigh )
		{
			q = p_tX - 0.5;
			r = q * q;
			return (TReal)(((((A1 * r + A2) * r + A3) * r + A4) * r + A5) * r + A6) * q /(((((B1 * r + B2) * r + B3) * r + B4) * r + B5) * r + 1);
		}
		
		// High < X < 1
		if ( p_tX > ldfXHigh && p_tX < 1 )
		{
			q = TMaths<TReal>::Sqrt( -2 * TMaths<TReal>::Ln( 1 - p_tX ) );
			return (TReal) -(((((C1 * q + C2) * q + C3) * q + C4) * q + C5) * q + C6) / ((((D1 * q + D2) * q + D3) * q + D4) * q + 1);
		} 
		
		// 0 <= X
		if ( p_tX < TMaths<TReal>::Epsilon )
			return TMaths<TReal>::Minimum;

		// 1 <= X
		return TMaths<TReal>::Maximum;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	static TReal Erf(TReal p_tX, TReal p_tTolerance = TMaths<TReal>::Tolerance, int p_nMaxTerms = 32)
	{
		// 
		//              2	  		  -1^n * x^(2n+1)    
		// erf(x) =  -------  * SUM ( ---------------, n=0 to Inf)
		//			 Sqrt(Pi)             n!(2n+1)
		//		

		static const TReal t2SqrtPi = 2 * TMaths<TReal>::InvSqrtPi;

		TReal tSum = p_tX, 
			  tTerm = p_tX, 
			  tXSqr = p_tX * p_tX;
		
		int nTerm = 1, nTermCount = 1;

		do 
		{
			// Unroll loop for -/+ terms
			tTerm *= tXSqr / nTerm;
			tSum -= tTerm / (2 * nTerm + 1);
			nTerm++;

			tTerm *= tXSqr / nTerm;
			tSum += tTerm / (2 * nTerm + 1);
			nTerm++;

			// Update term count
			nTermCount += 2;
		} while (TMaths<TReal>::FAbs( tTerm / tSum ) > p_tTolerance && nTermCount < p_nMaxTerms );

		return t2SqrtPi * tSum;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	static TReal Erfc(TReal p_tX, TReal p_tTolerance = TMaths<TReal>::Tolerance, int p_nMaxTerms = 32)
	{
		return 1.0 - Erf(p_tX, p_tTolerance, p_nMaxTerms );
	}

	//----------------------------------------------------------------------------------------------
	/// Evaluates the Gaussian probability density function.
	//----------------------------------------------------------------------------------------------
	static TReal GaussianPDF(TReal p_tX, TReal p_tMean = 0, TReal p_tStandardDeviation = 1)
	{
		// f(x ; m,s) =          1				  (x - m)^2
		//				------------------ exp( - --------- )
		//				s * sqrt( 2 * Pi )           2s^2

		TReal tSqrt2Pi = TMaths<TReal>::Sqrt(TMaths<TReal>::PiTwo);
		TReal tLeft = 1 / (p_tStandardDeviation * tSqrt2Pi );
		TReal tNum = TMaths<TReal>::Sqr(p_tX - p_tMean);
		TReal tDen = 2 * p_tStandardDeviation * p_tStandardDeviation;
		return tLeft * TMaths<TReal>::Exp( -(tNum/tDen) );
	}

	//----------------------------------------------------------------------------------------------
	/// Evaluates the Gaussian cumulative density function.
	//----------------------------------------------------------------------------------------------
	static TReal GaussianCDF(TReal p_tX, TReal p_tMean = 0, TReal p_tStandardDeviation = 1, TReal p_tTolerance = TMaths<TReal>::Tolerance, int p_nMaxTerms = 32)
	{
		// f(x ; m,s) =   0.5 * ( 1 + erf( (x - m) / (s * Sqrt(2)) )

		static const TReal tSqrtTwo = TMaths<TReal>::Sqrt( 2.0 );
		return 0.5 * ( 1 + Erf( (p_tX - p_tMean) / (p_tStandardDeviation * tSqrtTwo), p_tTolerance, p_nMaxTerms ) );
	}

	//----------------------------------------------------------------------------------------------
	/// Evaluates the Gaussian probability density function with mean zero and standard deviation one.
	//----------------------------------------------------------------------------------------------
	static TReal NormalPDF(TReal p_tX)
	{
		// f(x) =       1				x^2
		//		  -------------- exp( - --- )
		//		  sqrt( 2 * Pi )         2

		return TMaths<TReal>::InvSqrtPiTwo * TMaths<TReal>::Exp( -0.5 * (p_tX * p_tX) );
	}

	//----------------------------------------------------------------------------------------------
	/// Evaluates the Gaussian cumulative density function with mean zero and standard deviatin one.
	//----------------------------------------------------------------------------------------------
	static TReal NormalCDF(TReal p_tX, TReal p_tTolerance = TMaths<TReal>::Tolerance, int p_nMaxTerms = 32)
	{
		// f(x) = 0.5 * ( 1 + erf( x / Sqrt(2) )

		static const TReal tInvSqrtTwo = (TReal)1.0 / TMaths<TReal>::Sqrt( 2.0 );
		return 0.5 * ( 1 + Erf( p_tX * tInvSqrtTwo, p_tTolerance, p_nMaxTerms ) );
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the Exponential probability density function.
	//----------------------------------------------------------------------------------------------
	static TReal ExponentialPDF(TReal p_tX, TReal p_tLambda = 1)
	{
		// f(x) = le^-lx		l>0, x>=0
		//      = 0				elsewhere

		if ( p_tX < 0 || p_tLambda <= 0 )
			return 0;

		return p_tLambda * TMaths<TReal>::Exp( -p_tLambda * p_tX );
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the Exponential cumulative distribution function.
	//----------------------------------------------------------------------------------------------
	static TReal ExponentialCDF(TReal p_tX, TReal p_tLambda = 1)
	{
		// f(x) = 1 - e^-lx
		
		return 1 - TMaths<TReal>::Exp( -p_tLambda * p_tX );
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the inverse of the Exponential cumulative distribution function.
	//----------------------------------------------------------------------------------------------
	static TReal InverseExponentialCDF(TReal p_tX, TReal p_tLambda = 1)
	{
		// f(x) = -log(x) * 1 / l 

		return -( 1 / p_tLambda ) * TMaths<TReal>::Log( p_tX );
	}
};

typedef TStatistics<double>	Statistics;
typedef TStatistics<float>	Statisticsf;

Meson_Common_Maths_END