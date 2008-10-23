//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstVistasEngine.h"

#include "ArrayList.h"
#include "MesonException.h"
#include "DX9VertexDeclaration.h"
#include "DX9Definitions.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

/** Vertex data type map for direct 3d
 */
byte DX9VertexDeclaration::m_byVertexDataType[VDT_COUNT] = 
{
		D3DDECLTYPE_FLOAT1,		// VDT_FLOAT1,
		D3DDECLTYPE_FLOAT2,		// VDT_FLOAT2,
		D3DDECLTYPE_FLOAT3,		// VDT_FLOAT3,
		D3DDECLTYPE_FLOAT4,		// VDT_FLOAT4,

		D3DDECLTYPE_FLOAT16_2,	// VDT_FLOAT16_2,
		D3DDECLTYPE_FLOAT16_4,	// VDT_FLOAT16_4,

		D3DDECLTYPE_UBYTE4,		// VDT_UBYTE4,
		D3DDECLTYPE_UBYTE4N,	// VDT_UBYTE4N,

		D3DDECLTYPE_SHORT2,		// VDT_SHORT2,
		D3DDECLTYPE_SHORT4,		// VDT_SHORT4,

		D3DDECLTYPE_SHORT2N,	// VDT_SHORT2N,
		D3DDECLTYPE_SHORT4N,	// VDT_SHORT4N,

		D3DDECLTYPE_USHORT2N,	// VDT_USHORT2N,
		D3DDECLTYPE_USHORT4N,	// VDT_USHORT4N,

		D3DDECLTYPE_FLOAT4		// VDT_COLOUR,
};

/** Vertex data function map for direct 3d
 */
byte DX9VertexDeclaration::m_byVertexDataFunction[VDF_COUNT] = 
{
	D3DDECLUSAGE_POSITION,		// VDF_POSITION
	D3DDECLUSAGE_BLENDWEIGHT,	// VDF_BLEND_WEIGHT
	D3DDECLUSAGE_BLENDINDICES,	// VDF_BLEND_INDICES
	D3DDECLUSAGE_NORMAL,		// VDF_NORMAL
	D3DDECLUSAGE_TANGENT,		// VDF_TANGENT
	D3DDECLUSAGE_BINORMAL,		// VDF_BINORMAL
	D3DDECLUSAGE_COLOR,			// VDF_SPECULAR
	D3DDECLUSAGE_COLOR,			// VDF_DIFFUSE
	D3DDECLUSAGE_COLOR,			// VDF_COLOUR
	D3DDECLUSAGE_TEXCOORD		// VDF_TEXTURE_COORDINATES
};

//----------------------------------------------------------------------------------------------
/// Vertex declaration constructor.
/// \param p_pD3DDevice Direct3D9 device object
//----------------------------------------------------------------------------------------------
DX9VertexDeclaration::DX9VertexDeclaration(LPDIRECT3DDEVICE9 p_pD3DDevice) 
	: m_pD3DDevice(p_pD3DDevice), m_pD3DVertexElement(NULL), m_pD3DVertexDeclaration(NULL), m_bIsStale(true)
{
}

//----------------------------------------------------------------------------------------------
/// Vertex declaration destructor
//----------------------------------------------------------------------------------------------
DX9VertexDeclaration::~DX9VertexDeclaration(void)
{
	// Release vertex declaration
	SAFE_RELEASE(m_pD3DVertexDeclaration);

	// Free vertex element array
	SAFE_DELETE_ARRAY(m_pD3DVertexElement);
}

