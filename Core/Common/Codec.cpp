//----------------------------------------------------------------------------------------------
//	Meson::Common::Codec
//	Helper class that performs de/encoding of data.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	02/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "Codec.h"

Meson_Common_BEGIN

Codec::Codec(void)
{
}

Codec::~Codec(void)
{
}

//dword Codec::CRC32( byte *p_pData, dword p_dwSize )
//{
//	dword L = p_dwSize & 0x03;
//	dword R = p_dwSize;
//
//	if( L )
//	{
//		p_dwSize -= L;
//		L = R + (*((dword*)(p_pData + p_dwSize)) >> (L << 3));
//		R = ~L + (p_dwSize - L);
//	}
//	else
//	{
//		L = ~(R<<3);
//	}
//
//	p_dwSize >>= 2;
//	byte *pA = p_pData;
//	byte *pB = p_pData + p_dwSize;
//	byte *pC = pB + p_dwSize;
//	byte *pD = pC + p_dwSize;
//
//	while( p_dwSize )
//	{
//		L += *pA;
//		R += *pB;
//		L ^= (*pC << (R&31));
//		R ^= (*pD << (L&17));
//		
//		p_dwSize--;
//	}
//
//	return (L << 16) ^ R;
//}

//00010011001101100110110011001000
#define QL 0x13366CC8
//11101100110010011001001100110111
#define QR 0xECC99337

//dword Codec::CRC32( byte *p_pData, dword p_dwSize )
//{
//	dword L = p_dwSize & 0x03;
//	dword R = p_dwSize;
//	p_dwSize -= L;
//
//	byte *pA = p_pData;
//	byte *pD = p_pData + p_dwSize;
//
//	if( L )
//	{
//		L = R + (*((dword*)(p_pData + p_dwSize)) >> (L << 3));
//		R = p_dwSize;
//	}
//	else
//	{
//		L = QL; 
//		R = QR;
//	}
//
//	p_dwSize >>= 2;
//	byte *pB = p_pData + p_dwSize;
//	byte *pC = pB - 1;
//
//	dword T;
//
//	while( pA > pB )
//	{
//		T = (dword)(pB - pA);
//
//		L += (*pA + *pC) ^ T;
//		R += (*pB + *pD) ^ ~T;
//
//		L = L & 15;
//		R = R & 15;
//
//		/*L ^= QL;
//		R ^= QR;
//		
//		if( L & 0x8000 || R & 0x8000)
//		{
//			T = R; R = L; L = T;
//		}*/
//		
//		pA++;
//		pB--;
//		pC++;
//		pD--;
//	}
//
//	return (L & 0xFFFF0000 ) | R & 0xFFFF;
//}



union CRC 
{
	struct 
	{
		word R, L;
	};
	dword Q;
};

dword Codec::CRC32( byte *p_pData, dword p_dwSize )
{
	CRC crc;
	crc.L = 0x0100;
	crc.R = 0x0101;

	byte *pA = p_pData;
	byte *pD = p_pData + p_dwSize;

	if( p_dwSize & 1 )
	{
		crc.L += *pD;
		p_dwSize--;

	}

	byte *pB = p_pData + (p_dwSize>>1);
	byte *pC = pB - 1;

	while( pA > pB )
	{
		crc.L += ((*pA << 8) | *pC);
		crc.R += ((*pD << 8) | *pB);

		pA++;
		pB--;
	}

	return crc.Q;
}


Meson_Common_END
