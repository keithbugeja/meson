#pragma once

#include "GrvBody.h"

Meson_Gravitas_Body_BEGIN

struct RigidBodyState;

//----------------------------------------------------------------------------------------------
/// \class RigidBody
/// An idealised rigid body implementation.
//----------------------------------------------------------------------------------------------
class RigidBody
	: public IBody
{
private: // private variables
	RigidBodyState *m_pRigidBodyState;

public:
	//----------------------------------------------------------------------------------------------
	/// Constructs a new rigid body instance.
	//----------------------------------------------------------------------------------------------
	RigidBody(void);

	//----------------------------------------------------------------------------------------------
	/// Constructs a new rigid body instance with the given ID.
	/// \param p_strId ID to assign to the rigid body instance.
	//----------------------------------------------------------------------------------------------
	RigidBody(const Meson::Common::Text::String& p_strId);

	//----------------------------------------------------------------------------------------------
	/// Destroys a rigid body instance.
	//----------------------------------------------------------------------------------------------
	~RigidBody(void);

	// IGravitasEntity interface
	void RenderInstrumentation(
		Meson::Gravitas::Instrumentation::IInstrumentationDevice* p_pInstrumentationDevice);

	// IBody interface
	BodyType::BodyType GetType(void) const;

	Meson::Common::Collections::TSet<Meson::Gravitas::Collision::CollisionGroupPtr>& GetCollisionGroups();
	bool IsMemberOfCollisionGroup(Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup) const;
	void AddToCollisionGroup(Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup);
	void RemoveFromCollisionGroup(Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup);
	void RemoveFromAllCollisionGroups(void);

	Meson::Gravitas::Geometry::GeometryPtr GetGeometry(void) const;
	void SetGeometry(Meson::Gravitas::Geometry::GeometryPtr p_pGeometry);
	Meson::Gravitas::Geometry::BoundingVolumePtr GetBoundingVolume(void) const;
	MassProperties& GetMassProperties(void) const;
	Meson::Gravitas::Materials::MaterialPtr GetMaterial(void) const;
	Meson::Gravitas::Kinetics::KineticProperties& GetKineticProperties(void) const;
	Meson::Gravitas::Force::ForceAccumulator& GetForceAccumulator(void);

	// custom interface

	//----------------------------------------------------------------------------------------------
	/// Sets the body's geometry reference and computes its mass properties using the given
	/// geometry and uniform density value.
	/// \param p_pGeometry A managed geometry object reference to assign to the body. The geometry
	///        is also used to compute the body's mass properties.
	/// \param p_rDensity A uniform density value used to compute the body's mass properties.
	//----------------------------------------------------------------------------------------------
	void SetGeometryAndMassProperties(
		Meson::Gravitas::Geometry::GeometryPtr p_pGeometry, Real p_rDensity);

	//----------------------------------------------------------------------------------------------
	/// Sets the body's material reference. The material may be shared by any number
	/// of bodies.
	/// \param p_pMaterial material reference to assign to the body.
	//----------------------------------------------------------------------------------------------
	void SetMaterial(Meson::Gravitas::Materials::MaterialPtr p_pMaterial);

	//----------------------------------------------------------------------------------------------
	/// Sets the body's bounding volume with the given reference. The bounding volume must be given
	/// in local body coordinates.
	/// \param p_pBoundingVolume A managed reference to a bounding volume in local body space.
	//----------------------------------------------------------------------------------------------
	void SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr p_pBoundingVolume);

	//----------------------------------------------------------------------------------------------
	/// Sets the body's mass properties directly from the given source. This allows custom
	/// computation of a body's mass properties, independent of its geometry.
	/// \param p_massProperties A reference to a mass properties source structure.
	//----------------------------------------------------------------------------------------------
	void SetMassProperties(const MassProperties& p_massProperties);
};

//----------------------------------------------------------------------------------------------
/// A managed refenrece to a RigidBody.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<RigidBody> RigidBodyPtr;

Meson_Gravitas_Body_END
