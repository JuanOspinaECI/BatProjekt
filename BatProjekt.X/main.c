/*
 * File:   main.c
 * Author: 
 *
 * Created on October 16, 2023, 2:50 PM
 */

// PIC18F2220 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = INTIO1     // Oscillator Selection bits (Internal RC oscillator, CLKO function on RA6 and port function on RA7)
#pragma config FSCM = ON        // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON        // Internal/External Switchover bit (Internal/External Switchover mode enabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset enable bit (Brown-out Reset enabled)
#pragma config BORV = 20        // Brown-out Reset Voltage bits (VBOR set to 2.0V)

// CONFIG2H
#pragma config WDT = ON         // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBAD = ANA       // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
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

#define OSCC_8MHZ_Primary 01111100
    // Bit de configuración de reloj 
    //bit 7     =   IDLEN 0
    //bit 6-4   =   IRFC2:IRCF0 (111 8MHZ)
    //bit 3     =   OSTS 1 
    //bit 2     =   IOFS 1
    //bit 1-0     =   SCS1:SCS0 00 Primary oscillator
#define Analog_ch1_ConvOff 0000000
    //Analog config
    //bit 5-6, for channel 0 = 000
    //         for channel 1 = 001   
    // bit 1 -> lectura de estado de conversion
    // bit 0 -> 1 para habilitar conversion
#define Analog_ch1_ConvOn 0000001

#define AN_VDD_VSS_AN1_AN0 00001101
    // bit 3-0 1101 -> habitlar analogo los canales AN0 y 1 de resto digital
    // VSS ref
    // VDD 

#define RightJus_Acq12TAD_ConvFOS16 10101101
void Setup (void) {

    OSCCON = OSCC_8MHZ_Primary;
    
    //Configuracion Puerto A
    // Todos como entradas 
    // RA2 entrada = 1 (Led prueba)
    TRISA = 11111011;
    
    ADCON0 = Analog_ch1_ConvOff;
    
    ADCON1 = AN_VDD_VSS_AN1_AN0;
    
    ADCON2 = RightJus_Acq12TAD_ConvFOS16; //Revisar por si no funciona el ADC
    
    TRISB = 11100000;
    
    //Enable  USART bit 7 as 1 bit 6 as 0
    // Enable Ic2 bit 3-4 as 11 input
    TRISC = 10011000;
    
    
    //USART
    RCSTA = 10000000;
    TXSTA = 00100010; // configurar como asincrono uart
    
    //I2C
    //bit 3-0 1000 for master mode
    SSPCON1 = 00101000;
    //Poner bit 0 en 1 para iniciar el i2c 
    SSPCON2 = 01011101;
    
    SSPSTAT = 10001000;
    
    
}
unsigned int MedVoltageBat_1()
{
    // Funcion para leer el valor ADC de BAT 1
    // Convertir a dato int 
    return 1;
}

unsigned int MedVoltageBat_2()
{
    // Funcion para leer el valor ADC de BAT 1
    // Convertir a dato int 
    return 1;
}

int Abs(int ValueA)
{
    if (ValueA >= 0) return ValueA;
    else return -ValueA;
}

void main(void) {
    char V_usb;
    //unsigned int V_Bat1;
    //unsigned int V_Bat2;
    Setup();
    PORTAbits.RA2 = 1; 
    PORTCbits.RC0 = 1;
    
    V_usb = 0; // Definir como medir si USB conectado (fuente)
    if(V_usb) //medicion de cargador  no conectado 
    {
        PORTBbits.RB0 = 0; //Rele 1 BAT1 con BAT1_MCP
        PORTBbits.RB1 = 0; //Rele 2 BAT2 con BAT2_MCP
        if(MedVoltageBat_1() >= 4.0 && MedVoltageBat_1() >= 4.0 )
        {
            while(Abs(MedVoltageBat_1()  -  MedVoltageBat_1())>0.01)
            {
                if(V_usb == 0)break;
                //Cargar bateria que este con menor nivel de carga
            }
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


