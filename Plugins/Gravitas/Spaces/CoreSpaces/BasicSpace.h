#pragma once

#include "GrvNamespaces.h"
#include "GrvSpace.h"

Meson_Gravitas_Space_BEGIN

class BasicSpace
	: public Meson::Gravitas::Space::ISpace
{
private: // private variables
	Meson::Gravitas::Body::BodyArrayList m_listBodies;

public: // public methods
	BasicSpace(void);
	BasicSpace(const Meson::Common::Text::String& p_strId);
	~BasicSpace(void);

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
