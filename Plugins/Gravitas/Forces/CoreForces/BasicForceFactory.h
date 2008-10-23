#pragma once

#include "GrvForceFactory.h"

Meson_Gravitas_Force_BEGIN

class BasicForceFactory
	: public IForceFactory
{
private: // private variables
	Meson::Common::Text::String m_strName;

public: // public methods
	BasicForceFactory(void);
	~BasicForceFactory(void);

	// IForceFactory interface
	const Meson::Common::Text::String& GetName(void);
	ForcePtr CreateForce(void);
	ForcePtr CreateForce(const Meson::Common::Text::String& p_strId);
};

Meson_Gravitas_Force_END
