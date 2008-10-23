//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreConstraintsPlugIn
//	PlugIn interface for the Gravitas core constraints implementations.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		18/02/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "String8.h"
#include "Version.h"
#include "PlugIn.h"

#include "GrvNamespaces.h"

#include "SphericalJointConstraintFactory.h"
#include "RevoluteJointConstraintFactory.h"
#include "AngularMotorConstraintFactory.h"
#include "RigidJointConstraintFactory.h"

#include "SequentialConstraintSolver.h"
#include "PenaltyBasedConstraintSolver.h"

Meson_Gravitas_BEGIN

class CoreConstraintsPlugIn
	: public Meson::Common::PlugIn::IPlugIn
{
private:
	Meson::Common::Text::String	m_pName;
	Meson::Common::Version	m_version;
	Meson::Common::PlugIn::SharedObjectMap *m_pSharedObjectMap;

	// constraint factories
	Meson::Gravitas::Constraint::SphericalJointConstraintFactory* m_pSphericalJointConstraintFactory;
	Meson::Gravitas::Constraint::RevoluteJointConstraintFactory* m_pRevoluteJointConstraintFactory;
	Meson::Gravitas::Constraint::AngularMotorConstraintFactory* m_pAngularMotorConstraintFactory;
	Meson::Gravitas::Constraint::RigidJointConstraintFactory* m_pRigidJointConstraintFactory;

	// constraint solvers
	Meson::Gravitas::Constraint::SequentialConstraintSolver* m_pSequentialConstraintSolver;
	Meson::Gravitas::Constraint::PenaltyBasedConstraintSolver* m_pPenaltyBasedConstraintSolver;

public:
	CoreConstraintsPlugIn();
	~CoreConstraintsPlugIn();

	const Meson::Common::Text::String& GetName(void) const;
	const Meson::Common::Version& GetVersion(void) const;

	void Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap);
	void Unregister();

	void Initialise(void);
	void Shutdown(void);
};

Meson_Gravitas_END
