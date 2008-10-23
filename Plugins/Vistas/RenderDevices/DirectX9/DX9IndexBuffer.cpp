//----------------------------------------------------------------------------------------------
//	Meson::Vistas::DX9IndexBuffer
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstVistasEngine.h"
#include "VstHardwareResourceManager.h"
#include "DX9IndexBuffer.h"
#include "MesonException.h"

using namespace Meson::Common;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
DX9IndexBuffer::DX9IndexBuffer(LPDIRECT3DDEVICE9 p_pD3DDevice9, IndexType p_eIndexType, uint p_uiIndexCount, dword p_dwPool, dword p_dwUsage) 
	: IndexBuffer( p_eIndexType, p_uiIndexCount, p_dwPool, p_dwUsage ), 
	  m_pD3DDevice9(p_pD3DDevice9)
{
	D3DPOOL d3dPool;
	D3DFORMAT d3dFormat;
	DWORD dwUsage = 0;

	m_bIsLocked = false;

	// TODO: Extend usage
	if (p_dwUsage == HardwareResource::MU_DEFAULT)
		dwUsage = NULL;
	
	// TODO: Extend memory pool
	switch( p_dwPool )
	{
		case HardwareResource::MP_DEFAULT:
		default:
			d3dPool = D3DPOOL_MANAGED;
	}

	switch( p_eIndexType )
	{
		case IT_16BIT:
			d3dFormat = D3DFMT_INDEX16;
			break;

		case IT_32BIT:
		default:
			d3dFormat = D3DFMT_INDEX32;
			break;
	}

	if (D3D_OK != p_pD3DDevice9->CreateIndexBuffer(p_uiIndexCount * (uint)m_eIndexType, dwUsage, d3dFormat, d3dPool, &m_pD3DIndexBuffer9, NULL))
		throw new MesonException("Unable to create index buffer.", __FILE__, __LINE__ );

	// Create Resource ID
	m_resourceHandle = (void*)m_pD3DIndexBuffer9;
}
//----------------------------------------------------------------------------------------------
DX9IndexBuffer::~DX9IndexBuffer(void)
{
	Unlock();

	SAFE_RELEASE(m_pD3DIndexBuffer9);
}
//----------------------------------------------------------------------------------------------
void DX9IndexBuffer::Unlock(void) 
{
	if (m_bIsLocked)
	{
		m_pD3DIndexBuffer9->Unlock();
		m_bIsLocked = false;
	}
}
//----------------------------------------------------------------------------------------------
void DX9IndexBuffer::Lock(uint p_uiStart, uint p_uiSize, dword p_dwOptions)
{
	if (!m_bIsLocked)
	{
		if (D3D_OK != m_pD3DIndexBuffer9->Lock(p_uiStart, p_uiSize, (void**)&m_pbyData, p_dwOptions))
			throw new MesonException("Unable to lock index buffer.", __FILE__, __LINE__ );

		m_bIsLocked = true;
	}
}
//----------------------------------------------------------------------------------------------
void DX9IndexBuffer::Lock(dword p_dwOptions) 
{
	Lock(0, m_uiIndexCount * (uint)m_eIndexType, p_dwOptions);
}
//----------------------------------------------------------------------------------------------
void DX9IndexBuffer::Read(const uint p_nSourceOffset, const uint p_nDestinationOffset, uint p_nSize, byte* p_pbyBuffer)
{
	memcpy(p_pbyBuffer + p_nDestinationOffset, m_pbyData + p_nSourceOffset, p_nSize); 
}
//----------------------------------------------------------------------------------------------
void DX9IndexBuffer::Write(const uint p_nSourceOffset, const uint p_nDestinationOffset, uint p_nSize, const byte* p_pbyBuffer)
{
	memcpy(m_pbyData + p_nDestinationOffset, p_pbyBuffer + p_nSourceOffset, p_nSize); 
}

Meson_Vistas_END
