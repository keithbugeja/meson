//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::TVector3<T>
//	Templated class for 3D vectors.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "DebugSupport.h"
#include "Maths.h"
#include "Vector2.h"

Meson_Common_Maths_BEGIN

template <typename TReal> 
class TVector3
{
public:
	//----------------------------------------------------------------------------------------------
	/// Constructs an unitialised vector.
	//----------------------------------------------------------------------------------------------
	TVector3(void) {}

	//----------------------------------------------------------------------------------------------
	/// Constructor.
	/// \param p_vector A vector object used as a source for copy
	//----------------------------------------------------------------------------------------------
	TVector3(const TVector3& p_vector) : X(p_vector.X), Y(p_vector.Y), Z(p_vector.Z) { }
	
	//----------------------------------------------------------------------------------------------
	/// Constructor.
	/// \param p_vector A vector object used as a source for copy
	//----------------------------------------------------------------------------------------------
	TVector3(const TVector2<TReal>& p_vector) 
	{
		*this = p_vector;
	}

	//----------------------------------------------------------------------------------------------
	/// Construct an initialised vector.
	/// \param p_tX	X component
	/// \param p_tX	Y component
	/// \param p_tX	Z component
	//----------------------------------------------------------------------------------------------
	TVector3(TReal p_tX, TReal p_tY, TReal p_tZ) : X(p_tX), Y(p_tY), Z(p_tZ) { }
	
	//----------------------------------------------------------------------------------------------
	/// Construct a vector with components initialised to the given scalar.
	//----------------------------------------------------------------------------------------------
	TVector3(TReal p_tValue) : X(p_tValue), Y(p_tValue), Z(p_tValue) { }

