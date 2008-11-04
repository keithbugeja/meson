//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::TVector4<T>
//	Templated class for 4D vectors.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "DebugSupport.h"
#include "Maths.h"

#include "Vector3.h"
#include "Vector2.h"

Meson_Common_Maths_BEGIN

// Disable DLL export warnings
#pragma warning (disable:4251)

template <typename TReal> 
class TVector4
{
public:
	//----------------------------------------------------------------------------------------------
	/// Constructs an unitialised 4D vector.
	//----------------------------------------------------------------------------------------------
	TVector4( void ) {}	

	//----------------------------------------------------------------------------------------------
	/// 4D vector copy constructor.
	/// \param p_vector A 4D vector object to use as source for copy.
	//----------------------------------------------------------------------------------------------
	TVector4( const TVector4 &p_vector ) : X(p_vector.X), Y(p_vector.Y), Z(p_vector.Z), W(p_vector.W) { }

	//----------------------------------------------------------------------------------------------
	/// 4D vector copy constructor.
	/// \param p_vector A 4D vector object to use as source for copy.
	//----------------------------------------------------------------------------------------------
	TVector4( const TVector3<TReal>& p_vector ) 
	{
		*this = p_vector;
	}
	
	//----------------------------------------------------------------------------------------------
	/// 4D vector copy constructor.
	/// \param p_vector A 4D vector object to use as source for copy.
	//----------------------------------------------------------------------------------------------
	TVector4( const TVector2<TReal>& p_vector ) 
	{
		*this = p_vector;
	}

	//----------------------------------------------------------------------------------------------
	/// Constructs and initialises a 4D vector through single components.
	/// \param p_tX	X component
	/// \param p_tX	Y component
	/// \param p_tX	Z component
	/// \param p_tX	W component
	//----------------------------------------------------------------------------------------------
	TVector4( const TReal p_tX, const TReal p_tY, const TReal p_tZ, const TReal p_tW ) : X(p_tX), Y(p_tY), Z(p_tZ), W(p_tW) { }
	
	//----------------------------------------------------------------------------------------------
	/// Construct a vector with components initialised to the given scalar.
	//----------------------------------------------------------------------------------------------
	TVector4(TReal p_tValue) : X(p_tValue), Y(p_tValue), Z(p_tValue), W(p_tValue) { }

