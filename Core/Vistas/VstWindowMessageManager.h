//----------------------------------------------------------------------------------------------
//	Meson::Vistas::WindowMessageManager
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Platform.h"
#include "Singleton.h"
#include "EventDispatcher.h"

#include "VstNamespaces.h"
#include "VstDefinitions.h"
#include "VstTypes.h"

#ifdef PLATFORM_WIN32
	#include <windows.h>
#endif

Meson_Vistas_BEGIN

	/** Interface class for window message handling.
	 */
	class VISTAS_API IWindowMessageHandler
	{
	public:
		enum WindowState
		{
			WS_RESIZED,
			WS_MINIMISED,
			WS_MAXIMISED
		};

		/** Fired when a window close message is posted, prior to the window being closed.
		 */
		virtual void OnClose(void) { }

		/** Fired when when a mouse button is pushed or released.
			These two functions should be removed and encapsulated into an Input device object.
		 */
		enum MouseKey
		{
			MK_LEFT,
			MK_RIGHT,
			MK_COUNT
		};

		virtual void OnMouseDown(MouseKey p_eKey) { }
		virtual void OnMouseUp(MouseKey p_eKey) { }
		virtual void OnMouseMove(float p_fMouseX, float p_fMouseY) { }

		/** Fire when a key is pressed or unpressed.
			These two functions should be removed and encapsulated into an Input device object.
		 */
		enum KeyCode
		{
			KC_NONE,
			KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,
			KC_A, KC_B, KC_C, KC_D, KC_E, KC_F,
			KC_G, KC_H, KC_I, KC_J, KC_K, KC_L,
			KC_M, KC_N, KC_O, KC_P, KC_Q, KC_R,
			KC_S, KC_T, KC_U, KC_V, KC_W, KC_X,
			KC_Y, KC_Z,
			KC_NUMPAD0, KC_NUMPAD1, KC_NUMPAD2, KC_NUMPAD3, KC_NUMPAD4, 
			KC_NUMPAD5, KC_NUMPAD6, KC_NUMPAD7, KC_NUMPAD8, KC_NUMPAD9,
			KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9,
			KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
			KC_LSHIFT, KC_RSHIFT, KC_LCONTROL, KC_RCONTROL,
			KC_ESCAPE, KC_RETURN, KC_SPACE,
			KC_COUNT
		};

		virtual void OnKeyDown(KeyCode p_eKey) { }
		virtual void OnKeyUp(KeyCode p_eKey) { }

		/** Fired when the window is resized, minimised or maximised.
		 */
		virtual void OnResize(int p_nHorizontal, int p_nVertical, WindowState p_eWindowState) { }
		
		/** Fired when the window frame is repositioned.
		 */
		virtual void OnMove(int p_nHorizontal, int p_nVertical) { }
	};

	/** The Window Message manager dispatches window events to registered handlers. 
	 */
	class VISTAS_API WindowMessageManager : public Meson::Common::TSingleton<WindowMessageManager>
	{
	public:
		void Dispatch(void);

		#ifdef PLATFORM_WIN32
			// Seems like WIN32 callbacks cannot be dynamic 
			static LRESULT CALLBACK StaticWndProc(HWND p_hWnd, UINT p_uiMessage, WPARAM p_wParam, LPARAM p_lParam);
			LRESULT CALLBACK WndProc(HWND p_hWnd, UINT p_uiMessage, WPARAM p_wParam, LPARAM p_lParam);
		#endif

	private:
		Meson::Common::Collections::TArrayList<WindowRenderTarget*> m_windowRenderTargets;
		Meson::Common::Collections::THashMap<WindowRenderTarget*, IWindowMessageHandler*> m_windowMessageHandlerMap;

	public:
		void AttachWindow(WindowRenderTarget* p_window);
		void DetachWindow(WindowRenderTarget* p_window);
			
		void AttachMessageHandler(WindowRenderTarget* p_window, IWindowMessageHandler *p_messageHandler);
		void DetachMessageHandler(WindowRenderTarget* p_window);
	};

Meson_Vistas_END
