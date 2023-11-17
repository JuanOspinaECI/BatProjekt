/*
 * File:   main.c
 * Author: 
 *
 * Created on October 16, 2023, 2:50 PM
 */

// PIC18F2220 Configuration Bit Settings

// 'C' source line config statements



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
    
    //Configuracion Puerto A
    // Todos como entradas 
    // RA2 entrada = 1 (Led prueba)
    TRISA = 0b11110011;
    
    ADCON0 = Analog_ch1_ConvOff;
    
    ADCON1 = AN_VDD_VSS_AN1_AN0;
    
    ADCON2 = RightJus_Acq12TAD_ConvFOS16; //Revisar por si no funciona el ADC
    
    TRISB = 0b11100000;
    
    //Enable  USART bit 7 as 1 bit 6 as 0
    // Enable Ic2 bit 3-4 as 11 input
    TRISC = 0b10011000;
    
    
    //USART
    RCSTA = 0b10000000;
    TXSTA = 0b00100010; // configurar como asincrono uart
    
    //I2C
    //bit 3-0 1000 for master mode
    SSPCON1 = 0b00101000;
    //Poner bit 0 en 1 para iniciar el i2c 
    SSPCON2 = 0b01011101;
    
    SSPSTAT = 0b10001000;
    
    OSCCONbits.IRCF = 0x07;//0x07;
    OSCCONbits.SCS = 0b11;
    while(OSCCONbits.IOFS!=1)
    RCONbits.IPEN = 1; // enable priorities levels on interrupts
    INTCONbits.GIEH = 1; // Enable interrupts 
    INTCONbits.GIEL = 1; // Enable interrupts 
    
    
}
unsigned int MedVoltageBat_1()
{
    // Funcion para leer el valor ADC de BAT 1
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
    // Funcion para leer el valor ADC de BAT 2
    // Convertir a dato int 
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
    char V_usb;
    //unsigned int V_Bat1;
    //unsigned int V_Bat2;
    Setup();
    PORTAbits.RA2 = 1; 
    //PORTCbits.RC0 = 1;
    /*while (1){
    PORTCbits.RC0 = 1;
    __delay_ms(1000);
    PORTCbits.RC0 = 0;
    __delay_ms(1000);
    }*/
    Timer0IntInit();
    V_usb = 0; // Definir como medir si USB conectado (fuente)    
    uart_init();
    //printf("Hola");
    while(1){
        MedVoltageBat_1();
        MedVoltageBat_2();
        __delay_ms(10);
    }

    
    //test_uart();
   
    if(MedUsb()) //medicion de cargador  no conectado 
    {
        if(Abs(MedVoltageBat_1()  -  MedVoltageBat_1())> 2)
        { // dif mayor  a 0.01 v      
            PORTBbits.RB0 = 0; //Rele 1 BAT1 con BAT1_MCP
            PORTBbits.RB1 = 0; //Rele 2 BAT2 con BAT2_MCP
            __delay_ms(10);
            if(MedVoltageBat_1() <= 819 && MedVoltageBat_1() <= 819 ) //819 = 4.0 v
            {
                //CE enable
                while(Abs(MedVoltageBat_1()  -  MedVoltageBat_1())>0.01)
                {
                    if(MedUsb()== false)break;
                    __delay_ms(10);
                }
                PORTBbits.RB0 = 0; //Rele 1 BAT1 con BAT1_MCP
                PORTBbits.RB1 = 1; //Rele 2 BAT2 con Rele 3
                PORTBbits.RB1 = 0; //Rele 3 Rele 2 con BAT1
                __delay_ms(10);
            }
        }
        else 
        {
            // Enviar mensaje de baterias cargadas
            // CE disable
          PORTBbits.RB0 = 0; //Rele 1 BAT1 con BAT1_MCP
          PORTBbits.RB1 = 1; //Rele 2 BAT2 con Rele 3
          PORTBbits.RB1 = 0; //Rele 3 Rele 2 con BAT1  
        }

    }
    else
    {
        // Configuracion en paralelo de ambas baterias
        // Ambas quedan conectadas con el MCP 1
        PORTBbits.RB0 = 0; //Rele 1 BAT1 con BAT1_MCP
        PORTBbits.RB1 = 1; //Rele 2 BAT2 con Rele 3
        PORTBbits.RB1 = 0; //Rele 3 Rele 2 con BAT1
    }
    
    return;
}


