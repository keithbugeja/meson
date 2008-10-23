//----------------------------------------------------------------------------------------------
#pragma once

#include "VstSceneGeometryNode.h"
#include "VstCuller.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
SceneGeometryNode::SceneGeometryNode(Scenegraph* p_pOwner)
	: ISceneNode(p_pOwner),
	  m_pMaterial(NULL),
	  m_pGeometry(NULL)
{
}
//----------------------------------------------------------------------------------------------
SceneGeometryNode::SceneGeometryNode(const Meson::Common::Text::String& p_strId, Scenegraph* p_pOwner)
	: ISceneNode(p_strId, p_pOwner),
	  m_pMaterial(NULL),
	  m_pGeometry(NULL)
{
}
//----------------------------------------------------------------------------------------------
SceneGeometryNode::~SceneGeometryNode(void)
{
	m_pMaterial = NULL;
	m_pGeometry = NULL;
}
//----------------------------------------------------------------------------------------------
void SceneGeometryNode::SetGeometry(GeometryPtr& p_pGeometry)
{
	m_pGeometry = p_pGeometry;
	WorldBound = p_pGeometry->GetBoundingVolume()->Clone();
}
//----------------------------------------------------------------------------------------------
GeometryPtr SceneGeometryNode::GetGeometry(void)
{
	return m_pGeometry;
}
//----------------------------------------------------------------------------------------------
void SceneGeometryNode::SetMaterial(MaterialPtr& p_pMaterial)
{
	m_pMaterial = p_pMaterial;
}
//----------------------------------------------------------------------------------------------
MaterialPtr SceneGeometryNode::GetMaterial(void)
{
	if (m_pMaterial == NULL)
		m_pMaterial = m_pGeometry->GetMaterial();

	return m_pMaterial;
}
//----------------------------------------------------------------------------------------------
void SceneGeometryNode::UpdateWorldBound(void)
{
	WorldBound->ComputeFromVolume(WorldTransformation, *m_pGeometry->GetBoundingVolume());
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END