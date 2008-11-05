#pragma once

#include "GrvNamespaces.h"
#include "GrvTypes.h"

#include "GrvBody.h"

Meson_Gravitas_Kinetics_BEGIN

struct ActivityManagerState;

//----------------------------------------------------------------------------------------------
/// \class ActivityManager
/// Handles suspension and resumption of body kinetic activity. The activity manager is
/// typically contained in a simulator instance.
//----------------------------------------------------------------------------------------------
class GRAVITAS_API ActivityManager
{
private: // privvate variables
	ActivityManagerState* m_pActivityManagerState;

public: // public methods
	//----------------------------------------------------------------------------------------------
	/// Constructs an activity manager.
	//----------------------------------------------------------------------------------------------
	ActivityManager(void);

	//----------------------------------------------------------------------------------------------
	/// Destroys the activity manager.
	//----------------------------------------------------------------------------------------------
	~ActivityManager(void);

	//----------------------------------------------------------------------------------------------
	/// Sets the current simulstor time to test for inactivity intervals. 
	//----------------------------------------------------------------------------------------------
	void SetCurrentTime(Real p_rValue);

	//----------------------------------------------------------------------------------------------
	/// Returns the kinetic activity threshold. Bodies with a kinetic activity below this value for
	/// the currently set inactivity time threshold are considered static and set to an inactive
	/// state.
	/// \returns the kinetic activity threshold.
	//----------------------------------------------------------------------------------------------
	Real GetActivityThreshold(void) const;

	//----------------------------------------------------------------------------------------------
	/// Sets the kinetic activity threshold. Bodies with a kinetic activity below this value for the
	/// currently set inactivity time threshold are considered static and set to an inactive state.
	/// \param p_rValue Kinetic activity threshold to set.
	//----------------------------------------------------------------------------------------------
	void SetActivityThreshold(Real p_rValue);

	//----------------------------------------------------------------------------------------------
	/// Returns the inactivity time thershold. Bodies with a kinetic activity below the activity
	/// threshold for the current inactivity time threshold are considered static and set to an
	/// inactive state.
	/// \returns the inactivity time thershold.
	//----------------------------------------------------------------------------------------------
	Real GetInactivityTimeThreshold(void) const;

	//----------------------------------------------------------------------------------------------
	/// Sets the inactivity time thershold. Bodies with a kinetic activity below the activity
	/// threshold for the current inactivity time threshold are considered static and set to an
	/// inactive state.
	/// \param p_rValue Inactivity time thershold to set.
	//----------------------------------------------------------------------------------------------
	void SetInactivityTimeThreshold(Real p_rValue);

	//----------------------------------------------------------------------------------------------
	/// Reactivates the kinetic state of the given body. If the appropriate activity event flag is
	/// set, an activity event is also triggered.
	/// \param p_pBody Managed pointer to a body for which to resume kinetic activity.
	//----------------------------------------------------------------------------------------------
	void ResumeActivity(
		Meson::Gravitas::Body::BodyPtr p_pBody) const;

	//----------------------------------------------------------------------------------------------
	/// Deactivates the kinetic state of the given body. If the appropriate activity event flag is
	/// set, an activity event is also triggered.
	/// \param p_pBody Managed pointer to a body for which to suspend kinetic activity.
	//----------------------------------------------------------------------------------------------
	void SuspendActivity(
		Meson::Gravitas::Body::BodyPtr p_pBody) const;

	//----------------------------------------------------------------------------------------------
	/// Popagates the activity state between the two given bodies, assumed in contact. If both
	/// bodies are active or inactive, there is no change in kinetic state. Otherwise, the kinetic
	/// state of the suspended body is activated and if applicable, an activity event is triggered.
	/// Invocation of this function has the net effect of propagating activiation between islands
	/// of static bodies coming into contact with an active body.
	/// \param p_pBody1 Managed pointer to the first body involved in activity propagation.
	/// \param p_pBody2 Managed pointer to the second body involved in activity propagation.
	//----------------------------------------------------------------------------------------------
	void PropagateActivity(
		Meson::Gravitas::Body::BodyPtr p_pBody1,
		Meson::Gravitas::Body::BodyPtr p_pBody2) const;
};

Meson_Gravitas_Kinetics_END
