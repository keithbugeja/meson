//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstPropertySet.h"
#include "VstRenderable.h"
#include "VstVisibleEntitySet.h"

Meson_Vistas_BEGIN
	
	//----------------------------------------------------------------------------------------------
	class IEffect : public VistasObject, IPropertySet
	{
		VISTAS_IMPLEMENT_RTTI(IEffect, VistasObject);

	public:
		virtual ~IEffect(void) { }
		
		virtual void Initialise(IRenderDevice* p_pRenderDevice) { }
		virtual void Dispose(IRenderDevice* p_pRenderDevice) { }
	
		virtual void Draw(IRenderDevice* p_pRenderDevice, VisibleEntityPtr p_pVisibleEntity) { }
		virtual void Draw(IRenderDevice* p_pRenderDevice, VisibleEntitySetPtr p_pVisibleEntitySet) { }
	};

	typedef Meson::Common::TPointer<IEffect> EffectPtr;
	typedef Meson::Common::Collections::TEnumerator<EffectPtr>& EffectEnumerator;
	typedef Meson::Common::Collections::TArrayList<EffectPtr> EffectList;

Meson_Vistas_END