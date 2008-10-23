#include "VstWindowMessageHandler.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
WindowMessageHandler::WindowMessageHandler(void)
	: m_pWindow(NULL)
	, m_fLastMouseX(0)
	, m_fLastMouseY(0)
{
	for (uint uiIndex = 0; uiIndex < 2; uiIndex++)
		m_bMouseKey[uiIndex] = false;

	for (uint uiIndex = 0; uiIndex < 256; uiIndex++)
		m_bKey[uiIndex] = false;
}
//----------------------------------------------------------------------------------------------
WindowMessageHandler::WindowMessageHandler(WindowRenderTarget* p_pWindow) 
	: m_pWindow(p_pWindow)
	, m_fLastMouseX(0)
	, m_fLastMouseY(0)
{
	VistasEngine::GetInstance()->GetWindowMessageManager()->AttachMessageHandler(m_pWindow, this);

	for (uint uiIndex = 0; uiIndex < 2; uiIndex++)
		m_bMouseKey[uiIndex] = false;

	for (uint uiIndex = 0; uiIndex < 256; uiIndex++)
		m_bKey[uiIndex] = false;
}
//----------------------------------------------------------------------------------------------
void WindowMessageHandler::Terminate(void)
{
	VistasEngine::GetInstance()->GetWindowMessageManager()->DetachMessageHandler(m_pWindow);
}
//----------------------------------------------------------------------------------------------
void WindowMessageHandler::Hook(WindowRenderTarget* p_pWindow)
{
	m_pWindow = p_pWindow;

	VistasEngine::GetInstance()->GetWindowMessageManager()->AttachMessageHandler(m_pWindow, this);
}
//----------------------------------------------------------------------------------------------
void WindowMessageHandler::OnMouseMove(float p_fMouseX, float p_fMouseY)
{
	m_fLastMouseX = p_fMouseX;
	m_fLastMouseY = p_fMouseY;
}
//----------------------------------------------------------------------------------------------
void WindowMessageHandler::OnMouseDown(MouseKey p_eKey)
{
	m_bMouseKey[p_eKey] = true;
}
//----------------------------------------------------------------------------------------------
void WindowMessageHandler::OnMouseUp(MouseKey p_eKey)
{
	m_bMouseKey[p_eKey] = false;
}
//----------------------------------------------------------------------------------------------
void WindowMessageHandler::OnKeyDown(KeyCode p_eKey)
{
	m_bKey[p_eKey] = true;
}
//----------------------------------------------------------------------------------------------
void WindowMessageHandler::OnKeyUp(KeyCode p_eKey)
{
	m_bKey[p_eKey] = false;
}
//----------------------------------------------------------------------------------------------
void WindowMessageHandler::OnClose(void)
{
	m_bRunning = false;
}
//----------------------------------------------------------------------------------------------
void WindowMessageHandler::OnResize(int p_nHorizontal, int p_nVertical, WindowState p_eWindowState)
{
}
//----------------------------------------------------------------------------------------------
void WindowMessageHandler::OnMove(int p_nHorizontal, int p_nVertical)
{
}
//----------------------------------------------------------------------------------------------
void WindowMessageHandler::Dispatch(bool p_bActiveWindow)
{
	m_bRunning = true;

	while( m_bRunning )
	{
		VistasEngine::GetInstance()->GetWindowMessageManager()->Dispatch();
		
		if (p_bActiveWindow && !WindowMessageHandler::m_pWindow->IsActive())
			continue;
		
		OnIdle();
	}
}
//----------------------------------------------------------------------------------------------
bool WindowMessageHandler::DispatchSingle(bool p_bActiveWindow)
{
	VistasEngine::GetInstance()->GetWindowMessageManager()->Dispatch();
	
	if (p_bActiveWindow && !WindowMessageHandler::m_pWindow->IsActive())
		return m_bRunning;
	
	OnIdle();
	
	return m_bRunning;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
