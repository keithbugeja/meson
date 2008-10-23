//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::Plane<T>
//	Templated class for plane.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	28/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "DebugSupport.h"
#include "Vector3.h"
#include "Point3.h"
#include "Maths.h"

Meson_Common_Maths_BEGIN

template <typename TReal> class TPlane
{
public:
	enum Side
	{
		SIDE_NEITHER,
		SIDE_NEGATIVE,
		SIDE_POSITIVE,
		SIDE_BOTH,

		SIDE_COUNT
	};

public:
	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TPlane(void) { }	

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TPlane(const TPlane& p_plane) :
		Distance( p_plane.Distance),
		Normal( p_plane.Normal )
	{ }

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TPlane(const TVector3<TReal>& p_vec3Normal, TReal p_tDistance) : 
		Normal(p_vec3Normal), 
		Distance(p_tDistance) 
	{ }

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TPlane(const TVector3<TReal>& p_vec3Normal, const TVector3<TReal>& p_vec3Point) 
	{
		// This should formally be transcribed as:
		// TVector3<TReal> P = p_planePoint - p_planePoint.Origin;
		// Distance = P.DotProduct( p_normalVector );
		// Normal = p_normalVector;

		Distance = p_vec3Point.DotProduct(p_vec3Normal);
		Normal = p_vec3Normal;
	}

	TPlane( const TVector3<TReal>& p_vec3Normal, const TPoint3<TReal>& p_pt3Point ) 
	{
		Distance = ((TVector3<TReal>&)p_pt3Point).DotProduct(p_vec3Normal);
		Normal = p_vec3Normal;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TPlane(const TVector3<TReal>& p_point1, const TVector3<TReal>& p_point2, const TVector3<TReal>& p_point3) 
	{
		// N = norm(AB x AC)
		// D = (A - O) . N

		TVector3<TReal> AB = p_point2 - p_point1,
						AC = p_point3 - p_point1;
		
		Normal = AB.CrossProduct(AC).Normalise();
		Distance = p_point1.DotProduct(Normal);
	}

	TPlane( const TPoint3<TReal> &p_point1, const TPoint3<TReal> &p_point2, const TPoint3<TReal> &p_point3 ) 
	{
		TVector3<TReal> AB = p_point2 - p_point1,
						AC = p_point3 - p_point1;
		
		Normal = AB.CrossProduct(AC).Normalise();
		Distance = ((TVector3<TReal>)p_point1).DotProduct(Normal);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TVector3<TReal> GetReflectionVector(const TVector3<TReal>& p_vector) const
	{
		return p_vector - (Normal * 2 * (Normal.DotProduct(p_vector) - Distance));
	}


	TVector3<TReal> GetDisplacementVector(const TVector3<TReal>& p_vector) const
	{
		return Normal * (Normal.DotProduct(p_vector) - Distance);
	}

	float GetDisplacement(const TVector3<TReal>& p_vector) const
	{
		// s = (V - dN) . N
		//   = V . N - dN . N
		//   = V . N - dN^2

		// |N| = 1
		// N^2 = 1^2 = 1

		// s = V . N - d(1)
		//   = V . N - d

		return Normal.DotProduct(p_vector) - Distance;
	}

	float GetDistance(const TVector3<TReal>& p_vector) const
	{
		return Mathf::Abs(Normal.DotProduct(p_vector) - Distance);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	Side GetSide(const TVector3<TReal>& p_vector) const
	{
		// (P - dN) . N = 0
		TVector3<TReal> P = p_vector - (Distance * Normal);
		int nDotSign = TMaths<TReal>::ISgn( P.DotProduct(Normal) );
		
		if ( nDotSign == -1 )
			return SIDE_NEGATIVE;
		else if (nDotSign == 1)
			return SIDE_POSITIVE;
		
		// Co-planar
		return SIDE_NEITHER;
	}

	Side GetSide(const TVector3<TReal>& p_midPoint, const TVector3<TReal>& p_halfVector) const
	{
		TVector3<TReal> M = p_midPoint - (Distance * Normal),
						A = M + p_halfVector,
						B = M - p_halfVector;

		int nASign = TMaths<TReal>::ISgn( A.DotProduct(Normal) ),
			nBSign = TMaths<TReal>::ISgn( B.DotProduct(Normal) );

		// Both points lie in the same half-space
		if ( nASign - nBSign == 0 )
		{
			if ( nASign == -1 )
				return SIDE_NEGATIVE;
			else if (nASign == 1)
				return SIDE_POSITIVE;

			return SIDE_NEITHER;
		}
		
		return SIDE_BOTH;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	void Normalise(void)
	{
		Normal.Normalise();
	}

	//----------------------------------------------------------------------------------------------
	/// Evaluates the equality operator.
	//----------------------------------------------------------------------------------------------
	bool operator== (const TPlane& p_plane) const
	{
		return ( TMaths<TReal>::Equals(p_plane.Distance, Distance) && p_plane.Normal == Normal );
	}

	//----------------------------------------------------------------------------------------------
	/// Evaluates the inequality operator.
	//----------------------------------------------------------------------------------------------
	bool operator!= (const TPlane& p_plane) const
	{
		return ( !(TMaths<TReal>::Equals(p_plane.Distance, Distance) && p_plane.Normal == Normal) );
	}

public:
	TReal Distance;
	TVector3<TReal> Normal;
};

//----------------------------------------------------------------------------------------------
// Static member initialisation
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Type definitions
//----------------------------------------------------------------------------------------------
typedef TPlane<double> Plane;
typedef TPlane<float> Planef;

Meson_Common_Maths_END
