//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "String.h"
#include "VstTypes.h"

#include "VstCullerFactory.h"
#include "BasicCuller.h"

Meson_Vistas_BEGIN

class BasicCullerFactory : public ICullerFactory
{
public:
	BasicCullerFactory(void);
	~BasicCullerFactory(void);

	const Meson::Common::Text::String& GetName(void) const;
	
	CullerPtr CreateCuller(void);

protected:
	Meson::Common::Text::String m_strName;
};

Meson_Vistas_END