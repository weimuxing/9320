/*
	$Workfile:   gm_ColorControl.h  $
	$Revision: 1.6 $
	$Date: 2011/12/14 03:44:48 $
*/

#ifndef __GM_COLORCONTROL_H__
#define __GM_COLORCONTROL_H__
//**************************************************************************
// COPYRIGHT (C) STMicroelectronics 2011.
//																			
// All rights reserved. This document contains proprietary and
// confidential information of the STMicroelectronics Group. This
// document is not to be copied in whole or part. STMicroelectronics
// assumes no responsibility for the consequences of use of such
// information nor for any infringement of patents or other rights of
// third parties which may result from its use. No license is granted by
// implication or otherwise under any patent or patent rights of
// STMicroelectronics.
// STMicroelectronics
// products are not authorized for use as critical components in life
// support devices or systems without express written approval of
// STMicroelectronics.
//==========================================================================
//																			
// MODULE:      gm_ColorControl.h													
//																			
// USAGE:       Header file for color space control routines				
//																			
//**************************************************************************

/****************************************************************************/
/*	L O C A L   M A C R O   D E F I N I T I O N S							*/
/****************************************************************************/

#define	TRUNC(X,MIN,MAX)	if (X > MAX) X = MAX; else if (X < MIN) X = MIN;

//
// The following shift operations maintain the sign of signed variables
//
#define LSHFT(X,Y)			    X <<= Y;
#define RSHFT(X,Y)			    X >>= Y;

// Start of MAIN CSC block
#define MAIN_CSC_BLOCK_START		MAIN_P_MULT_COEF11
// Start of PIP CSC block		
#define PIP_CSC_BLOCK_START			PIP_MULT_COEF11

/****************************************************************************/
/*	G L O B A L    D E F I N I T I O N S									*/
/****************************************************************************/

// Offsets used to access registers within
// MAIN and PIP CSC blocks	   		MAIN    		PIP		 
//-------------------------------------------------------------------------

#if 1
#define COEF_11			0x0000		// Reg 0x85AE 		Reg 0x85E0
#define COEF_12	 		0x0004	
#define COEF_13	 		0x0008	
#define COEF_21	 		0x000C	
#define COEF_22 		0x0010	
#define COEF_23 		0x0014	
#define COEF_31	 		0x0018	
#define COEF_32 		0x001C	
#define COEF_33 		0x0020	
#define OFFSET1	 		0x0024	
#define OFFSET2 		0x0028	
#define OFFSET3 		0x002C
#else
#define COEF_11			0x0000		// Reg 0x85AE 		Reg 0x85E0
#define COEF_12	 		0x0002	
#define COEF_13	 		0x0004	
#define COEF_21	 		0x0006	
#define COEF_22 		0x0008	
#define COEF_23 		0x000A	
#define COEF_31	 		0x000C	
#define COEF_32 		0x000E	
#define COEF_33 		0x0010	
#define OFFSET1	 		0x0012	
#define OFFSET2 		0x0014	
#define OFFSET3 		0x0016
#endif

// TODO: ADD coefficients and offsets for SET B

/****************************************************************************/
/*	G L O B A L    V A R I A B L E S										*/
/****************************************************************************/

/****************************************************************************/
/*	G L O B A L    F U N C T I O N    P R O T O T Y P E S					*/
/****************************************************************************/

#endif
