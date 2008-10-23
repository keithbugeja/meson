#pragma once

#include "GrvGravitasEntity.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;
using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Instrumentation;

PropertyDescriptor::PropertyDescriptor(void)
	: Name()
	, Type(PropertyType::Integer)
	, ReadOnly(false)
{
}

PropertyDescriptor::PropertyDescriptor(
	const PropertyDescriptor &p_propertyDescriptor)
	: Name(p_propertyDescriptor.Name)
	, Type(p_propertyDescriptor.Type)
	, ReadOnly(p_propertyDescriptor.ReadOnly)
{
}

PropertyDescriptor::PropertyDescriptor(
	const String& p_strName, PropertyType::PropertyType p_propertyType,
	bool p_bReadOnly)
	: Name(p_strName)
	, Type(p_propertyType)
	, ReadOnly(p_bReadOnly)
{
}

bool PropertyDescriptor::IsSimpleProperty(void) const
{
	return Type == PropertyType::Boolean
		|| Type == PropertyType::Integer
		|| Type == PropertyType::Real
		|| Type == PropertyType::String
		|| Type == PropertyType::Vector
		|| Type == PropertyType::Point;
}

bool PropertyDescriptor::IsIndexedProperty(void) const
{
	return Type == PropertyType::IndexedBoolean
		|| Type == PropertyType::IndexedInteger
		|| Type == PropertyType::IndexedReal
		|| Type == PropertyType::IndexedString
		|| Type == PropertyType::IndexedVector
		|| Type == PropertyType::IndexedPoint; 
}

bool PropertyDescriptor::IsListProperty(void) const
{
	return Type == PropertyType::BooleanList
		|| Type == PropertyType::IntegerList 
		|| Type == PropertyType::RealList
		|| Type == PropertyType::StringList
		|| Type == PropertyType::VectorList
		|| Type == PropertyType::PointList;
}

bool PropertyDescriptor::operator==(
	const PropertyDescriptor& p_propertyDescriptor) const
{
	return Name == p_propertyDescriptor.Name;
}

Meson_Gravitas_BEGIN

struct IGravitasEntityState
{
	String Id;

	IGravitasEntityState(void)
		: Id(GravitasEngine::GetInstance()->GenerateGuid())	{ }

	IGravitasEntityState(const String& p_strId) : Id(p_strId) { }
};

Meson_Gravitas_END

IGravitasEntity::IGravitasEntity(void)
	: m_pGravitasEntityState(NULL)
{
	m_pGravitasEntityState = new IGravitasEntityState();
}

IGravitasEntity::IGravitasEntity(const String& p_strId)
	: m_pGravitasEntityState(NULL)
{
	m_pGravitasEntityState = new IGravitasEntityState(p_strId);
}

IGravitasEntity::~IGravitasEntity(void)
{
	delete m_pGravitasEntityState;
}

const String& IGravitasEntity::GetId(void) const
{
	return m_pGravitasEntityState->Id;
};

// property meta-data
void IGravitasEntity::EnumerateProperties(
	TMap<String, PropertyDescriptor>& p_mapProperties) const
{
	p_mapProperties.Clear();
};

// simple property queries
void IGravitasEntity::GetProperty(
	const String& p_strName, bool& p_bValue) const { }

void IGravitasEntity::GetProperty(const String& p_strName,
	int& p_nValue) const { }

void IGravitasEntity::GetProperty(const String& p_strName,
	Real& p_rValue) const { }

void IGravitasEntity::GetProperty(const String& p_strName,
	String& p_strValue) const { };

void IGravitasEntity::GetProperty(const String& p_strName,
	TVector3<Real>& p_vecValue) const { };

void IGravitasEntity::GetProperty(const String& p_strName,
	TPoint3<Real>& p_ptValue) const { };

// indexed property queries
void IGravitasEntity::GetProperty(const String& p_strName,
	size_t p_unIndex, bool& p_bValue) const { };

void IGravitasEntity::GetProperty(const String& p_strName,
	size_t p_unIndex, int& p_nValue) const { };

void IGravitasEntity::GetProperty(const String& p_strName,
	size_t p_unIndex, Real& p_rValue) const { };

void IGravitasEntity::GetProperty(const String& p_strName,
	size_t p_unIndex, String& p_strValue) const { };

void IGravitasEntity::GetProperty(const String& p_strName,
	size_t p_unIndex, TVector3<Real>& p_vecValue) const { };

void IGravitasEntity::GetProperty(const String& p_strName,
	size_t p_unIndex, TPoint3<Real>& p_ptValue) const { };

// list property queries
void IGravitasEntity::GetProperty(const String& p_strName,
	TList<bool>& p_listValues) const { };

void IGravitasEntity::GetProperty(const String& p_strName,
	TList<int>& p_listValues) const { };

void IGravitasEntity::GetProperty(const String& p_strName,
	TList<Real>& p_listValues) const { };

void IGravitasEntity::GetProperty(const String& p_strName,
	TList<String>& p_listValues) const { };

void IGravitasEntity::GetProperty(const String& p_strName,
	VectorList& p_listValues) const { };

void IGravitasEntity::GetProperty(const String& p_strName,
	PointList& p_listValues) const { };

// simple property updates
void IGravitasEntity::SetProperty(const String& p_strName,
	bool p_bValue) { };

void IGravitasEntity::SetProperty(const String& p_strName,
	int p_nValue) { };

void IGravitasEntity::SetProperty(const String& p_strName,
	Real p_rValue) { };

void IGravitasEntity::SetProperty(const String& p_strName,
	const String& p_strValue) { };

void IGravitasEntity::SetProperty(const String& p_strName,
	const TVector3<Real>& p_vecValue) { };

void IGravitasEntity::SetProperty(const String& p_strName,
	const TPoint3<Real>& p_ptValue) { };

// indexed property updates
void IGravitasEntity::SetProperty(const String& p_strName,
	size_t p_unIndex, bool p_bValue) { };

void IGravitasEntity::SetProperty(const String& p_strName,
	size_t p_unIndex, int p_nValue) { };

void IGravitasEntity::SetProperty(const String& p_strName,
	size_t p_unIndex, Real p_rValue) { };

void IGravitasEntity::SetProperty(const String& p_strName,
	size_t p_unIndex, const String& p_strValue) { };

void IGravitasEntity::SetProperty(const String& p_strName,
	size_t p_unIndex, const TVector3<Real>& p_vecValue) { };

void IGravitasEntity::SetProperty(const String& p_strName,
	size_t p_unIndex, const TPoint3<Real>& p_ptValue) { };

// list property updates
void IGravitasEntity::SetProperty(const String& p_strName,
	const TList<bool>& p_listValues) { };

void IGravitasEntity::SetProperty(const String& p_strName,
	const TList<int>& p_listValues) { };

void IGravitasEntity::SetProperty(const String& p_strName,
	const TList<Real>& p_listValues) { };

void IGravitasEntity::SetProperty(const String& p_strName,
	const TList<String>& p_listValues) { };

void IGravitasEntity::SetProperty(const String& p_strName,
	const VectorList& p_listValues) { };

void IGravitasEntity::SetProperty(const String& p_strName,
	const PointList& p_listValues) { };

// instrumentation
void IGravitasEntity::RenderInstrumentation(
	IInstrumentationDevice* p_pInstrumentationDevice) { }
