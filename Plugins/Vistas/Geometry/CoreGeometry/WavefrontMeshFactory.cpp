//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "WavefrontMeshFactory.h"
#include "VstMeshForge.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
WavefrontMeshFactory::WavefrontMeshFactory(void)
	: m_strName("WavefrontMesh")
{
	m_propertySet.CreateProperty("AssetName", "WavefrontMesh.obj");
	
	m_propertySet.CreateProperty("InvertNormals", false);

	m_propertySet.CreateProperty("TextureCoordinates", true);
	m_propertySet.CreateProperty("Binormals", true);
	m_propertySet.CreateProperty("Tangents", true);
	m_propertySet.CreateProperty("Normals", true);
	m_propertySet.CreateProperty("Colour", true);
}
//----------------------------------------------------------------------------------------------
WavefrontMeshFactory::~WavefrontMeshFactory(void)
{
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& WavefrontMeshFactory::GetName(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
IPropertySet* WavefrontMeshFactory::GetPropertySet(void)
{
	return (IPropertySet*)&m_propertySet;
}
//----------------------------------------------------------------------------------------------
GeometryPtr WavefrontMeshFactory::CreateGeometry(void)
{
	String strFilename;

	bool bInvertNormals,
		 bMeshFlags;

	m_propertySet.GetProperty("AssetName", strFilename);

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

	return MeshForge::CreateMeshFromFile(strFilename, 1.0f, dwMeshFlags);
}
//----------------------------------------------------------------------------------------------
GeometryPtr WavefrontMeshFactory::CreateGeometry(const Meson::Common::Text::String& p_strId)
{
	GeometryPtr pGeometry = CreateGeometry();
	pGeometry->SetId(p_strId);

	return pGeometry;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END