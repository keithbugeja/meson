//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstTransformation.h"
#include "Quaternion.h"

using namespace Meson::Common::Maths;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
const Transformation Transformation::Identity = Transformation();

//----------------------------------------------------------------------------------------------
Transformation::Transformation(void)
	: m_qtnRotation(true),
	  m_mat4Transform(true),
	  m_mat3Rotation(true),
	  m_vec3Translation(0.0f),
	  m_vec3Scaling(1.0f)
{
	m_bIsIdentity = true;
		
	m_bHasScalingComponent = 
		m_bHasRotationComponent = 
		m_bHasTranslationComponent = false;

	m_bIsTDirty = 
		m_bIsRSDirty = true;
}
//----------------------------------------------------------------------------------------------
Transformation::Transformation(const Transformation& p_transformation)
	: m_bIsRSDirty(p_transformation.m_bIsRSDirty),
	  m_bIsTDirty(p_transformation.m_bIsTDirty),
	  m_bIsIdentity(p_transformation.m_bIsIdentity),
	  m_bHasScalingComponent(p_transformation.m_bHasScalingComponent),
	  m_bHasRotationComponent(p_transformation.m_bHasRotationComponent),
	  m_bHasTranslationComponent(p_transformation.m_bHasTranslationComponent),
	  m_mat4Transform(p_transformation.m_mat4Transform),
	  m_mat3Rotation(p_transformation.m_mat3Rotation),
	  m_qtnRotation(p_transformation.m_qtnRotation),
	  m_vec3Scaling(p_transformation.m_vec3Scaling),
	  m_vec3Translation(p_transformation.m_vec3Translation)
{
}
//----------------------------------------------------------------------------------------------
Transformation::Transformation(const Meson::Common::Maths::Matrix3f& p_mat3Rotation, 
							   const Meson::Common::Maths::Vector3f& p_vec3Translation, 
							   const Meson::Common::Maths::Vector3f& p_vec3Scaling)
	: m_mat3Rotation(p_mat3Rotation),
	  m_vec3Translation(p_vec3Translation),
	  m_vec3Scaling(p_vec3Scaling)
{
	m_bIsIdentity = false;

	m_bHasRotationComponent = 
		m_bHasScalingComponent = 
		m_bHasTranslationComponent = true;

	m_bIsRSDirty =
		m_bIsTDirty = true;

	// Clear projection components
	m_mat4Transform._41 = m_mat4Transform._42 = m_mat4Transform._43 = 0.0f;
	m_mat4Transform._44 = 1.0f;
}
//----------------------------------------------------------------------------------------------
Transformation::Transformation(const Meson::Common::Maths::Matrix3f& p_mat3Rotation, 
							   const Meson::Common::Maths::Vector3f& p_vec3Translation,
							   float p_fScaling)
	: m_mat3Rotation(p_mat3Rotation),
	  m_vec3Translation(p_vec3Translation),
	  m_vec3Scaling(p_fScaling)
{
	m_bIsIdentity = false;

	m_bHasScalingComponent = 
		m_bHasRotationComponent = 
		m_bHasTranslationComponent = true;

	m_bIsRSDirty = 
		m_bIsTDirty = true;

	// Clear projection components
	m_mat4Transform._41 = m_mat4Transform._42 = m_mat4Transform._43 = 0.0f;
	m_mat4Transform._44 = 1.0f;
}
//----------------------------------------------------------------------------------------------
Transformation::Transformation(const Meson::Common::Maths::Matrix3f& p_mat3Rotation, 
							   const Meson::Common::Maths::Vector3f& p_vec3Translation)
	: m_mat3Rotation(p_mat3Rotation),
	  m_vec3Translation(p_vec3Translation),
	  m_vec3Scaling(1.0f)
{
	m_bIsIdentity = 
		m_bHasScalingComponent = false;

	m_bHasRotationComponent = 
		m_bHasTranslationComponent = true;

	m_bIsRSDirty = 
		m_bIsTDirty = true;

	// Clear projection components
	m_mat4Transform._41 = m_mat4Transform._42 = m_mat4Transform._43 = 0.0f;
	m_mat4Transform._44 = 1.0f;
}
//----------------------------------------------------------------------------------------------
Transformation::Transformation(const Meson::Common::Maths::Quaternionf& p_qtnRotation, 
							   const Meson::Common::Maths::Vector3f& p_vec3Translation,
							   const Meson::Common::Maths::Vector3f& p_vec3Scaling)
	: m_vec3Translation(p_vec3Translation),
	  m_vec3Scaling(p_vec3Scaling)
{
	// Provide an interface for quaternions
	p_qtnRotation.ExtractMatrix(m_mat3Rotation);

	m_bIsIdentity = false;

	m_bHasRotationComponent = 
		m_bHasScalingComponent = 
		m_bHasTranslationComponent = true;

	m_bIsRSDirty =
		m_bIsTDirty = true;

	// Clear projection components
	m_mat4Transform._41 = m_mat4Transform._42 = m_mat4Transform._43 = 0.0f;
	m_mat4Transform._44 = 1.0f;
}
//----------------------------------------------------------------------------------------------
Transformation::Transformation(const Meson::Common::Maths::Quaternionf& p_qtnRotation, 
							   const Meson::Common::Maths::Vector3f& p_vec3Translation,
							   float p_fScaling)
	: m_vec3Translation(p_vec3Translation),
	  m_vec3Scaling(p_fScaling)
{
	// Provide an interface for quaternions
	p_qtnRotation.ExtractMatrix(m_mat3Rotation);

	m_bIsIdentity = false;

	m_bHasScalingComponent = 
		m_bHasRotationComponent = 
		m_bHasTranslationComponent = true;

	m_bIsRSDirty = 
		m_bIsTDirty = true;

	// Clear projection components
	m_mat4Transform._41 = m_mat4Transform._42 = m_mat4Transform._43 = 0.0f;
	m_mat4Transform._44 = 1.0f;
}
//----------------------------------------------------------------------------------------------
Transformation::Transformation(const Meson::Common::Maths::Quaternionf& p_qtnRotation,  
							   const Meson::Common::Maths::Vector3f& p_vec3Translation)
	: m_vec3Translation(p_vec3Translation),
	  m_vec3Scaling(1.0f)
{
	// Provide an interface for quaternions
	p_qtnRotation.ExtractMatrix(m_mat3Rotation);

	m_bIsIdentity = 
		m_bHasScalingComponent = false;

	m_bHasRotationComponent = 
		m_bHasTranslationComponent = true;

	m_bIsRSDirty = 
		m_bIsTDirty = true;

	// Clear projection components
	m_mat4Transform._41 = m_mat4Transform._42 = m_mat4Transform._43 = 0.0f;
	m_mat4Transform._44 = 1.0f;
}
//----------------------------------------------------------------------------------------------
bool Transformation::IsIdentity(void) const
{
	return m_bIsIdentity;
}
//----------------------------------------------------------------------------------------------
bool Transformation::HasScalingComponent(void) const
{
	return m_bHasScalingComponent;
}
//----------------------------------------------------------------------------------------------
bool Transformation::HasTranslationComponent(void) const
{
	return m_bHasTranslationComponent;
}
//----------------------------------------------------------------------------------------------
bool Transformation::HasRotationComponent(void) const
{
	return m_bHasRotationComponent;
}
//----------------------------------------------------------------------------------------------
void Transformation::MakeIdentity(void)
{
	m_mat3Rotation.MakeIdentity();
	m_vec3Translation = 0.0f;
	m_vec3Scaling = 1.0f;

	m_bIsIdentity = 
		m_bIsRSDirty = 
		m_bIsTDirty = true;

	m_bHasScalingComponent = 
		m_bHasTranslationComponent = 
		m_bHasRotationComponent = false;
}
//----------------------------------------------------------------------------------------------
void Transformation::MakeUnitScale(void)
{
	m_vec3Scaling = 1.0f;

	m_bHasScalingComponent = false;
	m_bIsRSDirty = true;
}
//----------------------------------------------------------------------------------------------
void Transformation::SetRotation(const Meson::Common::Maths::Matrix3f& p_mat3Rotation)
{
	m_mat3Rotation = p_mat3Rotation;
	
	m_bIsIdentity = false;
	m_bHasRotationComponent = true;
	m_bIsRSDirty = true;
}
//----------------------------------------------------------------------------------------------
void Transformation::SetRotation(const Meson::Common::Maths::Quaternionf& p_qtnRotation)
{
	p_qtnRotation.ExtractMatrix(m_mat3Rotation);
	
	m_bIsIdentity = false;
	m_bHasRotationComponent = true;
	m_bIsRSDirty = true;
}
//----------------------------------------------------------------------------------------------
void Transformation::GetRotation(Meson::Common::Maths::Quaternionf& p_qtnRotationOut) const
{
	p_qtnRotationOut.MakeRotation(m_mat3Rotation);
}
//----------------------------------------------------------------------------------------------
void Transformation::GetRotation(Meson::Common::Maths::Matrix3f& p_mat3RotationOut) const
{
	p_mat3RotationOut = m_mat3Rotation;
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Maths::Quaternionf& Transformation::GetRotationInQuaternionForm(void)
{
	m_qtnRotation.MakeRotation(m_mat3Rotation);
	return m_qtnRotation;
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Maths::Matrix3f& Transformation::GetRotationInMatrixForm(void) const
{
	return m_mat3Rotation;
}
//----------------------------------------------------------------------------------------------
void Transformation::SetScaling(const Meson::Common::Maths::Vector3f& p_vec3Scaling)
{
	m_vec3Scaling = p_vec3Scaling;

	m_bIsIdentity = false;
	m_bHasScalingComponent = true;	
	m_bIsRSDirty = true;
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Maths::Vector3f& Transformation::GetScaling(void) const
{
	MESON_ASSERT(m_bHasScalingComponent == true, "Transformation does not define a scaling component");
	return m_vec3Scaling;
}
//----------------------------------------------------------------------------------------------
void Transformation::SetTranslation(const Meson::Common::Maths::Vector3f& p_vec3Translation)
{
	m_vec3Translation = p_vec3Translation;

	m_bIsIdentity = false;
	m_bHasTranslationComponent = true;
	m_bIsTDirty = true;
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Maths::Vector3f& Transformation::GetTranslation(void) const
{
	return m_vec3Translation;
}
//----------------------------------------------------------------------------------------------
void Transformation::Apply(Meson::Common::Maths::Vector3f& p_vec3Out) const
{
	if (!m_bIsIdentity)
	{
		// TODO: Test whether it is quicker to apply all three transformations without checks
		if (m_bHasScalingComponent)
			p_vec3Out = p_vec3Out.ComponentProductCopy(m_vec3Scaling);

		if (m_bHasRotationComponent)
			p_vec3Out = m_mat3Rotation * p_vec3Out;

		if (m_bHasTranslationComponent)
			p_vec3Out += m_vec3Translation;
	}
}
//----------------------------------------------------------------------------------------------
Vector3f Transformation::ApplyCopy(const Meson::Common::Maths::Vector3f& p_vec3Input) const
{
	if (m_bIsIdentity)
		return p_vec3Input;

	//// Y = M(SX) + T
	//if (m_bHasScalingComponent)
	//	return (m_mat3Rotation * (m_vec3Scaling.UnitProductCopy(p_vec3Input))) + m_vec3Translation;

	//return m_mat3Rotation * p_vec3Input + m_vec3Translation;

	Vector3f vec3Out = p_vec3Input;

	if (m_bHasScalingComponent)
		vec3Out = vec3Out.ComponentProductCopy(m_vec3Scaling);

	if (m_bHasRotationComponent)
		vec3Out = m_mat3Rotation * vec3Out;

	if (m_bHasTranslationComponent)
		vec3Out += m_vec3Translation;

	return vec3Out;
}
//----------------------------------------------------------------------------------------------
void Transformation::ApplyInverse(Meson::Common::Maths::Vector3f& p_vec3Out) const
{
	if (!m_bIsIdentity)
	{
		if (m_bHasTranslationComponent)
			p_vec3Out -= m_vec3Translation;

		if (m_bHasRotationComponent)
		{
			// TODO: Should try optimising non-scaling matrices by using
			//		 a transpose instead of an inverse.
			p_vec3Out = m_mat3Rotation.InvertCopy() * p_vec3Out;
		}

		if (m_bHasScalingComponent)
		{
			// Factor out divisions by using the reciprocal of X*Y*Z 
			// and multiplying by YZ, XZ and XY 
			// (Thanks to D. Eberly)
			float fYZ = m_vec3Scaling.Y * m_vec3Scaling.Z,
				  fXZ = m_vec3Scaling.X * m_vec3Scaling.Z,
				  fXY = m_vec3Scaling.X * m_vec3Scaling.Y,
				  fInvXYZ = 1 / (fXY * m_vec3Scaling.Z);

			p_vec3Out.X *= (fInvXYZ * fYZ);
			p_vec3Out.Y *= (fInvXYZ * fXZ);
			p_vec3Out.Z *= (fInvXYZ * fXY);
		}
	}
}
//----------------------------------------------------------------------------------------------
Meson::Common::Maths::Vector3f Transformation::ApplyInverseCopy(const Meson::Common::Maths::Vector3f& p_vec3Input) const
{
	if (m_bIsIdentity)
		return p_vec3Input;

	Vector3f vec3Out = p_vec3Input;

	if (m_bHasTranslationComponent)
		vec3Out -= m_vec3Translation;

	if (m_bHasRotationComponent)
	{
		// TODO: Should try optimising non-scaling matrices by using
		//		 a transpose instead of an inverse.
		vec3Out = m_mat3Rotation.InvertCopy() * vec3Out;
	}

	if (m_bHasScalingComponent)
	{
		// Factor out divisions by using the reciprocal of X*Y*Z 
		// and multiplying by YZ, XZ and XY 
		// (Thanks to D. Eberly)
		float fYZ = m_vec3Scaling.Y * m_vec3Scaling.Z,
			  fXZ = m_vec3Scaling.X * m_vec3Scaling.Z,
			  fXY = m_vec3Scaling.X * m_vec3Scaling.Y,
			  fInvXYZ = 1 / (fXY * m_vec3Scaling.Z);

		vec3Out.X *= (fInvXYZ * fYZ);
		vec3Out.Y *= (fInvXYZ * fXZ);
		vec3Out.Z *= (fInvXYZ * fXY);
	}

	return vec3Out;
}
//----------------------------------------------------------------------------------------------
void Transformation::Product(const Transformation& p_transformation)
{
	// If either transformation is identity, peform a straight copy
	if (p_transformation.m_bIsIdentity)
		return;

	if (m_bIsIdentity)
	{
		*this = p_transformation;
		return;
	}

	Matrix3f mat3R1S1(p_transformation.m_mat3Rotation),
			 mat3R2S2(m_mat3Rotation);

	if (p_transformation.m_bHasScalingComponent)
	{
		mat3R1S1._11 *= p_transformation.m_vec3Scaling.X;
		mat3R1S1._22 *= p_transformation.m_vec3Scaling.Y;
		mat3R1S1._33 *= p_transformation.m_vec3Scaling.Z;
	}

	if (m_bHasScalingComponent)
	{
		mat3R2S2._11 *= m_vec3Scaling.X;
		mat3R2S2._22 *= m_vec3Scaling.Y;
		mat3R2S2._33 *= m_vec3Scaling.Z;
	}

	if (p_transformation.m_bHasTranslationComponent)
		m_vec3Translation += mat3R2S2 * p_transformation.m_vec3Translation;

	m_mat3Rotation = mat3R1S1 * m_mat3Rotation;

	// Set Dirty flags
	m_bIsRSDirty = m_bIsTDirty = true;
	m_bIsIdentity = false;

	m_bHasRotationComponent |= p_transformation.m_bHasRotationComponent;
	m_bHasScalingComponent |= p_transformation.m_bHasScalingComponent;
	m_bHasTranslationComponent |= p_transformation.m_bHasTranslationComponent;
}
//----------------------------------------------------------------------------------------------
void Transformation::Product(const Transformation& p_transformation1, const Transformation& p_transformation2)
{
	// If either transformation is identity, peform a straight copy
	if (p_transformation1.m_bIsIdentity)
	{
		*this = p_transformation2;
		return;
	}

	if (p_transformation2.m_bIsIdentity)
	{
		*this = p_transformation1;
		return;
	}

	Matrix3f mat3R1S1(p_transformation1.m_mat3Rotation),
			 mat3R2S2(p_transformation2.m_mat3Rotation);

	if (p_transformation1.m_bHasScalingComponent)
	{
		mat3R1S1._11 *= p_transformation1.m_vec3Scaling.X;
		mat3R1S1._22 *= p_transformation1.m_vec3Scaling.Y;
		mat3R1S1._33 *= p_transformation1.m_vec3Scaling.Z;
	}

	if (p_transformation2.m_bHasScalingComponent)
	{
		mat3R2S2._11 *= p_transformation2.m_vec3Scaling.X;
		mat3R2S2._22 *= p_transformation2.m_vec3Scaling.Y;
		mat3R2S2._33 *= p_transformation2.m_vec3Scaling.Z;
	}

	// R1S1*R2S2 | R1S1T2 + T1
	// -----------------------
	//     0     |      1 

	if (p_transformation2.m_bHasTranslationComponent)
		m_vec3Translation = mat3R1S1 * p_transformation2.m_vec3Translation + p_transformation1.m_vec3Translation;
	else
		m_vec3Translation = p_transformation1.m_vec3Translation;

	m_vec3Scaling = p_transformation2.m_vec3Scaling;
	m_mat3Rotation = mat3R1S1 * mat3R2S2;

	// Set Dirty flags
	m_bIsRSDirty = m_bIsTDirty = true;
	m_bIsIdentity = false;

	m_bHasRotationComponent = p_transformation1.m_bHasRotationComponent | p_transformation2.m_bHasRotationComponent;
	m_bHasScalingComponent = p_transformation1.m_bHasScalingComponent | p_transformation2.m_bHasScalingComponent;
	m_bHasTranslationComponent = p_transformation1.m_bHasTranslationComponent | p_transformation2.m_bHasTranslationComponent;
}
//----------------------------------------------------------------------------------------------
Transformation Transformation::ProductCopy(const Transformation& p_transformation)
{
	Transformation result;
	result.Product(*this, p_transformation);
	return result;
}
//----------------------------------------------------------------------------------------------
Transformation Transformation::ProductCopy(const Transformation& p_transformation1, const Transformation& p_transformation2)
{
	Transformation result;
	result.Product(p_transformation1, p_transformation2);
	return result;
}
//----------------------------------------------------------------------------------------------
void Transformation::Invert(void)
{
	// TODO: 
	if (!m_bIsIdentity)
	{
		if (m_bHasScalingComponent)
		{
			m_mat3Rotation._11 *= m_vec3Scaling.X;
			m_mat3Rotation._22 *= m_vec3Scaling.Y;
			m_mat3Rotation._33 *= m_vec3Scaling.Z;
		}

		if (m_bHasRotationComponent)
			m_mat3Rotation.Invert();

		if (m_bHasTranslationComponent)
			m_vec3Translation = -(m_mat3Rotation * m_vec3Translation);
	}
}
//----------------------------------------------------------------------------------------------
Transformation Transformation::InvertCopy(void)
{
	return *this;
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Maths::Matrix4f& Transformation::ExtractMatrix(void) const
{
	if (m_bIsIdentity)
		return m_mat4Transform.Identity;

	if (m_bIsRSDirty)
	{
		m_mat4Transform._11 = m_mat3Rotation._11 * m_vec3Scaling.X;
		m_mat4Transform._12 = m_mat3Rotation._12;
		m_mat4Transform._13 = m_mat3Rotation._13;
		m_mat4Transform._21 = m_mat3Rotation._21;
		m_mat4Transform._22 = m_mat3Rotation._22 * m_vec3Scaling.Y;
		m_mat4Transform._23 = m_mat3Rotation._23;
		m_mat4Transform._31 = m_mat3Rotation._31;
		m_mat4Transform._32 = m_mat3Rotation._32;
		m_mat4Transform._33 = m_mat3Rotation._33 * m_vec3Scaling.Z;
		
		m_bIsRSDirty = false;
	}

	if (m_bIsTDirty)
	{
		m_mat4Transform._14 = m_vec3Translation.X;
		m_mat4Transform._24 = m_vec3Translation.Y;
		m_mat4Transform._34 = m_vec3Translation.Z;

		m_bIsTDirty = false;
	}

	return m_mat4Transform;
}
//----------------------------------------------------------------------------------------------
void Transformation::ExtractMatrix(Meson::Common::Maths::Matrix4f& p_mat4Out) const
{
	if (!m_bIsIdentity)
	{
		if (m_bIsRSDirty)
		{
			m_mat4Transform._11 = m_mat3Rotation._11 * m_vec3Scaling.X;
			m_mat4Transform._12 = m_mat3Rotation._12;
			m_mat4Transform._13 = m_mat3Rotation._13;
			m_mat4Transform._21 = m_mat3Rotation._21;
			m_mat4Transform._22 = m_mat3Rotation._22 * m_vec3Scaling.Y;
			m_mat4Transform._23 = m_mat3Rotation._23;
			m_mat4Transform._31 = m_mat3Rotation._31;
			m_mat4Transform._32 = m_mat3Rotation._32;
			m_mat4Transform._33 = m_mat3Rotation._33 * m_vec3Scaling.Z;
					
			m_bIsRSDirty = false;
		}

		if (m_bIsTDirty)
		{
			m_mat4Transform._14 = m_vec3Translation.X;
			m_mat4Transform._24 = m_vec3Translation.Y;
			m_mat4Transform._34 = m_vec3Translation.Z;

			m_bIsTDirty = false;
		}

		p_mat4Out = m_mat4Transform;
	}
	else
	{
		p_mat4Out.MakeIdentity();
	}
}
//----------------------------------------------------------------------------------------------
Transformation& Transformation::operator=(const Transformation& p_transformation)
{
	m_bIsIdentity = p_transformation.m_bIsIdentity;
	m_bIsTDirty = p_transformation.m_bIsTDirty;
	m_bIsRSDirty = p_transformation.m_bIsRSDirty;
	m_bHasScalingComponent = p_transformation.m_bHasScalingComponent;
	m_bHasRotationComponent = p_transformation.m_bHasRotationComponent;
	m_bHasTranslationComponent = p_transformation.m_bHasTranslationComponent;
	m_mat4Transform = p_transformation.m_mat4Transform;
	m_mat3Rotation = p_transformation.m_mat3Rotation;
	m_vec3Scaling = p_transformation.m_vec3Scaling;
	m_vec3Translation = p_transformation.m_vec3Translation;

	return *this;
}
//----------------------------------------------------------------------------------------------
bool Transformation::operator==(const Transformation& p_transformation) const
{
	if (m_bIsIdentity == p_transformation.m_bIsIdentity)
		return true;

	return (m_bHasScalingComponent == p_transformation.m_bHasScalingComponent &&
			m_bHasRotationComponent == p_transformation.m_bHasRotationComponent &&
			m_bHasTranslationComponent == p_transformation.m_bHasTranslationComponent &&
			m_mat3Rotation == p_transformation.m_mat3Rotation &&
			m_vec3Translation == p_transformation.m_vec3Translation &&
			m_vec3Scaling == p_transformation.m_vec3Scaling);
}
//----------------------------------------------------------------------------------------------
bool Transformation::operator!=(const Transformation& p_transformation) const
{
	if (m_bIsIdentity == p_transformation.m_bIsIdentity)
		return false;

	return ( m_bHasScalingComponent != p_transformation.m_bHasScalingComponent ||
			m_bHasRotationComponent != p_transformation.m_bHasRotationComponent ||
			m_bHasTranslationComponent != p_transformation.m_bHasTranslationComponent ||
			 m_mat3Rotation != p_transformation.m_mat3Rotation ||
			 m_vec3Translation != p_transformation.m_vec3Translation ||
			 m_vec3Scaling != p_transformation.m_vec3Scaling );
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END