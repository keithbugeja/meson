//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstAbstractGeometry.h"
#include "VstColour.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Vistas;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
AbstractGeometry::AbstractGeometry(void)
	: m_ePrimitiveType(IRenderDevice::PT_TRIANGLE_LIST),
	  m_pIndexData(new IndexData())
{
	VertexDescriptorPtr pVertexDescriptor(new VertexDescriptor());
	m_pVertexData = new VertexData(pVertexDescriptor);
	
}
//----------------------------------------------------------------------------------------------
AbstractGeometry::AbstractGeometry(IRenderDevice::PrimitiveType p_ePrimitiveType)
	: m_ePrimitiveType(p_ePrimitiveType),
	  m_pIndexData(new IndexData())
{
	VertexDescriptorPtr pVertexDescriptor(new VertexDescriptor());
	m_pVertexData = new VertexData(pVertexDescriptor);
	
}
//----------------------------------------------------------------------------------------------
AbstractGeometry::AbstractGeometry(IRenderDevice::PrimitiveType p_ePrimitiveType, VertexDescriptorPtr p_pVertexDescriptor)
	: m_ePrimitiveType(p_ePrimitiveType),
	  m_pIndexData(new IndexData()),
	  m_pVertexData(new VertexData(p_pVertexDescriptor))
{
}
//----------------------------------------------------------------------------------------------
AbstractGeometry::AbstractGeometry(IRenderDevice::PrimitiveType p_ePrimitiveType, VertexDataPtr p_pVertexData, IndexDataPtr p_pIndexData)
	: m_ePrimitiveType(p_ePrimitiveType),
	  m_pIndexData(p_pIndexData),
	  m_pVertexData(p_pVertexData)
{
}
//----------------------------------------------------------------------------------------------
VertexDataPtr AbstractGeometry::GetVertexData(void)
{
	return m_pVertexData;
}
//----------------------------------------------------------------------------------------------
IndexDataPtr AbstractGeometry::GetIndexData(void)
{
	return m_pIndexData;
}
//----------------------------------------------------------------------------------------------
IRenderDevice::PrimitiveType AbstractGeometry::GetPrimitiveType(void) const
{
	return m_ePrimitiveType;
}
//----------------------------------------------------------------------------------------------
void AbstractGeometry::SetPrimitiveType(IRenderDevice::PrimitiveType p_ePrimitiveType)
{
	m_ePrimitiveType = p_ePrimitiveType;
}
//----------------------------------------------------------------------------------------------
void AbstractGeometry::SetVertexData(VertexDataPtr p_pVertexData)
{
	m_pVertexData = p_pVertexData;
}
//----------------------------------------------------------------------------------------------
void AbstractGeometry::SetIndexData(IndexDataPtr p_pIndexData)
{
	m_pIndexData = p_pIndexData;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END