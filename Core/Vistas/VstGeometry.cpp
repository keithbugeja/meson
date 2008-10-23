//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstVistasEngine.h"
#include "VstGeometry.h"
#include "VstColour.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Vistas;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
/* Geometry subset
 */
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
GeometrySubset::GeometrySubset(void)
	: m_ePrimitiveType(IRenderDevice::PT_TRIANGLE_LIST),
	  m_dwFlags(GSF_NONE),
	  m_uiVertexCount(0),
	  m_uiVertexStart(0),
	  m_uiIndexStart(0),
	  m_uiPrimitiveCount(0)
{
}
//----------------------------------------------------------------------------------------------
GeometrySubset::GeometrySubset(IRenderDevice::PrimitiveType p_ePrimitiveType, uint p_uiVertexStart, uint p_uiVertexCount, uint p_uiIndexStart, uint p_uiPrimitiveCount)
	: m_ePrimitiveType(p_ePrimitiveType),
	  m_dwFlags(GSF_NONE),
	  m_uiVertexCount(p_uiVertexCount),
	  m_uiVertexStart(p_uiVertexStart),
	  m_uiIndexStart(p_uiIndexStart),
	  m_uiPrimitiveCount(p_uiPrimitiveCount)
{
}
//----------------------------------------------------------------------------------------------
GeometrySubset::GeometrySubset(const GeometrySubset& p_geometrySubset)
	: m_ePrimitiveType(p_geometrySubset.m_ePrimitiveType),
	  m_dwFlags(p_geometrySubset.m_dwFlags),
	  m_uiVertexCount(p_geometrySubset.m_uiVertexCount),
	  m_uiVertexStart(p_geometrySubset.m_uiVertexStart),
	  m_uiIndexStart(p_geometrySubset.m_uiIndexStart),
	  m_uiPrimitiveCount(p_geometrySubset.m_uiPrimitiveCount)
{
}
//----------------------------------------------------------------------------------------------
GeometrySubset& GeometrySubset::operator=(const GeometrySubset& p_geometrySubset)
{
	m_ePrimitiveType = p_geometrySubset.m_ePrimitiveType;
	m_dwFlags = p_geometrySubset.m_dwFlags;
	m_uiVertexCount = p_geometrySubset.m_uiVertexCount;
	m_uiVertexStart = p_geometrySubset.m_uiVertexStart;
	m_uiIndexStart = p_geometrySubset.m_uiIndexStart;
	m_uiPrimitiveCount = p_geometrySubset.m_uiPrimitiveCount;

	return *this;
}
//----------------------------------------------------------------------------------------------
bool GeometrySubset::operator==(const GeometrySubset& p_geometrySubset)
{
	return (m_ePrimitiveType == p_geometrySubset.m_ePrimitiveType &&
			m_dwFlags == p_geometrySubset.m_dwFlags &&
			m_uiVertexCount == p_geometrySubset.m_uiVertexCount &&
			m_uiVertexStart == p_geometrySubset.m_uiVertexStart &&
			m_uiIndexStart == p_geometrySubset.m_uiIndexStart &&
			m_uiPrimitiveCount == p_geometrySubset.m_uiPrimitiveCount);
}
//----------------------------------------------------------------------------------------------
IRenderDevice::PrimitiveType GeometrySubset::GetPrimitiveType(void) const
{
	return m_ePrimitiveType;
}
//----------------------------------------------------------------------------------------------
void GeometrySubset::SetPrimitiveType(IRenderDevice::PrimitiveType p_ePrimitiveType)
{
	m_ePrimitiveType = p_ePrimitiveType;
}
//----------------------------------------------------------------------------------------------
bool GeometrySubset::IsFlagSet(Flags p_eFlag) const
{
	return (m_dwFlags & p_eFlag) > 0;
}
//----------------------------------------------------------------------------------------------
void GeometrySubset::SetFlag(Flags p_eFlag, bool p_bEnabled)
{
	p_bEnabled ? m_dwFlags |= p_eFlag : m_dwFlags &= ~p_eFlag;
}
//----------------------------------------------------------------------------------------------
uint GeometrySubset::GetVertexStart(void) const
{
	return m_uiVertexStart;
}
//----------------------------------------------------------------------------------------------
uint GeometrySubset::GetVertexCount(void) const
{
	return m_uiVertexCount;
}
//----------------------------------------------------------------------------------------------
uint GeometrySubset::GetIndexStart(void) const
{
	return m_uiIndexStart;
}
//----------------------------------------------------------------------------------------------
uint GeometrySubset::GetPrimitiveCount(void) const
{
	return m_uiPrimitiveCount;
}
//----------------------------------------------------------------------------------------------
dword GeometrySubset::GetFlags(void) const
{
	return m_dwFlags;
}
//----------------------------------------------------------------------------------------------
void GeometrySubset::SetVertexStart(uint p_uiVertexStart)
{
	m_uiVertexStart = p_uiVertexStart;
}
//----------------------------------------------------------------------------------------------
void GeometrySubset::SetVertexCount(uint p_uiVertexCount)
{
	m_uiVertexCount = p_uiVertexCount;
}
//----------------------------------------------------------------------------------------------
void GeometrySubset::SetIndexStart(uint p_uiIndexStart)
{
	m_uiIndexStart = p_uiIndexStart;
}
//----------------------------------------------------------------------------------------------
void GeometrySubset::SetPrimitiveCount(uint p_uiPrimitiveCount)
{
	m_uiPrimitiveCount = p_uiPrimitiveCount;
}
//----------------------------------------------------------------------------------------------
void GeometrySubset::SetFlags(dword p_dwFlags)
{
	m_dwFlags = p_dwFlags;
}
//----------------------------------------------------------------------------------------------
uint GeometrySubset::GetPrimitiveCount(IRenderDevice::PrimitiveType p_ePrimitiveType, uint p_uiIndexCount)
{
	switch(p_ePrimitiveType)
	{
	case IRenderDevice::PT_POINTLIST:
		return p_uiIndexCount;
	case IRenderDevice::PT_LINELIST_SEGMENTS:
		return p_uiIndexCount / 2;
	case IRenderDevice::PT_LINELIST_OPEN:
		return p_uiIndexCount - 1;
	case IRenderDevice::PT_LINELIST_CLOSED:
		return p_uiIndexCount;
	case IRenderDevice::PT_TRIANGLE_LIST:
		return p_uiIndexCount / 3;
	case IRenderDevice::PT_TRIANGLE_STRIP:
		return p_uiIndexCount - 2;
	case IRenderDevice::PT_TRIANGLE_FAN:
		return p_uiIndexCount - 2;
	}

	return 0;
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
/** Geometry
 */
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
Geometry::Geometry(void)
	: Renderable(true)
	, m_pMaterial(NULL)
	, m_pGeometrySubsetList(NULL)
{
}
//----------------------------------------------------------------------------------------------
Geometry::Geometry(const Meson::Common::Text::String& p_strId)
	: Renderable(p_strId)
	, m_pMaterial(NULL)
	, m_pGeometrySubsetList(NULL)
{
}
//----------------------------------------------------------------------------------------------
Geometry::Geometry(const Geometry& p_geometry)
	: Renderable((Renderable&)p_geometry)
	, m_pMaterial(p_geometry.m_pMaterial)
	, m_pGeometrySubsetList(p_geometry.m_pGeometrySubsetList)
{
}
//----------------------------------------------------------------------------------------------
Geometry::~Geometry(void)
{
	m_pGeometrySubsetList = NULL;
	m_pMaterial = NULL;
}
//----------------------------------------------------------------------------------------------
void Geometry::SetGeometryParameters(IRenderDevice::PrimitiveType p_ePrimitiveType, VertexChannelPtr p_pVertexChannel, IndexBufferPtr p_pIndexBuffer)
{
	m_pVertexChannel = p_pVertexChannel;
	m_pIndexBuffer = p_pIndexBuffer;

	// Initialise GeometrySubsetList
	uint uiIndexCount = p_pIndexBuffer->GetIndexCount(),
		 uiVertexCount = p_pVertexChannel->GetVertexBufferGroup()->GetMinimumVertexCount();

	GeometrySubset geometrySubset(p_ePrimitiveType, 0, uiVertexCount, 0, GeometrySubset::GetPrimitiveCount(p_ePrimitiveType, uiIndexCount));
	m_pGeometrySubsetList = new GeometrySubsetList();
	m_pGeometrySubsetList->Add(geometrySubset);

	m_pMaterial = NULL;
}
//----------------------------------------------------------------------------------------------
void Geometry::SetGeometryParameters(IRenderDevice::PrimitiveType p_ePrimitiveType, VertexDeclarationPtr p_pVertexDeclaration, VertexBufferGroupPtr p_pVertexBufferGroup, IndexBufferPtr p_pIndexBuffer)
{
	m_bIsIndexed = true;

	// Initialise Index Buffer
	m_pIndexBuffer = p_pIndexBuffer;

	// Initialise VertexChannel
	m_pVertexChannel = new VertexChannel();
	m_pVertexChannel->SetVertexDeclaration(p_pVertexDeclaration);

	VertexDeclaration::StreamEnumerator streamEnumerator = p_pVertexDeclaration->GetStreamEnumerator();

	for (uint uiIndex = 0; uiIndex < p_pVertexBufferGroup->GetVertexBufferCount() && streamEnumerator.HasMoreElements(); uiIndex++)
		m_pVertexChannel->SetStreamBinding(streamEnumerator.NextElement(), p_pVertexBufferGroup->GetVertexBuffer(uiIndex));

	// Initialise GeometrySubsetList
	GeometrySubset geometrySubset(p_ePrimitiveType, 0, p_pVertexBufferGroup->GetMinimumVertexCount(), 0, GeometrySubset::GetPrimitiveCount(p_ePrimitiveType, p_pIndexBuffer->GetIndexCount()));
	m_pGeometrySubsetList = new GeometrySubsetList();
	m_pGeometrySubsetList->Add(geometrySubset);

	m_pMaterial = NULL;
}
//----------------------------------------------------------------------------------------------
void Geometry::SetGeometryParameters(IRenderDevice::PrimitiveType p_ePrimitiveType, VertexDeclarationPtr p_pVertexDeclaration, VertexBufferPtr p_pVertexBuffer, IndexBufferPtr p_pIndexBuffer)
{
	m_bIsIndexed = true;

	// Initialise Index Buffer
	m_pIndexBuffer = p_pIndexBuffer;

	// Initialise VertexChannel
	m_pVertexChannel = new VertexChannel();
	m_pVertexChannel->SetVertexDeclaration(p_pVertexDeclaration);
	m_pVertexChannel->SetStreamBinding(0, p_pVertexBuffer);
	
	// Initialise GeometrySubsetList
	GeometrySubset geometrySubset(p_ePrimitiveType, 0, p_pVertexBuffer->GetVertexCount(), 0, GeometrySubset::GetPrimitiveCount(p_ePrimitiveType, p_pIndexBuffer->GetIndexCount()));
	m_pGeometrySubsetList = new GeometrySubsetList();
	m_pGeometrySubsetList->Add(geometrySubset);

	m_pMaterial = NULL;
}
//----------------------------------------------------------------------------------------------
void Geometry::Draw(IRenderDevice* p_pRenderDevice)
{
	p_pRenderDevice->SetVertexChannel(m_pVertexChannel);
	p_pRenderDevice->SetIndexBuffer(m_pIndexBuffer);
	
	GeometrySubsetEnumerator geometrySubsetEnumerator = m_pGeometrySubsetList->GetEnumerator();
	
	while (geometrySubsetEnumerator.HasMoreElements())
	{
		GeometrySubset& geometrySubset = geometrySubsetEnumerator.NextElement();
		p_pRenderDevice->DrawIndexedPrimitive(geometrySubset.GetPrimitiveType(), geometrySubset.GetVertexStart(), geometrySubset.GetIndexStart(), 0, geometrySubset.GetVertexStart() + geometrySubset.GetVertexCount(), geometrySubset.GetPrimitiveCount());
	}
}
//----------------------------------------------------------------------------------------------
void Geometry::DrawSubset(IRenderDevice* p_pRenderDevice, uint p_uiSubset)
{
	MESON_ASSERT(p_uiSubset < m_pGeometrySubsetList->Size(), "Cannot draw subset : index out of range.");

	p_pRenderDevice->SetVertexChannel(m_pVertexChannel);
	p_pRenderDevice->SetIndexBuffer(m_pIndexBuffer);

	GeometrySubset& geometrySubset = (*m_pGeometrySubsetList)[p_uiSubset];
	p_pRenderDevice->DrawIndexedPrimitive(geometrySubset.GetPrimitiveType(), geometrySubset.GetVertexStart(), geometrySubset.GetIndexStart(), 0, geometrySubset.GetVertexStart() + geometrySubset.GetVertexCount(), geometrySubset.GetPrimitiveCount());
}
//----------------------------------------------------------------------------------------------
uint Geometry::GetSubsetCount(void) const
{
	return (uint)m_pGeometrySubsetList->Size();
}
//----------------------------------------------------------------------------------------------
MaterialPtr Geometry::GetMaterial(void)
{
	return m_pMaterial;
}
//----------------------------------------------------------------------------------------------
void Geometry::SetMaterial(MaterialPtr p_pMaterial)
{
	m_pMaterial = p_pMaterial;
}
//----------------------------------------------------------------------------------------------
BoundingVolumePtr Geometry::GetBoundingVolume(void)
{
	return m_pBoundingVolume;
}
//----------------------------------------------------------------------------------------------
void Geometry::SetBoundingVolume(BoundingVolumePtr p_pBoundingVolume)
{
	m_pBoundingVolume = p_pBoundingVolume;
}
//----------------------------------------------------------------------------------------------
GeometrySubsetListPtr Geometry::GetGeometrySubsetList(void)
{
	return m_pGeometrySubsetList;
}
//----------------------------------------------------------------------------------------------
void Geometry::SetGeometrySubsetList(GeometrySubsetListPtr p_pGeometrySubsetList)
{
	m_pGeometrySubsetList = p_pGeometrySubsetList;
}
//----------------------------------------------------------------------------------------------
Geometry& Geometry::operator=(const Geometry& p_geometry)
{
	Renderable::operator =(p_geometry);

	m_pMaterial = p_geometry.m_pMaterial;
	m_pBoundingVolume = p_geometry.m_pBoundingVolume;
	m_pGeometrySubsetList = p_geometry.m_pGeometrySubsetList;

	return *this;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END