	//----------------------------------------------------------------------------------------------
	/// Zeros the vector components.
	//----------------------------------------------------------------------------------------------
	void MakeZero(void) 
	{
		X = Y = Z = (TReal)0.0;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns whether the vector components are zero.
	/// \returns True if the vector is zero, false otherwise
	//----------------------------------------------------------------------------------------------
	bool IsZero(void) const
	{
		return ( TMaths<TReal>::Equals(X, (TReal)0.0) && 
				 TMaths<TReal>::Equals(Y, (TReal)0.0) &&
				 TMaths<TReal>::Equals(Z, (TReal)0.0) );
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the square of the magnitude of the vector.
	/// \returns Square of the magnitude
	//----------------------------------------------------------------------------------------------
	TReal LengthSquared(void) const
	{
		return X*X + Y*Y + Z*Z;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the magnitude of the vector.
	/// \returns Magnitude of the vector
	//----------------------------------------------------------------------------------------------
	TReal Length(void) const
	{
		return TMaths<TReal>::Sqrt(X*X + Y*Y + Z*Z);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the dot product between the vector and a source operand.
	/// \param p_vector Second operand in dot product operation
	/// \returns Dot product
	//----------------------------------------------------------------------------------------------
	TReal DotProduct(const TVector3& p_vector) const
	{
		return X * p_vector.X + Y * p_vector.Y + Z * p_vector.Z;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the cross product.
	/// \param p_vector Second operand in cross product operation
	/// \returns Vector result of cross product
	//----------------------------------------------------------------------------------------------
	TVector3 CrossProduct(const TVector3& p_vector) const 
	{
		return TVector3(Y * p_vector.Z - Z * p_vector.Y, Z * p_vector.X - X * p_vector.Z, X * p_vector.Y - Y * p_vector.X);
	}
	
	//----------------------------------------------------------------------------------------------
	void ComponentProduct(const TVector3& p_vector)
	{
		X*=p_vector.X;
		Y*=p_vector.Y;
		Z*=p_vector.Z;
	}

	//----------------------------------------------------------------------------------------------
	TVector3 ComponentProductCopy(const TVector3& p_vector) const
	{
		return TVector3(X*p_vector.X, Y*p_vector.Y, Z*p_vector.Z);
	}

	//----------------------------------------------------------------------------------------------
	/// Negates the vector.
	//----------------------------------------------------------------------------------------------
	void Negate(void) 
	{
		X=-X; 
		Y=-Y; 
		Z=-Z;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the negation of the vector.
	/// \returns Negation
	//----------------------------------------------------------------------------------------------
	TVector3 NegateCopy(void) const 
	{
		return TVector3(-X, -Y, -Z);
	}

	//----------------------------------------------------------------------------------------------
	/// Normalises the vector.
	//----------------------------------------------------------------------------------------------
	void Normalise(void) 
	{
		TReal tLength = (TReal)1.0 / Length();
		X *= tLength; 
		Y *= tLength; 
		Z *= tLength;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the normalised vector.
	/// \returns Normal
	//----------------------------------------------------------------------------------------------
	TVector3 NormaliseCopy(void) const
	{
		TReal tLength = (TReal)1.0 / Length();
		return TVector3( X * tLength, Y * tLength, Z * tLength );
	}

	//----------------------------------------------------------------------------------------------
	/// Computes two arbitrary orthonormal vectors, both normal to this vector
	//----------------------------------------------------------------------------------------------
	void ComputeOrthonormals(TVector3<TReal>& p_normal1, TVector3<TReal>& p_normal2) const
	{
		TVector3<TReal> normal = NormaliseCopy();
		p_normal1.Set((TReal) 1.0, (TReal) 0.0, (TReal) 0.0);
		p_normal1 -= (p_normal1 * normal) * normal;
		if (p_normal1.IsZero())
		{
			p_normal1.Set((TReal) 0.0, (TReal) 1.0, (TReal) 0.0);
			p_normal1 -= (p_normal1 * normal) * normal;
		}
		p_normal1.Normalise();
		p_normal2 = normal ^ p_normal1;
	}

	//----------------------------------------------------------------------------------------------
	/// Sets the vector to the specified values.
	//----------------------------------------------------------------------------------------------
	void Set(TReal p_tX, TReal p_tY, TReal p_tZ)
	{
		X = p_tX; Y = p_tY; Z = p_tZ;
	}

	template <typename TRealParam>
	void Set(const TVector3<TRealParam>& p_vector)
	{
		X = (TReal)p_vector.X; Y = (TReal)p_vector.Y; Z = (TReal)p_vector.Z;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the negation of the vector.
	/// \returns Negation
	//----------------------------------------------------------------------------------------------
	TVector3 operator- (void) const 
	{
		return NegateCopy();
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the difference between two vectors.
	//----------------------------------------------------------------------------------------------
	TVector3 operator- (const TVector3& p_vector) const
	{
		return TVector3(X - p_vector.X, Y - p_vector.Y, Z - p_vector.Z);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes addition of two vectors.
	//----------------------------------------------------------------------------------------------
	TVector3 operator+ (const TVector3& p_vector) const
	{
		return TVector3(X + p_vector.X, Y + p_vector.Y, Z + p_vector.Z);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the scalar multiplication of a vector.
	//----------------------------------------------------------------------------------------------
	TVector3 operator* (TReal p_tValue) const
	{
		return TVector3(X * p_tValue, Y * p_tValue, Z * p_tValue);
	}

	friend TVector3 operator* (const TReal p_tValue, const TVector3 p_vector )
	{
		return TVector3(p_vector.X * p_tValue, p_vector.Y * p_tValue, p_vector.Z * p_tValue);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the scalar division of a vector.
	//----------------------------------------------------------------------------------------------
	TVector3 operator/ (TReal p_tValue) const
	{
		if (!TMaths<TReal>::Equals( p_tValue, (TReal)0.0 ))
		{
			TReal tInvValue = (TReal)1.0 / p_tValue;
			return TVector3(X * tInvValue, Y * tInvValue, Z * tInvValue);
		}

		TReal tValue = TMaths<TReal>::Maximum;
		return TVector3( tValue, tValue, tValue );
	}
	
	//----------------------------------------------------------------------------------------------
	/// Computes the dot product of two vectors.
	//----------------------------------------------------------------------------------------------
	TReal operator* (const TVector3& p_vector) const
	{
		return DotProduct(p_vector);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the 3D cross product.
	//----------------------------------------------------------------------------------------------
	TVector3 operator^ (const TVector3& p_vector) const
	{
		return CrossProduct(p_vector);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the difference between two vectors.
	//----------------------------------------------------------------------------------------------
	TVector3& operator-= (const TVector3& p_vector) 
	{
		X-=p_vector.X; 
		Y-=p_vector.Y; 
		Z-=p_vector.Z;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the addition of two vectors.
	//----------------------------------------------------------------------------------------------
	TVector3& operator+= (const TVector3& p_vector) 
	{
		X+=p_vector.X; 
		Y+=p_vector.Y; 
		Z+=p_vector.Z;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the scaling by a divisor.
	//----------------------------------------------------------------------------------------------
	TVector3& operator/= (TReal p_tValue) 
	{
		if ( !TMaths<TReal>::Equals( p_tValue, (TReal)0.0 ) )
		{
			TReal tInvValue = (TReal)1.0 / p_tValue;
			X*=tInvValue; 
			Y*=tInvValue; 
			Z*=tInvValue;
		} 
		else 
		{
			X = Y = Z = TMaths<TReal>::Maximum;
		}

		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the scaling by a multiplicand.
	//----------------------------------------------------------------------------------------------
	TVector3& operator*= (TReal p_tValue) 
	{
		X*=p_tValue; 
		Y*=p_tValue; 
		Z*=p_tValue;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes and stores the cross product.
	//----------------------------------------------------------------------------------------------
	TVector3& operator^= (const TVector3& p_vector)
	{
		TVector3 result = Cross(p_vector);
		X = result.X; 
		Y = result.Y; 
		Z = result.Z;
		return *this;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Performs assignment.
	//----------------------------------------------------------------------------------------------
	TVector3& operator= (TReal p_tScalar) 
	{
		X = Y = Z = p_tScalar;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Performs assignment.
	//----------------------------------------------------------------------------------------------
	TVector3& operator= (const TVector2<TReal>& p_vector) 
	{
		X = p_vector.X; 
		Y = p_vector.Y; 
		Z = (TReal)0.0;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Performs assignment.
	//----------------------------------------------------------------------------------------------
	TVector3& operator= (const TVector3& p_vector) 
	{
		X = p_vector.X; 
		Y = p_vector.Y; 
		Z = p_vector.Z;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Evaluates the equality operator.
	//----------------------------------------------------------------------------------------------
	bool operator== (const TVector3& p_vector) const
	{
		return ( TMaths<TReal>::Equals( p_vector.X, X ) &&
				 TMaths<TReal>::Equals( p_vector.Y, Y ) &&
				 TMaths<TReal>::Equals( p_vector.Z, Z ) );
	}

	//----------------------------------------------------------------------------------------------
	/// Evaluates the inequality operator.
	//----------------------------------------------------------------------------------------------
	bool operator!= (const TVector3& p_vector) const
	{
		return ( !TMaths<TReal>::Equals( p_vector.X, X ) ||
				 !TMaths<TReal>::Equals( p_vector.Y, Y ) ||
				 !TMaths<TReal>::Equals( p_vector.Z, Z ) );
	}

	//----------------------------------------------------------------------------------------------
	/// Cast operations for vector.
	//----------------------------------------------------------------------------------------------
	operator TReal*() const
	{
		return (TReal*)&Element;
	}

	template <typename TRealParam>
	operator TVector3<TRealParam>() const
	{
		return TVector3<TRealParam>((TRealParam)X, (TRealParam)Y, (TRealParam)Z);
	}

public:
	//----------------------------------------------------------------------------------------------
	static const TVector3 Zero;
	static const TVector3 Up;
	static const TVector3 Down; 
	static const TVector3 Left;
	static const TVector3 Right; 
	static const TVector3 In; 
	static const TVector3 Out;

	//----------------------------------------------------------------------------------------------
	union 
	{
		TReal Element[3];
		struct
		{
			TReal X, Y, Z;
		};
	};
	//----------------------------------------------------------------------------------------------
};

//----------------------------------------------------------------------------------------------
// Static member initialisation
//----------------------------------------------------------------------------------------------
template <typename TReal> TVector3<TReal> const TVector3<TReal>::Zero = TVector3<TReal>((TReal)0.0, (TReal)0.0, (TReal)0.0);
template <typename TReal> TVector3<TReal> const TVector3<TReal>::Up = TVector3<TReal>((TReal)0.0, (TReal)1.0, (TReal)0.0);
template <typename TReal> TVector3<TReal> const TVector3<TReal>::Down = TVector3<TReal>((TReal)0.0, (TReal)-1.0, (TReal)0.0);
template <typename TReal> TVector3<TReal> const TVector3<TReal>::Left = TVector3<TReal>((TReal)-1.0, (TReal)0.0, (TReal)0.0);
template <typename TReal> TVector3<TReal> const TVector3<TReal>::Right = TVector3<TReal>((TReal)1.0, (TReal)0.0, (TReal)0.0);
template <typename TReal> TVector3<TReal> const TVector3<TReal>::In = TVector3<TReal>((TReal)0.0, (TReal)0.0, (TReal)-1.0);
template <typename TReal> TVector3<TReal> const TVector3<TReal>::Out = TVector3<TReal>((TReal)0.0, (TReal)0.0, (TReal)1.0);

//----------------------------------------------------------------------------------------------
// Type definitions
//----------------------------------------------------------------------------------------------
typedef TVector3<double> Vector3;
typedef TVector3<float>	 Vector3f;

Meson_Common_Maths_END
