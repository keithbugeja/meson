//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include <stdarg.h>
#include <varargs.h>

#include "VstIndexData.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Vistas;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
IndexData::IndexData(void)
{
	m_indexBuffer.Clear();
}
//----------------------------------------------------------------------------------------------
IndexData::IndexData(uint p_uiInitialIndexCount)
{
	m_indexBuffer.Clear();
	
	for (uint uiIndex = 0; uiIndex < p_uiInitialIndexCount; uiIndex++)
		m_indexBuffer.Add(0);
}
//----------------------------------------------------------------------------------------------
void* IndexData::GetIndexBuffer(uint p_uiIndex)
{
	return (void*)&m_indexBuffer.GetElement(p_uiIndex);
}
//----------------------------------------------------------------------------------------------
uint IndexData::GetIndex(uint p_uiIndex)
{
	return m_indexBuffer.GetElement(p_uiIndex);
}
//----------------------------------------------------------------------------------------------
void IndexData::Swap(uint p_uiIndex1, uint p_uiIndex2)
{
	m_indexBuffer.Swap(p_uiIndex1, p_uiIndex2);
}
//----------------------------------------------------------------------------------------------
void IndexData::AddMulti(uint p_uiCount, ...)
{
	va_list argList;
	va_start(argList, p_uiCount);

	uint uiIndex;

	for (uint uiIdx = 0; uiIdx < p_uiCount; uiIdx++)
	{
		uiIndex = va_arg(argList, uint);
		m_indexBuffer.Add(uiIndex);
	}

	va_end(argList);
}
//----------------------------------------------------------------------------------------------
void IndexData::Add(uint p_uiValue)
{
	m_indexBuffer.Add(p_uiValue);
}
//----------------------------------------------------------------------------------------------
void IndexData::Add(uint p_uiValue1, uint p_uiValue2)
{
	m_indexBuffer.Add(p_uiValue1);
	m_indexBuffer.Add(p_uiValue2);
}
//----------------------------------------------------------------------------------------------
void IndexData::Add(uint p_uiValue1, uint p_uiValue2, uint p_uiValue3)
{
	m_indexBuffer.Add(p_uiValue1);
	m_indexBuffer.Add(p_uiValue2);
	m_indexBuffer.Add(p_uiValue3);
}
//----------------------------------------------------------------------------------------------
void IndexData::Add(uint p_uiValue1, uint p_uiValue2, uint p_uiValue3, uint p_uiValue4)
{
	m_indexBuffer.Add(p_uiValue1);
	m_indexBuffer.Add(p_uiValue2);
	m_indexBuffer.Add(p_uiValue3);
	m_indexBuffer.Add(p_uiValue4);
}
//----------------------------------------------------------------------------------------------
void IndexData::InsertMulti(uint p_uiIndex, uint p_uiCount, ...)
{
	va_list argList;
	va_start(argList, p_uiCount);

	uint uiIndex;

	for (uint uiIdx = 0; uiIdx < p_uiCount; uiIdx++)
	{
		uiIndex = va_arg(argList, uint);
		m_indexBuffer.Insert(p_uiIndex + uiIdx, uiIndex);
	}

	va_end(argList);
}
//----------------------------------------------------------------------------------------------
void IndexData::Insert(uint p_uiIndex, uint p_uiValue)
{
	m_indexBuffer.Insert(p_uiIndex, p_uiValue);
}
//----------------------------------------------------------------------------------------------
void IndexData::Insert(uint p_uiIndex, uint p_uiValue1, uint p_uiValue2)
{
	m_indexBuffer.Insert(p_uiIndex, p_uiValue2);
	m_indexBuffer.Insert(p_uiIndex, p_uiValue1);
}
//----------------------------------------------------------------------------------------------
void IndexData::Insert(uint p_uiIndex, uint p_uiValue1, uint p_uiValue2, uint p_uiValue3)
{
	m_indexBuffer.Insert(p_uiIndex, p_uiValue3);
	m_indexBuffer.Insert(p_uiIndex, p_uiValue2);
	m_indexBuffer.Insert(p_uiIndex, p_uiValue1);
}
//----------------------------------------------------------------------------------------------
void IndexData::Insert(uint p_uiIndex, uint p_uiValue1, uint p_uiValue2, uint p_uiValue3, uint p_uiValue4)
{
	m_indexBuffer.Insert(p_uiIndex, p_uiValue4);
	m_indexBuffer.Insert(p_uiIndex, p_uiValue3);
	m_indexBuffer.Insert(p_uiIndex, p_uiValue2);
	m_indexBuffer.Insert(p_uiIndex, p_uiValue1);
}
//----------------------------------------------------------------------------------------------
void IndexData::Set(uint p_uiIndex, uint p_uiValue)
{
	m_indexBuffer[p_uiIndex] = p_uiValue;
}
//----------------------------------------------------------------------------------------------
void IndexData::RemoveAt(uint p_uiIndex)
{
	m_indexBuffer.RemoveAt(p_uiIndex);
}
//----------------------------------------------------------------------------------------------
void IndexData::RemoveRange(uint p_uiStart, uint p_uiLength)
{
	MESON_ASSERT(p_uiLength > 0, "Unable to remove range : length of range is zero.");
	m_indexBuffer.RemoveRange(p_uiStart, p_uiLength);
}
//----------------------------------------------------------------------------------------------
void IndexData::RemoveAll(void)
{
	m_indexBuffer.Clear();
}
//----------------------------------------------------------------------------------------------
uint IndexData::GetIndexCount(void) const
{
	return (uint)m_indexBuffer.Size();
}
//----------------------------------------------------------------------------------------------
uint IndexData::operator[] (uint p_uiIndex)
{
	return GetIndex(p_uiIndex);
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END