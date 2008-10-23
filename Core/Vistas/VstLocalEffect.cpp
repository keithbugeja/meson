#include "VstLocalEffect.h"

#include "VstSceneNode.h"
#include "VstSceneGeometryNode.h"

Meson_Vistas_BEGIN
		
//----------------------------------------------------------------------------------------------
LocalEffect::LocalEffect(void) 
{ 
}
//----------------------------------------------------------------------------------------------
LocalEffect::~LocalEffect(void) 
{ 
}
//----------------------------------------------------------------------------------------------
void LocalEffect::Initialise(IRenderDevice* p_pRenderDevice) 
{ 
}
//----------------------------------------------------------------------------------------------
void LocalEffect::Dispose(IRenderDevice* p_pRenderDevice) 
{ 
}
//----------------------------------------------------------------------------------------------
void LocalEffect::Draw(IRenderDevice* p_pRenderDevice, VisibleEntityPtr p_pVisibleEntity)
{ 
	MESON_ASSERT(p_pVisibleEntity->GetEntity()->IsDerived(SceneGeometryNode::GetStaticType()), "Entity is not drawable : Drawable nodes inherit from SceneGeometryNode");

	SceneGeometryNode* pNode = ((SceneGeometryNode*)p_pVisibleEntity->GetEntity());
	p_pRenderDevice->SetWorldMatrix(pNode->WorldTransformation.ExtractMatrix());
	pNode->GetGeometry()->Draw(p_pRenderDevice);
}
//----------------------------------------------------------------------------------------------		
void LocalEffect::Draw(IRenderDevice* p_pRenderDevice, VisibleEntitySetPtr p_pVisibleEntitySet) 
{
	ISceneNode* pNode = NULL;

	for (uint uiIndex = 0; uiIndex < p_pVisibleEntitySet->Size(); uiIndex++)
	{
		pNode = (ISceneNode*)p_pVisibleEntitySet->GetElement(uiIndex)->GetEntity();
		if (pNode->IsDerived(SceneGeometryNode::GetStaticType()))
		{
			p_pRenderDevice->SetWorldMatrix(pNode->WorldTransformation.ExtractMatrix());
			((SceneGeometryNode*)pNode)->GetGeometry()->Draw(p_pRenderDevice);
		}
	}
}
//----------------------------------------------------------------------------------------------		

Meson_Vistas_END