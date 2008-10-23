//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstTechniqueGroup.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
TechniqueGroup::TechniqueGroup(void)
{
}
//----------------------------------------------------------------------------------------------
TechniqueGroup::TechniqueGroup(const TechniqueGroup& p_techniqueGroup)
	: m_techniqueList(p_techniqueGroup.m_techniqueList)
{
}
//----------------------------------------------------------------------------------------------
TechniqueGroup::~TechniqueGroup(void)
{
	RemoveAllTechniques();
}
//----------------------------------------------------------------------------------------------
uint TechniqueGroup::GetTechniqueCount(void) const
{
	return (uint)m_techniqueList.Size();
}
//----------------------------------------------------------------------------------------------
TechniqueEnumerator TechniqueGroup::GetTechniqueEnumerator(void)
{
	return m_techniqueList.GetEnumerator();
}
//----------------------------------------------------------------------------------------------
bool TechniqueGroup::ContainsTechnique(const Meson::Common::Text::String& p_strTechniqueName)
{
	TechniqueEnumerator techniqueEnumerator = m_techniqueList.GetEnumerator();
	TechniquePtr pTechnique(NULL);

	while(techniqueEnumerator.HasMoreElements())
	{
		pTechnique = techniqueEnumerator.NextElement();
		if (pTechnique->GetName() == p_strTechniqueName)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------------------------
bool TechniqueGroup::ContainsTechnique(const Meson::Common::Text::String& p_strTechniqueName, TechniquePtr& p_pTechnique)
{
	TechniqueEnumerator techniqueEnumerator = m_techniqueList.GetEnumerator();
	TechniquePtr pTechnique(NULL);

	while(techniqueEnumerator.HasMoreElements())
	{
		pTechnique = techniqueEnumerator.NextElement();
		if (pTechnique->GetName() == p_strTechniqueName)
		{
			p_pTechnique = pTechnique;
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------------------------
TechniquePtr TechniqueGroup::CreateTechnique(const Meson::Common::Text::String& p_strTechniqueName)
{
	TechniquePtr pTechnique(new Technique(p_strTechniqueName));
	m_techniqueList.Add(pTechnique);

	return pTechnique;
}
//----------------------------------------------------------------------------------------------
TechniquePtr TechniqueGroup::CreateTechnique(void)
{
	TechniquePtr pTechnique(new Technique());
	m_techniqueList.Add(pTechnique);
	
	return pTechnique;
}
//----------------------------------------------------------------------------------------------
TechniquePtr TechniqueGroup::GetTechnique(uint p_uiTechniqueIndex)
{
	MESON_ASSERT(p_uiTechniqueIndex < m_techniqueList.Size(), "Cannot return technique : Technique index exceeds array size");
	return m_techniqueList.GetElement(p_uiTechniqueIndex);
}
//----------------------------------------------------------------------------------------------
TechniquePtr TechniqueGroup::GetTechnique(const Meson::Common::Text::String& p_strTechniqueName)
{
	TechniqueEnumerator techniqueEnumerator = m_techniqueList.GetEnumerator();
	TechniquePtr pTechnique(NULL);

	while(techniqueEnumerator.HasMoreElements())
	{
		pTechnique = techniqueEnumerator.NextElement();
		
		if (pTechnique->GetName() == p_strTechniqueName)
			return pTechnique;
	}

	throw new MesonException("Cannot return technique : No technique with such name exists", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void TechniqueGroup::AddTechnique(TechniquePtr p_pTechnique)
{
	m_techniqueList.Add(p_pTechnique);
}
//----------------------------------------------------------------------------------------------
void TechniqueGroup::AddTechniqueGroup(const TechniqueGroup& p_techniqueGroup)
{
	m_techniqueList.AddList(p_techniqueGroup.m_techniqueList);
}
//----------------------------------------------------------------------------------------------
void TechniqueGroup::RemoveTechnique(const Meson::Common::Text::String& p_strTechniqueName)
{
	TechniqueEnumerator techniqueEnumerator = m_techniqueList.GetEnumerator();
	TechniquePtr pTechnique(NULL);

	while(techniqueEnumerator.HasMoreElements())
	{
		pTechnique = techniqueEnumerator.NextElement();
		
		if (pTechnique->GetName() == p_strTechniqueName)
			m_techniqueList.Remove(pTechnique);
	}

	throw new MesonException("Cannot return technique : No technique with such name exists", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void TechniqueGroup::RemoveTechnique(uint p_uiTechniqueIndex)
{
	MESON_ASSERT(p_uiTechniqueIndex < m_techniqueList.Size(), "Cannot remove technique : Technique index exceeds array size");
	m_techniqueList[p_uiTechniqueIndex] = NULL;
	m_techniqueList.RemoveAt(p_uiTechniqueIndex);
}
//----------------------------------------------------------------------------------------------
void TechniqueGroup::RemoveTechniqueRange(uint p_uiStartIndex, uint p_uiCount)
{
	MESON_ASSERT(p_uiStartIndex + p_uiCount <= m_techniqueList.Size(), "Cannot remove techniques : Technique range exceeds array size");
	m_techniqueList.RemoveRange(p_uiStartIndex, p_uiCount);
}
//----------------------------------------------------------------------------------------------
void TechniqueGroup::RemoveAllTechniques(void)
{
	for (uint uiTechniqueIndex = 0; uiTechniqueIndex < m_techniqueList.Size(); uiTechniqueIndex++)
		m_techniqueList[uiTechniqueIndex] = NULL;

	m_techniqueList.Clear();
}
//----------------------------------------------------------------------------------------------
TechniqueGroup& TechniqueGroup::operator =(const Meson::Vistas::TechniqueGroup &p_techniqueGroup)
{
	m_techniqueList.Clear();
	m_techniqueList.AddList(p_techniqueGroup.m_techniqueList);

	return *this;
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END