//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::TVector2<T>
//	Templated class for 2D vectors.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "DebugSupport.h"
#include "Maths.h"

Meson_Common_Maths_BEGIN

template <typename TReal> class TVector2
{
public:
	//----------------------------------------------------------------------------------------------
	/// Constructs an unitialised vector.
	//----------------------------------------------------------------------------------------------
	TVector2(void) {}	

	//----------------------------------------------------------------------------------------------
	/// Copy constructor.
	/// \param p_vector A vector object used as a source for copy
	//----------------------------------------------------------------------------------------------
	TVector2(const TVector2& p_vector) : X(p_vector.X), Y(p_vector.Y) { }

	//----------------------------------------------------------------------------------------------
	/// Construct an initialised vector.
	/// \param p_tX	X component
	/// \param p_tX	Y component
	//----------------------------------------------------------------------------------------------
	TVector2(TReal p_tX, TReal p_tY) : X(p_tX), Y(p_tY) { }
	
	//----------------------------------------------------------------------------------------------
	/// Construct a vector with components initialised to the given scalar.
	//----------------------------------------------------------------------------------------------
	TVector2(TReal p_tValue) : X(p_tValue), Y(p_tValue) { }

	//----------------------------------------------------------------------------------------------
	/// Sets the components of the vector to zero.
	//----------------------------------------------------------------------------------------------
	void MakeZero(void) 
	{
		X = Y = (TReal)0.0;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns whether the vector components are zero.
	/// \returns True if the vector is zero, false otherwise
	//----------------------------------------------------------------------------------------------
	bool IsZero(void) const
	{
		return ( TMaths<TReal>::Equals(X, (TReal)0.0) && 
				 TMaths<TReal>::Equals(Y, (TReal)0.0) );
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the square of the magnitude of the vector.
	/// \returns Square of the magnitude
	//----------------------------------------------------------------------------------------------
	TReal LengthSquared(void) const
	{
		return X*X + Y*Y;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the magnitude of the vector.
	/// \returns Magnitude of the vector
	//----------------------------------------------------------------------------------------------
	TReal Length(void) const
	{
		return TMaths<TReal>::Sqrt(X*X + Y*Y);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the dot product between the vector and a source operand.
	/// \param p_vector Second operand in dot product operation
	/// \returns Dot product
	//----------------------------------------------------------------------------------------------
	TReal DotProduct(const TVector2& p_vector) const
	{
		return X * p_vector.X + Y * p_vector.Y;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the 2D cross product.
	/// \param p_vector Second operand in cross product operation
	/// \returns Result
	//----------------------------------------------------------------------------------------------
	TReal CrossProduct(const TVector2& p_vector) const
	{
		return X * p_vector.Y - p_vector.X * Y;
	}
	
	//----------------------------------------------------------------------------------------------
	void ComponentProduct(const TVector2& p_vector)
	{
		X*=p_vector.X;
		Y*=p_vector.Y;
	}

	//----------------------------------------------------------------------------------------------
	TVector2 ComponentProductCopy(const TVector2& p_vector) const
	{
		return TVector2(X*p_vector.X, Y*p_vector.Y);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes in place the result of the perp operation.
	//----------------------------------------------------------------------------------------------
	void Perpendicular(void) 
	{
		TReal tMinX = -X;
		
		X = Y;
		Y = tMinX;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the result of the perp operation and returns the result.
	/// \returns Result of perp operation
	//----------------------------------------------------------------------------------------------
	inline TVector2 PerpendicularCopy(void) const
	{
		return TVector2(Y, -X);
	}
	
	//----------------------------------------------------------------------------------------------
	/// Negates the vector.
	//----------------------------------------------------------------------------------------------
	void Negate(void) 
	{
		X=-X; 
		Y=-Y;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns a negative copy of the vector.
	/// \returns Negative vector counterpart
	//----------------------------------------------------------------------------------------------
	inline TVector2 NegateCopy(void) const
	{
		return TVector2(-X, -Y);
	}

	//----------------------------------------------------------------------------------------------
	/// Normalises the vector.
	//----------------------------------------------------------------------------------------------
	void Normalise(void) 
	{
		TReal tLength = (TReal)1.0 / Length();
		X *= tLength; 
		Y *= tLength;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns a normalised copy of the vector.
	/// \returns Normal
	//----------------------------------------------------------------------------------------------
	inline TVector2 NormaliseCopy(void) const
	{
		TReal tLength = (TReal)1.0 / Length();
		return TVector2(X * tLength, Y * tLength);
	}
	
	//----------------------------------------------------------------------------------------------
	/// Sets the vector to the specified values.
	//----------------------------------------------------------------------------------------------
	void Set(TReal p_tX, TReal p_tY)
	{
		X = p_tX; Y = p_tY;
	}

	template <typename TRealParam>
	void Set(const TVector2<TRealParam>& p_vector)
	{
		X = (TReal)p_vector.X; 
		Y = (TReal)p_vector.Y;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns a negative copy of the vector.
	/// \returns Negative vector couterpart
	//----------------------------------------------------------------------------------------------
	TVector2 operator- (void) const
	{
		return NegateCopy();
	}
	
	//----------------------------------------------------------------------------------------------
	/// Computes the difference between two vectors.
	/// \returns Difference vector
	//----------------------------------------------------------------------------------------------
	TVector2 operator- (const TVector2& p_vector) const
	{
		return TVector2(X - p_vector.X, Y - p_vector.Y);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes addition of two vectors.
	/// \returns Sum vector
	//----------------------------------------------------------------------------------------------
	TVector2 operator+ (const TVector2& p_vector) const
	{
		return TVector2(X + p_vector.X, Y + p_vector.Y);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the scalar multiplication of a vector.
	/// \returns Scaled vector
	//----------------------------------------------------------------------------------------------
	TVector2 operator* (const TReal p_tValue) const
	{
		return TVector2(X * p_tValue, Y * p_tValue);
	}

	friend TVector2 operator* (const TReal p_tValue, const TVector2 p_vector ) 
	{
		return TVector2(p_vector.X * p_tValue, p_vector.Y * p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the scalar division of a vector.
	/// \returns Scaled vector
	//----------------------------------------------------------------------------------------------
	TVector2 operator/ (const TReal p_tValue) const
	{
		if ( !TMaths<TReal>::Equals(p_tValue, (TReal)0.0) )
		{
			TReal tInvValue = (TReal)1.0 / p_tValue;
			return TVector2(X * tInvValue, Y * tInvValue);
		}

		TReal tValue = TMaths<TReal>::Maximum;
		return TVector2( tValue, tValue );
	}
	
	//----------------------------------------------------------------------------------------------
	/// Computes the dot product of two vectors.
	/// \returns Dot product
	//----------------------------------------------------------------------------------------------
	TReal operator* (const TVector2& p_vector) const
	{
		return DotProduct(p_vector);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the cross product of two vectors.
	/// \returns Cross product
	//----------------------------------------------------------------------------------------------
	TReal operator^ (const TVector2& p_vector) const
	{
		return CrossProduct(p_vector);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the difference between two vectors.
	//----------------------------------------------------------------------------------------------
	TVector2& operator-= (const TVector2& p_vector) 
	{
		X-=p_vector.X; 
		Y-=p_vector.Y;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the addition of two vectors.
	//----------------------------------------------------------------------------------------------
	TVector2& operator+= (const TVector2& p_vector) 
	{
		X+=p_vector.X; 
		Y+=p_vector.Y;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the scaling by a divisor.
	//----------------------------------------------------------------------------------------------
	TVector2& operator/= (TReal p_tValue) 
	{
		if ( !TMaths<TReal>::Equals(p_tValue, (TReal)0.0) )
		{
			TReal tInvValue = (TReal)1.0 / p_tValue;
			X*=tInvValue; 
			Y*=tInvValue;
		} 
		else 
		{
			X = Y = TMaths<TReal>::Maximum;
		}

		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the scaling by a multiplicand.
	//----------------------------------------------------------------------------------------------
	TVector2& operator*= (TReal p_tValue) 
	{
		X*=p_tValue; 
		Y*=p_tValue;
		return *this;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Performs assignment.
	//----------------------------------------------------------------------------------------------
	TVector2& operator= (TReal p_tScalar) 
	{
		X = Y = p_tScalar; 
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Performs assignment.
	//----------------------------------------------------------------------------------------------
	TVector2& operator= (const TVector2& p_vector) 
	{
		X = p_vector.X; 
		Y = p_vector.Y;
		return *this;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Evaluates the equality operator.
	//----------------------------------------------------------------------------------------------
	bool operator== (const TVector2& p_vector) const
	{
		return ( TMaths<TReal>::Equals(p_vector.X, X) && 
				 TMaths<TReal>::Equals(p_vector.Y, Y) );
	}

	//----------------------------------------------------------------------------------------------
	/// Evaluates the inequality operator.
	//----------------------------------------------------------------------------------------------
	bool operator!= (const TVector2& p_vector) const
	{
		return ( !TMaths<TReal>::Equals(p_vector.X, X) || 
				 !TMaths<TReal>::Equals(p_vector.Y, Y) );
	}

	//----------------------------------------------------------------------------------------------
	/// Cast operations for vector.
	//----------------------------------------------------------------------------------------------
	operator TReal*() const
	{
		return (TReal*)&Element;
	}

	template <typename TRealParam>
	operator TVector2<TRealParam>() const
	{
		return TVector2<TRealParam>((TRealParam)X, (TRealParam)Y);
	}

public:
	//----------------------------------------------------------------------------------------------
	static const TVector2 Zero; 
	static const TVector2 Up; 
	static const TVector2 Down; 
	static const TVector2 Left; 
	static const TVector2 Right;

	//----------------------------------------------------------------------------------------------
	union 
	{
		TReal Element[2];
		struct
		{
			TReal X, Y;
		};
	};
	//----------------------------------------------------------------------------------------------
};

//----------------------------------------------------------------------------------------------
// Static member initialisation
//----------------------------------------------------------------------------------------------
template <typename TReal> TVector2<TReal> const TVector2<TReal>::Zero = TVector2<TReal>((TReal)0.0, (TReal)0.0);
template <typename TReal> TVector2<TReal> const TVector2<TReal>::Up = TVector2<TReal>((TReal)0.0, (TReal)1.0);
template <typename TReal> TVector2<TReal> const TVector2<TReal>::Down = TVector2<TReal>((TReal)0.0, (TReal)-1.0);
template <typename TReal> TVector2<TReal> const TVector2<TReal>::Left = TVector2<TReal>((TReal)-1.0, (TReal)0.0);
template <typename TReal> TVector2<TReal> const TVector2<TReal>::Right = TVector2<TReal>((TReal)1.0, (TReal)0.0);

//----------------------------------------------------------------------------------------------
// Type definitions
//----------------------------------------------------------------------------------------------
typedef TVector2<double> Vector2;
typedef TVector2<float>	 Vector2f;

Meson_Common_Maths_END
