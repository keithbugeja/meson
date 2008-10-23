// 14/12/2007

#pragma once

#define CNS_R00  0x00
#define CNS_R01  0x01
#define CNS_R02  0x02
#define CNS_R03  0x03
#define CNS_R04  0x04
#define CNS_R05  0x05
#define CNS_R06  0x06
#define CNS_R07  0x07
#define CNS_R08  0x08
#define CNS_R09  0x09
#define CNS_R10  0x0a
#define CNS_R11  0x0b
#define CNS_R12  0x0c
#define CNS_R13  0x0d
#define CNS_R14  0x0e
#define CNS_R15  0x0f
#define CNS_R16  0x10
#define CNS_R17  0x11
#define CNS_R18  0x12
#define CNS_R19  0x13
#define CNS_R20  0x14
#define CNS_R21  0x15
#define CNS_R22  0x16
#define CNS_R23  0x17
#define CNS_R24  0x18
#define CNS_R25  0x19
#define CNS_R26  0x1a
#define CNS_R27  0x1b
#define CNS_R28  0x1c
#define CNS_R29  0x1d
#define CNS_R30  0x1e
#define CNS_R31  0x1f

#define CNS_PC  0x20
#define CNS_SP  0x21
#define CNS_FP  0x22


// OPCODES
#define CNS_NOP         0x00
#define CNS_LDGC        0x01
#define CNS_LDCC        0x02

#define CNS_LDGD        0x03
#define CNS_LDLD        0x04
#define CNS_LDP         0x05
#define CNS_LDOD        0x06

#define CNS_STGD        0x07
#define CNS_STLD        0x08
#define CNS_STP         0x09
#define CNS_STOD        0x0a

#define CNS_COPY        0x0b


#define CNS_NEG         0x10
#define CNS_ADD         0x11
#define CNS_SUB         0x12
#define CNS_MUL         0x13
#define CNS_DIV         0x14
#define CNS_MOD         0x15
#define CNS_INC         0x16
#define CNS_DEC         0x17
#define CNS_NOT         0x18
#define CNS_AND         0x19
#define CNS_OR          0x1a
#define CNS_XOR         0x1b
#define CNS_SHL         0x1c
#define CNS_SHR         0x1d
#define CNS_USHR        0x1e

#define CNS_XCHG        0x20

#define CNS_PUSH        0x30
#define CNS_PUSHALL     0x31
#define CNS_POP         0x32
#define CNS_POPALL      0x33
#define CNS_PEEK        0x34

#define CNS_CMP         0x35

#define CNS_EQ          0x40
#define CNS_NE          0x41
#define CNS_LT          0x42
#define CNS_GT          0x43
#define CNS_LE          0x44
#define CNS_GE          0x45

#define CNS_HALT        0x50

#define CNS_RET         0x51
#define CNS_RETN        0x52


#define CNS_CALLNATIVE  0x53
#define CNS_CALLOBJECT  0x54
#define CNS_CALLVIRTUAL 0x55

#define CNS_JRT         0x56
#define CNS_JRF         0x57
#define CNS_JR          0x58

#define CNS_CALL        0x5a
#define CNS_POPN        0x5b
#define CNS_CAST        0x5c


#define CNS_NEWOBJECT       0x60
#define CNS_DISPOSEOBJECT   0x61
#define CNS_NEWARRAY        0x62
#define CNS_DISPOSEARRAY    0x63
#define CNS_GETELM          0x64
#define CNS_SETELM          0x65
#define CNS_LENGHT          0x66

#define CNS_LVARS           0x70

#define CNS_STT             0x71
#define CNS_CLT             0x72

//---------------------------------------------------------------------------------------------------------

