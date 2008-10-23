//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "InstrumentationRenderer.h"

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
InstrumentationRenderer::InstrumentationRenderer(void)
	: m_strName("Instrumentation")
	, m_bIsEnabledLocalEffects(true)
	, m_bIsEnabledGlobalEffects(true)
	, m_pRenderDevice(NULL)
{
	m_pMaterialWireframe = new MaterialRenderState();
	m_pMaterialWireframe->SetWireframe(true);

	m_pCullDisabled = new CullRenderState();
	m_pCullDisabled->SetCulling(false);
}
//----------------------------------------------------------------------------------------------
InstrumentationRenderer::~InstrumentationRenderer(void)
{
	m_pMaterialWireframe = NULL;
	m_pCullDisabled = NULL;

	m_pCubeIndexBuffer = NULL;
	m_pCubeVertexBuffer = NULL;
	m_pCubeVertexDeclaration = NULL;

	m_pSphere = NULL;
}
//----------------------------------------------------------------------------------------------
const String& InstrumentationRenderer::GetName(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
TPointer<IRenderer> InstrumentationRenderer::Clone(bool p_bShallow)
{
	InstrumentationRenderer* pInstrumentationRenderer = new InstrumentationRenderer();

	if (p_bShallow)
	{
		pInstrumentationRenderer->m_pRenderDevice = m_pRenderDevice;
		pInstrumentationRenderer->m_pVisibleEntitySet = m_pVisibleEntitySet;
	}
	else
	{
		throw new MesonException("Method not yet implemented!", __FILE__, __LINE__);
	}

	return RendererPtr(pInstrumentationRenderer);
}
//----------------------------------------------------------------------------------------------
TPointer<IRenderer> InstrumentationRenderer::CreateInstance(void)
{
	return RendererPtr(new InstrumentationRenderer());
}
//----------------------------------------------------------------------------------------------
void InstrumentationRenderer::SetVisibleSet(VisibleEntitySetPtr p_pVisibleEntitySet)
{
	m_pVisibleEntitySet = p_pVisibleEntitySet;
}
//----------------------------------------------------------------------------------------------
void InstrumentationRenderer::CreateInstrumentationShaders(void)
{
	ShaderProgramManager* pShaderProgramManager = m_pRenderDevice->GetShaderProgramManager();

	if (!pShaderProgramManager->ContainsShaderProgram("ShaderCatalogue", "vs_instrumentation", m_pInstrumentationVS))
	{
		String strVertexShader = 
			"float4x4 matWorldViewProjection;"
			"float4 instrumentationColour;"
			"struct VS_INPUT"
			"{"
			" float4 Position : POSITION0;"
			"};"
			"struct VS_OUTPUT"
			"{"
			" float4 Position : POSITION0;"
			"};"
			"VS_OUTPUT vs_main( VS_INPUT Input )"
			"{"
			"	VS_OUTPUT Output;"
			"	Output.Position = mul( Input.Position, matWorldViewProjection );"
			"	return( Output );"
			"};";

		m_pInstrumentationVS = pShaderProgramManager->CreateShaderProgram(IShaderProgram::SPT_VERTEX, "ShaderCatalogue", "vs_instrumentation");
		m_pInstrumentationVS->SetSource(strVertexShader);
		m_pInstrumentationVS->SetEntryPoint("vs_main");
		m_pInstrumentationVS->SetProfile("vs_2_0");
		m_pInstrumentationVS->Compile();

		m_pInstrumentationVS->AddRenderConstant(RenderConstantType::RCT_MATRIX_WORLD_VIEW_PROJECTION);
	}
	
	if (!pShaderProgramManager->ContainsShaderProgram("ShaderCatalogue", "ps_instrumentation", m_pInstrumentationPS))
	{
		String strPixelShader = 
			"float4 ps_main() : COLOR0"
			"{"
			"	return float4(1.0, 1.0, 0.0, 0);" 
			"};";
		
		m_pInstrumentationPS = pShaderProgramManager->CreateShaderProgram(IShaderProgram::SPT_PIXEL, "ShaderCatalogue", "ps_instrumentation");
		m_pInstrumentationPS->SetSource(strPixelShader);
		m_pInstrumentationPS->SetEntryPoint("ps_main");
		m_pInstrumentationPS->SetProfile("ps_2_0");
		m_pInstrumentationPS->Compile();
	}
}

//----------------------------------------------------------------------------------------------
void InstrumentationRenderer::CreateInstrumentationPrimitives(void)
{
	// Cube
	HardwareResourceManager* pHardwareResourceManager = m_pRenderDevice->GetHardwareResourceManager();
	m_pCubeVertexDeclaration = pHardwareResourceManager->CreateVertexDeclaration();
	m_pCubeVertexDeclaration->AddVertexElement(0, 0, VDT_FLOAT3, VDF_POSITION, 0);
	
	m_pCubeVertexBuffer = pHardwareResourceManager->CreateVertexBuffer(m_pCubeVertexDeclaration->GetStreamSize(0), 8, HardwareResource::MP_DEFAULT, HardwareResource::MU_DEFAULT);

	short sIndexBuffer[] = { 0, 1, 1, 2, 2, 3, 3, 0, 
						     0, 4, 1, 5, 2, 6, 3, 7,
						     4, 5, 5, 6, 6, 7, 7, 4 };

	m_pCubeIndexBuffer = pHardwareResourceManager->CreateIndexBuffer(IndexBuffer::IT_16BIT, 24, HardwareResource::MP_DEFAULT, HardwareResource::MU_DEFAULT);
	m_pCubeIndexBuffer->Lock(HardwareResource::ML_NONE);
	m_pCubeIndexBuffer->Write(0, 0, 24 * 2, (const byte*)&sIndexBuffer);
	m_pCubeIndexBuffer->Unlock();

	//Sphere
	m_pSphere = MeshForge::CreateSphereMesh(1, 16, false, MeshForge::MF_NONE);
}

//----------------------------------------------------------------------------------------------
/** Immediate drawing functions
 */
//----------------------------------------------------------------------------------------------
void InstrumentationRenderer::Draw(void)
{
	Draw(m_pVisibleEntitySet);
}
//----------------------------------------------------------------------------------------------
void InstrumentationRenderer::Draw(VisibleEntityPtr p_pVisibleEntity)
{
	if (p_pVisibleEntity->IsEntity())
		ProcessNode(p_pVisibleEntity);
}
//----------------------------------------------------------------------------------------------
void InstrumentationRenderer::Draw(VisibleEntitySetPtr p_pVisibleEntitySet)
{
	if (m_pRenderDevice == NULL)
	{
		m_pRenderDevice = VistasEngine::GetInstance()->GetRenderDeviceManager()->GetActive();
		
		CreateInstrumentationShaders();
		CreateInstrumentationPrimitives();
	}
	
	VisibleEntityEnumerator visibleEnumerator = p_pVisibleEntitySet->GetEnumerator(true);

	// Draw renderable objects, without any global effects
	for (uint uiObjectIndex = 0; visibleEnumerator.HasMoreElements(); uiObjectIndex++)
	{
		VisibleEntityPtr& pVisibleEntity = visibleEnumerator.NextElement();

		if (pVisibleEntity != NULL && pVisibleEntity->IsEntity())
			ProcessNode(pVisibleEntity);
	}
}
//----------------------------------------------------------------------------------------------
void InstrumentationRenderer::ProcessNode(VisibleEntityPtr& p_pVisibleEntity)
{
	ISceneNode* pNode = p_pVisibleEntity->GetEntity();
	DrawBoundingVolume(pNode->GetWorldBound(), (Transformation*)&(pNode->GetWorldTransform()));
}
//----------------------------------------------------------------------------------------------
void InstrumentationRenderer::DrawBoundingVolume(BoundingVolumePtr& p_pBoundingVolume, Transformation* p_pTransformation)
{
	switch(p_pBoundingVolume->GetType())
	{
		case IBoundingVolume::BV_AXIS_ALIGNED_BOX:
		{
			BoundingAxisAlignedBox* pBoundingBox = (BoundingAxisAlignedBox*)(IBoundingVolume*)p_pBoundingVolume;
			DrawAxisAlignedBoundingBox(pBoundingBox, p_pTransformation);
			break;
		}

		case IBoundingVolume::BV_SPHERE:
		{
			BoundingSphere* pBoundingSphere = (BoundingSphere*)(IBoundingVolume*)p_pBoundingVolume;
			DrawBoundingSphere(pBoundingSphere, p_pTransformation);
			break;
		}
	}
}
//----------------------------------------------------------------------------------------------
void InstrumentationRenderer::DrawAxisAlignedBoundingBox(BoundingAxisAlignedBox* p_pBoundingBox, Transformation* p_pTransformation)
{
	Vector3f fMinExtent, 
			 fMaxExtent;

	fMinExtent = p_pBoundingBox->MinExtent;
	fMaxExtent = p_pBoundingBox->MaxExtent;

	Vector3f extents[8];
	
	extents[0].Set(fMinExtent.X, fMaxExtent.Y, fMinExtent.Z);
	extents[1].Set(fMaxExtent.X, fMaxExtent.Y, fMinExtent.Z);
	extents[2].Set(fMaxExtent.X, fMinExtent.Y, fMinExtent.Z);
	extents[3].Set(fMinExtent.X, fMinExtent.Y, fMinExtent.Z);
	extents[4].Set(fMinExtent.X, fMaxExtent.Y, fMaxExtent.Z);
	extents[5].Set(fMaxExtent.X, fMaxExtent.Y, fMaxExtent.Z);
	extents[6].Set(fMaxExtent.X, fMinExtent.Y, fMaxExtent.Z);
	extents[7].Set(fMinExtent.X, fMinExtent.Y, fMaxExtent.Z);
	
	m_pCubeVertexBuffer->Lock(HardwareResource::ML_NONE);
	m_pCubeVertexBuffer->Write(0, 0, sizeof(Vector3f) * 8, (const byte*)extents);
	m_pCubeVertexBuffer->Unlock();

	m_pRenderDevice->BindShaderProgram(m_pInstrumentationVS);
	m_pRenderDevice->BindShaderProgram(m_pInstrumentationPS);

	m_pRenderDevice->SetWorldMatrix(Matrix4f::Identity);
	m_pInstrumentationVS->LoadParameters();
	
	m_pRenderDevice->SetVertexDeclaration(m_pCubeVertexDeclaration);
	m_pRenderDevice->SetStreamSource(0, m_pCubeVertexBuffer);
	m_pRenderDevice->SetIndexBuffer(m_pCubeIndexBuffer);
	m_pRenderDevice->DrawIndexedPrimitive(IRenderDevice::PT_LINELIST_SEGMENTS, 0, 0, 0, 24, 12);
}
//----------------------------------------------------------------------------------------------
void InstrumentationRenderer::DrawBoundingSphere(BoundingSphere* p_pBoundingSphere, Transformation* p_pTransformation)
{
	Matrix4f matSphere(true);
	Vector3f vecRadius(p_pBoundingSphere->Radius);

	matSphere.SetTranslation(p_pBoundingSphere->Centre);
	matSphere.SetScaling(vecRadius);
	
	m_pRenderDevice->SetWorldMatrix(matSphere);

	m_pRenderDevice->BindShaderProgram(m_pInstrumentationVS);
	m_pRenderDevice->BindShaderProgram(m_pInstrumentationPS);

	m_pInstrumentationVS->LoadParameters();

	m_pRenderDevice->PushRenderState(IRenderState::RST_MATERIAL);
	m_pRenderDevice->PushRenderState(IRenderState::RST_CULL);

	m_pRenderDevice->SetMaterialRenderState(m_pMaterialWireframe);
	m_pRenderDevice->SetCullRenderState(m_pCullDisabled);

	m_pSphere->Draw(m_pRenderDevice);

	m_pRenderDevice->PopRenderState(IRenderState::RST_CULL);
	m_pRenderDevice->PopRenderState(IRenderState::RST_MATERIAL);
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END
