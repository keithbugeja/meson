//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstRenderDeviceAdapter.h"
#include "VstRenderConstant.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Config;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
RenderDeviceAdapter::RenderDeviceAdapter(void) 
	: m_pViewport(NULL)
	, m_pLights(NULL)
	, m_pRenderTarget(NULL)
	, m_pRenderContext(NULL)
	, m_pSamplerStates(NULL)
	, m_pHardwareResourceManager(NULL)
	, m_pShaderProgramManager(NULL)
	, m_eColourMask(IRenderDevice::CM_RED_GREEN_BLUE_ALPHA)
{
	// Clear shader program placeholders
	memset(m_pShaderPrograms, 0, sizeof(IShaderProgram*) * IShaderProgram::SPT_COUNT);
}
//----------------------------------------------------------------------------------------------
RenderDeviceAdapter::~RenderDeviceAdapter(void) 
{
	m_pRenderContext = NULL;
}
//----------------------------------------------------------------------------------------------
WindowRenderTarget* RenderDeviceAdapter::Startup(void)
{
	m_pGlobalRenderContext = new RenderContext();
	m_pRenderContext = new RenderContext(m_pGlobalRenderContext);

	for (int nIndex = 0; nIndex < RenderConstantType::RCT_COUNT; nIndex++)
		m_pRenderConstants[nIndex] = new RenderConstant((RenderConstantType::RenderConstantType)nIndex, this);

	return NULL;
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::Shutdown(void)
{
	m_pRenderContext = NULL;

	for (int nIndex = 0; nIndex < RenderConstantType::RCT_COUNT; nIndex++)
		SAFE_DELETE(m_pRenderConstants[nIndex]);
}
//----------------------------------------------------------------------------------------------
RenderConstant* RenderDeviceAdapter::GetRenderConstant(RenderConstantType::RenderConstantType p_eRenderConstantType)
{
	return m_pRenderConstants[p_eRenderConstantType];
}
//----------------------------------------------------------------------------------------------
Configuration* RenderDeviceAdapter::GetConfiguration(void)
{
	return &m_configuration;
}
//----------------------------------------------------------------------------------------------
HardwareResourceManager* RenderDeviceAdapter::GetHardwareResourceManager(void)
{
	return m_pHardwareResourceManager;
}
//----------------------------------------------------------------------------------------------
ShaderProgramManager* RenderDeviceAdapter::GetShaderProgramManager(void)
{
	return m_pShaderProgramManager;
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::AttachRenderTarget(RenderTarget* p_pRenderTarget)
{
	MESON_ASSERT(p_pRenderTarget != NULL, "Render target instance is NULL!");

	if ( p_pRenderTarget != NULL )
	{
		String strTargetName = p_pRenderTarget->GetName();
		
		if(m_renderTargetMap.ContainsKey(strTargetName))
			throw new MesonException("Render target name is not unique.", __FILE__, __LINE__);
			
		m_renderTargetMap.Insert(p_pRenderTarget->GetName(), p_pRenderTarget);
	}
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::DetachRenderTarget(const String& p_strRenderTargetName)
{
	if (!m_renderTargetMap.ContainsKey(p_strRenderTargetName))
		throw new MesonException("Render target does not exist.", __FILE__, __LINE__);
	
	m_renderTargetMap.RemoveKey(p_strRenderTargetName);
}
//----------------------------------------------------------------------------------------------
RenderTargetEnumerator RenderDeviceAdapter::GetRenderTargetEnumeration(void)
{
	return m_renderTargetMap.GetValueEnumerator();
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetProjectionMatrix(const Meson::Common::Maths::Matrix4f& p_projectionMatrix)
{
	m_projectionMatrix = p_projectionMatrix;
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetWorldMatrix(const Meson::Common::Maths::Matrix4f& p_worldMatrix)
{
	m_worldMatrix = p_worldMatrix;
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetViewMatrix(const Meson::Common::Maths::Matrix4f& p_viewMatrix)
{
	m_viewMatrix = p_viewMatrix;
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Maths::Matrix4f& RenderDeviceAdapter::GetProjectionMatrix(void)
{
	return m_projectionMatrix;
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Maths::Matrix4f& RenderDeviceAdapter::GetViewMatrix(void)
{
	return m_viewMatrix;
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Maths::Matrix4f& RenderDeviceAdapter::GetWorldMatrix(void)
{
	return m_worldMatrix;
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetViewport(Viewport* p_pViewport)
{
	m_pViewport = p_pViewport;
}
//----------------------------------------------------------------------------------------------
Viewport* RenderDeviceAdapter::GetViewport(void)
{
	return m_pViewport;
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetColourMask(IRenderDevice::ColourMask p_eColourMask)
{
	m_eColourMask = p_eColourMask;
}
//----------------------------------------------------------------------------------------------
IRenderDevice::ColourMask RenderDeviceAdapter::GetColourMask(void)
{
	return m_eColourMask;
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetSamplerState(uint p_uiSamplerIndex, const SamplerState* p_pSamplerState)
{
	MESON_ASSERT(p_uiSamplerIndex >= 0 && 
		p_uiSamplerIndex < GetDeviceCapabilities(IRenderDevice::DCI_MAXIMUM_SIMULATANEOUS_TEXTURES), 
		"Failed to set sampler : Sampler index is invalid.");

	if (p_pSamplerState != NULL)
		 m_pSamplerStates[p_uiSamplerIndex] = *p_pSamplerState;
}
//----------------------------------------------------------------------------------------------
const SamplerState* RenderDeviceAdapter::GetSamplerState(uint p_uiSamplerIndex)
{
	return &m_pSamplerStates[p_uiSamplerIndex];
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetLight(uint p_uiLightIndex, const Light* p_pLight)
{
	MESON_ASSERT(p_uiLightIndex >= 0 && 
		p_uiLightIndex < GetDeviceCapabilities(IRenderDevice::DCI_MAXIMUM_LIGHTS), 
		"Failed to set light : Light index is invalid.");

	if (p_pLight != NULL)
		m_pLights[p_uiLightIndex] = *p_pLight;
	else
		m_pLights[p_uiLightIndex].SetLight(false);
}
//----------------------------------------------------------------------------------------------
const Light* RenderDeviceAdapter::GetLight(uint p_nLightIndex) const
{
	return &m_pLights[p_nLightIndex];
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::LoadShaderParameters(IShaderProgram* p_pShaderProgram)
{
	MESON_ASSERT(p_pShaderProgram != NULL, 
		"Failed to load shader parameters : Shader program is invalid.");
	
	p_pShaderProgram->LoadParameters();
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::BindShaderProgram(IShaderProgram* p_pShaderProgram)
{
	MESON_ASSERT(p_pShaderProgram != NULL, 
		"Failed to bind shader program: Shader program is invalid.");
	
	if (m_pShaderPrograms[p_pShaderProgram->GetType()] != NULL)
		UnbindShaderProgram(p_pShaderProgram->GetType());

	p_pShaderProgram->Bind();
	m_pShaderPrograms[p_pShaderProgram->GetType()] = p_pShaderProgram;
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::UnbindShaderProgram(IShaderProgram::ShaderProgramType p_eShaderProgramType)
{
	if ( m_pShaderPrograms[p_eShaderProgramType] == NULL)
		throw new MesonException("Cannot unbind shader program : no program currently bound.", __FILE__, __LINE__);

	m_pShaderPrograms[p_eShaderProgramType]->Unbind();
	m_pShaderPrograms[p_eShaderProgramType] = NULL;
}
//----------------------------------------------------------------------------------------------
IShaderProgram* RenderDeviceAdapter::GetShaderProgram(IShaderProgram::ShaderProgramType p_eShaderProgramType)
{
	return m_pShaderPrograms[p_eShaderProgramType];
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetRenderTarget(const Meson::Common::Text::String& p_strName)
{
	RenderTarget* pRenderTarget;

	if (m_renderTargetMap.ContainsKey(p_strName, pRenderTarget))
	{
		if (m_pRenderTarget != NULL)
			m_pRenderTarget->SetActive(false);

		m_pRenderTarget = pRenderTarget;
		m_pRenderTarget->SetActive(true);
	}
}
//----------------------------------------------------------------------------------------------
RenderTarget* RenderDeviceAdapter::GetRenderTarget(void)
{
	return m_pRenderTarget;
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::PushRenderTarget(void)
{
	if (m_pRenderTarget != NULL)
		m_renderTargetStack.Push(m_pRenderTarget);
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::PopRenderTarget(void)
{
	if (!m_renderTargetStack.IsEmpty())
		SetRenderTarget(m_renderTargetStack.Pop()->GetName());
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SwitchRenderContext(RenderContextPtr p_pOldContext, RenderContextPtr p_pNewContext)
{
	if (m_pRenderContext != p_pNewContext)
	{
		m_pRenderContext = p_pNewContext;

		for (uint uiStateIndex = 0; uiStateIndex < IRenderState::RST_COUNT; uiStateIndex++)
		{
			RenderStatePtr p1(p_pOldContext->GetRenderState((IRenderState::RenderStateType)uiStateIndex));
			RenderStatePtr p2(p_pNewContext->GetRenderState((IRenderState::RenderStateType)uiStateIndex));

			if (p_pOldContext->GetRenderState((IRenderState::RenderStateType)uiStateIndex) != p_pNewContext->GetRenderState((IRenderState::RenderStateType)uiStateIndex))
				SetRenderState(p_pNewContext->GetRenderState((IRenderState::RenderStateType)uiStateIndex));
		}
	}
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetRenderContext(RenderContextType p_eRenderContextType, RenderContextPtr p_pRenderContext)
{
	MESON_ASSERT(p_pRenderContext != NULL, "Render context is invalid!"); 

	if (p_eRenderContextType == RTT_GLOBAL_CONTEXT)
		m_pGlobalRenderContext = p_pRenderContext;
	else 
		if (p_eRenderContextType == RTT_ACTIVE_CONTEXT)
			SwitchRenderContext(m_pRenderContext, p_pRenderContext);
}
//----------------------------------------------------------------------------------------------
RenderContextPtr RenderDeviceAdapter::GetRenderContext(RenderContextType p_eRenderContextType)
{
	if (p_eRenderContextType == RTT_GLOBAL_CONTEXT)
		return m_pGlobalRenderContext;

	return m_pRenderContext;
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::RestoreRenderContext(void)
{
	SwitchRenderContext(m_pRenderContext, m_pGlobalRenderContext);
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetRenderState(IRenderState* p_pRenderState)
{
	switch(p_pRenderState->GetType())
	{
		case IRenderState::RST_ALPHA:
			SetAlphaRenderState((AlphaRenderState*)p_pRenderState);
			break;

		case IRenderState::RST_BIAS:
			SetBiasRenderState((BiasRenderState*)p_pRenderState);
			break;

		case IRenderState::RST_CULL:
			SetCullRenderState((CullRenderState*)p_pRenderState);
			break;

		case IRenderState::RST_DEPTH:
			SetDepthRenderState((DepthRenderState*)p_pRenderState);
			break;

		case IRenderState::RST_LIGHT:
			SetLightRenderState((LightRenderState*)p_pRenderState);
			break;

		case IRenderState::RST_MATERIAL:
			SetMaterialRenderState((MaterialRenderState*)p_pRenderState);
			break;

		case IRenderState::RST_SAMPLER:
			SetSamplerRenderState((SamplerRenderState*)p_pRenderState);
			break;

		case IRenderState::RST_STENCIL:
			SetStencilRenderState((StencilRenderState*)p_pRenderState);
			break;

		case IRenderState::RST_TARGET:
			SetTargetRenderState((TargetRenderState*)p_pRenderState);
			break;
	}
}
//----------------------------------------------------------------------------------------------
IRenderState* RenderDeviceAdapter::GetRenderState(IRenderState::RenderStateType p_eRenderStateType)
{
	return m_pRenderContext->GetRenderState(p_eRenderStateType);
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::PushRenderState(IRenderState::RenderStateType p_eRenderStateType)
{
	m_renderStateStack[p_eRenderStateType].Push(m_pRenderContext->GetRenderState(p_eRenderStateType));
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::PopRenderState(IRenderState::RenderStateType p_eRenderStateType)
{
	if (!m_renderStateStack[p_eRenderStateType].IsEmpty())
		SetRenderState(m_renderStateStack[p_eRenderStateType].Pop());
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetAlphaRenderState(AlphaRenderState* p_pAlphaRenderState) 
{ 
	m_pRenderContext->SetRenderState(p_pAlphaRenderState);
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetBiasRenderState(BiasRenderState* p_pBiasRenderState)
{
	m_pRenderContext->SetRenderState(p_pBiasRenderState);
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetCullRenderState(CullRenderState* p_pCullRenderState) 
{ 
	m_pRenderContext->SetRenderState(p_pCullRenderState);
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetDepthRenderState(DepthRenderState* p_pDepthRenderState) 
{
	m_pRenderContext->SetRenderState(p_pDepthRenderState);
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetLightRenderState(LightRenderState* p_pLightRenderState) 
{ 
	m_pRenderContext->SetRenderState(p_pLightRenderState);
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetMaterialRenderState(MaterialRenderState* p_pMaterialRenderState) 
{ 
	m_pRenderContext->SetRenderState(p_pMaterialRenderState);
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetSamplerRenderState(SamplerRenderState* p_pSamplerRenderState) 
{ 
	m_pRenderContext->SetRenderState(p_pSamplerRenderState);
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetStencilRenderState(StencilRenderState* p_pStencilRenderState) 
{ 
	m_pRenderContext->SetRenderState(p_pStencilRenderState);
}
//----------------------------------------------------------------------------------------------
void RenderDeviceAdapter::SetTargetRenderState(TargetRenderState* p_pTargetRenderState)
{
	m_pRenderContext->SetRenderState(p_pTargetRenderState);
}
//----------------------------------------------------------------------------------------------
AlphaRenderState* RenderDeviceAdapter::GetAlphaRenderState(void) 
{ 
	return (AlphaRenderState*)(IRenderState*)m_pRenderContext->GetRenderState(IRenderState::RST_ALPHA);
}
//----------------------------------------------------------------------------------------------
BiasRenderState* RenderDeviceAdapter::GetBiasRenderState(void)
{
	return (BiasRenderState*)(IRenderState*)m_pRenderContext->GetRenderState(IRenderState::RST_BIAS);
}
//----------------------------------------------------------------------------------------------
CullRenderState* RenderDeviceAdapter::GetCullRenderState(void) 
{ 
	return (CullRenderState*)(IRenderState*)m_pRenderContext->GetRenderState(IRenderState::RST_CULL);
}
//----------------------------------------------------------------------------------------------
DepthRenderState* RenderDeviceAdapter::GetDepthRenderState(void) 
{ 
	return (DepthRenderState*)(IRenderState*)m_pRenderContext->GetRenderState(IRenderState::RST_DEPTH);
}
//----------------------------------------------------------------------------------------------
LightRenderState* RenderDeviceAdapter::GetLightRenderState(void) 
{ 
	return (LightRenderState*)(IRenderState*)m_pRenderContext->GetRenderState(IRenderState::RST_LIGHT);
}
//----------------------------------------------------------------------------------------------
MaterialRenderState* RenderDeviceAdapter::GetMaterialRenderState(void)
{ 
	return (MaterialRenderState*)(IRenderState*)m_pRenderContext->GetRenderState(IRenderState::RST_MATERIAL);
}
//----------------------------------------------------------------------------------------------
SamplerRenderState* RenderDeviceAdapter::GetSamplerRenderState(void) 
{ 
	return (SamplerRenderState*)(IRenderState*)m_pRenderContext->GetRenderState(IRenderState::RST_SAMPLER);
}
//----------------------------------------------------------------------------------------------
StencilRenderState* RenderDeviceAdapter::GetStencilRenderState(void)
{ 
	return (StencilRenderState*)(IRenderState*)m_pRenderContext->GetRenderState(IRenderState::RST_STENCIL);
}
//----------------------------------------------------------------------------------------------
TargetRenderState* RenderDeviceAdapter::GetTargetRenderState(void)
{
	return (TargetRenderState*)(IRenderState*)m_pRenderContext->GetRenderState(IRenderState::RST_TARGET);
}
//----------------------------------------------------------------------------------------------
uint RenderDeviceAdapter::GetDeviceCapabilities(IRenderDevice::DeviceCapabilitiesI p_eDeviceCapabilities)
{
	MESON_ASSERT(p_eDeviceCapabilities != IRenderDevice::DCI_COUNT, 
		"DCI_COUNT cannot be used to query device capabilities");
	
	return m_dwDeviceCapabilities[p_eDeviceCapabilities];
}
//----------------------------------------------------------------------------------------------
float RenderDeviceAdapter::GetDeviceCapabilities(IRenderDevice::DeviceCapabilitiesF p_eDeviceCapabilities)
{
	MESON_ASSERT(p_eDeviceCapabilities != IRenderDevice::DCF_COUNT, 
		"DCF_COUNT cannot be used to query device capabilities");
	
	return m_fDeviceCapabilities[p_eDeviceCapabilities];
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
