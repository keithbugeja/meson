#include "GrvRigidBody.h"

#include "TreeSet.h"
#include "LinkedList.h"

#include "GrvGravitasEngine.h"
#include "GrvCollisionGroup.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Instrumentation;
using namespace Meson::Gravitas::Kinetics;
using namespace Meson::Gravitas::Materials;

Meson_Gravitas_Body_BEGIN

struct RigidBodyState
{
	TTreeSet<CollisionGroupPtr> CollisionGroups;
	GeometryPtr Geometry;
	BoundingVolumePtr BoundingVolumeLocal, BoundingVolumeWorld;
	MassProperties MassProperties;
	KineticProperties KineticProperties;
	MaterialPtr Material;
	ForceAccumulator m_forceAccumulator;

	RigidBodyState(void)
		: CollisionGroups()
		, Geometry(NULL)
		, BoundingVolumeLocal(NULL)
		, BoundingVolumeWorld(NULL)
		, MassProperties()
		, KineticProperties()
		, Material(NULL)
		, m_forceAccumulator(&MassProperties, &KineticProperties, Geometry)
	{
	}
};

Meson_Gravitas_Body_END

RigidBody::RigidBody(void)
	: m_pRigidBodyState(NULL)
{
	m_pRigidBodyState = new RigidBodyState();
}

RigidBody::RigidBody(const String& p_strId)
	: IBody(p_strId)
	, m_pRigidBodyState(NULL)
{
	m_pRigidBodyState = new RigidBodyState();
}

RigidBody::~RigidBody(void)
{
	delete m_pRigidBodyState;
}

void RigidBody::RenderInstrumentation(IInstrumentationDevice* p_pInstrumentationDevice)
{
	p_pInstrumentationDevice->PushDeviceState();

	// draw local coordinate axes
	KineticProperties& kineticProperties = m_pRigidBodyState->KineticProperties;

	Transform transform(kineticProperties.Orientation, kineticProperties.Position.ToVector());
	TMatrix4<Real> matOrientation;
	transform.ExtractMatrix(matOrientation);
	p_pInstrumentationDevice->SetTransform(matOrientation);

	p_pInstrumentationDevice->DrawLine(
		TPoint3<Real>((Real) -1.0, (Real)  0.0, (Real)  0.0),
		TPoint3<Real>((Real)  1.0, (Real)  0.0, (Real)  0.0));
	p_pInstrumentationDevice->DrawLine(
		TPoint3<Real>((Real)  0.0, (Real) -1.0, (Real)  0.0),
		TPoint3<Real>((Real)  0.0, (Real)  1.0, (Real)  0.0));
	p_pInstrumentationDevice->DrawLine(
		TPoint3<Real>((Real)  0.0, (Real)  0.0, (Real) -1.0),
		TPoint3<Real>((Real)  0.0, (Real)  0.0, (Real)  1.0));

	// draw bounding volume if set
	IBoundingVolume* pBoundingVolume = m_pRigidBodyState->BoundingVolumeWorld;
	if (pBoundingVolume != NULL)
	{
		p_pInstrumentationDevice->SetTransform(TMatrix4<Real>::Identity);
		switch (pBoundingVolume->GetType())
		{
		case BoundingVolumeType::Sphere:
			{
				BoundingSphere* pBoundingSphere = (BoundingSphere*) pBoundingVolume;
				p_pInstrumentationDevice->DrawSphere(pBoundingSphere->Centre, pBoundingSphere->Radius);
			}
			break;
		case BoundingVolumeType::AxisAlignedBox:
			{
				BoundingAxisAlignedBox* pBoundingAxisAlignedBox = (BoundingAxisAlignedBox*) pBoundingVolume;
				p_pInstrumentationDevice->DrawBox(pBoundingAxisAlignedBox->Centre(), pBoundingAxisAlignedBox->Extent());
			}
			break;
		case BoundingVolumeType::OrientedBox:
			{
				BoundingOrientedBox* pBoundingOrientedBox = (BoundingOrientedBox*) pBoundingVolume;
				p_pInstrumentationDevice->DrawBox(
					pBoundingOrientedBox->Orientation(),
					pBoundingOrientedBox->Centre,
					pBoundingOrientedBox->Extent);
			}
			break;
		}
	}

	p_pInstrumentationDevice->PopDeviceState();
}

BodyType::BodyType RigidBody::GetType(void) const
{
	return BodyType::Rigid;
}

TSet<CollisionGroupPtr>& RigidBody::GetCollisionGroups()
{
	return m_pRigidBodyState->CollisionGroups;
}

bool RigidBody::IsMemberOfCollisionGroup(CollisionGroupPtr p_pCollisionGroup) const
{
	return m_pRigidBodyState->CollisionGroups.ContainsElement(p_pCollisionGroup);
}

