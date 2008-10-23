#pragma once

#include "List.h"
#include "Pair.h"

#include "GrvNamespaces.h"
#include "GrvBody.h"
#include "CollisionEstimate.h"

Meson_Gravitas_Simulation_BEGIN

class CollisionTimeEstimator
{
public:
	CollisionTimeEstimator(void);
	~CollisionTimeEstimator(void);
	
	void EstimateCollisions(
		Real p_rMaxTime,
		const Meson::Common::Collections::TList< Meson::Gravitas::Body::BodyPair >
			&p_listCandidateCollisions,
		Meson::Common::Collections::TList<CollisionEstimate>
			&p_listCollisionEstimates);
};

Meson_Gravitas_Simulation_END
