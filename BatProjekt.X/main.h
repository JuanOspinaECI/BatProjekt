/*
 * @file main.h
 * @author xpress_embedo
 * @date 1 Feb, 2020
 * 
 * @brief 
 *
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif
// CONFIG1H
#pragma config OSC = INTIO2     // Oscillator Selection bits (Internal RC oscillator, CLKO function on RA6 and port function on RA7)
#pragma config FSCM = OFF        // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = OFF       // Internal/External Switchover bit (Internal/External Switchover mode enabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer enable bit (PWRT disabled)
#pragma config BOR = OFF         // Brown-out Reset enable bit (Brown-out Reset enabled)
#pragma config BORV = 3        // Brown-out Reset Voltage bits (VBOR set to 2.0V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBAD = DIG       // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVR = ON        // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-0007FFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (000800-000FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0001FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-0007FFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (000800-000FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0001FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-0007FFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (000800-000FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0001FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
  
#define _XTAL_FREQ        8000000u
#define Delay_ms(x)       __delay_ms(x)
  
#define DISABLE_INT()     (GIE=0)
#define ENABLE_INT()      (GIE=1)

#define TRUE              1u
#define FALSE             0u

/* Public Functions Prototypes */
uint32_t millis( void );
uint16_t AdcBat1;
uint16_t AdcBat2;
//float VolBat1;

#define OSCC_8MHZ_Primary 0b01111111
    // Bit de configuración de reloj 
    //bit 7     =   IDLEN 0
    //bit 6-4   =   IRFC2:IRCF0 (111 8MHZ)
    //bit 3     =   OSTS 1 
    //bit 2     =   IOFS 1
    //bit 1-0     =   SCS1:SCS0 00 Primary oscillator

#define Analog_ch1_ConvOff 0b0000000
    //Analog config
    //bit 5-6, for channel 0 = 000
    //         for channel 1 = 001   
    // bit 1 -> lectura de estado de conversion
    // bit 0 -> 1 para habilitar conversion
#define Analog_ch1_ConvOn 0b0000001

#define AN_VDD_VSS_AN1_AN0 0b00001101
    // bit 3-0 1101 -> habitlar analogo los canales AN0 y 1 de resto digital
    // VSS ref
    // VDD 

#define RightJus_Acq12TAD_ConvFOS16 0b10101101

void __interrupt() high_isr(void);
void __interrupt(low_priority) low_isr(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

