//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Render::TargetRenderState
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"
#include "VstRenderState.h"

Meson_Vistas_BEGIN

	class VISTAS_API TargetRenderState : public IRenderState
	{
	public:
		TargetRenderState(void);
		RenderStateType GetType(void) const;
	
		void SetRenderTarget(const Meson::Common::Text::String& p_strRenderTargetName);
		const Meson::Common::Text::String& GetRenderTarget(void) const;

		void SetColourMask(dword p_eColourMask);
		dword GetColourMask(void) const;

		const TargetRenderState& operator= (const TargetRenderState& p_targetRenderState);
		bool operator==(const TargetRenderState& p_targetRenderState);

	protected:
		Meson::Common::Text::String m_strRenderTargetName;
		dword m_dwColourMask;
	};

	typedef Meson::Common::TPointer<TargetRenderState> TargetRenderStatePtr;

Meson_Vistas_END
