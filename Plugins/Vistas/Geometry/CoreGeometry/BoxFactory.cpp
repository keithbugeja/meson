//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "BoxFactory.h"
#include "VstMeshForge.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
BoxFactory::BoxFactory(void)
	: m_strName("Box")
{
	m_propertySet.CreateProperty("Width", 1.0f);
	m_propertySet.CreateProperty("Height", 1.0f);
	m_propertySet.CreateProperty("Depth", 1.0f);
	
	m_propertySet.CreateProperty("InvertNormals", false);
	
	m_propertySet.CreateProperty("TextureCoordinates", true);
	m_propertySet.CreateProperty("Binormals", true);
	m_propertySet.CreateProperty("Tangents", true);
	m_propertySet.CreateProperty("Normals", true);
	m_propertySet.CreateProperty("Colour", true);
}
//----------------------------------------------------------------------------------------------
BoxFactory::~BoxFactory(void)
{
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& BoxFactory::GetName(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
IPropertySet* BoxFactory::GetPropertySet(void)
{
	return (IPropertySet*)&m_propertySet;
}
//----------------------------------------------------------------------------------------------
GeometryPtr BoxFactory::CreateGeometry(void)
{
	float fWidth, 
		  fHeight, 
		  fDepth; 

	bool bInvertNormals,
		 bMeshFlags;

	m_propertySet.GetProperty("Width", fWidth);
	m_propertySet.GetProperty("Height", fHeight);
	m_propertySet.GetProperty("Depth", fDepth);

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

	return MeshForge::CreateBoxMesh(fWidth, fHeight, fDepth, bInvertNormals, dwMeshFlags);
}
//----------------------------------------------------------------------------------------------
GeometryPtr BoxFactory::CreateGeometry(const Meson::Common::Text::String& p_strId)
{
	GeometryPtr pGeometry = CreateGeometry();
	pGeometry->SetId(p_strId);

	return pGeometry;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END