	//----------------------------------------------------------------------------------------------
	/// Clears the vector components to zero.
	//----------------------------------------------------------------------------------------------
	void MakeZero( void ) 
	{
		X = Y = Z = W = (TReal)0.0;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Returns whether the vector components are zero.
	/// \returns True if the vector is zero, false otherwise
	//----------------------------------------------------------------------------------------------
	bool IsZero( void ) const
	{
		return ( TMaths<TReal>::Equals( X, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( Y, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( Z, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( W, (TReal)0.0 ) );
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the square of the magnitude of the vector.
	/// \returns Vector magnitude<sup>2</sup>
	//----------------------------------------------------------------------------------------------
	TReal LengthSquared(void) const
	{
		return X*X + Y*Y + Z*Z + W*W;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the magnitude of the vector.
	/// \returns Vector magnitude
	//----------------------------------------------------------------------------------------------
	TReal Length(void) const
	{
		return TMaths<TReal>::Sqrt(X*X + Y*Y + Z*Z + W*W);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the dot product between two vectors.
	/// \param p_vector Vector dot product operand
	/// \returns The dot product between the two vectors
	//----------------------------------------------------------------------------------------------
	TReal DotProduct( const TVector4& p_vector ) const
	{
		return X * p_vector.X + Y * p_vector.Y + Z * p_vector.Z + W * p_vector.W;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the cross product between three vectors.
	/// \param p_vector1 Vector cross product operand
	/// \param p_vector2 Vector cross product operand
	/// \returns Resultant vector from cross product operation
	//----------------------------------------------------------------------------------------------
	TVector4 CrossProduct( const TVector4& p_vector1, const TVector4& p_vector2 ) const
	{
		#define DETERMINANT3X3(a, b, c, d, e, f, g, h, i) (a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g))

		return TVector4(   DETERMINANT3X3( Y, Z, W, p_vector1.Y, p_vector1.Z, p_vector1.W, p_vector2.Y, p_vector2.Z, p_vector2.W ), 
						 - DETERMINANT3X3( X, Z, W, p_vector1.X, p_vector1.Z, p_vector1.W, p_vector2.X, p_vector2.Z, p_vector2.W ),
						   DETERMINANT3X3( X, Y, W, p_vector1.X, p_vector1.Y, p_vector1.W, p_vector2.X, p_vector2.Y, p_vector2.W ),
						 - DETERMINANT3X3( X, Y, Z, p_vector1.X, p_vector1.Y, p_vector1.Z, p_vector2.X, p_vector2.Y, p_vector2.Z ) );
		
		#undef determinant
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the cross product, assuming the third vector to be <0, 0, 0, 1>.
	/// \param p_vector Vector cross product operand
	/// \returns Resultant vector from cross product operation
	//----------------------------------------------------------------------------------------------
	TVector4 CrossProduct( const TVector4& p_vector ) const
	{
		return TVector4(Y * p_vector.Z - Z * p_vector.Y, Z * p_vector.X - X * p_vector.Z, X * p_vector.Y - Y * p_vector.X, 0);
	}
	
	//----------------------------------------------------------------------------------------------
	void ComponentProduct(const TVector4& p_vector)
	{
		X*=p_vector.X;
		Y*=p_vector.Y;
		Z*=p_vector.Z;
		W*=p_vector.W;
	}

	//----------------------------------------------------------------------------------------------
	TVector4 ComponentProductCopy(const TVector4& p_vector) const
	{
		return TVector4(X*p_vector.X, Y*p_vector.Y, Z*p_vector.Z, W*p_vector.W);
	}

	//----------------------------------------------------------------------------------------------
	/// Negates the vector.
	//----------------------------------------------------------------------------------------------
	void Negate(void) {
		X=-X; Y=-Y; Z=-Z; W=-W;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Returns the negation of the vector.
	/// \returns Negation
	//----------------------------------------------------------------------------------------------
	TVector4 NegateCopy(void) const
	{
		return TVector4(-X, -Y, -Z, -W);
	}

	//----------------------------------------------------------------------------------------------
	/// Normalises the vector.
	//----------------------------------------------------------------------------------------------
	void Normalise(void) 
	{
		TReal tLength = (TReal)1.0 / Length();
		X *= tLength; Y *= tLength; Z *= tLength; W *= tLength;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the normalised vector.
	/// \returns Normal
	//----------------------------------------------------------------------------------------------
	TVector4 NormaliseCopy(void) const
	{
		TReal tLength = (TReal)1.0 / Length();
		return TVector4( X * tLength, Y * tLength, Z * tLength, W * tLength );
	}
	
	//----------------------------------------------------------------------------------------------
	/// 
	//----------------------------------------------------------------------------------------------
	void Set(TReal p_tX, TReal p_tY, TReal p_tZ, TReal p_tW)
	{
		X = p_tX; Y = p_tY; Z = p_tZ; W = p_tW;
	}
	
	//----------------------------------------------------------------------------------------------
	/// 
	//----------------------------------------------------------------------------------------------
	template <typename TRealParam>
	void Set(const TVector4<TRealParam>& p_vector)
	{
		X = (TReal)p_vector.X; Y = (TReal)p_vector.Y; Z = (TReal)p_vector.Z; W = (TReal)p_vector.W;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the negation of the vector.
	/// \returns Negation
	//----------------------------------------------------------------------------------------------
	TVector4 operator- (void) const
	{
		return NegateCopy();
	}
	
	//----------------------------------------------------------------------------------------------
	/// Computes the difference between two vectors.
	//----------------------------------------------------------------------------------------------
	TVector4 operator- (const TVector4 &p_vector) const
	{
		return TVector4(X - p_vector.X, Y - p_vector.Y, Z - p_vector.Z, W - p_vector.W);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes addition of two vectors.
	//----------------------------------------------------------------------------------------------
	TVector4 operator+ (const TVector4 &p_vector) const
	{
		return TVector4(X + p_vector.X, Y + p_vector.Y, Z + p_vector.Z, W + p_vector.W);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the scalar multiplication of a vector.
	//----------------------------------------------------------------------------------------------
	TVector4 operator* (const TReal p_tValue) const
	{
		return TVector4(X * p_tValue, Y * p_tValue, Z * p_tValue, W * p_tValue);
	}

	friend TVector4 operator* (const TReal p_tValue, const TVector4 p_vector ) {
		return TVector4(p_vector.X * p_tValue, p_vector.Y * p_tValue, p_vector.Z * p_tValue, p_vector.W * p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the scalar division of a vector.
	//----------------------------------------------------------------------------------------------
	TVector4 operator/ (const TReal p_tValue) const
	{
		if ( !TMaths<TReal>::Equals(p_tValue, (TReal)0.0) )
		{
			TReal tInvValue = (TReal)1.0 / p_tValue;
			return TVector4(X * tInvValue, Y * tInvValue, Z * tInvValue, W * tInvValue);
		}

		TReal tValue = TMaths<TReal>::Maximum;
		return TVector4( tValue, tValue, tValue, tValue );
	}
	
	//----------------------------------------------------------------------------------------------
	/// Computes the dot product of two vectors.
	//----------------------------------------------------------------------------------------------
	TReal operator* (const TVector4& p_vector) const
	{
		return DotProduct(p_vector);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the special case 3D cross product of two 4D vectors.
	//----------------------------------------------------------------------------------------------
	TVector4 operator^ (const TVector4& p_vector) const
	{
		return CrossProduct(p_vector);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the difference between two vectors.
	//----------------------------------------------------------------------------------------------
	TVector4& operator-= (const TVector4& p_vector) 
	{
		X-=p_vector.X; 
		Y-=p_vector.Y; 
		Z-=p_vector.Z; 
		W-=p_vector.W;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the addition of two vectors.
	//----------------------------------------------------------------------------------------------
	TVector4& operator+= (const TVector4& p_vector) 
	{
		X+=p_vector.X; 
		Y+=p_vector.Y; 
		Z+=p_vector.Z; 
		W+=p_vector.W;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the scaling by a divisor.
	//----------------------------------------------------------------------------------------------
	TVector4& operator/= (const TReal p_tValue) 
	{
		if ( !TMaths<TReal>::Equals(p_tValue, (TReal)0.0) )
		{
			TReal tInvValue = (TReal)1.0 / p_tValue;
			X*=tInvValue; 
			Y*=tInvValue; 
			Z*=tInvValue; 
			W*=tInvValue;
		} 
		else 
		{
			X = Y = Z = W = TMaths<TReal>::Maximum;
		}

		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the scaling by a multiplicand.
	//----------------------------------------------------------------------------------------------
	TVector4& operator*= (const TReal p_tValue) 
	{
		X*=p_tValue; 
		Y*=p_tValue; 
		Z*=p_tValue; 
		W*=p_tValue;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the cross product.
	//----------------------------------------------------------------------------------------------
	TVector4& operator^= (const TVector4& p_vector)
	{
		TVector4 result = Cross(p_vector);
		X = result.X; 
		Y = result.Y; 
		Z = result.Z; 
		W = result.W;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Performs assignment.
	//----------------------------------------------------------------------------------------------
	TVector4& operator= (const TReal p_tScalar) 
	{
		X = Y = Z = W = p_tScalar;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Performs assignment.
	//----------------------------------------------------------------------------------------------
	TVector4& operator= (const TVector2<TReal>& p_vector) 
	{
		X = p_vector.X; 
		Y = p_vector.Y; 
		Z = W = (TReal)0.0;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Performs assignment.
	//----------------------------------------------------------------------------------------------
	TVector4& operator= (const TVector3<TReal>& p_vector) 
	{
		X = p_vector.X; 
		Y = p_vector.Y; 
		Z = p_vector.Z;
		W = (TReal)0.0;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Performs assignment.
	//----------------------------------------------------------------------------------------------
	TVector4& operator= (const TVector4& p_vector) 
	{
		X = p_vector.X; 
		Y = p_vector.Y; 
		Z = p_vector.Z; 
		W = p_vector.W;
		return *this;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Evaluates the equality operator.
	//----------------------------------------------------------------------------------------------
	bool operator== (const TVector4& p_vector) const
	{
		return ( TMaths<TReal>::Equals( p_vector.X, X ) &&
				 TMaths<TReal>::Equals( p_vector.Y, Y ) &&
				 TMaths<TReal>::Equals( p_vector.Z, Z ) &&
				 TMaths<TReal>::Equals( p_vector.W, W ) );
	}
	//----------------------------------------------------------------------------------------------
	/// Evaluates the inequality operator.
	//----------------------------------------------------------------------------------------------
	bool operator!= (const TVector4& p_vector) const
	{
		return ( !TMaths<TReal>::Equals( p_vector.X, X ) ||
				 !TMaths<TReal>::Equals( p_vector.Y, Y ) ||
				 !TMaths<TReal>::Equals( p_vector.Z, Z ) ||
				 !TMaths<TReal>::Equals( p_vector.W, W ) );
	}

	//----------------------------------------------------------------------------------------------
	/// Cast operations for vector.
	//----------------------------------------------------------------------------------------------
	operator TReal*() 
	{
		return (TReal*)&Element;
	}

	template <typename TRealParam>
	operator TVector4<TRealParam>()
	{
		return TVector4<TRealParam>((TRealParam)X, (TRealParam)Y, (TRealParam)Z, (TRealParam)W);
	}

public:
	//----------------------------------------------------------------------------------------------
	static const TVector4 Zero; 
	static const TVector4 Up; 
	static const TVector4 Down; 
	static const TVector4 Left; 
	static const TVector4 Right; 
	static const TVector4 In; 
	static const TVector4 Out;

	//----------------------------------------------------------------------------------------------
	union 
	{
		TReal Element[4];
		struct
		{
			TReal X, Y, Z, W;
		};
	};
	//----------------------------------------------------------------------------------------------
};

//----------------------------------------------------------------------------------------------
// Static member initialisation
//----------------------------------------------------------------------------------------------
template <typename TReal> TVector4<TReal> const TVector4<TReal>::Zero = TVector4<TReal>((TReal)0.0, (TReal)0.0, (TReal)0.0, (TReal)0.0);
template <typename TReal> TVector4<TReal> const TVector4<TReal>::Up = TVector4<TReal>((TReal)0.0, (TReal)1.0, (TReal)0.0, (TReal)0.0);
template <typename TReal> TVector4<TReal> const TVector4<TReal>::Down = TVector4<TReal>((TReal)0.0, (TReal)-1.0, (TReal)0.0, (TReal)0.0);
template <typename TReal> TVector4<TReal> const TVector4<TReal>::Left = TVector4<TReal>((TReal)-1.0, (TReal)0.0, (TReal)0.0, (TReal)0.0);
template <typename TReal> TVector4<TReal> const TVector4<TReal>::Right = TVector4<TReal>((TReal)1.0, (TReal)0.0, (TReal)0.0, (TReal)0.0);
template <typename TReal> TVector4<TReal> const TVector4<TReal>::In = TVector4<TReal>((TReal)0.0, (TReal)0.0, (TReal)1.0, (TReal)0.0);
template <typename TReal> TVector4<TReal> const TVector4<TReal>::Out = TVector4<TReal>((TReal)0.0, (TReal)0.0, (TReal)-1.0, (TReal)0.0);

//----------------------------------------------------------------------------------------------
// Type definitions
//----------------------------------------------------------------------------------------------
typedef TVector4<double> Vector4;
typedef TVector4<float>  Vector4f;

Meson_Common_Maths_END
