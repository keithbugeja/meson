#pragma once

#include "GrvBody.h"
#include "VstSceneGeometryNode.h"

struct SandboxEntity
{
	Meson::Gravitas::Body::BodyPtr Body;
	Meson::Vistas::SceneGeometryNode* GeometryNode;
	Meson::Gravitas::Kinetics::KineticProperties InitialKineticProperties;

	SandboxEntity(void);
	SandboxEntity(
		Meson::Gravitas::Body::BodyPtr p_pBody,
		Meson::Vistas::SceneGeometryNode* p_pGeometryNode);
	~SandboxEntity(void);
	void Reset(void);
	void Update(void);

	SandboxEntity& operator=(const SandboxEntity& p_sandboxEntity);
	bool operator==(const SandboxEntity& p_sandboxEntity) const;
};
