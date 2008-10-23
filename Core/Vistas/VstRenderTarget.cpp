//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Render::RenderTarget
//	Interface class for render targets.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstRenderTarget.h"
#include "VstViewport.h"
#include "VstCamera.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
RenderTarget::~RenderTarget(void)
{
	DestroyAllViewports();
}
//----------------------------------------------------------------------------------------------
const String& RenderTarget::GetName(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
void RenderTarget::GetDimensions(uint& p_uiWidth, uint& p_uiHeight, uint& p_uiColourDepth) const
{
	p_uiWidth = m_uiWidth;
	p_uiHeight = m_uiHeight;
	p_uiColourDepth = m_uiColourBufferSize;
}
//----------------------------------------------------------------------------------------------
uint RenderTarget::GetWidth(void) const 
{
	return m_uiWidth;
}
//----------------------------------------------------------------------------------------------
uint RenderTarget::GetHeight(void) const
{
	return m_uiHeight;
}
//----------------------------------------------------------------------------------------------
uint RenderTarget::GetColourDepth(void) const
{
	return m_uiColourBufferSize;
}
//----------------------------------------------------------------------------------------------
int RenderTarget::GetPriority(void) const
{
	return m_nPriority;
}
//----------------------------------------------------------------------------------------------
void RenderTarget::SetPriority(int p_nPriority)
{
	m_nPriority = p_nPriority;
}
//----------------------------------------------------------------------------------------------
bool RenderTarget::IsActive(void) const
{
	return m_bActive;
}
//----------------------------------------------------------------------------------------------
void RenderTarget::SetActive(bool p_bActive) 
{
	m_bActive = p_bActive;
}
//----------------------------------------------------------------------------------------------
void RenderTarget::CreateViewport(Camera* p_pCamera, uint p_uiPriorityIndex, float p_fLeft, float p_fTop, float p_fWidth, float p_fHeight, float p_fNear, float p_fFar)
{
	if (m_viewportMap.ContainsKey(p_uiPriorityIndex))
		throw new MesonException("Unable to create viewport : viewport priority is not unique.");

	Viewport *pViewport = new Viewport(this, p_pCamera, p_uiPriorityIndex, p_fLeft, p_fTop, p_fWidth, p_fHeight, p_fNear, p_fFar);
	m_viewportMap.Insert(p_uiPriorityIndex, pViewport);
	
	OnCreateViewport(pViewport);
}
//----------------------------------------------------------------------------------------------
Viewport* RenderTarget::GetViewport(uint p_uiPriorityIndex)
{
	if (m_viewportMap.ContainsKey(p_uiPriorityIndex))
		return m_viewportMap[p_uiPriorityIndex];

	return NULL;
}
//----------------------------------------------------------------------------------------------
uint RenderTarget::GetViewportCount(void) const
{
	return (uint)m_viewportMap.Size();
}
//----------------------------------------------------------------------------------------------
TEnumerator<Viewport*>& RenderTarget::GetAllViewports(void)
{
	return m_viewportMap.GetValueEnumerator();
}
//----------------------------------------------------------------------------------------------
void RenderTarget::DestroyViewport(uint p_uiPriorityIndex)
{
	if (m_viewportMap.ContainsKey(p_uiPriorityIndex))
	{
		Viewport* pViewport = m_viewportMap[p_uiPriorityIndex];
		OnDestroyViewport(pViewport);
		
		m_viewportMap.RemoveKey(p_uiPriorityIndex);
		delete pViewport;
	}
}
//----------------------------------------------------------------------------------------------
void RenderTarget::DestroyAllViewports(void)
{
	TEnumerator<Viewport*>& viewportEnumerator = m_viewportMap.GetValueEnumerator();
	Viewport* pViewport = NULL;

	while( viewportEnumerator.HasMoreElements() )
	{
		pViewport = viewportEnumerator.NextElement();

		OnDestroyViewport(pViewport);
		delete pViewport;
	}

	m_viewportMap.Clear();
}
//----------------------------------------------------------------------------------------------
void RenderTarget::OnCreateViewport( Viewport* p_pViewport ) { }
//----------------------------------------------------------------------------------------------
void RenderTarget::OnUpdateViewport( Viewport* p_pViewport ) { }
//----------------------------------------------------------------------------------------------
void RenderTarget::OnDestroyViewport( Viewport* p_pViewport ) { }
//----------------------------------------------------------------------------------------------
Meson_Vistas_END
