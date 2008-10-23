#pragma once


/*** Convert Binary to Decimal (at compile time) ****************************/

// Individual Bit test
#define _BIT1(X) (    X        & 1 )
#define _BIT2(X) ( ( (X >>  3) & 1 ) * 0x02 )
#define _BIT3(X) ( ( (X >>  6) & 1 ) * 0x04 )
#define _BIT4(X) ( ( (X >>  9) & 1 ) * 0x08 )
#define _BIT5(X) ( ( (X >> 12) & 1 ) * 0x10 )
#define _BIT6(X) ( ( (X >> 15) & 1 ) * 0x20 )
#define _BIT7(X) ( ( (X >> 18) & 1 ) * 0x40 )
#define _BIT8(X) ( ( (X >> 21) & 1 ) * 0x80 )

// Byte size
// Uses token pasting to create a leading 0, transforming the number into octal
#define BIN2BYTE(X) ( _BIT8(0##X) | _BIT7(0##X) | _BIT6(0##X) | _BIT5(0##X) | \
						_BIT4(0##X) | _BIT3(0##X) | _BIT2(0##X) | _BIT1(0##X) )

// Macros converting from binary to decimal 
#define BIN2DEC8( A ) BIN2BYTE(A)

#define BIN2DEC16( A, B ) ( ( BIN2BYTE(A) << 8 ) | BIN2BYTE(B) )

#define BIN2DEC32( A, B, C, D ) ( ( BIN2BYTE(A) << 24 ) | ( BIN2BYTE(B) << 16 ) | \
								( BIN2BYTE(C) << 8 ) | BIN2BYTE(D) )

#define BIN2DEC64( A, B, C, D, E, F, G, H ) ( ( BIN2BYTE(A) << 56 ) | ( BIN2BYTE(B) << 48 ) | \
												( BIN2BYTE(C) << 40 ) | ( BIN2BYTE(D) << 32 ) | \
												( BIN2BYTE(E) << 24 ) | ( BIN2BYTE(F) << 16 ) | \
												( BIN2BYTE(G) << 8 ) | BIN2BYTE(H) )

/****************************************************************************/
#define MAX_INT8 0x7F;
#define MIN_INT8 0x80;

#define MAX_INT16 0x7FFF;
#define MIN_INT16 0x8000;

#define MAX_INT32 0x7FFFFFFF;
#define MIN_INT32 0x80000000;

/****************************************************************************/
#define MIN(A, B)  (((A) < (B)) ? (A) : (B))
#define MAX(A, B)  (((A) > (B)) ? (A) : (B))
/****************************************************************************/
#ifndef NULL
	#define NULL 0
#endif 

#ifndef null
	#define null 0
#endif
/****************************************************************************/