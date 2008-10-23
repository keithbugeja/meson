#pragma once

#include "Namespaces.h"

#include "EventMessage.h"
#include "HashMap.h"

Meson_Common_Events_BEGIN

template <typename TObserver, typename TCallback> class TEventDispatcher
{
protected:
	Meson::Common::Collections::THashMap<TObserver, TCallback> m_observerMap;

public:
	void AttachObserver(TObserver p_pObserver, TCallback p_pCallback)
	{
		if (!m_observerMap.ContainsKey(p_pObserver))
			m_observerMap.Insert(p_pObserver, p_pCallback);
	}

	void DetachObserver(TObserver p_pObserver)
	{
		if (m_observerMap.ContainsKey(p_pObserver))
			m_observerMap.RemoveKey(p_pObserver);
	}

	// TEMP: UNTIL ENUM IMPLEMENTS PAIR ENUMERATION
	void Dispatch(EventMessage& p_eventMessage)
	{
		Meson::Common::Collections::TEnumerator<TObserver>& observerEnumerator = m_observerMap.GetKeyEnumerator();
		
		TObserver pObserver;
		TCallback pDispatchFunction;

		while (observerEnumerator.HasMoreElements())
		{
			pObserver = observerEnumerator.NextElement();
			pDispatchFunction = m_observerMap[pObserver];
			(pObserver->*pDispatchFunction)(p_eventMessage);
		}
	}
};

Meson_Common_Events_END