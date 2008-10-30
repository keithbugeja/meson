//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::TMatrix2<T>
//	Templated class for 2x2 matrices.
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

#include <memory.h>
#include <algorithm>

Meson_Common_Maths_BEGIN

// TMatrix2 template
template <typename TReal> 
class TMatrix2
{
public:
	//----------------------------------------------------------------------------------------------
	static const int Rows	 = 2;
	static const int Columns = 2;
	static const int Order	 = Rows * Columns;

	static const TMatrix2 Identity;
	static const TMatrix2 Zero;

	//----------------------------------------------------------------------------------------------
	union
	{
		TReal Element[Order];
		struct 
		{
			TReal _11, _12,
				  _21, _22;
		};
	};

public:
	//----------------------------------------------------------------------------------------------
	/// Constructs an uninitialised 2x2 matrix. 
	//----------------------------------------------------------------------------------------------
	TMatrix2(void) { }

	//----------------------------------------------------------------------------------------------
	/// Constructs a new 2x2 matrix.
	/// \param p_bIdentity Determines whether the new matrix is created as an identity matrix
	//----------------------------------------------------------------------------------------------
	TMatrix2(bool p_bIdentity)
	{
		p_bIdentity ? MakeIdentity() : MakeZero();
	}

	//----------------------------------------------------------------------------------------------
	/// Constructs a new 3x3 matrix from the provided real array.
	/// \param p_tElements[Order] Array of <b>TReal</b> elements used to populate matrix
	/// \param p_bRowMajor Specifies whether matrix is populated in row or column major order
	//----------------------------------------------------------------------------------------------
	TMatrix2(const TReal p_tElements[Order], bool p_bRowMajor = true)
	{
		if (p_bRowMajor) {
			memcpy( Element, p_tElements, sizeof(TReal) * Order );
		}
		else
		{
			_11 = p_tElements[0];
			_21 = p_tElements[1];
			_12 = p_tElements[2];
			_22 = p_tElements[3];
		}
	}

	//----------------------------------------------------------------------------------------------
	/// TMatrix2 copy constructor.
	/// \param p_matrix A matrix object used as a source for copy
	//----------------------------------------------------------------------------------------------
	TMatrix2(const TMatrix2& p_matrix) {
		memcpy( Element, p_matrix.Element, sizeof(TReal) * Order );
	}

