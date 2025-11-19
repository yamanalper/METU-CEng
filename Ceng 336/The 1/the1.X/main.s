PROCESSOR 18F8722

#include <xc.inc>

; CONFIGURATION (DO NOT EDIT)
; CONFIG1H
CONFIG OSC = HSPLL      ; Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
CONFIG FCMEN = OFF      ; Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
CONFIG IESO = OFF       ; Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
; CONFIG2L
CONFIG PWRT = OFF       ; Power-up Timer Enable bit (PWRT disabled)
CONFIG BOREN = OFF      ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
; CONFIG2H
CONFIG WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
; CONFIG3H
CONFIG LPT1OSC = OFF    ; Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
CONFIG MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
; CONFIG4L
CONFIG LVP = OFF        ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
CONFIG XINST = OFF      ; Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))
CONFIG DEBUG = OFF      ; Disable In-Circuit Debugger


GLOBAL var1
GLOBAL var2
GLOBAL result

; Define space for the variables in RAM
PSECT udata_acs
var1:
    DS 1 ; Allocate 1 byte for var1
var2:
    DS 1 
temp_result:
    DS 1   
result: 
    DS 1


PSECT resetVec,class=CODE,reloc=2
resetVec:
    goto       main

PSECT CODE
main:
    clrf var1	; var1 = 0		
    clrf var2   ; var2 = 0		
    clrf result ; result = 0

    movlw 4     ; WREG = 4 hexadecimal -> 04h, binary -> 0100B
    movwf var1  ; var1 = 4; 

    movlw 5     ; WREG = 5
    movwf var2  ; var1 = 5; 

    call fSum   ; call the function fSum and PC will push the next instruction address to the Stack
    movwf result; result = WREG
    comf result ; result = the complement of the result value 
  
    goto main   ; Jump to the label main 

fSum:
    clrf temp_result 	; temp_result = 0
    movf var1,W		; WREG = var1 = 4
    addwf var2,0 	; WREG = WREG + var2
    movwf temp_result,1   ; temp_result = WREG 
    return		; The function terminates

end resetVec