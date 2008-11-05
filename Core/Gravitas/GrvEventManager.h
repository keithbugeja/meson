//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Event::EventManager
//	Gravitas event manager.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		06/03/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Singleton.h"
#include "Enumerator.h"

#include "GrvNamespaces.h"
#include "GrvActivityListener.h"
#include "GrvCollisionFilterListener.h"
#include "GrvCollisionListener.h"
#include "GrvConstraintListener.h"
#include "GrvMaterialListener.h"

Meson_Gravitas_Event_BEGIN

namespace EventFlags
{
	//----------------------------------------------------------------------------------------------
	/// Event activity bit flags.
	//----------------------------------------------------------------------------------------------
	enum EventFlags
	{
		None              = 0,
		ActivitySuspend   = 1,
		ActivityResume    = 2,
		CollisionFilter   = 4,
		CollisionStart    = 8,
		CollisionOngoing  = 16,
		CollisionEnd      = 32,
		ConstraintBroken  = 64,
		Material          = 128,
		CollisionAll      = CollisionStart | CollisionOngoing | CollisionEnd,
		All               = CollisionFilter | CollisionAll | ConstraintBroken
	};
}

//----------------------------------------------------------------------------------------------
/// Returns an OR'ed result of the same type as the two given event flags.
/// \param p_eventFlags1 First event flag set to OR
/// \param p_eventFlags2 Second event flag set to OR
/// \returns an OR'ed result of the same type as the two given event flags.
//----------------------------------------------------------------------------------------------
EventFlags::EventFlags operator|(
	EventFlags::EventFlags p_eventFlags1,
	EventFlags::EventFlags p_eventFlags2);

struct EventManagerState;

