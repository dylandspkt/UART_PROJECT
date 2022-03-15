
// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#define _XTAL_FREQ  4000000
#define index_frame_6byte 6
#define index_frame_11byte  11
#define index_frame_5byte  5
#define index_crc   2
#define index_convert 2
#define index_data 8
#define index_dataread 8
/**/
#define _8byte_number_reg 8
#define _2byte_number_reg 2

#include <xc.h>  
#include <stdio.h>
#include "uarthex.h"                                                                                     
#include "crcLIBhex.h"
#include "reflect_byte.h"


//unsigned char data_response[] = {0x06, 0x03, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char data_response_13byte[] = {0x06, 0x03, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char data_response_7byte[] = {0x06, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char data_response_8byte[] = {0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
/*******Reflect Frame******/

//    input_ref[index_frame]=reflect_byte(input,input_ref,index_frame);
//    /*****Tinh CRC*****/
//    for (l = 0; l < index_frame; l++) {
//        txt[l] = input_ref[index_frame - 1 - l];
//    }
//
//    crc = crc16(txt, index_frame);
//    output[0] = crc & 0xff;
//    output[1] = (crc >> 8)&0xff;
//
//
//    /**********Reflect CRC**********/
//    output_ref[index_crc]=reflect_byte(output,output_ref,index_crc);
//    /*************UART**************/

///*                           */

void EEPROM_Write(int address, char data) {
    /*Write Operation*/
    EEADR = address; /* Write address to the EEADR register*/
    EEDATA = data; /* Copy data to the EEDATA register for write */
    EECON1bits.EEPGD = 0; /* Access data EEPROM memory*/
    EECON1bits.WREN = 1; /* Allow write to the memory*/
    INTCONbits.GIE = 0; /* Disable global interrupt*/

    /* Below sequence in EECON2 Register is necessary 
    to write data to EEPROM memory*/
    EECON2 = 0x55;
    EECON2 = 0xaa;

    EECON1bits.WR = 1; /* Start writing data to EEPROM memory*/
    INTCONbits.GIE = 1; /* Enable interrupt*/

    while (PIR2bits.EEIF == 0); /* Wait for write operation complete */
    PIR2bits.EEIF = 0; /* Reset EEIF for further write operation */

}

void EEPROM_WriteString(int address, char *data) {
    /*Write Operation for String*/
    while (*data != '\0') {
        EEPROM_Write(address, *data);
        address++;
        *data++;
    }
}

char EEPROM_Read(int address) {
    /*Read operation*/
    EEADR = address; /* Read data at location 0x00*/
    EECON1bits.WREN = 0; /* WREN bit is clear for Read operation*/
    EECON1bits.EEPGD = 0; /* Access data EEPROM memory*/
    EECON1bits.RD = 1; /* To Read data of EEPROM memory set RD=1*/
    return (EEDATA);
}

/*                   */

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

/**/
void CONVERT_Data_6Byte(char *input, unsigned char *output_ref) {
    /*******Reflect Frame******/
    unsigned char input_ref[index_frame_6byte] = "";
    unsigned char txt[index_frame_6byte] = "";
    unsigned int crc;
    unsigned char output[index_crc] = "";
    reflect_byte(input, input_ref, index_frame_6byte);
    // UART_Write_Text(input_ref);
    /*****Tinh CRC*****/

    for (unsigned int i = 0; i < index_frame_6byte; i++) {
        txt[i] = input_ref[index_frame_6byte - 1 - i];
    }

    crc = crc16(txt, index_frame_6byte);
    output[0] = crc & 0xff;
    output[1] = (crc >> 8)&0xff;
    //UART_Write_Text(output);

    /**********Reflect CRC**********/
    reflect_byte(output, output_ref, index_crc);
    //UART_Write_Text(output_ref);

}

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

/**/


