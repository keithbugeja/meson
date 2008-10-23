#pragma once

#include "CnsNamespaces.h"

#include "String8.h"
#include "Version.h"
#include "PlugIn.h"
#include "BasicByteCodeManager.h"

Meson_Cuneus_BEGIN

class CoreByteCodeManagersPlugIn : public Meson::Common::PlugIn::IPlugIn
{
private:
	IByteCodeMgr *m_pByteCodeMgr;
	Meson::Common::PlugIn::SharedObjectMap *m_pSharedObjectMap;
	Meson::Common::Text::String	m_strName;
	Meson::Common::Version	m_version;
	
public:
	CoreByteCodeManagersPlugIn( void );
	~CoreByteCodeManagersPlugIn( void );

	const Meson::Common::Text::String &GetName( void ) const;
	const Meson::Common::Version &GetVersion( void ) const;

	void Register( Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap );
	void Unregister( void );

	void Initialise( void );
	void Shutdown( void );
};

Meson_Cuneus_END