//----------------------------------------------------------------------------------------------
/// \class EventManager
/// The physics engine's event manager module provides event subscription and generation
/// services to all the other modules. Event management in Gravitas follows a subscriber /
/// publisher design pattern. Entities interested in listening to specific engine events must
/// implement the appropriate manager interface. This interface typically defines a method
/// with the naming convention OnXXXXEvent(XXXXEventPtr) that accepts a managed pointer to an
/// associated event type containing data related to the event. Some event types also allow a
/// callback functionlity through modification of event properties. A typical example are
/// constraint filtering events, that enable the event listener to change the outcome of the
/// collision filter for a given pair of bodies.
//----------------------------------------------------------------------------------------------
class GRAVITAS_API EventManager
	: public Meson::Common::TSingleton<EventManager>
{
	friend class Meson::Common::TSingleton<EventManager>;

private: // private variables
	EventManagerState* m_pEventManagerState;

private: // private methods
	EventManager(void);
	~EventManager(void);

public: // public methods

	// event flags

	//----------------------------------------------------------------------------------------------
	/// Returns true if the events defined in the event flag set are all enabled.
	/// \param p_eventFlags A set of event flags.
	/// \returns true if the events defined in the event flag set are all enabled.
	//----------------------------------------------------------------------------------------------
	bool IsEnabled(EventFlags::EventFlags p_eventFlags) const;

	//----------------------------------------------------------------------------------------------
	/// Enables event types through the given event flags.
	/// \param p_eventFlags A set of event flags to enable.
	//----------------------------------------------------------------------------------------------
	void Enable(EventFlags::EventFlags p_eventFlags);

	//----------------------------------------------------------------------------------------------
	/// Disables event types through the given event flags.
	/// \param p_eventFlags A set of event flags to disable.
	//----------------------------------------------------------------------------------------------
	void Disable(EventFlags::EventFlags p_eventFlags);

	// activity events

	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of subscribed activity event listeners.
	/// \returns an enumerator of subscribed activity event listeners.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<ActivityListenerPtr>& GetActivityListeners(void);

	//----------------------------------------------------------------------------------------------
	/// Subscribes the given activity event listener with the event manager.
	/// \param p_pActivityListener Managed pointer to an activity event listener.
	//----------------------------------------------------------------------------------------------
	void AddListener(ActivityListenerPtr p_pActivityListener);

	//----------------------------------------------------------------------------------------------
	/// Unsubscribes the given activity event listener from the event manager.
	/// \param p_pActivityListener Managed pointer to an activity event listener.
	//----------------------------------------------------------------------------------------------
	void RemoveListener(ActivityListenerPtr p_pActivityListener);

	//----------------------------------------------------------------------------------------------
	/// Notifies all registered activity event subscribers using the given event.
	/// \param p_pActivityEvent Managed pointer to an activity event.
	//----------------------------------------------------------------------------------------------
	void Notify(ActivityEventPtr p_pActivityEvent) const;

	// collision filter events

	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of subscribed collision filter event listeners.
	/// \returns an enumerator of subscribed collision filter event listeners.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<CollisionFilterListenerPtr>&
		GetCollisionFilterListeners(void);

	//----------------------------------------------------------------------------------------------
	/// Subscribes the given collision filter event listener with the event manager.
	/// \param p_pCollisionFilterListener Managed pointer to a collision filter event listener.
	//----------------------------------------------------------------------------------------------
	void AddListener(
		CollisionFilterListenerPtr p_pCollisionFilterListener);

	//----------------------------------------------------------------------------------------------
	/// Unsubscribes the given collision filter event listener from the event manager.
	/// \param p_pCollisionFilterListener Managed pointer to a collision filter event listener.
	//----------------------------------------------------------------------------------------------
	void RemoveListener(
		CollisionFilterListenerPtr p_pCollisionFilterListener);

	//----------------------------------------------------------------------------------------------
	/// Notifies all registered collision filter event subscribers using the given event.
	/// \param p_pCollisionFilterEvent Managed pointer to a collision filter event.
	//----------------------------------------------------------------------------------------------
	void Notify(CollisionFilterEventPtr p_pCollisionFilterEvent) const;

	// collision events

	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of subscribed collision event listeners.
	/// \returns an enumerator of subscribed collision event listeners.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<CollisionListenerPtr>& GetCollisionListeners(void);

	//----------------------------------------------------------------------------------------------
	/// Subscribes the given collision event listener with the event manager.
	/// \param p_pCollisionListener Managed pointer to a collision event listener.
	//----------------------------------------------------------------------------------------------
	void AddListener(CollisionListenerPtr p_pCollisionListener);

	//----------------------------------------------------------------------------------------------
	/// Unsubscribes the given collision event listener from the event manager.
	/// \param p_pCollisionListener Managed pointer to a collision event listener.
	//----------------------------------------------------------------------------------------------
	void RemoveListener(CollisionListenerPtr p_pCollisionListener);

	//----------------------------------------------------------------------------------------------
	/// Notifies all registered collision event subscribers using the given event.
	/// \param p_pCollisionEvent Managed pointer to a collision event.
	//----------------------------------------------------------------------------------------------
	void Notify(CollisionEventPtr p_pCollisionEvent) const;

	// constraint events

	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of subscribed constraint event listeners.
	/// \returns an enumerator of subscribed constraint event listeners.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<ConstraintListenerPtr>& GetConstraintListeners(void);

	//----------------------------------------------------------------------------------------------
	/// Subscribes the given constraint event listener with the event manager.
	/// \param p_pConstraintListener Managed pointer to a constraint event listener.
	//----------------------------------------------------------------------------------------------
	void AddListener(ConstraintListenerPtr p_pConstraintListener);

	//----------------------------------------------------------------------------------------------
	/// Unsubscribes the given constraint event listener from the event manager.
	/// \param p_pConstraintListener Managed pointer to a constraint event listener.
	//----------------------------------------------------------------------------------------------
	void RemoveListener(ConstraintListenerPtr p_pConstraintListener);

	//----------------------------------------------------------------------------------------------
	/// Notifies all registered constraint event subscribers using the given event.
	/// \param p_pConstraintEvent Managed pointer to a constraint event.
	//----------------------------------------------------------------------------------------------
	void Notify(ConstraintEventPtr p_pConstraintEvent) const;

	// material events

	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of subscribed material event listeners.
	/// \returns an enumerator of subscribed material event listeners.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<MaterialListenerPtr>& GetMaterialListeners(void);

	//----------------------------------------------------------------------------------------------
	/// Subscribes the given material event listener with the event manager.
	/// \param p_pMaterialtListener Managed pointer to a material event listener.
	//----------------------------------------------------------------------------------------------
	void AddListener(MaterialListenerPtr p_pMaterialtListener);

	//----------------------------------------------------------------------------------------------
	/// Unsubscribes the given material event listener from the event manager.
	/// \param p_pMaterialtListener Managed pointer to a material event listener.
	//----------------------------------------------------------------------------------------------
	void RemoveListener(MaterialListenerPtr p_pMaterialtListener);

	//----------------------------------------------------------------------------------------------
	/// Notifies all registered material event listeners using the given event.
	/// \param p_pMaterialEvent Managed pointer to a material event.
	//----------------------------------------------------------------------------------------------
	void Notify(MaterialEventPtr p_pMaterialEvent) const;
};

Meson_Gravitas_Event_END
