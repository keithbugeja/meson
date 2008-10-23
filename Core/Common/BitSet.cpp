//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "BitSet.h"
#include <memory.h>

using namespace Meson::Common::Collections;

BitSet::BitSet( void )
{
	Init( 256 );
}

BitSet::BitSet( dword p_dwCount )
{
	Init( p_dwCount );
}


BitSet::BitSet( const BitSet &p_bitSet )
{
	Init( p_bitSet.BitCount() );
	memcpy( m_pBitArray, p_bitSet.m_pBitArray, p_bitSet.Size() );
}


BitSet::~BitSet( void )
{
	if( m_pBitArray )
		delete m_pBitArray;
}


size_t BitSet::Size( void ) const
{
	return ( (m_dwCount & 0x07) ? 1 : 0 ) + (m_dwCount >> 3);
}

dword BitSet::BitCount( void ) const
{
	return m_dwCount;
}

bool BitSet::IsSet( dword p_dwPosition ) const
{
	if( p_dwPosition >= m_dwCount )
		return false;

	dword dwOffset = ( p_dwPosition & 0x07);
	dword dwPosition = ( p_dwPosition >> 3);
	byte  value = m_pBitArray[ dwPosition ];

	value &= (0x80 >> dwOffset); 

	return value>0;
}

void BitSet::Set( void )
{
	memset( m_pBitArray, 0xFF, Size() );
}

void BitSet::Set( dword p_dwPosition )
{
	if( p_dwPosition >= m_dwCount )
		Realloc( p_dwPosition );

	dword dwOffset = ( p_dwPosition & 0x07);
	dword dwPosition = ( p_dwPosition >> 3);
	m_pBitArray[ dwPosition ] |= (0x80 >> dwOffset); 
}

void BitSet::Set( dword p_dwPosition, bool m_bValue )
{
	if( m_bValue )
		Set( p_dwPosition );
	else
		Clear( p_dwPosition );
}

void BitSet::Set( dword *p_dwPosition, dword p_dwSize )
{
	for( dword dwIndex = 0; dwIndex < p_dwSize; dwIndex++)
	{
		Set( p_dwPosition[dwIndex] );
	}
}

void BitSet::Set( dword *p_dwPosition, dword p_dwSize, bool m_bValue )
{
	for( dword dwIndex = 0; dwIndex < p_dwSize; dwIndex++)
	{
		Set( p_dwPosition[dwIndex], m_bValue );
	}
}

void BitSet::SetRange( dword p_dwStart, dword p_dwEnd )
{
	while(p_dwStart<=p_dwEnd)
	{
		Set( p_dwStart );
		p_dwStart++;
	}
}

void BitSet::SetRange( dword p_dwStart, dword p_dwEnd, bool m_bValue )
{
	while(p_dwStart<=p_dwEnd)
	{
		Set( p_dwStart, m_bValue );
		p_dwStart++;
	}
}

void BitSet::Clear( void )
{
	memset( m_pBitArray, 0x00, Size() );
}

void BitSet::Clear( dword p_dwPosition )
{
	if( p_dwPosition >= m_dwCount )
		Realloc( p_dwPosition );

	dword dwOffset = ( p_dwPosition & 0x07);
	dword dwPosition = ( p_dwPosition >> 3);
	m_pBitArray[ dwPosition ] &= (~(0x80 >> dwOffset)); 
}

void BitSet::Clear( dword *p_dwPosition, dword p_dwSize )
{
	for( dword dwIndex = 0; dwIndex < p_dwSize; dwIndex++)
	{
		Clear( p_dwPosition[dwIndex] );
	}
}

void BitSet::ClearRange( dword p_dwStart, dword p_dwEnd )
{
	while(p_dwStart<=p_dwEnd)
	{
		Clear( p_dwStart );
		p_dwStart++;
	}
}

void BitSet::Toggle( void )
{
	byte *pStart = m_pBitArray;
	byte *pEnd   = m_pBitArray + Size();
	while( pStart < pEnd )
	{
		*pStart ^= 0xFF;
		pStart++;
	}
}

void BitSet::Toggle( dword p_dwPosition )
{
	if( p_dwPosition >= m_dwCount )
		Realloc( p_dwPosition );

	dword dwOffset = ( p_dwPosition & 0x07);
	dword dwPosition = ( p_dwPosition >> 3);
	m_pBitArray[ dwPosition ] ^= (0x80 >> dwOffset); 
}

void BitSet::Toggle( dword *p_dwPosition, dword p_dwSize )
{
	for( dword dwIndex = 0; dwIndex < p_dwSize; dwIndex++)
	{
		Toggle( p_dwPosition[dwIndex] );
	}
}

void BitSet::ToggleRange( dword p_dwStart, dword p_dwEnd )
{
	while(p_dwStart<=p_dwEnd)
	{
		Toggle( p_dwStart );
		p_dwStart++;
	}
}



void BitSet::Put( dword p_dwPosition, byte p_byValue )
{
	Set( p_dwPosition++, (p_byValue & 0x80) > 0 );
	Set( p_dwPosition++, (p_byValue & 0x40) > 0 );
	Set( p_dwPosition++, (p_byValue & 0x20) > 0 );
	Set( p_dwPosition++, (p_byValue & 0x10) > 0 );
	Set( p_dwPosition++, (p_byValue & 0x08) > 0 );
	Set( p_dwPosition++, (p_byValue & 0x04) > 0 );
	Set( p_dwPosition++, (p_byValue & 0x02) > 0 );
	Set( p_dwPosition++, (p_byValue & 0x01) > 0 );
}


