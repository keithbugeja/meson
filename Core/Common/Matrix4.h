//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::TMatrix4<T>
//	Templated class for 4x4 matrices.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "DebugSupport.h"
#include "MesonException.h"

#include "Maths.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix2.h"
#include "Matrix3.h"

#include <memory.h>
#include <algorithm>

// Set namespace to Meson::Common::Maths
Meson_Common_Maths_BEGIN

//----------------------------------------------------------------------------------------------
template <typename TReal> class TQuaternion;

//----------------------------------------------------------------------------------------------
template <typename TReal> 
class TMatrix4
{
public:
	//----------------------------------------------------------------------------------------------
	static const int Rows	 = 4;
	static const int Columns = 4;
	static const int Order	 = Rows * Columns;

	static const TMatrix4 Identity;
	static const TMatrix4 Zero;

	//----------------------------------------------------------------------------------------------
	union
	{
		TReal Element[Order];
		struct 
		{
			TReal _11, _12, _13, _14,
				  _21, _22, _23, _24,
				  _31, _32, _33, _34,
				  _41, _42, _43, _44;
		};
	};

public:
	//----------------------------------------------------------------------------------------------
	/// Constructs an uninitialised 4x4 matrix. 
	//----------------------------------------------------------------------------------------------
	TMatrix4(void) {}
	
	//----------------------------------------------------------------------------------------------
	/// TMatrix4 copy constructor.
	/// \param p_matrix A matrix object used as a source for copy
	//----------------------------------------------------------------------------------------------
	TMatrix4(const TMatrix4& p_matrix) 
	{
		memcpy( Element, p_matrix.Element, sizeof(TReal) * Order );
	}

	//----------------------------------------------------------------------------------------------
	/// Constructs a new 4x4 matrix.
	/// \param p_bIdentity Determines whether the new matrix is created as an identity matrix
	//----------------------------------------------------------------------------------------------
	explicit TMatrix4(bool p_bIdentity)
	{
		p_bIdentity ? MakeIdentity() : MakeZero();
	}

	//----------------------------------------------------------------------------------------------
	/// Constructs a new 4x4 matrix from the provided real array.
	/// \param p_tElements[Order] Array of <b>TReal</b> elements used to populate matrix
	/// \param p_bRowMajor Specifies whether matrix is populated in row or column major order
	//----------------------------------------------------------------------------------------------
	explicit TMatrix4(const TReal p_tElements[Order], bool p_bRowMajor = true)
	{
		if (p_bRowMajor)
		{
			memcpy( Element, p_tElements, sizeof(TReal) * Order );
		}
		else
		{
			_11 = p_tElements[0];
			_21 = p_tElements[1];
			_31 = p_tElements[2];
			_41 = p_tElements[3];
			_12 = p_tElements[4];
			_22 = p_tElements[5];
			_32 = p_tElements[6];
			_42 = p_tElements[7];
			_13 = p_tElements[8];
			_23 = p_tElements[9];
			_33 = p_tElements[10];
			_43 = p_tElements[11];
			_14 = p_tElements[12];
			_24 = p_tElements[13];
			_34 = p_tElements[14];
			_44 = p_tElements[15];
		}
	}

	//----------------------------------------------------------------------------------------------
	/// Constructs a new 4x4 matrix from a 3x3 matrix
	/// \param p_matrix A matrix of type TMatrix3
 	//----------------------------------------------------------------------------------------------
	explicit TMatrix4(const TMatrix3<TReal>& p_matrix) 
	{
		*this = p_matrix;
	}

	//----------------------------------------------------------------------------------------------
	/// Constructs a new 4x4 matrix from a 2x2 matrix
	/// \param p_matrix A matrix of type TMatrix2
 	//----------------------------------------------------------------------------------------------
	explicit TMatrix4(const TMatrix2<TReal>& p_matrix)
	{
		*this = p_matrix;
	}

	//----------------------------------------------------------------------------------------------
	/// TMatrix4 constructor for a combined rotation matrix.
	/// \param p_vecAxesRotation A vector containing the rotation for each axis
	/// \param p_eAxisOrder Determines the axial order of the rotation
	//----------------------------------------------------------------------------------------------
	explicit TMatrix4(const TVector3<TReal>& p_vecAxesRotation, AxisOrder::AxisOrder p_eAxisOrder) 
	{
		MakeRotation( p_vecAxesRotation, p_eAxisOrder);
	}

	//----------------------------------------------------------------------------------------------
	/// TMatrix4 constructor for a rotation matrix about a carthesian axis.
	/// \param p_tAngle Angle of rotation
	/// \param p_eAxis Axis of rotation
	//----------------------------------------------------------------------------------------------
	explicit TMatrix4(TReal p_tAngle, Axis::Axis p_eAxis) 
	{
		MakeRotation( p_tAngle, p_eAxis );
	}

	//----------------------------------------------------------------------------------------------
	/// TMatrix4 constructor for a rotation matrix about an arbitrary axis.
	/// \param p_vecAxis Normalised vector representing rotation axis
	/// \param p_tAngle Rotation extent in radians
	//----------------------------------------------------------------------------------------------
	explicit TMatrix4(const TVector3<TReal>& p_vecAxis, TReal p_tAngle) 
	{
		MakeRotation( p_vecAxis, p_tAngle );
	}

	//----------------------------------------------------------------------------------------------
	///	Returns whether the matrix is singular <i>(i.e. does not possess an inverse)</i> or not.
	///	\returns False if an inverse exists, true otherwise
	//----------------------------------------------------------------------------------------------
	inline bool IsSingular(void) const 
	{
		return TMaths<TReal>::Equals(Determinant(), (TReal)0.0);
	}

