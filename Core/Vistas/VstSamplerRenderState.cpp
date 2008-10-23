//----------------------------------------------------------------------------------------------
#include "MesonException.h"
#include "VstVistasEngine.h"
#include "VstSamplerRenderState.h"
#include "VstSamplerState.h"

using namespace Meson::Common;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
SamplerRenderState::SamplerRenderState(void)
{
}
//----------------------------------------------------------------------------------------------
SamplerRenderState::~SamplerRenderState(void)
{
	RemoveAllSamplerUnits();
}
//----------------------------------------------------------------------------------------------
const SamplerRenderState& SamplerRenderState::operator=(const SamplerRenderState& p_samplerRenderState)
{
	RemoveAllSamplerUnits();

	m_samplerUnitMap = p_samplerRenderState.m_samplerUnitMap;
	return *this;
}
//----------------------------------------------------------------------------------------------
bool SamplerRenderState::operator==(const SamplerRenderState& p_samplerRenderState)
{
	if (&p_samplerRenderState == this)
		return true;

	return false;
}
//----------------------------------------------------------------------------------------------
IRenderState::RenderStateType SamplerRenderState::GetType(void) const
{
	return RST_SAMPLER;
}
//----------------------------------------------------------------------------------------------
uint SamplerRenderState::GetSamplerUnitCount(void) const
{
	return (uint)m_samplerUnitMap.Size();
}
//----------------------------------------------------------------------------------------------
bool SamplerRenderState::Contains(uint p_uiSamplerIndex) const
{
	return m_samplerUnitMap.ContainsKey(p_uiSamplerIndex);
}
//----------------------------------------------------------------------------------------------
bool SamplerRenderState::Contains(uint p_uiSamplerIndex, SamplerUnitPtr& p_pSamplerUnit) const
{
	return m_samplerUnitMap.ContainsKey(p_uiSamplerIndex, p_pSamplerUnit);
}
//----------------------------------------------------------------------------------------------
SamplerUnitEnumerator SamplerRenderState::GetSamplerUnitEnumerator(void)
{
	return m_samplerUnitMap.GetValueEnumerator();
}
//----------------------------------------------------------------------------------------------
SamplerUnitPtr SamplerRenderState::GetSamplerUnit(uint p_uiSamplerIndex) const
{
	SamplerUnitPtr pSamplerUnit(NULL);
	if (m_samplerUnitMap.ContainsKey(p_uiSamplerIndex, pSamplerUnit))
		return pSamplerUnit;

	throw new MesonException("Sampler Unit not defined.");
}
//----------------------------------------------------------------------------------------------
TextureStatePtr SamplerRenderState::GetTextureState(uint p_uiSamplerIndex) const
{
	SamplerUnitPtr pSamplerUnit(NULL);
	if (m_samplerUnitMap.ContainsKey(p_uiSamplerIndex, pSamplerUnit))
		return pSamplerUnit->GetTextureState();

	throw new MesonException("Sampler Unit not defined.");
}
//----------------------------------------------------------------------------------------------
SamplerStatePtr SamplerRenderState::GetSamplerState(uint p_uiSamplerIndex) const
{
	SamplerUnitPtr pSamplerUnit(NULL);
	if (m_samplerUnitMap.ContainsKey(p_uiSamplerIndex, pSamplerUnit))
		return pSamplerUnit->GetSamplerState();

	throw new MesonException("Sampler Unit not defined.");
}
//----------------------------------------------------------------------------------------------
SamplerUnitPtr SamplerRenderState::AddSamplerUnit(uint p_uiSamplerIndex)
{
	if (m_samplerUnitMap.ContainsKey(p_uiSamplerIndex))
		throw new MesonException("Sampler Unit already exists.");

	SamplerUnitPtr pSamplerUnit(new SamplerUnit(p_uiSamplerIndex));
	m_samplerUnitMap.Insert(p_uiSamplerIndex, pSamplerUnit);

	return pSamplerUnit;
}
//----------------------------------------------------------------------------------------------
SamplerUnitPtr SamplerRenderState::AddSamplerUnit(uint p_uiSamplerIndex, TextureStatePtr p_pTextureState, SamplerStatePtr p_pSamplerState)
{
	if (m_samplerUnitMap.ContainsKey(p_uiSamplerIndex))
		throw new MesonException("Sampler Unit already exists.");

	SamplerUnitPtr pSamplerUnit(new SamplerUnit(p_uiSamplerIndex, p_pTextureState, p_pSamplerState));
	m_samplerUnitMap.Insert(p_uiSamplerIndex, pSamplerUnit);

	return pSamplerUnit;
}
//----------------------------------------------------------------------------------------------
void SamplerRenderState::SetSamplerUnit(uint p_uiSamplerIndex, TextureStatePtr p_pTextureState, SamplerStatePtr p_pSamplerState)
{
	SamplerUnitPtr pSamplerUnit(NULL);
	if (!m_samplerUnitMap.ContainsKey(p_uiSamplerIndex, pSamplerUnit))
		throw new MesonException("Sampler Unit not defined.");

	pSamplerUnit->SetTextureState(p_pTextureState);
	pSamplerUnit->SetSamplerState(p_pSamplerState);
}
//----------------------------------------------------------------------------------------------
void SamplerRenderState::SetTextureState(uint p_uiSamplerIndex, TextureStatePtr p_pTextureState)
{
	SamplerUnitPtr pSamplerUnit(NULL);
	if (!m_samplerUnitMap.ContainsKey(p_uiSamplerIndex, pSamplerUnit))
		throw new MesonException("Sampler Unit not defined.");

	pSamplerUnit->SetTextureState(p_pTextureState);
}
//----------------------------------------------------------------------------------------------
void SamplerRenderState::SetSamplerState(uint p_uiSamplerIndex, SamplerStatePtr p_pSamplerState)
{
	SamplerUnitPtr pSamplerUnit(NULL);
	if (!m_samplerUnitMap.ContainsKey(p_uiSamplerIndex, pSamplerUnit))
		throw new MesonException("Sampler Unit not defined.");

	pSamplerUnit->SetSamplerState(p_pSamplerState);
}
//----------------------------------------------------------------------------------------------
void SamplerRenderState::RemoveSamplerUnit(uint p_uiSamplerIndex)
{
	m_samplerUnitMap.RemoveKey(p_uiSamplerIndex);
}
//----------------------------------------------------------------------------------------------
void SamplerRenderState::RemoveSamplerUnit(SamplerUnitPtr p_pSamplerUnit)
{
	m_samplerUnitMap.RemoveValue(p_pSamplerUnit);
}
//----------------------------------------------------------------------------------------------
void SamplerRenderState::RemoveAllSamplerUnits(void)
{
	SamplerUnitEnumerator samplerUnitEnumerator = m_samplerUnitMap.GetValueEnumerator();
	while(samplerUnitEnumerator.HasMoreElements())
		samplerUnitEnumerator.NextElement() = NULL;

	m_samplerUnitMap.Clear();
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
