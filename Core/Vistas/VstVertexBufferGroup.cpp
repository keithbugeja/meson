//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstVertexBufferGroup.h"

using namespace Meson::Common;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
VertexBufferGroup::VertexBufferGroup(void)
	: m_bIsStale(true),
	  m_uiMinimumVertexCount(0),
	  m_uiMaximumVertexCount(0)
{
}
//----------------------------------------------------------------------------------------------
VertexBufferGroup::VertexBufferGroup(VertexBufferPtr p_pVertexBuffer)
	: m_bIsStale(true),
	  m_uiMinimumVertexCount(0),
	  m_uiMaximumVertexCount(0)
{
	m_vertexBufferList.Add(p_pVertexBuffer);
}
//----------------------------------------------------------------------------------------------
VertexBufferGroup::VertexBufferGroup(const VertexBufferGroup& p_vertexBufferGroup)
{
	m_bIsStale = p_vertexBufferGroup.m_bIsStale;
	m_uiMaximumVertexCount = p_vertexBufferGroup.m_uiMaximumVertexCount;
	m_uiMinimumVertexCount = p_vertexBufferGroup.m_uiMinimumVertexCount;
	m_vertexBufferList = p_vertexBufferGroup.m_vertexBufferList;
}
//----------------------------------------------------------------------------------------------
VertexBufferGroup::~VertexBufferGroup(void)
{
	RemoveAllVertexBuffers();
}
//----------------------------------------------------------------------------------------------
void VertexBufferGroup::RefreshVertexCounts(void)
{
	m_uiMaximumVertexCount = 0;
	m_uiMinimumVertexCount = MAX_INT32;

	uint uiVertexCount = 0;

	for (uint uiIndex = 0; uiIndex < m_vertexBufferList.Size(); uiIndex++)
	{
		uiVertexCount = m_vertexBufferList.GetElement(uiIndex)->GetVertexCount();

		m_uiMaximumVertexCount = MAX(m_uiMaximumVertexCount, uiVertexCount);
		m_uiMinimumVertexCount = MIN(m_uiMinimumVertexCount, uiVertexCount);
	}

	m_bIsStale = false;
}
//----------------------------------------------------------------------------------------------
uint VertexBufferGroup::GetMinimumVertexCount(void)
{
	if (m_bIsStale)
		RefreshVertexCounts();

	return m_uiMinimumVertexCount;
}
//----------------------------------------------------------------------------------------------
uint VertexBufferGroup::GetMaximumVertexCount(void)
{
	if (m_bIsStale)
		RefreshVertexCounts();

	return m_uiMaximumVertexCount;
}
//----------------------------------------------------------------------------------------------
uint VertexBufferGroup::GetVertexBufferCount(void)
{
	return (uint)m_vertexBufferList.Size();
}
//----------------------------------------------------------------------------------------------
uint VertexBufferGroup::GetVertexBufferSize(uint p_uiIndex)
{
	if (p_uiIndex < m_vertexBufferList.Size())
		return m_vertexBufferList.GetElement(p_uiIndex)->GetVertexCount();

	throw new MesonException("Cannot return vertex buffer size : index exceeds number of vertex buffers in list", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void VertexBufferGroup::AddVertexBuffer(VertexBufferPtr p_pVertexBuffer)
{
	m_vertexBufferList.Add(p_pVertexBuffer);
}
//----------------------------------------------------------------------------------------------
void VertexBufferGroup::SetVertexBuffer(uint p_uiIndex, VertexBufferPtr p_pVertexBuffer)
{
	if (p_uiIndex < m_vertexBufferList.Size())
		m_vertexBufferList[p_uiIndex] = p_pVertexBuffer;
	else
		throw new MesonException("Cannot return change vertex buffer : index exceeds number of vertex buffers in list", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void VertexBufferGroup::InsertVertexBuffer(uint p_uiIndex, VertexBufferPtr p_pVertexBuffer)
{
	if (p_uiIndex == 0 || p_uiIndex < m_vertexBufferList.Size())
		m_vertexBufferList.Insert(p_uiIndex, p_pVertexBuffer);
	else
		throw new MesonException("Cannot insert vertex buffer : index exceeds number of vertex buffers in list", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void VertexBufferGroup::RemoveVertexBuffer(uint p_uiIndex)
{
	if (p_uiIndex < m_vertexBufferList.Size())
		m_vertexBufferList.RemoveAt(p_uiIndex);
	else
		throw new MesonException("Cannot insert vertex buffer : index exceeds number of vertex buffers in list", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void VertexBufferGroup::RemoveVertexBuffer(VertexBufferPtr p_pVertexBuffer)
{
	for (uint uiIndex = 0; uiIndex < m_vertexBufferList.Size(); uiIndex++)
	{
		if (m_vertexBufferList.GetElement(uiIndex) == p_pVertexBuffer)
		{
			m_vertexBufferList[uiIndex] = NULL;
			m_vertexBufferList.RemoveAt(uiIndex);
		}
	}
}
//----------------------------------------------------------------------------------------------
void VertexBufferGroup::RemoveAllVertexBuffers(void)
{
	for (uint uiIndex = 0; uiIndex < m_vertexBufferList.Size(); uiIndex++)
		m_vertexBufferList[uiIndex] = NULL;

	m_vertexBufferList.Clear();
}
//----------------------------------------------------------------------------------------------
void VertexBufferGroup::SwapVertexBuffers(uint p_uiIndex1, uint p_uiIndex2)
{
	if (p_uiIndex1 < m_vertexBufferList.Size() && p_uiIndex2 < m_vertexBufferList.Size())
		m_vertexBufferList.Swap(p_uiIndex1, p_uiIndex2);
	else
		throw new MesonException("Cannot swap vertex buffers : source, destination or both indices exceed number of vertex buffers in list", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
VertexBufferPtr VertexBufferGroup::GetVertexBuffer(uint p_uiIndex)
{
	return m_vertexBufferList.GetElement(p_uiIndex);
}
//----------------------------------------------------------------------------------------------
VertexBufferPtr VertexBufferGroup::operator[](uint p_uiIndex)
{
	return GetVertexBuffer(p_uiIndex);
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END