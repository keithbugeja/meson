//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstRenderTarget.h"
#include "VstVisibleEntitySet.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class IRenderer 
		: public Meson::Common::TReferenceCounter<IRenderer>
	{
	public:
		virtual ~IRenderer() { };
		virtual const Meson::Common::Text::String& GetName(void) const = 0;
		
		virtual Meson::Common::TPointer<IRenderer> Clone(bool p_bShallow = true) = 0;
		virtual Meson::Common::TPointer<IRenderer> CreateInstance(void) = 0;

		virtual void SetVisibleSet(VisibleEntitySetPtr p_pVisibleEntitySet) = 0;
		virtual void Draw(void) = 0;
		virtual void Draw(VisibleEntityPtr p_pVisibleEntity) = 0;
		virtual void Draw(VisibleEntitySetPtr p_pVisibleEntitySet) = 0;

		virtual void SetGlobalEffects(bool p_bEnabled) = 0;
		virtual void SetLocalEffects(bool p_bEnabled) = 0;

		virtual bool IsEnabledGlobalEffects(void) const = 0;
		virtual bool IsEnabledLocalEffects(void) const = 0;
	};

	typedef Meson::Common::TPointer<IRenderer> RendererPtr;
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END