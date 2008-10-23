#include "Matrix4.h"
#include "Vector4.h"

#include "VstPropertySet.h"
#include "VstColour.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
/** PropertyDescriptor class
 */
//----------------------------------------------------------------------------------------------
PropertyDescriptor::PropertyDescriptor(void) { }
//----------------------------------------------------------------------------------------------
PropertyDescriptor::PropertyDescriptor(const PropertyDescriptor& p_propertyDescriptor)
	: m_strPropertyName(p_propertyDescriptor.m_strPropertyName)
	, m_ePropertyType(p_propertyDescriptor.m_ePropertyType)
	, m_ePropertyClass(p_propertyDescriptor.m_ePropertyClass)
	, m_bIsLocked(p_propertyDescriptor.m_bIsLocked)
{
}
//----------------------------------------------------------------------------------------------
PropertyDescriptor::PropertyDescriptor(const String& p_strPropertyName, PropertyType p_ePropertyType, 
									   PropertyClass p_ePropertyClass, bool p_bIsLocked)
	: m_strPropertyName(p_strPropertyName)
	, m_ePropertyType(p_ePropertyType)
	, m_ePropertyClass(p_ePropertyClass)
	, m_bIsLocked(p_bIsLocked)
{
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& PropertyDescriptor::GetName(void) const
{
	return m_strPropertyName;
}
//----------------------------------------------------------------------------------------------
PropertyDescriptor::PropertyType PropertyDescriptor::GetType(void) const
{	
	return m_ePropertyType;
}
//----------------------------------------------------------------------------------------------
PropertyDescriptor::PropertyClass PropertyDescriptor::GetClass(void) const
{
	return m_ePropertyClass;
}
//----------------------------------------------------------------------------------------------
bool PropertyDescriptor::IsLocked(void) const
{
	return m_bIsLocked;
}
//----------------------------------------------------------------------------------------------
PropertyDescriptor& PropertyDescriptor::operator=(const PropertyDescriptor& p_propertyDescriptor)
{
	m_strPropertyName = p_propertyDescriptor.m_strPropertyName;
	m_ePropertyType = p_propertyDescriptor.m_ePropertyType;
	m_ePropertyClass = p_propertyDescriptor.m_ePropertyClass;
	m_bIsLocked = p_propertyDescriptor.m_bIsLocked;

	return *this;
}
//----------------------------------------------------------------------------------------------
bool PropertyDescriptor::operator==(const PropertyDescriptor& p_propertyDescriptor) const
{
	return m_ePropertyType == p_propertyDescriptor.m_ePropertyType &&
		m_ePropertyClass == p_propertyDescriptor.m_ePropertyClass &&
		m_bIsLocked == p_propertyDescriptor.m_bIsLocked &&
		m_strPropertyName.Compare(p_propertyDescriptor.m_strPropertyName) == 0;
}
//----------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------
/** Property class
 */
//----------------------------------------------------------------------------------------------
Property::Property(void) { }
//----------------------------------------------------------------------------------------------
Property::Property(const Property& p_property)
	: PropertyDescriptor((PropertyDescriptor)p_property)
{
	memcpy(FloatArray, p_property.FloatArray, sizeof(FloatArray));
}
//----------------------------------------------------------------------------------------------
Property::Property(const String& p_strPropertyName, PropertyType p_ePropertyType, 
				   PropertyClass p_ePropertyClass, bool p_bIsLocked)
	: PropertyDescriptor(p_strPropertyName, p_ePropertyType, p_ePropertyClass, p_bIsLocked)
{ 
}
//----------------------------------------------------------------------------------------------
Property::~Property(void)
{
}
//----------------------------------------------------------------------------------------------
void Property::SetProperty(const String& p_strPropertyName, PropertyType p_ePropertyType, 
						   PropertyClass p_ePropertyClass, bool p_bIsLocked)
{
	m_strPropertyName = p_strPropertyName;
	m_ePropertyType = p_ePropertyType;
	m_ePropertyClass = p_ePropertyClass;
	m_bIsLocked = p_bIsLocked;
}
//----------------------------------------------------------------------------------------------
void Property::SetName(const String& p_strPropertyName)
{
	m_strPropertyName = p_strPropertyName;
}
//----------------------------------------------------------------------------------------------
void Property::SetType(PropertyType p_ePropertyType)
{
	m_ePropertyType = p_ePropertyType;
}
//----------------------------------------------------------------------------------------------
void Property::SetClass(PropertyClass p_ePropertyClass)
{
	m_ePropertyClass = p_ePropertyClass;
}
//----------------------------------------------------------------------------------------------
void Property::SetLocked(bool p_bIsLocked)
{
	m_bIsLocked = p_bIsLocked;
}
//----------------------------------------------------------------------------------------------
Property& Property::operator=(const Property& p_property)
{
	*((PropertyDescriptor*)this) = ((PropertyDescriptor)(p_property));
	memcpy(FloatArray, p_property.FloatArray, sizeof(FloatArray));
	
	return *this;
}
//----------------------------------------------------------------------------------------------
bool Property::operator==(const Property& p_property) const
{
	return (p_property.m_strPropertyName.Compare(m_strPropertyName) == 0);
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
/** IImmutablePropertySet class
 */
//----------------------------------------------------------------------------------------------
bool IImmutablePropertySet::ContainsProperty(const String& p_strPropertyName) { return false; }
//----------------------------------------------------------------------------------------------
void IImmutablePropertySet::EnumerateProperties(THashMap<String, PropertyDescriptor> p_propertyDescriptorMap) { }
//----------------------------------------------------------------------------------------------
void IImmutablePropertySet::GetPropertyDescriptor(const String& p_strPropertyName, PropertyDescriptor& p_propertyDescriptor) { }
//----------------------------------------------------------------------------------------------
void IImmutablePropertySet::GetProperty(const String& p_strPropertyName, bool& p_bValue) const { }
//----------------------------------------------------------------------------------------------
void IImmutablePropertySet::GetProperty(const String& p_strPropertyName, int& p_nValue) const { }
//----------------------------------------------------------------------------------------------
void IImmutablePropertySet::GetProperty(const String& p_strPropertyName, float& p_fValue) const { }
//----------------------------------------------------------------------------------------------
void IImmutablePropertySet::GetProperty(const String& p_strPropertyName, Colour& p_colour) const { }
//----------------------------------------------------------------------------------------------
void IImmutablePropertySet::GetProperty(const String& p_strPropertyName, Vector2f& p_vec2Vector) const { }
//----------------------------------------------------------------------------------------------
void IImmutablePropertySet::GetProperty(const String& p_strPropertyName, Vector3f& p_vec3Vector) const { }
//----------------------------------------------------------------------------------------------
void IImmutablePropertySet::GetProperty(const String& p_strPropertyName, Vector4f& p_vec4Vector) const { }
//----------------------------------------------------------------------------------------------
void IImmutablePropertySet::GetProperty(const String& p_strPropertyName, Matrix2f& p_mat2Matrix) const { }
//----------------------------------------------------------------------------------------------
void IImmutablePropertySet::GetProperty(const String& p_strPropertyName, Matrix3f& p_mat3Matrix) const { }
//----------------------------------------------------------------------------------------------
void IImmutablePropertySet::GetProperty(const String& p_strPropertyName, Matrix4f& p_mat4Matrix) const { }
//----------------------------------------------------------------------------------------------
void IImmutablePropertySet::GetProperty(const String& p_strPropertyName, String& p_string) const { }


//----------------------------------------------------------------------------------------------
/** IPropertySet class
 */
//----------------------------------------------------------------------------------------------
void IPropertySet::SetProperty(const String& p_strPropertyName, bool p_bValue) { }
//----------------------------------------------------------------------------------------------
void IPropertySet::SetProperty(const String& p_strPropertyName, int p_nValue) { }
//----------------------------------------------------------------------------------------------
void IPropertySet::SetProperty(const String& p_strPropertyName, float p_fValue) { }
//----------------------------------------------------------------------------------------------
void IPropertySet::SetProperty(const String& p_strPropertyName, const Colour& p_colour) { }
//----------------------------------------------------------------------------------------------
void IPropertySet::SetProperty(const String& p_strPropertyName, const Vector2f& p_vec2Vector) { }
//----------------------------------------------------------------------------------------------
void IPropertySet::SetProperty(const String& p_strPropertyName, const Vector3f& p_vec3Vector) { }
//----------------------------------------------------------------------------------------------
void IPropertySet::SetProperty(const String& p_strPropertyName, const Vector4f& p_vec4Vector) { }
//----------------------------------------------------------------------------------------------
void IPropertySet::SetProperty(const String& p_strPropertyName, const Matrix2f& p_mat2Matrix) { }
//----------------------------------------------------------------------------------------------
void IPropertySet::SetProperty(const String& p_strPropertyName, const Matrix3f& p_mat3Matrix) { }
//----------------------------------------------------------------------------------------------
void IPropertySet::SetProperty(const String& p_strPropertyName, const Matrix4f& p_mat4Matrix) { }
//----------------------------------------------------------------------------------------------
void IPropertySet::SetProperty(const String& p_strPropertyName, const String& p_string) { }
//----------------------------------------------------------------------------------------------
void IPropertySet::SetProperty(const String& p_strPropertyName, const char* p_szString) { }


//----------------------------------------------------------------------------------------------
/** PropertySet class
 */
//----------------------------------------------------------------------------------------------
void PropertySet::GetPropertyDescriptor(const String& p_strPropertyName, PropertyDescriptor& p_propertyDescriptor) 
{
	if (!PropertySet::ContainsProperty(p_strPropertyName))
		throw new MesonException("Property set does not contain specified property!", __FILE__, __LINE__ );

	p_propertyDescriptor = m_propertyMap[p_strPropertyName];
}
//----------------------------------------------------------------------------------------------
void PropertySet::EnumerateProperties(THashMap<String, PropertyDescriptor> p_propertyDescriptorMap)
{
	p_propertyDescriptorMap.Clear();

	TEnumerator<Property>& propertyEnumerator = m_propertyMap.GetValueEnumerator();
	while (propertyEnumerator.HasMoreElements())
	{
		PropertyDescriptor& descriptor = (PropertyDescriptor)propertyEnumerator.NextElement();
		p_propertyDescriptorMap.Insert(descriptor.GetName(), (PropertyDescriptor)descriptor);
	}
}
//----------------------------------------------------------------------------------------------
void PropertySet::SetProperty(const String& p_strPropertyName, bool p_bValue) 
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		m_propertyMap[p_strPropertyName].BooleanValue = p_bValue;
}
//----------------------------------------------------------------------------------------------
void PropertySet::SetProperty(const String& p_strPropertyName, int p_nValue) 
{ 
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		m_propertyMap[p_strPropertyName].IntegerValue = p_nValue;
}
//----------------------------------------------------------------------------------------------
void PropertySet::SetProperty(const String& p_strPropertyName, float p_fValue) 
{ 
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		m_propertyMap[p_strPropertyName].FloatArray[0] = p_fValue;
}
//----------------------------------------------------------------------------------------------
void PropertySet::SetProperty(const String& p_strPropertyName, const Colour& p_colour) 
{ 
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		*((Colour*)m_propertyMap[p_strPropertyName].FloatArray) = p_colour;
}
//----------------------------------------------------------------------------------------------
void PropertySet::SetProperty(const String& p_strPropertyName, const Vector2f& p_vector2) 
{ 
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		*((Vector2f*)m_propertyMap[p_strPropertyName].FloatArray) = p_vector2;
}
//----------------------------------------------------------------------------------------------
void PropertySet::SetProperty(const String& p_strPropertyName, const Vector3f& p_vector3) 
{ 
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		*((Vector3f*)m_propertyMap[p_strPropertyName].FloatArray) = p_vector3;
}
//----------------------------------------------------------------------------------------------
void PropertySet::SetProperty(const String& p_strPropertyName, const Vector4f& p_vector4) 
{ 
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		*((Vector4f*)m_propertyMap[p_strPropertyName].FloatArray) = p_vector4;
}
//----------------------------------------------------------------------------------------------
void PropertySet::SetProperty(const String& p_strPropertyName, const Matrix2f& p_matrix2) 
{ 
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		*((Matrix2f*)m_propertyMap[p_strPropertyName].FloatArray) = p_matrix2;
}
//----------------------------------------------------------------------------------------------
void PropertySet::SetProperty(const String& p_strPropertyName, const Matrix3f& p_matrix3) 
{ 
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		*((Matrix3f*)m_propertyMap[p_strPropertyName].FloatArray) = p_matrix3;
}
//----------------------------------------------------------------------------------------------
void PropertySet::SetProperty(const String& p_strPropertyName, const Matrix4f& p_matrix4) 
{ 
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		*((Matrix4f*)m_propertyMap[p_strPropertyName].FloatArray) = p_matrix4;
}
//----------------------------------------------------------------------------------------------
void PropertySet::SetProperty(const String& p_strPropertyName, const String& p_string) 
{ 
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		m_propertyMap[p_strPropertyName].StringValue = p_string;
}
//----------------------------------------------------------------------------------------------
void PropertySet::SetProperty(const String& p_strPropertyName, const char* p_szString)
{
	SetProperty(p_strPropertyName, String(p_szString));
}
//----------------------------------------------------------------------------------------------
void PropertySet::GetProperty(const String& p_strPropertyName, bool& p_bValue) const 
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		p_bValue = m_propertyMap.ValueAt(p_strPropertyName).BooleanValue;
}
//----------------------------------------------------------------------------------------------
void PropertySet::GetProperty(const String& p_strPropertyName, int& p_nValue) const 
{ 
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		p_nValue = m_propertyMap.ValueAt(p_strPropertyName).IntegerValue;
}
//----------------------------------------------------------------------------------------------
void PropertySet::GetProperty(const String& p_strPropertyName, float& p_fValue) const 
{ 
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		p_fValue = m_propertyMap.ValueAt(p_strPropertyName).FloatArray[0];
}
//----------------------------------------------------------------------------------------------
void PropertySet::GetProperty(const String& p_strPropertyName, Colour& p_colour) const 
{ 
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		p_colour = *((Colour*)m_propertyMap.ValueAt(p_strPropertyName).FloatArray);
}
//----------------------------------------------------------------------------------------------
void PropertySet::GetProperty(const String& p_strPropertyName, Vector2f& p_vector2) const 
{ 
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		p_vector2 = *((Vector2f*)m_propertyMap.ValueAt(p_strPropertyName).FloatArray);
}
//----------------------------------------------------------------------------------------------
void PropertySet::GetProperty(const String& p_strPropertyName, Vector3f& p_vector3) const 
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		p_vector3 = *((Vector3f*)m_propertyMap.ValueAt(p_strPropertyName).FloatArray);
}
//----------------------------------------------------------------------------------------------
void PropertySet::GetProperty(const String& p_strPropertyName, Vector4f& p_vector4) const 
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		p_vector4 = *((Vector4f*)m_propertyMap.ValueAt(p_strPropertyName).FloatArray);
}
//----------------------------------------------------------------------------------------------
void PropertySet::GetProperty(const String& p_strPropertyName, Matrix2f& p_matrix2) const 
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		p_matrix2 = *((Matrix2f*)m_propertyMap.ValueAt(p_strPropertyName).FloatArray);
}
//----------------------------------------------------------------------------------------------
void PropertySet::GetProperty(const String& p_strPropertyName, Matrix3f& p_matrix3) const 
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		p_matrix3 = *((Matrix3f*)m_propertyMap.ValueAt(p_strPropertyName).FloatArray);
}
//----------------------------------------------------------------------------------------------
void PropertySet::GetProperty(const String& p_strPropertyName, Matrix4f& p_matrix4) const 
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		p_matrix4 = *((Matrix4f*)m_propertyMap.ValueAt(p_strPropertyName).FloatArray);
}
//----------------------------------------------------------------------------------------------
void PropertySet::GetProperty(const String& p_strPropertyName, String& p_string) const 
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		p_string = m_propertyMap.ValueAt(p_strPropertyName).StringValue;
}
//----------------------------------------------------------------------------------------------
void PropertySet::CreateProperty(const String& p_strPropertyName, bool p_bValue, bool p_bIsLocked)
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		throw new MesonException("Property already exists!", __FILE__, __LINE__);

	Property prop(p_strPropertyName, Property::PT_BOOLEAN, Property::PC_SCALAR, p_bIsLocked);
	prop.BooleanValue = p_bValue;

	m_propertyMap.Insert(p_strPropertyName, prop);
}
//----------------------------------------------------------------------------------------------
void PropertySet::CreateProperty(const String& p_strPropertyName, int p_nValue, bool p_bIsLocked)
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		throw new MesonException("Property already exists!", __FILE__, __LINE__);

	Property prop(p_strPropertyName, Property::PT_INTEGER, Property::PC_SCALAR, p_bIsLocked);
	prop.IntegerValue = p_nValue;

	m_propertyMap.Insert(p_strPropertyName, prop);
}
//----------------------------------------------------------------------------------------------
void PropertySet::CreateProperty(const String& p_strPropertyName, float p_fValue, bool p_bIsLocked)
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		throw new MesonException("Property already exists!", __FILE__, __LINE__);

	Property prop(p_strPropertyName, Property::PT_REAL, Property::PC_SCALAR, p_bIsLocked);
	prop.FloatArray[0] = p_fValue;

	m_propertyMap.Insert(p_strPropertyName, prop);
}
//----------------------------------------------------------------------------------------------
void PropertySet::CreateProperty(const String& p_strPropertyName, const Colour& p_colour, bool p_bIsLocked)
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		throw new MesonException("Property already exists!", __FILE__, __LINE__);

	Property prop(p_strPropertyName, Property::PT_COLOUR, Property::PC_SCALAR, p_bIsLocked);
	*((Colour*)prop.FloatArray) = p_colour;

	m_propertyMap.Insert(p_strPropertyName, prop);
}
//----------------------------------------------------------------------------------------------
void PropertySet::CreateProperty(const String& p_strPropertyName, const Vector2f& p_vector2, bool p_bIsLocked)
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		throw new MesonException("Property already exists!", __FILE__, __LINE__);

	Property prop(p_strPropertyName, Property::PT_VECTOR2, Property::PC_SCALAR, p_bIsLocked);
	*((Vector2f*)prop.FloatArray) = p_vector2;

	m_propertyMap.Insert(p_strPropertyName, prop);
}
//----------------------------------------------------------------------------------------------
void PropertySet::CreateProperty(const String& p_strPropertyName, const Vector3f& p_vector3, bool p_bIsLocked)
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		throw new MesonException("Property already exists!", __FILE__, __LINE__);

	Property prop(p_strPropertyName, Property::PT_VECTOR3, Property::PC_SCALAR, p_bIsLocked);
	*((Vector3f*)prop.FloatArray) = p_vector3;

	m_propertyMap.Insert(p_strPropertyName, prop);
}
//----------------------------------------------------------------------------------------------
void PropertySet::CreateProperty(const String& p_strPropertyName, const Vector4f& p_vector4, bool p_bIsLocked)
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		throw new MesonException("Property already exists!", __FILE__, __LINE__);

	Property prop(p_strPropertyName, Property::PT_VECTOR4, Property::PC_SCALAR, p_bIsLocked);
	*((Vector4f*)prop.FloatArray) = p_vector4;

	m_propertyMap.Insert(p_strPropertyName, prop);
}
//----------------------------------------------------------------------------------------------
void PropertySet::CreateProperty(const String& p_strPropertyName, const Matrix2f& p_matrix2, bool p_bIsLocked)
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		throw new MesonException("Property already exists!", __FILE__, __LINE__);

	Property prop(p_strPropertyName, Property::PT_MATRIX2, Property::PC_SCALAR, p_bIsLocked);
	*((Matrix2f*)prop.FloatArray) = p_matrix2;

	m_propertyMap.Insert(p_strPropertyName, prop);
}
//----------------------------------------------------------------------------------------------
void PropertySet::CreateProperty(const String& p_strPropertyName, const Matrix3f& p_matrix3, bool p_bIsLocked)
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		throw new MesonException("Property already exists!", __FILE__, __LINE__);

	Property prop(p_strPropertyName, Property::PT_MATRIX3, Property::PC_SCALAR, p_bIsLocked);
	*((Matrix3f*)prop.FloatArray) = p_matrix3;

	m_propertyMap.Insert(p_strPropertyName, prop);
}
//----------------------------------------------------------------------------------------------
void PropertySet::CreateProperty(const String& p_strPropertyName, const Matrix4f& p_matrix4, bool p_bIsLocked)
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		throw new MesonException("Property already exists!", __FILE__, __LINE__);

	Property prop(p_strPropertyName, Property::PT_MATRIX4, Property::PC_SCALAR, p_bIsLocked);
	*((Matrix4f*)prop.FloatArray) = p_matrix4;

	m_propertyMap.Insert(p_strPropertyName, prop);
}
//----------------------------------------------------------------------------------------------
void PropertySet::CreateProperty(const String& p_strPropertyName, const String& p_string, bool p_bIsLocked)
{
	if (m_propertyMap.ContainsKey(p_strPropertyName))
		throw new MesonException("Property already exists!", __FILE__, __LINE__);

	Property prop(p_strPropertyName, Property::PT_STRING, Property::PC_SCALAR, p_bIsLocked);
	prop.StringValue = p_string;

	m_propertyMap.Insert(p_strPropertyName, prop);
}
//----------------------------------------------------------------------------------------------
void PropertySet::CreateProperty(const String& p_strPropertyName, const char* p_szString, bool p_bIsLocked)
{
	CreateProperty(p_strPropertyName, String(p_szString), p_bIsLocked);
}
//----------------------------------------------------------------------------------------------
bool PropertySet::ContainsProperty(const String& p_strPropertyName)
{
	return m_propertyMap.ContainsKey(p_strPropertyName);
}
//----------------------------------------------------------------------------------------------
bool PropertySet::ContainsProperty(const String& p_strPropertyName, Property& p_property)
{
	return m_propertyMap.ContainsKey(p_strPropertyName, p_property);
}
//----------------------------------------------------------------------------------------------
void PropertySet::DestroyProperty(const String& p_strPropertyName)
{
	m_propertyMap.RemoveKey(p_strPropertyName);
}
//----------------------------------------------------------------------------------------------
void PropertySet::DestroyAllProperties(void)
{
	m_propertyMap.Clear();
}
//----------------------------------------------------------------------------------------------


Meson_Vistas_END
