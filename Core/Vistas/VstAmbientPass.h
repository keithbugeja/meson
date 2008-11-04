//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstPass.h"

Meson_Vistas_BEGIN
	//----------------------------------------------------------------------------------------------
	class VISTAS_API AmbientPass : public IPass
	{
	public:
		AmbientPass(void); 
		AmbientPass(const Meson::Common::Text::String& p_strName);

		IPass::PassType GetType(void) const;

		/** Draws ambient pass even if illumination passes provide
			ambient component.
		 */
		//
		//

	protected:
		bool m_bForceDraw;
	};
	
	typedef Meson::Common::TPointer<EffectPass> EffectPassPtr;
	//----------------------------------------------------------------------------------------------
Meson_Vistas_END