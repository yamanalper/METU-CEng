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
GLOBAL var3
GLOBAL dig1
GLOBAL dig2
GLOBAL dig3
GLOBAL dig4
GLOBAL dig5
GLOBAL dig6
GLOBAL flag1
GLOBAL flag2
GLOBAL flag3
GLOBAL flag4
GLOBAL flag5
GLOBAL flag6
GLOBAL sflag
GLOBAL counter
GLOBAL t
GLOBAL t_count
GLOBAL stop_switch
GLOBAL dig_indx


; Define space for the variables in RAM
PSECT udata_acs
var1:
    DS 1 ; Allocate 1 byte for var1
var2:
    DS 1 
var3:
    DS 1
dig1:
    DS 1
dig2:
    DS 1
dig3:
    DS 1
dig4:
    DS 1
dig5:
    DS 1
dig6:
    DS 1
flag1:
    DS 1
flag2:
    DS 1
flag3:
    DS 1
flag4:
    DS 1
flag5:
    DS 1
flag6:
    DS 1
sflag:
    DS 1
counter:
    DS 1
t:
    DS 1
t_count:
    DS 1
dig_indx:
    DS 1
stop_switch:
    DS 1


PSECT resetVec,class=CODE,reloc=2
resetVec:
    goto       main

PSECT CODE
main:
    clrf var1	; var1 = 0		
    setf var2
    clrf var3
    clrf flag1
    clrf flag2
    clrf flag3
    clrf flag4
    clrf flag5
    clrf flag6
    clrf counter
    clrf t
    clrf t_count
    clrf sflag
    clrf dig_indx
    clrf stop_switch
    
    clrf TRISC	; port c output
    clrf TRISD	; port d output
    setf TRISE	; port e input
    
    setf LATC
    setf LATD
    
    call delay
    
    clrf LATC
    clrf LATD
    clrf PORTE
    
    movlw 2
    movwf dig1
    movlw 5
    movwf dig2
    movlw 8
    movwf dig3
    movlw 1
    movwf dig4
    movlw 1
    movwf dig5
    movlw 5
    movwf dig6
    
    movff dig1, PORTC
    
    clrf var1
    movlw 203
    movwf t
    call first_delay
    clrf t
    
    
main_loop:
    call check_input
    call check_flag
    incf t
    bov timer_control
    goto main_loop
    
    
timer_control:    
    incf t_count
    movf t_count,0
    sublw 10
    bz cycle_500
    goto main_loop
    
    
cycle_500:
    btg PORTD,0
    clrf t_count
    btfsc stop_switch, 0
    goto main_loop
    incf dig_indx
    movf dig_indx, 0
    iorlw 0
    bz set_dig1
    movf dig_indx, 0
    sublw 1
    bz set_dig2
    movf dig_indx, 0
    sublw 2
    bz set_dig3
    movf dig_indx, 0
    sublw 3
    bz set_dig4
    movf dig_indx, 0
    sublw 4
    bz set_dig5
    movf dig_indx, 0
    sublw 5
    bz set_dig6
    goto main_loop
    

set_dig1:
    movff dig1, PORTC
    goto main_loop
set_dig2:
    movff dig2, PORTC
    goto main_loop
set_dig3:
    movff dig3, PORTC
    goto main_loop
set_dig4:
    movff dig4, PORTC
    goto main_loop
set_dig5:
    movff dig5, PORTC
    goto main_loop
set_dig6:
    movff dig6, PORTC
    movlw -1
    movwf dig_indx
    goto main_loop
    
    
    
bit0_press:
    bsf flag1,0
    return
bit1_press:
    bsf flag2,0
    return
bit2_press:
    bsf flag3,0
    return
bit3_press:
    bsf flag4,0
    return
bit4_press:
    bsf flag5,0
    return
bit5_press:
    bsf flag6,0
    return
bit7_press:
    bsf sflag,0
    return
   
    
check_input:
    btfsc PORTE, 0
    rcall bit0_press
    btfsc PORTE, 1
    rcall bit1_press
    btfsc PORTE, 2
    rcall bit2_press
    btfsc PORTE, 3
    rcall bit3_press
    btfsc PORTE, 4
    rcall bit4_press
    btfsc PORTE, 5
    rcall bit5_press
    btfsc PORTE, 7
    rcall bit7_press
    clrf var1
    call little_delay
    return
    
check_flag:
    btfsc flag1, 0
    rcall check_release0
    btfsc flag2, 0
    rcall check_release1
    btfsc flag3, 0
    rcall check_release2
    btfsc flag4, 0
    rcall check_release3
    btfsc flag5, 0
    rcall check_release4
    btfsc flag6, 0
    rcall check_release5
    btfsc sflag, 0
    rcall check_release7
    return
    
check_release0:
    btfss PORTE, 0
    rcall bit0_release
    return
check_release1:
    btfss PORTE, 1
    rcall bit1_release
    return
check_release2:
    btfss PORTE, 2
    rcall bit2_release
    return
check_release3:
    btfss PORTE, 3
    rcall bit3_release
    return
check_release4:    
    btfss PORTE, 4
    rcall bit4_release
    return
check_release5:
    btfss PORTE, 5
    rcall bit5_release
    return
check_release7:
    btfss PORTE, 7
    rcall bit7_release
    return
    
    
    
bit0_release:
    incf dig1
    clrf flag1
    movf dig1,0
    sublw 10
    bz reset_dig1
    return
bit1_release:
    incf dig2
    clrf flag2
    movf dig2,0
    sublw 10
    bz reset_dig2
    return
bit2_release:
    incf dig3
    clrf flag3
    movf dig3,0
    sublw 10
    bz reset_dig3
    return
bit3_release:
    incf dig4
    clrf flag4
    movf dig4,0
    sublw 10
    bz reset_dig4
    return
bit4_release:
    incf dig5
    clrf flag5
    movf dig5,0
    sublw 10
    bz reset_dig5
    return
bit5_release:
    incf dig6
    clrf flag6
    movf dig6,0
    sublw 10
    bz reset_dig6
    return
bit7_release:
    btg stop_switch, 0
    clrf sflag
    return
    
   
    
reset_dig1:
    clrf dig1
    return
reset_dig2:
    clrf dig2
    return
reset_dig3:
    clrf dig3
    return
reset_dig4:
    clrf dig4
    return
reset_dig5:
    clrf dig5
    return
reset_dig6:
    clrf dig6
    return
    
    
    
little_delay:
    incf var1
    movf var1,0
    sublw 29
    bnz little_delay
    return
    
    
delay:
    movlw 5
    movwf var3
    timer:
	inner_timer: 
	    incf var1
	    bnc inner_timer
	    nop
	    nop
	    nop
	    nop
	    nop
	    nop
	    nop
	    nop
	    nop
	    nop
	    nop
	    nop
	    nop
	    nop
	    nop
	decf var2
	bnz timer

    setf var2
    repeat:
	decf var3
	bnz timer 
	
    return
    
    
    
first_delay:
    inner_delay:
	incf var1
	bnc inner_delay
    incf t
    bnz first_delay
    return
    

end resetVec