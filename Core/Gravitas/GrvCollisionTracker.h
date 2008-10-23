#pragma once

#include "GrvContactManifold.h"

Meson_Gravitas_Collision_BEGIN

struct CollisionTrackerState;

//----------------------------------------------------------------------------------------------
/// \class CollisionTracker
/// Tracks starting, ongoing and ending of collision events. This is an internal class used
/// by the event management module of the physics engine.
//----------------------------------------------------------------------------------------------
class CollisionTracker
{
private: // private variables
	CollisionTrackerState* m_pCollisionTrackerState;

public: // public methods
	//----------------------------------------------------------------------------------------------
	/// Constructs a collision tracker.
	//----------------------------------------------------------------------------------------------
	CollisionTracker(void);

	//----------------------------------------------------------------------------------------------
	/// Destroys the collision tracker.
	//----------------------------------------------------------------------------------------------
	~CollisionTracker(void);

	//----------------------------------------------------------------------------------------------
	/// Returns the sampling frequency of the collision tracker.
	/// \returns the sampling frequency of the collision tracker.
	//----------------------------------------------------------------------------------------------
	Real GetSamplingFrequency(void) const;

	//----------------------------------------------------------------------------------------------
	/// Sets the sampling frequency of the collision tracker. A high frequency value may impact the
	/// performance of the physics engine and hence should be limited to around 10Hz tp 20Hz.
	/// \param p_rValue Sampling frequency in seconds.
	//----------------------------------------------------------------------------------------------
	void SetSamplingFrequency(Real p_rValue);

	//----------------------------------------------------------------------------------------------
	/// Reports a starting or ongoing collision via a contact manifold structure.
	/// \param p_contactManifold Contact manifold structure containing data about the collision.
	//----------------------------------------------------------------------------------------------
	void ReportCollision(const ContactManifold& p_contactManifold);

	//----------------------------------------------------------------------------------------------
	/// Generates starting, ongoing and ending collision events depending on the event flags set.
	/// Provided that the method is invoked at least as often as the specified sampling rate, it
	/// will generate events in line with the given rate.
	//----------------------------------------------------------------------------------------------
	void GenerateCollisionEvents(void);
};

Meson_Gravitas_Collision_END
