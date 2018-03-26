#ifndef __GLOBAL_DEFINE_H__
#define __GLOBAL_DEFINE_H__

#ifdef __PARADIGM__
#pragma pack(push)
#pragma pack(1)
#endif //__PARADIGM__

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

//
// Common data type cast
//
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;
typedef signed char SBYTE;
typedef signed int SWORD;
typedef signed long SDWORD;

#define BYTE_SIZE	sizeof(BYTE)
#define WORD_SIZE	sizeof(WORD)

void far gm_Print(const far char *Bp_String, ...);


//
// Define variable with const far will place the variable in ROM area.
//
#define ROM 	const far
#define code 	const far 	//data in code segments
#define stack 	_ss 		//local variable assumed to be in stack
#define ndata 	near 		//global variable in 0 segment
#define fdata 	far 		//universal common pointer for data segent(s)
#define hostreg near 		//host register pointer
#define pcbreg 	near 		//intel core register pointer

//
// Data structure for WORD union
//
typedef struct			// Structure for WORD
{
   BYTE B_Lower;		// Intel 186 memory specific
   BYTE B_Upper;		//
} ST_WORD;

typedef union			// Union for WORD with upper and lower byte
{
   WORD    W_Data;
   ST_WORD	St_Data;
} UNION_WORD;

typedef int BOOL;
#define FALSE	0
#define TRUE	1
#define gmd_FALSE FALSE
#define gmd_TRUE TRUE

/*
typedef enum bool_enum
{
	FALSE,
    gmd_FALSE = FALSE,
	TRUE,
	gmd_TRUE = TRUE
} BOOL;*/

#define Off			FALSE
#define On			TRUE

#define DISABLE		0
#define ENABLE		1

//
// Common default value
//
#define NULL_PTR	0

//
// General bit definition
//
#define BIT0        0x0001
#define BIT1        0x0002
#define BIT2        0x0004
#define BIT3        0x0008
#define BIT4        0x0010
#define BIT5        0x0020
#define BIT6        0x0040
#define BIT7        0x0080
#define BIT8        0x0100
#define BIT9        0x0200
#define BIT10       0x0400
#define BIT11       0x0800
#define BIT12       0x1000
#define BIT13       0x2000
#define BIT14       0x4000
#define BIT15       0x8000

typedef BYTE uint8_t;
typedef SBYTE int8_t;
typedef WORD uint16_t;
typedef DWORD uint32_t;
typedef SWORD int16_t;
typedef SDWORD int32_t;
typedef SDWORD int64_t;
typedef BOOL Boolean;
typedef BOOL Bool;


#endif //__GLOBAL_DEFINE_H__
