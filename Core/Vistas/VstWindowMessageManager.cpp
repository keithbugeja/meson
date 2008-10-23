#include "VstWindowMessageManager.h"

#include "VstVistasEngine.h"
#include "VstWindowRenderTarget.h"

Meson_Vistas_BEGIN

#ifdef PLATFORM_WIN32

//----------------------------------------------------------------------------------------------
/// Event dispatcher for Win32 environment
//----------------------------------------------------------------------------------------------
void WindowMessageManager::Dispatch()
{
	MSG  message;
	
	while( PeekMessage( &message, NULL, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &message );
		DispatchMessage( &message );
	}
}

//----------------------------------------------------------------------------------------------
///	This static callback wrapper is used to overcome the limitation that WNDPROC functions
///	be defined statically. The actual instance of the message manager is acquired from the
///	VistasEngine singleton.
//----------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowMessageManager::StaticWndProc(HWND p_hWnd, UINT p_uiMessage, WPARAM p_wParam, LPARAM p_lParam)
{
	return VistasEngine::GetInstance()->GetWindowMessageManager()->WndProc(p_hWnd,p_uiMessage, p_wParam, p_lParam);
}

//----------------------------------------------------------------------------------------------
///	Actual non-static WndProc function.
//----------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowMessageManager::WndProc(HWND p_hWnd, UINT p_uiMessage, WPARAM p_wParam, LPARAM p_lParam)
{
	// Win32 and Win64 pointer sizes vary; use LongToPtr() on Win32 to avoid compiler warning.
	#ifdef _WIN64
		WindowRenderTarget* pWindow = (WindowRenderTarget*)GetWindowLongPtr(p_hWnd, GWLP_USERDATA);
	#else
		WindowRenderTarget* pWindow = (WindowRenderTarget*)LongToPtr(GetWindowLongPtr(p_hWnd, GWLP_USERDATA));
	#endif

	// Check whether window is both valid and registered
	if (pWindow == NULL || m_windowRenderTargets.IndexOf(pWindow) == -1)
		return DefWindowProc( p_hWnd, p_uiMessage, p_wParam, p_lParam );

	// Check whether a handler is registered for the specified window
	if (m_windowMessageHandlerMap.ContainsKey(pWindow))
	{
		IWindowMessageHandler* pMessageHandler = m_windowMessageHandlerMap[pWindow];

		switch( p_uiMessage )
		{
			//----------------------------------------------------------------------------------------------
			case WM_ACTIVATEAPP:
				pWindow->SetActive( LOWORD(p_wParam) == WA_ACTIVE );
				break;

			//----------------------------------------------------------------------------------------------
			case WM_MOVE:
				pWindow->OnWindowMoved();

				pMessageHandler->OnMove((int)(short)LOWORD(p_lParam),(int)(short)HIWORD(p_lParam));
				break;
		
			//----------------------------------------------------------------------------------------------
			case WM_SIZE:
			{
				pWindow->OnWindowResized();

				IWindowMessageHandler::WindowState windowState;

				switch (p_wParam) 
				{
					case SIZE_MAXIMIZED:
						windowState = IWindowMessageHandler::WS_MAXIMISED;
						break;

					case SIZE_MINIMIZED:
						windowState = IWindowMessageHandler::WS_MINIMISED;
						break;

					default:
						windowState = IWindowMessageHandler::WS_RESIZED;
				}

				pMessageHandler->OnResize((int)(short)LOWORD(p_lParam),(int)(short)HIWORD(p_lParam), windowState);
				break;
			}
		
			//----------------------------------------------------------------------------------------------
			case WM_CLOSE:
				pMessageHandler->OnClose();

				pWindow->Destroy();
				break;

			//----------------------------------------------------------------------------------------------
			case WM_KEYDOWN:
			{
				// Lame, I know, but this is temporary. Can't hardcode the values in the KeyCode enum anyway 
				// since this is Windows specific.
				IWindowMessageHandler::KeyCode eKeyCode = IWindowMessageHandler::KC_NONE;
				
				switch (p_wParam)
				{
					case VK_ESCAPE:
						eKeyCode = IWindowMessageHandler::KC_ESCAPE;
						break;
					case VK_RETURN:
						eKeyCode = IWindowMessageHandler::KC_RETURN;
						break;
					case VK_SPACE:
						eKeyCode = IWindowMessageHandler::KC_SPACE;
						break;

					case VK_LSHIFT:
						eKeyCode = IWindowMessageHandler::KC_LSHIFT;
						break;
					case VK_RSHIFT:
						eKeyCode = IWindowMessageHandler::KC_RSHIFT;
						break;
					case VK_LCONTROL:
						eKeyCode = IWindowMessageHandler::KC_LCONTROL;
						break;
					case VK_RCONTROL:
						eKeyCode = IWindowMessageHandler::KC_RCONTROL;
						break;

					case VK_F1:
						eKeyCode = IWindowMessageHandler::KC_F1;
						break;
					case VK_F2:
						eKeyCode = IWindowMessageHandler::KC_F2;
						break;
					case VK_F3:
						eKeyCode = IWindowMessageHandler::KC_F3;
						break;
					case VK_F4:
						eKeyCode = IWindowMessageHandler::KC_F4;
						break;
					case VK_F5:
						eKeyCode = IWindowMessageHandler::KC_F5;
						break;
					case VK_F6:
						eKeyCode = IWindowMessageHandler::KC_F6;
						break;
					case VK_F7:
						eKeyCode = IWindowMessageHandler::KC_F7;
						break;
					case VK_F8:
						eKeyCode = IWindowMessageHandler::KC_F8;
						break;
					case VK_F9:
						eKeyCode = IWindowMessageHandler::KC_F9;
						break;
					case VK_F10:
						eKeyCode = IWindowMessageHandler::KC_F10;
						break;
					case VK_F11:
						eKeyCode = IWindowMessageHandler::KC_F11;
						break;
					case VK_F12:
						eKeyCode = IWindowMessageHandler::KC_F12;
						break;

					case VK_NUMPAD0:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD0;
						break;
					case VK_NUMPAD1:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD1;
						break;
					case VK_NUMPAD2:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD2;
						break;
					case VK_NUMPAD3:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD3;
						break;
					case VK_NUMPAD4:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD4;
						break;
					case VK_NUMPAD5:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD5;
						break;
					case VK_NUMPAD6:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD6;
						break;
					case VK_NUMPAD7:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD7;
						break;
					case VK_NUMPAD8:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD8;
						break;
					case VK_NUMPAD9:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD9;
						break;

					case VK_LEFT:
						eKeyCode = IWindowMessageHandler::KC_LEFT;
						break;
					case VK_RIGHT:
						eKeyCode = IWindowMessageHandler::KC_RIGHT;
						break;
					case VK_UP:
						eKeyCode = IWindowMessageHandler::KC_UP;
						break;
					case VK_DOWN:
						eKeyCode = IWindowMessageHandler::KC_DOWN;
						break;

					case 0x30:
						eKeyCode = IWindowMessageHandler::KC_0;
						break;
					case 0x31:
						eKeyCode = IWindowMessageHandler::KC_1;
						break;
					case 0x32:
						eKeyCode = IWindowMessageHandler::KC_2;
						break;
					case 0x33:
						eKeyCode = IWindowMessageHandler::KC_3;
						break;
					case 0x34:
						eKeyCode = IWindowMessageHandler::KC_4;
						break;
					case 0x35:
						eKeyCode = IWindowMessageHandler::KC_5;
						break;
					case 0x36:
						eKeyCode = IWindowMessageHandler::KC_6;
						break;
					case 0x37:
						eKeyCode = IWindowMessageHandler::KC_7;
						break;
					case 0x38:
						eKeyCode = IWindowMessageHandler::KC_8;
						break;
					case 0x39:
						eKeyCode = IWindowMessageHandler::KC_9;
						break;
		
					case 0x41:
						eKeyCode = IWindowMessageHandler::KC_A;
						break;
					case 0x42:
						eKeyCode = IWindowMessageHandler::KC_B;
						break;
					case 0x43:
						eKeyCode = IWindowMessageHandler::KC_C;
						break;
					case 0x44:
						eKeyCode = IWindowMessageHandler::KC_D;
						break;
					case 0x45:
						eKeyCode = IWindowMessageHandler::KC_E;
						break;
					case 0x46:
						eKeyCode = IWindowMessageHandler::KC_F;
						break;
					case 0x47:
						eKeyCode = IWindowMessageHandler::KC_G;
						break;
					case 0x48:
						eKeyCode = IWindowMessageHandler::KC_H;
						break;
					case 0x49:
						eKeyCode = IWindowMessageHandler::KC_I;
						break;
					case 0x4a:
						eKeyCode = IWindowMessageHandler::KC_J;
						break;
					case 0x4b:
						eKeyCode = IWindowMessageHandler::KC_K;
						break;
					case 0x4c:
						eKeyCode = IWindowMessageHandler::KC_L;
						break;
					case 0x4d:
						eKeyCode = IWindowMessageHandler::KC_M;
						break;
					case 0x4e:
						eKeyCode = IWindowMessageHandler::KC_N;
						break;
					case 0x4f:
						eKeyCode = IWindowMessageHandler::KC_O;
						break;
					case 0x50:
						eKeyCode = IWindowMessageHandler::KC_P;
						break;
					case 0x51:
						eKeyCode = IWindowMessageHandler::KC_Q;
						break;
					case 0x52:
						eKeyCode = IWindowMessageHandler::KC_R;
						break;
					case 0x53:
						eKeyCode = IWindowMessageHandler::KC_S;
						break;
					case 0x54:
						eKeyCode = IWindowMessageHandler::KC_T;
						break;
					case 0x55:
						eKeyCode = IWindowMessageHandler::KC_U;
						break;
					case 0x56:
						eKeyCode = IWindowMessageHandler::KC_V;
						break;
					case 0x57:
						eKeyCode = IWindowMessageHandler::KC_W;
						break;
					case 0x58:
						eKeyCode = IWindowMessageHandler::KC_X;
						break;
					case 0x59:
						eKeyCode = IWindowMessageHandler::KC_Y;
						break;
					case 0x5A:
						eKeyCode = IWindowMessageHandler::KC_Z;
						break;
				}

				pMessageHandler->OnKeyDown(eKeyCode);
				break;
			}

			case WM_KEYUP:
			{
				// Lame, I know, but this is temporary. Can't hardcode the values in the KeyCode enum anyway 
				// since this is Windows specific.
				IWindowMessageHandler::KeyCode eKeyCode = IWindowMessageHandler::KC_NONE;
				
				switch (p_wParam)
				{
					case VK_ESCAPE:
						eKeyCode = IWindowMessageHandler::KC_ESCAPE;
						break;
					case VK_RETURN:
						eKeyCode = IWindowMessageHandler::KC_RETURN;
						break;
					case VK_SPACE:
						eKeyCode = IWindowMessageHandler::KC_SPACE;
						break;

					case VK_LSHIFT:
						eKeyCode = IWindowMessageHandler::KC_LSHIFT;
						break;
					case VK_RSHIFT:
						eKeyCode = IWindowMessageHandler::KC_RSHIFT;
						break;
					case VK_LCONTROL:
						eKeyCode = IWindowMessageHandler::KC_LCONTROL;
						break;
					case VK_RCONTROL:
						eKeyCode = IWindowMessageHandler::KC_RCONTROL;
						break;

					case VK_F1:
						eKeyCode = IWindowMessageHandler::KC_F1;
						break;
					case VK_F2:
						eKeyCode = IWindowMessageHandler::KC_F2;
						break;
					case VK_F3:
						eKeyCode = IWindowMessageHandler::KC_F3;
						break;
					case VK_F4:
						eKeyCode = IWindowMessageHandler::KC_F4;
						break;
					case VK_F5:
						eKeyCode = IWindowMessageHandler::KC_F5;
						break;
					case VK_F6:
						eKeyCode = IWindowMessageHandler::KC_F6;
						break;
					case VK_F7:
						eKeyCode = IWindowMessageHandler::KC_F7;
						break;
					case VK_F8:
						eKeyCode = IWindowMessageHandler::KC_F8;
						break;
					case VK_F9:
						eKeyCode = IWindowMessageHandler::KC_F9;
						break;
					case VK_F10:
						eKeyCode = IWindowMessageHandler::KC_F10;
						break;
					case VK_F11:
						eKeyCode = IWindowMessageHandler::KC_F11;
						break;
					case VK_F12:
						eKeyCode = IWindowMessageHandler::KC_F12;
						break;

					case VK_NUMPAD0:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD0;
						break;
					case VK_NUMPAD1:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD1;
						break;
					case VK_NUMPAD2:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD2;
						break;
					case VK_NUMPAD3:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD3;
						break;
					case VK_NUMPAD4:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD4;
						break;
					case VK_NUMPAD5:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD5;
						break;
					case VK_NUMPAD6:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD6;
						break;
					case VK_NUMPAD7:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD7;
						break;
					case VK_NUMPAD8:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD8;
						break;
					case VK_NUMPAD9:
						eKeyCode = IWindowMessageHandler::KC_NUMPAD9;
						break;

					case VK_LEFT:
						eKeyCode = IWindowMessageHandler::KC_LEFT;
						break;
					case VK_RIGHT:
						eKeyCode = IWindowMessageHandler::KC_RIGHT;
						break;
					case VK_UP:
						eKeyCode = IWindowMessageHandler::KC_UP;
						break;
					case VK_DOWN:
						eKeyCode = IWindowMessageHandler::KC_DOWN;
						break;

					case 0x30:
						eKeyCode = IWindowMessageHandler::KC_0;
						break;
					case 0x31:
						eKeyCode = IWindowMessageHandler::KC_1;
						break;
					case 0x32:
						eKeyCode = IWindowMessageHandler::KC_2;
						break;
					case 0x33:
						eKeyCode = IWindowMessageHandler::KC_3;
						break;
					case 0x34:
						eKeyCode = IWindowMessageHandler::KC_4;
						break;
					case 0x35:
						eKeyCode = IWindowMessageHandler::KC_5;
						break;
					case 0x36:
						eKeyCode = IWindowMessageHandler::KC_6;
						break;
					case 0x37:
						eKeyCode = IWindowMessageHandler::KC_7;
						break;
					case 0x38:
						eKeyCode = IWindowMessageHandler::KC_8;
						break;
					case 0x39:
						eKeyCode = IWindowMessageHandler::KC_9;
						break;
		
					case 0x41:
						eKeyCode = IWindowMessageHandler::KC_A;
						break;
					case 0x42:
						eKeyCode = IWindowMessageHandler::KC_B;
						break;
					case 0x43:
						eKeyCode = IWindowMessageHandler::KC_C;
						break;
					case 0x44:
						eKeyCode = IWindowMessageHandler::KC_D;
						break;
					case 0x45:
						eKeyCode = IWindowMessageHandler::KC_E;
						break;
					case 0x46:
						eKeyCode = IWindowMessageHandler::KC_F;
						break;
					case 0x47:
						eKeyCode = IWindowMessageHandler::KC_G;
						break;
					case 0x48:
						eKeyCode = IWindowMessageHandler::KC_H;
						break;
					case 0x49:
						eKeyCode = IWindowMessageHandler::KC_I;
						break;
					case 0x4a:
						eKeyCode = IWindowMessageHandler::KC_J;
						break;
					case 0x4b:
						eKeyCode = IWindowMessageHandler::KC_K;
						break;
					case 0x4c:
						eKeyCode = IWindowMessageHandler::KC_L;
						break;
					case 0x4d:
						eKeyCode = IWindowMessageHandler::KC_M;
						break;
					case 0x4e:
						eKeyCode = IWindowMessageHandler::KC_N;
						break;
					case 0x4f:
						eKeyCode = IWindowMessageHandler::KC_O;
						break;
					case 0x50:
						eKeyCode = IWindowMessageHandler::KC_P;
						break;
					case 0x51:
						eKeyCode = IWindowMessageHandler::KC_Q;
						break;
					case 0x52:
						eKeyCode = IWindowMessageHandler::KC_R;
						break;
					case 0x53:
						eKeyCode = IWindowMessageHandler::KC_S;
						break;
					case 0x54:
						eKeyCode = IWindowMessageHandler::KC_T;
						break;
					case 0x55:
						eKeyCode = IWindowMessageHandler::KC_U;
						break;
					case 0x56:
						eKeyCode = IWindowMessageHandler::KC_V;
						break;
					case 0x57:
						eKeyCode = IWindowMessageHandler::KC_W;
						break;
					case 0x58:
						eKeyCode = IWindowMessageHandler::KC_X;
						break;
					case 0x59:
						eKeyCode = IWindowMessageHandler::KC_Y;
						break;
					case 0x5A:
						eKeyCode = IWindowMessageHandler::KC_Z;
						break;
				}

				pMessageHandler->OnKeyUp(eKeyCode);
				break;
			}

			//----------------------------------------------------------------------------------------------
			case WM_LBUTTONDOWN:
				pMessageHandler->OnMouseDown(IWindowMessageHandler::MK_LEFT);
				break;

			case WM_LBUTTONUP:
				pMessageHandler->OnMouseUp(IWindowMessageHandler::MK_LEFT);
				break;

			case WM_RBUTTONDOWN:
				pMessageHandler->OnMouseDown(IWindowMessageHandler::MK_RIGHT);
				break;

			case WM_RBUTTONUP:
				pMessageHandler->OnMouseUp(IWindowMessageHandler::MK_RIGHT);
				break;

			case WM_MOUSEMOVE:
			{
				RECT clientRect;
				GetClientRect(p_hWnd, &clientRect);
				float fMouseX = ((float)LOWORD(p_lParam) / (float)(clientRect.right - clientRect.left)) * 2 - 1;
				float fMouseY = ((float)HIWORD(p_lParam) / (float)(clientRect.bottom - clientRect.top)) * 2 - 1;
				
				pMessageHandler->OnMouseMove(fMouseX, fMouseY);
				break;
			}

			default:
				break;
		}
	}

	// Execute default window proc
	return DefWindowProc( p_hWnd, p_uiMessage, p_wParam, p_lParam );
}
#else
//----------------------------------------------------------------------------------------------
///	LINUX | OSX Specific
//----------------------------------------------------------------------------------------------
#endif

//----------------------------------------------------------------------------------------------
/// Attach window to handler list.
//----------------------------------------------------------------------------------------------
void WindowMessageManager::AttachWindow(WindowRenderTarget* p_window)
{
	m_windowRenderTargets.Add(p_window);
}

//----------------------------------------------------------------------------------------------
///	Detach window from handler list.
//----------------------------------------------------------------------------------------------
void WindowMessageManager::DetachWindow(WindowRenderTarget* p_window)
{
	m_windowRenderTargets.Remove(p_window);
}
	
//----------------------------------------------------------------------------------------------
///	Attach a window message handler to dispatching system.
//----------------------------------------------------------------------------------------------
void WindowMessageManager::AttachMessageHandler(WindowRenderTarget* p_window, IWindowMessageHandler *p_messageHandler)
{
	m_windowMessageHandlerMap.Insert(p_window, p_messageHandler);
}

//----------------------------------------------------------------------------------------------
///	Detach window message handler from dispatching system.
//----------------------------------------------------------------------------------------------
void WindowMessageManager::DetachMessageHandler(WindowRenderTarget* p_window)
{
	m_windowMessageHandlerMap.RemoveKey(p_window);
}

Meson_Vistas_END