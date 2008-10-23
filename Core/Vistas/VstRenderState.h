//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Render::IRenderState
//	Interface class for device render-states.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"

Meson_Vistas_BEGIN

	class IRenderState : public Meson::Common::TReferenceCounter<IRenderState>
	{
	public:
		enum RenderStateType
		{
			RST_ALPHA,
			RST_BIAS,
			RST_CULL,
			RST_DEPTH,
			RST_LIGHT,
			RST_MATERIAL,
			RST_SAMPLER,
			RST_STENCIL,
			RST_TARGET,

			RST_COUNT
		};

	public:
		virtual RenderStateType GetType(void) const = 0;  
	};

	typedef Meson::Common::TPointer<IRenderState> RenderStatePtr;
	typedef Meson::Common::Collections::TArrayList<RenderStatePtr> RenderStateList;
	typedef Meson::Common::Collections::THashMap<IRenderState::RenderStateType, RenderStatePtr> RenderStateMap;
	typedef Meson::Common::Collections::TEnumerator<RenderStatePtr>& RenderStateEnumerator;

Meson_Vistas_END
