//----------------------------------------------------------------------------------------------
//	VstHardwareResource.h
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstResource.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	//  \class HardwareResource
	//  Base class for all hardware resource entities in the Vistas framework. 
	//----------------------------------------------------------------------------------------------	
	class VISTAS_API HardwareResource : public Resource
	{
		VISTAS_IMPLEMENT_RTTI(HardwareResource, Resource);

	public:
		//----------------------------------------------------------------------------------------------	
		// Memory pooling options.
		//----------------------------------------------------------------------------------------------	
		enum MemoryPool
		{
			MP_DEFAULT			= 0x01
		};
		
		//----------------------------------------------------------------------------------------------	
		// Memory usage options.
		//----------------------------------------------------------------------------------------------	
		enum MemoryUsage
		{
			MU_DEFAULT			= 0x01
		};

		//----------------------------------------------------------------------------------------------	
		// Memory locking options.
		//----------------------------------------------------------------------------------------------	
		enum MemoryLock
		{
			ML_NONE			= 0x00,
			ML_DISCARD		= 0x01,
			ML_READ_ONLY	= 0x02,
			ML_NO_OVERWRITE	= 0x04
		};

	public:
		//----------------------------------------------------------------------------------------------
		/** IHardwareResource constructor.
			
			\param p_dwPool	Memory pool flags
			\param p_dwUsage Memory usage flags
		 */
		//----------------------------------------------------------------------------------------------
		HardwareResource(dword p_dwPool, dword p_dwUsage);
		HardwareResource(const Meson::Common::Text::String& p_strId, dword p_dwPool, dword p_dwUsage);

		//----------------------------------------------------------------------------------------------
		/** IHardwareResource virtual destructor.
		 */
		//----------------------------------------------------------------------------------------------
		virtual ~HardwareResource(void);

		//----------------------------------------------------------------------------------------------
		/** Returns the locking status of the buffer. 

			\return <b>true</b> if the buffer is locked, or <b>false</b> otherwise
		 */
		//----------------------------------------------------------------------------------------------
		bool IsLocked(void);

		//----------------------------------------------------------------------------------------------
		/** Returns the memory pooling flags.

			\return Memory pooling flags
		 */
		//----------------------------------------------------------------------------------------------
		dword GetMemoryPool(void);

		//----------------------------------------------------------------------------------------------
		/** Returns the memory usage flags.

			\return Memory usage flags
		 */
		//----------------------------------------------------------------------------------------------
		dword GetMemoryUsage(void);

		//----------------------------------------------------------------------------------------------
		/** Unlocks the buffer if it is locked. Note that this method must be implemented by the 
			respecitve sub-classes.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void Unlock(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Locks the whole buffer using the specified locking options. This method must be filled in
			by all HardwareResource implementations.

			\param p_dwLockOptions	Buffer locking flags
		 */
		//----------------------------------------------------------------------------------------------
		virtual void Lock(dword p_dwLockOptions) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Reads data from a buffer into a user supplied buffer. Note that in order for this operation
			to be carried out, the buffer must be locked.

			\param p_uiSourceOffset			Position at which to start reading from hardware resource 
											buffer
			\param p_uiDestinationOffset	Position at which to start writing in destination buffer
			\param p_uiSize					Size of data to copy
			\param p_pbyBuffer				Pointer to destination buffer
		 */
		//----------------------------------------------------------------------------------------------
		virtual void Read(const uint p_uiSourceOffset, const uint p_uiDestinationOffset, uint p_uiSize, byte* p_pbyBuffer) = 0;

		//----------------------------------------------------------------------------------------------
		/** Writes data to a hardware resource buffer. Note that in order for this operation to be 
			carried out, the buffer must be locked.

			\param p_uiSourceOffset			Position within source buffer to start copying from
			\param p_uiDestinationOffset	Position in destination buffer at which to start writing
			\param p_uiSize					Size of data to copy
			\param p_pbyBuffer				Pointer to source data buffer
		 */
		//----------------------------------------------------------------------------------------------
		virtual void Write(const uint p_uiSourceOffset, const uint p_uiDestinationOffset, uint p_uiSize, const byte* p_pbyBuffer) = 0;

	protected:
		dword m_dwMemoryPool,
			  m_dwMemoryUsage;

		bool m_bIsLocked;
	};

Meson_Vistas_END