//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "BasicCuller.h"

#include "VstVistasEngine.h"
#include "VstSceneNode.h"
#include "VstCamera.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
BasicCuller::BasicCuller(void)
	: m_strType("Basic"),
	  m_pCamera(NULL),
	  m_pRootNode(NULL),
	  m_pVisibleEntitySet(NULL)
{
	m_pVisibleEntitySet = new VisibleEntitySet();
}
//----------------------------------------------------------------------------------------------
BasicCuller::~BasicCuller(void)
{
	m_pVisibleEntitySet = NULL;
}
//----------------------------------------------------------------------------------------------
const String& BasicCuller::GetType(void) const
{
	return m_strType;
}
//----------------------------------------------------------------------------------------------
TPointer<ICuller> BasicCuller::Clone(bool p_bShallow)
{
	BasicCuller* pBasicCuller = new BasicCuller();

	if (p_bShallow)
	{
		pBasicCuller->m_pCamera = m_pCamera;
		pBasicCuller->m_pRootNode = m_pRootNode;
		pBasicCuller->m_pVisibleEntitySet = m_pVisibleEntitySet;
		pBasicCuller->m_planeList = m_planeList;
	}
	else
		throw new Meson::Common::MesonException("Method not yet implemented!", __FILE__, __LINE__);

	return CullerPtr(pBasicCuller);
}
//----------------------------------------------------------------------------------------------
TPointer<ICuller> BasicCuller::CreateInstance(void)
{
	return CullerPtr(new BasicCuller());
}
//----------------------------------------------------------------------------------------------
uint BasicCuller::GetPlaneCount(void) const
{
	return (uint)m_userPlaneList.Size();
}
//----------------------------------------------------------------------------------------------
void BasicCuller::SetPlane(uint p_uiPlaneIndex, bool p_bEnabled)
{
	// To do
}
//----------------------------------------------------------------------------------------------
bool BasicCuller::IsPlaneEnabled(uint p_uiPlaneIndex) const
{
	// To do
	return true;
}
//----------------------------------------------------------------------------------------------
void BasicCuller::PushPlane(Meson::Common::Maths::Planef& p_plane)
{
	m_userPlaneList.Add(p_plane);
}
//----------------------------------------------------------------------------------------------
void BasicCuller::PopPlane(void)
{
	MESON_ASSERT(m_userPlaneList.Size() > 0, "Plane list contains no elements!");
	m_userPlaneList.Pop(0x01);
}
//----------------------------------------------------------------------------------------------
void BasicCuller::SetCamera(Camera* p_pCamera)
{
	m_pCamera = p_pCamera;
}
//----------------------------------------------------------------------------------------------
void BasicCuller::SetRootNode(ISceneNode* p_pLocalRootNode)
{
	m_pRootNode = p_pLocalRootNode;
}
//----------------------------------------------------------------------------------------------
void BasicCuller::Push(ISceneNode* p_pVisibleEntity)
{
	m_pVisibleEntitySet->Insert(p_pVisibleEntity, NULL, VisibleEntity::VACT_PUSH);
}
//----------------------------------------------------------------------------------------------
void BasicCuller::Enter(ISceneCompositeNode* p_pVisibleEntity, IEffect* p_pEffect)
{
	m_pVisibleEntitySet->Insert((ISceneNode*)p_pVisibleEntity, p_pEffect, VisibleEntity::VACT_ENTER);
}
//----------------------------------------------------------------------------------------------
void BasicCuller::Leave(ISceneCompositeNode* p_pVisibleEntity, IEffect* p_pEffect)
{
	m_pVisibleEntitySet->Insert((ISceneNode*)p_pVisibleEntity, p_pEffect, VisibleEntity::VACT_LEAVE);
}
//----------------------------------------------------------------------------------------------
bool BasicCuller::IsVisible(ISceneNode* p_pEntity) const
{
	if (!p_pEntity->IsVisible())
		return false;

	BoundingVolumePtr pWorldBound = p_pEntity->GetWorldBound();

	for (uint uiPlane = 0, uiPlaneCount = (uint)m_planeList.Size(); uiPlane < uiPlaneCount; uiPlane++)
	{
		if (pWorldBound->GetSide(m_planeList[uiPlane]) == Planef::SIDE_POSITIVE)
			return false;
	}

	return true;
}
//----------------------------------------------------------------------------------------------
bool BasicCuller::IsVisible(IBoundingVolume* p_pBoundingVolume) const
{
	for (uint uiPlane = 0, uiPlaneCount = (uint)m_planeList.Size(); uiPlane < uiPlaneCount; uiPlane++)
	{
		if (p_pBoundingVolume->GetSide(m_planeList[uiPlane]) == Planef::SIDE_POSITIVE)
			return false;
 	}

	return true;
}
//----------------------------------------------------------------------------------------------
void BasicCuller::SetVisibleSet(VisibleEntitySetPtr p_pVisibleEntitySet)
{
	m_pVisibleEntitySet = p_pVisibleEntitySet;
}
//----------------------------------------------------------------------------------------------
VisibleEntitySetPtr BasicCuller::GetVisibleSet(void)
{
	return m_pVisibleEntitySet;
}
//----------------------------------------------------------------------------------------------
VisibleEntitySetPtr BasicCuller::ComputeVisibleSet(bool p_bAccumulate)
{
	// Setup clipping planes
	m_planeList.Clear();

	for (uint uiPlane = 0; uiPlane < Camera::CF_COUNT; uiPlane++)
		m_planeList.Add(m_pCamera->GetFrustumPlane((Camera::FrustumPlane)uiPlane));

	for (uint uiPlane = 0; uiPlane < m_userPlaneList.Size(); uiPlane++)
		m_planeList.Add(m_userPlaneList[uiPlane]);

	// Clear entity set
	if (!p_bAccumulate)
		m_pVisibleEntitySet->Clear();

	// Find potentially visible objects
	m_pRootNode->Cull(*this);

	return m_pVisibleEntitySet;
}
//----------------------------------------------------------------------------------------------
void BasicCuller::Rebuild(void)
{
}
//----------------------------------------------------------------------------------------------
void BasicCuller::Update(void)
{
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END