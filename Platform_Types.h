/*CPU_TYPE*/
#ifndef PLATFORM_TYPES_H_
#define PLATFORM_TYPES_H_

#define CPU_TYPE_8 8
#define CPU_TYPE_16 16
#define CPU_TYPE_32 32
#define CPU_TYPE_64 64

				/************************************************/	
/*Endianness*/
#define MSB_FIRST 0
#define LSB_FIRST 1
#define HIGH_BYTE_FIRST 0
#define LOW_BYTE_FIRST 1
				/************************************************/	
/*For Tiva C*/
#define CPU_TYPE CPU_TYPE_32
#define CPU_BIT_ORDER LSB_FIRST
#define CPU_BYTE_ORDER LOW_BYTE_FIRST

typedef unsigned char boolean;
#define FALSE  0x00u
#define TRUE   0x01u

/*unsigned*/

typedef unsigned char 			uint8;				/* 0 =>255*/
typedef unsigned short int 	    uint16;				/*0=>65536*/
typedef unsigned long int       uint32;				/*0=>4294967296*/
typedef long long unsigned int 	uint64;  			/*0=>18446744073709551616*/

				/************************************************/	

/*signed*/

typedef signed char 		    sint8;				/* -128 => +127*/
typedef signed short int        sint16;				/* -32768 => +32767*/
typedef signed long int         sint32;				/* -2147483648 => +2147483647*/
typedef long long int 			sint64;				/* -9223372036854775808 => +9223372036854775807*/

				/************************************************/	

/*Float*/

typedef  float 					float32;
typedef double 					float64;
typedef long double 			float128;  

				/************************************************/	
#endif
