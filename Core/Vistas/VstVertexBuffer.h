//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstHardwareResource.h"
#include "VstVertexDeclaration.h"

Meson_Vistas_BEGIN
	
	//----------------------------------------------------------------------------------------------
	class VISTAS_API VertexBuffer : public HardwareResource
	{
		VISTAS_IMPLEMENT_RTTI(VertexBuffer, HardwareResource);

    public:
		VertexBuffer(uint p_uiVertexSize, uint p_uiVertexCount, dword p_dwPool, dword p_dwUsage);
		
		//----------------------------------------------------------------------------------------------
		/** Locks the buffer in whole or in part using the specified locking options.

			\param p_uiStart		Starting position of buffer portion that is to be locked
			\param p_uiSize			Lock size
			\param p_dwLockOptions	Buffer locking flags
		 */
		//----------------------------------------------------------------------------------------------
		virtual void Lock(uint p_uiStart, uint p_uiSize, dword p_dwLockOptions) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Locks the whole buffer using the specified locking options. This method must be filled in
			by all HardwareResource implementations.

			\param p_dwLockOptions	Buffer locking flags
		 */
		//----------------------------------------------------------------------------------------------
		virtual void Lock(dword p_dwLockOptions) = 0;

		//----------------------------------------------------------------------------------------------
		/** Returns the size of each vertex element within the buffer.

			\return Vertex size in bytes
		 */ 
		//----------------------------------------------------------------------------------------------
		uint GetVertexSize(void) const;
        
		//----------------------------------------------------------------------------------------------
		/** Returns the number of vertices in the buffer

			\return Vertex count
		 */ 
		//----------------------------------------------------------------------------------------------
		uint GetVertexCount(void) const;
	
	protected:
		uint m_uiVertexCount,
			 m_uiVertexSize;
	};

	//----------------------------------------------------------------------------------------------
	typedef Meson::Common::TPointer<VertexBuffer> VertexBufferPtr;
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END