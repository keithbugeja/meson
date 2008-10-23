#pragma once

#include "HashMap.h"

#include "GrvNamespaces.h"
#include "GrvSpace.h"

Meson_Gravitas_Space_BEGIN

struct SweepAndPruneEntity;

struct SweepAndPruneLimit
{
	enum LimitType
	{
		Begin,
		End
	};

	static const size_t MAX_AXES = 3;

	SweepAndPruneLimit* Previous[MAX_AXES];
	SweepAndPruneLimit* Next[MAX_AXES];
	Real Value[MAX_AXES];
	LimitType Type;

	SweepAndPruneLimit(void);
	SweepAndPruneLimit(SweepAndPruneLimit::LimitType p_limitType);
	SweepAndPruneEntity* GetEntity(void);
	SweepAndPruneLimit* GetTwinLimit(void);
};

struct SweepAndPruneEntity
{
	SweepAndPruneLimit Begin;
	SweepAndPruneLimit End;
	Meson::Gravitas::Body::BodyPtr Body;

	SweepAndPruneEntity(void);

	const SweepAndPruneEntity& operator=(const SweepAndPruneEntity& p_sweepAndPruneEntity);
	bool operator==(const SweepAndPruneEntity& p_sweepAndPruneEntity) const;
};

class SweepAndPruneSpace
	: public Meson::Gravitas::Space::ISpace
{
private: // private variables
	Meson::Gravitas::Body::BodyArrayList m_listBodies;
	size_t m_unAxes; 
	Meson::Common::Maths::TVector3<Real> m_vecAxes[SweepAndPruneLimit::MAX_AXES];
	SweepAndPruneLimit m_limitMin;
	SweepAndPruneLimit m_limitMax;
	Meson::Gravitas::Body::BodyArrayList m_listBoundedBodies;
	Meson::Gravitas::Body::BodyArrayList m_listUnboundedBodies;
	Meson::Common::Collections::THashMap<Meson::Gravitas::Body::BodyPair, byte>
		m_mapBodyPairIntervalCounts;

public: // public methods
	SweepAndPruneSpace(void);
	SweepAndPruneSpace(const Meson::Common::Text::String& p_strId);
	~SweepAndPruneSpace(void);

	// IGravitasEntity interface
	void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String, PropertyDescriptor>
		&p_mapProperties) const;
	void GetProperty(const Meson::Common::Text::String& p_strName,
		VectorList& p_listValues) const;
	void SetProperty(const Meson::Common::Text::String& p_strName,
		const VectorList& p_listValues);
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
