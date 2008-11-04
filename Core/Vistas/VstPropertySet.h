#pragma once
#include "Types.h"
#include "HashMap.h"
#include "StringMap.h"
#include "Vector4.h"
#include "Matrix4.h"

#include "VstTypes.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class Property;

	class VISTAS_API PropertyDescriptor
	{
	public:
		enum PropertyType
		{
			PT_BOOLEAN,
			PT_INTEGER,
			PT_REAL,
			PT_STRING,
			PT_VECTOR2,
			PT_VECTOR3,
			PT_VECTOR4,
			PT_MATRIX2,
			PT_MATRIX3,
			PT_MATRIX4,
			PT_COLOUR,
			PT_COUNT
		};

		enum PropertyClass
		{
			PC_SCALAR,
			PC_INDEXED,
			PC_ARRAY,
			PC_COUNT
		};

	public:
		PropertyDescriptor(void);
		PropertyDescriptor(const PropertyDescriptor& p_propertyDescriptor);
		PropertyDescriptor(const Meson::Common::Text::String& p_strPropertyName, 
			PropertyType p_ePropertyType, PropertyClass p_ePropertyClass, bool p_bIsLocked);

		const Meson::Common::Text::String& GetName(void) const;
		PropertyType GetType(void) const;
		PropertyClass GetClass(void) const;
		bool IsLocked(void) const;

		PropertyDescriptor& operator=(const PropertyDescriptor& p_propertyDescriptor);
		bool operator==(const PropertyDescriptor& p_propertyDescriptor) const;

	protected:
		Meson::Common::Text::String m_strPropertyName;

		PropertyType m_ePropertyType;
		PropertyClass m_ePropertyClass;
		
		bool m_bIsLocked;
	};

	//----------------------------------------------------------------------------------------------
	class VISTAS_API Property : public PropertyDescriptor
	{
	public:
		Property(void);
		Property(const Property& p_property);
		Property(const Meson::Common::Text::String& p_strPropertyName, 
			PropertyType p_ePropertyType, PropertyClass p_ePropertyClass, bool p_bIsLocked);
		
		~Property(void);

		void SetProperty(const Meson::Common::Text::String& p_strPropertyName, 
			PropertyType p_ePropertyType, PropertyClass p_ePropertyClass, bool p_bIsLocked);

		void SetName(const Meson::Common::Text::String& p_strPropertyName);
		void SetType(PropertyType p_ePropertyType);
		void SetClass(PropertyClass p_ePropertyClass);
		void SetLocked(bool p_bIsLocked);
		
		Property& operator=(const Property& p_property);
		bool operator==(const Property& p_property) const;
	public:
		union
		{
			void* ArrayListPointer;
			bool  BooleanValue;
			int	  IntegerValue;
			float FloatArray[Meson::Common::Maths::Matrix4f::Order];
		};

		Meson::Common::Text::String StringValue;
	};

	//----------------------------------------------------------------------------------------------
	class VISTAS_API IImmutablePropertySet
	{
	public:
		virtual bool ContainsProperty(const Meson::Common::Text::String& p_strPropertyName);
		virtual void EnumerateProperties(Meson::Common::Collections::THashMap<Meson::Common::Text::String, PropertyDescriptor> p_propertyDescriptorMap);
		virtual void GetPropertyDescriptor(const Meson::Common::Text::String& p_strPropertyName, PropertyDescriptor& p_propertyDescriptor);

		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, bool& p_bValue) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, int& p_nValue) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, float& p_fValue) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Colour& p_colour) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Maths::Vector2f& p_vector2) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Maths::Vector3f& p_vector3) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Maths::Vector4f& p_vector4) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Maths::Matrix2f& p_matrix2) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Maths::Matrix3f& p_matrix3) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Maths::Matrix4f& p_matrix4) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Text::String& p_string) const;
	};

	//----------------------------------------------------------------------------------------------
	class VISTAS_API IPropertySet : public IImmutablePropertySet
	{
	public:
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, bool p_bValue);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, int p_nValue);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, float p_fValue);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Colour& p_colour);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Vector2f& p_vector2);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Vector3f& p_vector3);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Vector4f& p_vector4);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Matrix2f& p_matrix2);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Matrix3f& p_matrix3);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Matrix4f& p_matrix4);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Text::String& p_string);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const char* p_szString);
	};

	//----------------------------------------------------------------------------------------------
	class VISTAS_API PropertySet : public IPropertySet
	{
	public:
		virtual void GetPropertyDescriptor(const Meson::Common::Text::String& p_strPropertyName, PropertyDescriptor& p_propertyDescriptor);
		virtual void EnumerateProperties(Meson::Common::Collections::THashMap<Meson::Common::Text::String, PropertyDescriptor> p_propertyDescriptorMap);

		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, bool& p_bValue) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, int& p_nValue) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, float& p_fValue) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Colour& p_colour) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Maths::Vector2f& p_vector2) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Maths::Vector3f& p_vector3) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Maths::Vector4f& p_vector4) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Maths::Matrix2f& p_matrix2) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Maths::Matrix3f& p_matrix3) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Maths::Matrix4f& p_matrix4) const;
		virtual void GetProperty(const Meson::Common::Text::String& p_strPropertyName, Meson::Common::Text::String& p_string) const;

		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, bool p_bValue);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, int p_nValue);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, float p_fValue);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Colour& p_colour);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Vector2f& p_vector2);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Vector3f& p_vector3);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Vector4f& p_vector4);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Matrix2f& p_matrix2);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Matrix3f& p_matrix3);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Matrix4f& p_matrix4);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Text::String& p_string);
		virtual void SetProperty(const Meson::Common::Text::String& p_strPropertyName, const char* p_szString);

		virtual void CreateProperty(const Meson::Common::Text::String& p_strPropertyName, bool p_bValue, bool p_bIsLocked=false);
		virtual void CreateProperty(const Meson::Common::Text::String& p_strPropertyName, int p_nValue, bool p_bIsLocked=false);
		virtual void CreateProperty(const Meson::Common::Text::String& p_strPropertyName, float p_fValue, bool p_bIsLocked=false);
		virtual void CreateProperty(const Meson::Common::Text::String& p_strPropertyName, const Colour& p_colour, bool p_bIsLocked=false);
		virtual void CreateProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Vector2f& p_vector2, bool p_bIsLocked=false);
		virtual void CreateProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Vector3f& p_vector3, bool p_bIsLocked=false);
		virtual void CreateProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Vector4f& p_vec4Vector, bool p_bIsLocked=false);
		virtual void CreateProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Matrix2f& p_matrix2, bool p_bIsLocked=false);
		virtual void CreateProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Matrix3f& p_matrix3, bool p_bIsLocked=false);
		virtual void CreateProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Maths::Matrix4f& p_matrix4, bool p_bIsLocked=false);
		virtual void CreateProperty(const Meson::Common::Text::String& p_strPropertyName, const Meson::Common::Text::String& p_string, bool p_bIsLocked=false);
		virtual void CreateProperty(const Meson::Common::Text::String& p_strPropertyName, const char* p_szString, bool p_bIsLocked=false);

		virtual bool ContainsProperty(const Meson::Common::Text::String& p_strPropertyName);
		virtual bool ContainsProperty(const Meson::Common::Text::String& p_strPropertyName, Property& p_property);

		virtual void DestroyProperty(const Meson::Common::Text::String& p_strPropertyName);
		virtual void DestroyAllProperties(void);

	protected:
		Meson::Common::Collections::TStringMap<Property> m_propertyMap;
	};
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END
