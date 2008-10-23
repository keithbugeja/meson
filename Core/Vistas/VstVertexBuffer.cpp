//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstVertexBuffer.h"

using namespace Meson::Common;

Meson_Vistas_BEGIN
	
//----------------------------------------------------------------------------------------------
VertexBuffer::VertexBuffer(uint p_uiVertexSize, uint p_uiVertexCount, dword p_dwPool, dword p_dwUsage) 
	: HardwareResource(p_dwPool, p_dwUsage),
	  m_uiVertexSize(p_uiVertexSize), 
	  m_uiVertexCount(p_uiVertexCount)
{
}
//----------------------------------------------------------------------------------------------
uint VertexBuffer::GetVertexSize(void) const 
{ 
	return m_uiVertexSize; 
}
//----------------------------------------------------------------------------------------------
uint VertexBuffer::GetVertexCount(void) const 
{ 
	return m_uiVertexCount; 
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END