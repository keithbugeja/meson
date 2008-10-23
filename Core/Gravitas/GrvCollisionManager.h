#pragma once

//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CollisionManager
//	Gravitas collision manager.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		09/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Singleton.h"

#include "GrvNamespaces.h"
#include "GrvCollisionDetector.h"

Meson_Gravitas_Collision_BEGIN

struct CollisionManagerState;

//----------------------------------------------------------------------------------------------
/// \class CollisionManager
/// The physics engine's collision management module maintains a mapping of collision detection
/// algorithms implementing the ICollisionDetector interface for specific IGeometry
/// implementations.
//----------------------------------------------------------------------------------------------
class CollisionManager
	: public Meson::Common::TSingleton<CollisionManager>
{
	friend class Meson::Common::TSingleton<CollisionManager>;

private: // private variables
	CollisionManagerState* m_pCollisionManagerState;

private: // private methods
	//----------------------------------------------------------------------------------------------
	/// Constructs a collision manager.
	//----------------------------------------------------------------------------------------------
	CollisionManager(void);

	//----------------------------------------------------------------------------------------------
	/// Destroys the collision manager.
	//----------------------------------------------------------------------------------------------
	~CollisionManager(void);

public:
	// collision detection

	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of collision detector implementations registered with the manager.
	/// \returns an enumerator of collision detector implementations registered with the manager.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<ICollisionDetector*>&
		GetCollisionDetectors(void);

	//----------------------------------------------------------------------------------------------
	/// Returns true if the manager contains a collision detector implementation for the given
	/// geometry implementations, or false otherwise.
	/// \returns true if the manager contains a collision detector implementation for the given
	///          geometry implementations, or false otherwise.
	//----------------------------------------------------------------------------------------------
	bool HasCollisionDetector(
		Meson::Gravitas::Geometry::IGeometry* p_pGeometry1,
		Meson::Gravitas::Geometry::IGeometry* p_pGeometry2);

	//----------------------------------------------------------------------------------------------
	/// Returns a collision detector implementation for the given geometry implementations. If no
	/// such implementation is registered within the manager, the method returns NULL. 
	/// \returns a collision detector implementation for the given geometry implementations.
	//----------------------------------------------------------------------------------------------
	ICollisionDetector *GetCollisionDetector(
		Meson::Gravitas::Geometry::IGeometry* p_pGeometry1,
		Meson::Gravitas::Geometry::IGeometry* p_pGeometry2);

	//----------------------------------------------------------------------------------------------
	/// Registers the given collision detector algorithm with the manager.
	/// \param p_pCollisionDetector Collision detector to register.
	//----------------------------------------------------------------------------------------------
	void RegisterCollisionDetector(ICollisionDetector* p_pCollisionDetector);

	//----------------------------------------------------------------------------------------------
	/// Unregisters the given collision detector algorithm from the manager.
	/// \param p_pCollisionDetector Collision detector to unregister.
	//----------------------------------------------------------------------------------------------
	void UnregisterCollisionDetector(ICollisionDetector* p_pCollisionDetector);
};

Meson_Gravitas_Collision_END
