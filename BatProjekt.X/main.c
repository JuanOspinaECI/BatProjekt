/*
 * File:   main.c
 * Author: 
 *
 * Created on October 16, 2023, 2:50 PM
 */


#include <xc.h>

void Setup (void) {
    // Bit de configuración de reloj 
    //bit 7     =   IDLEN 0
    //bit 6-4   =   IRFC2:IRCF0 (111 8MHZ)
    //bit 3     =   OSTS 1 
    //bit 2     =   IOFS 
    //bit 1     =   SCS1:SCS0
    OSCCON = 01111010;
    
    //Configuracion Puerto A
    // Todos como entradas 
    // RA2 entrada = 1 (Led prueba)
    TRISA = 11111011;
    
    
    //Analog config
    //bit 5-6, for channel 0 = 000
    //         for channel 1 = 001   
    // bit 1 -> lectura de estado de conversion
    // bit 0 -> 1 para habilitar conversion
    ADCON0 = 0000000;
    
    // bit 3-0 1101 -> habitlar analogo los canales AN0 y 1 de resto digital
    ADCON1 = 00001101;
    
    ADCON2 = 10101101; //Revisar por si no funciona el ADC
    
    TRISB = 11100000;
    
    
    //Enable  USART bit 7 as 1 bit 6 as 0
    // Enable Ic2 bit 3-4 as 11 input
    TRISC = 10011000;
    
    
    //USART
    RCSTA = 10000000;
    
    
    //I2C
    //bit 3-0 1000 for master mode
    SSPCON1 = 00101000;
    //Poner bit 0 en 1 para iniciar el i2c 
    SSPCON2 = 01011101;
    
    SSPSTAT = 10001000;
    
    
}

void main(void) {
    return;
}

