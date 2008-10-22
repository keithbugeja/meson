#pragma once

#include "Namespaces.h"
#include "String8.h"

Meson_Common_Events_BEGIN

class EventMessage
{ 
	Meson::Common::Text::String m_strEventName;
	void *m_pEventDispatcher;
	void *m_pEventDataBuffer;

public:
	EventMessage(void* p_pEventDispatcher, Meson::Common::Text::String p_strEventName) :
	  m_pEventDispatcher(p_pEventDispatcher), m_strEventName(p_strEventName)
	{
	}

	Meson::Common::Text::String& GetEventName(void)
	{
		return m_strEventName;
	}

	void* GetEventDispatcher(void)
	{
		return m_pEventDispatcher;
	}
};

Meson_Common_Events_END