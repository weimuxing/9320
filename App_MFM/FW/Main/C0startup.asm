;/*
;    $Workfile:   C0startup.asm  $
;    $Revision: 1.13 $
;    $Date: 2012/04/10 10:01:26 $
;*/
;//******************************************************************************
;// COPYRIGHT (C) STMicroelectronics 2011.
;//
;// All rights reserved. This document contains proprietary and
;// confidential information of the STMicroelectronics Group. This
;// document is not to be copied in whole or part. STMicroelectronics
;// assumes no responsibility for the consequences of use of such
;// information nor for any infringement of patents or other rights of
;// third parties which may result from its use. No license is granted by
;// implication or otherwise under any patent or patent rights of
;// STMicroelectronics.
;// STMicroelectronics
;// products are not authorized for use as critical components in life
;// support devices or systems without express written approval of
;// STMicroelectronics.
;//==============================================================================
;//
;// MODULE:      C0Startup.asm
;//
;// USAGE:       This module contains system and External ROM data initialization
;//
;//              The __ADDR24__ is automatically defined for the project
;//              when the Extended Address Mode option is choosen in the IDE file
;//
;//******************************************************************************

.186                            ; Required for target independence

include main/Define.Inc
;//******************************************************************************
;//  L O C A L    D E F I N I T I O N S
;//******************************************************************************
; Some options can be disabled to reduce the size of the startup code
; and run-time library helper code.  Simply place a semicolon in front
; of the following definitions to remove support for selected item.
;
;    __ENABLE_EXCEPTIONS      Includes C++ exception handling support
;__ENABLE_EXCEPTIONS            =     1    ;

STACK_SIZE_WORD              EQU 0400h    ;default 2KB(0x400*2) stack

IRAM_SIZE                EQU 08000h
EXT_ROM_BASE_ADDR        EQU 0C9C0h
EXT_CS2_BASE_ADDR        EQU 0C9CCh

;//******************************************************************************
;//  S E G M E N T S    D E F I N I T I O N S
;//******************************************************************************
IFNDEF __ADDR24__
_PAD_ = 16
_ALIGN_ equ <para>
ELSE
_PAD_ = 256
_ALIGN_ equ <page>
ENDIF

; Segment and group declarations.  The order of these declarations is
; used to control the order of segments in the .ROM file since the
; Paradigm linker copies segments in the same order they
; are encountered in the object files.
; Make sure that this startup module is specified first when linking
; the application.
% _TEXT segment _ALIGN_ public use16 'CODE'    ;Default code
_TEXT ends

; These are segment definitions for the classes ROMDATA and ENDROMDATA.  Class
; ROMDATA never contains anything (it is filled in by a Paradigm LOCATE DUP
; directive).  Class ENDROMDATA is used to mark the end of class ROMDATA and
; prevent a segment alias (since class ROMDATA will have zero length).
;
; NOTE: You can't take the address of segments in class ENDROMDATA as they are
;       aliased with class ROMDATA.
;
% _RD segment _ALIGN_ public use16 'ROMDATA'
public _romdata_start ;Mark the start of class ROMDATA
_romdata_start label byte
_RD ends

% _ERD segment _ALIGN_ public use16 'ENDROMDATA'
public _romdata_end ;Mark the end of class ROMDATA
_romdata_end label byte
    db _PAD_ dup (?) ;Force the next segment to a new paragraph/page
_ERD ends

% _FAR_CONST segment _ALIGN_ public 'FAR_CONST'
_FAR_CONST ends

; This segment is dedicated for IROM usage.  
; Note: The start address of the segment is specified in the cfg file.
% _IROM_DATA segment _ALIGN_ public use16 'IROM_DATA'

_IROM_DATA ends
DGROUP group _IROM_DATA

; This segment is dedicated for OSD Command Buffer usage.
; Note: This segment is not initalized by system.
% _OSD_CMDBUF segment _ALIGN_ public use16 'OSD_CMDBUF'
_OSD_CMDBUF ends
DGROUP group _OSD_CMDBUF

; Initialized data
% _DATA segment _ALIGN_ public use16 'DATA'        
public _initialized_data_start 
_initialized_data_start label byte
_DATA ends
DGROUP group _DATA

; Uninitialized data
_BSS segment word public use16 'BSS'            
public _uninitialized_data_start
_uninitialized_data_start label byte
_BSS ends
DGROUP group _BSS

;Dummy NVRAM para to make linker happy
% _NVRAM segment word public use16 'NVRAM'    
public _NVRAM_data_start
_NVRAM_data_start label byte
    dw  ?
_NVRAM ends
DGROUP group _NVRAM

;Dummy STACK para to make linker happy
% _STACK segment para stack use16 'STACK'       
    dw STACK_SIZE_WORD dup (?)    
_STACK ends
DGROUP group _STACK

;IFDEF FLI326xxBx
% _FARBSS segment _ALIGN_ public use16  'FAR_BSS'        ; uninitialized far data
    _uninitialized_far_data_start label byte
    dw ?
