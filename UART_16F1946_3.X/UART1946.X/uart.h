char UART_Init(const long int baudrate)
{
	unsigned int x;
	x = (_XTAL_FREQ - baudrate*64)/(baudrate*64);
	if(x>255)
	{
		x = (_XTAL_FREQ - baudrate*16)/(baudrate*16);
		TX1STAbits.BRGH = 1;
	}
	if(x<256)
	{
	  SP1BRGL = x;
	  TX1STAbits.SYNC = 0;
	  RC1STAbits.SPEN = 1;
          TRISC7 = 1;
          TRISC6 = 0;
          RC1STAbits.CREN = 1;
          TX1STAbits.TXEN = 1;
	  return 1;
	}
	return 0;
}

char UART_TX_Empty()
{
  return TX1STAbits.TRMT;
}

char UART_Data_Ready()
{
   return PIR1bits.RCIF;
}
char UART_Read() 
{
    if(RC1STAbits.OERR)
    {
        RC1STAbits.CREN=0;
        NOP();
        RC1STAbits.CREN=1;
    }
  while(!PIR1bits.RCIF);
  return RC1REG;
}

void UART_Read_Text(char *Output, unsigned int length)
{
	unsigned int i;
	for(int i=0;i<length;i++)
		Output[i] = UART_Read();
    
    
}

void UART_Write(char data)
{
  while(!TX1STAbits.TRMT);
  TX1REG = data;
}

void UART_Write_Text(char *text)
{
  int i;
  for(i=0;text[i]!='\0';i++)
	  UART_Write(text[i]);
}



void  UART_Write_Text_Length (char* str, int length )
{
    volatile int count=0;
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