	//----------------------------------------------------------------------------------------------
	///	Returns whether the matrix is an identity or not.
	///	\returns True if matrix is an identity, false otherwise
	//----------------------------------------------------------------------------------------------
	inline bool IsIdentity(void) const
	{
		return ( TMaths<TReal>::Equals( _11, (TReal)1.0 ) &&
				 TMaths<TReal>::Equals( _22, (TReal)1.0 ) &&
				 TMaths<TReal>::Equals( _33, (TReal)1.0 ) &&
				 TMaths<TReal>::Equals( _44, (TReal)1.0 ) &&
				 TMaths<TReal>::Equals( _12, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _13, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _14, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _21, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _23, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _24, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _31, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _32, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _34, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _41, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _42, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _43, (TReal)0.0 ) );
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline bool IsZero(void) const
	{
		return ( TMaths<TReal>::Equals( _11, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _22, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _33, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _44, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _12, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _13, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _14, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _21, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _23, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _24, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _31, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _32, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _34, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _41, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _42, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _43, (TReal)0.0 ) );
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into a Zero matrix.
	//----------------------------------------------------------------------------------------------
	void MakeZero(void) 
	{
		_11 = _12 = _13 = _14 = _21 = _22 = _23 = _24 = _31 = _32 = _33 = _34 = _41 = _42 = _43 = _44 = (TReal)0.0;
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into an Identity matrix.
	//----------------------------------------------------------------------------------------------
	void MakeIdentity(void)
	{
		_12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = (TReal)0.0;
		_11 = _22 = _33 = _44 = (TReal)1.0;
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into a diagonal matrix.
	///	\param p_vector A vector containing the diagonal components
	//----------------------------------------------------------------------------------------------
	void MakeDiagonal(const TVector4<TReal>& p_vector)
	{
		_12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = (TReal)0.0;
		_11 = p_vector.W; _22 = p_vector.X; _33 = p_vector.Y; _44 = p_vector.Z;
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into a translation matrix.
	///	\param p_vector A vector containing the translation components
	//----------------------------------------------------------------------------------------------
	void MakeTranslation(const TVector3<TReal>& p_vector) 
	{
		_12 = _13 = _21 = _23 = _31 = _32 = _41 = _42 = _43 = (TReal)0.0;
		_11 = _22 = _33 = _44 = (TReal)1.0;

		_14 = p_vector.X; _24 = p_vector.Y; _34 = p_vector.Z;
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into a scaling matrix.
	///	\param p_vector A vector containing the scaling components
	//----------------------------------------------------------------------------------------------
	void MakeScaling(const TVector3<TReal>& p_vector)
	{
		_12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = (TReal)0.0;
		_44 = (TReal)1.0;

		_11 = p_vector.X; _22 = p_vector.Y; _33 = p_vector.Z;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into a rotation matrix.
	/// \param p_tAngle The angle of rotation in radians
	/// \param p_eAxis The axis about which to rotate
	//----------------------------------------------------------------------------------------------
	void MakeRotation(TReal p_tAngle, Axis::Axis p_eAxis)
	{
		TReal tCos = TMaths<TReal>::Cos( p_tAngle ),
			  tSin = TMaths<TReal>::Sin( p_tAngle );

		switch ( p_eAxis )
		{
			case Axis::X:
				_12 = _13 = _14 = _21 = _24 = _31 = _34 = _41 = _42 = _43 = (TReal)0.0;
				_11 = _44 = (TReal)1.0;
				
				_22 = _33 = tCos;
				_23 = tSin;
				_32 = -tSin;
				return;

			case Axis::Y:
				_12 = _14 = _21 = _23 = _24 = _32 = _34 = _41 = _42 = _43 = (TReal)0.0;
				_22 = _44 = (TReal)1.0;

				_11 = _33 = tCos;
				_13 = -tSin;
				_31 = tSin;
				return;

			case Axis::Z:
				_13 = _14 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = (TReal)0.0;
				_33 = _44 = (TReal)1.0;
				_11 = _22 = tCos;
				_12 = tSin;
				_21 = -tSin;
				return;
		}
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into a rotation matrix.
	/// \param p_vecAxesRotation A vector specifying the angle of rotation <i>(in radians)</i> about each axis
	/// \param p_eAxisOrder The order of rotation about each axis
	//----------------------------------------------------------------------------------------------
	void MakeRotation(const TVector3<TReal>& p_vecAxesRotation, AxisOrder::AxisOrder p_eAxisOrder)
	{
		TReal tCosX = TMaths<TReal>::Cos( p_vecAxesRotation.X),
			  tSinX = TMaths<TReal>::Sin( p_vecAxesRotation.X),
			  tCosY = TMaths<TReal>::Cos( p_vecAxesRotation.Y),
			  tSinY = TMaths<TReal>::Sin( p_vecAxesRotation.Y),
			  tCosZ = TMaths<TReal>::Cos( p_vecAxesRotation.Z),
			  tSinZ = TMaths<TReal>::Sin( p_vecAxesRotation.Z);

		// Common for all transformations
		_14 = _24 = _34 = _41 = _42 = _43 = (TReal)0.0;
		_44 = (TReal)1.0;

		switch (p_eAxisOrder)
		{
			case AxisOrder::XYZ:
			{
				_11 =  tCosY *  tCosZ;
				_12 =  tCosY *  tSinZ;
				_13 = -tSinY;
				_21 =  tSinX *  tSinY *  tCosZ + tCosX * -tSinZ;
				_22 =  tSinX *  tSinY *  tSinZ + tCosX *  tCosZ;
				_23 =  tSinX *  tCosY;
				_31 =  tCosX *  tSinY *  tCosZ + -tSinZ * -tSinX;
				_32 =  tCosX *  tSinY *  tSinZ +  tCosZ * -tSinX;
				_33 =  tCosX *  tCosY;
				
				return;
			}

			case AxisOrder::XZY:
			{
				_11 =  tCosY *  tCosZ;
				_12 =  tSinZ;
				_13 = -tSinY *  tCosZ;
				_21 =  tCosX * -tSinZ *  tCosY + tSinX *  tSinY;
				_22 =  tCosX *  tCosZ;
				_23 =  tCosX * -tSinZ * -tSinY + tSinX *  tCosY;
				_31 = -tSinX * -tSinZ *  tCosY + tCosX *  tSinY;
				_32 = -tSinX *  tCosZ;
				_33 = -tSinX * -tSinY * -tSinZ + tCosX *  tCosY;
				return;
			}

			case AxisOrder::YXZ:
			{
				_11 =  tCosY *  tCosZ + -tSinX * -tSinY * -tSinZ;
				_12 =  tCosY *  tSinZ +  tCosZ * -tSinY * -tSinX;
				_13 = -tSinY *  tCosX;
				_21 =  tCosX * -tSinZ;
				_22 =  tCosX *  tCosZ;
				_23 =  tSinX;
				_31 =  tSinY *  tCosZ +  tCosY * -tSinX * -tSinZ;
				_32 =  tSinY *  tSinZ + -tSinX *  tCosY *  tCosZ;
				_33 =  tCosX *  tCosY;
				return;
			}

			case AxisOrder::YZX:
			{
				_11 =  tCosY *  tCosZ;
				_12 =  tCosY *  tSinZ *  tCosX +  -tSinX * -tSinY;
				_13 =  tCosY *  tSinZ *  tSinX +  -tSinY *  tCosX;
				_21 = -tSinZ;
				_22 =  tCosX *  tCosZ;
				_23 =  tSinX *  tCosZ;
				_31 =  tSinY *  tCosZ;
				_32 =  tSinY *  tSinZ *  tCosX +   tCosY * -tSinX;
				_33 =  tSinX *  tSinY *  tSinZ +   tCosX *  tCosY;
				return;
			}

			case AxisOrder::ZXY:
			{
				_11 =  tCosZ *  tCosY +  tSinX *  tSinY *  tSinZ;
				_12 =  tSinZ *  tCosX;
				_13 =  tCosZ * -tSinY +  tCosY *  tSinX *  tSinZ;
				_21 = -tSinZ *  tCosY +  tCosZ *  tSinX *  tSinY;
				_22 =  tCosX *  tCosZ;
				_23 = -tSinY * -tSinZ +  tCosZ *  tSinX *  tCosY;
				_31 =  tCosX *  tSinY;
				_32 = -tSinX;
				_33 =  tCosX *  tCosY;
				return;
			}
				
			case AxisOrder::ZYX:
			{
				_11 =  tCosZ *  tCosY;
				_12 =  tSinZ *  tCosX + -tSinY * -tSinX *  tCosZ;
				_13 =  tSinZ *  tSinX + -tSinY *  tCosX *  tCosZ;
				_21 = -tSinZ *  tCosY;
				_22 =  tCosX *  tCosZ + -tSinX * -tSinY * -tSinZ;
				_23 =  tCosZ *  tSinX +  tCosX * -tSinY * -tSinZ;
				_31 =  tSinY;
				_32 = -tSinX *  tCosY;
				_33 =  tCosX *  tCosY;
				return;
			}
		}
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into a rotation matrix about an arbitrary axis.
	/// \param p_vecAxis Normalised vector representing rotation axis
	/// \param p_tAngle Rotation extent in radians
	//----------------------------------------------------------------------------------------------
	void MakeRotation(const TVector3<TReal>& p_vecAxis, TReal p_tAngle)
	{
		TReal tCos = TMaths<TReal>::Cos( p_tAngle ),
			  tSin = TMaths<TReal>::Sin( p_tAngle ),
			  tOneMinusCos = (TReal)1.0 - tCos;
		
		TReal tXY = p_vecAxis.X * p_vecAxis.Y * tOneMinusCos,
			  tXZ = p_vecAxis.X * p_vecAxis.Z * tOneMinusCos,
			  tYZ = p_vecAxis.Y * p_vecAxis.Z * tOneMinusCos,
			  tSinX = p_vecAxis.X * tSin,
			  tSinY = p_vecAxis.Y * tSin,
			  tSinZ = p_vecAxis.Z * tSin;

		_14 = _24 = _34 = _41 = _42 = _43 = (TReal)0; _44 = (TReal)1;

		_11 = p_vecAxis.X * p_vecAxis.X * tOneMinusCos + tCos;
		_12 = tXY - tSinZ;
		_13 = tXZ + tSinY;

		_21 = tXY + tSinZ;
		_22 = p_vecAxis.Y * p_vecAxis.Y * tOneMinusCos + tCos;
		_23 = tYZ - tSinX;

		_31 = tXZ - tSinY;
		_32 = tYZ + tSinX;
		_33 = p_vecAxis.Z * p_vecAxis.Z * tOneMinusCos + tCos;
	}
	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	void MakeTransformation(const TVector3<TReal>& p_vec3Scale, const TVector3<TReal>& p_vec3Translation, const TQuaternion<TReal>& p_qtnRotation)
	{
		p_qtnRotation.ExtractMatrix(*this);
		
		_11 *= p_vec3Scale.X;
		_12 *= p_vec3Scale.X;
		_13 *= p_vec3Scale.X;
		
		_21 *= p_vec3Scale.Y;
		_22 *= p_vec3Scale.Y;
		_23 *= p_vec3Scale.Y;
		
		_31 *= p_vec3Scale.Z;
		_32 *= p_vec3Scale.Z;
		_33 *= p_vec3Scale.Z;

		_14 = p_vec3Translation.X;
		_24 = p_vec3Translation.Y;
		_34 = p_vec3Translation.Z;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	void MakeInverseTransform(const TVector3<TReal>& p_vec3Scale, const TVector3<TReal>& p_vec3Translation, const TQuaternion<TReal>& p_qtnRotation)
	{
		TQuaternion<TReal> qtnInvRotation = p_qtnRotation.InvertCopy();
		qtnInvRotation.ExtractMatrix(*this);

		float fYZ = p_vec3Scale.Y * p_vec3Scale.Z,
			  fXZ = p_vec3Scale.X * p_vec3Scale.Z,
			  fXY = p_vec3Scale.X * p_vec3Scale.Y,
			  fInvXYZ = 1.0 / (fXY * p_vec3Scale.Z);
		
		// S^1
		TVector3<TReal> vec3InvScale(fYZ * fInvXYZ, fXZ * fInvXYZ, fXY * fInvXYZ);

		// T^-1
		TVector3<TReal> vec3InvTranslation = vec3InvScale.UnitProductCopy(-p_vec3Translation);
		vec3InvTranslation = *this * vec3InvTranslation;

		// R^1*S^1
		_11 *= vec3InvScale.X;
		_12 *= vec3InvScale.X;
		_13 *= vec3InvScale.X;
		
		_21 *= vec3InvScale.Y;
		_22 *= vec3InvScale.Y;
		_23 *= vec3InvScale.Y;
		
		_31 *= vec3InvScale.Z;
		_32 *= vec3InvScale.Z;
		_33 *= vec3InvScale.Z;

		// Set T
		_14 = vec3InvTranslation.X;
		_24 = vec3InvTranslation.Y;
		_34 = vec3InvTranslation.Z;
	}

	//----------------------------------------------------------------------------------------------
	/// Extracts an axis and a rotation angle from matrix.
	/// \returns A vector containing the axis and angle of rotation
	//----------------------------------------------------------------------------------------------
	TVector4<TReal> ExtractRotation(void) const
	{
		TVector4<TReal> result;
		ExtractRotation( result );
		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Extracts an axis and a rotation angle from matrix.
	/// \param p_vector Vector for holding axis and rotation
	//----------------------------------------------------------------------------------------------
	void ExtractRotation(TVector4<TReal>& p_vec4Out ) const
	{
		// Graphics Gems I, Converting between Matrix and Axis-Amount representations. pg 466
		TReal tCosTheta = 0.5 * (_11 + _22 + _33 - 1.0);
		TReal tSinTheta = TMaths<TReal>::Sqrt((TReal)1.0 - tCosTheta * tCosTheta);

		if ( TMaths<TReal>::Abs(tSinTheta) > TMaths<TReal>::Epsilon )
		{
			tSinTheta = (TReal) 0.5 / tSinTheta;

			p_vec4Out.X = ( _32 - _23 ) * tSinTheta;
			p_vec4Out.Y = ( _13 - _31 ) * tSinTheta;
			p_vec4Out.Z = ( _21 - _12 ) * tSinTheta;
			p_vec4Out.W = TMaths<TReal>::ACos( tCosTheta );
		}
		else
		{
			p_vec4Out = TVector4<TReal>::Zero;
		}
	}

	//----------------------------------------------------------------------------------------------
	/// Extracts the translation amount from the matrix.
	/// \returns A vector containing the translation
	//----------------------------------------------------------------------------------------------
	TVector3<TReal> ExtractTranslation(void) const
	{
		TVector3<TReal> result;
		ExtractTranslation( result );
		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Extracts the translation amount from the matrix.
	/// \param p_vector Vector for holding translation values
	//----------------------------------------------------------------------------------------------
	void ExtractTranslation(TVector3<TReal>& p_vec3Out ) const
	{
		p_vec3Out.X = _14;
		p_vec3Out.Y = _24;
		p_vec3Out.Z = _34;
	}

	//----------------------------------------------------------------------------------------------
	/// Extracts the scaling amount from the matrix.
	/// \returns A vector containing the scaling components
	//----------------------------------------------------------------------------------------------
	TVector3<TReal> ExtractScaling(void) const
	{
		TVector3<TReal> result;
		ExtractScaling( result );
		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Extracts the scaling amount from the matrix.
	/// \param p_vector Vector for holding scaling components
	//----------------------------------------------------------------------------------------------
	void ExtractScaling(TVector3<TReal>& p_vec3Out) const
	{
		p_vec3Out.X = _11;
		p_vec3Out.Y = _22;
		p_vec3Out.Z = _33;
	}

	
	//----------------------------------------------------------------------------------------------
	/// Creates a scaling matrix.
	///	\param p_vector A vector containing the scaling components
	/// \returns 4x4 Scaling matrix
	//----------------------------------------------------------------------------------------------
	static TMatrix4 CreateScaling(const TVector3<TReal>& p_vector)
	{
		TMatrix4 result;

		result._12 = result._13 = result._14 = result._21 = result._23 = result._24 = result._31 = 
			result._32 = result._34 = result._41 = result._42 = result._43 = (TReal)0.0;
		result._44 = (TReal)1.0;

		result._11 = p_vector.X;
		result._22 = p_vector.Y;
		result._33 = p_vector.Z;

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into a translation matrix.
	///	\param p_vector A vector containing the translation components
	/// \returns 4x4 Translation matrix
	//----------------------------------------------------------------------------------------------
	static TMatrix4 CreateTranslation(const TVector3<TReal>& p_vector) 
	{
		TMatrix4 result;

		result._11 = result._22 = result._33 = result._44 = (TReal)1.0;

		result._12 = result._13 = result._21 = result._23 = result._31 = 
			result._32 = result._41 = result._42 = result._43 = (TReal)0.0;

		result._14 = p_vector.X;
		result._24 = p_vector.Y;
		result._34 = p_vector.Z;
	}

	//----------------------------------------------------------------------------------------------
	/// Creates a rotation matrix about a Cartesian axis.  
	/// \param p_tAngle Angle of rotation about axis
	/// \param p_eAxis Axis of rotation
	/// \returns 4x4 Rotation matrix
	//----------------------------------------------------------------------------------------------
	static TMatrix4 CreateRotation(TReal p_tAngle, Axis::Axis p_eAxis) 
	{
		return TMatrix4( p_tAngle, p_eAxis);
	}

	//----------------------------------------------------------------------------------------------
	/// Creates a combined rotation matrix about the Cartesian axes.
	/// \param p_vecAxesRotation Angle of rotation about axes
	/// \param p_eAxisOrder Order of axial rotation
	/// \returns 4x4 Rotation matrix
	//----------------------------------------------------------------------------------------------
	static TMatrix4 CreateRotation(const TVector3<TReal>& p_vecAxesRotation, AxisOrder::AxisOrder p_eAxisOrder) 
	{
		return TMatrix4( p_vecAxesRotation, p_eAxisOrder);
	}

	//----------------------------------------------------------------------------------------------
	/// Creates a rotation matrix about an arbitrary axis.
	/// \param p_vecAxis Normalised vector representing rotation axis
	/// \param p_tAngle Rotation extent in radians
	/// \returns 4x4 Rotation matrix
	//----------------------------------------------------------------------------------------------
	static TMatrix4 CreateRotation(const TVector3<TReal>& p_vecAxis, TReal p_tAngle) 
	{
		return TMatrix4( p_vecAxis, p_tAngle );
	}

	//----------------------------------------------------------------------------------------------
	/// Sets the matrix translation components.
	///	\param p_vector A vector containing the translation components
	//----------------------------------------------------------------------------------------------
	void SetTranslation(const TVector3<TReal>& p_vector) {
		_14 = p_vector.X; _24 = p_vector.Y; _34 = p_vector.Z;
	}

	//----------------------------------------------------------------------------------------------
	/// Sets the matrix scaling components.
	///	\param p_vector A vector containing the scaling components
	//----------------------------------------------------------------------------------------------
	void SetScaling(const TVector3<TReal>& p_vector) {
		_11 = p_vector.X; _22 = p_vector.Y; _33 = p_vector.Z;
	}

	//----------------------------------------------------------------------------------------------
	/// Sets the rotation components of the matrix to rotation about an arbitrary axis.
	/// \param p_vecAxis Normalised vector representing rotation axis
	/// \param p_tAngle Rotation extent in radians
	//----------------------------------------------------------------------------------------------
	void SetRotation(const TVector3<TReal>& p_vecAxis, TReal p_tAngle)
	{
		TReal tCos = TMaths<TReal>::Cos( p_tAngle ),
			  tSin = TMaths<TReal>::Sin( p_tAngle ),
			  tOneMinusCos = (TReal)1.0 - tCos;
		
		TReal tXY = p_vecAxis.X * p_vecAxis.Y * tOneMinusCos,
			  tXZ = p_vecAxis.X * p_vecAxis.Z * tOneMinusCos,
			  tYZ = p_vecAxis.Y * p_vecAxis.Z * tOneMinusCos,
			  tSinX = p_vecAxis.X * tSin,
			  tSinY = p_vecAxis.Y * tSin,
			  tSinZ = p_vecAxis.Z * tSin;

		_11 = p_vecAxis.X * p_vecAxis.X * tOneMinusCos + tCos;
		_12 = tXY - tSinZ;
		_13 = tXZ + tSinY;

		_21 = tXY + tSinZ;
		_22 = p_vecAxis.Y * p_vecAxis.Y * tOneMinusCos + tCos;
		_23 = tYZ - tSinX;

		_31 = tXZ - tSinY;
		_32 = tYZ + tSinX;
		_33 = p_vecAxis.Z * p_vecAxis.Z * tOneMinusCos + tCos;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Returns the matrix element at the specified row and column.
	/// \param p_nRow Element row index
	/// \param p_nColumn Element column index
	/// \returns Element at specified index
	//----------------------------------------------------------------------------------------------
	TReal operator()(int p_nRow, int p_nColumn) const 
	{
		return Element[ Index(p_nRow, p_nColumn) ];
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the reference of the matrix element at the specified row and column.
	/// \param p_nRow Zero-based row index
	/// \param p_nColumn Zero-based column index
	/// \returns Reference of element at specified index
	//----------------------------------------------------------------------------------------------
	TReal& operator()(int p_nRow, int p_nColumn) 
	{
		return Element[ Index(p_nRow, p_nColumn) ];
	}

	//----------------------------------------------------------------------------------------------
	/// Sets a column of the matrix to the specified values.
	/// \param p_nColumn Index of column, zero-based
	/// \param p_vector Vector containing the values used to set the column
	//----------------------------------------------------------------------------------------------
	void SetColumn(int p_nColumn, const TVector4<TReal>& p_vector)
	{
		MESON_ASSERT( p_nColumn >= 0 && p_nColumn < Columns, "Matrix column index out of range!" );

		switch( p_nColumn )
		{
			case 0:
				_11 = p_vector.X; _21 = p_vector.Y; _31 = p_vector.Z; _41 = p_vector.W;
				return;
			
			case 1:
				_12 = p_vector.X; _22 = p_vector.Y; _32 = p_vector.Z; _42 = p_vector.W;
				return;
			
			case 2:
				_13 = p_vector.X; _23 = p_vector.Y; _33 = p_vector.Z; _43 = p_vector.W;
				return;

			case 3:
				_14 = p_vector.X; _24 = p_vector.Y; _34 = p_vector.Z; _44 = p_vector.W;
				return;
		}
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the column vector at the specified index.
	/// \param p_nColumn Zero-based column index
	///	\returns Column vector
	//----------------------------------------------------------------------------------------------
	TVector4<TReal> GetColumn(int p_nColumn) const
	{
		MESON_ASSERT( p_nColumn >= 0 && p_nColumn < Columns, "Matrix column index out of range!" );

		switch( p_nColumn )
		{
			case 0:
				return TVector4<TReal>( _11, _21, _31, _41 );
			
			case 1:
				return TVector4<TReal>( _12, _22, _32, _42 );
			
			case 2:
				return TVector4<TReal>( _13, _23, _33, _43 );

			case 3:
				return TVector4<TReal>( _14, _24, _34, _44 );
		}

		return TVector4<TReal>::Zero;
	}

	//----------------------------------------------------------------------------------------------
	/// Sets a row of the matrix to the specified values.
	/// \param p_nRow Index of row, zero-based
	/// \param p_vector Vector containing the values used to set the row
	//----------------------------------------------------------------------------------------------
	void SetRow(int p_nRow, const TVector4<TReal>& p_vector)
	{
		MESON_ASSERT( p_nRow >= 0 && p_nRow < Rows, "Matrix row index out of range!" );

		switch( p_nRow )
		{
			case 0:
				_11 = p_vector.X; _12 = p_vector.Y; _13 = p_vector.Z; _14 = p_vector.W;
				return;

			case 1:
				_21 = p_vector.X; _22 = p_vector.Y; _23 = p_vector.Z; _24 = p_vector.W;
				return;

			case 2:
				_31 = p_vector.X; _32 = p_vector.Y; _33 = p_vector.Z; _34 = p_vector.W;
				return;

			case 3:
				_41 = p_vector.X; _42 = p_vector.Y; _43 = p_vector.Z; _44 = p_vector.W;
				return;
		}
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the row vector at the specified index.
	/// \param p_nRow Zero-based row index
	///	\returns Row vector
	//----------------------------------------------------------------------------------------------
	TVector4<TReal> GetRow(int p_nRow) const
	{
		MESON_ASSERT( p_nRow >= 0 && p_nRow < Rows, "Matrix row index out of range!" );

		switch( p_nRow )
		{
			case 0:
				return TVector4<TReal>( _11, _12, _13, _14 );

			case 1:
				return TVector4<TReal>( _21, _22, _23, _24 );

			case 2:
				return TVector4<TReal>( _31, _32, _33, _34 );

			case 3:
				return TVector4<TReal>( _41, _42, _43, _44 );
		}

		return TVector4<TReal>::Zero;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the determinant of the matrix.
	/// \returns Determinant
	//----------------------------------------------------------------------------------------------
	inline TReal Determinant(void) const
	{
		//     _11 * ( _22*(_33*_44-_34*_43) - _23*(_32*_44-_34*_42) + _24*(_32*_43-_33*_42)) -
		//	   _12 * ( _21*(_33*_44-_34*_43) - _23*(_31*_44-_34*_41) + _24*(_31*_43-_33*_41)) +
		//	   _13 * ( _21*(_32*_44-_34*_42) - _22*(_31*_44-_34*_41) + _24*(_31*_42-_32*_41)) -
		//	   _14 * ( _21*(_32*_43-_33*_42) - _22*(_31*_43-_33*_41) + _23*(_31*_42-_32*_41));

		// Cofactors
		TReal _33x44_34x43 = _33*_44 - _34*_43,
			  _32x44_34x42 = _32*_44 - _34*_42,
			  _32x43_33x42 = _32*_43 - _33*_42,
			  _31x44_34x41 = _31*_44 - _34*_41,
			  _31x43_33x41 = _31*_43 - _33*_41,
			  _31x42_32x41 = _31*_42 - _32*_41;

		return _11 * ( _22*(_33x44_34x43) - _23*(_32x44_34x42) + _24*(_32x43_33x42) ) -
			   _12 * ( _21*(_33x44_34x43) - _23*(_31x44_34x41) + _24*(_31x43_33x41) ) +
			   _13 * ( _21*(_32x44_34x42) - _22*(_31x44_34x41) + _24*(_31x42_32x41) ) -
			   _14 * ( _21*(_32x43_33x42) - _22*(_31x43_33x41) + _23*(_31x42_32x41) );
	}
	

	//----------------------------------------------------------------------------------------------
	/// Negates the matrix elements; the same as multiplying the matrix by -1.
	//----------------------------------------------------------------------------------------------
	void Negate(void)
	{
		for (int n = 0; n < Order; n++)
			Element[n] = -Element[n];
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the negation of the matrix elements; the same as multiplying the matrix by -1.
	/// \returns The negative matrix
	//----------------------------------------------------------------------------------------------
	TMatrix4 NegateCopy(void) const
	{
		TMatrix4 result;

		for (int n=0; n<Order; n++)
			result.Element[n] = -Element[n];

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Transposes the matrix.
	//----------------------------------------------------------------------------------------------
	void Transpose(void)
	{
		std::swap(_21, _12);
		std::swap(_31, _13);
		std::swap(_41, _14);
		std::swap(_32, _23);
		std::swap(_42, _24);
		std::swap(_43, _34);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the transposition of the matrix.
	/// \returns The transposed matrix
	//----------------------------------------------------------------------------------------------
	TMatrix4 TransposeCopy(void)
	{
		TMatrix4 result;

		result._11 = _11; result._12 = _21; result._13 = _31; result._14 = _41;
		result._21 = _12; result._22 = _22; result._23 = _32; result._24 = _42;
		result._31 = _13; result._32 = _23; result._33 = _33; result._34 = _43;
		result._41 = _14; result._42 = _24; result._43 = _34; result._44 = _44;

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the inverse of the matrix.
	//----------------------------------------------------------------------------------------------
	void Invert(void) 
	{
		// Macro returning determinant of 3x3 matrix
		#define DETERMINANT3X3(a, b, c, d, e, f, g, h, i) (a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g))

		TReal mat[16], tInvDeterminant = (TReal)1.0 / Determinant();

		// b(i,j) = pow(-1, i+j) * det(A(j,i))/det(A)
		mat[0]  =  DETERMINANT3X3(_22,_23,_24,_32,_33,_34,_42,_43,_44) * tInvDeterminant;
		mat[1]  = -DETERMINANT3X3(_12,_13,_14,_32,_33,_34,_42,_43,_44) * tInvDeterminant;
		mat[2]  =  DETERMINANT3X3(_12,_13,_14,_22,_23,_24,_42,_43,_44) * tInvDeterminant;
		mat[3]  = -DETERMINANT3X3(_12,_13,_14,_22,_23,_24,_32,_33,_34) * tInvDeterminant;

		mat[4]  = -DETERMINANT3X3(_21,_23,_24,_31,_33,_34,_41,_43,_44) * tInvDeterminant;
		mat[5]  =  DETERMINANT3X3(_11,_13,_14,_31,_33,_34,_41,_43,_44) * tInvDeterminant;
		mat[6]  = -DETERMINANT3X3(_11,_13,_14,_21,_23,_24,_41,_43,_44) * tInvDeterminant;
		mat[7]  =  DETERMINANT3X3(_11,_13,_14,_21,_23,_24,_31,_33,_34) * tInvDeterminant;

		mat[8]  =  DETERMINANT3X3(_21,_22,_24,_31,_32,_34,_41,_42,_44) * tInvDeterminant;
		mat[9]  = -DETERMINANT3X3(_11,_12,_14,_31,_32,_34,_41,_42,_44) * tInvDeterminant;
		mat[10] =  DETERMINANT3X3(_11,_12,_14,_21,_22,_24,_41,_42,_44) * tInvDeterminant;
		mat[11] = -DETERMINANT3X3(_11,_12,_14,_21,_22,_24,_31,_32,_34) * tInvDeterminant;

		mat[12] = -DETERMINANT3X3(_21,_22,_23,_31,_32,_33,_41,_42,_43) * tInvDeterminant;
		mat[13] =  DETERMINANT3X3(_11,_12,_13,_31,_32,_33,_41,_42,_43) * tInvDeterminant;
		mat[14] = -DETERMINANT3X3(_11,_12,_13,_21,_22,_23,_41,_42,_43) * tInvDeterminant;
		mat[15] =  DETERMINANT3X3(_11,_12,_13,_21,_22,_23,_31,_32,_33) * tInvDeterminant;

		memcpy( Element, mat, Order * sizeof(TReal) );

		// Undefine determinant macro
		#undef DETERMINANT3X3
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the inverse of the matrix, if the matrix is not singular.
	/// \returns The inverse matrix
	//----------------------------------------------------------------------------------------------
	// TODO : 
	//	Factorise w. internal determinant
	//	Optimise process
	//----------------------------------------------------------------------------------------------
	TMatrix4 InvertCopy(void) const
	{
		// Macro returning determinant of 3x3 matrix
		#define DETERMINANT3X3(a, b, c, d, e, f, g, h, i) (a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g))

		TMatrix4 result;
		TReal tInvDeterminant = (TReal)1.0 / Determinant();

		// b(i,j) = pow(-1, i+j) * det(A(j,i))/det(A)
		result._11 =  DETERMINANT3X3(_22,_23,_24,_32,_33,_34,_42,_43,_44) * tInvDeterminant;
		result._12 = -DETERMINANT3X3(_12,_13,_14,_32,_33,_34,_42,_43,_44) * tInvDeterminant;
		result._13 =  DETERMINANT3X3(_12,_13,_14,_22,_23,_24,_42,_43,_44) * tInvDeterminant;
		result._14 = -DETERMINANT3X3(_12,_13,_14,_22,_23,_24,_32,_33,_34) * tInvDeterminant;

		result._21 = -DETERMINANT3X3(_21,_23,_24,_31,_33,_34,_41,_43,_44) * tInvDeterminant;
		result._22 =  DETERMINANT3X3(_11,_13,_14,_31,_33,_34,_41,_43,_44) * tInvDeterminant;
		result._23 = -DETERMINANT3X3(_11,_13,_14,_21,_23,_24,_41,_43,_44) * tInvDeterminant;
		result._24 =  DETERMINANT3X3(_11,_13,_14,_21,_23,_24,_31,_33,_34) * tInvDeterminant;

		result._31 =  DETERMINANT3X3(_21,_22,_24,_31,_32,_34,_41,_42,_44) * tInvDeterminant;
		result._32 = -DETERMINANT3X3(_11,_12,_14,_31,_32,_34,_41,_42,_44) * tInvDeterminant;
		result._33 =  DETERMINANT3X3(_11,_12,_14,_21,_22,_24,_41,_42,_44) * tInvDeterminant;
		result._34 = -DETERMINANT3X3(_11,_12,_14,_21,_22,_24,_31,_32,_34) * tInvDeterminant;

		result._41 = -DETERMINANT3X3(_21,_22,_23,_31,_32,_33,_41,_42,_43) * tInvDeterminant;
		result._42 =  DETERMINANT3X3(_11,_12,_13,_31,_32,_33,_41,_42,_43) * tInvDeterminant;
		result._43 = -DETERMINANT3X3(_11,_12,_13,_21,_22,_23,_41,_42,_43) * tInvDeterminant;
		result._44 =  DETERMINANT3X3(_11,_12,_13,_21,_22,_23,_31,_32,_33) * tInvDeterminant;

		return result;

		// Undefine determinant macro
		#undef DETERMINANT3X3
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the inverse of the matrix; if the matrix has no inverse an exception is thrown.
	//----------------------------------------------------------------------------------------------
	void Invert_Safe(void) 
	{
		// Macro returning determinant of 3x3 matrix
		#define DETERMINANT3X3(a, b, c, d, e, f, g, h, i) (a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g))

		TReal tDeterminant = Determinant();

		if ( IsSingular() )
			throw new MesonException("Matrix inverse is undefined", __FILE__, __LINE__);

		TReal mat[16], tInvDeterminant = 1.0 / tDeterminant;

		// b(i,j) = pow(-1, i+j) * det(A(j,i))/det(A)
		mat[0]  =  DETERMINANT3X3(_22,_23,_24,_32,_33,_34,_42,_43,_44) * tInvDeterminant;
		mat[1]  = -DETERMINANT3X3(_12,_13,_14,_32,_33,_34,_42,_43,_44) * tInvDeterminant;
		mat[2]  =  DETERMINANT3X3(_12,_13,_14,_22,_23,_24,_42,_43,_44) * tInvDeterminant;
		mat[3]  = -DETERMINANT3X3(_12,_13,_14,_22,_23,_24,_32,_33,_34) * tInvDeterminant;

		mat[4]  = -DETERMINANT3X3(_21,_23,_24,_31,_33,_34,_41,_43,_44) * tInvDeterminant;
		mat[5]  =  DETERMINANT3X3(_11,_13,_14,_31,_33,_34,_41,_43,_44) * tInvDeterminant;
		mat[6]  = -DETERMINANT3X3(_11,_13,_14,_21,_23,_24,_41,_43,_44) * tInvDeterminant;
		mat[7]  =  DETERMINANT3X3(_11,_13,_14,_21,_23,_24,_31,_33,_34) * tInvDeterminant;

		mat[8]  =  DETERMINANT3X3(_21,_22,_24,_31,_32,_34,_41,_42,_44) * tInvDeterminant;
		mat[9]  = -DETERMINANT3X3(_11,_12,_14,_31,_32,_34,_41,_42,_44) * tInvDeterminant;
		mat[10] =  DETERMINANT3X3(_11,_12,_14,_21,_22,_24,_41,_42,_44) * tInvDeterminant;
		mat[11] = -DETERMINANT3X3(_11,_12,_14,_21,_22,_24,_31,_32,_34) * tInvDeterminant;

		mat[12] = -DETERMINANT3X3(_21,_22,_23,_31,_32,_33,_41,_42,_43) * tInvDeterminant;
		mat[13] =  DETERMINANT3X3(_11,_12,_13,_31,_32,_33,_41,_42,_43) * tInvDeterminant;
		mat[14] = -DETERMINANT3X3(_11,_12,_13,_21,_22,_23,_41,_42,_43) * tInvDeterminant;
		mat[15] =  DETERMINANT3X3(_11,_12,_13,_21,_22,_23,_31,_32,_33) * tInvDeterminant;

		memcpy( Element, mat, Order * sizeof(TReal) );
		
		// Undefine determinant macro
		#undef DETERMINANT3X3
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the inverse matrix, if the matrix is not singular. If the matrix is singular,
	/// an exception is thrown.
	/// \returns The inverse matrix
	//----------------------------------------------------------------------------------------------
	// TODO : 
	//	Factorise w. internal determinant
	//	Optimise process
	//----------------------------------------------------------------------------------------------
	TMatrix4 InvertCopy_Safe(void) const
	{
		// Macro returning determinant of 3x3 matrix
		#define DETERMINANT3X3(a, b, c, d, e, f, g, h, i) (a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g))

		TMatrix4 result;
		TReal tDeterminant = Determinant();

		if ( IsSingular() )
			throw new MesonException("Matrix inverse is undefined", __FILE__, __LINE__);

		TReal tInvDeterminant = 1.0 / tDeterminant;

		// b(i,j) = pow(-1, i+j) * det(A(j,i))/det(A)
		result._11 =  DETERMINANT3X3(_22,_23,_24,_32,_33,_34,_42,_43,_44) * tInvDeterminant;
		result._12 = -DETERMINANT3X3(_12,_13,_14,_32,_33,_34,_42,_43,_44) * tInvDeterminant;
		result._13 =  DETERMINANT3X3(_12,_13,_14,_22,_23,_24,_42,_43,_44) * tInvDeterminant;
		result._14 = -DETERMINANT3X3(_12,_13,_14,_22,_23,_24,_32,_33,_34) * tInvDeterminant;

		result._21 = -DETERMINANT3X3(_21,_23,_24,_31,_33,_34,_41,_43,_44) * tInvDeterminant;
		result._22 =  DETERMINANT3X3(_11,_13,_14,_31,_33,_34,_41,_43,_44) * tInvDeterminant;
		result._23 = -DETERMINANT3X3(_11,_13,_14,_21,_23,_24,_41,_43,_44) * tInvDeterminant;
		result._24 =  DETERMINANT3X3(_11,_13,_14,_21,_23,_24,_31,_33,_34) * tInvDeterminant;

		result._31 =  DETERMINANT3X3(_21,_22,_24,_31,_32,_34,_41,_42,_44) * tInvDeterminant;
		result._32 = -DETERMINANT3X3(_11,_12,_14,_31,_32,_34,_41,_42,_44) * tInvDeterminant;
		result._33 =  DETERMINANT3X3(_11,_12,_14,_21,_22,_24,_41,_42,_44) * tInvDeterminant;
		result._34 = -DETERMINANT3X3(_11,_12,_14,_21,_22,_24,_31,_32,_34) * tInvDeterminant;

		result._41 = -DETERMINANT3X3(_21,_22,_23,_31,_32,_33,_41,_42,_43) * tInvDeterminant;
		result._42 =  DETERMINANT3X3(_11,_12,_13,_31,_32,_33,_41,_42,_43) * tInvDeterminant;
		result._43 = -DETERMINANT3X3(_11,_12,_13,_21,_22,_23,_41,_42,_43) * tInvDeterminant;
		result._44 =  DETERMINANT3X3(_11,_12,_13,_21,_22,_23,_31,_32,_33) * tInvDeterminant;

		return result;

		// Undefine determinant macro
		#undef DETERMINANT3X3
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the inverse of an orthonormal matrix.
	//----------------------------------------------------------------------------------------------
	void InvertOrthonormal(void) 
	{
		Transpose();
	}


	//----------------------------------------------------------------------------------------------
	/// Computes the inverse of an orthonormal matrix.
	/// \returns The inverse matrix
	//----------------------------------------------------------------------------------------------
	TMatrix4 InvertOrthonormalCopy(void) const
	{
		TMatrix4 result;

		result._11 = _11; result._12 = _21; result._13 = _31; result._14 = _41;
		result._21 = _12; result._22 = _22; result._23 = _32; result._24 = _42;
		result._31 = _13; result._32 = _23; result._33 = _33; result._34 = _43;
		result._41 = _14; result._42 = _24; result._43 = _34; result._44 = _44;

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the addition of two matrices.
	/// \param p_matrix Matrix to compute addition with
	/// \returns Addition result
	//----------------------------------------------------------------------------------------------
	TMatrix4 operator+ (const TMatrix4& p_matrix) const
	{
		TMatrix4 result;

		for (int n=0; n<Order; n++)
			result.Element[n] = Element[n] + p_matrix.Element[n];

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the difference between two matrices.
	/// \param p_matrix Matrix to compute difference with
	/// \returns Difference result
	//----------------------------------------------------------------------------------------------
	TMatrix4 operator- (const TMatrix4& p_matrix) const
	{
		TMatrix4 result;

		for (int n=0; n<Order; n++)
			result.Element[n] = Element[n] - p_matrix.Element[n];

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the multiplication of a matrix with a scalar.
	/// \param p_tValue Scalar value by which to multiply each matrix element
	/// \returns Scaled matrix
	//----------------------------------------------------------------------------------------------
	TMatrix4 operator* (TReal p_tValue) const
	{
		TMatrix4 result;

		for (int n=0; n<Order; n++)
			result.Element[n] = Element[n] * p_tValue;

		return result;
	}

	friend TMatrix4 operator* (TReal p_tValue, const TMatrix4& p_matrix)
	{
		TMatrix4 result;

		for (int n=0; n<Order; n++)
			result.Element[n] = p_matrix.Element[n] * p_tValue;

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the division of a matrix by a scalar.
	/// \param p_tValue Scalar value by which to divide each matrix element
	/// \returns Scaled matrix
	//----------------------------------------------------------------------------------------------
	TMatrix4 operator/ (TReal p_tValue) const
	{
		TMatrix4 result;

		if ( !TMaths<TReal>::Equals(p_tValue, (TReal)0.0) )
		{
			TReal tInvValue = (TReal)1.0 / p_tValue;

			for (int n=0; n<Order; n++)
				result.Element[n] = Element[n] * tInvValue;
		}
		else
		{
			for (int n=0; n<Order; n++)
				result.Element[n] = TMaths<TReal>::Maximum;
		}

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the addition of two matrices and stores the result in the matrix.
	//----------------------------------------------------------------------------------------------
	TMatrix4& operator+= (const TMatrix4& p_matrix)
	{
		for (int n=0; n<Order; n++)
			Element[n] += p_matrix.Element[n];

		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the difference of two matrices and stores the result in the matrix.
	//----------------------------------------------------------------------------------------------
	TMatrix4& operator-= (const TMatrix4& p_matrix)
	{
		for (int n=0; n<Order; n++)
			Element[n] -= p_matrix.Element[n];

		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Multiplies the matrix by a scaling factor.
	//----------------------------------------------------------------------------------------------
	TMatrix4& operator*= (TReal p_tValue)
	{
		for (int n=0; n<Order; n++)
			Element[n] *= p_tValue;

		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Divides the matrix by a scaling factor.
	//----------------------------------------------------------------------------------------------
	TMatrix4& operator/= (TReal p_tValue)
	{
		if ( !TMaths<TReal>::Equals(p_tValue, (TReal)0.0) )
		{
			TReal tInvValue = (TReal)1.0 / p_tValue;

			for (int n=0; n<Order; n++)
				Element[n] *= tInvValue;
		}
		else
		{
			for (int n=0; n<Order; n++)
				Element[n] = TMaths<TReal>::Maximum;
		}

		return *this;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Performs multiplication between a matrix and a vector. 
	//----------------------------------------------------------------------------------------------
	TVector2<TReal> operator* (const TVector2<TReal>& p_vector) const
	{
		return TVector2<TReal>( _11 * p_vector.X + _12 * p_vector.Y + _13 + _14,
								_21 * p_vector.X + _22 * p_vector.Y + _23 + _24,
								_31 * p_vector.X + _32 * p_vector.Y + _33 + _34);
	}

	//----------------------------------------------------------------------------------------------
	/// Performs multiplication between a matrix and a vector. 
	//----------------------------------------------------------------------------------------------
	TVector3<TReal> operator* (const TVector3<TReal>& p_vector) const
	{
		return TVector3<TReal>( _11 * p_vector.X + _12 * p_vector.Y + _13 * p_vector.Z + _14,
								_21 * p_vector.X + _22 * p_vector.Y + _23 * p_vector.Z + _24,
								_31 * p_vector.X + _32 * p_vector.Y + _33 * p_vector.Z + _34);
	}

	//----------------------------------------------------------------------------------------------
	/// Performs multiplication between a matrix and a vector. 
	//----------------------------------------------------------------------------------------------
	TVector4<TReal> operator* (const TVector4<TReal>& p_vector) const
	{
		return TVector4<TReal>( _11 * p_vector.X + _12 * p_vector.Y + _13 * p_vector.Z + _14 * p_vector.W,
								_21 * p_vector.X + _22 * p_vector.Y + _23 * p_vector.Z + _24 * p_vector.W,
								_31 * p_vector.X + _32 * p_vector.Y + _33 * p_vector.Z + _34 * p_vector.W,
								_41 * p_vector.X + _42 * p_vector.Y + _43 * p_vector.Z + _44 * p_vector.W );
	}

	//----------------------------------------------------------------------------------------------
	/// Performs multiplication between two matrices.
	//----------------------------------------------------------------------------------------------
	TMatrix4 operator* (const TMatrix4& p_matrix) const
	{
		TMatrix4 result;

		result._11 = _11 * p_matrix._11 + _12 * p_matrix._21 + _13 * p_matrix._31 + _14 * p_matrix._41;
		result._12 = _11 * p_matrix._12 + _12 * p_matrix._22 + _13 * p_matrix._32 + _14 * p_matrix._42;
		result._13 = _11 * p_matrix._13 + _12 * p_matrix._23 + _13 * p_matrix._33 + _14 * p_matrix._43;
		result._14 = _11 * p_matrix._14 + _12 * p_matrix._24 + _13 * p_matrix._34 + _14 * p_matrix._44;

		result._21 = _21 * p_matrix._11 + _22 * p_matrix._21 + _23 * p_matrix._31 + _24 * p_matrix._41;
		result._22 = _21 * p_matrix._12 + _22 * p_matrix._22 + _23 * p_matrix._32 + _24 * p_matrix._42;
		result._23 = _21 * p_matrix._13 + _22 * p_matrix._23 + _23 * p_matrix._33 + _24 * p_matrix._43;
		result._24 = _21 * p_matrix._14 + _22 * p_matrix._24 + _23 * p_matrix._34 + _24 * p_matrix._44;

		result._31 = _31 * p_matrix._11 + _32 * p_matrix._21 + _33 * p_matrix._31 + _34 * p_matrix._41;
		result._32 = _31 * p_matrix._12 + _32 * p_matrix._22 + _33 * p_matrix._32 + _34 * p_matrix._42;
		result._33 = _31 * p_matrix._13 + _32 * p_matrix._23 + _33 * p_matrix._33 + _34 * p_matrix._43;
		result._34 = _31 * p_matrix._14 + _32 * p_matrix._24 + _33 * p_matrix._34 + _34 * p_matrix._44;

		result._41 = _41 * p_matrix._11 + _42 * p_matrix._21 + _43 * p_matrix._31 + _44 * p_matrix._41;
		result._42 = _41 * p_matrix._12 + _42 * p_matrix._22 + _43 * p_matrix._32 + _44 * p_matrix._42;
		result._43 = _41 * p_matrix._13 + _42 * p_matrix._23 + _43 * p_matrix._33 + _44 * p_matrix._43;
		result._44 = _41 * p_matrix._14 + _42 * p_matrix._24 + _43 * p_matrix._34 + _44 * p_matrix._44;

		return result;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Copies the matrix through assignment.
	//----------------------------------------------------------------------------------------------
	TMatrix4& operator= (const TMatrix2<TReal>& p_matrix)
	{
		_11 = p_matrix._11;
		_12 = p_matrix._12;
		_21 = p_matrix._21;
		_22 = p_matrix._22;
		
		_13 = _14 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = (TReal)0.0;
		_33 = _44 = (TReal)1.0;

		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Copies the matrix through assignment.
	//----------------------------------------------------------------------------------------------
	TMatrix4& operator= (const TMatrix3<TReal>& p_matrix)
	{
		_11 = p_matrix._11;
		_12 = p_matrix._12;
		_13 = p_matrix._13;
		_21 = p_matrix._21;
		_22 = p_matrix._22;
		_23 = p_matrix._23;
		_31 = p_matrix._31;
		_32 = p_matrix._32;
		_33 = p_matrix._33;
		
		_14 = _24 = _34 = _41 = _42 = _43 = (TReal)0.0;
		_44 = (TReal)1.0;

		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Copies the matrix through assignment.
	//----------------------------------------------------------------------------------------------
	TMatrix4& operator= (const TMatrix4& p_matrix)
	{
		memcpy( Element, p_matrix.Element, sizeof(TReal) * Order );
		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Negates the matrix.
	//----------------------------------------------------------------------------------------------
	TMatrix4 operator- (void) const
	{
		return NegateCopy();
	}

	//----------------------------------------------------------------------------------------------
	/// Transposes the matrix.
	//----------------------------------------------------------------------------------------------
	TMatrix4 operator~ (void) const 
	{
		return TransposeCopy();
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	bool operator== (const TMatrix4& p_matrix) const
	{
		for (int nIdx = 0; nIdx < Order; nIdx++)
			if ( !TMaths<TReal>::Equals( Element[nIdx], p_matrix.Element[nIdx] ) )
				return false;

		return true;
	}
	
	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	bool operator!= (const TMatrix4& p_matrix) const
	{
		for (int nIdx = 0; nIdx < Order; nIdx++)
			if ( !TMaths<TReal>::Equals( Element[nIdx], p_matrix.Element[nIdx] ) )
				return true;

		return false;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the matrix array list under the cast operator.
	//----------------------------------------------------------------------------------------------
	operator TReal*() const
	{
		return (TReal*)&Element;
	}

	template <typename TRealParam>
	operator TMatrix4<TRealParam>() const
	{
		TMatrix4<TRealParam> matrix;
		matrix._11 = (TRealParam) _11;
		matrix._12 = (TRealParam) _12;
		matrix._13 = (TRealParam) _13;
		matrix._14 = (TRealParam) _14;
		matrix._21 = (TRealParam) _21;
		matrix._22 = (TRealParam) _22;
		matrix._23 = (TRealParam) _23;
		matrix._24 = (TRealParam) _24;
		matrix._31 = (TRealParam) _31;
		matrix._32 = (TRealParam) _32;
		matrix._33 = (TRealParam) _33;
		matrix._34 = (TRealParam) _34;
		matrix._41 = (TRealParam) _41;
		matrix._42 = (TRealParam) _42;
		matrix._43 = (TRealParam) _43;
		matrix._44 = (TRealParam) _44;
		return matrix;
	}

private:
	//----------------------------------------------------------------------------------------------
	inline int Index(int p_nRow, int p_nColumn) const
	{
		MESON_ASSERT( p_nRow >= 0 && p_nRow < Rows && p_nColumn >= 0 && p_nColumn < Columns, "Matrix indices out of range!" );
		return p_nRow * Columns + p_nColumn;
	}
};

//----------------------------------------------------------------------------------------------
// Static member initialisation
//----------------------------------------------------------------------------------------------
template <typename TReal> TMatrix4<TReal> const TMatrix4<TReal>::Identity	= TMatrix4<TReal>(true);
template <typename TReal> TMatrix4<TReal> const TMatrix4<TReal>::Zero		= TMatrix4<TReal>(false);

//----------------------------------------------------------------------------------------------
// Type definitions
//----------------------------------------------------------------------------------------------
typedef TMatrix4<double> Matrix4;
typedef TMatrix4<float>  Matrix4f;

Meson_Common_Maths_END