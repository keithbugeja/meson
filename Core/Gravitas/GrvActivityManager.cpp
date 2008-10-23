#include "GrvActivityManager.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Event;
using namespace Meson::Gravitas::Kinetics;

Meson_Gravitas_Kinetics_BEGIN

struct ActivityManagerState
{
	Real CurrentTime;
	Real ActivityThreshold;
	Real InactivityTimeThreshold;

	ActivityManagerState(void)
		: CurrentTime((Real) 0.0)
		, ActivityThreshold((Real) 0.5)
		, InactivityTimeThreshold((Real) 2.0)
	{ }
};

Meson_Gravitas_Kinetics_END

ActivityManager::ActivityManager(void)
	: m_pActivityManagerState(NULL)
{
	m_pActivityManagerState = new ActivityManagerState();
}

ActivityManager::~ActivityManager(void)
{
	delete m_pActivityManagerState;
}

void ActivityManager::SetCurrentTime(Real p_rValue)
{
	m_pActivityManagerState->CurrentTime = p_rValue;
}

Real ActivityManager::GetActivityThreshold(void) const
{
	return m_pActivityManagerState->ActivityThreshold;
}

void ActivityManager::SetActivityThreshold(Real p_rValue)
{
	m_pActivityManagerState->ActivityThreshold = p_rValue;
}

Real ActivityManager::GetInactivityTimeThreshold(void) const
{
	return m_pActivityManagerState->InactivityTimeThreshold;
}

void ActivityManager::SetInactivityTimeThreshold(Real p_rValue)
{
	m_pActivityManagerState->InactivityTimeThreshold = p_rValue;
}

void ActivityManager::ResumeActivity(
	Meson::Gravitas::Body::BodyPtr p_pBody) const
{
	KineticProperties& kineticProperties = p_pBody->GetKineticProperties();

	if (kineticProperties.Active)
		return;

	kineticProperties.Active = true;
	kineticProperties.LastActive = m_pActivityManagerState->CurrentTime;

	// generate event if enabled
	EventManager* pEventManager = GravitasEngine::GetInstance()->GetEventManager();
	if (pEventManager->IsEnabled(EventFlags::ActivityResume))
		pEventManager->Notify(ActivityEventPtr(
			new ActivityEvent(p_pBody, ActivityEventType::Resume)));
}

void ActivityManager::SuspendActivity(
	Meson::Gravitas::Body::BodyPtr p_pBody) const
{
	KineticProperties& kineticProperties = p_pBody->GetKineticProperties();

	// do nothing if already asleep
	if(!kineticProperties.Active)
		return;

	/// do nothing if active but can't put to sleep
	if (!kineticProperties.CanSleep)
		return;

	Real rActivity = kineticProperties.GetActivity();

	// reset activity time if currently active and do nothing else
	bool bActivityExceeded = rActivity > m_pActivityManagerState->ActivityThreshold;
	if (bActivityExceeded || kineticProperties.LastActive == (Real) 0.0)
	{
		kineticProperties.LastActive = m_pActivityManagerState->CurrentTime;
		return;
	}

	// otherwise, activity within threshold (candidate for sleep)

	// do put to sleep if not relatively inactive for given time
	Real rActivityDeltaTime = m_pActivityManagerState->CurrentTime
		- kineticProperties.LastActive;
	if (rActivityDeltaTime < m_pActivityManagerState->InactivityTimeThreshold)
		return;

	kineticProperties.LinearVelocity.MakeZero();
	kineticProperties.AngularVelocity.MakeZero();
	kineticProperties.Active = false;

	// generate event if enabled
	EventManager* pEventManager = GravitasEngine::GetInstance()->GetEventManager();
	if (pEventManager->IsEnabled(EventFlags::ActivityResume))
		pEventManager->Notify(ActivityEventPtr(
		new ActivityEvent(p_pBody, ActivityEventType::Suspend)));
}

void ActivityManager::PropagateActivity(
	BodyPtr p_pBody1, BodyPtr p_pBody2) const
{
	bool bBody1Fixed(TMaths<Real>::Equals(p_pBody1->GetMassProperties().MassInverse, (Real) 0.0));
	bool bBody2Fixed(TMaths<Real>::Equals(p_pBody2->GetMassProperties().MassInverse, (Real) 0.0));

	// no activity to propagate if either body is fixed
	if (bBody1Fixed || bBody2Fixed)
		return;

	KineticProperties& kineticProperties1 = p_pBody1->GetKineticProperties();
	KineticProperties& kineticProperties2 = p_pBody2->GetKineticProperties();

	// if both active, or both inactive, nothing to do
	if (kineticProperties1.Active == kineticProperties2.Active)
		return;

	// otherwise, one is active and one isn't
	bool bFirstActive = kineticProperties1.Active;
	
	// ensure both are active
	kineticProperties1.Active = kineticProperties2.Active = true;
	kineticProperties1.LastActive
		= kineticProperties2.LastActive
		= m_pActivityManagerState->CurrentTime;

	// generate event if enabled
	EventManager* pEventManager = GravitasEngine::GetInstance()->GetEventManager();
	if (pEventManager->IsEnabled(EventFlags::ActivityResume))
		pEventManager->Notify(ActivityEventPtr(
		new ActivityEvent(
			bFirstActive ? p_pBody2 : p_pBody1,
			ActivityEventType::Resume)));
}
