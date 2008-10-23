//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstVertexDeclaration.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
/** Section for the VertexElement class.
 */
//----------------------------------------------------------------------------------------------
VertexElement::VertexElement(uint p_uiStream, uint p_uiOffset, VertexDataType p_eVertexDataType, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex)
	: m_uiStream(p_uiStream), 
	  m_uiOffset(p_uiOffset), 
	  m_eVertexDataType(p_eVertexDataType), 
	  m_eVertexDataFunction(p_eVertexDataFunction), 
	  m_uiUsageIndex(p_uiUsageIndex)
{
}
//----------------------------------------------------------------------------------------------
VertexElement::VertexElement(const VertexElement& p_vertexElement)
{
	m_uiStream = p_vertexElement.m_uiStream;
	m_uiOffset = p_vertexElement.m_uiOffset;
	m_eVertexDataType = p_vertexElement.m_eVertexDataType;
	m_eVertexDataFunction = p_vertexElement.m_eVertexDataFunction;
	m_uiUsageIndex = p_vertexElement.m_uiUsageIndex;
}
//----------------------------------------------------------------------------------------------
void VertexElement::SetVertexElement(uint p_uiStream, uint p_uiOffset, VertexDataType p_eVertexDataType, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex)
{
	m_uiStream = p_uiStream;
	m_uiOffset = p_uiOffset;
	m_eVertexDataType = p_eVertexDataType;
	m_eVertexDataFunction = p_eVertexDataFunction;
	m_uiUsageIndex = p_uiUsageIndex;
}
//----------------------------------------------------------------------------------------------
void VertexElement::SetVertexDataType(VertexDataType p_eVertexDataType)
{
	m_eVertexDataType = p_eVertexDataType;
}
//----------------------------------------------------------------------------------------------
void VertexElement::SetVertexDataFunction(VertexDataFunction p_eVertexDataFunction)
{
	m_eVertexDataFunction = p_eVertexDataFunction;
}
//----------------------------------------------------------------------------------------------
void VertexElement::SetUsageIndex(uint p_uiUsageIndex)
{
	m_uiUsageIndex = p_uiUsageIndex;
}
//----------------------------------------------------------------------------------------------
void VertexElement::SetStream(uint p_uiStream)
{
	m_uiStream = p_uiStream;
}
//----------------------------------------------------------------------------------------------
void VertexElement::SetOffset(uint p_uiOffset)
{
	m_uiOffset = p_uiOffset;
}
//----------------------------------------------------------------------------------------------
VertexDataType VertexElement::GetVertexDataType(void) const
{ 
	return m_eVertexDataType;
}
//----------------------------------------------------------------------------------------------
VertexDataFunction VertexElement::GetVertexDataFunction(void) const
{ 
	return m_eVertexDataFunction; 
}
//----------------------------------------------------------------------------------------------
uint VertexElement::GetUsageIndex(void) const
{ 
	return m_uiUsageIndex; 
}
//----------------------------------------------------------------------------------------------
uint VertexElement::GetStream(void) const
{ 
	return m_uiStream; 
}
//----------------------------------------------------------------------------------------------
uint VertexElement::GetOffset(void) const
{ 
	return m_uiOffset; 
}
//----------------------------------------------------------------------------------------------
bool VertexElement::IsVertexDataType(VertexDataType p_eVertexDataType) const
{
	return (p_eVertexDataType == m_eVertexDataType);
}
//----------------------------------------------------------------------------------------------
bool VertexElement::IsVertexDataFunction(VertexDataFunction p_eVertexDataFunction) const
{
	return (p_eVertexDataFunction == m_eVertexDataFunction);
}

//----------------------------------------------------------------------------------------------
/** Section for the VertexDeclaration class.
 */
