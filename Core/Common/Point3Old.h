/*

//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::TVector3<T>
//	Templated class for 3D points.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		28/08/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "DebugSupport.h"
#include "Maths.h"

#include "Vector3.h"
#include "Vector2.h"

Meson_Common_Maths_BEGIN

template <typename TReal> class TPoint3
{
public:
	//----------------------------------------------------------------------------------------------
	/// Constructs an unitialised point.
	//----------------------------------------------------------------------------------------------
	TPoint3( void ) {}

	//----------------------------------------------------------------------------------------------
	/// Constructs a point from another point.
	/// \param p_point A point object used as a source for copy
	//----------------------------------------------------------------------------------------------
	TPoint3( const TPoint3& p_point ) : X(p_point.X), Y(p_point.Y), Z(p_point.Z) { }

	//----------------------------------------------------------------------------------------------
	/// Construct an initialised poiny.
	/// \param p_tX	X component
	/// \param p_tX	Y component
	/// \param p_tX	Z component
	//----------------------------------------------------------------------------------------------
	TPoint3( const TReal p_tX, const TReal p_tY, const TReal p_tZ ) : X(p_tX), Y(p_tY), Z(p_tZ) { }

	//----------------------------------------------------------------------------------------------
	/// Zeros the vector components.
	//----------------------------------------------------------------------------------------------
	void MakeOrigin( void ) 
	{
		X = Y = Z = (TReal)0.0;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns whether the point components are zero.
	/// \returns True if the point corresponds to the origin, false otherwise
	//----------------------------------------------------------------------------------------------
	bool IsOrigin( void ) const
	{
		return ( TMaths<TReal>::Equals(X, (TReal)0.0) && 
				 TMaths<TReal>::Equals(Y, (TReal)0.0) &&
				 TMaths<TReal>::Equals(Z, (TReal)0.0) );
	}

	//----------------------------------------------------------------------------------------------
	/// Negates the point.
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
	TPoint3 NegateCopy(void) const 
	{
		return TPoint3(-X, -Y, -Z);
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the negation of the point around the origin.
	/// \returns Negation
	//----------------------------------------------------------------------------------------------
	TPoint3 operator- (void) const 
	{
		return NegateCopy();
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the vector difference between two points.
	//----------------------------------------------------------------------------------------------
	TVector3<TReal> operator- (const TPoint3& p_point) const
	{
		return TVector3<TReal>(X - p_point.X, Y - p_point.Y, Z - p_point.Z);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes a new point via the addition of a vector offset with this point
	//----------------------------------------------------------------------------------------------
	TPoint3 operator+ (const TVector3<TReal>& p_vector) const
	{
		return TPoint3(X + p_vector.X, Y + p_vector.Y, Z + p_vector.Z);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes a new point via the subtraction of a vector offset with this point
	//----------------------------------------------------------------------------------------------
	TPoint3 operator- (const TVector3<TReal>& p_vector) const
	{
		return TPoint3(X - p_vector.X, Y - p_vector.Y, Z - p_vector.Z);
	}

	//----------------------------------------------------------------------------------------------
	/// Changes the value of the point by subtracting the vector offset
	//----------------------------------------------------------------------------------------------
	TPoint3& operator-= (const TVector3<TReal>& p_vector) 
	{
		X-=p_vector.X; 
		Y-=p_vector.Y; 
		Z-=p_vector.Z;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Changes the value of the point by adding the vector offset
	//----------------------------------------------------------------------------------------------
	TPoint3& operator+= (const TVector3<TReal>& p_vector) 
	{
		X+=p_vector.X; 
		Y+=p_vector.Y; 
		Z+=p_vector.Z;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Performs assignment.
	//----------------------------------------------------------------------------------------------
	TPoint3& operator= (const TReal p_tScalar) 
	{
		X = Y = Z = p_tScalar;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Performs assignment.
	//----------------------------------------------------------------------------------------------
	TPoint3& operator= (const TPoint3& p_point) 
	{
		X = p_point.X; 
		Y = p_point.Y; 
		Z = p_point.Z;
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Evaluates the equality operator.
	//----------------------------------------------------------------------------------------------
	bool operator== (const TPoint3& p_point) const
	{
		return ( TMaths<TReal>::Equals( p_point.X, X ) &&
				 TMaths<TReal>::Equals( p_point.Y, Y ) &&
				 TMaths<TReal>::Equals( p_point.Z, Z ) );
	}

	//----------------------------------------------------------------------------------------------
	/// Evaluates the inequality operator.
	//----------------------------------------------------------------------------------------------
	bool operator!= (const TPoint3& p_point) const
	{
		return ( !TMaths<TReal>::Equals( p_point.X, X ) ||
				 !TMaths<TReal>::Equals( p_point.Y, Y ) ||
				 !TMaths<TReal>::Equals( p_point.Z, Z ) );
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the triple array list under the cast operator.
	//----------------------------------------------------------------------------------------------
	operator TReal*() 
	{
		return (TReal*)&Element;
	}

	operator TVector3<TReal>&(void)
	{
		return (TVector3<TReal>&) *Element;
	}
	
	operator TVector3<TReal>(void)
	{
		return (TVector3<TReal>)*this;
	}

	TVector3<TReal>& ToVector(void) const
	{
		return (TVector3<TReal>&)*this;
	}

public:
	//----------------------------------------------------------------------------------------------
	static const TPoint3 Origin;

	//----------------------------------------------------------------------------------------------
	union 
	{
		TReal Element[3];
		struct
		{
			TReal X, Y, Z;
		};
	};
};

//----------------------------------------------------------------------------------------------
// Static member initialisation
//----------------------------------------------------------------------------------------------
template <typename TReal> TPoint3<TReal> const TPoint3<TReal>::Origin = TPoint3<TReal>((TReal)0.0, (TReal)0.0, (TReal)0.0);

typedef TPoint3<double> Point3;
typedef TPoint3<float>	Point3f;

Meson_Common_Maths_END

*/
