char UART2_Init(const long int baudrate) {
    unsigned int x;
    x = (_XTAL_FREQ - baudrate * 64) / (baudrate * 64);
    if (x > 255) {
        x = (_XTAL_FREQ - baudrate * 16) / (baudrate * 16);
        TX2STAbits.BRGH = 1;
    }
    if (x < 256) {
        ANSELG = 0;
        SP2BRGL = x;
        TX2STAbits.SYNC = 0;
        RC2STAbits.SPEN = 1;
        TRISG2 = 1;
        TRISG1 = 0;
        RC2STAbits.CREN = 1;
        TX2STAbits.TXEN = 1;
        return 1;
    }
    return 0;
}

char UART2_TX_Empty() {
    return TX2STAbits.TRMT;
}

char UART2_Data_Ready() {
    return PIR4bits.RC2IF;
}

char UART2_Read() {
    if (RC2STAbits.OERR) {
        RC2STAbits.CREN = 0;
        NOP();
        RC2STAbits.CREN = 1;
        //RC2STAbits.SPEN =0;
        //RC2STAbits.SPEN=1;
    }
   while (!PIR4bits.RC2IF);
    return RC2REG;
}

void UART2_Read_Text(char *Output, unsigned int length) {
    unsigned int i;
    for (i = 0; i < length; i++)
        Output[i] = UART2_Read();


}

void UART2_Write(char data) {
    while (!TX2STAbits.TRMT);
    TX2REG = data;
}

void UART2_Write_Text(char *text) {
    int i;
    for (i = 0; text[i] != '\0'; i++)
        UART2_Write(text[i]);
}
void UART2_Write_Text_Phan_Tu_Thu_7(char *text) {
    int i;
    for (i = 0; i<7; i++)
        UART2_Write(text[i]);
}

void UART2_Write_Text_Phan_Tu_Thu_8(char *text)
{
  int i;
  for(i=0;i<8;i++)
	  UART2_Write(text[i]);
}

void UART2_Write_Text_Phan_Tu_Thu_13(char *text)
{
  int i;
  for(i=0;i<13;i++)
	  UART2_Write(text[i]);
}


void UART2_Write_Text_2(char* str) {
    while (*str) {

        UART2_Write(*str++);
    }
}

void UART2_Write_Text_Length(char* str, int length) {
    volatile int count = 0;
    do {
        UART2_Write(*str++);
        count++;
        if ((*str == 0x00)&&(count == length)) {
            count = 0;
            break;
        }

    } while (1);

}