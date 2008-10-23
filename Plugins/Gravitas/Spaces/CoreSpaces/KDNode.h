#pragma once

#include "StopWatch.h"

#include "GrvNamespaces.h"
#include "GrvSpace.h"

Meson_Gravitas_Space_BEGIN


//------------------------------------------------------------------------------------------
/// Returns a pointer to the memory manager for Gravitas KD tree nodes.
/// \returns a pointer to the memory manager for Gravitas KD tree nodes.
//------------------------------------------------------------------------------------------
Meson::Common::Memory::MemoryManager* GetKDTreeNodeMemoryManager(void);

static const Meson::Common::Maths::TVector3<Real> KDAxes[3] = {
	Meson::Common::Maths::TVector3<Real>::Right,
	Meson::Common::Maths::TVector3<Real>::Up,
	Meson::Common::Maths::TVector3<Real>::Out};

struct KDNode
{
	enum KDNodeType { X, Y, Z };
	enum PartitionStatus { Straddling, Positive, Negative};

	KDNode* Parent;
	KDNode* Child[2];
	KDNodeType Type;
	Real Partition;
	Meson::Gravitas::Body::BodyArrayList Bodies;

	KDNode(void);
	~KDNode(void);

	void* operator new(size_t p_unSize);
	void operator delete(void* p_pObject, size_t p_unSize);

	bool IsLeaf(void) const;
	PartitionStatus GetPartitionStatus(
		const Meson::Gravitas::Geometry::IBoundingVolume& p_boundingVolume) const;
	void UpdateNode(void);
	void ComputeCandidateCollisions(
		Meson::Gravitas::Body::BodyPtr p_pBodyExternal,
		Meson::Gravitas::Body::BodyPairList& p_listCandidateCollisions);
	void ComputeCandidateCollisions(
		Meson::Gravitas::Body::BodyPairList& p_listCandidateCollisions);
	void IntersectRay(const Meson::Gravitas::Geometry::Ray& p_ray,
		BodyRayIntersectionList& p_listBodyRayIntersections) const;

	void RenderInstrumentation(
		Meson::Gravitas::Instrumentation::IInstrumentationDevice* p_pInstrumentationDevice,
		const Meson::Gravitas::Geometry::BoundingAxisAlignedBox& p_boundingAxisAlignedBox);
};

Meson_Gravitas_Space_END
