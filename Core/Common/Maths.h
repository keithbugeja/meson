//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::TMaths<T>
//	Templated class for mathematical functions.
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
#include "DebugSupport.h"

// Standard Library Includes
#include <limits>
#include <math.h>

// namespace Meson.Common.Math
Meson_Common_Maths_BEGIN

// TMaths Template
template <typename TReal> 
class TMaths
{
public:
	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	static int Factorial(int p_nValue)
	{
		//MESON_ASSERT( p_nValue >= 0, "Factorial is not defined for negative integers!" );

		int nSum = 1;

		if ( p_nValue > 0 )
			for (int nTerm = 2; nTerm <= p_nValue; nSum *= nTerm, nTerm++);
		
		return nSum;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	static int ISgn(TReal p_tValue)
	{
		if (p_tValue > Epsilon) return 1;
		else if (p_tValue < -Epsilon) return -1;

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	static TReal Sgn(TReal p_tValue) 
	{
		if (p_tValue > Epsilon) return (TReal)1.0;
		else if (p_tValue < -Epsilon) return (TReal)-1.0;

		return (TReal)0.0;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	static TReal Ceiling(TReal p_tValue) {
		return (TReal)ceil((double)p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	static TReal Floor(TReal p_tValue) {
		return (TReal)floor((double)p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline static TReal Abs(TReal p_tValue) {
		return (TReal)fabs((double)p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline static TReal FAbs(TReal p_tValue) {
		return (p_tValue < 0) ? -p_tValue : p_tValue;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline static TReal Mod(TReal p_tValueX, TReal p_tValueY) {
		return fmod( p_tValueX, p_tValueY );
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline static TReal Pow(TReal p_tValue, TReal p_tPow) {
		return pow( p_tValue, p_tPow );
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline static TReal Pow(TReal p_tValue, int p_nPow) {
		return pow( p_tValue, p_nPow );
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline static TReal Sqr(TReal p_tValue) {
		return p_tValue * p_tValue;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline static TReal Sqrt(TReal p_tValue) {
		return (TReal)sqrt((double)p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline static TReal Sin(TReal p_tAngle) {
		return (TReal)sin((double)p_tAngle);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline static TReal ASin(TReal p_tValue) {
		return (TReal)asin((double)p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline static TReal Cos(TReal p_tAngle) {
		return (TReal)cos((double)p_tAngle);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline static TReal ACos(TReal p_tValue) {
		return (TReal)acos((double)p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline static TReal Tan(TReal p_tAngle) {
		return (TReal)tan((double)p_tAngle);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline static TReal ATan(TReal p_tValue) {
		return (TReal)atan( (double)p_tValue );
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline static TReal ATan(TReal p_YValue, TReal p_tXValue) {
		return (TReal)atan2( (double)p_YValue, (double)p_tXValue );
	}

	inline static TReal Cot(TReal p_tValue ) {
		return (TReal)tan((double)(PiHalf - p_tValue));
	}

	inline static TReal Sec(TReal p_tValue ) {
		return (TReal)1.0 / sin((double)p_tValue);
	}

	inline static TReal Cosec(TReal p_tValue ) {
		return (TReal)1.0 / cos((double)p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	/// Fast approximation for the cosine function  (-pi < x < pi).
	/// \param p_tAngle Angle in radians in the range (-pi, pi)
	/// \returns Cosine of angle
	//----------------------------------------------------------------------------------------------
	static TReal FCos1( TReal p_tAngle )
	{
		#define FCOS1_C0 0.5
		#define FCOS1_C1 0.04092
		
		//----------------------------------------------------------------------------------------------
		// cos(x) =   1 - (x^2)/2! + (x^4)/4!, -pi/2 <= x <= pi/2
		//           -1 + (x^2)/2! - (x^4)/4!,  |x| > pi/2
		//----------------------------------------------------------------------------------------------
		if ( p_tAngle < -PiHalf )
		{
			p_tAngle += Pi;
			p_tAngle*=p_tAngle;
			return -1 + p_tAngle * (FCOS1_C0 - FCOS1_C1 * p_tAngle);
		}
		else if ( p_tAngle > PiHalf )
		{
			p_tAngle -= Pi;
			p_tAngle*=p_tAngle;
			return -1 + p_tAngle * (FCOS1_C0 - FCOS1_C1 * p_tAngle);
		}

		p_tAngle*=p_tAngle;
		return 1 + p_tAngle * ( FCOS1_C1 * p_tAngle - FCOS1_C0 );

		#undef FCOS1_C0
		#undef FCOS1_C1
	}

	//----------------------------------------------------------------------------------------------
	/// Fast approximation for the sine function (-pi < x < pi).
	/// \param p_tAngle Angle in radians in the range (-pi,pi)
	/// \returns Sine of angle
	//----------------------------------------------------------------------------------------------
	static TReal FSin1( TReal p_tAngle )
	{
		//----------------------------------------------------------------------------------------------
		// Maclaurin series with slightly modified coefficients s.t. sin(pi/2) = 1
		// sin(x) = x - 0.16667 * x^3 + 0.00786 * x^5, -pi <= x <= pi
		//----------------------------------------------------------------------------------------------
		if ( p_tAngle < -PiHalf )
			p_tAngle = -Pi-p_tAngle; 
		else if ( p_tAngle > PiHalf ) 
			p_tAngle = Pi-p_tAngle;
	
		TReal tAngleSqr = p_tAngle * p_tAngle;
		return p_tAngle + tAngleSqr * p_tAngle * ( 0.00786 * tAngleSqr - 0.16667 );
	}
	
	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline static TReal FTan1( TReal p_tAngle ) 
	{
		return (TReal)tanf( (float)p_tAngle );
	}

	//----------------------------------------------------------------------------------------------
	/// Fast and less accurate approximation for the tangent function for (|x| < pi/2).
	/// \param p_tAngle Angle in radians
	/// \returns Tangent
	//----------------------------------------------------------------------------------------------
	inline static TReal FTan2( TReal p_tAngle ) 
	{
		TReal tAngleSqr = p_tAngle * p_tAngle;
		return p_tAngle + (tAngleSqr * p_tAngle) * (0.33333 + tAngleSqr * 0.13333);
	}

	//----------------------------------------------------------------------------------------------
	/// Fast and less accurate approximation for the sine function in the range (-pi,pi).
	/// \param p_tAngle Angle in radians in the range (-pi,pi)
	/// \returns Sine of angle
	//----------------------------------------------------------------------------------------------
	static TReal FCos2( TReal p_tAngle )
	{
		// Adapted from http://lab.polygonal.de/wp-content/articles/fast_trig/fastTrig.as
		p_tAngle += PiHalf;

		if (p_tAngle > Pi ) p_tAngle -= PiTwo;
		else if (p_tAngle <-Pi ) p_tAngle += PiTwo;

		return (p_tAngle > 0) ? p_tAngle * (1.27323954 - 0.405284735 * p_tAngle)
							  : p_tAngle * (1.27323954 + 0.405284735 * p_tAngle) ;
	}

	//----------------------------------------------------------------------------------------------
	/// Fast and less accurate approximation for the sine function in the range (-pi,pi).
	/// \param p_tAngle Angle in radians in the range (-pi,pi)
	/// \returns Sine of angle
	//----------------------------------------------------------------------------------------------
	static TReal FSin2( TReal p_tAngle )
	{
		// Adapted from http://lab.polygonal.de/wp-content/articles/fast_trig/fastTrig.as
		if (p_tAngle > Pi ) p_tAngle -= PiTwo;
		else if (p_tAngle <-Pi ) p_tAngle += PiTwo;

		return (p_tAngle > 0) ? p_tAngle * (1.27323954 - 0.405284735 * p_tAngle)
							  : p_tAngle * (1.27323954 + 0.405284735 * p_tAngle);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	static TReal FMod( TReal p_tValueX, TReal p_tValueY) {
		return (TReal)fmod((double)p_tValueX, (double)p_tValueY);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	static TReal Exp( TReal p_tValue ) {
		return exp( p_tValue );
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	static TReal Exp( TReal p_tValue, TReal p_tPrecision )
	{
		// Taken from : A note on approximating e^x by S. Lubkin
		//         _                    _  
		// e^x =  |  (6n^2 + 4nx + x^2)  |^n
		//        |  ------------------  |
		//		  |_    2n * (3n - x)   _|
		//

		TReal tResult = ( 6 * p_tPrecision * p_tPrecision + 4 * p_tPrecision * p_tValue + p_tValue * p_tValue ) / 
					    ( 2 * p_tPrecision * ( 3 * p_tPrecision - p_tValue ) );

		return pow( tResult, p_tPrecision );
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	static TReal Ln( TReal p_tValue ) {
		return (TReal)log( (double)p_tValue );
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	static TReal Ld( TReal p_tValue ) {
		return (TReal)log( (double)p_tValue ) / log(2.0);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	static TReal Log( TReal p_tValue ) {
		return (TReal)log10( (double)p_tValue );
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the logarithm of p_tValue in the given base.	
	/// \param p_tBase Logarithm base
	/// \param p_tValue Value
	/// \returns Log of p_tValue in Base p_tBase
	//----------------------------------------------------------------------------------------------
	static TReal LogBase( TReal p_tValue, TReal p_tBase ) {
		return log( p_tValue ) / log( p_tBase );
	}

	//----------------------------------------------------------------------------------------------
	/// Converts an angle in radians to degrees.
	/// \param Angle in radians
	/// \returns Angle in degrees
	//----------------------------------------------------------------------------------------------
	inline static TReal RadianToDegree( TReal p_tAngle ) {
		return (TReal)(p_tAngle * RadToDeg );
	}

	//----------------------------------------------------------------------------------------------
	/// Converts an angle in degrees to radians.
	/// \param Angle in degrees
	/// \returns Angle in radians
	//----------------------------------------------------------------------------------------------
	inline static TReal DegreeToRadian( TReal p_tAngle ) {
		return (TReal)(p_tAngle * DegToRad );
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the minimum of two values.
	/// \param p_tValue1 First number
	/// \param p_tValue2 Second number
	/// \returns Smaller value
	//----------------------------------------------------------------------------------------------
	inline static TReal Min( TReal p_tValue1, TReal p_tValue2 ) {
		return (p_tValue1 < p_tValue2) ? p_tValue1 : p_tValue2;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the given value clamped within the given minimum and maximum values.
	/// \param p_tValue Value to clamp
	/// \param p_tMin Lower clamp limit
	/// \param p_tMax Upper clamp limit
	/// \returns Clamped value
	//----------------------------------------------------------------------------------------------
	inline static TReal Clamp( TReal p_tValue, TReal p_tMin, TReal p_tMax )
	{
		if (p_tValue < p_tMin) return p_tMin;
		if (p_tValue > p_tMax) return p_tMax;
		return p_tValue;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the given value clamped within 0.0 and 1.0.
	/// \param p_tValue Value to clamp
	/// \returns Clamped value
	//----------------------------------------------------------------------------------------------
	inline static TReal Clamp( TReal p_tValue )
	{
		return Clamp(p_tValue, (TReal) 0.0, (TReal) 1.0);
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the maximum of two values.
	/// \param p_tValue1 First number
	/// \param p_tValue2 Second number
	/// \returns Greater value
	//----------------------------------------------------------------------------------------------
	inline static TReal Max( TReal p_tValue1, TReal p_tValue2 ) {
		return (p_tValue1 > p_tValue2) ? p_tValue1 : p_tValue2;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the equality relationship between two values, with a tolerance e(x).
	/// \param p_tValue1 First number
	/// \param p_tValue2 Second number
	/// \param p_Tolerance Tolerance value (default = Epsilon)
	/// \returns true if numbers are equal within tolerance, false if not equal
	//----------------------------------------------------------------------------------------------
	inline static bool Equals( TReal p_tValue1, TReal p_tValue2, TReal p_tTolerance = Epsilon ) {
		return ( Abs(p_tValue1 - p_tValue2) <= Epsilon );
	}

	static const TReal E;
	static const TReal Pi;
	static const TReal PiTwo;
	static const TReal PiHalf;
	static const TReal InvPi;
	static const TReal InvPiTwo;
	static const TReal SqrtPi;
	static const TReal SqrtPiTwo;
	static const TReal InvSqrtPi;
	static const TReal InvSqrtPiTwo;
	static const TReal Epsilon;
	static const TReal Infinity;
	static const TReal Minimum;
	static const TReal Maximum;
	static const TReal DegToRad;
	static const TReal RadToDeg;
	static const TReal Tolerance;
};

// Prevent confusion with min/max macros
#if (defined min) 
	#define ___min min
	#undef min 
#endif

#if (defined max) 
	#define ___max max
	#undef max
#endif

// Tolerance  
template<> const float  TMaths<float>::Tolerance	= 1e-06f;
template<> const double TMaths<double>::Tolerance	= 1e-08;

// E approximation
template <typename TReal> const TReal TMaths<TReal>::E	= (TReal)(TMaths<TReal>::Exp(1.0));

// PI approximation (3.1415926535897932384626433832795)
template <typename TReal> const TReal TMaths<TReal>::Pi				= (TReal)(TMaths<TReal>::ATan(1.0) * 4.0);
template <typename TReal> const TReal TMaths<TReal>::PiTwo			= (TReal)(TMaths<TReal>::ATan(1.0) * 8.0);
template <typename TReal> const TReal TMaths<TReal>::PiHalf			= (TReal)(TMaths<TReal>::ATan(1.0) * 2.0);
template <typename TReal> const TReal TMaths<TReal>::InvPi			= (TReal)(0.25 / TMaths<TReal>::ATan(1.0));
template <typename TReal> const TReal TMaths<TReal>::InvPiTwo		= (TReal)(0.5 / TMaths<TReal>::ATan(1.0));
template <typename TReal> const TReal TMaths<TReal>::SqrtPi			= TMaths<TReal>::Sqrt(TMaths<TReal>::ATan(1.0) * 4.0);
template <typename TReal> const TReal TMaths<TReal>::SqrtPiTwo		= TMaths<TReal>::Sqrt(TMaths<TReal>::ATan(1.0) * 8.0);
template <typename TReal> const TReal TMaths<TReal>::InvSqrtPi		= (TReal)(1.0 / TMaths<TReal>::Sqrt(TMaths<TReal>::ATan(1.0) * 4.0));
template <typename TReal> const TReal TMaths<TReal>::InvSqrtPiTwo	= (TReal)(1.0 / TMaths<TReal>::Sqrt(TMaths<TReal>::ATan(1.0) * 8.0));

// Degree to Radian and Radian to Degree conversion multipliers
template <typename TReal> const TReal TMaths<TReal>::DegToRad = (TReal)(TMaths<TReal>::ATan(1.0) / 45);
template <typename TReal> const TReal TMaths<TReal>::RadToDeg = (TReal)(45 / TMaths<TReal>::ATan(1.0));

// Epsilon
template <typename TReal> const TReal TMaths<TReal>::Epsilon = (TReal)std::numeric_limits<TReal>::epsilon();

// Infinity
template <typename TReal> const TReal TMaths<TReal>::Infinity = (TReal)std::numeric_limits<TReal>::infinity();

// Minimum
template <typename TReal> const TReal TMaths<TReal>::Minimum = (TReal)std::numeric_limits<TReal>::min();

// Maximum
template <typename TReal> const TReal TMaths<TReal>::Maximum = (TReal)std::numeric_limits<TReal>::max();

// Math type defaults to TMath<float>
typedef TMaths<double> Math;
typedef TMaths<float>  Mathf;

// Prevent confusion with min/max macros
#ifdef ___min
	#define min ___min
	#undef ___min
#endif

#ifdef ___max
	#define max ___max
	#undef ___max
#endif

// Namespaces for grouping enumerands
namespace Axis { enum Axis { X, Y, Z }; }
namespace AxisOrder { enum AxisOrder { XYZ, XZY, ZYX, ZXY, YXZ, YZX }; }

// Meson.Common.Maths
Meson_Common_Maths_END
