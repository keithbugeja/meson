#include "VstVistasEngine.h"
#include "VstLight.h"

#include "Maths.h"
#include "Vector3.h"
#include "Matrix4.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
void Light::EnumerateProperties(Meson::Common::Collections::THashMap<Meson::Common::Text::String, PropertyDescriptor> p_propertyDescriptorMap)
{
	p_propertyDescriptorMap.Insert("Enabled", PropertyDescriptor("Enabled", PropertyDescriptor::PT_BOOLEAN, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("Position", PropertyDescriptor("Position", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("Direction", PropertyDescriptor("Direction", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("VerticalAxis", PropertyDescriptor("VerticalAxis", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("HorizontalAxis", PropertyDescriptor("HorizontalAxis", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("Ambient", PropertyDescriptor("Ambient", PropertyDescriptor::PT_COLOUR, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("Diffuse", PropertyDescriptor("Diffuse", PropertyDescriptor::PT_COLOUR, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("Specular", PropertyDescriptor("Specular", PropertyDescriptor::PT_COLOUR, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("Intensity", PropertyDescriptor("Intensity", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("Range", PropertyDescriptor("Range", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("Attenuation", PropertyDescriptor("Attenuation", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("ConstantAttenuation", PropertyDescriptor("ConstantAttenuation", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("LinearAttenuation", PropertyDescriptor("LinearAttenuation", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("QuadraticAttenuation", PropertyDescriptor("QuadraticAttenuation", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("Cone", PropertyDescriptor("Cone", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("CutoffAngle", PropertyDescriptor("CutoffAngle", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("FalloffAngle", PropertyDescriptor("FalloffAngle", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false));
	p_propertyDescriptorMap.Insert("FalloffExponent", PropertyDescriptor("FalloffExponent", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false));
}
//----------------------------------------------------------------------------------------------
void Light::GetPropertyDescriptor(const Meson::Common::Text::String& p_strPropertyName, PropertyDescriptor& p_propertyDescriptor)
{
	if (!p_strPropertyName.Compare("Enabled"))
		p_propertyDescriptor = PropertyDescriptor("Enabled", PropertyDescriptor::PT_BOOLEAN, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("Position"))
		p_propertyDescriptor = PropertyDescriptor("Position", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("Direction"))
		p_propertyDescriptor =  PropertyDescriptor("Direction", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("VerticalAxis"))
		p_propertyDescriptor =  PropertyDescriptor("VerticalAxis", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("HorizontalAxis"))
		p_propertyDescriptor =  PropertyDescriptor("HorizontalAxis", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("Ambient"))
		p_propertyDescriptor =  PropertyDescriptor("Ambient", PropertyDescriptor::PT_COLOUR, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("Diffuse"))
		p_propertyDescriptor =  PropertyDescriptor("Diffuse", PropertyDescriptor::PT_COLOUR, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("Specular"))
		p_propertyDescriptor =  PropertyDescriptor("Specular", PropertyDescriptor::PT_COLOUR, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("Intensity"))
		p_propertyDescriptor =  PropertyDescriptor("Intensity", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("Range"))
		p_propertyDescriptor =  PropertyDescriptor("Range", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("Attenuation"))
		p_propertyDescriptor =  PropertyDescriptor("Attenuation", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("ConstantAttenuation"))
		p_propertyDescriptor =  PropertyDescriptor("ConstantAttenuation", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("LinearAttenuation"))
		p_propertyDescriptor =  PropertyDescriptor("LinearAttenuation", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("QuadraticAttenuation"))
		p_propertyDescriptor =  PropertyDescriptor("QuadraticAttenuation", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("Cone"))
		p_propertyDescriptor =  PropertyDescriptor("Cone", PropertyDescriptor::PT_VECTOR3, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("CutoffAngle"))
		p_propertyDescriptor =  PropertyDescriptor("CutoffAngle", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("FalloffAngle"))
		p_propertyDescriptor =  PropertyDescriptor("FalloffAngle", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false);
	else if (!p_strPropertyName.Compare("FalloffExponent"))
		p_propertyDescriptor =  PropertyDescriptor("FalloffExponent", PropertyDescriptor::PT_REAL, PropertyDescriptor::PC_SCALAR, false);
	else
		throw new MesonException("Property set does not contain specified property!", __FILE__, __LINE__ );
}
//----------------------------------------------------------------------------------------------
bool Light::ContainsProperty(const Meson::Common::Text::String& p_strPropertyName)
{
	return (!p_strPropertyName.Compare("Enabled") ||
		!p_strPropertyName.Compare("Position") ||
		!p_strPropertyName.Compare("Direction") ||
		!p_strPropertyName.Compare("VerticalAxis") ||
		!p_strPropertyName.Compare("HorizontalAxis") ||
		!p_strPropertyName.Compare("Ambient") ||
		!p_strPropertyName.Compare("Diffuse") ||
		!p_strPropertyName.Compare("Specular") ||
		!p_strPropertyName.Compare("Intensity") ||
		!p_strPropertyName.Compare("Range") ||
		!p_strPropertyName.Compare("Attenuation") ||
		!p_strPropertyName.Compare("ConstantAttenuation") ||
		!p_strPropertyName.Compare("LinearAttenuation") ||
		!p_strPropertyName.Compare("QuadraticAttenuation") ||
		!p_strPropertyName.Compare("Cone") ||
		!p_strPropertyName.Compare("CutoffAngle") ||
		!p_strPropertyName.Compare("FalloffAngle") ||
		!p_strPropertyName.Compare("FalloffExponent"));
}
//----------------------------------------------------------------------------------------------
void Light::SetProperty(const Meson::Common::Text::String& p_strPropertyName, bool p_bValue)
{
	if (!p_strPropertyName.Compare("Enabled")) SetLight(p_bValue);
}
//----------------------------------------------------------------------------------------------
void Light::SetProperty(const Meson::Common::Text::String& p_strPropertyName, float p_fValue)
{
	if (!p_strPropertyName.Compare("Intensity"))
		SetIntensity(p_fValue);
	else if (!p_strPropertyName.Compare("Range"))
		SetRange(p_fValue);
	else if (!p_strPropertyName.Compare("ConstantAttenuation"))
		SetConstantAttenuation(p_fValue);
	else if (!p_strPropertyName.Compare("LinearAttenuation"))
		SetLinearAttenuation(p_fValue);
	else if (!p_strPropertyName.Compare("QuadraticAttenuation"))
		SetQuadraticAttenuation(p_fValue);
	else if (!p_strPropertyName.Compare("CutoffAngle"))
		SetCutoffAngle(p_fValue);
	else if (!p_strPropertyName.Compare("FalloffAngle"))
		SetFalloffAngle(p_fValue);
	else if (!p_strPropertyName.Compare("FalloffExponent"))
		SetFalloffExponent(p_fValue);
}
//----------------------------------------------------------------------------------------------
void Light::SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Colour& p_colour)
{
	if (!p_strPropertyName.Compare("Ambient"))
		SetAmbient(p_colour);
	else if (!p_strPropertyName.Compare("Diffuse"))
		SetDiffuse(p_colour);
	else if (!p_strPropertyName.Compare("Specular"))
		SetSpecular(p_colour);
}
//----------------------------------------------------------------------------------------------
void Light::SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Vector3f& p_vector3)
{
	if (!p_strPropertyName.Compare("Position"))
		SetPosition(p_vector3);
	else if (!p_strPropertyName.Compare("Direction"))
		SetDirection(p_vector3, true);
	else if (!p_strPropertyName.Compare("VerticalAxis"))
		SetVerticalAxis(p_vector3, true);
	else if (!p_strPropertyName.Compare("HorizontalAxis"))
		SetHorizontalAxis(p_vector3, true);
	else if (!p_strPropertyName.Compare("Attenuation"))
		SetAttenuation(p_vector3.X, p_vector3.Y, p_vector3.Z);
	else if (!p_strPropertyName.Compare("Cone"))
		SetCone(p_vector3.X, p_vector3.Y, p_vector3.Z);
}
//----------------------------------------------------------------------------------------------
void Light::GetProperty(const Meson::Common::Text::String& p_strPropertyName, bool p_bValue) const
{
	if (!p_strPropertyName.Compare("Enabled")) p_bValue = IsLightEnabled();
}
//----------------------------------------------------------------------------------------------
void Light::GetProperty(const Meson::Common::Text::String& p_strPropertyName, float& p_fValue) const
{
	if (!p_strPropertyName.Compare("Intensity"))
		p_fValue = GetIntensity();
	else if (!p_strPropertyName.Compare("Range"))
		p_fValue = GetRange();
	else if (!p_strPropertyName.Compare("ConstantAttenuation"))
		p_fValue = GetConstantAttenuation();
	else if (!p_strPropertyName.Compare("LinearAttenuation"))
		p_fValue = GetLinearAttenuation();
	else if (!p_strPropertyName.Compare("QuadraticAttenuation"))
		p_fValue = GetQuadraticAttenuation();
	else if (!p_strPropertyName.Compare("CutoffAngle"))
		p_fValue = GetCutoffAngle();
	else if (!p_strPropertyName.Compare("FalloffAngle"))
		p_fValue = GetFalloffAngle();
	else if (!p_strPropertyName.Compare("FalloffExponent"))
		p_fValue = GetFalloffExponent();
}
//----------------------------------------------------------------------------------------------
void Light::GetProperty(const Meson::Common::Text::String& p_strPropertyName, Colour& p_colour) const
{
	if (!p_strPropertyName.Compare("Ambient"))
		p_colour = GetAmbient();
	else if (!p_strPropertyName.Compare("Diffuse"))
		p_colour = GetDiffuse();
	else if (!p_strPropertyName.Compare("Specular"))
		p_colour = GetSpecular();
}
//----------------------------------------------------------------------------------------------
void Light::GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Maths::Vector3f& p_vector3) const
{
	if (!p_strPropertyName.Compare("Position"))
		p_vector3 = GetPosition();
	else if (!p_strPropertyName.Compare("Direction"))
		p_vector3 = GetDirection();
	else if (!p_strPropertyName.Compare("VerticalAxis"))
		p_vector3 = GetVerticalAxis();
	else if (!p_strPropertyName.Compare("HorizontalAxis"))
		p_vector3 = GetHorizontalAxis();
	else if (!p_strPropertyName.Compare("Attenuation"))
		p_vector3.Set(GetConstantAttenuation(), GetLinearAttenuation(), GetQuadraticAttenuation());
	else if (!p_strPropertyName.Compare("Cone"))
		p_vector3.Set(GetCutoffAngle(), GetFalloffAngle(), GetFalloffExponent());
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
Light::Light(void)
	: VistasObject()
{
}
//----------------------------------------------------------------------------------------------
Light::Light(const String& p_strName)
	: VistasObject(p_strName)
{
}
//----------------------------------------------------------------------------------------------
Light::Light(const Light& p_light)
{
	*this = p_light;
}
//----------------------------------------------------------------------------------------------
Light::~Light(void)
{
}
//----------------------------------------------------------------------------------------------
void Light::Reset(void)
{
	m_eLightType = Light::LT_DIRECTIONAL;
	m_bIsLightEnabled = true;
	m_fIntensity = 1.0f;
	m_fRange = 32.0f;

	SetAmbient(Colour::Black);
	SetDiffuse(Colour::Black);
	SetSpecular(Colour::Black);
		
	SetAttenuation(1.0f, 0.0f, 0.0f);
	SetFrame(Vector3f::Zero, Vector3f::In, Vector3f::Up, Vector3f::Right);
	SetCone(Mathf::DegreeToRadian(45.0f), Mathf::DegreeToRadian(25.0f), 1.0f);
}
//----------------------------------------------------------------------------------------------
Light::LightType Light::GetType(void) const 
{
	return m_eLightType;
}
//----------------------------------------------------------------------------------------------
void Light::SetType(const LightType p_eLightType) 
{
	m_eLightType = p_eLightType;
}
//----------------------------------------------------------------------------------------------
bool Light::IsLightEnabled(void) const 
{
	return m_bIsLightEnabled;
}
//----------------------------------------------------------------------------------------------
void Light::SetLight(bool p_bEnabled) 
{
	m_bIsLightEnabled = p_bEnabled;
}
//----------------------------------------------------------------------------------------------
const Colour& Light::GetAmbient(void) const
{
	return m_ambientColour;
}
//----------------------------------------------------------------------------------------------
const Colour& Light::GetDiffuse(void) const
{
	return m_diffuseColour;
}
//----------------------------------------------------------------------------------------------
const Colour& Light::GetSpecular(void) const
{
	return m_specularColour;
}
//----------------------------------------------------------------------------------------------
void Light::SetAmbient(const Colour& p_ambientColour)
{
	m_ambientColour = p_ambientColour;
}
//----------------------------------------------------------------------------------------------
void Light::SetDiffuse(const Colour& p_diffuseColour)
{
	m_diffuseColour = p_diffuseColour;
}
//----------------------------------------------------------------------------------------------
void Light::SetSpecular(const Colour& p_specularColour)
{
	m_specularColour = p_specularColour;
}
//----------------------------------------------------------------------------------------------
const Vector3f& Light::GetPosition(void) const
{
	return m_vec3Position;
}
//----------------------------------------------------------------------------------------------
const Vector3f& Light::GetDirection(void) const
{
	return m_vec3Direction;
}
//----------------------------------------------------------------------------------------------
const Vector3f& Light::GetHorizontalAxis(void) const
{
	return m_vec3Right;
}
//----------------------------------------------------------------------------------------------
const Vector3f& Light::GetVerticalAxis(void) const
{
	return m_vec3Up;
}
//----------------------------------------------------------------------------------------------
void Light::SetPosition(const Vector3f& p_vec3Position) 
{
	m_vec3Position = p_vec3Position;
}
//----------------------------------------------------------------------------------------------
void Light::SetDirection(const Vector3f& p_vec3Direction, bool p_bNormalise)
{
	m_vec3Direction = p_vec3Direction;

	if (p_bNormalise)
		m_vec3Direction.Normalise();
}
//----------------------------------------------------------------------------------------------
void Light::SetHorizontalAxis(const Vector3f& p_vec3HorizontalAxis, bool p_bNormalise)
{
	m_vec3Right = p_vec3HorizontalAxis;

	if (p_bNormalise)
		m_vec3Right.Normalise();
}
//----------------------------------------------------------------------------------------------
void Light::SetVerticalAxis(const Vector3f& p_vec3VerticalAxis, bool p_bNormalise)
{
	m_vec3Up = p_vec3VerticalAxis;

	if (p_bNormalise)
		m_vec3Up.Normalise();
}
//----------------------------------------------------------------------------------------------
void Light::SetAxes(const Vector3f& p_vec3Direction, const Vector3f& p_vec3VerticalAxis, 
					const Vector3f& p_vec3HorizontalAxis, bool p_bNormaliseAxes)
{
	m_vec3Up = p_vec3VerticalAxis;
	m_vec3Right = p_vec3HorizontalAxis;
	m_vec3Direction = p_vec3Direction;

	if (p_bNormaliseAxes)
	{
		m_vec3Up.Normalise();
		m_vec3Right.Normalise();
		m_vec3Direction.Normalise();
	}
}
//----------------------------------------------------------------------------------------------
void Light::SetFrame(const Vector3f& p_vec3Position, const Vector3f& p_vec3Direction, 
					 const Vector3f& p_vec3VerticalAxis, const Vector3f& p_vec3HorizontalAxis,
					 bool p_bNormaliseAxes)
{
	m_vec3Position = p_vec3Position;
	m_vec3Up = p_vec3VerticalAxis;
	m_vec3Right = p_vec3HorizontalAxis;
	m_vec3Direction = p_vec3Direction;

	if (p_bNormaliseAxes)
	{
		m_vec3Up.Normalise();
		m_vec3Right.Normalise();
		m_vec3Direction.Normalise();
	}
}
//----------------------------------------------------------------------------------------------
float Light::GetRange(void) const
{
	return m_fRange;
}
//----------------------------------------------------------------------------------------------
void Light::SetRange(float p_fRange)
{
	m_fRange = p_fRange;
}
//----------------------------------------------------------------------------------------------
float Light::GetIntensity(void) const
{
	return m_fIntensity;
}
//----------------------------------------------------------------------------------------------
void Light::SetIntensity(float p_fIntensity)
{
	m_fIntensity = p_fIntensity;
}
//----------------------------------------------------------------------------------------------
float Light::GetConstantAttenuation(void) const
{
	return m_fConstantAttenuation;
}
//----------------------------------------------------------------------------------------------
float Light::GetLinearAttenuation(void) const
{
	return m_fLinearAttenuation;
}
//----------------------------------------------------------------------------------------------
float Light::GetQuadraticAttenuation(void) const
{
	return m_fQuadraticAttenuation;
}
//----------------------------------------------------------------------------------------------
void Light::SetAttenuation(float p_fConstant, float p_fLinear, float p_fQuadratic)
{
	m_fConstantAttenuation = p_fConstant;
	m_fLinearAttenuation = p_fLinear;
	m_fQuadraticAttenuation = p_fQuadratic;
}
//----------------------------------------------------------------------------------------------
void Light::SetConstantAttenuation(float p_fConstant)
{
	m_fConstantAttenuation = p_fConstant;
}
//----------------------------------------------------------------------------------------------
void Light::SetLinearAttenuation(float p_fLinear)
{
	m_fLinearAttenuation = p_fLinear;
}
//----------------------------------------------------------------------------------------------
void Light::SetQuadraticAttenuation(float p_fQuadratic)
{
	m_fQuadraticAttenuation = p_fQuadratic;
}
//----------------------------------------------------------------------------------------------
float Light::GetCutoffAngle(void) const
{
	return m_fCutoffAngle;
}
//----------------------------------------------------------------------------------------------
float Light::GetFalloffAngle(void) const
{
	return m_fFalloffAngle;
}
//----------------------------------------------------------------------------------------------
float Light::GetFalloffExponent(void) const
{
	return m_fFalloffExponent;
}
//----------------------------------------------------------------------------------------------
void Light::SetCone(float p_fCutoffAngle, float p_fFalloffAngle, float p_fFalloffExponent)
{
	m_fCutoffAngle = p_fCutoffAngle;
	m_fFalloffAngle = p_fFalloffAngle;
	m_fFalloffExponent = p_fFalloffExponent;
}
//----------------------------------------------------------------------------------------------
void Light::SetCutoffAngle(float p_fAngle)
{
	m_fCutoffAngle = p_fAngle;
}
//----------------------------------------------------------------------------------------------
void Light::SetFalloffAngle(float p_fAngle)
{
	m_fFalloffAngle = p_fAngle;
}
//----------------------------------------------------------------------------------------------
void Light::SetFalloffExponent(float p_fExponent)
{
	m_fFalloffExponent = p_fExponent;
}
//----------------------------------------------------------------------------------------------
bool Light::operator==(const Light& p_light) const
{
	return m_strId.Compare(p_light.m_strId) == 0 &&
		m_bIsLightEnabled == p_light.m_bIsLightEnabled &&
		m_eLightType == p_light.m_eLightType &&

		Mathf::Equals(m_fRange, p_light.m_fRange) &&
		Mathf::Equals(m_fIntensity, p_light.m_fIntensity) &&
		Mathf::Equals(m_fConstantAttenuation, p_light.m_fConstantAttenuation) &&
		Mathf::Equals(m_fLinearAttenuation, p_light.m_fLinearAttenuation) &&
		Mathf::Equals(m_fQuadraticAttenuation, p_light.m_fQuadraticAttenuation) &&
		Mathf::Equals(m_fCutoffAngle, p_light.m_fCutoffAngle) &&
		Mathf::Equals(m_fFalloffAngle, p_light.m_fFalloffAngle) &&
		Mathf::Equals(m_fFalloffExponent, p_light.m_fFalloffExponent) &&

		m_ambientColour == p_light.m_ambientColour &&
		m_diffuseColour == p_light.m_diffuseColour &&
		m_specularColour == p_light.m_specularColour &&

		m_vec3Position == p_light.m_vec3Position &&
		m_vec3Right	== p_light.m_vec3Right &&
		m_vec3Up == p_light.m_vec3Up &&
		m_vec3Direction == p_light.m_vec3Direction;
}
//----------------------------------------------------------------------------------------------
const Light& Light::operator=(const Light& p_light)
{
	m_strId = p_light.m_strId;

	m_eLightType = p_light.m_eLightType;
	m_bIsLightEnabled = p_light.m_bIsLightEnabled;

	m_ambientColour = p_light.m_ambientColour;
	m_diffuseColour = p_light.m_diffuseColour;
	m_specularColour = p_light.m_specularColour;
	
	m_vec3Position = p_light.m_vec3Position;
	m_vec3Right = p_light.m_vec3Right;
	m_vec3Up = p_light.m_vec3Up;
	m_vec3Direction = p_light.m_vec3Direction;
	
	m_fRange = p_light.m_fRange;
	m_fIntensity = p_light.m_fIntensity;
	
	m_fConstantAttenuation = p_light.m_fConstantAttenuation;
	m_fLinearAttenuation = p_light.m_fLinearAttenuation;
	m_fQuadraticAttenuation = p_light.m_fQuadraticAttenuation;
	
	m_fCutoffAngle = p_light.m_fCutoffAngle;
	m_fFalloffAngle = p_light.m_fFalloffAngle;
	m_fFalloffExponent = p_light.m_fFalloffExponent;

	return *this;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
