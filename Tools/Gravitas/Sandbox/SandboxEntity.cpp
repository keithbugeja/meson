#include "SandboxEntity.h"

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Kinetics;

using namespace Meson::Vistas;

SandboxEntity::SandboxEntity(void)
	: Body(NULL)
	, GeometryNode(NULL)
{
}

SandboxEntity::SandboxEntity(
	BodyPtr p_pBody,
	SceneGeometryNode* p_pGeometryNode)
	: Body(p_pBody)
	, GeometryNode(p_pGeometryNode)
	, InitialKineticProperties(p_pBody->GetKineticProperties())
{
}

SandboxEntity::~SandboxEntity(void)
{
}

void SandboxEntity::Reset(void)
{
	if (Body == NULL)
		return;

	Body->GetKineticProperties() = InitialKineticProperties;
	Update();
}

void SandboxEntity::Update(void)
{
	if (Body == NULL || GeometryNode == NULL)
		return;

	KineticProperties& kineticProperties = Body->GetKineticProperties();
	Meson::Vistas::Transformation transformation(
		kineticProperties.Orientation, kineticProperties.Position);
	GeometryNode->SetLocalTransform(transformation);
}

SandboxEntity& SandboxEntity::operator=(const SandboxEntity& p_sandboxEntity)
{
	Body = p_sandboxEntity.Body;
	GeometryNode = p_sandboxEntity.GeometryNode;
	InitialKineticProperties = p_sandboxEntity.InitialKineticProperties;
	return *this;
}

bool SandboxEntity::operator==(const SandboxEntity& p_sandboxEntity) const
{
	return Body == p_sandboxEntity.Body;
}
