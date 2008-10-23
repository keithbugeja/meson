//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "InstrumentationRendererFactory.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
InstrumentationRendererFactory::InstrumentationRendererFactory(void)
	: m_strName("Instrumentation")
{
}
//----------------------------------------------------------------------------------------------
InstrumentationRendererFactory::~InstrumentationRendererFactory(void)
{
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& InstrumentationRendererFactory::GetName(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
RendererPtr InstrumentationRendererFactory::CreateRenderer(void)
{
	RendererPtr pRenderer(new InstrumentationRenderer());
	return pRenderer;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END