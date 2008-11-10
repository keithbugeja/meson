#pragma once

#include "String8.h"
#include "Vector3.h"
#include "List.h"
#include "Map.h"
#include "Pointer.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvInstrumentationDevice.h"

Meson_Gravitas_BEGIN

namespace PropertyType
{
	enum PropertyType
	{
		Boolean,
		Integer,
		Real,
		String,
		Vector,
		IndexedBoolean,
		IndexedInteger,
		IndexedReal,
		IndexedString,
		IndexedVector,
		BooleanList,
		IntegerList,
		RealList,
		StringList,
		VectorList,
	};
}

struct GRAVITAS_API PropertyDescriptor
{
public: // public variables
	Meson::Common::Text::String Name;
	PropertyType::PropertyType Type;
	bool ReadOnly;

public: // public methods
	PropertyDescriptor(void);
	PropertyDescriptor(const PropertyDescriptor& p_propertyDescriptor);
	PropertyDescriptor(const Meson::Common::Text::String& p_strName,
		PropertyType::PropertyType p_propertyType, bool p_bReadOnly);
	bool IsSimpleProperty(void) const;
	bool IsIndexedProperty(void) const;
	bool IsListProperty(void) const;
	bool operator==(const PropertyDescriptor& p_propertyDescriptor) const;
};

struct IGravitasEntityState;

class GRAVITAS_API IGravitasEntity
	: public Meson::Common::TReferenceCounter<IGravitasEntity>
{
private:
	IGravitasEntityState* m_pGravitasEntityState;

public:
	IGravitasEntity(void);
	IGravitasEntity(const Meson::Common::Text::String& p_strId);
	virtual ~IGravitasEntity(void);

	const Meson::Common::Text::String& GetId(void) const;

	// property meta-data
	virtual void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String, PropertyDescriptor>&
			p_mapProperties) const;

	// simple property queries
	virtual void GetProperty(const Meson::Common::Text::String& p_strName, bool& p_bValue) const;
	virtual void GetProperty(const Meson::Common::Text::String& p_strName, int& p_nValue) const;
	virtual void GetProperty(const Meson::Common::Text::String& p_strName, Real& p_rValue) const;
	virtual void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Text::String& p_strValue) const;
	virtual void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Maths::TVector3<Real>& p_vecValue) const;

	// indexed property queries
	virtual void GetProperty(const Meson::Common::Text::String& p_strName, size_t p_unIndex,
		bool& p_bValue) const;
	virtual void GetProperty(const Meson::Common::Text::String& p_strName, size_t p_unIndex,
		int& p_nValue) const;
	virtual void GetProperty(const Meson::Common::Text::String& p_strName, size_t p_unIndex,
		Real& p_rValue) const;
	virtual void GetProperty(const Meson::Common::Text::String& p_strName, size_t p_unIndex,
		Meson::Common::Text::String& p_strValue) const;
	virtual void GetProperty(const Meson::Common::Text::String& p_strName, size_t p_unIndex,
		Meson::Common::Maths::TVector3<Real>& p_vecValue) const;

	// list property queries
	virtual void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Collections::TList<bool>& p_listValues) const;
	virtual void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Collections::TList<int>& p_listValues) const;
	virtual void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Collections::TList<Real>& p_listValues) const;
	virtual void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Collections::TList<Meson::Common::Text::String>& p_listValues) const;
	virtual void GetProperty(const Meson::Common::Text::String& p_strName,
		VectorList& p_listValues) const;

	// simple property updates
	virtual void SetProperty(const Meson::Common::Text::String& p_strName, bool p_bValue);
	virtual void SetProperty(const Meson::Common::Text::String& p_strName, int p_nValue);
	virtual void SetProperty(const Meson::Common::Text::String& p_strName, Real p_rValue);
	virtual void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Text::String& p_strValue);
	virtual void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Maths::TVector3<Real>& p_vecValue);

	// indexed property updates
	virtual void SetProperty(const Meson::Common::Text::String& p_strName,
		size_t p_unIndex, bool p_bValue);
	virtual void SetProperty(const Meson::Common::Text::String& p_strName,
		size_t p_unIndex, int p_nValue);
	virtual void SetProperty(const Meson::Common::Text::String& p_strName,
		size_t p_unIndex, Real p_rValue);
	virtual void SetProperty(const Meson::Common::Text::String& p_strName,
		size_t p_unIndex,
		const Meson::Common::Text::String& p_strValue);
	virtual void SetProperty(const Meson::Common::Text::String& p_strName,
		size_t p_unIndex,
		const Meson::Common::Maths::TVector3<Real>& p_vecValue);

	// list property updates
	virtual void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Collections::TList<bool>& p_listValues);
	virtual void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Collections::TList<int>& p_listValues);
	virtual void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Collections::TList<Real>& p_listValues);
	virtual void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Collections::TList<Meson::Common::Text::String>& p_listValues);
	virtual void SetProperty(const Meson::Common::Text::String& p_strName,
		const VectorList& p_listValues);

	// instrumentation
	virtual void RenderInstrumentation(
		Meson::Gravitas::Instrumentation::IInstrumentationDevice* p_pInstrumentationDevice);
};

Meson_Gravitas_END
