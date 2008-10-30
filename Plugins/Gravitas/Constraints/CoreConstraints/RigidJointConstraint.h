#pragma once

#include "GrvConstraint.h"

Meson_Gravitas_Constraint_BEGIN

class RigidJointConstraint
	: public IConstraint
{
private: // private variables
	static Meson::Common::Text::String s_strType;
	Meson::Gravitas::Body::BodyPtr m_pBody1;
	Meson::Gravitas::Body::BodyPtr m_pBody2;
	bool m_bBroken;
	Real m_rBreakingThreshold;
	Meson::Common::Maths::TVector3<Real> m_vecBody1Anchor2;
	Meson::Common::Maths::TVector3<Real> m_vecBody2Anchor1;
	Meson::Common::Maths::TVector3<Real> m_vecBody1Anchor3;
	Meson::Common::Maths::TVector3<Real> m_vecBody2Anchor3;
	ConstraintDiscrepancyArrayList m_listConstraintDiscrepancies;

public: // public methods
	RigidJointConstraint(void);
	RigidJointConstraint(const Meson::Common::Text::String& p_strId);
	~RigidJointConstraint(void);

	// IGravitasEntity interface
	void RenderInstrumentation(
		Meson::Gravitas::Instrumentation::IInstrumentationDevice* p_pInstrumentationDevice);

	// IConstraint interface
	const Meson::Common::Text::String& GetType(void) const;

	Meson::Gravitas::Body::BodyPtr GetBody1(void);
	void SetBody1(Meson::Gravitas::Body::BodyPtr p_pBody1);

	Meson::Gravitas::Body::BodyPtr GetBody2(void);
	void SetBody2(Meson::Gravitas::Body::BodyPtr p_pBody2);

	void Bind(void);
	void Unbind(void);

	bool IsInstantaneous(void) const;

	bool IsBroken(void) const;
	void SetBroken(bool p_bBroken);
	Real GetBreakingThreshold(void) const;
	void SetBreakingThreshold(Real p_rValue);

	bool CanRecomputeDiscrepancies(void);
	void ComputeDiscrepancies(void);
	const ConstraintDiscrepancyList& GetDiscrepancies(void);
};

Meson_Gravitas_Constraint_END
