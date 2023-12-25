/*
 * File:   main.c
 * Author: 
 *
 * Created on October 16, 2023, 2:50 PM
 */

// PIC18F2220 Configuration Bit Settings

// 'C' source line config statements

//#define LATB4_bit CE2
//#define LATB5_bit CE1


#include <xc.h>
#include "main.h"
#include "stdint.h"
#include "stdbool.h"
#include <stdio.h>
#include "UART.h"

char NumStr(unsigned int Value);
void ConvInt(unsigned int Value);
char  Str[4];



void Setup (void) {

    OSCCON = OSCC_8MHZ_Primary;
    
    //Setup A Port
    // All input 
    // RA2 input = 1 (test LED)
    TRISA = 0b11110011;
    
    ADCON0 = Analog_ch1_ConvOff;
    
    ADCON1 = AN_VDD_VSS_AN1_AN0;
    
    ADCON2 = RightJus_Acq12TAD_ConvFOS16; 
    
    TRISB = 0b11100000;
    
    //Enable  USART bit 7 as 1 bit 6 as 0
    //Enable Ic2 bit 3-4 as 11 input
    TRISC = 0b10011000;
    
    
    //USART
    RCSTA = 0b10000000;
    TXSTA = 0b00100010; // configurar como asincrono uart
    
    //I2C
    //bit 3-0 1000 for master mode
    SSPCON1 = 0b00101000;
    //Set bit 0 in 1 to start i2c 
    SSPCON2 = 0b01011101;
    
    SSPSTAT = 0b10001000;
    
    OSCCONbits.IRCF = 0x07;//0x07;
    OSCCONbits.SCS = 0b11;
    //while(OSCCONbits.IOFS!=1)
    RCONbits.IPEN = 1; // enable priorities levels on interrupts
    INTCONbits.GIEH = 1; // Enable interrupts 
    INTCONbits.GIEL = 1; // Enable interrupts 
    
    
}
unsigned int MedVoltageBat_1()
{
    // Function to read ADC value from Battery 1
    
    // Isolate batteries
    // Convertir a dato int 
    ADCON2bits.ACQT = 0b111; // Acquisition Time 20TAD
    ADCON2bits.ADCS = 0b100; // A/D Conversion Clock FOSC/4
    ADCON0bits.ADON = 1; // ON
    ADCON0bits.CHS = 0b0000; // CH0
    __delay_ms(1);
    ADCON0bits.GO = 1;
    while(ADCON0bits.DONE == 1);
    __delay_ms(1);
    AdcBat1 = (ADRESH << 8) + ADRESL;
    ADCON0bits.ADON = 1; // OFF
    return AdcBat1;
}

unsigned int MedVoltageBat_2()
{
    // Function to read ADC value from Battery 1
    
    ADCON2bits.ACQT = 0b111; // Acquisition Time 20TAD
    ADCON2bits.ADCS = 0b100; // A/D Conversion Clock FOSC/4
    ADCON0bits.ADON = 1; // ON
    ADCON0bits.CHS = 0b0001; // CH0
    __delay_ms(1);
    ADCON0bits.GO = 1;
    while(ADCON0bits.DONE == 1);
    __delay_ms(1);
    AdcBat2 = (ADRESH << 8) + ADRESL;
    ADCON0bits.ADON = 1; // OFF
    return AdcBat1;
}
bool MedUsb()
{
    //ADCON1bits.PCFG = 0b1011; // Habilitar AN3 como entrada analog 
    //ADCON0bits.CHS = 0b1111; // Deshabiliutar AN3 como Analog
    TRISAbits.RA3 = 0; // just in case
    if (PORTAbits.RA3 == 1) return true;
    return false;
    
}

int Abs(int ValueA)
{
    if (ValueA >= 0) return ValueA;
    else return -ValueA;
}

void Timer0IntInit()
{
    T0CONbits.TMR0ON    = 1; // Timer ON
    T0CONbits.T08BIT    = 0; // 16 bit control
    T0CONbits.T0CS      = 0; //Internal clock
    T0CONbits.PSA       = 0; //Prescaler is assigned
    T0CONbits.T0PS      = 0b011; //1:8
    
    GIE     =   1;		/* Enable Global Interrupt */
    PEIE    =   1;  		/* Enable Peripheral Interrupt */
    TMR0IE  =   1;		/* Enable Timer1 Overflow Interrupt */
    TMR0IF  =   0;
    
    /* Enable 16-bit TMR1 register,no pre-scale,internal clock, timer OFF */
    T1CON   =   0x80;		

    TMR0    =   0xF856;//F856;	/* Load Count for generating delay of 1ms */
    TMR0ON  =   1;		/* Turn ON Timer1 */
}

void __interrupt() high_isr(void){
    INTCONbits.GIEH = 0;
    if(PIR1bits.RCIF){
        uart_receiver(&data,&got_data_bool);
       PIR1bits.RCIF=0;
    }
    
    if(TMR0IF == 1)
    { 
        TMR0IF = 0;
        UART_printf("1 ");
        ConvInt(AdcBat1);
        uart_send(&Str[0]);
        uart_send(&Str[1]);
        uart_send(&Str[2]);
        uart_send(&Str[3]);
        UART_printf(". \n");
        //uart_send(0x0A);
        UART_printf("2 ");
        ConvInt(AdcBat2);
        uart_send(&Str[0]);
        uart_send(&Str[1]);
        uart_send(&Str[2]);
        uart_send(&Str[3]);
        UART_printf(". \n");
        //uart_send(0x0A);
        PORTCbits.RC0 = ~PORTCbits.RC0; 
    }
    
    INTCONbits.GIEH = 1;
}

