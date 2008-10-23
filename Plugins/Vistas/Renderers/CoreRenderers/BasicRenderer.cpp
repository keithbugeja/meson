//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "BasicRenderer.h"

#include "VstSceneGroupNode.h"
#include "VstSceneGeometryNode.h"

#include "VstMeshForge.h"
#include "VstVistasEngine.h"

#include "ArrayList.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

//#define DEBUG_RENDERER

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
BasicRenderer::BasicRenderer(void)
	: m_strName("Basic")
	, m_bIsEnabledLocalEffects(true)
	, m_bIsEnabledGlobalEffects(true)
	, m_pRenderDevice(NULL)
{
	m_pFirstPassAlpha = new AlphaRenderState();
	m_pFirstPassAlpha->SetAlphaBlend(false);

	m_pNextPassAlpha = new AlphaRenderState();
	m_pNextPassAlpha->SetAlphaBlend(true);
	m_pNextPassAlpha->SetSourceBlendMode(AlphaRenderState::ARSB_ONE);
	m_pNextPassAlpha->SetDestinationBlendMode(AlphaRenderState::ARSB_ONE);
}
//----------------------------------------------------------------------------------------------
BasicRenderer::~BasicRenderer(void)
{
	m_pFirstPassAlpha = NULL;
	m_pNextPassAlpha = NULL;
}
//----------------------------------------------------------------------------------------------
const String& BasicRenderer::GetName(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
TPointer<IRenderer> BasicRenderer::Clone(bool p_bShallow)
{
	BasicRenderer* pBasicRenderer = new BasicRenderer();

	if (p_bShallow)
	{
		pBasicRenderer->m_pRenderDevice = m_pRenderDevice;
		pBasicRenderer->m_pVisibleEntitySet = m_pVisibleEntitySet;
	}
	else
	{
		throw new MesonException("Method not yet implemented!", __FILE__, __LINE__);
	}

	return RendererPtr(pBasicRenderer);
}
//----------------------------------------------------------------------------------------------
TPointer<IRenderer> BasicRenderer::CreateInstance(void)
{
	return RendererPtr(new BasicRenderer());
}
//----------------------------------------------------------------------------------------------
void BasicRenderer::SetVisibleSet(VisibleEntitySetPtr p_pVisibleEntitySet)
{
	m_pVisibleEntitySet = p_pVisibleEntitySet;
}
//----------------------------------------------------------------------------------------------
/** Immediate drawing functions
 */
//----------------------------------------------------------------------------------------------
void BasicRenderer::Draw(void)
{
	Draw(m_pVisibleEntitySet);
}
//----------------------------------------------------------------------------------------------
void BasicRenderer::Draw(VisibleEntityPtr p_pVisibleEntity)
{
	if (p_pVisibleEntity->IsEntity() && p_pVisibleEntity->GetEntity()->IsDerived(SceneGeometryNode::GetStaticType()))
		ProcessGeometryNode(p_pVisibleEntity);
}
//----------------------------------------------------------------------------------------------
void BasicRenderer::Draw(VisibleEntitySetPtr p_pVisibleEntitySet)
{
	if (m_pRenderDevice == NULL)
		m_pRenderDevice = VistasEngine::GetInstance()->GetRenderDeviceManager()->GetActive();
	
	// Effect stack is declared locally, to allow interim calls to the Draw function.
	TStack<uint> pEffectStack;

	// Get enumerator
	VisibleEntityEnumerator visibleEnumerator = p_pVisibleEntitySet->GetEnumerator(true);

	// Split because we don't want to push the condition to the inner loop
	if (m_bIsEnabledGlobalEffects)
	{
		// Loop through enumeration
		for (uint uiObjectIndex = 0; visibleEnumerator.HasMoreElements(); uiObjectIndex++)
		{
			VisibleEntityPtr& pVisibleEntity = visibleEnumerator.NextElement();

			if (pVisibleEntity != NULL)
			{
				if (pVisibleEntity->IsEffect())
				{
					if (pVisibleEntity->GetEntity()->IsDerived(SceneGroupNode::GetStaticType()))
					{
						// Push effect
						if (pVisibleEntity->GetAction() == VisibleEntity::VACT_ENTER)
						{
							pEffectStack.Push(uiObjectIndex);
						} 
						// Pop effect and draw
						else if (pVisibleEntity->GetAction() == VisibleEntity::VACT_LEAVE)
						{
							VisibleEntitySetPtr pEffectSet = m_pVisibleEntitySet->GetVisibleEntitySubset(pEffectStack.Pop(), uiObjectIndex);
							pVisibleEntity->GetEffect()->Draw(m_pRenderDevice, pEffectSet);
						}
					}
				}
				// If effect stack is clear, process node
				else if (pEffectStack.Size() == 0 && pVisibleEntity->IsEntity())
				{
					if (pVisibleEntity->GetEntity()->IsDerived(SceneGeometryNode::GetStaticType()))
						ProcessGeometryNode(pVisibleEntity);
				}
			}
		}
	}
	else
	{
		// Draw renderable objects, without any global effects
		for (uint uiObjectIndex = 0; visibleEnumerator.HasMoreElements(); uiObjectIndex++)
		{
			VisibleEntityPtr& pVisibleEntity = visibleEnumerator.NextElement();

			if (pVisibleEntity != NULL && pVisibleEntity->IsEntity())
			{
				if (pVisibleEntity->GetEntity()->IsDerived(SceneGeometryNode::GetStaticType()))
					ProcessGeometryNode(pVisibleEntity);
			}
		}
	}
}
//----------------------------------------------------------------------------------------------
void BasicRenderer::ProcessGeometryNode(VisibleEntityPtr& p_pVisibleEntity)
{
	SceneGeometryNode* pGeometryNode = (SceneGeometryNode*)p_pVisibleEntity->GetEntity();
	m_pRenderDevice->SetRenderContext(IRenderDevice::RTT_ACTIVE_CONTEXT, pGeometryNode->GetActiveRenderContext());
	
	// Draw material passes
	if (pGeometryNode->GetMaterial() != NULL)
		DrawGeometry(pGeometryNode->GetGeometry(), pGeometryNode->GetMaterial()->GetBestTechnique(), pGeometryNode->GetActiveLightList(), (Transformation*)&(pGeometryNode->GetWorldTransform()));
	
	// Draw additional effect passes
	if (pGeometryNode->GetEffectCount() && m_bIsEnabledLocalEffects)
	{
		EffectEnumerator effectEnumerator = pGeometryNode->GetEffectEnumerator();
		while (effectEnumerator.HasMoreElements())
			effectEnumerator.NextElement()->Draw(m_pRenderDevice, p_pVisibleEntity);
	}
}
//----------------------------------------------------------------------------------------------
void BasicRenderer::DrawGeometry(GeometryPtr& p_pGeometry, TechniquePtr& p_pTechnique, LightListPtr& p_pLightList, Transformation* p_pTransformation, bool p_bFirstPass)
{
	IShaderProgram *pVertexShader,
				   *pPixelShader;

	// Load world matrix for object
	m_pRenderDevice->SetWorldMatrix(p_pTransformation->ExtractMatrix());

	// Begin technique
	p_pTechnique->BeginTechnique();

	// Do ambient only if this is the first time we are drawing this object
	if (p_bFirstPass)
	{
		// First pass is opaque
		m_pRenderDevice->PushRenderState(IRenderState::RST_ALPHA);
		m_pRenderDevice->SetAlphaRenderState(m_pFirstPassAlpha);

		// Draw ambient pass
		if (p_pTechnique->GetPassCount(IPass::PT_AMBIENT_PASS) > 0)
		{
			PassEnumerator passEnumerator = p_pTechnique->GetPassEnumerator(IPass::PT_AMBIENT_PASS);
			while (passEnumerator.HasMoreElements())
			{
				AmbientPass* pPass = (AmbientPass*)(IPass*)passEnumerator.NextElement();
				
				pPass->BeginPass();
				p_pGeometry->Draw(m_pRenderDevice);
				pPass->EndPass();

				m_pRenderDevice->SetAlphaRenderState(m_pNextPassAlpha);
			}
		}
	}
	else
	{
		// Start with alpha blended passes
		m_pRenderDevice->SetAlphaRenderState(m_pNextPassAlpha);
	}

	// Do we have any lights to process?
	if (p_pTechnique->GetPassCount(IPass::PT_ILLUMINATION_PASS) > 0)
	{
		uint uiLightCount = (uint)p_pLightList->Size();

		if (uiLightCount > 0)
		{
			PassEnumerator passEnumerator = p_pTechnique->GetPassEnumerator(IPass::PT_ILLUMINATION_PASS);
			while (passEnumerator.HasMoreElements())
			{
				IlluminationPass* pPass = (IlluminationPass*)(IPass*)passEnumerator.NextElement();
				pPass->BeginPass();
				
				pVertexShader = pPass->GetShaderProgram(IShaderProgram::SPT_VERTEX);
				pPixelShader = pPass->GetShaderProgram(IShaderProgram::SPT_PIXEL);

				for (uint uiLightIndex = 0; uiLightIndex < uiLightCount; uiLightIndex++)
				{
					LightPtr pLight = p_pLightList->GetElement(uiLightIndex);
					
					// Do not process light source if inactive
					if (!pLight->IsLightEnabled())
						continue;

					// Is pass bound to a specific light type?
					if (pPass->IsBoundToLightType())
					{
						if (pPass->GetBoundLightType() != pLight->GetType() )
							continue;
					}
					
					// Is pass bound to a specific named light?
					if (pPass->IsBoundToNamedLight())
					{
						if (pPass->GetBoundNamedLight().Compare(pLight->GetId()) != 0)
							continue;
					}

					// Load light
					m_pRenderDevice->SetLight(0, pLight);

					// Load shader parameters
					pPixelShader->LoadParameters();
					pVertexShader->LoadParameters();

					// Draw geometry
					p_pGeometry->Draw(m_pRenderDevice);
				
					// Next pass is alpha-blended
					m_pRenderDevice->SetAlphaRenderState(m_pNextPassAlpha);
				}
				
				pPass->EndPass();
			}
		}
	}

	// Restore alpha renderstate
	m_pRenderDevice->PopRenderState(IRenderState::RST_ALPHA);

	// Any effects?
	if (p_pTechnique->GetPassCount(IPass::PT_EFFECT_PASS) > 0)
	{
		PassEnumerator passEnumerator = p_pTechnique->GetPassEnumerator(IPass::PT_EFFECT_PASS);

		/** Passes which are 'unrelated' to illumination manage alpha blending
			on their own.
		 */

		while (passEnumerator.HasMoreElements())
		{
			// Begin next pass
			EffectPass* pPass =  (EffectPass*)(IPass*)passEnumerator.NextElement();
			pPass->BeginPass();

			// Draw passes
			for (uint uiPassIteration = 0, uiPassIterationCount = (uint)pPass->GetIterationCount(); 
				 uiPassIteration < uiPassIterationCount; 
				 uiPassIteration++)
			{
				p_pGeometry->Draw(m_pRenderDevice);
			}

			pPass->EndPass();
		}
	}

	p_pTechnique->EndTechnique();
}

Meson_Vistas_END
