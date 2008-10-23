//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::TComplex<T>
//	Templated class for complex numbers.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		01/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "DebugSupport.h"
#include "Maths.h"

Meson_Common_Maths_BEGIN

template <class TReal> class TComplex
{
public:
	//----------------------------------------------------------------------------------------------
	/// Constructs an unitialised complex number.
	//----------------------------------------------------------------------------------------------
	TComplex( void ) {}	

	//----------------------------------------------------------------------------------------------
	/// Copy constructor.
	/// \param p_complex A complex number object used as a source for copy
	//----------------------------------------------------------------------------------------------
	TComplex( const TComplex &p_complex ) : R(p_complex.R), I(p_complex.I) { }

	//----------------------------------------------------------------------------------------------
	/// Copy constructor.
	/// \param p_complex A complex number object used as a source for copy
	//----------------------------------------------------------------------------------------------
	TComplex( const TReal &p_real) : R(p_real), I(0) { }

	//----------------------------------------------------------------------------------------------
	/// Construct an initialised complex number.
	/// \param p_tR	Real component
	/// \param p_tI	Imaginary component
	//----------------------------------------------------------------------------------------------
	TComplex( const TReal p_tR, const TReal p_tI ) : R(p_tR), I(p_tI) { }

	//----------------------------------------------------------------------------------------------
	/// Sets the components of the complex number to zero.
	//----------------------------------------------------------------------------------------------
	void MakeZero( void ) 
	{
		R = I = (TReal)0.0;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns whether the complex number is zero.
	/// \returns True if the complex number is zero, false otherwise
	//----------------------------------------------------------------------------------------------
	bool IsZero( void ) const
	{
		return ( TMaths<TReal>::Equals(R, (TReal)0.0) && 
				 TMaths<TReal>::Equals(I, (TReal)0.0) );
	}

	//----------------------------------------------------------------------------------------------
	/// Returns whether the complex number lies on the real axis.
	/// \returns True if the complex number is real, false otherwise
	//----------------------------------------------------------------------------------------------
	bool IsReal( void ) const
	{
		return ( TMaths<TReal>::Equals(I, (TReal)0.0) );
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the magnitude of the complex number.
	/// \returns Magnitude of the complex number
	//----------------------------------------------------------------------------------------------
	TReal Magnitude(void) const
	{
		return TMaths<TReal>::Sqrt(R*R + I*I);
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the argument of the complex number.
	/// \returns Argument of the complex number
	//----------------------------------------------------------------------------------------------
	TReal Argument(void) const
	{
		return TMaths<TReal>::ATan(I, R);
	}

	//----------------------------------------------------------------------------------------------
	/// Multiples the complex number with the given complex number.
	/// \param p_complex Second operand in complex multiplication
	/// \returns Result
	//----------------------------------------------------------------------------------------------
	TComplex Multiply( const TComplex &p_complex ) const
	{
		return TComplex(R * p_complex.R - I * p_complex.I, I * p_complex.R + R * p_complex.I);
	}

	//----------------------------------------------------------------------------------------------
	/// Divides the complex number with the given complex number.
	/// \param p_complex Second operand in complex division
	/// \returns Result
	//----------------------------------------------------------------------------------------------
	TComplex Divide( const TComplex &p_complex ) const
	{
		if ( TMaths<TReal>::Equals(p_complex.Magnitude(), (TReal)0.0) )
		{
			TReal tValue = TMaths<TReal>::Maximum;
			return TComplex( tValue, tValue );
		}

		TReal rDenominatorInverse = (TReal)1.0
			/ (p_complex.R * p_complex.R + p_complex.I * p_complex.I);

		return TComplex(
			(R * p_complex.R + I * p_complex.I) * rDenominatorInverse,
			(I * p_complex.R - R * p_complex.I) * rDenominatorInverse);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the scalar power of a complex number.
	/// \returns Scaled complex number
	//----------------------------------------------------------------------------------------------
	TComplex PowerCopy(const TReal p_tValue) const
	{
		TReal rMagnitude = TMaths<TReal>::Pow(Magnitude(), p_tValue);
		TReal rArgument = Argument() * p_tValue;
		return TComplex(
			rMagnitude * TMaths<TReal>::Cos(rArgument),
			rMagnitude * TMaths<TReal>::Sin(rArgument));
	}

	void Power(const TReal p_tValue) const
	{
		TReal rMagnitude = TMaths<TReal>::Pow(Magnitude(), p_tValue);
		TReal rArgument = Argument() * p_tValue;
		R = rMagnitude * TMaths<TReal>::Cos(rArgument);
		I = rMagnitude * TMaths<TReal>::Sin(rArgument);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes in place the conjugate of the complex number.
	//----------------------------------------------------------------------------------------------
	void Conjugate( void ) 
	{
		I = -I;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the conjugate and returns the result.
	/// \returns Result of conjugation
	//----------------------------------------------------------------------------------------------
	inline TComplex ConjugateCopy( void ) const
	{
		return TComplex( R, -I );
	}
	
	//----------------------------------------------------------------------------------------------
	/// Negates the complex number.
	//----------------------------------------------------------------------------------------------
	void Negate(void) 
	{
		R = -R; 
		I = -I;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns a negative copy of the complex number.
	/// \returns Negative complex counterpart
	//----------------------------------------------------------------------------------------------
	inline TComplex NegateCopy(void) const
	{
		return TComplex(-R, -I);
	}

	//----------------------------------------------------------------------------------------------
	/// Normalises the complex number.
	//----------------------------------------------------------------------------------------------
	void Normalise(void) 
	{
		TReal tLengthInverse = (TReal)1.0 / Length();
		R *= tLengthInverse; 
		I *= tLengthInverse;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns a normalised copy of the complex number.
	/// \returns Normalised complex number
	//----------------------------------------------------------------------------------------------
	inline TComplex NormaliseCopy(void) const
	{
		TReal tLengthInverse = (TReal)1.0 / Length();
		return TComplex( R * tLength, I * tLength );
	}


	//----------------------------------------------------------------------------------------------
	/// Returns a negative copy of the complex number.
	/// \returns Negative complex couterpart
	//----------------------------------------------------------------------------------------------
	TComplex operator- (void) const
	{
		return NegateCopy();
	}
	
	//----------------------------------------------------------------------------------------------
	/// Computes the difference between two complex numbers.
	/// \returns Difference complex number
	//----------------------------------------------------------------------------------------------
	TComplex operator- (const TComplex &p_complex) const
	{
		return TComplex(R - p_complex.R, I - p_complex.I);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes addition of two complex numbers.
	/// \returns Sum vector
	//----------------------------------------------------------------------------------------------
	TComplex operator+ (const TComplex &p_complex) const
	{
		return TComplex(R + p_complex.R, I + p_complex.I);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the scalar multiplication of a complex number.
	/// \returns Scaled complex number
	//----------------------------------------------------------------------------------------------
	TComplex operator* (const TReal p_tValue) const
	{
		return TComplex(R * p_tValue, I * p_tValue);
	}

	friend TComplex operator* (const TReal p_tValue, const TComplex p_complex ) 
	{
		return TComplex(p_complex.R * p_tValue, p_complex.I * p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the scalar division of a complex number.
	/// \returns Scaled complex number
	//----------------------------------------------------------------------------------------------
	TComplex operator/ (const TReal p_tValue) const
	{
		if ( !TMaths<TReal>::Equals(p_tValue, (TReal)0.0) )
		{
			TReal tInvValue = (TReal)1.0 / p_tValue;
			return TComplex(R * tInvValue, I * tInvValue);
		}

		TReal tValue = TMaths<TReal>::Maximum;
		return TComplex( tValue, tValue );
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the scalar power of a complex number.
	/// \returns Scaled complex number
	//----------------------------------------------------------------------------------------------
	TComplex operator^ (const TReal p_tValue) const
	{
		return PowerCopy(p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the product of two complex numbers.
	/// \returns Complex product
	//----------------------------------------------------------------------------------------------
	TComplex operator* (const TComplex &p_complex) const
	{
		return Multiply(p_complex);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the division of two complex numbers.
	/// \returns Complex division
	//----------------------------------------------------------------------------------------------
	TComplex operator/ (const TComplex &p_complex) const
	{
		return Divide(p_complex);
	}

	//----------------------------------------------------------------------------------------------
	/// Returns a conjugate copy of the complex number.
	/// \returns Negative complex couterpart
	//----------------------------------------------------------------------------------------------
	TComplex operator~ (void) const
	{
		return ConjugateCopy();
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the difference between two complex numbers.
	//----------------------------------------------------------------------------------------------
	TComplex& operator-= (const TComplex &p_complex) 
	{
		R -= p_complex.R; 
		I -= p_complex.I;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the addition of two complex numbers.
	//----------------------------------------------------------------------------------------------
	TComplex& operator+= (const TComplex &p_complex) 
	{
		R += p_complex.R; 
		I += p_complex.I;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the scaling by a divisor.
	//----------------------------------------------------------------------------------------------
	TComplex& operator/= (const TReal p_tValue) 
	{
		if ( !TMaths<TReal>::Equals(p_tValue, (TReal)0.0) )
		{
			TReal tInvValue = (TReal)1.0 / p_tValue;
			R*=tInvValue; 
			I*=tInvValue;
		} 
		else 
		{
			R = I = TMaths<TReal>::Maximum;
		}

		return *this;
	}

	TComplex& operator/= (const TComplex p_complex) 
	{
		if ( TMaths<TReal>::Equals(p_complex.Magnitude(), (TReal)0.0) )
		{
			R = I = TReal tValue = TMaths<TReal>::Maximum;
			return *this;
		}

		TReal rDenominatorInverse = (TReal)1.0
			/ (p_complex.R * p_complex.R + p_complex.I * p_complex.I);

		TReal rNewR = (R * p_complex.R + I * p_complex.I) * rDenominatorInverse;
		I = (I * p_complex.R - R * p_complex.I) * rDenominatorInverse;
		R = rNewR;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the scaling by a multiplicand.
	//----------------------------------------------------------------------------------------------
	TComplex& operator*= (const TReal p_tValue) 
	{
		R*=p_tValue; 
		I*=p_tValue;
		return *this;
	}

	TComplex& operator*= (const TComplex p_complex) 
	{
		TReal rNewR = R * p_complex.R - I * p_complex.I;
		I = I * p_complex.R + R * p_complex.I;
		R = rNewR;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Performs assignment.
	//----------------------------------------------------------------------------------------------
	TComplex& operator= (const TReal p_tScalar) 
	{
		R = p_tScalar;
		I = 0.0;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Performs assignment.
	//----------------------------------------------------------------------------------------------
	TComplex& operator= (const TComplex &p_complex) 
	{
		R = p_complex.R; 
		I = p_complex.I;
		return *this;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Evaluates the equality operator.
	//----------------------------------------------------------------------------------------------
	bool operator== (const TComplex &p_complex) const
	{
		return ( TMaths<TReal>::Equals(p_complex.R, R) && 
				 TMaths<TReal>::Equals(p_complex.I, I) );
	}

	//----------------------------------------------------------------------------------------------
	/// Evaluates the inequality operator.
	//----------------------------------------------------------------------------------------------
	bool operator!= (const TComplex &p_complex) const
	{
		return ( !TMaths<TReal>::Equals(p_complex.R, R) || 
				 !TMaths<TReal>::Equals(p_complex.I, I) );
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the quaternion array list under the cast operator.
	//----------------------------------------------------------------------------------------------
	operator TReal*() const
	{
		return (TReal*)&Element;
	}

public:
	//----------------------------------------------------------------------------------------------
	static const TComplex Zero, RealOne, ImaginaryOne;

	//----------------------------------------------------------------------------------------------
	union 
	{
		TReal Element[2];
		struct
		{
			TReal R, I;
		};
	};
	//----------------------------------------------------------------------------------------------
};

//----------------------------------------------------------------------------------------------
// Static member initialisation
//----------------------------------------------------------------------------------------------
template <class TReal> TComplex<TReal> const TComplex<TReal>::Zero = TComplex<TReal>((TReal)0.0, (TReal)0.0);
template <class TReal> TComplex<TReal> const TComplex<TReal>::RealOne = TComplex<TReal>((TReal)1.0, (TReal)0.0);
template <class TReal> TComplex<TReal> const TComplex<TReal>::ImaginaryOne = TComplex<TReal>((TReal)0.0, (TReal)1.0);

//----------------------------------------------------------------------------------------------
// Type definitions
//----------------------------------------------------------------------------------------------
typedef TComplex<double> Complex;
typedef TComplex<float>	 Complexf;

Meson_Common_Maths_END

//----------------------------------------------------------------------------------------------
// Global operators
//----------------------------------------------------------------------------------------------
template <class TReal>
Meson::Common::Maths::TComplex<TReal> operator+(TReal p_rValue,
	const Meson::Common::Maths::TComplex<TReal> p_complex)
{
	return p_complex + p_rValue;
};

template <class TReal>
Meson::Common::Maths::TComplex<TReal> operator-(TReal p_rValue,
	const Meson::Common::Maths::TComplex<TReal> p_complex)
{
	return -p_complex + p_rValue;
};

template <class TReal>
Meson::Common::Maths::TComplex<TReal> operator*(TReal p_rValue,
	const Meson::Common::Maths::TComplex<TReal> p_complex)
{
	return p_complex * p_rValue;
};

template <class TReal>
Meson::Common::Maths::TComplex<TReal> operator/(TReal p_rValue,
	const Meson::Common::Maths::TComplex<TReal> p_complex)
{
	return Meson::Common::Maths::TComplex<TReal>(p_rValue, (TReal)0.0)
		/ p_complex;
};