//#define CNS_NOP       0x00
//#define CNS_SET       0x01
//#define CNS_COPY      0x02
//
//#define CNS_NEG       0x03
//#define CNS_NEGF      0x04
//
//
//#define CNS_ADD       0x05
//#define CNS_ADDF      0x06
//#define CNS_SUB       0x07
//#define CNS_SUBF      0x08
//#define CNS_MUL       0x09
//#define CNS_MULF      0x0A
//#define CNS_DIV       0x0B
//#define CNS_DIVF      0x0C
//#define CNS_MOD       0x0D
//#define CNS_MODF      0x0E
//
//#define CNS_INC       0x0F
//#define CNS_INCF      0x10
//#define CNS_DEC       0x11
//#define CNS_DECF      0x12
//#define CNS_NOT       0x13
//#define CNS_AND       0x14
//#define CNS_OR		  0x15
//#define CNS_XOR       0x16
//#define CNS_SHL       0x17
//#define CNS_SHR       0x18
//#define CNS_SAR       0x19
//#define CNS_ROL       0x1A
//#define CNS_ROR       0x1B
//
//#define CNS_PUSHV     0x1D
//#define CNS_PUSH      0x1E
//#define CNS_PUSHALL   0x1F
//#define CNS_POP       0x20
//#define CNS_POPALL    0x21
//#define CNS_PEEK      0x22
//#define CNS_DUP       0x23
//
//#define CNS_SWAP      0x24
//#define CNS_XCHG      0x25
//
//#define CNS_HALT      0x26
//
//#define CNS_CMP       0x27
//#define CNS_CMPF      0x28
//
//#define CNS_CAST      0x29
//	/*** CAST SUB-FUNCTIONS BEGIN ***/
//	#define CNS_i2b   0x00
//	#define CNS_i2c   0x01
//	#define CNS_i2w   0x02
//	#define CNS_i2s   0x03
//	#define CNS_i2dw  0x04
//	#define CNS_i2q   0x05
//	#define CNS_i2l   0x06
//	#define CNS_i2f   0x07
//	#define CNS_i2d   0x08
//	#define CNS_dw2b  0x09
//	#define CNS_dw2c  0x0a
//	#define CNS_dw2w  0x0b
//	#define CNS_dw2s  0x0c
//	#define CNS_dw2i  0x0d
//	#define CNS_dw2q  0x0e
//	#define CNS_dw2l  0x0f
//	#define CNS_dw2f  0x10
//	#define CNS_dw2d  0x11
//	#define CNS_l2b   0x12
//	#define CNS_l2c   0x13
//	#define CNS_l2w   0x14
//	#define CNS_l2s   0x15
//	#define CNS_l2dw  0x16
//	#define CNS_l2i   0x17
//	#define CNS_l2q   0x18
//	#define CNS_l2f   0x19
//	#define CNS_l2d   0x1a
//	#define CNS_q2b   0x1b
//	#define CNS_q2c   0x1c
//	#define CNS_q2w   0x1d
//	#define CNS_q2s   0x1e
//	#define CNS_q2dw  0x1f
//	#define CNS_q2i   0x20
//	#define CNS_q2l   0x21
//	#define CNS_q2f   0x22
//	#define CNS_q2d   0x23
//	#define CNS_f2b   0x24
//	#define CNS_f2c   0x25
//	#define CNS_f2w   0x26
//	#define CNS_f2s   0x27
//	#define CNS_f2dw  0x28
//	#define CNS_f2i   0x29
//	#define CNS_f2q   0x2a
//	#define CNS_f2l   0x2b
//	#define CNS_f2d   0x2c
//	#define CNS_d2b   0x2d
//	#define CNS_d2c   0x2e
//	#define CNS_d2w   0x2f
//	#define CNS_d2s   0x30
//	#define CNS_d2dw  0x31
//	#define CNS_d2i   0x32
//	#define CNS_d2q   0x33
//	#define CNS_d2l   0x34
//	#define CNS_d2f   0x35
//	/*** CAST SUB-FUNCTIONS END ***/
//						 
//#define CNS_BIT_SET   0x30
//#define CNS_BIT_RES   0x31
//#define CNS_BIT_TGL   0x32
//#define CNS_BIT_TST   0x33
//
//#define CNS_JMP       0x34
//#define CNS_JRE       0x35
//#define CNS_CALL      0x36
//#define CNS_JMPC      0x37
//#define CNS_JREC      0x38
//#define CNS_CALLC     0x39
//
//#define CNS_RET       0x3a
//#define CNS_RETC      0x3b
//
//#define CNS_GET       0x3c
//#define CNS_PUT       0x3d
//#define CNS_GETREF    0x3e
//#define CNS_PUTREF    0x3f
//						 
////#define CNS_LOAD      0x02
////#define CNS_STORE     0x03

#define CNS_XTND      0x7F
