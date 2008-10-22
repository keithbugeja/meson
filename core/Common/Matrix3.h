//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::TMatrix3<T>
//	Templated class for 3x3 matrices.
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

#include <memory.h>
#include <algorithm>

Meson_Common_Maths_BEGIN

// TMatrix3 template
template <typename TReal> class TMatrix3
{
public:
	//----------------------------------------------------------------------------------------------
	static const int Rows	 = 3;
	static const int Columns = 3;
	static const int Order	 = Rows * Columns;

	static const TMatrix3 Identity;
	static const TMatrix3 Zero;
	
	//----------------------------------------------------------------------------------------------
	union
	{
		TReal Element[Order];
		struct 
		{
			TReal _11, _12, _13,
				  _21, _22, _23,
				  _31, _32, _33;
		};
	};

	//----------------------------------------------------------------------------------------------
public:
	//----------------------------------------------------------------------------------------------
	/// Constructs an uninitialised 3x3 matrix. 
	//----------------------------------------------------------------------------------------------
	TMatrix3(void) { }

	//----------------------------------------------------------------------------------------------
	/// Constructs a new 3x3 matrix.
	/// \param p_bIdentity Determines whether the new matrix is created as an identity matrix
	//----------------------------------------------------------------------------------------------
	TMatrix3(bool p_bIdentity)
	{
		p_bIdentity ? MakeIdentity() : MakeZero();
	}

	//----------------------------------------------------------------------------------------------
	/// Constructs a new 3x3 matrix from the provided real array.
	/// \param p_tElements[Order] Array of <b>TReal</b> elements used to populate matrix
	/// \param p_bRowMajor Specifies whether matrix is populated in row or column major order
	//----------------------------------------------------------------------------------------------
	TMatrix3(const TReal p_tElements[Order], bool p_bRowMajor = true)
	{
		if (p_bRowMajor) {
			memcpy( Element, p_tElements, sizeof(TReal) * Order );
		}
		else
		{
			_11 = p_tElements[0];
			_21 = p_tElements[1];
			_31 = p_tElements[2];
			_12 = p_tElements[3];
			_22 = p_tElements[4];
			_32 = p_tElements[5];
			_13 = p_tElements[6];
			_23 = p_tElements[7];
			_33 = p_tElements[8];
		}
	}

	//----------------------------------------------------------------------------------------------
	/// TMatrix3 copy constructor.
	/// \param p_matrix A matrix object used as a source for copy
	//----------------------------------------------------------------------------------------------
	TMatrix3(const TMatrix3& p_matrix) 
	{
		memcpy( Element, p_matrix.Element, sizeof(TReal) * Order );
	}
	
	//----------------------------------------------------------------------------------------------
	/// TMatrix2 copy constructor.
	/// \param p_matrix A matrix object used as a source for copy
	//----------------------------------------------------------------------------------------------
	TMatrix3(const TMatrix2<TReal>& p_matrix) 
	{
		*this = p_matrix;
	}

	//----------------------------------------------------------------------------------------------
	/// TMatrix3 constructor for a combined rotation matrix.
	/// \param p_vecAxesRotation A vector containing the rotation for each axis
	/// \param p_eAxisOrder Determines the axial order of the rotation
	//----------------------------------------------------------------------------------------------
	TMatrix3(const TVector3<TReal>& p_vecAxesRotation, AxisOrder::AxisOrder p_eAxisOrder) 
	{
		MakeRotation( p_vecAxesRotation, p_eAxisOrder);
	}

	//----------------------------------------------------------------------------------------------
	/// TMatrix3 constructor for a rotation matrix about a carthesian axis.
	/// \param p_tAngle Angle of rotation
	/// \param p_eAxis Axis of rotation
	//----------------------------------------------------------------------------------------------
	TMatrix3(TReal p_tAngle, Axis::Axis p_eAxis) 
	{
		MakeRotation( p_tAngle, p_eAxis );
	}

	//----------------------------------------------------------------------------------------------
	/// TMatrix3 constructor for a rotation matrix about an arbitrary axis.
	/// \param p_vecAxis Normalised vector representing rotation axis
	/// \param p_tAngle Rotation extent in radians
	//----------------------------------------------------------------------------------------------
	TMatrix3(const TVector3<TReal>& p_vecAxis, TReal p_tAngle) 
	{
		MakeRotation( p_vecAxis, p_tAngle );
	}
	
	//----------------------------------------------------------------------------------------------
	/// TMatrix3 constructor for a rotation matrix about an arbitrary axis.
	/// \param p_vecAxis Normalised vector representing rotation axis
	/// \param p_tAngle Rotation extent in radians
	//----------------------------------------------------------------------------------------------
	TMatrix3(const TVector3<TReal>& p_vec3One, const TVector3<TReal>& p_vec3Two, const TVector3<TReal>& p_vec3Three, bool p_bRowMajor = true)
	{
		if (p_bRowMajor)
		{
			SetRow(0, p_vec3One);
			SetRow(1, p_vec3Two);
			SetRow(2, p_vec3Three);
		}
		else
		{
			SetColumn(0, p_vec3One);
			SetColumn(1, p_vec3Two);
			SetColumn(2, p_vec3Three);
		}
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
				 TMaths<TReal>::Equals( _12, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _13, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _21, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _23, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _31, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _32, (TReal)0.0 ) );
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline bool IsZero(void) const
	{
		return ( TMaths<TReal>::Equals( _11, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _22, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _33, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _12, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _13, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _21, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _23, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _31, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _32, (TReal)0.0 ) );
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into a Zero matrix.
	//----------------------------------------------------------------------------------------------
	void MakeZero(void) 
	{
		_11 = _12 = _13 = _21 = _22 = _23 = _31 = _32 = _33 = (TReal)0.0;
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into an Identity matrix.
	//----------------------------------------------------------------------------------------------
	void MakeIdentity(void)
	{
		_12 = _13 = _21 = _23 = _31 = _32 = (TReal)0.0;
		_11 = _22 = _33 = (TReal)1.0;
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into a diagonal matrix.
	///	\param p_vector A vector containing the diagonal components
	//----------------------------------------------------------------------------------------------
	void MakeDiagonal(const TVector3<TReal>& p_vector)
	{
		_12 = _13 = _21 = _23 = _31 = _32 = (TReal)0.0;

		_11 = p_vector.X;
		_22 = p_vector.Y;
		_33 = p_vector.Z;
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into a scaling matrix.
	///	\param p_vector A vector containing the scaling components
	//----------------------------------------------------------------------------------------------
	void MakeScaling(const TVector3<TReal>& p_vector)
	{
		_12 = _13 = _21 = _23 = _31 = _32 = (TReal)0.0;

		_11 = p_vector.X;
		_22 = p_vector.Y;
		_33 = p_vector.Z;
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
				_11 = (TReal)1.0;
				_12 = _13 = _21 = _31 = (TReal)0.0;
				
				_22 = _33 = tCos;
				_23 = tSin;
				_32 = -tSin;
				return;

			case Axis::Y:
				_22 = (TReal)1.0;
				_12 = _21 = _23 = _32 = (TReal)0.0;

				_11 = _33 = tCos;
				_13 = -tSin;
				_31 = tSin;
				return;

			case Axis::Z:
				_33 = (TReal)1.0;
				_13 = _23 = _31 = _32 = (TReal)0.0;
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
	/// Extracts an axis and a rotation angle from matrix.
	/// \returns A vector containing the axis and angle of rotation
	//----------------------------------------------------------------------------------------------
	TVector4<TReal> ExtractRotation(void) const
	{
		TVector4<TReal> result;
		ExtractRotation(result);
		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Extracts an axis and a rotation angle from matrix.
	/// \param p_vector Vector for holding axis and rotation
	//----------------------------------------------------------------------------------------------
	//  TODO: Refine when angle is zero or a multiple of PI 
	//----------------------------------------------------------------------------------------------
	void ExtractRotation(TVector4<TReal>& p_vec4Out) const
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
			p_vec4Out = TVector4<TReal>::Zero;
	}

	//----------------------------------------------------------------------------------------------
	/// Extracts the scaling amount from the matrix.
	/// \returns A vector containing the scaling components
	//----------------------------------------------------------------------------------------------
	TVector3<TReal> ExtractScaling(void) const
	{
		TVector3<TReal> result;
		ExtractScaling(result);
		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Extracts the scaling amount from the matrix.
	/// \param p_vector Vector for holding scaling components
	//----------------------------------------------------------------------------------------------
	void ExtractScaling(TVector3<TReal> &p_vec3Out) const
	{
		p_vec3Out.X = _11;
		p_vec3Out.Y = _22;
		p_vec3Out.Z = _33;
	}


	//----------------------------------------------------------------------------------------------
	/// Creates a scaling matrix.
	///	\param p_vector A vector containing the scaling components
	/// \returns 3x3 Scaling matrix
	//----------------------------------------------------------------------------------------------
	static TMatrix3 CreateScaling(const TVector3<TReal>& p_vector)
	{
		TMatrix3 result;

		result._12 = result._13 = result._21 = result._23 = result._31 = result._32 = (TReal)0.0;

		result._11 = p_vector.X;
		result._22 = p_vector.Y;
		result._33 = p_vector.Z;

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Creates a rotation matrix about a Cartesian axis.  
	/// \param p_tAngle Angle of rotation about axis
	/// \param p_eAxis Axis of rotation
	/// \returns 3x3 Rotation matrix
	//----------------------------------------------------------------------------------------------
	static TMatrix3 CreateRotation(TReal p_tAngle, Axis::Axis p_eAxis) 
	{
		return TMatrix3( p_tAngle, p_eAxis);
	}

	//----------------------------------------------------------------------------------------------
	/// Creates a combined rotation matrix about the Cartesian axes.
	/// \param p_vecAxesRotation Angle of rotation about axes
	/// \param p_eAxisOrder Order of axial rotation
	/// \returns 3x3 Rotation matrix
	//----------------------------------------------------------------------------------------------
	static TMatrix3 CreateRotation(const TVector3<TReal>& p_vecAxesRotation, AxisOrder::AxisOrder p_eAxisOrder) 
	{
		return TMatrix3( p_vecAxesRotation, p_eAxisOrder);
	}

	//----------------------------------------------------------------------------------------------
	/// Creates a rotation matrix about an arbitrary axis.
	/// \param p_vecAxis Normalised vector representing rotation axis
	/// \param p_tAngle Rotation extent in radians
	/// \returns 3x3 Rotation matrix
	//----------------------------------------------------------------------------------------------
	static TMatrix3 CreateRotation(const TVector3<TReal>& p_vecAxis, TReal p_tAngle) 
	{
		return TMatrix3( p_vecAxis, p_tAngle );
	}

	//----------------------------------------------------------------------------------------------
	/// Sets the matrix scaling components.
	///	\param p_vector A vector containing the scaling components
	//----------------------------------------------------------------------------------------------
	void SetScaling(const TVector3<TReal>& p_vector)
	{
		_11 = p_vector.X;
		_22 = p_vector.Y;
		_33 = p_vector.Z;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the matrix element at the specified row and column.
	/// \param p_nRow Element row index
	/// \param p_nColumn Element column index
	/// \returns Element at specified index
	//----------------------------------------------------------------------------------------------
	TReal operator()(int p_nRow, int p_nColumn) const {
		return Element[ Index(p_nRow, p_nColumn) ];
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the reference of the matrix element at the specified row and column.
	/// \param p_nRow Zero-based row index
	/// \param p_nColumn Zero-based column index
	/// \returns Reference of element at specified index
	//----------------------------------------------------------------------------------------------
	TReal& operator()(int p_nRow, int p_nColumn) {
		return Element[ Index(p_nRow, p_nColumn) ];
	}

	//----------------------------------------------------------------------------------------------
	/// Sets a column of the matrix to the specified values.
	/// \param p_nColumn Index of column, zero-based
	/// \param p_vector Vector containing the values used to set the column
	//----------------------------------------------------------------------------------------------
	void SetColumn(int p_nColumn, const TVector3<TReal>& p_vector)
	{
		MESON_ASSERT( p_nColumn >= 0 && p_nColumn < Columns, "Matrix column index out of range!" );

		switch( p_nColumn )
		{
			case 0:
				_11 = p_vector.X; _21 = p_vector.Y; _31 = p_vector.Z;
				return;
			
			case 1:
				_12 = p_vector.X; _22 = p_vector.Y; _32 = p_vector.Z;
				return;
			
			case 2:
				_13 = p_vector.X; _23 = p_vector.Y; _33 = p_vector.Z;
				return;
		}
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the column vector at the specified index.
	/// \param p_nColumn Zero-based column index
	///	\returns Column vector
	//----------------------------------------------------------------------------------------------
	TVector3<TReal> GetColumn(int p_nColumn ) const
	{
		MESON_ASSERT( p_nColumn >= 0 && p_nColumn < Columns, "Matrix column index out of range!" );

		switch( p_nColumn )
		{
			case 0:
				return TVector3<TReal>( _11, _21, _31 );
			
			case 1:
				return TVector3<TReal>( _12, _22, _32 );
			
			case 2:
				return TVector3<TReal>( _13, _23, _33 );
		}

		return TVector3<TReal>::Zero;
	}

	//----------------------------------------------------------------------------------------------
	/// Sets a row of the matrix to the specified values.
	/// \param p_nRow Index of row, zero-based
	/// \param p_vector Vector containing the values used to set the row
	//----------------------------------------------------------------------------------------------
	void SetRow(int p_nRow, const TVector3<TReal>& p_vector)
	{
		MESON_ASSERT( p_nRow >= 0 && p_nRow < Rows, "Matrix row index out of range!" );

		switch( p_nRow )
		{
			case 0:
				_11 = p_vector.X; _12 = p_vector.Y; _13 = p_vector.Z;
				return;

			case 1:
				_21 = p_vector.X; _22 = p_vector.Y; _23 = p_vector.Z;
				return;

			case 2:
				_31 = p_vector.X; _32 = p_vector.Y; _33 = p_vector.Z;
				return;
		}
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the row vector at the specified index.
	/// \param p_nRow Zero-based row index
	///	\returns Row vector
	//----------------------------------------------------------------------------------------------
	TVector3<TReal> GetRow(int p_nRow) const
	{
		MESON_ASSERT( p_nRow >= 0 && p_nRow < Rows, "Matrix row index out of range!" );

		switch( p_nRow )
		{
			case 0:
				return TVector3<TReal>( _11, _12, _13 );

			case 1:
				return TVector3<TReal>( _21, _22, _23 );

			case 2:
				return TVector3<TReal>( _31, _32, _33 );
		}

		return TVector3<TReal>::Zero;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the determinant of the matrix.
	/// \returns Determinant
	//----------------------------------------------------------------------------------------------
	TReal Determinant(void) const
	{
		return _11 * (_22*_33 - _32*_23) - 
			   _12 * (_21*_33 - _31*_23) + 
			   _13 * (_21*_32 - _31*_22);
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
	/// Computes the negation of the matrix.
	/// \returns The negative matrix
	//----------------------------------------------------------------------------------------------
	TMatrix3 NegateCopy(void) const
	{
		TMatrix3 result;

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
		std::swap(_32, _23);
	}
	
	//----------------------------------------------------------------------------------------------
	/// Computes the transposition of the matrix.
	/// \returns The transposed matrix
	//----------------------------------------------------------------------------------------------
	TMatrix3 TransposeCopy(void) const
	{
		TMatrix3 result;

		result._11 = _11;
		result._12 = _21;
		result._13 = _31;

		result._21 = _12;
		result._22 = _22;
		result._23 = _32;

		result._31 = _13;
		result._32 = _23;
		result._33 = _33;

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the inverse of the matrix.
	//----------------------------------------------------------------------------------------------
	void Invert(void) 
	{
		TReal tDeterminant = Determinant();

		// Find inverse of determinant
		tDeterminant = (TReal)1.0 / tDeterminant;

		// Find inverse of each cell
		TReal t11 =  (_22*_33 - _23*_32) * tDeterminant,
			  t12 = -(_12*_33 - _32*_13) * tDeterminant,
			  t13 =  (_12*_23 - _22*_13) * tDeterminant,
		      t21 = -(_21*_33 - _23*_31) * tDeterminant,
			  t22 =  (_11*_33 - _31*_13) * tDeterminant,
			  t23 = -(_11*_23 - _21*_13) * tDeterminant,
			  t31 =  (_21*_32 - _21*_22) * tDeterminant,
			  t32 = -(_11*_32 - _31*_12) * tDeterminant,
			  t33 =  (_11*_22 - _12*_21) * tDeterminant;

		// Copy inverse to matrix cells
		_11 = t11; _12 = t12; _13 = t13;
		_21 = t21; _22 = t12; _23 = t23;
		_31 = t31; _32 = t12; _33 = t33;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the inverse of the matrix, if the matrix is not singular.
	/// \returns The inverse matrix
	//----------------------------------------------------------------------------------------------
	// TODO : 
	//	Factorise w. internal determinant
	//	Optimise process
	//----------------------------------------------------------------------------------------------
	TMatrix3 InvertCopy(void) const
	{
		TMatrix3 result;
		TReal tDeterminant = Determinant();

		tDeterminant = (TReal)1.0 / tDeterminant;

		result._11 =  (_22*_33 - _23*_32) * tDeterminant;
		result._12 = -(_12*_33 - _32*_13) * tDeterminant;
		result._13 =  (_12*_23 - _22*_13) * tDeterminant;
		result._21 = -(_21*_33 - _23*_31) * tDeterminant;
		result._22 =  (_11*_33 - _31*_13) * tDeterminant;
		result._23 = -(_11*_23 - _21*_13) * tDeterminant;
		result._31 =  (_21*_32 - _21*_22) * tDeterminant;
		result._32 = -(_11*_32 - _31*_12) * tDeterminant;
		result._33 =  (_11*_22 - _12*_21) * tDeterminant;

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the inverse of the matrix; if the matrix has no inverse an exception is thrown.
	//----------------------------------------------------------------------------------------------
	void Invert_Safe(void) 
	{
		TReal tDeterminant = Determinant();

		if ( IsSingular() )
			throw new MesonException("Matrix inverse is undefined", __FILE__, __LINE__);

		// Find inverse of determinant
		tDeterminant = (TReal)1.0 / tDeterminant;

		// Find inverse of each cell
		TReal t11 =  (_22*_33 - _23*_32) * tDeterminant,
			  t12 = -(_12*_33 - _32*_13) * tDeterminant,
			  t13 =  (_12*_23 - _22*_13) * tDeterminant,
		      t21 = -(_21*_33 - _23*_31) * tDeterminant,
			  t22 =  (_11*_33 - _31*_13) * tDeterminant,
			  t23 = -(_11*_23 - _21*_13) * tDeterminant,
			  t31 =  (_21*_32 - _21*_22) * tDeterminant,
			  t32 = -(_11*_32 - _31*_12) * tDeterminant,
			  t33 =  (_11*_22 - _12*_21) * tDeterminant;

		// Copy inverse to matrix cells
		_11 = t11; _12 = t12; _13 = t13;
		_21 = t21; _22 = t12; _23 = t23;
		_31 = t31; _32 = t12; _33 = t33;
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
	TMatrix3 InvertCopy_Safe(void) const
	{
		TMatrix3 result;
		TReal tDeterminant = Determinant();

		if ( IsSingular() )
			throw new MesonException("Matrix inverse is undefined", __FILE__, __LINE__);

		tDeterminant = (TReal)1.0 / tDeterminant;

		result._11 =  (_22*_33 - _23*_32) * tDeterminant;
		result._12 = -(_12*_33 - _32*_13) * tDeterminant;
		result._13 =  (_12*_23 - _22*_13) * tDeterminant;
		result._21 = -(_21*_33 - _23*_31) * tDeterminant;
		result._22 =  (_11*_33 - _31*_13) * tDeterminant;
		result._23 = -(_11*_23 - _21*_13) * tDeterminant;
		result._31 =  (_21*_32 - _21*_22) * tDeterminant;
		result._32 = -(_11*_32 - _31*_12) * tDeterminant;
		result._33 =  (_11*_22 - _12*_21) * tDeterminant;

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the inverse of an orthonormal matrix.
	//----------------------------------------------------------------------------------------------
	inline void InvertOrthonormal(void) 
	{
		Transpose();
	}
	
	//----------------------------------------------------------------------------------------------
	/// Computes the inverse of an orthonormal matrix.
	/// \returns The inverse matrix
	//----------------------------------------------------------------------------------------------
	inline TMatrix3 InvertOrthonormalCopy(void) const
	{
		return TransposeCopy();
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the addition of two matrices.
	//----------------------------------------------------------------------------------------------
	TMatrix3 operator+ (const TMatrix3& p_matrix) const
	{
		TMatrix3 result;

		for (int n=0; n<Order; n++)
			result.Element[n] = Element[n] + p_matrix.Element[n];

		return result;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Computes the difference between two matrices.
	//----------------------------------------------------------------------------------------------
	TMatrix3 operator- (const TMatrix3& p_matrix) const
	{
		TMatrix3 result;

		for (int n=0; n<Order; n++)
			result.Element[n] = Element[n] - p_matrix.Element[n];

		return result;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Computes the multiplication of a matrix with a scalar.
	//----------------------------------------------------------------------------------------------
	TMatrix3 operator* (TReal p_tValue) const
	{
		TMatrix3 result;

		for (int n=0; n<Order; n++)
			result.Element[n] = Element[n] * p_tValue;

		return result;
	}

	friend TMatrix3 operator* (TReal p_tValue, const TMatrix3& p_matrix)
	{
		TMatrix3 result;

		for (int n=0; n<Order; n++)
			result.Element[n] = p_matrix.Element[n] * p_tValue;

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the division of a matrix by a scalar.
	//----------------------------------------------------------------------------------------------
	TMatrix3 operator/ (TReal p_tValue) const
	{
		TMatrix3 result;

		if ( !TMaths<TReal>::Equals( p_tValue, (TReal)0.0 ) )
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
	TMatrix3& operator+= (const TMatrix3 &p_matrix)
	{
		for (int n=0; n<Order; n++)
			Element[n] += p_matrix.Element[n];

		return *this;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Computes the difference of two matrices and stores the result in the matrix.
	//----------------------------------------------------------------------------------------------
	TMatrix3& operator-= (const TMatrix3 &p_matrix)
	{
		for (int n=0; n<Order; n++)
			Element[n] -= p_matrix.Element[n];

		return *this;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Multiplies the matrix by a scaling factor.
	//----------------------------------------------------------------------------------------------
	TMatrix3& operator*= (TReal p_tValue)
	{
		for (int n=0; n<Order; n++)
			Element[n] *= p_tValue;

		return *this;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Divides the matrix by a scaling factor.
	//----------------------------------------------------------------------------------------------
	TMatrix3& operator/= (TReal p_tValue)
	{
		if ( !TMaths<TReal>::Equals( p_tValue, (TReal)0.0 ) )
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
	TVector3<TReal> operator* (const TVector3<TReal>& p_vector) const
	{
		return TVector3<TReal>( _11 * p_vector.X + _12 * p_vector.Y + _13 * p_vector.Z,
								_21 * p_vector.X + _22 * p_vector.Y + _23 * p_vector.Z,
								_31 * p_vector.X + _32 * p_vector.Y + _33 * p_vector.Z);
	}
	
	//----------------------------------------------------------------------------------------------
	/// Performs multiplication between two matrices.
	//----------------------------------------------------------------------------------------------
	TMatrix3 operator* (const TMatrix3& p_matrix) const
	{
		TMatrix3 result;

		result._11 = _11 * p_matrix._11 + _12 * p_matrix._21 + _13 * p_matrix._31;
		result._12 = _11 * p_matrix._12 + _12 * p_matrix._22 + _13 * p_matrix._32;
		result._13 = _11 * p_matrix._13 + _12 * p_matrix._23 + _13 * p_matrix._33;

		result._21 = _21 * p_matrix._11 + _22 * p_matrix._21 + _23 * p_matrix._31;
		result._22 = _21 * p_matrix._12 + _22 * p_matrix._22 + _23 * p_matrix._32;
		result._23 = _21 * p_matrix._13 + _22 * p_matrix._23 + _23 * p_matrix._33;

		result._31 = _31 * p_matrix._11 + _32 * p_matrix._21 + _33 * p_matrix._31;
		result._32 = _31 * p_matrix._12 + _32 * p_matrix._22 + _33 * p_matrix._32;
		result._33 = _31 * p_matrix._13 + _32 * p_matrix._23 + _33 * p_matrix._33;

		return result;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Copies the matrix through assignment.
	//----------------------------------------------------------------------------------------------
	TMatrix3& operator= (const TMatrix2<TReal>& p_matrix)
	{
		_11 = p_matrix._11;
		_12 = p_matrix._12;
		_21 = p_matrix._21;
		_22 = p_matrix._22;

		_13 = _23 = _31 = _32 = (TReal)0.0;
		_33 = (TReal)1.0;

		return *this;
	}

	//----------------------------------------------------------------------------------------------
	/// Copies the matrix through assignment.
	//----------------------------------------------------------------------------------------------
	TMatrix3& operator= (const TMatrix3& p_matrix)
	{
		memcpy( Element, p_matrix.Element, sizeof(TReal) * Order );
		return *this;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Negates the matrix.
	//----------------------------------------------------------------------------------------------
	TMatrix3 operator- (void) const 
	{
		return NegateCopy();
	}
	
	//----------------------------------------------------------------------------------------------
	/// Transposes the matrix.
	//----------------------------------------------------------------------------------------------
	TMatrix3 operator~ (void) const
	{
		return TransposeCopy();
	}
	
	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	bool operator== (const TMatrix3& p_matrix) const
	{
		for (int nIdx = 0; nIdx < Order; nIdx++)
			if ( !TMaths<TReal>::Equals( Element[nIdx], p_matrix.Element[nIdx] ) )
				return false;

		return true;
	}
	
	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	bool operator!= (const TMatrix3& p_matrix) const
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
	operator TMatrix3<TRealParam>() const
	{
		TMatrix3<TRealParam> matrix;
		matrix._11 = (TRealParam) _11;
		matrix._12 = (TRealParam) _12;
		matrix._13 = (TRealParam) _13;
		matrix._21 = (TRealParam) _21;
		matrix._22 = (TRealParam) _22;
		matrix._23 = (TRealParam) _23;
		matrix._31 = (TRealParam) _31;
		matrix._32 = (TRealParam) _32;
		matrix._33 = (TRealParam) _33;
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
template <typename TReal> TMatrix3<TReal> const TMatrix3<TReal>::Identity	= TMatrix3<TReal>(true);
template <typename TReal> TMatrix3<TReal> const TMatrix3<TReal>::Zero		= TMatrix3<TReal>(false);

//----------------------------------------------------------------------------------------------
// Type definitions
//----------------------------------------------------------------------------------------------
typedef TMatrix3<double> Matrix3;
typedef TMatrix3<float>	 Matrix3f;

Meson_Common_Maths_END