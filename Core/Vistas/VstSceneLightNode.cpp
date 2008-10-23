//----------------------------------------------------------------------------------------------
#include "VstSceneLightNode.h"
#include "VstBoundingSphere.h"
#include "VstCuller.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
SceneLightNode::SceneLightNode(Scenegraph* p_pOwner)
	: ISceneNode(p_pOwner)
	, m_pLight(NULL)
{
}
//----------------------------------------------------------------------------------------------
SceneLightNode::SceneLightNode(const String& p_strId, Scenegraph* p_pOwner)
	: ISceneNode(p_strId, p_pOwner)
	, m_pLight(NULL)
{
}
//----------------------------------------------------------------------------------------------
SceneLightNode::~SceneLightNode(void)
{
	m_pLight = NULL;
}
//----------------------------------------------------------------------------------------------
void SceneLightNode::UpdateWorldSynchronisation(double p_dfApplicationTime)
{
	ISceneNode::UpdateWorldSynchronisation(p_dfApplicationTime);

	Matrix3f mat3Rotation = WorldTransformation.GetRotationInMatrixForm();
	m_pLight->SetFrame(WorldTransformation.GetTranslation(), 
						mat3Rotation.GetColumn(0),
						mat3Rotation.GetColumn(1),
						mat3Rotation.GetColumn(2));
}
//----------------------------------------------------------------------------------------------
void SceneLightNode::UpdateWorldBound(void)
{
	if (ISceneNode::WorldBound->GetType() == IBoundingVolume::BV_SPHERE)
	{
		BoundingSphere* pBoundingSphere = (BoundingSphere*)(IBoundingVolume*)ISceneNode::WorldBound;
		pBoundingSphere->Centre = m_pLight->GetPosition();
		pBoundingSphere->Radius = m_pLight->GetRange();
	}
	else
		ISceneNode::WorldBound = new BoundingSphere(m_pLight->GetPosition(), m_pLight->GetRange());
}
//----------------------------------------------------------------------------------------------
void SceneLightNode::SetLight(LightPtr& p_pLight)
{
	m_pLight = p_pLight;

	LocalTransformation.SetTranslation(m_pLight->GetPosition());
	LocalTransformation.SetRotation(Matrix3f(m_pLight->GetVerticalAxis(), m_pLight->GetDirection(), m_pLight->GetHorizontalAxis(), false));
}
//----------------------------------------------------------------------------------------------
LightPtr SceneLightNode::GetLight(void)
{
	return m_pLight;
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END