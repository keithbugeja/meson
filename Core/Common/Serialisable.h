//----------------------------------------------------------------------------------------------
//	Meson::Common::Serialisation::ISerialisable
//	Interface class for serialisable objects
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	25/08/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "DebugSupport.h"
#include "MesonException.h"
#include "Serialiser.h"

Meson_Common_Serialisation_BEGIN

class ISerialisable
{
public:
	virtual bool Load(ISerialiser& p_serialiser, Meson::Common::IO::IInputStream& p_inputStream) = 0;
	virtual bool Store(ISerialiser& p_serialiser, Meson::Common::IO::IOutputStream& p_outputStream) = 0;
};

Meson_Common_Serialisation_END
