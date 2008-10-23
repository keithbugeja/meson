#pragma once

#include "GrvConstraintSolver.h"
#include "GrvActivityManager.h"

Meson_Gravitas_Constraint_BEGIN

class SequentialConstraintSolver
	: public Meson::Gravitas::Constraint::IConstraintSolver
{
private: // private variables
	Meson::Common::Text::String m_strName;
	Meson::Gravitas::Kinetics::ActivityManager m_activityManager;
	uint m_unIterations;
	Real m_rIterationInverse;
	Real m_rActivityThreshold;
	Real m_rActivityThresholdSquared;

private: // private methods
	void ResolveConstraintDiscrepancy(
		ConstraintPtr p_pConstraint,
		const ConstraintDiscrepancy& p_constraintDiscrepancy);
	void ResolveConstraint(ConstraintPtr p_pConstraint, Real p_rCurrentTime);

public: // public methods
	SequentialConstraintSolver(void);
	~SequentialConstraintSolver(void);

	// IGravitasEntity interface
	void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String, PropertyDescriptor>
		&p_mapProperties) const;
	void GetProperty(const Meson::Common::Text::String& p_strName, int& p_nValue) const;
	void SetProperty(const Meson::Common::Text::String& p_strName, int p_nValue);
	void GetProperty(const Meson::Common::Text::String& p_strName, Real& p_rValue) const;
	void SetProperty(const Meson::Common::Text::String& p_strName, Real p_rValue);

	// IConstraintSolver interface
	const Meson::Common::Text::String& GetName(void) const;
	void Solve(ConstraintBatch& p_constraintBatch);
};

Meson_Gravitas_Constraint_END
