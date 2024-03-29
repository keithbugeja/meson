
#include "GrvTransform.h"

using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

Transform::Transform(void)
	: Rotation(TQuaternion<Real>::Identity)
	, Translation(TVector3<Real>::Zero)
{
}

Transform::Transform(const Transform& p_transform)
	: Rotation(p_transform.Rotation)
	, Translation(p_transform.Translation)
{
}

Transform::Transform(
	const TQuaternion<Real>& p_rotation,
	const TVector3<Real>& p_translation)
	: Rotation(p_rotation)
	, Translation(p_translation)
{
}

Transform::~Transform(void)
{
}

void Transform::Compose(const Transform& p_transform)
{
	// T2(T1(X)) = (Q2 Q1) X (Q2 Q1)* + (Q2 V1 Q2* + V2)
	// i.e. Q12 = Q2 Q1,   V12 = Q2 V1 Q2* + V2
	TQuaternion<Real> qtnRotationNew = p_transform.Rotation * Rotation;
	Translation = p_transform.Rotation.TransformCopy(Translation)
		+ p_transform.Translation;
	Rotation = qtnRotationNew;
}

Transform Transform::ComposeCopy(const Transform& p_transform) const
{
	// T2(T1(X)) = (Q2 Q1) X (Q2 Q1)* + (Q2 V1 Q2* + V2)
	// i.e. Q12 = Q2 Q1,   V12 = Q2 V1 Q2* + V2
	return Transform(
		p_transform.Rotation * Rotation,
		p_transform.Rotation.TransformCopy(Translation) + p_transform.Translation);
}

void Transform::Invert(void)
{
	// if   X' = Q X Q* + V
	// then X = Q* X' Q - (Q* V Q)
	Rotation.Conjugate();
	Translation = -(Rotation.TransformCopy(Translation));
}

Transform Transform::InvertCopy(void) const
{
	// if   X' = Q X Q* + V
	// then X = Q* X' Q - (Q* V Q)
	TQuaternion<Real> qtnConjugate = Rotation.ConjugateCopy();
	return Transform(qtnConjugate, -qtnConjugate.TransformCopy(Translation));
}

void Transform::Apply(TVector3<Real>& p_vecPoint) const
{
	p_vecPoint = Rotation.TransformCopy(p_vecPoint) + Translation;
}

void Transform::Apply(VectorList& p_vectorList) const
{
	for (size_t unIndex = 0; unIndex < p_vectorList.Size(); unIndex++)
		Apply(p_vectorList[unIndex]);
}

TVector3<Real> Transform::ApplyCopy(const TVector3<Real>& p_vecPoint) const
{
	return Rotation.TransformCopy(p_vecPoint) + Translation;
}

void Transform::ApplyCopy(const VectorList& p_vectorList, VectorList& p_vectorListTransformed) const
{
	p_vectorListTransformed.Clear();
	for (size_t unIndex = 0; unIndex < p_vectorList.Size(); unIndex++)
		p_vectorListTransformed.Add(ApplyCopy(p_vectorList[unIndex]));
}

void Transform::ChangeBasis(const Transform& p_trnBasis)
{
	TQuaternion<Real> qtnConjugate = p_trnBasis.Rotation.ConjugateCopy();

	Rotation = qtnConjugate * Rotation;
	Translation = qtnConjugate.TransformCopy(
		Translation - p_trnBasis.Translation);
}

Transform Transform::ChangeBasisCopy(const Transform& p_trnBasis) const
{
	TQuaternion<Real> qtnConjugate = p_trnBasis.Rotation.ConjugateCopy();

	return Transform(qtnConjugate * Rotation,
		qtnConjugate.TransformCopy(Translation - p_trnBasis.Translation));
}

void Transform::ExtractMatrix( TMatrix4<Real> &p_matrix ) const
{
	Rotation.ExtractMatrix(p_matrix);

	p_matrix._14 = Translation.X;
	p_matrix._24 = Translation.Y;
	p_matrix._34 = Translation.Z;
}

bool Transform::operator==(const Transform& p_transform) const
{
	if (Translation != p_transform.Translation) return false;
	if (Rotation != p_transform.Rotation) return false;
	return true;
}

bool Transform::operator!=(const Transform& p_transform) const
{
	if (Translation != p_transform.Translation) return true;
	if (Rotation != p_transform.Rotation) return true;
	return false;
}

Transform Transform::operator*(const Transform& p_transform) const
{
	return ComposeCopy(p_transform);
}

TVector3<Real> Transform::operator()(const TVector3<Real>& p_vecPoint) const
{
	return ApplyCopy(p_vecPoint);
}
