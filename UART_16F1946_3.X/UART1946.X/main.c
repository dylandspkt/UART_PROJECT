
// PIC16F1946 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled) 
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable (VCAP pin functionality is disabled)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 16000000
#define index_frame 6
#define index_crc   2
#define index_convert 8
#define index_data 6
#define index_frame_5byte 5
#define index_frame_11byte 11
#include "config.h"
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "uart.h"
#include "uart2.h"
#include "crcLIBhex.h"
#include "reflect_byte.h"
#include "variable.h"

unsigned char address_data;

/***********Convert DaTa*************/
void CONVERT_Data_6Byte(char *input, unsigned char *output_ref) {
    /*******Reflect Frame******/
    unsigned char input_ref[index_frame] = "";
    unsigned char txt[index_frame] = "";
    unsigned int crc;
    unsigned char output[index_crc] = "";
    reflect_byte(input, input_ref, index_frame);
    // UART_Write_Text(input_ref);
    /*****Tinh CRC*****/

    for (unsigned int i = 0; i < index_frame; i++) {
        txt[i] = input_ref[index_frame - 1 - i];
    }

    crc = crc16(txt, index_frame);
    output[0] = crc & 0xff;
    output[1] = (crc >> 8)&0xff;
    //UART_Write_Text(output);

    /**********Reflect CRC**********/
    reflect_byte(output, output_ref, index_crc);
    //UART_Write_Text(output_ref);

}

/******************/
void CONVERT_Data_1(char *input, unsigned char *output_ref) {
    /*******Reflect Frame******/
    unsigned char input_ref[index_frame] = "";
    unsigned char txt[index_frame] = "";
    unsigned int crc;
    unsigned char output[index_crc] = "";
    reflect_byte(input, input_ref, index_frame);

    /*****Tinh CRC*****/

    for (unsigned int i = 0; i < index_frame; i++) {
        txt[i] = input_ref[index_frame - 1 - i];
    }

    crc = crc16(txt, index_frame);
    output[0] = crc & 0xff;
    output[1] = (crc >> 8)&0xff;


    /**********Reflect CRC**********/
    reflect_byte(output, output_ref, index_crc);


}

/**************************************/

void CONVERT_Data_5Byte(char *input, unsigned char *output_ref) {
    /*******Reflect Frame******/
    unsigned char input_ref[index_frame_5byte] = "";
    unsigned char txt[index_frame_5byte] = "";
    unsigned int crc;
    unsigned char output[index_crc] = "";
    reflect_byte(input, input_ref, index_frame_5byte);
    //UART_Write_Text(input_ref);
    /*****Tinh CRC*****/

    for (unsigned int i = 0; i < index_frame_5byte; i++) {
        txt[i] = input_ref[index_frame_5byte - 1 - i];
    }

    crc = crc16(txt, index_frame_5byte);
    output[0] = crc & 0xff;
    output[1] = (crc >> 8)&0xff;
    //UART_Write_Text(output);

    /**********Reflect CRC**********/
    reflect_byte(output, output_ref, index_crc);
    //if(UART_TX_Empty())
    // UART_Write_Text(output_ref);

}

/************************************/

void CONVERT_Data_11Byte(char *input, unsigned char *output_ref) {
    /*******Reflect Frame******/
    unsigned char input_ref[index_frame_11byte] = "";
    unsigned char txt[index_frame_11byte] = "";
    unsigned int crc;
    unsigned char output[index_crc] = "";
    reflect_byte(input, input_ref, index_frame_11byte);
    //UART_Write_Text(input_ref);
    /*****Tinh CRC*****/

    for (unsigned int i = 0; i < index_frame_11byte; i++) {
        txt[i] = input_ref[index_frame_11byte - 1 - i];
    }

    crc = crc16(txt, index_frame_11byte);
    output[0] = crc & 0xff;
    output[1] = (crc >> 8)&0xff;
    //UART_Write_Text(output);

    /**********Reflect CRC**********/
    reflect_byte(output, output_ref, index_crc);
    //if(UART_TX_Empty())
    // UART_Write_Text(output_ref);

}

inline static void config_interrupt() {

    INTCON = 0xC0;
    // PIE4bits.RC2IE = 1;
    PIE1bits.RCIE = 1;
}


/////////*************************//////////////////////

void __interrupt() ISR() {
    unsigned char data1[13] = "";
    volatile unsigned char crc_code_1[2] = "";
    volatile unsigned char flag = 0;
    volatile unsigned char len = 0;
    if (PIR1bits.RCIF) {
        if (UART_Data_Ready()) {

            for (int i = 0; i < 13 - len; i++) {
                data1[i] = UART_Read();
                if (data1[2] == 0x08 && data1[1] == 0x03 && flag == 0) {
                    len = 0;
                    flag = 1;
                }
                if (data1[2] == 0x02 && data1[1] == 0x03 && flag == 0) {
                    len = 6;
                    flag = 2;
                }
                if (data1[1] == 0x06 && flag == 0) {
                    len = 5;
                    flag = 3;

                }

            }
            //UART_Read_Text(data1, 7);
            //data1[13] = '\0';
            //UART2_Write_Text_Phan_Tu_Thu_13(data1);
            if (data1[1] == 0x03) {

                switch (data1[2]) {
                    case 0x08:

                        CONVERT_Data_11Byte(data1, crc_code_1);

                        data1[13] = '\0';
                        //UART2_Write_Text_Phan_Tu_Thu_13(data1);

                        if ((data1[11] == crc_code_1[0]) && (data1[12] == crc_code_1[1])) {
                            //if (UART2_TX_Empty()) {
                            UART2_Write_Text_Phan_Tu_Thu_13(data1);
                            UART2_Write_Text("OK");
                            //}
                        }
                        break;
                    case 0x02:

                        CONVERT_Data_5Byte(data1, crc_code_1);
                        data1[7] = '\0';
                        if (data1[5] == crc_code_1[0] && data1[6] == crc_code_1[1]) {

                            UART2_Write_Text_Phan_Tu_Thu_7(data1);
                            UART2_Write_Text("OK");

                        }
                        break;


                    default:
                        break;

                }

            }

            if (data1[1] == 0x06) {


                CONVERT_Data_1(data1, crc_code_1);
                data1[8] = '\0';

                if ((data1[6] == crc_code_1[0]) && (data1[7] == crc_code_1[1])) {


                    UART2_Write_Text_Phan_Tu_Thu_8(data1);
                    UART2_Write_Text("OK");



                }

            }

        }

    }

}

void main() {


    OSCCON = 0b01111010;
    unsigned char temp[1] = "";
    unsigned int i, j = 0;

    UART_Init(9600);
    UART2_Init(9600);
    config_interrupt();
    while (1) {

        unsigned char data[8] = "";
        unsigned char crc_code[2] = "";
        if (UART2_Data_Ready()) {
            UART2_Read_Text(data, 6);
            CONVERT_Data_1(data, crc_code);
            data[6] = crc_code[0];
            data[7] = crc_code[1];
            data[8] = '\0';

            // address_data = data[1];
            if (UART_TX_Empty()) {
                UART_Write_Text_Length(data, 8);

            }
        }




        //    while (1) {
        //        if (UART2_Data_Ready()) {
        //            UART2_Read_Text(data,8 );
        //            data[8]='\0';
        //            if (UART_TX_Empty()) {
        //                UART_Write_Text_Length(data, 8);
        //            }
        //
        //
        //        }
        //    }









    }

}
