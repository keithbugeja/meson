#pragma once

#include "CnsNamespaces.h"

#include "String8.h"
#include "Version.h"
#include "PlugIn.h"

#include <CnsCuneusInterpreter.h>

#include "Interpreter.h"

Meson_Cuneus_BEGIN

class InterpreterPlugIn : public Meson::Common::PlugIn::IPlugIn
{
	ICuneusInterpreter *m_pInterpreter;
	Meson::Common::PlugIn::SharedObjectMap *m_pSharedObjectMap;
	Meson::Common::Text::String	m_strName;
	Meson::Common::Version	m_version;
public:
	InterpreterPlugIn( void );
	~InterpreterPlugIn( void );

	const Meson::Common::Text::String &GetName( void ) const;
	const Meson::Common::Version &GetVersion( void ) const;

	void Register( Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap );
	void Unregister( void );

	void Initialise( void );
	void Shutdown( void );
};

Meson_Cuneus_END
