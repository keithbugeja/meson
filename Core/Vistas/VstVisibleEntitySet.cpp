//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstVisibleEntitySet.h"
#include "VstEffect.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
VisibleEntitySet::VisibleEntitySet(void) 
	: VistasObject(true)
	, m_visibleEntityQueue()
{ 
}
//----------------------------------------------------------------------------------------------
VisibleEntitySet::VisibleEntitySet(const Meson::Common::Text::String& p_strName) 
	: VistasObject(p_strName)
	, m_visibleEntityQueue()
{ 
}
//----------------------------------------------------------------------------------------------
VisibleEntitySet::VisibleEntitySet(const VisibleEntitySet& p_visibleEntitySet)
	: VistasObject(p_visibleEntitySet.m_strId)
	, m_visibleEntityQueue(p_visibleEntitySet.m_visibleEntityQueue)
{
}
//----------------------------------------------------------------------------------------------
VisibleEntitySet::~VisibleEntitySet() 
{ 	
	VisibleEntityEnumerator enumerator = m_visibleEntityQueue.GetEnumerator();

	while (enumerator.HasMoreElements())
		enumerator.NextElement() = NULL;
 }
//----------------------------------------------------------------------------------------------
void VisibleEntitySet::Insert(ISceneNode* p_pEntity, IEffect* p_pEffect, VisibleEntity::Action p_eAction)
{
	m_visibleEntityQueue.Push(VisibleEntityPtr(new VisibleEntity(p_pEntity, p_pEffect, p_eAction)));
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySet::GetVisibleEntitySubset(VisibleEntitySetPtr p_pVisibleEntitySet, 
											  uint p_uiStartIndex, uint p_uiEndIndex)
{
	TQueue<VisibleEntityPtr>& visibleEntityQueue = p_pVisibleEntitySet->m_visibleEntityQueue;
	visibleEntityQueue.Clear();

	for (uint uiIndex = p_uiStartIndex; uiIndex < p_uiEndIndex; uiIndex++)
		visibleEntityQueue.Push(m_visibleEntityQueue[uiIndex]);
}
//----------------------------------------------------------------------------------------------
VisibleEntitySetPtr VisibleEntitySet::GetVisibleEntitySubset(uint p_uiStartIndex, uint p_uiEndIndex)
{
	VisibleEntitySetPtr pVisibleEntitySet(new VisibleEntitySet());
	GetVisibleEntitySubset(pVisibleEntitySet, p_uiStartIndex, p_uiEndIndex);
	return pVisibleEntitySet;
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySet::GetVisibleEntitySubset(VisibleEntitySetPtr p_pVisibleEntitySet, uint p_uiSentinelIndex)
{
	VisibleEntityPtr pSentinel(m_visibleEntityQueue[p_uiSentinelIndex]),
					 pEntity(NULL);

	TQueue<VisibleEntityPtr>& visibleEntityQueue = p_pVisibleEntitySet->m_visibleEntityQueue;
	visibleEntityQueue.Clear();

	switch(pSentinel->GetAction())
	{
		case VisibleEntity::VACT_ENTER:
		{
			for (uint uiIndex = p_uiSentinelIndex + 1, uiCount = (uint)m_visibleEntityQueue.Size(); uiIndex < uiCount; uiIndex++)
			{
				pEntity = m_visibleEntityQueue[uiIndex];

				if (pSentinel->IsMatchingSentinel(pEntity))
					break;

				visibleEntityQueue.Push(pEntity);
			}

			break;
		}

		case VisibleEntity::VACT_LEAVE:
		{
			for (uint uiIndex = p_uiSentinelIndex - 1; uiIndex >= 0; uiIndex--)
			{
				pEntity = m_visibleEntityQueue[uiIndex];

				if (pSentinel->IsMatchingSentinel(pEntity))
					break;

				visibleEntityQueue.Push(pEntity);
			}

			break;
		}
		
		case VisibleEntity::VACT_PUSH:
			break;
	}
}
//----------------------------------------------------------------------------------------------
VisibleEntitySetPtr VisibleEntitySet::GetVisibleEntitySubset(uint p_uiSentinelIndex)
{
	VisibleEntitySetPtr pVisibleEntitySet(new VisibleEntitySet());
	GetVisibleEntitySubset(pVisibleEntitySet, p_uiSentinelIndex);
	return pVisibleEntitySet;
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySet::Clear(void)
{
	m_visibleEntityQueue.Clear();
}
//----------------------------------------------------------------------------------------------
size_t VisibleEntitySet::Size(void) const
{
	return m_visibleEntityQueue.Size();
}
//----------------------------------------------------------------------------------------------
VisibleEntityPtr& VisibleEntitySet::GetElement(int p_nIndex)
{
	return m_visibleEntityQueue.GetElement(p_nIndex);
}
//----------------------------------------------------------------------------------------------
VisibleEntityPtr& VisibleEntitySet::operator[](int p_nIndex)
{
	return m_visibleEntityQueue[p_nIndex];
}
//----------------------------------------------------------------------------------------------
VisibleEntityEnumerator VisibleEntitySet::GetEnumerator(bool p_bReset)
{
	return m_visibleEntityQueue.GetEnumerator(p_bReset);
}
//----------------------------------------------------------------------------------------------
bool VisibleEntitySet::operator==(const VisibleEntitySet& p_visibleEntitySet) const
{
	return m_strId.Compare(p_visibleEntitySet.m_strId) == 0 && 
		m_visibleEntityQueue == p_visibleEntitySet.m_visibleEntityQueue;
}
//----------------------------------------------------------------------------------------------
VisibleEntitySet& VisibleEntitySet::operator=(const VisibleEntitySet& p_visibleEntitySet)
{
	m_visibleEntityQueue.Clear();

	size_t uiQueueSize = m_visibleEntityQueue.Size();

	for( size_t uiIndex = 0; uiIndex < uiQueueSize; uiIndex++ )
		m_visibleEntityQueue.Push( p_visibleEntitySet.m_visibleEntityQueue( uiIndex ) );

	return *this;
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END