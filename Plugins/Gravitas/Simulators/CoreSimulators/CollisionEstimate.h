#pragma once

#include "GrvNamespaces.h"
#include "GrvBody.h"

Meson_Gravitas_Simulation_BEGIN

struct CollisionEstimate
{
	Meson::Gravitas::Body::BodyPtr Body1;
	Meson::Gravitas::Body::BodyPtr Body2;
	Real Time;

	CollisionEstimate(void);
	~CollisionEstimate(void);
	CollisionEstimate(
		Meson::Gravitas::Body::BodyPtr p_pBody1,
		Meson::Gravitas::Body::BodyPtr p_pBody2,
		Real p_rTime);
	CollisionEstimate &operator=(const CollisionEstimate &p_collisionEstimate);
	bool operator==(const CollisionEstimate &p_collisionEstimate) const;
	bool operator<(const CollisionEstimate &p_collisionEstimate) const;
};

Meson_Gravitas_Simulation_END
