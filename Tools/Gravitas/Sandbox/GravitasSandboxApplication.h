#pragma once

// Common headers
#include "ArrayList.h"
#include "StopWatch.h"

// Gravitas headers
#include "GrvGravitasEngine.h"
#include "GrvSimulator.h"

// Vistas headers
#include "VstVistasSandbox.h"
#include "VstSkyEffect.h"
#include "VstShadowEffect.h"
#include "VstReflectionEffect.h"

// Sandbox headers
#include "SandboxEntity.h"

class GravitasSandboxApplication
	: public Meson::Vistas::VistasSandbox
{
private: // private variables
	Meson::Common::Text::String m_strSimulationFilename;
	Meson::Common::Collections::TArrayList<SandboxEntity> m_listSandboxEntities;
	Meson::Common::System::StopWatch m_stopWatchPhysics;
	Meson::Common::System::StopWatch m_stopWatchGraphics;
	double m_dCurrentTime;
	bool m_bRunning;
	int m_nCameraLock;
	Meson::Gravitas::Body::BodyPtr m_pBodyPicking;
	Meson::Gravitas::Real m_rBodyPickingDepth;

	Meson::Gravitas::GravitasEngine*			m_pGravitasEngine;
	Meson::Gravitas::Simulation::SimulatorPtr	m_pSimulator;
	Meson::Gravitas::EnvironmentPtr				m_pEnvironment;

	Meson::Vistas::ShadowEffect*		m_pShadowMapEffect;
	Meson::Vistas::ReflectionEffect*	m_pReflectionEffect;
	Meson::Vistas::SkyEffect*			m_pSkyEffect;

public: // public methods
	GravitasSandboxApplication(uint p_uiWidth, uint p_uiHeight, bool p_bFullScreen, bool p_bInstrumentationDriver);
	~GravitasSandboxApplication();

	void OnInitialise(void);
	void OnShutdown(void);
	void OnIdle(void);
	void OnFrameDraw(void);

	void OnKeyDown(KeyCode p_eKey);

	void OnMouseMove(float p_fMouseX, float p_fMouseY);
	void OnMouseDown(IWindowMessageHandler::MouseKey p_eKey);
	void OnMouseUp(IWindowMessageHandler::MouseKey p_eKey);

	// custom interface
	void SetSimulationFilename(const Meson::Common::Text::String& p_strSimulationFilename);
	void InitialiseGravitas(void);
	void ShutdownGravitas(void);
	void LoadEnvironment(void);

protected:
	void CreateScene(void);
	void DestroyScene(void);
};