//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Render::MaterialRenderState
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"
#include "VstColour.h"
#include "VstRenderState.h"

Meson_Vistas_BEGIN

	class VISTAS_API MaterialRenderState : public IRenderState
	{
	public:
		MaterialRenderState(void);

		RenderStateType GetType(void) const;

		void SetAmbient(float p_fRed, float p_fGreen, float p_fBlue);
		void SetAmbient(const Colour& p_ambient);

		void SetDiffuse(float p_fRed, float p_fGreen, float p_fBlue);
		void SetDiffuse(const Colour& p_diffuse);

		void SetEmissive(float p_fRed, float p_fGreen, float p_fBlue);
		void SetEmissive(const Colour& p_emissive);

		void SetSpecular(float p_fRed, float p_fGreen, float p_fBlue);
		void SetSpecular(const Colour& p_specular);

		void SetShininess(float p_fShininess);
		void SetAlpha(float p_fAlpha);

		void SetWireframe(bool p_bEnabled);

		Colour GetAmbient(void) const;
		Colour GetDiffuse(void) const;
		Colour GetEmissive(void) const;
		Colour GetSpecular(void) const;

		float GetShininess(void) const;
		float GetAlpha(void) const;

		bool IsWireframeEnabled(void) const;

		const MaterialRenderState& operator= (const MaterialRenderState& p_materialRenderState);
		bool operator==(const MaterialRenderState& p_materialRenderState);

	protected:
		Colour m_ambient,
			   m_diffuse,
			   m_emissive,
			   m_specular;

		float m_fShininess,
			  m_fAlpha;

		bool m_bIsWireframe;
	};

	typedef Meson::Common::TPointer<MaterialRenderState> MaterialRenderStatePtr;

Meson_Vistas_END
