//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstVistasApplication.h"
#include "Stopwatch.h"

Meson_Vistas_BEGIN
	//----------------------------------------------------------------------------------------------
	/** \class VistasSandbox
		\brief A generic sanbox application for Vistas applications.
	 */
	//----------------------------------------------------------------------------------------------
	class VISTAS_API VistasSandbox : public VistasApplication
	{
	public:
		//----------------------------------------------------------------------------------------------
		/** \brief VistasSandbox constructor.
			\param p_strDeviceName Name of render device to start up.
			\param p_strTitle Title of application window.
			\param p_uiWidth Width in pixels of window.
			\param p_uiHeight Height in pixels of window.
			\param p_bFullScreen Starts the application in fullscreen mode.
			\param p_bInstrumentationDriver Starts the application in instrumentation driver mode.
		 */ 
		//----------------------------------------------------------------------------------------------
		VistasSandbox(const Meson::Common::Text::String& p_strDeviceName, const Meson::Common::Text::String& p_strTitle, uint p_uiWidth, uint p_uiHeight, bool p_bFullScreen, bool p_bInstrumentationDriver);

		//----------------------------------------------------------------------------------------------
		/** \brief VistasSandbox constructor.
		 */ 
		//----------------------------------------------------------------------------------------------
		~VistasSandbox();
		
		//----------------------------------------------------------------------------------------------
		/** \brief Loads the plug-ins required for execution of the Vistas Application.
		 */ 
		//----------------------------------------------------------------------------------------------
		void OnLoadPlugIns(void);

		//----------------------------------------------------------------------------------------------
		/** \brief Unloads formerly loaded plug-ins when the application terminates.
		 */ 
		//----------------------------------------------------------------------------------------------
		void OnUnloadPlugIns(void);

		void OnInitialise(void);
		void OnShutdown(void);

		void OnIdle(void);

		virtual void OnMoveCamera(void);
		virtual void OnFrameStart(void);
		virtual void OnFrameDraw(void);
		virtual void OnFrameEnd(void);

		virtual void OnDrawStart(void);
		virtual void OnDraw(void);
		virtual void OnDrawEnd(void);

		virtual void OnMouseMove(float p_fMouseX, float p_fMouseY);
		virtual void OnMouseUp(IWindowMessageHandler::MouseKey p_eKey);
		virtual void OnMouseDown(IWindowMessageHandler::MouseKey p_eKey);

		Meson::Common::Text::String MaterialCatalogue;
		Meson::Common::Text::String ApplicationPath;

		Meson::Vistas::Colour BackgroundColour;

		//----------------------------------------------------------------------------------------------
	protected:
		virtual void InitialisePlugIns(void);
		virtual void ShutdownPlugIns(void);

		virtual void InitialiseCamera(void);
		virtual void ShutdownCamera(void);

		virtual void InitialiseTimer(void);
		virtual void InitialiseContext(void);
		virtual void InitialiseRepository(void);
		virtual void InitialiseMaterialCatalogue(void);

		virtual void ConfigureCamera(float p_fAngle, float p_fDistance);

		virtual void LookUp(void);
		virtual void LookDown(void);
		virtual void TurnLeft(void);
		virtual void TurnRight(void);
		virtual void RollLeft(void);
		virtual void RollRight(void);
		
		virtual void MoveForward(void);
		virtual void MoveBack(void);
		virtual void MoveLeft(void);
		virtual void MoveRight(void);
		virtual void MoveUp(void);
		virtual void MoveDown(void);

		virtual void LookFree(float p_fDeltaX, float p_fDeltaY);

		//----------------------------------------------------------------------------------------------
	protected:
		SceneGroupNode* GetRootNode(void) const { return m_pRootNode; }
		
		Camera* GetCamera(void) const { return m_pCamera; }
		Viewport* GetViewport(void) const { return m_pViewport; }
		
		CullerPtr GetCuller(void) { return m_pCuller; }
		RendererPtr GetRenderer(void) { return m_pRenderer; }
		
		double GetFrameTime(void) { return m_dfFrameTime; }
		double GetDeltaTime(void) { return m_dfDeltaTime; }

		//----------------------------------------------------------------------------------------------
	protected:
		SceneGroupNode*	m_pRootNode;

		Camera*		m_pCamera;
		Viewport*	m_pViewport;
		
		CullerPtr	m_pCuller;
		RendererPtr	m_pRenderer;

		Meson::Common::System::StopWatch m_stopwatch;

		double	m_dfCurrentTime,
				m_dfPreviousTime,
				m_dfDeltaTime,
				m_dfFrameTime;

		float	m_fCameraRotationSpeed,
				m_fCameraMovementSpeed;

		Meson::Common::Maths::Vector2f m_vec2CameraMouseDelta,
									   m_vec2CameraMouseTrack;

		Meson::Common::Maths::Vector3f	m_vec3CameraPosition,
										m_vec3CameraAxes[3];
	};
//----------------------------------------------------------------------------------------------
Meson_Vistas_END
