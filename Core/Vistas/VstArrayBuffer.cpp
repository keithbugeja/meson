//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------

#include <memory.h>
#include "VstArrayBuffer.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
void ArrayBuffer::Copy(void* p_pSource, void* p_pDestination, uint p_uiCount)
{
	uint uiCopyLength = p_uiCount * m_uiElementSize;
	memcpy(p_pDestination, p_pSource, uiCopyLength);
}
//----------------------------------------------------------------------------------------------
void ArrayBuffer::CopyOffset(void* p_pSrc, uint p_uiOffsetSrc, void* p_pDst, uint p_uiOffsetDst, uint p_uiCount)
{
	uint uiCopyLength = p_uiCount * m_uiElementSize;
	
	void *pSrc = ((byte*)p_pSrc) + p_uiOffsetSrc * m_uiElementSize,
		 *pDst = ((byte*)p_pDst) + p_uiOffsetDst * m_uiElementSize;

	memcpy(pDst, pSrc, uiCopyLength);
}
//----------------------------------------------------------------------------------------------
void ArrayBuffer::GrowByFactor(float p_fGrowthFactor, uint p_uiMinimum)
{
	uint uiNewCount = (uint)(Meson::Common::Maths::Mathf::Ceiling((float)m_uiMaximumCount * (p_fGrowthFactor + 1)));
	
	if (uiNewCount <= p_uiMinimum)
		uiNewCount = (uint)(Meson::Common::Maths::Mathf::Ceiling((float)p_uiMinimum * (p_fGrowthFactor + 1)));

	void *pNewArrayBuffer = (void*)new byte[uiNewCount * m_uiElementSize];

	Copy(m_pArrayBuffer, pNewArrayBuffer, m_uiMaximumCount);

	// TODO: SAFEDELETE
	SAFE_DELETE_ARRAY(m_pArrayBuffer);

	m_pArrayBuffer = pNewArrayBuffer;
	m_uiMaximumCount = uiNewCount;
}
//----------------------------------------------------------------------------------------------
void ArrayBuffer::GrowByCount(uint p_uiCount)
{
	uint uiNewCount = m_uiMaximumCount + p_uiCount;
	void *pNewArrayBuffer = (void*)new byte[uiNewCount * m_uiElementSize];

	Copy(m_pArrayBuffer, pNewArrayBuffer, m_uiMaximumCount);

	// TODO: SAFEDELETE
	SAFE_DELETE_ARRAY(m_pArrayBuffer);

	m_pArrayBuffer = pNewArrayBuffer;
	m_uiMaximumCount = uiNewCount;
}
//----------------------------------------------------------------------------------------------
ArrayBuffer::ArrayBuffer(uint p_uiElementSize, uint p_uiElementCount, uint p_uiMaxElementCount)
	: m_uiElementSize(p_uiElementSize),
	  m_uiElementCount(p_uiElementCount),
	  m_uiMaximumCount(p_uiMaxElementCount),
	  m_fGrowthFactor(0.5f)
{
	m_pArrayBuffer = (void*)new byte[p_uiMaxElementCount * p_uiElementSize];
	memset(m_pArrayBuffer, 0, p_uiMaxElementCount * p_uiElementSize);
}
//----------------------------------------------------------------------------------------------
ArrayBuffer::ArrayBuffer(const ArrayBuffer& p_arrayBuffer)
	: m_uiElementSize(p_arrayBuffer.m_uiElementSize),
	  m_uiElementCount(p_arrayBuffer.m_uiElementCount),
	  m_uiMaximumCount(p_arrayBuffer.m_uiMaximumCount),
	  m_fGrowthFactor(p_arrayBuffer.m_fGrowthFactor)
{
	m_pArrayBuffer = (void*)new byte[m_uiMaximumCount * m_uiElementSize];
	Copy(p_arrayBuffer.m_pArrayBuffer, m_pArrayBuffer, m_uiElementCount);
}
//----------------------------------------------------------------------------------------------
ArrayBuffer::~ArrayBuffer(void)
{
	SAFE_DELETE_ARRAY(m_pArrayBuffer);
}
//----------------------------------------------------------------------------------------------
void ArrayBuffer::Insert(uint p_uiIndex, void* p_pData, uint p_uiCount)
{
	if (m_uiElementCount + p_uiCount > m_uiMaximumCount)
		GrowByFactor(m_fGrowthFactor, m_uiElementCount + p_uiCount);

	CopyOffset(m_pArrayBuffer, p_uiIndex, m_pArrayBuffer, p_uiIndex + p_uiCount, m_uiElementCount - p_uiIndex);
	CopyOffset(p_pData, 0, m_pArrayBuffer, p_uiIndex, p_uiCount);

	m_uiElementCount += p_uiCount;
}
//----------------------------------------------------------------------------------------------
void ArrayBuffer::Insert(uint p_uiIndex, const ArrayBuffer& p_arrayBuffer)
{
	MESON_ASSERT(p_arrayBuffer.m_uiElementSize == m_uiElementSize, "Array buffers differ in element size.");

	uint uiElementsToShift = m_uiElementCount - p_uiIndex;

	if (m_uiMaximumCount < m_uiElementCount + p_arrayBuffer.m_uiElementCount)
		GrowByCount((m_uiElementCount + p_arrayBuffer.m_uiElementCount) - m_uiMaximumCount);

	CopyOffset(m_pArrayBuffer, p_uiIndex, m_pArrayBuffer, p_uiIndex + p_arrayBuffer.m_uiElementCount, uiElementsToShift);
	CopyOffset(p_arrayBuffer.m_pArrayBuffer, 0, m_pArrayBuffer, p_uiIndex, p_arrayBuffer.m_uiElementCount);

	m_uiElementCount += p_arrayBuffer.m_uiElementCount;
}
//----------------------------------------------------------------------------------------------
void ArrayBuffer::Add(void* p_pData, uint p_uiCount)
{
	if (m_uiElementCount + p_uiCount > m_uiMaximumCount)
		GrowByFactor(m_fGrowthFactor, m_uiElementCount + p_uiCount);

	CopyOffset(p_pData, 0, m_pArrayBuffer, m_uiElementCount, p_uiCount);

	m_uiElementCount += p_uiCount;
}
//----------------------------------------------------------------------------------------------
void ArrayBuffer::Add(const ArrayBuffer& p_arrayBuffer)
{
	MESON_ASSERT(p_arrayBuffer.m_uiElementSize == m_uiElementSize, "Array buffers differ in element size.");

	if (m_uiMaximumCount < m_uiElementCount + p_arrayBuffer.m_uiElementCount)
		GrowByCount((m_uiElementCount + p_arrayBuffer.m_uiElementCount) - m_uiMaximumCount);

	CopyOffset(p_arrayBuffer.m_pArrayBuffer, 0, m_pArrayBuffer, m_uiElementCount, p_arrayBuffer.m_uiElementCount);

	m_uiElementCount += p_arrayBuffer.m_uiElementCount;
}
//----------------------------------------------------------------------------------------------
void ArrayBuffer::Set(uint p_uiIndex, void* p_pData, uint p_uiCount)
{
	MESON_ASSERT(p_uiIndex + p_uiCount <= m_uiElementCount, "Element index exceeds element count");
	CopyOffset(p_pData, 0, m_pArrayBuffer, p_uiIndex, p_uiCount);
}
//----------------------------------------------------------------------------------------------
void ArrayBuffer::Set(uint p_uiIndex, const ArrayBuffer& p_arrayBuffer)
{
	MESON_ASSERT(p_arrayBuffer.m_uiElementSize == m_uiElementSize, "Array buffers differ in element size.");

	uint uiCount = MIN(p_arrayBuffer.m_uiElementCount, m_uiMaximumCount - p_uiIndex);
	CopyOffset(p_arrayBuffer.m_pArrayBuffer, 0, m_pArrayBuffer, p_uiIndex, uiCount);
}
//----------------------------------------------------------------------------------------------
void ArrayBuffer::RemoveAt(uint p_uiIndex)
{
	MESON_ASSERT(p_uiIndex < m_uiElementCount, "Element index exceeds element count");
	
	if (m_uiElementCount > 0) 
	{
		m_uiElementCount--;
		CopyOffset(m_pArrayBuffer, p_uiIndex + 1, m_pArrayBuffer, p_uiIndex, m_uiElementCount - p_uiIndex);
	}
}
//----------------------------------------------------------------------------------------------
void* ArrayBuffer::ElementAt(uint p_uiIndex)
{
	MESON_ASSERT(p_uiIndex < m_uiElementCount, "Element index exceeds element count");
	return ((byte*)m_pArrayBuffer) + p_uiIndex * m_uiElementSize;
}
//----------------------------------------------------------------------------------------------
int ArrayBuffer::IndexOf(void* p_pData)
{
	byte *pList = (byte*)m_pArrayBuffer,
		 *pItem = (byte*)p_pData;

	for (uint uiIdx = 0; uiIdx < m_uiElementCount; uiIdx++, pList += m_uiElementSize)
		if (memcmp(pList, pItem, m_uiElementSize) == 0)
			return (int)uiIdx;

	return -1;
}
//----------------------------------------------------------------------------------------------
void ArrayBuffer::Swap(uint p_uiIndex1, uint p_uiIndex2)
{
	if (m_uiElementCount == m_uiMaximumCount)
		GrowByFactor(m_fGrowthFactor);

	CopyOffset(m_pArrayBuffer, p_uiIndex1, m_pArrayBuffer, m_uiMaximumCount - 1);
	CopyOffset(m_pArrayBuffer, p_uiIndex2, m_pArrayBuffer, p_uiIndex1);
	CopyOffset(m_pArrayBuffer, m_uiMaximumCount - 1, m_pArrayBuffer, p_uiIndex2);
}
//----------------------------------------------------------------------------------------------
uint ArrayBuffer::Size(void) const
{
	return m_uiElementCount;
}
//----------------------------------------------------------------------------------------------
void ArrayBuffer::Clear(void)
{
	m_uiElementCount = 0;
	memset(m_pArrayBuffer, 0, m_uiMaximumCount * m_uiElementSize);
}
//----------------------------------------------------------------------------------------------
void ArrayBuffer::SetGrowthFactor(float p_fGrowthFactor)
{
	m_fGrowthFactor = p_fGrowthFactor;
}
//----------------------------------------------------------------------------------------------
void ArrayBuffer::Rebind(uint p_uiElementSize, uint p_uiElementCount, uint p_uiMaxElementCount)
{
	m_uiElementSize = p_uiElementSize;
	m_uiElementCount = p_uiElementCount;
	m_uiMaximumCount = p_uiMaxElementCount;

	SAFE_DELETE_ARRAY(m_pArrayBuffer);

	m_pArrayBuffer = (void*)new byte[p_uiMaxElementCount * p_uiElementSize];
	memset(m_pArrayBuffer, 0, p_uiMaxElementCount * p_uiElementSize);
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
