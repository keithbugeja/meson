//----------------------------------------------------------------------------------------------
//	Meson::Common::PlugIn::SharedObjectMap
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "Types.h"

#include "MesonException.h"
#include "DebugSupport.h"

#include "HashMap.h"
#include "String8.h"

Meson_Common_PlugIn_BEGIN

class SharedObjectMap 
{
private:
	Meson::Common::Collections::THashMap<Meson::Common::Text::String,void*> m_sharedObjectMap;

public:
	void RegisterObject(void* p_pObject, Meson::Common::Text::String p_strObjectName);
	void* GetRegisteredObject(Meson::Common::Text::String p_strObjectName);
	void UnregisterObject(Meson::Common::Text::String p_strObjectName);
	void UnregisterObject(void* p_pObject);
	void UnregisterAllObjects(void);
};

Meson_Common_PlugIn_END


