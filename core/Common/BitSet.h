#pragma once

//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "Namespaces.h"
#include "Types.h"

Meson_Common_Collections_BEGIN

class BitSet
{
private:
	byte  *m_pBitArray;
	dword  m_dwCount;

public:
	BitSet( void );
	explicit BitSet( dword p_dwCount );
	BitSet( const BitSet &p_bitSet );
	~BitSet( void );

	size_t Size( void ) const;
	dword  BitCount( void ) const;

	bool IsSet( dword p_dwPosition ) const;

	void Set( void );
	void Set( dword p_dwPosition );
	void Set( dword p_dwSize, bool m_bValue );
	void Set( dword *p_dwPosition, dword p_dwSize );
	void Set( dword *p_dwPosition, dword p_dwSize, bool m_bValue );
	void SetRange( dword p_dwStart, dword p_dwEnd );
	void SetRange( dword p_dwStart, dword p_dwEnd, bool m_bValue );

	void Clear( void );
	void Clear( dword p_dwSize );
	void Clear( dword *p_dwPosition, dword p_dwSize );
	void ClearRange( dword p_dwStart, dword p_dwEnd );

	void Toggle( void );
	void Toggle( dword p_dwPosition );
	void Toggle( dword *p_dwPosition, dword p_dwSize );
	void ToggleRange( dword p_dwStart, dword p_dwEnd );

	void Put( dword p_dwPosition, byte p_byValue );
	void Put( dword p_dwPosition, byte *p_pbyValue, size_t p_unSize );
	

	void Put( dword p_dwPosition, int p_nValue );
	void Put( dword p_dwPosition, long p_lValue );
	void Put( dword p_dwPosition, long long p_llValue );

	void Put( dword p_dwPosition, float  p_fValue );
	void Put( dword p_dwPosition, double p_dfValue );


	byte      GetByte    ( dword p_dwPosition );
	short     GetShort   ( dword p_dwPosition );
	int       GetInt     ( dword p_dwPosition );
	long      GetLong    ( dword p_dwPosition );
	long long GetLongLong( dword p_dwPosition );
	float     GetFloat   ( dword p_dwPosition );
	double    GetDouble  ( dword p_dwPosition );

private:
	void Init( dword p_dwCount );
	void Realloc( dword p_dwCount );
};

Meson_Common_Collections_END
