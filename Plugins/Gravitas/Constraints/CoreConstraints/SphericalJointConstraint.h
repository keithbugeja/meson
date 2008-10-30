#pragma once

#include "GrvConstraint.h"

Meson_Gravitas_Constraint_BEGIN

class SphericalJointConstraint
	: public IConstraint
{
private: // private variables
	static Meson::Common::Text::String s_strType;
	Meson::Gravitas::Body::BodyPtr m_pBody1;
	Meson::Gravitas::Body::BodyPtr m_pBody2;
	bool m_bBroken;
	Real m_rBreakingThreshold;
	Meson::Common::Maths::TVector3<Real> m_vecBindPoint;
	Meson::Common::Maths::TVector3<Real> m_vecLocalConnectionPoint1;
	Meson::Common::Maths::TVector3<Real> m_vecLocalConnectionPoint2;
	Real m_rAngleLimit;
	Real m_rJointRestitution;
	Real m_rAngleLimitCosine;
	Real m_rMinimumCentroidDistance;
	ConstraintDiscrepancyArrayList m_listConstraintDiscrepancies;

private: // private methods
	void UpdateAngleLimitDerivatives(void);
	Meson::Common::Maths::TVector3<Real> GetCurrentJointPosition(void) const;

public: // public methods
	SphericalJointConstraint(void);
	SphericalJointConstraint(const Meson::Common::Text::String& p_strId);
	~SphericalJointConstraint(void);

	// IGravitasEntity interface
	void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String, PropertyDescriptor>
		&p_mapProperties) const;
	void GetProperty(const Meson::Common::Text::String &p_strName, Real &p_rValue) const;
	void SetProperty(const Meson::Common::Text::String &p_strName, Real p_rValue);
	void GetProperty(const Meson::Common::Text::String &p_strName, Meson::Common::Maths::TVector3<Real> &p_vecValue) const;
	void SetProperty(const Meson::Common::Text::String &p_strName, const Meson::Common::Maths::TVector3<Real> &p_vecValue);

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