void __interrupt() ISR() {
    unsigned char crc_code_read[index_convert] = "";
    unsigned char crc_code_response[index_convert] = "";
    unsigned char data[index_data] = "";
    unsigned char i = 0;
    unsigned int address;
    if (PIR1bits.RCIF) {
        if (UART_Data_Ready) {
            UART_Read_Text(data, 8);
            CONVERT_Data_6Byte(data, crc_code_read);

            address = data[2];
            address = (address << 8) | data[3];

            if ((data[index_data - 2] == crc_code_read[index_convert - 2])&&(data[index_data - 1] == crc_code_read[index_convert - 1])) {
                if (data[1] == 0x03) {
                    switch (address) {
                        case 0x0000: //8byte data
                            for (i = 0; i < _8byte_number_reg; i++) {
                                data_response_13byte[3 + i] = EEPROM_Read(i);
                            }
                            CONVERT_Data_11Byte(data_response_13byte, crc_code_response);
                            data_response_13byte[11] = crc_code_response[0];
                            data_response_13byte[12] = crc_code_response[1];
                            if (UART_TX_Empty())
                                UART_Write_Text_Length(data_response_13byte, 13);
                            break;
                        case 0x1006:
                            for (i = 0; i < _2byte_number_reg; i++) {
                                data_response_7byte[3 + i] = EEPROM_Read(1006 + i);
                            }
                            CONVERT_Data_5Byte(data_response_7byte, crc_code_response);
                            data_response_7byte[5] = crc_code_response[0];
                            data_response_7byte[6] = crc_code_response[1];
                            if (UART_TX_Empty())
                                UART_Write_Text_Length(data_response_7byte, 7);
                            break;
                        case 0x1008:
                            for (i = 0; i < _2byte_number_reg; i++) {
                                data_response_7byte[3 + i] = EEPROM_Read(1008 + i);
                            }
                            CONVERT_Data_5Byte(data_response_7byte, crc_code_response);
                            data_response_7byte[5] = crc_code_response[0];
                            data_response_7byte[6] = crc_code_response[1];
                            if (UART_TX_Empty())
                                UART_Write_Text_Length(data_response_7byte, 7);

                            break;
                        case 0x1010:

                            for (i = 0; i < _2byte_number_reg; i++) {
                                data_response_7byte[3 + i] = EEPROM_Read(1010 + i);
                            }
                            CONVERT_Data_5Byte(data_response_7byte, crc_code_response);
                            data_response_7byte[5] = crc_code_response[0];
                            data_response_7byte[6] = crc_code_response[1];
                            if (UART_TX_Empty())
                                UART_Write_Text_Length(data_response_7byte, 7);

                            break;
                        case 0x2002:
                            for (i = 0; i < _2byte_number_reg; i++) {
                                data_response_7byte[3 + i] = EEPROM_Read(2002 + i);
                            }
                            CONVERT_Data_5Byte(data_response_7byte, crc_code_response);
                            data_response_7byte[5] = crc_code_response[0];
                            data_response_7byte[6] = crc_code_response[1];
                            if (UART_TX_Empty())
                                UART_Write_Text_Length(data_response_7byte, 7);

                            break;

                        default:
                            break;
                    }
                } else {
                    switch (address) {
                        case 0x1000:
                            for (i = 0; i < _2byte_number_reg; i++) {
                                EEPROM_Write(1000 + i, data[4 + i]);
                            }
                            data_response_8byte[3] = address;
                            data_response_8byte[2] = (char) (address >> 8);
                            for (i = 0; i < _2byte_number_reg; i++) {
                                data_response_8byte[4 + i] = EEPROM_Read(1000 + i);

                            }
                            CONVERT_Data_6Byte(data_response_8byte, crc_code_response);
                            data_response_8byte[6] = crc_code_response[0];
                            data_response_8byte[7] = crc_code_response[1];
                            if (UART_TX_Empty())
                                UART_Write_Text_Length(data_response_8byte, 8);

                            break;

                        case 0x1004:
                            for (i = 0; i < _2byte_number_reg; i++) {
                                EEPROM_Write(1004 + i, data[4 + i]);
                            }
                            data_response_8byte[3] = address;
                            data_response_8byte[2] = (char) (address >> 8);
                            for (i = 0; i < _2byte_number_reg; i++) {
                                data_response_8byte[4 + i] = EEPROM_Read(1004 + i);

                            }
                            CONVERT_Data_6Byte(data_response_8byte, crc_code_response);
                            data_response_8byte[6] = crc_code_response[0];
                            data_response_8byte[7] = crc_code_response[1];
                            if (UART_TX_Empty())
                                UART_Write_Text_Length(data_response_8byte, 8);

                            break;
                        case 0x1010:
                            for (i = 0; i < _2byte_number_reg; i++) {
                                EEPROM_Write(1010 + i, data[4 + i]);
                            }
                            data_response_8byte[3] = address;
                            data_response_8byte[2] = (char) (address >> 8);
                            for (i = 0; i < _2byte_number_reg; i++) {
                                data_response_8byte[4 + i] = EEPROM_Read(1010 + i);

                            }
                            CONVERT_Data_6Byte(data_response_8byte, crc_code_response);
                            data_response_8byte[6] = crc_code_response[0];
                            data_response_8byte[7] = crc_code_response[1];
                            if (UART_TX_Empty())
                                UART_Write_Text_Length(data_response_8byte, 8);

                            break;
                        case 0x2002:
                            for (i = 0; i < _2byte_number_reg; i++) {
                                EEPROM_Write(2002 + i, data[4 + i]);
                            }
                            data_response_8byte[0] = data[5];
                            data_response_8byte[3] = address;
                            data_response_8byte[2] = (char) (address >> 8);
                            for (i = 0; i < _2byte_number_reg; i++) {
                                data_response_8byte[4 + i] = EEPROM_Read(2002 + i);

                            }
                            CONVERT_Data_6Byte(data_response_8byte, crc_code_response);
                            data_response_8byte[6] = crc_code_response[0];
                            data_response_8byte[7] = crc_code_response[1];
                            if (UART_TX_Empty())
                                UART_Write_Text_Length(data_response_8byte, 8);
                            data_response_8byte[0] = 0x06;

                            break;
                        case 0x2020:
                            for (i = 0; i < _2byte_number_reg; i++) {
                                EEPROM_Write(2020 + i, data[4 + i]);
                            }

                            data_response_8byte[3] = address;
                            data_response_8byte[2] = (char) (address >> 8);
                            for (i = 0; i < _2byte_number_reg; i++) {
                                data_response_8byte[4 + i] = EEPROM_Read(2020 + i);

                            }
                            CONVERT_Data_6Byte(data_response_8byte, crc_code_response);
                            data_response_8byte[6] = crc_code_response[0];
                            data_response_8byte[7] = crc_code_response[1];
                            if (UART_TX_Empty())
                                UART_Write_Text_Length(data_response_8byte, 8);
                            break;
                        default:
                            break;
                    }
                }

            }
        }
    }
}

