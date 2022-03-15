
//#include "uart.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>
char UART_Init(const long int baudrate) {
    unsigned int x;
    x = (_XTAL_FREQ - baudrate * 64) / (baudrate * 64);
    if (x > 255) {
        x = (_XTAL_FREQ - baudrate * 16) / (baudrate * 16);
        TXSTAbits.BRGH = 1;
    }
    if (x < 256) {
        SPBRG = x;
        TXSTAbits.SYNC = 0;
        RCSTAbits.SPEN = 1;
        TRISC7 = 1;
        TRISC6 = 0;
        RCSTAbits.CREN = 1;
        TXSTAbits.TXEN = 1;
        return 1;
    }
    return 0;
}

char UART_TX_Empty() {
    return TXSTAbits.TRMT;
}

char UART_Data_Ready() {
    return PIR1bits.RCIF;
}

char UART_Read() {
    if (RCSTAbits.OERR) {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
    while (!PIR1bits.RCIF);
    return RCREG;
}

void UART_Write(char datau) {
    while (!TXSTAbits.TRMT);
    TXREG = datau;
}


void UART_Read_Text(char *Output, unsigned int length) {
    unsigned int i;
    for (i = 0; i < length; i++)
        Output[i] = UART_Read();

}



void UART_Write_Text(char *text) {
    int i;
    for (i = 0; *(text+i) != '\0'; i++)
        UART_Write(*(text+i));
}

void UART_Write_Text_2(char* str) {
    while (*str) {

        UART_Write(*str++);
    }
}

void  UART_Write_Text_Length (char* str, int length )
{
    int count=0;
    do {
        UART_Write(*str++);
        count++;
        if((*str=='\0')&&(count == length ))
        {
            count=0;
            break;
        }
    }
    while(1);
    
}