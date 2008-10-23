#pragma once

#include "GrvSpaceFactory.h"

Meson_Gravitas_Space_BEGIN

class KDTreeSpaceFactory
	: public Meson::Gravitas::Space::ISpaceFactory
{
private: // private variables
	Meson::Common::Text::String m_strName;

public: // public methods
	KDTreeSpaceFactory(void);
	~KDTreeSpaceFactory(void);
	const Meson::Common::Text::String& GetName(void);
	SpacePtr CreateSpace(void);
	SpacePtr CreateSpace(const Meson::Common::Text::String& p_strId);
};

Meson_Gravitas_Space_END