inline static void config_interrupt() {

    INTCON = 0xC0;
    PIE1bits.RCIE = 1;
}

void main() {



    /************READ ADDRESS REGISTER**************/
    /*                     */
    EEPROM_Write(0000, 0x01);
    EEPROM_Write(0001, 0x02);
    EEPROM_Write(0002, 0x00);
    EEPROM_Write(0003, 0x01);
    EEPROM_Write(0004, 0x00);
    EEPROM_Write(0005, 0xB0);
    EEPROM_Write(0006, 0x00);
    EEPROM_Write(0007, 0x01);
    /*                      */
    EEPROM_Write(1006, 0x00);
    EEPROM_Write(1007, 0x05);
    /*                     */
    EEPROM_Write(1008, 0x04);
    EEPROM_Write(1009, 0x12);
    /*              */
    EEPROM_Write(1010, 0x06);
    EEPROM_Write(1011, 0x00);
    /*                 */
    EEPROM_Write(2002, 0x02);
    EEPROM_Write(2003, 0x40);

    /*****WRITE ADDRESS REGISTER****/
    EEPROM_Write(1000, 0x40);
    EEPROM_Write(1001, 0x40);
    /**/
    EEPROM_Write(1004, 0x05);
    EEPROM_Write(1005, 0x40);
    /**/
    EEPROM_Write(2020, 0x00);
    EEPROM_Write(2021, 0x25);

    UART_Init(9600);
    config_interrupt();

    while (1) {

    }

}



