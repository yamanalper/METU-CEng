// ============================ //
// Do not edit this part!!!!    //
// ============================ //
// 0x300001 - CONFIG1H
#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator,
                                // PLL enabled (Clock Frequency = 4 x FOSC1))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit
                                // (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit
                                // (Oscillator Switchover mode disabled)
// 0x300002 - CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out
                                // Reset disabled in hardware and software)
// 0x300003 - CONFIG1H
#pragma config WDT = OFF        // Watchdog Timer Enable bit
                                // (WDT disabled (control is placed on the SWDTEN bit))
// 0x300004 - CONFIG3L
// 0x300005 - CONFIG3H
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit
                                // (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled;
                                // RE3 input pin disabled)
// 0x300006 - CONFIG4L
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply
                                // ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit
                                // (Instruction set extension and Indexed
                                // Addressing mode disabled (Legacy mode))
#pragma config DEBUG = OFF      // Disable In-Circuit Debugger

// Timer Related Definitions
#define KHZ 1000UL
#define MHZ (KHZ * KHZ)
#define _XTAL_FREQ (40UL * MHZ)
// ============================ //
//             End              //
// ============================ //
#include <xc.h>

// ============================ //
//        DEFINITIONS           //
// ============================ //

struct hippo{
    int size;
    int position;
}hippo;

// ============================ //
//          GLOBALS             //
// ============================ //

int blink_counter;
int gravity_counter;
int score_counter;
int display_counter;
int softReset_counter;
int wait = 0;
int total_score = 0;
int round_score = 0;
int softReset = 0;
int reset_switch = 0;
unsigned char list[10] = {
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111
};
// ============================ //
//          FUNCTIONS           //
// ============================ //

void initial(){
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;
    LATF = 0x00;
    LATG = 0x00;
    TRISD = 0x00;
    TRISB = 0x01;
    TRISH = 0x00;
    TRISJ = 0x00;
    LATH = 0x0F;
    LATJ = 0x7F;
    INTCON = 0b10110000;
    __delay_ms(1000);
    T0CON = 0b10001000; 
    TMR0H = 0x3C;
    TMR0L = 0xAF;
    blink_counter = 0;
    gravity_counter = 0;
    score_counter = 0;
    display_counter = 0;
    softReset_counter = 0;
    round_score = 100;
    LATDbits.LATD0 = 1;
    LATDbits.LATD7 = 1;
    hippo.size = 1;
    hippo.position = 0;
    
}


void handleBlink(){
    blink_counter = 0;
    LATDbits.LATD0 = ~LATDbits.LATD0;
}
void handleGravity(){
    gravity_counter = 0;
    if (hippo.position > 0){
        hippo.position--;
        LATDbits.LD1 = 0;
        LATDbits.LD2 = 0;
        LATDbits.LD3 = 0;
        LATDbits.LD4 = 0;
        LATDbits.LD5 = 0;
        LATDbits.LD6 = 0;
        LATDbits.LD7 = 0;
        for(int i = 0; i < hippo.size; i++){
            LATD |= (1 << (7 - hippo.position - i));
        }
    }
    
}
void handleScore(){
    score_counter = 0;
    if (round_score >= 10){
        round_score -= 10;
    } 
}


void hardReset(){
    LATD = 0x00;
    hippo.position = 0;
    hippo.size = 1;
    round_score = 100;
    LATDbits.LATD7 = 1;
    LATDbits.LATD0 = 1;
}



void handleUp(){
    hippo.position++;
    if(hippo.position + hippo.size == 8){
        hippo.size++;
        hippo.position = 0;
        softReset = 1;
    }
    else{
        LATDbits.LD1 = 0;
        LATDbits.LD2 = 0;
        LATDbits.LD3 = 0;
        LATDbits.LD4 = 0;
        LATDbits.LD5 = 0;
        LATDbits.LD6 = 0;
        LATDbits.LD7 = 0;
        for(int i = 0; i < hippo.size; i++){
            LATD |= (1 << (7 - hippo.position - i));
        }
    }
}

void updateDisplay(){
    int bit3 = total_score / 1000;
    int bit2 = (total_score / 100) % 10;
    int bit1 = (total_score / 10) % 10;
    int bit0 = total_score % 10;
    display_counter++;
    if(display_counter == 1){
        LATHbits.LATH3 = 0;
        LATHbits.LATH0 = 1;
        LATJ = list[bit3];
    }
    else if(display_counter == 2){
        LATHbits.LATH0 = 0;
        LATHbits.LATH1 = 1;
        LATJ = list[bit2];
    }
    else if(display_counter == 3){
        LATHbits.LATH1 = 0;
        LATHbits.LATH2 = 1;
        LATJ = list[bit1];
    }
    else if(display_counter == 4){
        LATHbits.LATH2 = 0;
        LATHbits.LATH3 = 1;
        LATJ = list[bit0];
        display_counter = 0;
    }
}




// ============================ //
//   INTERRUPT SERVICE ROUTINE  //
// ============================ //
__interrupt(high_priority)
void HandleInterrupt()
{
    if(INTCONbits.TMR0IF){
        if(softReset){
            INTCONbits.TMR0IF = 0;
            INTCONbits.INT0IF = 0;
            total_score += round_score;
            round_score = 0;
            TMR0H = 0x3C;
            TMR0L = 0xAF;
            if(reset_switch == 6){
                    round_score = 100;
                    reset_switch = 0;
                    softReset = 0;
                    softReset_counter = 0;
                    blink_counter = 0;
                    gravity_counter = 0;
                    score_counter = 0;
                    LATD = 0x00;
                    if(hippo.size == 1){LATD = 0x81;}
                    else if(hippo.size == 2){LATD = 0xC1;}
                    else if(hippo.size == 3){LATD = 0xE1;}
                    else if(hippo.size == 4){LATD = 0xF1;}
                    else if(hippo.size == 5){LATD = 0xF9;}
                    else if(hippo.size == 6){hardReset();}
            }
            else{
                if(softReset_counter == 0){
                    reset_switch++;
                    LATD = 0xFF;
                }
                softReset_counter++;
                if(softReset_counter == 80){
                    reset_switch++;
                    softReset_counter = -80;
                    LATD = 0x00;
                }
            }
        }
        else{
            INTCONbits.TMR0IF = 0;
            TMR0H = 0x3C;
            TMR0L = 0xAF;
            blink_counter++;
            if(blink_counter == 100){ handleBlink();}
            gravity_counter++;
            if(gravity_counter == 70){ handleGravity();}
            score_counter++;
            if(score_counter == 200) {handleScore();}
        }
        updateDisplay();
    }

    if(INTCONbits.INT0IF){
        if(!softReset){
            INTCONbits.INT0IF = 0;
            handleUp();
        }
        
    }

}

// ============================ //
//            MAIN              //
// ============================ //
void main()
{
    initial();
    while(1){}
}