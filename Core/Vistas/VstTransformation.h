//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "VstTypes.h"

Meson_Vistas_BEGIN

	class Transformation
	{
	public:
		//----------------------------------------------------------------------------------------------
		Transformation(void);
		Transformation(const Transformation& p_transformation);

		//----------------------------------------------------------------------------------------------
		Transformation(const Meson::Common::Maths::Quaternionf& p_qtnRotation,
			const Meson::Common::Maths::Vector3f& p_vec3Translation,
			const Meson::Common::Maths::Vector3f& p_vec3Scaling);
		Transformation(const Meson::Common::Maths::Quaternionf& p_qtnRotation,
			const Meson::Common::Maths::Vector3f& p_vec3Translation,
			float p_fScaling);
		Transformation(const Meson::Common::Maths::Quaternionf& p_qtnRotation,
			const Meson::Common::Maths::Vector3f& p_vec3Translation);

		//----------------------------------------------------------------------------------------------
		Transformation(const Meson::Common::Maths::Matrix3f& p_mat3Rotation, 
			const Meson::Common::Maths::Vector3f& p_vec3Translation,
			const Meson::Common::Maths::Vector3f& p_vec3Scaling);
		Transformation(const Meson::Common::Maths::Matrix3f& p_mat3Rotation, 
			const Meson::Common::Maths::Vector3f& p_vec3Translation,
			float p_fScaling);
		Transformation(const Meson::Common::Maths::Matrix3f& p_mat3Rotation, 
			const Meson::Common::Maths::Vector3f& p_vec3Translation);

		//----------------------------------------------------------------------------------------------
		bool IsIdentity(void) const;

		bool HasScalingComponent(void) const;
		bool HasRotationComponent(void) const;
		bool HasTranslationComponent(void) const;

		//----------------------------------------------------------------------------------------------
		void MakeIdentity(void);
		void MakeUnitScale(void);

		//----------------------------------------------------------------------------------------------
		void SetRotation(const Meson::Common::Maths::Quaternionf& p_qtnRotation);
		void SetRotation(const Meson::Common::Maths::Matrix3f& p_mat3Rotation);

		// This is a means to avoid discriminating either matrices or quaternions
		void GetRotation(Meson::Common::Maths::Quaternionf& p_qtnRotationOut) const;
		void GetRotation(Meson::Common::Maths::Matrix3f& p_mat3RotationOut) const;

		const Meson::Common::Maths::Matrix3f& GetRotationInMatrixForm(void) const;
		const Meson::Common::Maths::Quaternionf& GetRotationInQuaternionForm(void);

		//----------------------------------------------------------------------------------------------
		void SetScaling(const Meson::Common::Maths::Vector3f& p_vec3Scaling);
		const Meson::Common::Maths::Vector3f& GetScaling(void) const;

		//----------------------------------------------------------------------------------------------
		void SetTranslation(const Meson::Common::Maths::Vector3f& p_vec3Translation);
		const Meson::Common::Maths::Vector3f& GetTranslation(void) const;

		//----------------------------------------------------------------------------------------------
		void Apply(Meson::Common::Maths::Vector3f& p_vec3Out) const;
		Meson::Common::Maths::Vector3f ApplyCopy(const Meson::Common::Maths::Vector3f& p_vec3Input) const;

		void ApplyInverse(Meson::Common::Maths::Vector3f& p_vec3Out) const;
		Meson::Common::Maths::Vector3f ApplyInverseCopy(const Meson::Common::Maths::Vector3f& p_vec3Input) const;
		//----------------------------------------------------------------------------------------------
		void Product(const Transformation& p_transformation);
		void Product(const Transformation& p_transformation1, const Transformation& p_transformation2);

		Transformation ProductCopy(const Transformation& p_transformation);
		Transformation ProductCopy(const Transformation& p_transformation1, const Transformation& p_transformation2);

		//----------------------------------------------------------------------------------------------
		void Invert(void);
		Transformation InvertCopy(void);

		//----------------------------------------------------------------------------------------------
		const Meson::Common::Maths::Matrix4f& ExtractMatrix(void) const;
		void ExtractMatrix(Meson::Common::Maths::Matrix4f& p_mat4Out) const;

		//----------------------------------------------------------------------------------------------
		Transformation& operator=(const Transformation& p_transformation);
		bool operator==(const Transformation& p_transformation) const;
		bool operator!=(const Transformation& p_transformation) const;
		//----------------------------------------------------------------------------------------------

	protected:
		mutable Meson::Common::Maths::Matrix4f m_mat4Transform;
		Meson::Common::Maths::Matrix3f m_mat3Rotation;
		Meson::Common::Maths::Vector3f m_vec3Scaling;
		Meson::Common::Maths::Vector3f m_vec3Translation;
		Meson::Common::Maths::Quaternionf m_qtnRotation;

		mutable bool m_bIsTDirty, 
					 m_bIsRSDirty;
			 
		bool m_bIsIdentity,
			 m_bHasScalingComponent,
			 m_bHasRotationComponent,
			 m_bHasTranslationComponent;
	
	public:
		static const Transformation Identity;
	};

Meson_Vistas_END