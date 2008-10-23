//----------------------------------------------------------------------------------------------
#pragma once

#include "ArrayList.h"
#include "VstTypes.h"
#include "VstColour.h"
#include "VstRenderState.h"

Meson_Vistas_BEGIN

	class LightRenderState : public IRenderState
	{
	public:
		LightRenderState();
		~LightRenderState();

		RenderStateType GetType(void) const;

		bool IsLightingEnabled(void) const;
		void SetLighting(const bool p_bEnabled);

		const Colour& GetAmbient(void) const;
		void SetAmbient(const Colour& p_ambient);

		const LightRenderState& operator= (const LightRenderState& p_lightRenderState);
		bool operator==(const LightRenderState& p_lightRenderState);

	protected:
		bool m_bIsLightingEnabled;
		Colour m_ambientLight;
	};

	typedef Meson::Common::TPointer<LightRenderState> LightRenderStatePtr;

Meson_Vistas_END
