//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "MesonException.h"
#include "VstVistasEngine.h"
#include "VstTechnique.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
Technique::Technique(bool p_bSaveState)
	: m_strName(),
	  m_bIsSaveStateEnabled(p_bSaveState),
	  m_bIsTechniqueStarted(false),
	  m_bIsPassStarted(false)
{
}
//----------------------------------------------------------------------------------------------
Technique::Technique(const Meson::Common::Text::String& p_strName, bool p_bSaveState)
	: m_strName(p_strName),
	  m_bIsSaveStateEnabled(p_bSaveState),
	  m_bIsTechniqueStarted(false),
	  m_bIsPassStarted(false)
{
}
//----------------------------------------------------------------------------------------------
Technique::~Technique(void)
{
	RemoveAllPasses();
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& Technique::GetName(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
bool Technique::IsSaveStateEnabled(void) const
{
	return m_bIsSaveStateEnabled;
}
//----------------------------------------------------------------------------------------------
void Technique::SetSaveState(bool p_bEnabled)
{
	if (m_bIsTechniqueStarted)
		throw new MesonException("Cannot change save state within Begin/End technique block", __FILE__, __LINE__);
	
	m_bIsSaveStateEnabled = p_bEnabled;
}
//----------------------------------------------------------------------------------------------
uint Technique::GetPassCount(void) const
{
	return (uint)m_passList.Size();
}
//----------------------------------------------------------------------------------------------
uint Technique::GetPassCount(IPass::PassType p_ePassType) const
{
	return (uint)m_categorisedPassList[p_ePassType].Size();
}
//----------------------------------------------------------------------------------------------
bool Technique::ContainsPass(const Meson::Common::Text::String& p_strPassName)
{
	PassEnumerator passEnumerator = m_passList.GetEnumerator();
	PassPtr pPass(NULL);
	
	while(passEnumerator.HasMoreElements())
	{
		pPass = passEnumerator.NextElement();
		
		if (pPass->GetName() == p_strPassName)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------------------------
bool Technique::ContainsPass(const Meson::Common::Text::String& p_strPassName, PassPtr& p_pPass)
{
	PassEnumerator passEnumerator = m_passList.GetEnumerator();
	PassPtr pPass(NULL);
	
	while(passEnumerator.HasMoreElements())
	{
		pPass = passEnumerator.NextElement();
		
		if (pPass->GetName() == p_strPassName)
		{
			p_pPass = pPass;
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------------------------
bool Technique::ContainsPassType(IPass::PassType p_ePassType) const
{
	return m_categorisedPassList[p_ePassType].Size() > 0;
}
//----------------------------------------------------------------------------------------------
void Technique::RebuildTechnique(void)
{
	for (uint uiPassTypeIndex = 0, uiPassTypeCount = IPass::PT_COUNT; uiPassTypeIndex < uiPassTypeCount; uiPassTypeIndex++)
		m_categorisedPassList[uiPassTypeIndex].Clear();

	PassEnumerator passEnumerator = m_passList.GetEnumerator();

	while(passEnumerator.HasMoreElements())
	{
		PassPtr& pPass = passEnumerator.NextElement();
		m_categorisedPassList[(uint)(pPass->GetType())].Add(pPass);
	}
}
//----------------------------------------------------------------------------------------------
PassEnumerator Technique::GetPassEnumerator(void)
{
	return m_passList.GetEnumerator();
}
//----------------------------------------------------------------------------------------------
PassEnumerator Technique::GetPassEnumerator(IPass::PassType p_ePassType)
{
	return m_categorisedPassList[p_ePassType].GetEnumerator();
}
//----------------------------------------------------------------------------------------------
PassPtr Technique::CreatePass(IPass::PassType p_ePassType)
{
	return CreatePass(VistasEngine::GetInstance()->GenerateGuid(), p_ePassType);
}
//----------------------------------------------------------------------------------------------
PassPtr Technique::CreatePass(const Meson::Common::Text::String& p_strPassName, IPass::PassType p_ePassType)
{
	PassPtr pPass(NULL);

	switch(p_ePassType)
	{
		case IPass::PT_AMBIENT_PASS:
			pPass = new AmbientPass(p_strPassName);
			break;

		case IPass::PT_ILLUMINATION_PASS:
			pPass = new IlluminationPass(p_strPassName);
			break;

		case IPass::PT_EFFECT_PASS:
			pPass = new EffectPass(p_strPassName);
			break;
		
		default:
			return pPass;
	}

	m_categorisedPassList[p_ePassType].Add(pPass);
	m_passList.Add(pPass);

	return pPass;
}
//----------------------------------------------------------------------------------------------
PassPtr Technique::GetPass(uint p_uiPassNumber)
{
	if (p_uiPassNumber < m_passList.Size())
		return m_passList[p_uiPassNumber];

	throw new MesonException("Cannot return pass : Index exceeds number of passes in technique", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
PassPtr Technique::GetPass(const Meson::Common::Text::String& p_strPassName)
{
	PassEnumerator passEnumerator = m_passList.GetEnumerator();
	PassPtr pPass(NULL);
	
	while(passEnumerator.HasMoreElements())
	{
		pPass = passEnumerator.NextElement();
		
		if (pPass->GetName() == p_strPassName)
			return pPass;
	}

	throw new MesonException("Cannot return pass : No pass with such name exists", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void Technique::AddPass(PassPtr p_pPass)
{
	m_passList.Add(p_pPass);
}
//----------------------------------------------------------------------------------------------
void Technique::InsertPass(uint p_uiPassNumber, PassPtr p_pPass)
{
	m_passList.Insert(p_uiPassNumber, p_pPass);
}
//----------------------------------------------------------------------------------------------
void Technique::SetPass(uint p_uiPassNumber, PassPtr p_pPass)
{
	if (p_uiPassNumber < m_passList.Size())
		m_passList[p_uiPassNumber] = p_pPass;

	throw new MesonException("Cannot set pass : Index exceeds number of passes in technique", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void Technique::MovePass(uint p_uiSourcePassNumber, uint p_uiDestinationPassNumber)
{
	if (p_uiSourcePassNumber < m_passList.Size() && p_uiDestinationPassNumber < m_passList.Size())
	{
		if (p_uiSourcePassNumber >= p_uiDestinationPassNumber)
		{
			m_passList.Insert(p_uiDestinationPassNumber, m_passList.GetElement(p_uiSourcePassNumber));
			m_passList.RemoveAt(p_uiSourcePassNumber + 1);
		}
		else
		{
			m_passList.Insert(p_uiDestinationPassNumber + 1, m_passList.GetElement(p_uiSourcePassNumber));
			m_passList.RemoveAt(p_uiSourcePassNumber);
		}
	}
	else
		throw new MesonException("Cannot relocate pass : Source or destination index exceed number of passes in technique", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void Technique::RemovePass(const Meson::Common::Text::String& p_strPassName)
{
	PassPtr pPass(NULL);
	
	for (uint uiIndex = 0; uiIndex < m_passList.Size(); uiIndex++)
	{
		if (m_passList[uiIndex]->GetName() == p_strPassName)
		{
			m_passList[uiIndex] = NULL;
			m_passList.RemoveAt(uiIndex);
		}
	}
}
//----------------------------------------------------------------------------------------------
void Technique::RemovePass(uint p_uiPassNumber)
{
	if (p_uiPassNumber < m_passList.Size())
	{
		m_passList[p_uiPassNumber] = NULL;
		m_passList.RemoveAt(p_uiPassNumber);
	}
	else
		throw new MesonException("Cannot remove pass : Index exceeds number of passes in technique", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void Technique::RemoveAllPasses(void)
{
	for (uint uiIndex = 0; uiIndex < m_passList.Size(); uiIndex++)
		m_passList[uiIndex] = NULL;

	for (uint uiIndex = 0; uiIndex < IPass::PT_COUNT; uiIndex++)
		m_categorisedPassList[uiIndex].Clear();

	m_passList.Clear();
}
//----------------------------------------------------------------------------------------------
uint Technique::BeginTechnique(void)
{
	if (m_bIsTechniqueStarted)
		throw new MesonException("Cannot begin technique : Technique already started");

	m_bIsTechniqueStarted = true;

	if (m_bIsSaveStateEnabled) 
		SaveState();

	return (uint)m_passList.Size();
}
//----------------------------------------------------------------------------------------------
void Technique::BeginPass(uint p_uiPassNumber)
{
	if (m_bIsPassStarted)
		throw new MesonException("Cannot begin pass : Pass already started");

	if (p_uiPassNumber < m_passList.Size())
	{
		m_bIsPassStarted = true;
		m_uiCurrentPass = p_uiPassNumber;
		m_passList[p_uiPassNumber]->BeginPass(m_bIsSaveStateEnabled);
	}
	else
		throw new MesonException("Cannot being pass : Index exceeds number of passes in technique", __FILE__, __LINE__ );
}
//----------------------------------------------------------------------------------------------
void Technique::BeginPass(PassPtr p_pPass)
{
	if (m_bIsPassStarted)
		throw new MesonException("Cannot begin pass : Pass already started");

	// Get pass index
	uint uiPassNumber = m_passList.IndexOf(p_pPass);

	if (uiPassNumber != -1)
	{
		m_bIsPassStarted = true;
		m_uiCurrentPass = uiPassNumber;
		m_passList[uiPassNumber]->BeginPass(m_bIsSaveStateEnabled);
	}
	else
		throw new MesonException("Cannot being pass : Index exceeds number of passes in technique", __FILE__, __LINE__ );
}
//----------------------------------------------------------------------------------------------
void Technique::EndPass()
{
	if (!m_bIsPassStarted)
		throw new MesonException("Cannot end pass : Pass not started");

	m_passList[m_uiCurrentPass]->EndPass(m_bIsSaveStateEnabled);
	m_bIsPassStarted = false;
}
//----------------------------------------------------------------------------------------------
void Technique::EndTechnique()
{
	if (!m_bIsTechniqueStarted)
		throw new MesonException("Cannot end technique : Technique not started");

	m_bIsTechniqueStarted = false;

	if (m_bIsSaveStateEnabled)
		RestoreState();
}
//----------------------------------------------------------------------------------------------
void Technique::SaveState(void)
{
}
//----------------------------------------------------------------------------------------------
void Technique::RestoreState(void)
{
}
//----------------------------------------------------------------------------------------------
Technique& Technique::operator=(const Technique& p_technique)
{
	throw new MesonException("Operation not implemented.", __FILE__, __LINE__ );

	return *this;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END