#pragma once

#include "VstTypes.h"
#include "VstVistasEngine.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class VISTAS_API WindowMessageHandler : public IWindowMessageHandler
	{
	public:
		//----------------------------------------------------------------------------------------------
		WindowMessageHandler(void);
		WindowMessageHandler(WindowRenderTarget* p_pWindow);

		virtual void OnMouseMove(float p_fMouseX, float p_fMouseY);
		virtual void OnMouseDown(MouseKey p_eKey);
		virtual void OnMouseUp(MouseKey p_eKey);

		virtual void OnKeyDown(KeyCode p_eKey);
		virtual void OnKeyUp(KeyCode p_eKey);

		virtual void OnClose(void);
		virtual void OnResize(int p_nHorizontal, int p_nVertical, WindowState p_eWindowState);
		virtual void OnMove(int p_nHorizontal, int p_nVertical);

		virtual void OnIdle(void) = 0;

		virtual void Dispatch(bool p_bActiveWindow = true);
		virtual bool DispatchSingle(bool p_bActiveWindow = true);

		virtual void Hook(WindowRenderTarget* p_pWindow);
		virtual void Terminate(void);

		//----------------------------------------------------------------------------------------------
	protected:
		bool IsRunning(void) { return m_bRunning; }

		WindowRenderTarget* GetHookedWindow(void) { return m_pWindow; }

		float GetMouseX(void) { return m_fLastMouseX; }
		float GetMouseY(void) { return m_fLastMouseY; }

		bool IsMouseDown(MouseKey p_eKey) { return m_bMouseKey[p_eKey]; }
		bool IsKeyDown(KeyCode p_eKey) { return m_bKey[p_eKey]; }

		//----------------------------------------------------------------------------------------------
	protected:
		WindowRenderTarget* m_pWindow;

		bool m_bRunning;

		float m_fLastMouseX, 
			  m_fLastMouseY;

		bool m_bMouseKey[Meson::Vistas::WindowMessageHandler::MK_COUNT],
			 m_bKey[Meson::Vistas::WindowMessageHandler::KC_COUNT];
	};
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END
