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
	class EffectPass : public IPass
	{
	public:
		EffectPass(void); 
		EffectPass(const Meson::Common::Text::String& p_strName);

		IPass::PassType GetType(void) const;

		/** Determines the number of times this pass is iterated over.
		 */
		bool IsSingleIteration(void) const;
		uint GetIterationCount(void) const;
		void SetIterationCount(uint p_uiIterations);

	protected:
		uint m_uiIterationCount;
	};
	
	typedef Meson::Common::TPointer<EffectPass> EffectPassPtr;
	//----------------------------------------------------------------------------------------------
Meson_Vistas_END