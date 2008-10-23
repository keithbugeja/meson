//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Viewport
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstVistasEngine.h"
#include "VstViewport.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
Viewport::Viewport(RenderTarget* p_pRenderTarget, Camera* p_pCamera, uint p_uiPriorityIndex, float p_fLeft, float p_fTop, float p_fWidth, float p_fHeight, float p_fNear, float p_fFar) 
		: m_pRenderTarget(p_pRenderTarget), m_pCamera(p_pCamera), m_uiPriorityIndex(p_uiPriorityIndex), 
		  m_fLeft(p_fLeft), m_fTop(p_fTop), m_fWidth(p_fWidth), m_fHeight(p_fHeight), 
		  m_fNear(p_fNear), m_fFar(p_fFar)
{
}
//----------------------------------------------------------------------------------------------
void Viewport::Update(bool p_bForce)
{
	IRenderDevice* pRenderDevice = VistasEngine::GetInstance()->GetRenderDeviceManager()->GetActive();

	if (p_bForce) 
	{
		m_pCamera->InvalidateView();
		m_pCamera->InvalidateFrustum();
	}

	m_pCamera->UpdateView(pRenderDevice);
	m_pCamera->UpdateProjection(pRenderDevice);
}
//----------------------------------------------------------------------------------------------
void Viewport::SetFrame(float p_fLeft, float p_fTop, float p_fWidth, float p_fHeight)
{
	m_fLeft = p_fLeft;
	m_fTop = p_fTop;
	m_fWidth = p_fWidth;
	m_fHeight = p_fHeight;
}
//----------------------------------------------------------------------------------------------
void Viewport::GetFrame(float& p_fLeft, float& p_fTop, float& p_fWidth, float& p_fHeight)
{
	p_fLeft = m_fLeft;
	p_fTop = m_fTop;
	p_fWidth = m_fWidth;
	p_fHeight = m_fHeight;
}
//----------------------------------------------------------------------------------------------
void Viewport::SetDepthRange(float p_fNear, float p_fFar)
{
	m_fNear = p_fNear;
	m_fFar = p_fFar;
}
//----------------------------------------------------------------------------------------------
void Viewport::GetDepthRange(float& p_fNear, float& p_fFar)
{
	p_fNear = m_fNear;
	p_fFar = m_fFar;
}
//----------------------------------------------------------------------------------------------
float Viewport::GetFrameLeft(void)
{
	return m_fLeft;
}
//----------------------------------------------------------------------------------------------
float Viewport::GetFrameTop(void)
{
	return m_fTop;
}
//----------------------------------------------------------------------------------------------
float Viewport::GetFrameWidth(void)
{
	return m_fWidth;
}
//----------------------------------------------------------------------------------------------
float Viewport::GetFrameHeight(void)
{
	return m_fHeight;
}
//----------------------------------------------------------------------------------------------
float Viewport::GetNearDepth(void)
{
	return m_fNear;
}
//----------------------------------------------------------------------------------------------
float Viewport::GetFarDepth(void)
{
	return m_fFar;
}
//----------------------------------------------------------------------------------------------
float Viewport::GetMappedFrameLeft(void)
{
	return m_fLeft * m_pRenderTarget->GetWidth();
}
//----------------------------------------------------------------------------------------------
float Viewport::GetMappedFrameTop(void)
{
	return m_fTop * m_pRenderTarget->GetHeight();
}
//----------------------------------------------------------------------------------------------
float Viewport::GetMappedFrameWidth(void)
{
	return m_fWidth * m_pRenderTarget->GetWidth();
}
//----------------------------------------------------------------------------------------------
float Viewport::GetMappedFrameHeight(void)
{
	return m_fHeight * m_pRenderTarget->GetHeight();
}
//----------------------------------------------------------------------------------------------
uint Viewport::GetDeviceWidth(void)
{
	return m_pRenderTarget->GetWidth();
}
//----------------------------------------------------------------------------------------------
uint Viewport::GetDeviceHeight(void)
{
	return m_pRenderTarget->GetHeight();
}
//----------------------------------------------------------------------------------------------
RenderTarget* Viewport::GetRenderTarget(void)
{
	return m_pRenderTarget;
}
//----------------------------------------------------------------------------------------------
uint Viewport::GetPriorityIndex(void)
{
	return m_uiPriorityIndex;
}
//----------------------------------------------------------------------------------------------
void Viewport::SetCamera(Camera* p_pCamera)
{
	MESON_ASSERT(p_pCamera != NULL, "Invalid camera object.");
	m_pCamera = p_pCamera;
}
//----------------------------------------------------------------------------------------------
Camera* Viewport::GetCamera(void)
{
	return m_pCamera;
}

Meson_Vistas_END