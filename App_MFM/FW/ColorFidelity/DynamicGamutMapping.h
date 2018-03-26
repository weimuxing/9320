#ifndef __DYNAMIC_GAMUT_MAPPING_H__
#define __DYNAMIC_GAMUT_MAPPING_H__

#ifdef __PARADIGM__
#pragma pack(push)
#pragma pack(1)
#endif //__PARADIGM__

typedef struct _ChipLUTInfoStruct_
{
   BYTE 		inBits;
   BYTE		outBits;
   DWORD    baseAddr;
   WORD		offset[3];	//in word, offset of red, green and blue LUT from base address
}ChipLUTInfoStruct;

typedef struct _InputRangeStruct_
{
   SDWORD 	min[3];  	//min of r, g, b; = realValue*2^24
   SDWORD 	max[3];	 	//max of r, g, b; = realValue*2^24
}InputRangeStruct;

typedef struct _GamutDataStruct_
{
   WORD	 	whiteLum;		//white luminance in cd/m^2
   WORD		white_x;			//white_x * 2^16
   WORD		white_y;			//white_x * 2^16
   WORD		red_x;			//red_x * 2^16
   WORD		red_y;			//red_x * 2^16
   WORD		green_x;			//green_x * 2^16
   WORD		green_y;			//green_x * 2^16
   WORD		blue_x;			//blue_x * 2^16
   WORD		blue_y;			//blue_x * 2^16
   WORD		gamma_A0;			//gamma A0 * 2^10
   WORD		gamma_A1;			//gamma A1 * 2^10
   WORD		gamma_A2;			//gamma A2 * 2^10
   WORD		gamma_A3;			//gamma A3 * 2^10
   WORD		red_min;          //1.3.12 fixed format. Multiplying actual value by 4096
   WORD		red_max;          //1.3.12 fixed format. Multiplying actual value by 4096
   WORD		green_min;        //1.3.12 fixed format. Multiplying actual value by 4096
   WORD		green_max;        //1.3.12 fixed format. Multiplying actual value by 4096
   WORD		blue_min;         //1.3.12 fixed format. Multiplying actual value by 4096
   WORD		blue_max;         //1.3.12 fixed format. Multiplying actual value by 4096
   WORD		bias_0;           //to specify the digital code for actual value 0.0
   WORD		divisor_1;        //to specify the divisor digital code so that the real value is completely defined by the
   //mapping from input digital code value: realValue = (input code - bias_0) / divisor_1
   BYTE		Flag;
} GamutDataStruct;


//for wide gamut support mode with simplified color profile through VCP code
#define WGSM_QUERY_CAPABILITY			0x00 	//– to query display wide gamut support(WGS) capabilities
#define WGSM_SWITCH_COLORMODE 		0x01 	//– to switch color mode
#define WGSM_QUERY_COLORMODE 			0x02 	//– to query specific color mode information
#define WGSM_UPDATE_COLORMODE 		0x03 	//– to update color mode info
#define WGSM_QUERY_NATIVEPROFILE		0x04 	//– to query native display profile
#define WGSM_UPDATE_NATIVEPROFILE	0x05 	//– to update native display profile
#define WGSM_DISPLAY_COLOR 			0x06 	//– to display color on screen
#define WGSM_CALIBRATION_MODE 		0x07 	//– to update calibration mode

#define WGSM_CS_MODE_STANDARD			0x00	//standard color space mode
#define WGSM_CS_MODE_CUSTOM			0x01	//custom color space mode
#define WGSM_CS_MODE_PROFILE			0x02	//simplified color profile mode

#define WGSM_UPDATE_SRC_PROFILE		0x00  //update current simplified source color profile
#define WGSM_UPDATE_DSP_PROFILE		0x01  //update native display color profile
#define WGSM_DISPLAY_COLOR_DISABLE	0x00	//disable color processing
#define WGSM_DISPLAY_COLOR_ENABLE	0x01	//enable color processing


#define WGS_NUM_CUSTOM_CS_MASK			0x0F		//# of custom color spaces mask
#define WGS_NUM_STANDARD_CS_MASK			0xF0		//# of standard color spaces mask
#define WGS_SIMPLE_PROFILE_MASK			0x03   	//simplified color profile support
#define WGS_SRC_SIMPLE_PROFILE_MASK		0x01   	//source simplified color profile support
#define WGS_DSP_SIMPLE_PROFILE_MASK		0x02   	//display simplified color profile support
#define WGS_DSP_PROFILE_MAXSIZE_MASK	0x0F   	//mask of maximum native display profile size
#define WGS_DSP_PROFILE_RETRIEVE_MASK  0x10     //mask of support to retrieve native display profile
#define WGS_DSP_PROFILE_STORE_MASK     0x20     //mask of support to store native display profile

typedef struct _WGS_CAPABILITIES_Struct_
{
   BYTE	 	nm;								//# of color spaces
   BYTE		pf;								//simplified profile support
   WORD		whiteLum[3];					//typical, minimum, maximum native display white lumiance in cd/m^2
   BYTE		nativeDisplayProfile;		//native display profile size, retrieve and store flag
} WGSCapStruct;


#ifdef __PARADIGM__
#pragma pack(pop)
#endif //__PARADIGM__ 


//external variables or prototypes
extern BYTE ROM PredefinedGammaLUTData[];    //defined in PredefinedGammaLUT.c
extern BYTE ROM GammaLUTData_xvYCC_FLUT[];
//extern WORD ROM PostLUT_Gamma22_6500K_AdobeRGB_RED_id[];       //defined in postLUT.h, included in csc.c file
//extern WORD ROM PostLUT_Gamma22_6500K_AdobeRGB_GREEN_id[];
//extern WORD ROM PostLUT_Gamma22_6500K_AdobeRGB_BLUE_id[];

extern WORD ROM Gamma_Adobe98_R_LTM240W1_L01[];
extern WORD ROM Gamma_Adobe98_G_LTM240W1_L01[];
extern WORD ROM Gamma_Adobe98_B_LTM240W1_L01[];

#define PANEL_COMP_GAMMA_RED   	Gamma_Adobe98_R_LTM240W1_L01
#define PANEL_COMP_GAMMA_GREEN   Gamma_Adobe98_G_LTM240W1_L01
#define PANEL_COMP_GAMMA_BLUE   	Gamma_Adobe98_B_LTM240W1_L01

void FindGammaLutIndex(WORD gamma, BYTE* lutId);
void far LoadCustomGamma(const GamutDataStruct* gamutData);
//BYTE far DoGamutMapping(BYTE* gamutDataBuf, BYTE mode);

void GetWGSCapabilities(WGSCapStruct* cs);
BYTE SetColorSpaceMode(BYTE mode);
BYTE UpdateSimplifiedColorProfile(GamutDataStruct* gamutDataStruct, BYTE size, BYTE type);

void DisableColorProcessing(void);
void EnableColorProcessing(void);

extern void far PreLUTAccessEnable(void);
extern void far WritePreLutRed(WORD W_AddrOffset, BYTE B_Val);
extern void far WritePreLutGreen(WORD W_AddrOffset, BYTE B_Val);
extern void far WritePreLutBlue(WORD W_AddrOffset, BYTE B_Val);
extern void far WritePostLut(void);
extern void far ColorForceUpdate(void);



#define  USE_DYNAMIC_GAMUTMAPPING_SUPPORT 	1

#endif //__DYNAMIC_GAMUT_MAPPING_H__
