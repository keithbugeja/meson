#include "CollisionEstimate.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Simulation;

CollisionEstimate::CollisionEstimate(void)
{
}

CollisionEstimate::~CollisionEstimate(void)
{
}

CollisionEstimate::CollisionEstimate(
	BodyPtr p_pBody1, BodyPtr p_pBody2,
	Real p_rTime)
{
	Body1 = p_pBody1;
	Body2 = p_pBody2;
	Time = p_rTime;
}

CollisionEstimate &CollisionEstimate::operator=(
	const CollisionEstimate &p_collisionEstimate)
{
	Body1 = p_collisionEstimate.Body1;
	Body2 = p_collisionEstimate.Body2;
	Time = p_collisionEstimate.Time;
	return *this;
}

bool CollisionEstimate::operator==(
	const CollisionEstimate &p_collisionEstimate) const
{
	if (Body1 != p_collisionEstimate.Body1) return false;
	if (Body2 != p_collisionEstimate.Body2) return false;
	if (!TMaths<Real>::Equals(Time,
		p_collisionEstimate.Time)) return false;
	return true;
}

bool CollisionEstimate::operator<(
	const CollisionEstimate &p_collisionEstimate) const
{
	return Time < p_collisionEstimate.Time;
}