void ConvInt(unsigned int Value)
{
    unsigned int     Mil     = 0;
    unsigned int     Cien    = 0;
    unsigned int     Diez    = 0;
    unsigned int     Uno     = 0;
    Mil     = Value/1000;
    Value   = Value - (1000*Mil);
    Cien    = Value/100;
    Value   = Value - (Cien*100);
    Diez    = Value/10;
    Uno     = Value - (Diez*10);
    Str[0] = NumStr(Mil);
    Str[1] = NumStr(Cien);
    Str[2] = NumStr(Diez);
    Str[3] = NumStr(Uno);
}

char NumStr(unsigned int Value)
{
  switch(Value)
  {
      case 0:
          return '0';
      case 1:
          return '1';
      case 2: 
          return '2';
      case 3:
          return '3';
      case 4:
          return '4';
      case 5:
          return '5';
      case 6:
          return '6';
      case 7:
          return '7';
      case 8:
          return '8';
      case 9:
          return '9';
      default:
          return '0';
  }
}

void __interrupt(low_priority) low_isr(void){
    INTCONbits.GIEH = 0;
    if(PIR1bits.TXIF){
        PIR1bits.TXIF=0;
    }
    INTCONbits.GIEH = 1;
}


void main(void) {
    bool V_usb;
    unsigned int V_Bat1;
    unsigned int V_Bat2;
    
    __delay_ms(10);

    while(1){
        if(MedUsb())
        {
        //MedVoltageBat_1();
        //MedVoltageBat_2();
            CE1 = 0;
            CE2 = 0;
            S1 = 1;
            S2 = 1;
            S3 = 0;
            __delay_ms(5);
        
            if((Abs(MedVoltageBat_1()  -  MedVoltageBat_1())> 21) && MedUsb()) // 21 == 0.102 V
            {
                while((Abs(MedVoltageBat_1()  -  MedVoltageBat_1())> 18) && MedUsb()) // 18 == 0.087
                {
                    //Isolate battery 1 from 2
                    
                    S1 = 0;
                    S2 = 0;
                    S3 = 0;
                    __delay_ms(5);
                    //Start charge in both chargers
                    
                    CE1 = 1;
                    CE2 = 1;
                    __delay_ms(5);
                }
            }
            
            if(MedUsb() && ( (MedVoltageBat_1() >= 819 ) || ( MedVoltageBat_2() >= 819 ) ) && (Abs(MedVoltageBat_1()  -  MedVoltageBat_1())< 21) )
            { // 819 = 3.999
                while(MedUsb() && ( (MedVoltageBat_1() >= 798 ) || ( MedVoltageBat_2() >= 798 ) ) && (Abs(MedVoltageBat_1()  -  MedVoltageBat_1())< 41) )
                { //798 = 3.89
                    //Disable charge
                    CE1 = 0;
                    CE2 = 0;
                    __delay_ms(5);
                    //Set batteries in parallel
                    //Remove batteries from chargers
                    S1 = 1;
                    S2 = 1;
                    S3 = 1;
                    __delay_ms(5);
                }
            }
            if(MedUsb() && ( (MedVoltageBat_1() < 819 ) || ( MedVoltageBat_2() < 819 ) ) && (Abs(MedVoltageBat_1()  -  MedVoltageBat_1())> 21) )
            { // 819 = 3.999
                while(MedUsb() && ( (MedVoltageBat_1() < 839 ) || ( MedVoltageBat_2() < 839 ) ) && (Abs(MedVoltageBat_1()  -  MedVoltageBat_1())< 41) )
                { //798 = 3.89
                    //enable charger 1
                    CE1 = 1;
                    CE2 = 0;
                    __delay_ms(5);
                    //Set batteries in parallel
                    //Remove batteries from charger 2
                    S1 = 0;
                    S2 = 0;
                    S3 = 1;
                    __delay_ms(5);
                }
            }
            
        }
        else 
        {
           if((Abs(MedVoltageBat_1()  -  MedVoltageBat_1())> 41) && ~MedUsb()) // 41==0.2 
           {
               while((Abs(MedVoltageBat_1()  -  MedVoltageBat_1())> 21) && ~MedUsb())
               {
                   //isolate batteries
                   CE1 = 0 ;
                   CE2 = 0;
                   S1 = 1;
                   S2 = 1;
                   S3 = 0;
                   __delay_ms(5);
                   
                   
               }
           }
           if((Abs(MedVoltageBat_1()  -  MedVoltageBat_1())<= 41) && ~MedUsb()) // 41==0.2 
           {
               while((Abs(MedVoltageBat_1()  -  MedVoltageBat_1())< 61) && ~MedUsb())
               {//61 = 0.3
                   //set batteries in parallel
                   CE1 = 0 ;
                   CE2 = 0;
                   S1 = 1;
                   S2 = 1;
                   S3 = 1;
                   __delay_ms(5);
               }
           }
        }
        CE1 = 0;
        CE2 = 0;
    }

    return;
}


