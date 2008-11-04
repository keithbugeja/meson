//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"
#include "VstColour.h"
#include "VstPropertySet.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class VISTAS_API Light : public VistasObject, IPropertySet
	{
		VISTAS_IMPLEMENT_RTTI(Light, VistasObject);

	public:
		enum LightType
		{
			LT_DIRECTIONAL,
			LT_POINT,
			LT_SPOT,

			LT_COUNT
		};

	public:
		//----------------------------------------------------------------------------------------------
		// IPropertySet methods
		//----------------------------------------------------------------------------------------------
		void EnumerateProperties(Meson::Common::Collections::THashMap<Meson::Common::Text::String, 
			PropertyDescriptor> p_propertyDescriptorMap);
		void GetPropertyDescriptor(const Meson::Common::Text::String& p_strPropertyName,
			PropertyDescriptor& p_propertyDescriptor);
		bool ContainsProperty(const Meson::Common::Text::String& p_strPropertyName);

		void SetProperty(const Meson::Common::Text::String& p_strPropertyName, 
			bool p_bValue);
		void SetProperty(const Meson::Common::Text::String& p_strPropertyName, 
			float p_fValue);
		void SetProperty(const Meson::Common::Text::String& p_strPropertyName, 
			const Colour& p_colour);
		void SetProperty(const Meson::Common::Text::String& p_strPropertyName, 
			const Meson::Common::Maths::Vector3f& p_vector3);
		
		void GetProperty(const Meson::Common::Text::String& p_strPropertyName, 
			bool p_bValue) const;
		void GetProperty(const Meson::Common::Text::String& p_strPropertyName, 
			float& p_fValue) const;
		void GetProperty(const Meson::Common::Text::String& p_strPropertyName, 
			Colour& p_colour) const;
		void GetProperty(const Meson::Common::Text::String& p_strPropertyName, 
			Meson::Common::Maths::Vector3f& p_vector3) const;
		//----------------------------------------------------------------------------------------------

	public:
		Light(void);
		Light(const Light& p_light);
		Light(const Meson::Common::Text::String& p_strName);

		~Light(void);

		void Reset(void);

		LightType GetType(void) const;
		void SetType(const LightType p_eLightType);

		bool IsLightEnabled(void) const;
		void SetLight(bool p_bEnabled);

		const Colour& GetAmbient(void) const;
		const Colour& GetDiffuse(void) const;
		const Colour& GetSpecular(void) const;
		float GetSpecularPower(void) const;

		void SetAmbient(const Colour& p_ambientColour);
		void SetDiffuse(const Colour& p_diffuseColour);
		void SetSpecular(const Colour& p_specularColour);

		void SetAxes(const Meson::Common::Maths::Vector3f& p_vec3Direction, 
			const Meson::Common::Maths::Vector3f& p_vec3VerticalAxis, 
			const Meson::Common::Maths::Vector3f& p_vec3HorizontalAxis,
			bool p_bNormaliseAxes = false);

		void SetFrame(const Meson::Common::Maths::Vector3f& p_vec3Position, 
			const Meson::Common::Maths::Vector3f& p_vec3Direction, 
			const Meson::Common::Maths::Vector3f& p_vec3VerticalAxis, 
			const Meson::Common::Maths::Vector3f& p_vec3HorizontalAxis,
			bool p_bNormaliseAxes = false);

		const Meson::Common::Maths::Vector3f& GetPosition(void) const;
		const Meson::Common::Maths::Vector3f& GetDirection(void) const;
		const Meson::Common::Maths::Vector3f& GetVerticalAxis(void) const;
		const Meson::Common::Maths::Vector3f& GetHorizontalAxis(void) const;

		void SetPosition(const Meson::Common::Maths::Vector3f& p_vec3Position);
		void SetDirection(const Meson::Common::Maths::Vector3f& p_vec3Direction, bool p_bNormalise = false);
		void SetHorizontalAxis(const Meson::Common::Maths::Vector3f& p_vec3HorizontalAxis, bool p_bNormalise = false);
		void SetVerticalAxis(const Meson::Common::Maths::Vector3f& p_vec3VerticalAxis, bool p_bNormalise = false);

		float GetRange(void) const;
		void SetRange(float p_fRange);

		float GetIntensity(void) const;
		void SetIntensity(float p_fIntensity);
		
		float GetConstantAttenuation(void) const;
		float GetLinearAttenuation(void) const;
		float GetQuadraticAttenuation(void) const;

		void SetAttenuation(float p_fConstant, float p_fLinear, float p_fQuadratic);
		void SetConstantAttenuation(float p_fConstant);
		void SetLinearAttenuation(float p_fLinear);
		void SetQuadraticAttenuation(float p_fQuadratic);
		
		float GetCutoffAngle(void) const;
		float GetFalloffAngle(void) const;
		float GetFalloffExponent(void) const;

		void SetCone(float p_fAngle, float p_fFalloffAngle, float p_fFalloffExponent);
		void SetCutoffAngle(float p_fAngle);
		void SetFalloffAngle(float p_fAngle);
		void SetFalloffExponent(float p_fExponent);

		bool operator==(const Light& p_light) const;
		const Light& operator=(const Light& p_light);

	protected:
		bool m_bIsLightEnabled;

		float m_fRange, 
			  m_fIntensity;

		float m_fConstantAttenuation,
			  m_fLinearAttenuation,
			  m_fQuadraticAttenuation;
		
		float m_fCutoffAngle,
			  m_fFalloffAngle,
			  m_fFalloffExponent;

		LightType m_eLightType;

		Meson::Vistas::Colour m_ambientColour,
							  m_diffuseColour,
							  m_specularColour;

		Meson::Common::Maths::Vector3f m_vec3Position,
									   m_vec3Direction,
									   m_vec3Right,
									   m_vec3Up;
	};
	//----------------------------------------------------------------------------------------------
	typedef Meson::Common::TPointer<Light> LightPtr;
	typedef Meson::Common::Collections::TArrayList<LightPtr> LightList;
	typedef Meson::Common::Collections::TEnumerator<LightPtr>& LightEnumerator;
	typedef Meson::Common::TPointer<LightList> LightListPtr;
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END
