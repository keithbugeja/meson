//----------------------------------------------------------------------------------------------
//	Meson::Common::Maths::TQuaternion<T>
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/01/2008	1.0.0		Description
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "DebugSupport.h"
#include "Maths.h"

#include "Matrix4.h"
#include "Matrix3.h"
#include "Vector3.h"

Meson_Common_Maths_BEGIN

template <typename TReal> class TQuaternion
{
public:
	//----------------------------------------------------------------------------------------------
	/// Constructs an uninitialised quaternion.
	//----------------------------------------------------------------------------------------------
	TQuaternion() { }
	
	//----------------------------------------------------------------------------------------------
	/// Constructs an identity or a zero quaternion.
	/// \param p_bIdentity If true, quaternion is initialised to identity, otherwise to zero
	//----------------------------------------------------------------------------------------------
	TQuaternion(bool p_bIdentity) 
	{
		X = Y = Z = (TReal)0.0;
		W = p_bIdentity ? (TReal)1.0 : (TReal)0.0;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Quaternion copy constructor.
	/// \param p_quaternion Quaternion to copy
	//----------------------------------------------------------------------------------------------
	TQuaternion(const TQuaternion& p_quaternion) : W(p_quaternion.W), X(p_quaternion.X), Y(p_quaternion.Y), Z(p_quaternion.Z) { }
	
	//----------------------------------------------------------------------------------------------
	/// Constructs a quaternion from a vector and a scalar value.
	/// \param p_vector Vector used to set imaginary components (X, Y, Z)
	/// \param p_tScalar Value used to set real component (W) 
	//----------------------------------------------------------------------------------------------
	TQuaternion(TReal p_tScalar, const TVector3<TReal>& p_vector) : W(p_tScalar), Vector(p_vector) { }

	//----------------------------------------------------------------------------------------------
	/// Constructs a quaternion from a vector.
	/// \param p_vector Vector used to set imaginary components (X, Y, Z)
	//----------------------------------------------------------------------------------------------
	TQuaternion(const TVector3<TReal>& p_vector) : W((TReal)0.0), Vector(p_vector) { }

	//----------------------------------------------------------------------------------------------
	/// Constructs a quaternion from scalar components, W, X, Y, Z.
	/// \param p_tW Real component
	/// \param p_tX Imaginary component
	/// \param p_tY Imaginary component
	/// \param p_tZ Imaginary component
	//----------------------------------------------------------------------------------------------
	TQuaternion(TReal p_tW, TReal p_tX, TReal p_tY, TReal p_tZ) : W(p_tW), X(p_tX), Y(p_tY), Z(p_tZ) { }
	
	//----------------------------------------------------------------------------------------------
	/// Constructs a quaternion from a 4x4 matrix.
	/// \param p_matrix 4x4 Matrix
	//----------------------------------------------------------------------------------------------
	TQuaternion(const TMatrix4<TReal>& p_matrix) 
	{
		MakeRotation( p_matrix );
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	bool IsZero(void) const
	{
		return ( TMaths<TReal>::Equals( W, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( X, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( Y, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( Z, (TReal)0.0 ) );
	}
	
	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	bool IsIdentity(void) const
	{
		return ( TMaths<TReal>::Equals( W, (TReal)1.0 ) &&
				 TMaths<TReal>::Equals( X, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( Y, (TReal)0.0 ) &&
				 TMaths<TReal>::Equals( Z, (TReal)0.0 ) );
	}

	//----------------------------------------------------------------------------------------------
	/// Initialises the quaternion to a zero quaternion.
	//----------------------------------------------------------------------------------------------
	void MakeZero(void) 
	{
		W = X = Y = Z = (TReal)0.0;
	}

	//----------------------------------------------------------------------------------------------
	/// Initialises the quaternion to an identity quaternion.
	//----------------------------------------------------------------------------------------------
	void MakeIdentity(void) 
	{ 
		W = (TReal)1.0;
		X = Y = Z = (TReal)0.0;
	}

	//----------------------------------------------------------------------------------------------
	/// Composes a rotation quaternion using an arbitrary axis and angle.
	/// \param p_vecAxis Arbitrary axis for rotation
	/// \param p_tAngle Angle of rotation about axis
	//----------------------------------------------------------------------------------------------
	void MakeRotation(const TVector3<TReal>& p_vecAxis, TReal p_tAngle)
	{
		TReal tHalfAngle = p_tAngle * 0.5,
			  tSin = TMaths<TReal>::Sin(tHalfAngle),
			  tCos = TMaths<TReal>::Cos(tHalfAngle);

		W = tCos;
		X = p_vecAxis.X * tSin;
		Y = p_vecAxis.Y * tSin;
		Z = p_vecAxis.Z * tSin;
	}

	//----------------------------------------------------------------------------------------------
	/// Converts the matrix to a rotation quaternion.
	/// \param p_matrix 3x3 Rotation matrix
	//----------------------------------------------------------------------------------------------
	void MakeRotation(const TMatrix3<TReal>& p_matrix)
	{
		/*
		// Based on the algorithm found in Ken Shoemake's paper:
		// Animating rotation with quaternion curves, 1985 

		const TReal rThird = (TReal) 1.0 / (TReal) 3.0;

		TReal tWSqr = rThird * (p_matrix._11 + p_matrix._22 + p_matrix._33);

		if ( tWSqr > TMaths<TReal>::Epsilon )
		{
			W = TMaths<TReal>::Sqrt( tWSqr );

			// W' = (3W)^-1
			TReal tInv3W = (rThird / W);

			X = (p_matrix._32 - p_matrix._23) * tInv3W;
			Y = (p_matrix._13 - p_matrix._31) * tInv3W;
			Z = (p_matrix._21 - p_matrix._12) * tInv3W;
		}
		else
		{
			W = (TReal)0.0;

			// X^2 = -1/2 * (M22 + M23)
			TReal tXSqr = (TReal)-0.5 * (p_matrix._22 + p_matrix._33);

			if ( tXSqr > TMaths<TReal>::Epsilon )
			{
				X = TMaths<TReal>::Sqrt( tXSqr );

				TReal tInv2X = (TReal)0.5 / X;

				Y = p_matrix._21 * tInv2X;
				Z = p_matrix._31 * tInv2X;
			}
			else
			{
				X = (TReal)0.0;

				// Y^2 = 1/2 * (1 - M33)
				TReal tYSqr = (TReal)0.5 * (1 - p_matrix._33);
				
				if ( tYSqr > TMaths<TReal>::Epsilon )
				{
					Y = TMaths<TReal>::Sqrt( tYSqr );
					Z = p_matrix._32 / (Y*2);
				}
				else
				{
					Y = (TReal)0.0;
					Z = (TReal)1.0;
				}
			}
		}
		*/

		W = (TReal) 0.5 * TMaths<TReal>::Sqrt((TReal) 1.0 + p_matrix._11 + p_matrix._22 + p_matrix._33);
		TReal r4WInv = (TReal) 0.25 / W;
		X = r4WInv * (p_matrix._32 - p_matrix._23);
		Y = r4WInv * (p_matrix._13 - p_matrix._31);
		Z = r4WInv * (p_matrix._21 - p_matrix._12);
	}

	//----------------------------------------------------------------------------------------------
	/// Converts the matrix to a rotation quaternion.
	/// \param p_matrix 4x4 Rotation matrix
	//----------------------------------------------------------------------------------------------
	void MakeRotation(const TMatrix4<TReal>& p_matrix)
	{
		// Based on the algorithm found in Ken Shoemake's paper:
		// Animating rotation with quaternion curves, 1985 
		TReal tWSqr = (TReal)0.25 * (p_matrix._11 + p_matrix._22 + p_matrix._33 + p_matrix._44);

		if ( tWSqr > TMaths<TReal>::Epsilon )
		{
			W = TMaths<TReal>::Sqrt( tWSqr );

			// W' = (4W)^-1
			TReal tInv4W = ((TReal)0.25 / W);

			X = (p_matrix._32 - p_matrix._23) * tInv4W;
			Y = (p_matrix._13 - p_matrix._31) * tInv4W;
			Z = (p_matrix._21 - p_matrix._12) * tInv4W;
		}
		else
		{
			W = (TReal)0.0;

			// X^2 = -1/2 * (M22 + M23)
			TReal tXSqr = (TReal)-0.5 * (p_matrix._22 + p_matrix._33);

			if ( tXSqr > TMaths<TReal>::Epsilon )
			{
				X = TMaths<TReal>::Sqrt( tXSqr );

				TReal tInv2X = (TReal)0.5 / X;

				Y = p_matrix._21 * tInv2X;
				Z = p_matrix._31 * tInv2X;
			}
			else
			{
				X = (TReal)0.0;

				// Y^2 = 1/2 * (1 - M33)
				TReal tYSqr = (TReal)0.5 * (1 - p_matrix._33);
				
				if ( tYSqr > TMaths<TReal>::Epsilon )
				{
					Y = TMaths<TReal>::Sqrt( tYSqr );
					Z = p_matrix._32 / (Y*2);
				}
				else
				{
					Y = (TReal)0.0;
					Z = (TReal)1.0;
				}
			}
		}
	}

	//----------------------------------------------------------------------------------------------
	/// TODO:
	//----------------------------------------------------------------------------------------------
	void MakeReflection(void) { }

	//----------------------------------------------------------------------------------------------
	/// Returns the length (2-norm) of the quaternion.
	/// \returns Length
	//----------------------------------------------------------------------------------------------
	TReal Length(void) const 
	{
		return TMaths<TReal>::Sqrt(W*W + X*X + Y*Y + Z*Z);
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the square of the length of the quaternion.
	/// \returns Length squared
	//----------------------------------------------------------------------------------------------
	TReal LengthSquared(void) const
	{
		return W*W + X*X + Y*Y + Z*Z;
	}
		
	//----------------------------------------------------------------------------------------------
	/// Sets the quaternion to its conjugate.
	//----------------------------------------------------------------------------------------------
	void Conjugate(void) 
	{
		X=-X; 
		Y=-Y; 
		Z=-Z;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the conjugate of the quaternion.
	/// \returns Conjugate
	//----------------------------------------------------------------------------------------------
	TQuaternion ConjugateCopy(void) const 
	{
		return TQuaternion( W, -X, -Y, -Z );
	}

	//----------------------------------------------------------------------------------------------
	/// Negates the quaternion.
	//----------------------------------------------------------------------------------------------
	void Negate(void) 
	{
		 W=-W; 
		 X=-X; 
		 Y=-Y; 
		 Z=-Z;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the negation of the quaternion.
	/// \returns Negation
	//----------------------------------------------------------------------------------------------
	TQuaternion NegateCopy(void) const
	{
		return TQuaternion( -W, -X, -Y, -Z );
	}

	//----------------------------------------------------------------------------------------------
	/// Sets the quaternion to its inverse.
	//----------------------------------------------------------------------------------------------
	void Invert(void) 
	{
		TReal tNormSqr = LengthSquared();
		MESON_ASSERT (tNormSqr > TMaths<TReal>::Epsilon, "Quaternion length is zero - cannot invert!");

		if ( tNormSqr > TMaths<TReal>::Epsilon )
		{
			tNormSqr = (TReal)1.0 / tNormSqr;
			W *=  tNormSqr; X *= -tNormSqr; Y *= -tNormSqr; Z *= -tNormSqr;
		}
		else
			W = X = Y = Z = TMaths<TReal>::Maximum;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the inverse of the quaternion.
	/// \returns Inverse
	//----------------------------------------------------------------------------------------------
	TQuaternion InvertCopy(void) const
	{
		TReal tNormSqr = LengthSquared();
		MESON_ASSERT (tNormSqr > TMaths<TReal>::Epsilon, "Quaternion length is zero - cannot invert!");

		if ( tNormSqr > TMaths<TReal>::Epsilon )
		{
			tNormSqr = (TReal)1.0 / tNormSqr;
			return TQuaternion(W * tNormSqr, -X * tNormSqr, -Y * tNormSqr, -Z * tNormSqr);
		}

		return TQuaternion( TMaths<TReal>::Maximum, TMaths<TReal>::Maximum, TMaths<TReal>::Maximum, TMaths<TReal>::Maximum );
	}

	//----------------------------------------------------------------------------------------------
	/// Normalises the quaternion.
	//----------------------------------------------------------------------------------------------
	void Normalise(void)
	{
		TReal tNorm = Length();
		MESON_ASSERT (tNorm > TMaths<TReal>::Epsilon, "Quaternion length is zero - cannot normalise!");

		if ( tNorm > TMaths<TReal>::Epsilon )
		{
			tNorm = (TReal)1.0 / tNorm;
			 W *= tNorm; X *= tNorm; Y *= tNorm; Z *= tNorm;
		}
		else
			W = X = Y = Z = TMaths<TReal>::Maximum;
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the normalised version of the quaternion.
	/// \returns Normal
	//----------------------------------------------------------------------------------------------
	TQuaternion NormaliseCopy(void) const
	{
		TReal tNorm = Length();
		MESON_ASSERT (tNorm > TMaths<TReal>::Epsilon, "Quaternion length is zero - cannot normalise!");
	
		if ( tNorm > TMaths<TReal>::Epsilon )
		{
			tNorm = (TReal)1.0 / tNorm;
			return TQuaternion(W * tNorm, X * tNorm, Y * tNorm, Z * tNorm);
		}

		return TQuaternion( TMaths<TReal>::Maximum, TMaths<TReal>::Maximum, TMaths<TReal>::Maximum, TMaths<TReal>::Maximum );
	}

	//----------------------------------------------------------------------------------------------
	/// Rotates a point about the axis and angle specified by the quaternion.
	/// \param p_qPoint Point to rotate in quaternion form
	/// \param p_qResult Quaternion used to store output result
	//----------------------------------------------------------------------------------------------
	void Rotate(const TQuaternion& p_qPoint, TQuaternion& p_qResult) const
	{
		TQuaternion &qRotation = *this,
					qRotationConj = ConjugateCopy();

		p_qResult = qRotation * p_qPoint * qRotationConj;
	}

	//----------------------------------------------------------------------------------------------
	/// Rotates the point according the the rotation quaternion.
	/// \param p_qRotation Quaternion representing rotation transform
	//----------------------------------------------------------------------------------------------
	void RotateBy(const TQuaternion& p_qRotation)
	{
		TQuaternion &qPoint = *this,
					qRotationConj = p_qRotation.ConjugateCopy();

		qPoint = p_qRotation * qPoint * qRotationConj;
	}

	//----------------------------------------------------------------------------------------------
	/// Rotates a point about the axis and angle specified by the quaternion.
	/// \param p_qPoint Point in quaternion form to rotate
	/// \returns Rotated quaternion
	//----------------------------------------------------------------------------------------------
	TQuaternion RotateCopy(const TQuaternion& p_qPoint) const
	{
		TQuaternion &qRotation = (TQuaternion &) *this,
					qRotationConj = ConjugateCopy();

		return qRotation * p_qPoint * qRotationConj;
	}

	//----------------------------------------------------------------------------------------------
	/// Reflects a point about the axis and angle specified by the quaternion.
	/// \param p_qPoint Point to reflect in quaternion form
	/// \param p_qResult Quaternion used to store output result
	//----------------------------------------------------------------------------------------------
	void Reflect(const TQuaternion& p_qPoint, TQuaternion& p_qResult) const
	{
		TQuaternion &qReflection = *this,
					qReflectionNeg = NegateCopy();

		qResult = qReflection * p_qPoint * qReflectionNeg;
	}
	
	//----------------------------------------------------------------------------------------------
	/// Reflects the point according the the reflection quaternion.
	/// \param p_qRotation Quaternion representing reflection transform
	//----------------------------------------------------------------------------------------------
	void ReflectBy(TQuaternion& p_qReflection)
	{
		TQuaternion &qPoint = *this,
					qReflectionNeg = p_qReflection.NegateCopy();

		qPoint = p_qReflection * qPoint * qReflectionNeg;
	}

	//----------------------------------------------------------------------------------------------
	/// Reflects a point about the axis and angle specified by the quaternion.
	/// \param p_qPoint Point to reflect in quaternion form
	/// \returns Reflected quaternion
	//----------------------------------------------------------------------------------------------
	TQuaternion ReflectCopy(const TQuaternion& p_qPoint) const
	{
		TQuaternion &qReflection = (TQuaternion &) *this,
					qReflectionNeg =  NegateCopy();

		return qReflection * p_qPoint * qReflectionNeg;
	}

	//----------------------------------------------------------------------------------------------
	// Animating Rotation with Quaternion Curves - Ken Shoemake, 85
	//----------------------------------------------------------------------------------------------
	TMatrix4<TReal> ExtractMatrix(void) const
	{
		TMatrix4<TReal> matrix;

		matrix._11 = 1 - 2*Y*Y - 2*Z*Z;
		matrix._12 = 2*X*Y - 2*Z*W;
		matrix._13 = 2*X*Z + 2*Y*W;
		
		matrix._21 = 2*X*Y + 2*Z*W;
		matrix._22 = 1 - 2*X*X - 2*Z*Z;
		matrix._23 = 2*Y*Z - 2*X*W;

		matrix._31 = 2*X*Z - 2*Y*W;
		matrix._32 = 2*Y*Z - 2*X*W;
		matrix._33 = 1 - 2*X*X - 2*Y*Y;
		
		matrix._41 = matrix._42 = matrix._43 = matrix._14 = matrix._24 = matrix._34 = (TReal)0.0;
		matrix._44 = (TReal)1.0;

		return matrix;
	}

	//----------------------------------------------------------------------------------------------
	// TODO: Consider adding an optimised version for order 3 matrix extraction into an order 4 matrix
	//----------------------------------------------------------------------------------------------
	void ExtractMatrix(TMatrix4<TReal>& p_mat4Out) const
	{
		p_mat4Out._11 = 1 - 2*Y*Y - 2*Z*Z;
		p_mat4Out._12 = 2*X*Y - 2*Z*W;
		p_mat4Out._13 = 2*X*Z + 2*Y*W;
		
		p_mat4Out._21 = 2*X*Y + 2*Z*W;
		p_mat4Out._22 = 1 - 2*X*X - 2*Z*Z;
		p_mat4Out._23 = 2*Y*Z - 2*X*W;

		p_mat4Out._31 = 2*X*Z - 2*Y*W;
		p_mat4Out._32 = 2*Y*Z + 2*X*W;
		p_mat4Out._33 = 1 - 2*X*X - 2*Y*Y;

		p_mat4Out._41 = p_mat4Out._42 = p_mat4Out._43 = p_mat4Out._14 = p_mat4Out._24 = p_mat4Out._34 = (TReal)0.0;
		p_mat4Out._44 = (TReal)1.0;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	void ExtractMatrix(TMatrix3<TReal>& p_matrix) const
	{
		p_matrix._11 = 1 - 2*Y*Y - 2*Z*Z;
		p_matrix._12 = 2*X*Y - 2*Z*W;
		p_matrix._13 = 2*X*Z + 2*Y*W;
		
		p_matrix._21 = 2*X*Y + 2*Z*W;
		p_matrix._22 = 1 - 2*X*X - 2*Z*Z;
		p_matrix._23 = 2*Y*Z - 2*X*W;

		p_matrix._31 = 2*X*Z - 2*Y*W;
		p_matrix._32 = 2*Y*Z + 2*X*W;
		p_matrix._33 = 1 - 2*X*X - 2*Y*Y;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	void Transform(TVector3<TReal>& p_vector) const
	{
		p_vector = (*this * TQuaternion<TReal>(p_vector) * ConjugateCopy()).Vector;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TVector3<TReal> TransformCopy(const TVector3<TReal>& p_vector) const
	{
		return (*this * TQuaternion<TReal>(p_vector) * ConjugateCopy()).Vector;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	void Transform(const TMatrix3<TReal>& p_matrix) const
	{
		TQuaternion<TReal> qtnConjugate = ConjugateCopy();
		p_matrix.SetRow(0, (*this * TQuaternion<TReal>(p_matrix.GetRow(0)) * qtnConjugate).Vector);
		p_matrix.SetRow(1, (*this * TQuaternion<TReal>(p_matrix.GetRow(1)) * qtnConjugate).Vector);
		p_matrix.SetRow(2, (*this * TQuaternion<TReal>(p_matrix.GetRow(2)) * qtnConjugate).Vector);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TMatrix3<TReal> TransformCopy(const TMatrix3<TReal>& p_matrix) const
	{
		TQuaternion<TReal> qtnConjugate = ConjugateCopy();
		TMatrix3<TReal> matResult;
		matResult.SetRow(0, (*this * TQuaternion<TReal>(p_matrix.GetRow(0)) * qtnConjugate).Vector);
		matResult.SetRow(1, (*this * TQuaternion<TReal>(p_matrix.GetRow(1)) * qtnConjugate).Vector);
		matResult.SetRow(2, (*this * TQuaternion<TReal>(p_matrix.GetRow(2)) * qtnConjugate).Vector);
		return matResult;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TQuaternion operator- (void) const
	{
		return NegateCopy();
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TQuaternion operator~ (void) const
	{
		return ConjugateCopy();
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TQuaternion operator- (const TQuaternion& p_quaternion) const
	{
		return TQuaternion(W - p_quaternion.W, X - p_quaternion.X, Y - p_quaternion.Y, Z - p_quaternion.Z);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TQuaternion operator+ (const TQuaternion& p_quaternion) const
	{
		return TQuaternion(W + p_quaternion.W, X + p_quaternion.X, Y + p_quaternion.Y, Z + p_quaternion.Z);
	}
	
	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TQuaternion operator* (TReal p_tScale) const
	{
		return TQuaternion(W * p_tScale, X * p_tScale, Y * p_tScale, Z * p_tScale);
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TQuaternion operator* (const TQuaternion& p_quaternion) const
	{
		return TQuaternion( W * p_quaternion.W - X * p_quaternion.X - Y * p_quaternion.Y - Z * p_quaternion.Z,
							X * p_quaternion.W + W * p_quaternion.X + Y * p_quaternion.Z - Z * p_quaternion.Y,
							W * p_quaternion.Y - X * p_quaternion.Z + Y * p_quaternion.W + Z * p_quaternion.X,
							W * p_quaternion.Z + X * p_quaternion.Y - Y * p_quaternion.X + Z * p_quaternion.W );
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TQuaternion& operator-= (const TQuaternion& p_quaternion) 
	{
		W-=p_quaternion.W;
		X-=p_quaternion.X;
		Y-=p_quaternion.Y;
		Z-=p_quaternion.Z;

		return *this;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TQuaternion& operator+= (const TQuaternion& p_quaternion) 
	{
		W+=p_quaternion.W;
		X+=p_quaternion.X;
		Y+=p_quaternion.Y;
		Z+=p_quaternion.Z;

		return *this;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TQuaternion& operator*= (TReal p_tScale) 
	{
		W*=p_tScale;
		X*=p_tScale;
		Y*=p_tScale;
		Z*=p_tScale;

		return *this;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TQuaternion& operator*= (const TQuaternion& p_quaternion) 
	{
		TReal tW, tX, tY, tZ;

		tW = X * p_quaternion.W + Y * p_quaternion.Z - Z * p_quaternion.Y + W * p_quaternion.X;
		tX = X * p_quaternion.X - Y * p_quaternion.Y - Z * p_quaternion.Z - W * p_quaternion.W;
		tY = X * p_quaternion.Y + Y * p_quaternion.X + Z * p_quaternion.W - W * p_quaternion.Z;
		tZ = X * p_quaternion.Z - Y * p_quaternion.W + Z * p_quaternion.X - W * p_quaternion.Y;

		W = tW; X = tX; Y = tY; X = tZ; 

		return *this;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TQuaternion& operator= (TReal p_tScalar) 
	{
		W = X = Y = Z = p_tScalar;

		return *this;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	TQuaternion& operator= (const TQuaternion& p_quaternion) 
	{
		W = p_quaternion.W;
		X = p_quaternion.X;
		Y = p_quaternion.Y;
		Z = p_quaternion.Z;

		return *this;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	bool operator== (const TQuaternion& p_quaternion) const
	{
		return (TMaths<TReal>::Equals( p_quaternion.W, W ) &&
				TMaths<TReal>::Equals( p_quaternion.X, X ) &&
				TMaths<TReal>::Equals( p_quaternion.Y, Y ) &&
				TMaths<TReal>::Equals( p_quaternion.Z, Z ) );
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	bool operator!= (const TQuaternion& p_quaternion) const
	{
		return (!TMaths<TReal>::Equals( p_quaternion.W, W ) ||
				!TMaths<TReal>::Equals( p_quaternion.X, X ) ||
				!TMaths<TReal>::Equals( p_quaternion.Y, Y ) ||
				!TMaths<TReal>::Equals( p_quaternion.Z, Z ) );
	}

	//----------------------------------------------------------------------------------------------
	/// Returns the quaternion array list under the cast operator.
	//----------------------------------------------------------------------------------------------
	operator TReal*() 
	{
		return (TReal*)&Element;
	}

	template <typename TRealParam>
	operator TQuaternion<TRealParam>() const
	{
		return TQuaternion<TRealParam>((TRealParam)W, (TRealParam)X, (TRealParam)Y, (TRealParam)Z);
	}

public:
	//----------------------------------------------------------------------------------------------
	union 
	{
		TReal Element[4];

		//----------------------------------------------------------------------------------------------
		struct
		{
			TReal W;

			union 
			{
				struct { TReal X, Y, Z; };
				struct { TVector3<TReal> Vector; };
			};
		};
	};

	//----------------------------------------------------------------------------------------------
	static const TQuaternion Zero;
	static const TQuaternion Identity;

	//----------------------------------------------------------------------------------------------
};

template <typename TReal> TQuaternion<TReal> const TQuaternion<TReal>::Zero = TQuaternion<TReal>(false);
template <typename TReal> TQuaternion<TReal> const TQuaternion<TReal>::Identity = TQuaternion<TReal>(true);

typedef TQuaternion<double> Quaternion;
typedef TQuaternion<float>  Quaternionf;

Meson_Common_Maths_END