//----------------------------------------------------------------------------------------------
/// Adds a vertex element to vertex declaration.
/// \param p_uiStream Stream number through which a vertex element is bound to a vertex buffer
///	\param p_uiOffset Position of element within vertex declaration; byte offset.
/// \param p_eVertexDataType The type of the specified element
/// \param p_eVertexDataFunction The semantic function of the specified element
/// \param p_eUsageIndex The index for multiple semantically-identical elements
//----------------------------------------------------------------------------------------------
void DX9VertexDeclaration::AddVertexElement(uint p_uiStream, uint p_uiOffset, VertexDataType p_eVertexDataType, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex)
{
	VertexDeclaration::AddVertexElement(p_uiStream, p_uiOffset, p_eVertexDataType, p_eVertexDataFunction, p_uiUsageIndex);
	m_bIsStale = true;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void DX9VertexDeclaration::AppendVertexElement(uint p_uiStream, VertexDataType p_eVertexDataType, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex)
{
	VertexDeclaration::AppendVertexElement(p_uiStream, p_eVertexDataType, p_eVertexDataFunction, p_uiUsageIndex);
	m_bIsStale = true;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void DX9VertexDeclaration::RemoveVertexElement(uint p_uiStream, uint p_uiOffset)
{
	VertexDeclaration::RemoveVertexElement(p_uiStream, p_uiOffset);
	m_bIsStale = true;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void DX9VertexDeclaration::RemoveAllVertexElements(void)
{
	VertexDeclaration::RemoveAllVertexElements();
	m_bIsStale = true;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXDECLARATION9 DX9VertexDeclaration::GetVertexDeclaration(bool p_bRegenerate)
{
	if (m_bIsStale || p_bRegenerate)
	{
		Refresh();
		m_bIsStale = false;
	}

	return m_pD3DVertexDeclaration;
}

//----------------------------------------------------------------------------------------------
/// Refreshes the vertex declaration.
//----------------------------------------------------------------------------------------------
void DX9VertexDeclaration::Refresh(void)
{
	//----------------------------------------------------------------------------------------------	
	// Regenerate vertex declaration
	//----------------------------------------------------------------------------------------------
	VertexElement* pVertexElement = NULL;

	// Release buffer if already in use
	SAFE_DELETE_ARRAY(m_pD3DVertexElement);
	
	// Allocate memory for new buffer
	m_pD3DVertexElement = (LPD3DVERTEXELEMENT9) new byte[sizeof(D3DVERTEXELEMENT9) * (m_vertexElementList.Size() + 1)];
	
	// Parse vertex elements and construct a new D3D9 vertex declaration
	LPD3DVERTEXELEMENT9 pD3DVertexElement = m_pD3DVertexElement;

	// Map vertex elements to d3d vertex elements
	for (TEnumerator<VertexElement*>& vertexElementEnumerator = m_vertexElementList.GetEnumerator(); vertexElementEnumerator.HasMoreElements(); pD3DVertexElement++)
	{
		pVertexElement = vertexElementEnumerator.NextElement();

		pD3DVertexElement->Stream = pVertexElement->GetStream();
		pD3DVertexElement->Method = D3DDECLMETHOD_DEFAULT;
		pD3DVertexElement->Offset = pVertexElement->GetOffset();

		switch ( pVertexElement->GetVertexDataFunction() )
		{
			case VDF_DIFFUSE:
				pD3DVertexElement->Type = D3DDECLTYPE_FLOAT4;
				pD3DVertexElement->Usage = D3DDECLUSAGE_COLOR;
				pD3DVertexElement->UsageIndex = 0;
				break;

			case VDF_SPECULAR:
				pD3DVertexElement->Type = D3DDECLTYPE_FLOAT4;
				pD3DVertexElement->Usage = D3DDECLUSAGE_COLOR;
				pD3DVertexElement->UsageIndex = 1;
				break;

			default:
				pD3DVertexElement->Type = m_byVertexDataType[pVertexElement->GetVertexDataType()];
				pD3DVertexElement->Usage = m_byVertexDataFunction[pVertexElement->GetVertexDataFunction()];
				pD3DVertexElement->UsageIndex = pVertexElement->GetUsageIndex();
		} 
	}

	// Add end-marker element
	D3DVERTEXELEMENT9 d3dVertexElementEndTag = D3DDECL_END();
	memcpy(pD3DVertexElement, &d3dVertexElementEndTag, sizeof(D3DVERTEXELEMENT9));

	// Create actual d3d vertex declaration
	SAFE_RELEASE(m_pD3DVertexDeclaration);

	if (D3D_OK != m_pD3DDevice->CreateVertexDeclaration(m_pD3DVertexElement, &m_pD3DVertexDeclaration))
		throw new MesonException("Unable to create vertex declaration.", __FILE__, __LINE__ );
}

Meson_Vistas_END