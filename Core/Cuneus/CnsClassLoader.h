#pragma once

#include <InputStream.h>
#include <OutputStream.h>

#include "CnsNamespaces.h"


#include "CnsTypeClass.h"


Meson_Cuneus_RTS_BEGIN

class ClassLoader
{
public:
	ClassLoader(void);
	virtual ~ClassLoader(void);

	//Meson::Cuneus::RTS::Reflection::TypeClass* Fetch( String ClassName );

	//static Meson::Common::Text::String ReadString( Meson::Common::IO::IInputStream& p_stream );

	virtual bool Load( Meson::Common::IO::IInputStream& p_stream );
};

Meson_Cuneus_RTS_END
