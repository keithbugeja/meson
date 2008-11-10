//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstEffect.h"

Meson_Vistas_BEGIN
	
	//----------------------------------------------------------------------------------------------
	class VISTAS_API LocalEffect : public IEffect
	{
		VISTAS_IMPLEMENT_RTTI(LocalEffect, IEffect);

	public:
		LocalEffect(void);
		virtual ~LocalEffect(void);
		
		virtual void Initialise(IRenderDevice* p_pRenderDevice);
		virtual void Dispose(IRenderDevice* p_pRenderDevice);
		
		virtual void Draw(IRenderDevice* p_pRenderDevice, VisibleEntityPtr p_pVisibleEntity);
		virtual void Draw(IRenderDevice* p_pRenderDevice, VisibleEntitySetPtr p_pVisibleEntitySet);
	};

Meson_Vistas_END