	//----------------------------------------------------------------------------------------------
	/// TMatrix2 constructor for a rotation matrix.
	/// \param p_tAngle Angle of rotation
	//----------------------------------------------------------------------------------------------
	TMatrix2(TReal p_tAngle) {
		MakeRotation( p_tAngle );
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
				 TMaths<TReal>::Equals( _12, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _21, (TReal)0.0 ));
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	inline bool IsZero(void) const
	{
		return ( TMaths<TReal>::Equals( _11, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _22, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _12, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( _21, (TReal)0.0 ));
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into a Zero matrix.
	//----------------------------------------------------------------------------------------------
	void MakeZero(void)
	{
		_11 = _12 = _21 = _22 = (TReal)0.0;
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into an Identity matrix.
	//----------------------------------------------------------------------------------------------
	void MakeIdentity(void)
	{
		_12 = _21 = (TReal)0.0;
		_11 = _22 = (TReal)1.0;
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into a diagonal matrix.
	///	\param p_vector A vector containing the diagonal components
	//----------------------------------------------------------------------------------------------
	void MakeDiagonal(const TVector2<TReal>& p_vector)
	{
		_12 = _21 = (TReal)0.0;

		_11 = p_vector.X;
		_22 = p_vector.Y;
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into a scaling matrix.
	///	\param p_vector A vector containing the scaling components
	//----------------------------------------------------------------------------------------------
	void MakeScaling(const TVector2<TReal>& p_vector)
	{
		_12 = _21 = (TReal)0.0;

		_11 = p_vector.X;
		_22 = p_vector.Y;
	}

	//----------------------------------------------------------------------------------------------
	/// Transforms the matrix into a rotation matrix.
	/// \param p_tAngle The angle of rotation in radians
	//----------------------------------------------------------------------------------------------
	void MakeRotation(const TReal p_tAngle)
	{
		_11 = _22 = TMaths<TReal>::Cos( p_tAngle ),
		_21 = TMaths<TReal>::Sin( p_tAngle );
		_12 = -_21;
	}

	//----------------------------------------------------------------------------------------------
	/// Extracts an axis and a rotation angle from matrix.
	/// \returns The angle of rotation
	//----------------------------------------------------------------------------------------------
	TReal ExtractRotation(void) const
	{
		return TMaths<TReal>::ATan( _21, _11 );
	}

	//----------------------------------------------------------------------------------------------
	/// Extracts the scaling amount from the matrix.
	/// \returns A vector containing the scaling components
	//----------------------------------------------------------------------------------------------
	TVector2<TReal> ExtractScaling(void) const
	{
		TVector2<TReal> result;
		ExtractScaling( result );
		return result;
	}
	//----------------------------------------------------------------------------------------------
	/// Extracts the scaling amount from the matrix.
	/// \param p_vector Vector for holding scaling components
	//----------------------------------------------------------------------------------------------
	void ExtractScaling(TVector2<TReal>& p_vector) const
	{
		p_vector.X = _11;
		p_vector.Y = _22;
	}

	//----------------------------------------------------------------------------------------------
	/// Creates a scaling matrix.
	///	\param p_vector A vector containing the scaling components
	/// \returns 2x2 Scaling matrix
	//----------------------------------------------------------------------------------------------
	static TMatrix2 CreateScaling(const TVector2<TReal>& p_vector)
	{
		TMatrix2 result;

		result._12 = result._21 = (TReal)0.0;

		result._11 = p_vector.X;
		result._22 = p_vector.Y;

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Creates a rotation matrix about a Cartesian axis.  
	/// \param p_tAngle Angle of rotation about axis
	/// \param p_eAxis Axis of rotation
	/// \returns 2x2 Rotation matrix
	//----------------------------------------------------------------------------------------------
	static TMatrix2 CreateRotation(TReal p_tAngle)
	{
		return TMatrix2( p_tAngle );
	}
	
	//----------------------------------------------------------------------------------------------
	/// Sets the matrix scaling components.
	///	\param p_vector A vector containing the scaling components
	//----------------------------------------------------------------------------------------------
	void SetScaling(const TVector2<TReal>& p_vector)
	{
		_11 = p_vector.X;
		_22 = p_vector.Y;
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
	void SetColumn(int p_nColumn, const TVector2<TReal> &p_vector)
	{
		MESON_ASSERT( p_nColumn >= 0 && p_nColumn < Columns, "Matrix column index out of range!" );

		if (p_nColumn == 0) {
			_11 = p_vector.X; _21 = p_vector.Y;
		}
		else if (p_nColumn == 1) {
			_12 = p_vector.X; _22 = p_vector.Y;
		}
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the column vector at the specified index.
	/// \param p_nColumn Zero-based column index
	///	\returns Column vector
	//----------------------------------------------------------------------------------------------
	TVector2<TReal> GetColumn(int p_nColumn) const
	{
		MESON_ASSERT( p_nColumn >= 0 && p_nColumn < Columns, "Matrix column index out of range!" );

		if (p_nColumn == 0) {
			return TVector2<TReal>( _11, _21 );
		}
		else if (p_nColumn == 1) {
			return TVector2<TReal>( _12, _22 );
		}

		return TVector2<TReal>::Zero;
	}

	//----------------------------------------------------------------------------------------------
	/// Sets a row of the matrix to the specified values.
	/// \param p_nRow Index of row, zero-based
	/// \param p_vector Vector containing the values used to set the row
	//----------------------------------------------------------------------------------------------
	void SetRow(int p_nRow, const TVector2<TReal>& p_vector)
	{
		MESON_ASSERT( p_nRow >= 0 && p_nRow < Rows, "Matrix row index out of range!" );

		if (p_nRow == 0) {
			_11 = p_vector.X; _12 = p_vector.Y;
		}
		else if (p_nRow == 1) {
			_21 = p_vector.X; _22 = p_vector.Y;
		}
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the row vector at the specified index.
	/// \param p_nRow Zero-based row index
	///	\returns Row vector
	//----------------------------------------------------------------------------------------------
	TVector2<TReal> GetRow(int p_nRow) const
	{
		MESON_ASSERT( p_nRow >= 0 && p_nRow < Rows, "Matrix row index out of range!" );

		if (p_nRow == 0) {
			return TVector2<TReal>( _11, _12 );
		}
		else if (p_nRow == 1) {
			return TVector2<TReal>( _21, _22 );
		}

		return TVector2<TReal>::Zero;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Returns the determinant of the matrix.
	/// \returns Determinant
	//----------------------------------------------------------------------------------------------
	inline TReal Determinant(void) const
	{
		return _11 * _22 - _12 * _21;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Negates the matrix elements; the same as multiplying the matrix by -1.
	//----------------------------------------------------------------------------------------------
	void Negate(void)
	{
		_11 = -_11; 
		_12 = -_12;
		_21 = -_21; 
		_22 = -_22;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the negation of the matrix.
	/// \returns The negative matrix
	//----------------------------------------------------------------------------------------------
	TMatrix2 NegateCopy(void) const
	{
		TMatrix2 result;

		result._11 = -_11;
		result._12 = -_12;
		result._21 = -_21;
		result._22 = -_22;

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Transposes the matrix.
	//----------------------------------------------------------------------------------------------
	inline void Transpose(void) 
	{
		std::swap(_21, _12);
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the transposition of the matrix.
	/// \returns The transposed matrix
	//----------------------------------------------------------------------------------------------
	TMatrix2 TransposeCopy(void) const
	{
		TMatrix2 result;

		result._11 = _11;
		result._12 = _21;
		
		result._21 = _12;
		result._22 = _22;

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the inverse of the matrix.
	//----------------------------------------------------------------------------------------------
	void Invert(void)
	{
		TReal tDeterminant = (TReal)1.0 / Determinant();
		
		TReal t11 =  tDeterminant * _22,
			  t12 = -tDeterminant * _12, 
			  t21 = -tDeterminant * _21, 
			  t22 =  tDeterminant * _11;

		_11 = t11; 
		_12 = t12;
		_21 = t21; 
		_22 = t22;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the inverse of the matrix, if the matrix is not singular.
	/// \returns The inverse matrix
	//----------------------------------------------------------------------------------------------
	TMatrix2 InvertCopy(void) const
	{
		TMatrix2 inverse;
		TReal tDeterminant = (TReal)1.0 / Determinant();

		inverse._11 =  tDeterminant * _22;
		inverse._12 = -tDeterminant * _12;
		inverse._21 = -tDeterminant * _21;
		inverse._22 =  tDeterminant * _11;

		return inverse;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the inverse of the matrix; if the matrix has no inverse an exception is thrown.
	//----------------------------------------------------------------------------------------------
	void Invert_Safe(void)
	{
		TReal tDeterminant = Determinant();

		if ( IsSingular() )
			throw new MesonException("Matrix inverse is undefined", __FILE__, __LINE__);

		tDeterminant = (TReal)1.0 / tDeterminant;

		TReal t11 =  tDeterminant * _22,
			  t12 = -tDeterminant * _12, 
			  t21 = -tDeterminant * _21, 
			  t22 =  tDeterminant * _11;

		_11 = t11; 
		_12 = t12;
		_21 = t21; 
		_22 = t22;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the inverse matrix, if the matrix is not singular. If the matrix is singular,
	/// an exception is thrown.
	/// \returns The inverse matrix
	//----------------------------------------------------------------------------------------------
	TMatrix2 InvertCopy_Safe(void) const
	{
		TMatrix2 inverse;
		TReal tDeterminant = Determinant();

		if ( IsSingular() )
			throw new MesonException("Matrix inverse is undefined", __FILE__, __LINE__);

		tDeterminant = (TReal)1.0 / tDeterminant;

		inverse._11 =  tDeterminant * _22;
		inverse._12 = -tDeterminant * _12;
		inverse._21 = -tDeterminant * _21;
		inverse._22 =  tDeterminant * _11;

		return inverse;
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
	TMatrix2 InvertOrthonormalCopy(void) const
	{
		return TransposeCopy();
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the addition of two matrices.
	//----------------------------------------------------------------------------------------------
	TMatrix2 operator+ (const TMatrix2& p_matrix) const
	{
		TMatrix2 result;

		for (int n=0; n<Order; n++)
			result.Element[n] = Element[n] + p_matrix.Element[n];

		return result;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Computes the difference between two matrices.
	//----------------------------------------------------------------------------------------------
	TMatrix2 operator- (const TMatrix2& p_matrix) const
	{
		TMatrix2 result;

		for (int n=0; n<Order; n++)
			result.Element[n] = Element[n] - p_matrix.Element[n];

		return result;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Computes the multiplication of a matrix with a scalar.
	//----------------------------------------------------------------------------------------------
	TMatrix2 operator* (TReal p_tValue) const
	{
		TMatrix2 result;

		for (int n=0; n<Order; n++)
			result.Element[n] = Element[n] * p_tValue;

		return result;
	}

	friend TMatrix2 operator* (TReal p_tValue, const TMatrix2& p_matrix)
	{
		TMatrix2 result;

		for (int n=0; n<Order; n++)
			result.Element[n] = p_matrix.Element[n] * p_tValue;

		return result;
	}

	//----------------------------------------------------------------------------------------------
	/// Computes the division of a matrix by a scalar.
	//----------------------------------------------------------------------------------------------
	TMatrix2 operator/ (TReal p_tValue) const
	{
		TMatrix2 result;

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
	TMatrix2& operator+= (const TMatrix2& p_matrix)
	{
		for (int n=0; n<Order; n++)
			Element[n] += p_matrix.Element[n];

		return *this;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Computes the difference of two matrices and stores the result in the matrix.
	//----------------------------------------------------------------------------------------------
	TMatrix2& operator-= (const TMatrix2& p_matrix)
	{
		for (int n=0; n<Order; n++)
			Element[n] -= p_matrix.Element[n];

		return *this;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Multiplies the matrix by a scaling factor.
	//----------------------------------------------------------------------------------------------
	TMatrix2& operator*= (TReal p_tValue)
	{
		for (int n=0; n<Order; n++)
			Element[n] *= p_tValue;

		return *this;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Divides the matrix by a scaling factor.
	//----------------------------------------------------------------------------------------------
	TMatrix2& operator/= (TReal p_tValue)
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
		return TVector2<TReal>( _11 * p_vector.X + _12 * p_vector.Y, _21 * p_vector.X + _22 * p_vector.Y );
	}
	
	//----------------------------------------------------------------------------------------------
	/// Performs multiplication between two matrices.
	//----------------------------------------------------------------------------------------------
	TMatrix2 operator* (const TMatrix2& p_matrix) const
	{
		TMatrix2 result;

		result._11 = _11 * p_matrix._11 + _12 * p_matrix._21;
		result._12 = _11 * p_matrix._12 + _12 * p_matrix._22;
		
		result._21 = _21 * p_matrix._11 + _22 * p_matrix._21;
		result._22 = _21 * p_matrix._12 + _22 * p_matrix._22;

		return result;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Copies the matrix through assignment.
	//----------------------------------------------------------------------------------------------
	TMatrix2& operator= (const TMatrix2& p_matrix)
	{
		_11 = p_matrix._11;
		_12 = p_matrix._12;
		_21 = p_matrix._21;
		_22 = p_matrix._22;

		return *this;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Negates the matrix.
	//----------------------------------------------------------------------------------------------
	TMatrix2 operator- (void) const
	{
		return NegateCopy();
	}

	
	//----------------------------------------------------------------------------------------------
	/// Transposes the matrix.
	//----------------------------------------------------------------------------------------------
	TMatrix2 operator~(void) const
	{
		return TransposeCopy();
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	bool operator== (const TMatrix2& p_matrix) const
	{
		for (int nIdx = 0; nIdx < Order; nIdx++)
			if ( !TMaths<TReal>::Equals( Element[nIdx], p_matrix.Element[nIdx] ) )
				return false;

		return true;
	}
	
	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	bool operator!= (const TMatrix2& p_matrix) const
	{
		for (int nIdx = 0; nIdx < Order; nIdx++)
			if ( !TMaths<TReal>::Equals( Element[nIdx], p_matrix.Element[nIdx] ) )
				return true;

		return false;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the matrix array list under the cast operator.
	//----------------------------------------------------------------------------------------------
	operator TReal*() 
	{
		return (TReal*)&Element;
	}

	//----------------------------------------------------------------------------------------------
private:
	inline int Index( const int p_nRow, const int p_nColumn ) 
	{
		MESON_ASSERT( p_nRow >= 0 && p_nRow < Rows && p_nColumn >= 0 && p_nColumn < Columns, "Matrix indices out of range!" );
		return p_nRow * Columns + p_nColumn;
	}
};

template <typename TReal> TMatrix2<TReal> const TMatrix2<TReal>::Identity	= TMatrix2<TReal>(true);
template <typename TReal> TMatrix2<TReal> const TMatrix2<TReal>::Zero		= TMatrix2<TReal>(false);

typedef TMatrix2<double> Matrix2;
typedef TMatrix2<float> Matrix2f;

Meson_Common_Maths_END
