//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "CylinderFactory.h"
#include "VstMeshForge.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
CylinderFactory::CylinderFactory(void)
	: m_strName("Cylinder")
{
	m_propertySet.CreateProperty("Radius", 1.0f);
	m_propertySet.CreateProperty("Height", 2.0f);
	m_propertySet.CreateProperty("Segments", 32);
	m_propertySet.CreateProperty("Caps", true);
	
	m_propertySet.CreateProperty("InvertNormals", false);
	
	m_propertySet.CreateProperty("TextureCoordinates", true);
	m_propertySet.CreateProperty("Binormals", true);
	m_propertySet.CreateProperty("Tangents", true);
	m_propertySet.CreateProperty("Normals", true);
	m_propertySet.CreateProperty("Colour", true);
}
//----------------------------------------------------------------------------------------------
CylinderFactory::~CylinderFactory(void)
{
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& CylinderFactory::GetName(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
IPropertySet* CylinderFactory::GetPropertySet(void)
{
	return (IPropertySet*)&m_propertySet;
}
//----------------------------------------------------------------------------------------------
GeometryPtr CylinderFactory::CreateGeometry(void)
{
	float fRadius,
		  fHeight;

	int nSegments;

	bool bCaps,
		 bInvertNormals,
		 bMeshFlags;

	m_propertySet.GetProperty("Radius", fRadius);
	m_propertySet.GetProperty("Height", fHeight);
	m_propertySet.GetProperty("Segments", nSegments);
	m_propertySet.GetProperty("Caps", bCaps);

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

	return MeshForge::CreateCylinderMesh(fRadius, fHeight, nSegments, bCaps, bInvertNormals, dwMeshFlags);
}
//----------------------------------------------------------------------------------------------
GeometryPtr CylinderFactory::CreateGeometry(const Meson::Common::Text::String& p_strId)
{
	GeometryPtr pGeometry = CreateGeometry();
	pGeometry->SetId(p_strId);

	return pGeometry;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END