//----------------------------------------------------------------------------------------------
VertexDeclaration::~VertexDeclaration(void)
{
	RemoveAllVertexElements();
}
//----------------------------------------------------------------------------------------------
void VertexDeclaration::RefreshStreamSet(void)
{
	m_streamSet.Clear();

	TEnumerator<VertexElement*>& vertexElementEnumerator = m_vertexElementList.GetEnumerator();
	VertexElement* pVertexElement = NULL;

	while (vertexElementEnumerator.HasMoreElements())
	{
		pVertexElement = vertexElementEnumerator.NextElement();
		m_streamSet.Insert(pVertexElement->GetStream());	
	}
}
//----------------------------------------------------------------------------------------------
bool VertexDeclaration::IsAllocated(uint p_uiStream, uint p_uiOffset)
{
	if (m_streamSet.ContainsElement(p_uiStream))
	{
		VertexElement* pVertexElement = NULL;
		TEnumerator<VertexElement*>& vertexElementEnumerator = m_vertexElementList.GetEnumerator();

		while (vertexElementEnumerator.HasMoreElements())
		{
			pVertexElement = vertexElementEnumerator.NextElement();
			
			if (pVertexElement->GetStream() == p_uiStream && pVertexElement->GetOffset() == p_uiOffset)
				return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------
bool VertexDeclaration::IsAllocated(uint p_uiStream, uint p_uiOffset, VertexElement** p_pVertexElement)
{
	if (m_streamSet.ContainsElement(p_uiStream))
	{
		VertexElement* pVertexElement = NULL;
		TEnumerator<VertexElement*>& vertexElementEnumerator = m_vertexElementList.GetEnumerator();

		while (vertexElementEnumerator.HasMoreElements())
		{
			pVertexElement = vertexElementEnumerator.NextElement();
			
			if (pVertexElement->GetStream() == p_uiStream && pVertexElement->GetOffset() == p_uiOffset)
			{
				*p_pVertexElement = pVertexElement;
				return true;
			}
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------
bool VertexDeclaration::Contains(uint p_uiStream, VertexDataFunction p_eVertexDataFunction, VertexDataType p_eVertexDataType, uint p_uiUsageIndex)
{
	if (m_streamSet.ContainsElement(p_uiStream))
	{
		TEnumerator<VertexElement*>& vertexElementEnumerator = m_vertexElementList.GetEnumerator();
		VertexElement* pVertexElement = NULL;

		while (vertexElementEnumerator.HasMoreElements())
		{
			pVertexElement = vertexElementEnumerator.NextElement();

			if (pVertexElement->GetStream() == p_uiStream && 
				pVertexElement->IsVertexDataFunction(p_eVertexDataFunction) && 
				pVertexElement->IsVertexDataType(p_eVertexDataType) && 
				pVertexElement->GetUsageIndex() == p_uiUsageIndex)
				return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------
bool VertexDeclaration::Contains(uint p_uiStream, VertexDataFunction p_eVertexDataFunction, VertexDataType p_eVertexDataType, uint p_uiUsageIndex, VertexElement** p_pVertexElement)
{
	if (m_streamSet.ContainsElement(p_uiStream))
	{
		TEnumerator<VertexElement*>& vertexElementEnumerator = m_vertexElementList.GetEnumerator();
		VertexElement* pVertexElement = NULL;

		while (vertexElementEnumerator.HasMoreElements())
		{
			pVertexElement = vertexElementEnumerator.NextElement();

			if (pVertexElement->GetStream() == p_uiStream &&
				pVertexElement->IsVertexDataFunction(p_eVertexDataFunction) && 
				pVertexElement->IsVertexDataType(p_eVertexDataType) && 
				pVertexElement->GetUsageIndex() == p_uiUsageIndex)
			{
				*p_pVertexElement = pVertexElement;
				return true;
			}
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------
bool VertexDeclaration::Contains(uint p_uiStream, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex)
{
	if (m_streamSet.ContainsElement(p_uiStream))
	{
		TEnumerator<VertexElement*>& vertexElementEnumerator = m_vertexElementList.GetEnumerator();
		VertexElement* pVertexElement = NULL;

		while (vertexElementEnumerator.HasMoreElements())
		{
			pVertexElement = vertexElementEnumerator.NextElement();

			if (pVertexElement->GetStream() == p_uiStream && 
				pVertexElement->IsVertexDataFunction(p_eVertexDataFunction) && 
				pVertexElement->GetUsageIndex() == p_uiUsageIndex)
				return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------
bool VertexDeclaration::Contains(uint p_uiStream, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex, VertexElement** p_pVertexElement)
{
	if (m_streamSet.ContainsElement(p_uiStream))
	{
		TEnumerator<VertexElement*>& vertexElementEnumerator = m_vertexElementList.GetEnumerator();
		VertexElement* pVertexElement = NULL;

		while (vertexElementEnumerator.HasMoreElements())
		{
			pVertexElement = vertexElementEnumerator.NextElement();

			if (pVertexElement->GetStream() == p_uiStream && 
				pVertexElement->IsVertexDataFunction(p_eVertexDataFunction) && 
				pVertexElement->GetUsageIndex() == p_uiUsageIndex)
			{
				*p_pVertexElement = pVertexElement;
				return true;
			}
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------
uint VertexDeclaration::GetCount(uint p_uiStream, VertexDataFunction p_eVertexDataFunction)
{
	if (m_streamSet.ContainsElement(p_uiStream))
	{
		TEnumerator<VertexElement*>& vertexElementEnumerator = m_vertexElementList.GetEnumerator();
		VertexElement* pVertexElement = NULL;
		uint uiFunctionCount = 0;

		while (vertexElementEnumerator.HasMoreElements())
		{
			pVertexElement = vertexElementEnumerator.NextElement();

			if (pVertexElement->GetStream() == p_uiStream && pVertexElement->IsVertexDataFunction(p_eVertexDataFunction))
				uiFunctionCount++;
		}

		return uiFunctionCount;
	}

	return 0;
}
//----------------------------------------------------------------------------------------------
uint VertexDeclaration::GetCount(uint p_uiStream, VertexDataFunction p_eVertexDataFunction, VertexDataType p_eVertexDataType)
{
	if (m_streamSet.ContainsElement(p_uiStream))
	{
		TEnumerator<VertexElement*>& vertexElementEnumerator = m_vertexElementList.GetEnumerator();
		VertexElement* pVertexElement = NULL;
		uint uiFunctionCount = 0;

		while (vertexElementEnumerator.HasMoreElements())
		{
			pVertexElement = vertexElementEnumerator.NextElement();

			if (pVertexElement->GetStream() == p_uiStream && 
				pVertexElement->IsVertexDataFunction(p_eVertexDataFunction) && 
				pVertexElement->GetVertexDataType() == p_eVertexDataType)
				uiFunctionCount++;
		}

		return uiFunctionCount;
	}

	return 0;
}
//----------------------------------------------------------------------------------------------
void VertexDeclaration::AddVertexElement(uint p_uiStream, uint p_uiOffset, VertexDataType p_eVertexDataType, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex)
{
	if (IsAllocated(p_uiStream, p_uiOffset))
		throw new MesonException("Cannot add vertex element : VertexElement offset is not unique within stream!");

	m_vertexElementList.Add(new VertexElement(p_uiStream, p_uiOffset, p_eVertexDataType, p_eVertexDataFunction, p_uiUsageIndex));
	m_streamSet.Insert(p_uiStream);
}
//----------------------------------------------------------------------------------------------
void VertexDeclaration::AppendVertexElement(uint p_uiStream, VertexDataType p_eVertexDataType, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex)
{
	uint uiOffset = GetStreamSize(p_uiStream);

	m_vertexElementList.Add(new VertexElement(p_uiStream, uiOffset, p_eVertexDataType, p_eVertexDataFunction, p_uiUsageIndex));
	m_streamSet.Insert(p_uiStream);
}
//----------------------------------------------------------------------------------------------
const VertexElement* VertexDeclaration::GetVertexElement(uint p_uiStream, uint p_uiOffset)
{
	VertexElement* pVertexElement = NULL;

	if (IsAllocated(p_uiStream, p_uiOffset, &pVertexElement))
		return pVertexElement;

	throw new MesonException("Cannot return vertex element : No vertex element at offset within stream!");
}
//----------------------------------------------------------------------------------------------
void VertexDeclaration::RemoveVertexElement(uint p_uiStream, uint p_uiOffset)
{
	VertexElement* pVertexElement = NULL;

	if (!IsAllocated(p_uiStream, p_uiOffset, &pVertexElement))
	{
		m_vertexElementList.Remove(pVertexElement);
		delete pVertexElement;

		RefreshStreamSet();
	}
	else
		throw new MesonException("Cannot remove vertex element : No vertex element at offset within stream!");
}
//----------------------------------------------------------------------------------------------
void VertexDeclaration::RemoveAllVertexElements(void)
{
	TEnumerator<VertexElement*>& vertexElementEnumerator = m_vertexElementList.GetEnumerator();
	VertexElement* pVertexElement = NULL;

	while( vertexElementEnumerator.HasMoreElements() )
	{
		pVertexElement = vertexElementEnumerator.NextElement();
		delete pVertexElement;
	}

	m_vertexElementList.Clear();
}
//----------------------------------------------------------------------------------------------
uint VertexDeclaration::GetStreamSize(uint p_uiStream)
{
	if (m_streamSet.ContainsElement(p_uiStream))
	{
		TEnumerator<VertexElement*>& vertexElementEnumerator = m_vertexElementList.GetEnumerator();
		VertexElement* pVertexElement = NULL;

		uint uiOffset = 0, 
			 uiSize = 0;

		while( vertexElementEnumerator.HasMoreElements() )
		{
			pVertexElement = vertexElementEnumerator.NextElement();

			if (pVertexElement->GetStream() == p_uiStream)
			{
				if (uiOffset <= pVertexElement->GetOffset())
				{
					uiOffset = pVertexElement->GetOffset();
					uiSize = SizeOf(pVertexElement->GetVertexDataType());
				}
			}
		}

		return uiOffset + uiSize;
	}

	return 0;
}
//----------------------------------------------------------------------------------------------
uint VertexDeclaration::SizeOf(VertexDataType p_eVertexDataType)
{
	switch (p_eVertexDataType)
	{
		case VDT_FLOAT1: return 4;
		case VDT_FLOAT2: return 8;
		case VDT_FLOAT3: return 12;
		case VDT_FLOAT4: return 16;
		case VDT_FLOAT16_2: return 4;
		case VDT_FLOAT16_4: return 8;
		case VDT_SHORT2: return 4;
		case VDT_SHORT4: return 8;
		case VDT_SHORT2N: return 4;
		case VDT_SHORT4N: return 4;
		case VDT_UBYTE4: return 4;
		case VDT_UBYTE4N: return 4;
		case VDT_COLOUR: return 16;
		default:
			throw new MesonException("Unknown Vertex Data Type");
	}
}

Meson_Vistas_END