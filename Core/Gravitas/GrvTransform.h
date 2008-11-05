#pragma once

#include "Quaternion.h"
#include "Vector3.h"
#include "Matrix4.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"

Meson_Gravitas_Geometry_BEGIN

//----------------------------------------------------------------------------------------------
/// \struct Transform
/// The transform object defines a rotation followed by a translation. The rotation and
/// translation components of the transform are defined, respectively, by a unit quaternion and
/// a vector. The transform may be composisted with other transforms, inverted, or transformed
/// to a different basis.
//----------------------------------------------------------------------------------------------
struct GRAVITAS_API Transform
{
	//----------------------------------------------------------------------------------------------
	/// Unit quaternion for the rotation component of the transform.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TQuaternion<Real> Rotation;

	//----------------------------------------------------------------------------------------------
	/// Vector for the translation component of the transform.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Translation;

	//----------------------------------------------------------------------------------------------
	/// Constructs an identity transform.
	//----------------------------------------------------------------------------------------------
	Transform(void);

	//----------------------------------------------------------------------------------------------
	/// Constructs a transform using the given source transform.
	/// \param p_transform Source transform to copy.
	//----------------------------------------------------------------------------------------------
	Transform(const Transform& p_transform);

	//----------------------------------------------------------------------------------------------
	/// Constructs a transform using the given rotation quaternion and translation vector.
	/// \param p_rotation Unit quaternion for the rotation component.
	/// \param p_translation Vector for the translation component.
	//----------------------------------------------------------------------------------------------
	Transform::Transform(
		const Meson::Common::Maths::TQuaternion<Real>& p_rotation,
		const Meson::Common::Maths::TVector3<Real>& p_translation);

	//----------------------------------------------------------------------------------------------
	/// Destroys the transform.
	//----------------------------------------------------------------------------------------------
	~Transform(void);

	//----------------------------------------------------------------------------------------------
	/// Composes this transform with the given transform. The order of the composition is such that
	/// this transform in original form is applied first.
	/// \param p_transform Transform to compose with this transform.
	//----------------------------------------------------------------------------------------------
	void Compose(const Transform& p_transform);

	//----------------------------------------------------------------------------------------------
	/// Returns a Composes this transform with the given transform. The order of the composition is such that
	/// this transform in original form is applied first.
	/// \param p_transform Transform to compose with this transform.
	//----------------------------------------------------------------------------------------------
	Transform ComposeCopy(const Transform& p_transform) const;

	//----------------------------------------------------------------------------------------------
	/// Inverts the transform.
	//----------------------------------------------------------------------------------------------
	void Invert(void);

	//----------------------------------------------------------------------------------------------
	/// Returns an inverse the transform.
	/// \returns an inverse the transform.
	//----------------------------------------------------------------------------------------------
	Transform InvertCopy(void) const;

	//----------------------------------------------------------------------------------------------
	/// Applies the transform on the given point.
	/// \param p_vecPoint Point to transform.
	//----------------------------------------------------------------------------------------------
	void Apply(Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Applies the transform on the given vector list.
	/// \param p_vectorList Abstract list of vectors on which to apply the transform.
	//----------------------------------------------------------------------------------------------
	void Apply(VectorList& p_vectorList) const;

	//----------------------------------------------------------------------------------------------
	/// Returns a transformed copy of the given point.
	/// \param p_vecPoint Point to transform.
	/// \returns a transformed copy of the given point.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> ApplyCopy(
		const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Transforms the given point list and stores it into the given destination list.
	/// \param p_pointList List of points to transform.
	/// \param p_pointListTransformed Destination list of transformed points.
	//----------------------------------------------------------------------------------------------
	void ApplyCopy(
		const VectorList& p_vectorList, VectorList& p_vectorListTransformed) const;

	//----------------------------------------------------------------------------------------------
	/// Changes the transform with respect to the given basis transform.
	/// \param p_trnBasis New basis given as a transform.
	//----------------------------------------------------------------------------------------------
	void ChangeBasis(const Transform& p_trnBasis);

	//----------------------------------------------------------------------------------------------
	/// Returns a copy of the transform witn respect to the given basis transform.
	/// \param p_trnBasis New basis given as a transform.
	/// \returns a copy of the transform witn respect to the given basis transform.
	//----------------------------------------------------------------------------------------------
	Transform ChangeBasisCopy(const Transform& p_trnBasis) const;

	//----------------------------------------------------------------------------------------------
	/// Extracts the transform into the given 4x4 matrix.
	/// \param p_matrix 4x4 matrix in which to store the transform.
	//----------------------------------------------------------------------------------------------
	void ExtractMatrix( Meson::Common::Maths::TMatrix4<Real> &p_matrix ) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given transform is equal to this transform. The transform must match
	/// component-wise.
	/// \param p_transform Transform to compare.
	/// \returns true if the given transform is equal to this transform.
	//----------------------------------------------------------------------------------------------
	bool operator==(const Transform& p_transform) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given transform is not equal to this transform. The comparison is
	/// performed component-wise.
	/// \param p_transform Transform to compare.
	/// \returns true if the given transform is not equal to this transform.
	//----------------------------------------------------------------------------------------------
	bool operator!=(const Transform& p_transform) const;

	//----------------------------------------------------------------------------------------------
	/// Returns a composition of this transform with the given transform.
	/// \param p_transform Outer transform to composite with this transform.
	/// \returns a composition of this transform with the given transform.
	//----------------------------------------------------------------------------------------------
	Transform operator*(const Transform& p_transform) const;

	//----------------------------------------------------------------------------------------------
	/// Applies the transform as a functor on the given point.
	/// \param p_ptPoint Point to transform.
	/// \returns a transformed copy of the given point.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> operator()(
		const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;
};

Meson_Gravitas_Geometry_END