void RigidBody::AddToCollisionGroup(CollisionGroupPtr p_pCollisionGroup)
{
	m_pRigidBodyState->CollisionGroups.Insert(p_pCollisionGroup);
	p_pCollisionGroup->Add(BodyPtr(this));
}

void RigidBody::RemoveFromCollisionGroup(CollisionGroupPtr p_pCollisionGroup)
{
	TSet<CollisionGroupPtr>& setCollisionGroups = m_pRigidBodyState->CollisionGroups;
	if (setCollisionGroups.ContainsElement(p_pCollisionGroup))
	{
		setCollisionGroups.Remove(p_pCollisionGroup);
		p_pCollisionGroup->Remove(BodyPtr(this));
	}
}

void RigidBody::RemoveFromAllCollisionGroups(void)
{
	// prepare list of groups to remove
	TEnumerator<CollisionGroupPtr>& enumCollisionGroups
		= m_pRigidBodyState->CollisionGroups.GetEnumerator();
	TArrayList<CollisionGroupPtr> listCollisionGroups;
	while (enumCollisionGroups.HasMoreElements())
	{
		CollisionGroupPtr pCollisionGroup
			= enumCollisionGroups.NextElement();
		listCollisionGroups.Add(pCollisionGroup);
	}

	// delete body from each group involved
	for (size_t unIndex = 0; unIndex < listCollisionGroups.Size(); unIndex++)
		listCollisionGroups[unIndex]->Remove(BodyPtr(this));

	// clear body's group list
	m_pRigidBodyState->CollisionGroups.Clear();
}

GeometryPtr RigidBody::GetGeometry(void) const
{
	return m_pRigidBodyState->Geometry;
}

void RigidBody::SetGeometry(GeometryPtr p_pGeometry)
{
	m_pRigidBodyState->Geometry = p_pGeometry;

	// assign type id if not set
	if (p_pGeometry->GetTypeId() < 0)
		p_pGeometry->SetTypeId(
			GravitasEngine::GetInstance()->GetGeometryManager()->GetGeometryTypeId(
				p_pGeometry->GetTypeName()));
}

BoundingVolumePtr RigidBody::GetBoundingVolume(void) const
{
	if (m_pRigidBodyState->BoundingVolumeLocal == NULL)
		return BoundingVolumePtr();
	m_pRigidBodyState->BoundingVolumeWorld->AssignFrom(
		m_pRigidBodyState->BoundingVolumeLocal);

	KineticProperties& kineticProperties = m_pRigidBodyState->KineticProperties;
	Transform transform(kineticProperties.Orientation, kineticProperties.Position.ToVector());

	m_pRigidBodyState->BoundingVolumeWorld->Transform(transform);
	return m_pRigidBodyState->BoundingVolumeWorld;
}

MassProperties& RigidBody::GetMassProperties(void) const
{
	return m_pRigidBodyState->MassProperties;
}

KineticProperties& RigidBody::GetKineticProperties(void) const
{
	return m_pRigidBodyState->KineticProperties;
}

ForceAccumulator& RigidBody::GetForceAccumulator(void)
{
	return m_pRigidBodyState->m_forceAccumulator;
}

void RigidBody::SetGeometryAndMassProperties(
	GeometryPtr p_pGeometry, Real p_rDensity)
{
	SetGeometry(p_pGeometry);
	m_pRigidBodyState->MassProperties.Mass = p_pGeometry->GetVolume() * p_rDensity;
	m_pRigidBodyState->MassProperties.MassInverse
		= (Real) 1.0 / m_pRigidBodyState->MassProperties.Mass;

	m_pRigidBodyState->MassProperties.InertiaTensor = p_pGeometry->GetDistributionTensor() * p_rDensity;
	m_pRigidBodyState->MassProperties.InertiaTensorInverse
		= m_pRigidBodyState->MassProperties.InertiaTensorWorldInverse
		= m_pRigidBodyState->MassProperties.InertiaTensor.InvertCopy();
}

MaterialPtr RigidBody::GetMaterial(void) const
{
	return m_pRigidBodyState->Material;
}

void RigidBody::SetMaterial(MaterialPtr p_pMaterial)
{
	m_pRigidBodyState->Material = p_pMaterial;
}

void RigidBody::SetBoundingVolume(BoundingVolumePtr p_pBoundingVolume)
{
	m_pRigidBodyState->BoundingVolumeLocal = p_pBoundingVolume;

	if (p_pBoundingVolume == NULL)
	{
		m_pRigidBodyState->BoundingVolumeWorld = NULL;
		return;
	}

	m_pRigidBodyState->BoundingVolumeWorld = p_pBoundingVolume->Duplicate();
}

void RigidBody::SetMassProperties(const MassProperties& p_massProperties)
{
	m_pRigidBodyState->MassProperties = p_massProperties;
}
