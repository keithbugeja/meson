//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "SphereFactory.h"
#include "VstMeshForge.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
SphereFactory::SphereFactory(void)
	: m_strName("Sphere")
{
	m_propertySet.CreateProperty("Radius", 1.0f);
	m_propertySet.CreateProperty("Segments", 32);

	m_propertySet.CreateProperty("InvertNormals", false);
	
	m_propertySet.CreateProperty("TextureCoordinates", true);
	m_propertySet.CreateProperty("Binormals", true);
	m_propertySet.CreateProperty("Tangents", true);
	m_propertySet.CreateProperty("Normals", true);
	m_propertySet.CreateProperty("Colour", true);
}
//----------------------------------------------------------------------------------------------
SphereFactory::~SphereFactory(void)
{
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& SphereFactory::GetName(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
IPropertySet* SphereFactory::GetPropertySet(void)
{
	return (IPropertySet*)&m_propertySet;
}
//----------------------------------------------------------------------------------------------
GeometryPtr SphereFactory::CreateGeometry(void)
{
	float fRadius;
	int nSegments;

	bool bInvertNormals,
		 bMeshFlags;

	m_propertySet.GetProperty("Radius", fRadius);
	m_propertySet.GetProperty("Segments", nSegments);

	m_propertySet.GetProperty("InvertNormals", bInvertNormals);

	dword dwMeshFlags = MeshForge::MF_NONE;

	m_propertySet.GetProperty("TextureCoordinates", bMeshFlags);
	if (bMeshFlags) dwMeshFlags |= MeshForge::MF_TEXCOORDS;

	m_propertySet.GetProperty("Binormals", bMeshFlags);
	if (bMeshFlags) dwMeshFlags |= MeshForge::MF_BINORMALS;

	m_propertySet.GetProperty("Tangents", bMeshFlags);
	if (bMeshFlags) dwMeshFlags |= MeshForge::MF_TANGENTS;

	m_propertySet.GetProperty("Normals", bMeshFlags);
	if (bMeshFlags) dwMeshFlags |= MeshForge::MF_NORMALS;

	m_propertySet.GetProperty("Colour", bMeshFlags);
	if (bMeshFlags) dwMeshFlags |= MeshForge::MF_COLOUR;

	return MeshForge::CreateSphereMesh(fRadius, nSegments, bInvertNormals, dwMeshFlags);
}
//----------------------------------------------------------------------------------------------
GeometryPtr SphereFactory::CreateGeometry(const Meson::Common::Text::String& p_strId)
{
	GeometryPtr pGeometry = CreateGeometry();
	pGeometry->SetId(p_strId);

	return pGeometry;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END