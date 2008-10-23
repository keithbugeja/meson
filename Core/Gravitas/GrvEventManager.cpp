#include "GrvEventManager.h"

#include "ArrayList.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas::Event;

EventFlags::EventFlags operator|(
	EventFlags::EventFlags p_eventFlags1,
	EventFlags::EventFlags p_eventFlags2)
{
	return (EventFlags::EventFlags)
		((uint) p_eventFlags1 | (uint) p_eventFlags2);
}

Meson_Gravitas_Event_BEGIN

struct EventManagerState
{
	uint m_unEventFlags;
	Meson::Common::Collections::TArrayList<ActivityListenerPtr> m_listActivityListeners;
	Meson::Common::Collections::TArrayList<CollisionFilterListenerPtr> m_listCollisionFilterListeners;
	Meson::Common::Collections::TArrayList<CollisionListenerPtr> m_listCollisionListeners;
	Meson::Common::Collections::TArrayList<ConstraintListenerPtr> m_listConstraintListeners;
	Meson::Common::Collections::TArrayList<MaterialListenerPtr> m_listMaterialListeners;

	EventManagerState(void)
		: m_unEventFlags(EventFlags::None)
		, m_listActivityListeners()
		, m_listCollisionFilterListeners()
		, m_listCollisionListeners()
		, m_listConstraintListeners()
		, m_listMaterialListeners()
	{ }
};

Meson_Gravitas_Event_END

EventManager::EventManager()
	: m_pEventManagerState(NULL)
{
	m_pEventManagerState = new EventManagerState();
}

EventManager::~EventManager()
{
	delete m_pEventManagerState;
}

bool EventManager::IsEnabled(EventFlags::EventFlags p_eventFlags) const
{
	return (m_pEventManagerState->m_unEventFlags & p_eventFlags) != NULL;
}

void EventManager::Enable(
	EventFlags::EventFlags p_eventFlags)
{
	m_pEventManagerState->m_unEventFlags |= p_eventFlags;
}

void EventManager::Disable(
	EventFlags::EventFlags p_eventFlags)
{
	m_pEventManagerState->m_unEventFlags &= !p_eventFlags;
}

TEnumerator<ActivityListenerPtr>& EventManager::GetActivityListeners(void)
{
	return m_pEventManagerState->m_listActivityListeners.GetEnumerator();
}

void EventManager::AddListener(ActivityListenerPtr p_pActivityListener)
{
	if (m_pEventManagerState->m_listActivityListeners.IndexOf(p_pActivityListener) >= 0)
		throw new MesonException("Activity listener already registered.",
			__FILE__, __LINE__);
	m_pEventManagerState->m_listActivityListeners.Add(p_pActivityListener);
}

void EventManager::RemoveListener(ActivityListenerPtr p_pActivityListener)
{
	if (m_pEventManagerState->m_listActivityListeners.IndexOf(p_pActivityListener) < 0)
		throw new MesonException("Activity listener not registered.",
			__FILE__, __LINE__);
	m_pEventManagerState->m_listActivityListeners.Remove(p_pActivityListener);
}

void EventManager::Notify(ActivityEventPtr p_pActivityEvent) const
{
	TArrayList<ActivityListenerPtr>& listActivityListeners
		= m_pEventManagerState->m_listActivityListeners;
	size_t unCount = listActivityListeners.Size();
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
		listActivityListeners[unIndex]->OnActivityEvent(p_pActivityEvent);
}

TEnumerator<CollisionFilterListenerPtr>& EventManager::GetCollisionFilterListeners(void)
{
	return m_pEventManagerState->m_listCollisionFilterListeners.GetEnumerator();
}

void EventManager::AddListener(CollisionFilterListenerPtr p_pCollisionFilterListener)
{
	if (m_pEventManagerState->m_listCollisionFilterListeners.IndexOf(p_pCollisionFilterListener) >= 0)
		throw new MesonException("Collision filter listener already registered.",
			__FILE__, __LINE__);
	m_pEventManagerState->m_listCollisionFilterListeners.Add(p_pCollisionFilterListener);
}

void EventManager::RemoveListener(CollisionFilterListenerPtr p_pCollisionFilterListener)
{
	if (m_pEventManagerState->m_listCollisionFilterListeners.IndexOf(p_pCollisionFilterListener) < 0)
		throw new MesonException("Collision filter listener not registered.",
			__FILE__, __LINE__);
	m_pEventManagerState->m_listCollisionFilterListeners.Remove(p_pCollisionFilterListener);
}