void BitSet::Put( dword p_dwPosition, byte *p_pbyValue, size_t p_unSize )
{

	do
	{
		p_unSize--;
		Put( p_dwPosition, p_pbyValue[ p_unSize ] );
		
	} while( p_unSize > 0 );
}



void BitSet::Put( dword p_dwPosition, int p_nValue )
{
	Put( p_dwPosition, (byte*)&p_nValue, sizeof( p_nValue ) );
}

void BitSet::Put( dword p_dwPosition, long p_lValue )
{
	Put( p_dwPosition, (byte*)&p_lValue, sizeof( p_lValue ) );
}

void BitSet::Put( dword p_dwPosition, long long p_llValue )
{
	Put( p_dwPosition, (byte*)&p_llValue, sizeof( p_llValue ) );
}

void BitSet::Put( dword p_dwPosition, float  p_fValue )
{
	Put( p_dwPosition, (byte*)&p_fValue, sizeof( p_fValue ) );
}

void BitSet::Put( dword p_dwPosition, double p_dfValue )
{
	Put( p_dwPosition, (byte*)&p_dfValue, sizeof( p_dfValue ) );
}



byte BitSet::GetByte( dword p_dwPosition )
{
	byte result = 0;

	if( IsSet( p_dwPosition++ ) ) result |= 0x80;
	if( IsSet( p_dwPosition++ ) ) result |= 0x40;
	if( IsSet( p_dwPosition++ ) ) result |= 0x20;
	if( IsSet( p_dwPosition++ ) ) result |= 0x10;
	if( IsSet( p_dwPosition++ ) ) result |= 0x08;
	if( IsSet( p_dwPosition++ ) ) result |= 0x04;
	if( IsSet( p_dwPosition++ ) ) result |= 0x02;
	if( IsSet( p_dwPosition++ ) ) result |= 0x01;

	return result;
}

short BitSet::GetShort( dword p_dwPosition )
{
	short result = 0;

	result = ( GetByte( p_dwPosition ) << 8 ) | GetByte( p_dwPosition );

	return result;
}

int BitSet::GetInt( dword p_dwPosition )
{
	int result = 0;

	result = ( GetByte( p_dwPosition ) << 24 ) | ( GetByte( p_dwPosition ) << 16 ) | 
				( GetByte( p_dwPosition ) << 8 ) | GetByte( p_dwPosition );

	return result;
}

long BitSet::GetLong( dword p_dwPosition )
{
	long result = 0;

	result = ( GetByte( p_dwPosition ) << 24 ) | ( GetByte( p_dwPosition ) << 16 ) | 
				( GetByte( p_dwPosition ) << 8 ) | GetByte( p_dwPosition );

	return result;
}

long long BitSet::GetLongLong( dword p_dwPosition )
{
	long long result = 0;

	result = ( (long long)GetByte( p_dwPosition ) << 56 ) | ( (long long)GetByte( p_dwPosition ) << 48 ) | 
				( (long long)GetByte( p_dwPosition ) << 40 ) | ( (long long)GetByte( p_dwPosition ) << 32 ) | 
				( (long long)GetByte( p_dwPosition ) << 24 ) | ( (long long)GetByte( p_dwPosition ) << 16 ) | 
				( (long long)GetByte( p_dwPosition ) << 8 ) |    (long long)GetByte( p_dwPosition );

	return result;
}

float BitSet::GetFloat( dword p_dwPosition )
{
	long result = 0;

	result = ( GetByte( p_dwPosition ) << 24 ) | ( GetByte( p_dwPosition ) << 16 ) | 
				( GetByte( p_dwPosition ) << 8 ) | GetByte( p_dwPosition );

	return *((float*)&result);
}

double BitSet::GetDouble( dword p_dwPosition )
{
	long long result = 0;

	result = ( (long long)GetByte( p_dwPosition ) << 56 ) | ( (long long)GetByte( p_dwPosition ) << 48 ) | 
			( (long long)GetByte( p_dwPosition ) << 40 ) | ( (long long)GetByte( p_dwPosition ) << 32 ) | 
			( (long long)GetByte( p_dwPosition ) << 24 ) | ( (long long)GetByte( p_dwPosition ) << 16 ) | 
			( (long long)GetByte( p_dwPosition ) << 8 ) |    (long long)GetByte( p_dwPosition );

	return *((double*)&result);
}



void BitSet::Init( dword p_dwCount )
{
	m_dwCount = p_dwCount;
	
	dword dwSize = (p_dwCount & 0x07) ? 1 : 0;
	dwSize += (p_dwCount >> 3);

	m_pBitArray = new byte[ dwSize ];
	memset( m_pBitArray, 0, dwSize );
}



void BitSet::Realloc( dword p_dwCount )
{

	size_t dwNewSize = (p_dwCount >> 3) + (p_dwCount & 0x07) ? 1 : 0;
	if( dwNewSize > Size() )
	{
		byte *pTemp = new byte[ dwNewSize << 1 ];
		memset( pTemp, 0, dwNewSize - Size() );
		memcpy( pTemp, m_pBitArray, Size() );
		delete m_pBitArray;

		m_pBitArray = pTemp;
		m_dwCount = (dword)( dwNewSize << 3 );
	}

}
