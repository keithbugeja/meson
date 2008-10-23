#pragma once

#include "GrvConstraintFactory.h"

Meson_Gravitas_Constraint_BEGIN

class SphericalJointConstraintFactory
	: public Meson::Gravitas::Constraint::IConstraintFactory
{
private: // private variables
	Meson::Common::Text::String m_strName;

public: // public methods
	SphericalJointConstraintFactory(void);
	~SphericalJointConstraintFactory(void);
	const Meson::Common::Text::String& GetName(void);
	ConstraintPtr CreateConstraint(void);
	ConstraintPtr CreateConstraint(const Meson::Common::Text::String& p_strId);
};

Meson_Gravitas_Constraint_END