_FARBSS ends
;ENDIF

;//******************************************************************************
;//  E X T E R N A L    F U N C T I O N S
;//******************************************************************************
IFDEF __ENABLE_EXCEPTIONS
    extrn __ExceptInit:far ;Exception handling initialization
ENDIF

extrn _main:far ;User application entry point

;//******************************************************************************
;//  C O D E
;//******************************************************************************
_TEXT    segment
    assume    cs:_TEXT
CUT_2_0 = 1
IF CUT_2_0
    ;---------------------------------------------------------------------------
    ; Start of bootup block 
    ;---------------------------------------------------------------------------
       org 000h            ;Bootup Address = 0x410000 (4100:00) // Cut 2.0 __ADDR24__
; // Signature Block
	signature    db 'A','T','H','E'
	vertag	     db 0ffh
	not_used6    db 6 dup(0ffh)
	crcCheckFlag db 0
    	configCrc2   dd 00001021h 
; // Startup
    mov    ax, 0h              ; DS = 00000h
    mov    ds, ax              
    assume    ds:0h           
        
    ;following code is very correlated with CODE_START_ADDRESS calculation 
    ;There is a difference in 24bit address mode EXT_ROM_BASE_ADDR is always configured for 4M (even if code size is 1M or 2M. 
    IFDEF __ADDR24__
        mov word ptr ds:[EXT_ROM_BASE_ADDR], 0400h        ;XROM_START = 0x400000 (8M Bytes)
    ELSE
        mov word ptr ds:[EXT_ROM_BASE_ADDR], 0020h        ;XROM_START = 0x20000 (832K Bytes)
    ENDIF
    
    nop    ;Time delay for the system to setup the XROM size
    nop
    nop
    nop
    jmp   _startup_XROM        

    ;---Signature Block and xrom_config STRUC---  
    IFDEF __ADDR24__
        org 032h    ; // Cut 2.0
    ELSE
        org 022h                 
    ENDIF        
        configCrc    dw 0000h        ; contrl block CRC, need post process by tool
        configLen    dw 30            ; length of the control block
    IFDEF __ADDR24__
        flashSize    dd FLASH_BANK_SIZE ; flash size
    ELSE
        flashSize    dd 100000h      ; max. flash size, 1M bytes
    ENDIF
        codeCrc      dw 0000h
        fw_Major     db FW_VER_MAJOR
        fw_Minor     db FW_VER_MINOR
        fw_Build     db FW_VER_BUILD
        not_used     db ?
    ;---------------------------------------------------------------------------
    ; End of bootup block 
    ;---------------------------------------------------------------------------
ENDIF

_startup_XROM proc far                    ; Startup code entry point
public _startup_XROM
    ;---------------------------------------------------------------------------
    ;Check flash image end signature.
    mov    ax, seg end_signature
    mov    es, ax
    cmp    word ptr es:[offset end_signature], 5441h ; END_SIGNATURE_LO
    jne    __EnterIrom
    cmp    word ptr es:[offset end_signature + 2], 414Eh ; END_SIGNATURE_HI
    jne    __EnterIrom

    mov    ax, 0
    mov    ss, ax                            ;Stack setup SS = DGROUP = CLASS DATA = 0x0000 (ax=0)
    mov    ds, ax
    mov    es, ax
    assume  ss:0h, ds:0, es:0
    mov    sp, IRAM_SIZE                    ;SP = top of RAM

    ;---------------------------------------------------------------------------
    ;Prepare the segment registers for initialization.  The initialized
    ;data is assumed to have been located in the class ROMDATA which begins
    ;with the segment _RD.
    ;Copy DGROUP initialized data from its position in ROM to the target address
    ;in RAM.
    ;Because this is a group, there is never more than 64K of data to copy.
    mov    ax, _RD
    mov    ds, ax
    assume ds:_RD
    mov    si, offset _romdata_start
    mov    di, offset DGROUP:_initialized_data_start
    mov    cx, offset DGROUP:_uninitialized_data_start
    sub    cx, di
    shr    cx, 1
    rep    movsw

    ;---------------------------------------------------------------------------
    ;Zero out the uninitialized data area (_uninitialized_data_start - _NVRAM_data_start)
    mov    ax, 0
    mov    ds, ax               ;DGROUP = 0x0000 (ax=0)
    assume ds:0
    mov    di, offset DGROUP:_uninitialized_data_start
    mov    cx, offset DGROUP:_NVRAM_data_start	
    sub    cx, di
    shr    cx, 1
    rep    stosw

    ;---------------------------------------------------------------------------
    ;fill stack with a constant pattern to be able to monitor a stack watermark
    mov    ax, 0
    mov    ds, ax               ;DGROUP = 0x0000 (ax=0)
    assume ds:0
    std                         ;set direction flag to have decrement addr
    mov    ax, 5555h            ;ax = fill data (0x5555)
    mov    di, (IRAM_SIZE-2)    ;di = data start (0x8000 - one word = 0x7FFE)
    mov    cx, STACK_SIZE_WORD  ;cx = length (0x0400 word = 2KB)
    rep    stosw                ;rep store pattern
    cld
        
    ;Memory is now initialized.  We need to initialize the exception handling
    ;logic and call any linked in initializers before handling control over
    ;to the user code.
    mov    bp, 0h
    IFDEF __ENABLE_EXCEPTIONS
        push    bp
        call    __ExceptInit
        pop    ax
    ENDIF

    ;---------------------------------------------------------------------------
    ;Call the C entry point main() - initialization is complete 
    jmp        _main

_startup_XROM endp

;******************************************************************************
;Call for switch to IROM 
__EnterIrom proc dist
    public  __EnterIrom
    OCM_NR_SCRATCHPAD   EQU 0CA6Ah
    CLOCK_CONFIG1       EQU  8100h
    HARD_RESETS_2       EQU 0C80Eh
    AUDIO_HARD_RESETS   EQU 0C810h
    LVDS_RESET_CTRL     EQU  9C58h
    HARD_RESETS         EQU 0C80Ch
    mov    word ptr ds:[OCM_NR_SCRATCHPAD], 01234h    
    mov    word ptr ds:[CLOCK_CONFIG1], 0h ; OCM_CLK_SEL = 0
    mov    word ptr ds:[HARD_RESETS_2], 0FFFFh
    mov    word ptr ds:[AUDIO_HARD_RESETS], 0FFFFh
    mov    word ptr ds:[LVDS_RESET_CTRL], 2h ; LVDSTX_HARD_RESET
    mov    word ptr ds:[HARD_RESETS], 0FFFFh
__EnterIrom endp

;******************************************************************************
;Called by the exit handling code to execute C++ static destructors and #pragma exit routines.
__cleanup proc dist
    public  __cleanup
    ret
__cleanup endp

_TEXT    ends


;******************************************************************************
; This segment contains bootup code for XROM. 
; The Boot code address is 0xEFFC0 (20bit) and 0xBFFFC0 (24bit) is defined
; in ExtRom.cfg
; The __ADDR24__ is automatically defined for the project when the Extended
; Address Mode option is choosen in the IDE file
;
; should be defined at 0xEFFC0  in 20-bit mode
; should be defined at 0x7FFF00 in 24-bit mode
;******************************************************************************
IF CUT_2_0
 % _END_SEGMENT segment _ALIGN_ public  'END_SEG'
    org 0F0h
    end_signature     db 'A','T','N','A'
_END_SEGMENT  ends
ELSE
% _START_SEGMENT segment _ALIGN_ public  'START'
    assume cs:nothing         ;to avoid near jmp

    IFDEF __ADDR24__
        org 0C0h            ;Bootup Address = 0xBFFFC0 (BFFF:C0)
    ELSE
        org 00h                ;Bootup Address = 0x0EFFC0 (EFFC:00)
    ENDIF

    mov    ax, 0h              ; DS = 00000h
    mov    ds, ax              
    assume    ds:0h           
        
    ;following code is very correlated with CODE_START_ADDRESS calculation 
    ;There is a difference in 24bit address mode EXT_ROM_BASE_ADDR is always configured for 4M (even if code size is 1M or 2M. 
    IFDEF __ADDR24__
        mov word ptr ds:[EXT_ROM_BASE_ADDR], 0400h        ;XROM_START = 0x400000 (8M Bytes)
    ELSE
        mov word ptr ds:[EXT_ROM_BASE_ADDR], 0020h        ;XROM_START = 0x20000 (832K Bytes)
    ENDIF
    
    nop    ;Time delay for the system to setup the XROM size
    nop
    nop
    nop
    jmp   _startup_XROM        

    ;---Signature Block and xrom_config STRUC---  
    IFDEF __ADDR24__
        org 0E2h    
    ELSE
        org 022h                 
    ENDIF        
        configCrc    dw 0000h        ; contrl block CRC, need post process by tool
        configLen    dw 30            ; length of the control block
    IFDEF __ADDR24__
        flashSize    dd 400000h        ; max. flash size, 4M bytes
    ELSE
        flashSize    dd 100000h      ; max. flash size, 1M bytes
    ENDIF
        codeCrc      dw 0000h         ; VER_LEN=8 if version string starts here
        fw_Minor     db 0h            ; VER_LEN=4 if version string starts here
        fw_Major     db 0h            ; VER_LEN=3 if version string starts here
        fw_Build     db 0             ; VER_LEN=2 if version string starts here	
        not_used     db ?	      ; VER_LEN=1 if version string starts here
	signature    db 'A','T','H','E'
	vertag	     db 0ffh
	not_used6    db 6 dup(0ffh)
	crcCheckFlag db 0
    	configCrc2   dd 00001021h 
                                    
_START_SEGMENT  ends
ENDIF

end    _startup_XROM       
;//*********************************  END  **************************************

