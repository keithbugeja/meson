//----------------------------------------------------------------------------------------------
//	Meson::Vistas::IIndexBuffer
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstHardwareResource.h"

Meson_Vistas_BEGIN
	
	//----------------------------------------------------------------------------------------------
	class IndexBuffer : public HardwareResource
	{
		VISTAS_IMPLEMENT_RTTI(IndexBuffer, HardwareResource);

    public:
		/** Enumeration specifying size of each index
			element within buffer.
		 */
		enum IndexType
		{
			IT_16BIT = 2,
			IT_32BIT = 4
		};

		IndexBuffer(IndexType p_eIndexType, uint p_uiIndexCount, dword p_dwPool, dword p_dwUsage);
		
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
		/** Returns the buffer index type.

			\return Buffer index type
		 */ 
		//----------------------------------------------------------------------------------------------
		IndexType GetIndexType(void) const;
        
		//----------------------------------------------------------------------------------------------
		/** Returns the buffer index count.

			\return Buffer index count
		 */
		//----------------------------------------------------------------------------------------------
		uint GetIndexCount(void) const;
	
	protected:
		IndexType m_eIndexType;
		uint m_uiIndexCount;
	};

	//----------------------------------------------------------------------------------------------
	typedef Meson::Common::TPointer<IndexBuffer> IndexBufferPtr;
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END