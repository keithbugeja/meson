//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "PlaneFactory.h"
#include "VstMeshForge.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
PlaneFactory::PlaneFactory(void)
	: m_strName("Plane")
{
	m_propertySet.CreateProperty("Normal", Vector3f::Up);
	m_propertySet.CreateProperty("Distance", 0.0f);
	m_propertySet.CreateProperty("Length", 64.0f);
	m_propertySet.CreateProperty("Breadth", 64.0f);
	
	m_propertySet.CreateProperty("TextureMultiplier", Vector2f(1.0f, 1.0f));

	m_propertySet.CreateProperty("InvertNormals", false);

	m_propertySet.CreateProperty("TextureCoordinates", true);
	m_propertySet.CreateProperty("Binormals", true);
	m_propertySet.CreateProperty("Tangents", true);
	m_propertySet.CreateProperty("Normals", true);
	m_propertySet.CreateProperty("Colour", true);
}
//----------------------------------------------------------------------------------------------
PlaneFactory::~PlaneFactory(void)
{
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& PlaneFactory::GetName(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
IPropertySet* PlaneFactory::GetPropertySet(void)
{
	return (IPropertySet*)&m_propertySet;
}
//----------------------------------------------------------------------------------------------
GeometryPtr PlaneFactory::CreateGeometry(void)
{
	Vector3f vec3Normal;
	Vector2f vec2TextureMultiplier;

	float fDistance,
		  fLength,
		  fBreadth;

	bool bInvertNormals,
		 bMeshFlags;

	m_propertySet.GetProperty("Normal", vec3Normal);
	m_propertySet.GetProperty("Distance", fDistance);
	m_propertySet.GetProperty("Length", fLength);
	m_propertySet.GetProperty("Breadth", fBreadth);

	m_propertySet.GetProperty("TextureMultiplier", vec2TextureMultiplier);

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

	return MeshForge::CreatePlaneMesh(vec3Normal, fDistance, fLength, fBreadth, vec2TextureMultiplier.X, vec2TextureMultiplier.Y, bInvertNormals, dwMeshFlags);
}
//----------------------------------------------------------------------------------------------
GeometryPtr PlaneFactory::CreateGeometry(const Meson::Common::Text::String& p_strId)
{
	GeometryPtr pGeometry = CreateGeometry();
	pGeometry->SetId(p_strId);

	return pGeometry;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END