void EventManager::Notify(CollisionFilterEventPtr p_pCollisionFilterEvent) const
{
	TArrayList<CollisionFilterListenerPtr>& listCollisionFilterListeners
		= m_pEventManagerState->m_listCollisionFilterListeners;
	size_t unCount = listCollisionFilterListeners.Size();
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
		listCollisionFilterListeners[unIndex]->OnCollisionFilterEvent(p_pCollisionFilterEvent);
}

TEnumerator<CollisionListenerPtr>& EventManager::GetCollisionListeners(void)
{
	return m_pEventManagerState->m_listCollisionListeners.GetEnumerator();
}

void EventManager::AddListener(CollisionListenerPtr p_pCollisionListener)
{
	if (m_pEventManagerState->m_listCollisionListeners.IndexOf(p_pCollisionListener) >= 0)
		throw new MesonException("Collision listener already registered.",
			__FILE__, __LINE__);
	m_pEventManagerState->m_listCollisionListeners.Add(p_pCollisionListener);
}

void EventManager::RemoveListener(CollisionListenerPtr p_pCollisionListener)
{
	if (m_pEventManagerState->m_listCollisionListeners.IndexOf(p_pCollisionListener) < 0)
		throw new MesonException("Collision listener not registered.",
			__FILE__, __LINE__);
	m_pEventManagerState->m_listCollisionListeners.Remove(p_pCollisionListener);
}

void EventManager::Notify(CollisionEventPtr p_pCollisionEvent) const
{
	TArrayList<CollisionListenerPtr>& listCollisionListeners
		= m_pEventManagerState->m_listCollisionListeners;
	size_t unCount = listCollisionListeners.Size();
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
		listCollisionListeners[unIndex]->OnCollisionEvent(p_pCollisionEvent);
}

TEnumerator<ConstraintListenerPtr>& EventManager::GetConstraintListeners(void)
{
	return m_pEventManagerState->m_listConstraintListeners.GetEnumerator();
}

void EventManager::AddListener(ConstraintListenerPtr p_pConstraintListener)
{
	if (m_pEventManagerState->m_listConstraintListeners.IndexOf(p_pConstraintListener) >= 0)
		throw new MesonException("Constraint listener already registered.",
			__FILE__, __LINE__);
	m_pEventManagerState->m_listConstraintListeners.Add(p_pConstraintListener);
}

void EventManager::RemoveListener(ConstraintListenerPtr p_pConstraintListener)
{
	if (m_pEventManagerState->m_listConstraintListeners.IndexOf(p_pConstraintListener) < 0)
		throw new MesonException("Collision listener not registered.",
			__FILE__, __LINE__);
	m_pEventManagerState->m_listConstraintListeners.Remove(p_pConstraintListener);
}

void EventManager::Notify(ConstraintEventPtr p_pConstraintEvent) const
{
	TArrayList<ConstraintListenerPtr>& listConstraintListeners
		= m_pEventManagerState->m_listConstraintListeners;
	size_t unCount = listConstraintListeners.Size();
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
		listConstraintListeners[unIndex]->OnConstraintEvent(p_pConstraintEvent);
}

TEnumerator<MaterialListenerPtr>& EventManager::GetMaterialListeners(void)
{
	return m_pEventManagerState->m_listMaterialListeners.GetEnumerator();
}

void EventManager::AddListener(MaterialListenerPtr p_pMaterialListener)
{
	if (m_pEventManagerState->m_listMaterialListeners.IndexOf(p_pMaterialListener) >= 0)
		throw new MesonException("Material listener already registered.",
			__FILE__, __LINE__);
	m_pEventManagerState->m_listMaterialListeners.Add(p_pMaterialListener);
}

void EventManager::RemoveListener(MaterialListenerPtr p_pMaterialListener)
{
	if (m_pEventManagerState->m_listMaterialListeners.IndexOf(p_pMaterialListener) < 0)
		throw new MesonException("Material listener not registered.",
			__FILE__, __LINE__);
	m_pEventManagerState->m_listMaterialListeners.Remove(p_pMaterialListener);
}

void EventManager::Notify(MaterialEventPtr p_pMaterialEvent) const
{
	TArrayList<MaterialListenerPtr>& listMaterialListeners
		= m_pEventManagerState->m_listMaterialListeners;
	size_t unCount = listMaterialListeners.Size();
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
		listMaterialListeners[unIndex]->OnMaterialEvent(p_pMaterialEvent);
}
