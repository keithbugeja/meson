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
	class VISTAS_API IlluminationPass : public IPass
	{
	public:
		IlluminationPass(void);
		IlluminationPass(const Meson::Common::Text::String& p_strName);

		PassType GetType(void) const;
		
		uint GetSimultaneousLightsProcessed(void) const;
		void SetSimultaneousLightsProcessed(uint p_uiLightCount);

		/** If this pass is bound to a named light object, it is only executed when that specific light
			is processed.
		 */
		bool IsBoundToNamedLight(void) const;
		void UnbindFromNamedLight(void);
		void BindToNamedLight(const Meson::Common::Text::String& p_strLightName);
		const Meson::Common::Text::String& GetBoundNamedLight(void) const;

		/** If this pass is bound to a specific light type, it is only executed for that specific light 
			type.
		 */
		bool IsBoundToLightType(void) const;
		void UnbindFromLightType(void);
		void BindToLightType(Light::LightType p_eLightType);
		Light::LightType GetBoundLightType(void) const;

	protected:
		Meson::Common::Text::String m_strNamedLight;

		Light::LightType m_eBoundLightType;

		bool m_bIsBoundToLightType,
			 m_bIsBoundToNamedLight;

		uint m_uiSimultaneousLightsProcessed;
	};
	
	typedef Meson::Common::TPointer<IlluminationPass> IlluminationPassPtr;
	//----------------------------------------------------------------------------------------------
Meson_Vistas_END