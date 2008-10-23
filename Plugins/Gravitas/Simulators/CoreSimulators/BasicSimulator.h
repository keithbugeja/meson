#pragma once

#include "GrvSimulator.h"
#include "GrvActivityManager.h"
#include "GrvConstraintBatchAggregator.h"
#include "GrvCollisionTracker.h"

Meson_Gravitas_Simulation_BEGIN

class BasicSimulator
	: public ISimulator
{
private: // private variables
	Meson::Gravitas::EnvironmentPtr m_pEnvironment;
	Meson::Gravitas::Kinetics::ActivityManager m_activityManager;
	Meson::Gravitas::Collision::CollisionTracker m_collisionTracker;
	Meson::Gravitas::Constraint::ConstraintBatchAggregator m_constraintBatchAggregator;
	Real m_rCurrentTime;
	Meson::Gravitas::Body::BodyPairLinkedList m_listCandidateCollisions;
	Meson::Gravitas::Constraint::ConstraintArrayList m_listConstraints;
	Meson::Gravitas::Constraint::ConstraintBatchArrayList m_listConstraintBatches;

private: // private methods
	void IntegrateVelocity(Real p_rSubDeltaTime);
	void IntegratePosition(Real p_rSubDeltaTime);
	void DetectCollisions(void);
	void SolveConstraints(void);
	void SubStep(Real p_rSubDeltaTime);

public: // public methods
	BasicSimulator(void);
	BasicSimulator(const Meson::Common::Text::String& p_strId);
	~BasicSimulator(void);

	// ISimulator interface
	void Initialise(Meson::Gravitas::EnvironmentPtr p_pEnvironment);
	void Terminate(void);

	void Step(Real p_rCurrentTime, Real p_rDeltaTime);
};

Meson_Gravitas_Simulation_END
