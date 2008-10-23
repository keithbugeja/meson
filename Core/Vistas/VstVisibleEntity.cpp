//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstVisibleEntity.h"
#include "VstSceneNode.h"

Meson_Vistas_BEGIN
	
VisibleEntity::VisibleEntity(void)
	: m_pEntity(NULL),
	  m_pEffect(NULL),
	  m_eAction(VACT_PUSH)
{
}
//----------------------------------------------------------------------------------------------
VisibleEntity::VisibleEntity(ISceneNode* p_pSceneNode, IEffect* p_pEffect, Action p_eAction)
	: m_pEntity(p_pSceneNode),
	  m_pEffect(p_pEffect),
	  m_eAction(p_eAction)
{
}
//----------------------------------------------------------------------------------------------
VisibleEntity::VisibleEntity(const VisibleEntity& p_visibleEntity)
	: m_pEntity(p_visibleEntity.m_pEntity),
	  m_pEffect(p_visibleEntity.m_pEffect),
	  m_eAction(p_visibleEntity.m_eAction)
{
}
//----------------------------------------------------------------------------------------------
const VistasType& VisibleEntity::GetType(void) const { return m_pEntity->GetInstanceType(); }
//----------------------------------------------------------------------------------------------
ISceneNode* VisibleEntity::GetEntity(void) const { return m_pEntity; }
//----------------------------------------------------------------------------------------------
void VisibleEntity::SetEntity(ISceneNode* p_pEntity) { m_pEntity = p_pEntity; }
//----------------------------------------------------------------------------------------------
IEffect* VisibleEntity::GetEffect(void) const { return m_pEffect; }
//----------------------------------------------------------------------------------------------
void VisibleEntity::SetEffect(IEffect* p_pEffect) { m_pEffect = p_pEffect; }
//----------------------------------------------------------------------------------------------
VisibleEntity::Action VisibleEntity::GetAction(void) const { return m_eAction; }
//----------------------------------------------------------------------------------------------
void VisibleEntity::SetAction(VisibleEntity::Action p_eAction) { m_eAction = p_eAction; }
//----------------------------------------------------------------------------------------------
bool VisibleEntity::IsEffect(void) const { return (m_pEffect != NULL); }
//----------------------------------------------------------------------------------------------
bool VisibleEntity::IsEntity(void) const { return (m_pEntity != NULL); }
//----------------------------------------------------------------------------------------------
bool VisibleEntity::IsSentinel(void) const { return (m_eAction != VisibleEntity::VACT_PUSH); }
//----------------------------------------------------------------------------------------------
bool VisibleEntity::IsMatchingSentinel(const VisibleEntity* p_pVisibleEntity) const
{
	if (m_eAction == VisibleEntity::VACT_PUSH)
		return false;

	if ((m_eAction == VisibleEntity::VACT_ENTER && p_pVisibleEntity->m_eAction == VisibleEntity::VACT_LEAVE) ||
		(m_eAction == VisibleEntity::VACT_LEAVE && p_pVisibleEntity->m_eAction == VisibleEntity::VACT_ENTER))
	{
		return ((m_pEffect == p_pVisibleEntity->m_pEffect) || (m_pEntity == p_pVisibleEntity->m_pEntity));
	}

	return false;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END