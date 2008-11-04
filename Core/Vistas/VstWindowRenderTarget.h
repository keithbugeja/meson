//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Render::WindowRenderTarget
//	Interface class for window render targets.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Configuration.h"

#include "VstTypes.h"
#include "VstRenderTarget.h"

Meson_Vistas_BEGIN

class VISTAS_API WindowRenderTarget : public RenderTarget
{
public:
	const RenderTargetType GetType(void) const { return RTT_WINDOW; }

	virtual void Create(const Meson::Common::Text::String& p_strName, uint p_uiWidth, uint p_uiHeight, bool p_bFullScreen, Meson::Common::Config::Configuration* p_pConfiguration) = 0;
	virtual void Destroy(void) = 0;

	virtual void Present(void) = 0;
	virtual bool IsPrimaryWindow(void) { return m_bIsPrimaryWindow; }

	virtual void OnWindowMoved(void) = 0;
	virtual void OnWindowResized(void) = 0;

protected:
	bool m_bIsPrimaryWindow,
		 m_bIsFullScreen;

	int m_nLeft,
		m_nTop;
};

Meson_Vistas_END
