#pragma once

#include "VstVistasSandbox.h"
#include "VstSceneNode.h"
#include "VstSceneGroupNode.h"
#include "VstSceneGeometryNode.h"

#include "VstSkyEffect.h"
#include "VstShadowEffect.h"
#include "VstReflectionEffect.h"
#include "VstRefractionEffect.h"

class DX9SandboxApplication : public Meson::Vistas::VistasSandbox
{
public:
	DX9SandboxApplication(const Meson::Common::Text::String& p_strTitle, uint p_uiWidth, uint p_uiHeight, bool p_bFullScreen, bool p_bInstrumentationDriver);
	~DX9SandboxApplication();

	void OnInitialise(void);
	void OnDraw(void);
	void OnShutdown(void);

	Meson::Common::Text::String EnvironmentFilename;

protected:
	void LoadEnvironment(const Meson::Common::Text::String& p_strFilename); 

	void CreateScene(void);
	void DestroyScene(void);

	void InitialiseCamera(void);
	void ShutdownCamera(void);
};