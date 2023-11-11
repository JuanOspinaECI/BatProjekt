
#include "UART.h"


void __interrupt() high_isr(void);
void __interrupt(low_priority) low_isr(void);
void uart_init(void);

void uart_init(void){
    
    TRISCbits.RC7   =   1;
    TRISCbits.RC6   =   1;
    
    //SPBRGH = (gen_reg & 0xFF00) >> 8;
    SPBRG   =   0x33;//gen_reg & 0x00FF; 59 
    
    RCSTAbits.CREN  =   1;
    RCSTAbits.SPEN  =   1;
    //BAUDCONbits.BRG16 = brg16;
    
    TXSTAbits.SYNC  =   0; //sync;
    TXSTAbits.BRGH  =   1; //brgh;
    TXSTAbits.TXEN  =   1;
    
    IPR1bits.RCIP   =   1;
    PIE1bits.RCIE   =   1;
    

    
   //IPR1bits.TXIP=0;
    //PIE1bits.TXIE=1;   
}
void UART_write(char dato)
{
    TXREG = dato;
     while(TXSTAbits.TRMT == 0);
}

void UART_printf(char *cadena)
{
    while(*cadena !=0x00){
        UART_write(*cadena);
        cadena++;
    }
}

void uart_send(uint8_t *c){
    TXREG=*c;
    while(TXSTAbits.TRMT==0){
        Nop();
    }
}
void putch(char dato)
{
    UART_write(dato);
}

void uart_receiver(uint8_t *c, bool *rx_flag){
    
    if(RCSTAbits.FERR){
        uint8_t er = RCREG;
    }else if(RCSTAbits.OERR){
        RCSTAbits.CREN=0;
        RCSTAbits.CREN=1;
    }else{
        *c = RCREG;
        *rx_flag = true;
    }
    
}

void test_uart(void) {

    
    while(OSCCONbits.IOFS!=1)
    RCONbits.IPEN   = 1; // enable priorities levels on interrupts
    INTCONbits.GIEH = 1; // Enable interrupts 
    INTCONbits.GIEL = 1; // Enable interrupts 
    
    uart_init();
  
    char bufer[21];
    unsigned int a = 10U;
    //sprintf(bufer, "%u", a);
    while(1){
    __delay_ms(1000);
     
    //uart_send(data);
    printf("El entero %u se convierte a '%s' como cadena", a, bufer);
    UART_printf(bufer);
    /* 
       if(got_data_bool)
        {
            uart_send(&data);
            
            got_data_bool = false;
        }*/
    } 
}





