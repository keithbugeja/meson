#pragma once

#include "StopWatch.h"

#include "GrvNamespaces.h"
#include "GrvSpace.h"

#include "KDNode.h"

Meson_Gravitas_Space_BEGIN

class KDTreeSpace
	: public Meson::Gravitas::Space::ISpace
{
private: // private variables
	Meson::Gravitas::Body::BodyArrayList m_listBodies;
	KDNode* m_pKDNodeRoot;
	Meson::Common::System::StopWatch m_stopWatchRebuild;
	uint m_unMaximumDepth;
	uint m_unMinimumBodiesPerNode;
	Real m_rRebuildInterval;

private: // private methods
	KDNode* PartitionBodies(uint p_unLevel,
		Meson::Gravitas::Body::BodyList& p_listBodies);
	bool RemoveBodyFromNode(Meson::Gravitas::Body::BodyPtr p_pBody,
		const Meson::Gravitas::Geometry::IBoundingVolume& p_boundingVolume,
		KDNode* p_pKDNode);

public: // public methods
	KDTreeSpace(void);
	KDTreeSpace(const Meson::Common::Text::String& p_strId);
	~KDTreeSpace(void);

	// IGravitasEntity interface
	void RenderInstrumentation(
		Meson::Gravitas::Instrumentation::IInstrumentationDevice* p_pInstrumentationDevice);

	// ISpace interface
	Meson::Common::Collections::TEnumerator<Meson::Gravitas::Body::BodyPtr>&
		GetBodies(void);
	Meson::Gravitas::Body::BodyPtr GetBody(
		const Meson::Common::Text::String& p_strId);
	void AddBody(Meson::Gravitas::Body::BodyPtr p_pBody);
	void RemoveBody(Meson::Gravitas::Body::BodyPtr p_pBody);

	void Rebuild(void);
	void Update(void);

	void ComputeCandidateCollisions(
		Meson::Gravitas::Body::BodyPairList& p_listCandidateCollisions);

	void IntersectRay(const Meson::Gravitas::Geometry::Ray& p_ray,
		BodyRayIntersectionList& p_listBodyRayIntersections) const;
};

Meson_Gravitas_Space_END
