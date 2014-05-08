    #include "custom_uart.h"
	#include"setup.h"
	#include "debug.h"
/*timeout*/

_SW_UART_STATUS_t sw_uart_status;

//EUSART1
/* sends a string n-number bytes*/
char sci_PutStrn1(ROM char *str,UINT16  n)
{
	while(n--)
		{
			while(!PIR1bits.TX1IF) ;
			PIR1bits.TX1IF=0;
			TXREG1 = *(str++);	
		}
	return 0;
}

char sci_PutStrn2(ROM char *str,UINT16 n)
{
	while(n--)
		{
			while(!PIR3bits.TX2IF) ;
			PIR3bits.TX2IF=0;
			TXREG2 = *(str++);	
		}
	return 0;
}

void sci_PutStr1(ROM char *str)	
	{
		while(*(str) != '\0' )
		{
			while(!PIR1bits.TX1IF) ;
			PIR1bits.TX1IF=0;
			TXREG1 = *(str++);	
		}
	}
	
void sci_PutStr2(ROM char *str)	
	{
		while(*(str) != '\0' )
		{
			while(!PIR3bits.TX2IF) ;
			PIR3bits.TX2IF=0;
			TXREG2 = *(str++);	
		}
	}
	
void sci_PutChar1( char c)
{
	while(!PIR1bits.TX1IF) ;
	PIR1bits.TX1IF=0;
	TXREG1 = c;	
}

void sci_PutChar2( char c)
{
	while(!PIR3bits.TX2IF) ;
	PIR3bits.TX2IF = 0;
	TXREG2 = c;	
}

INT16 sci_GetChar1(UINT16 to)
{
	timer0(to); // 100 msec for symbol
    while(!timer0_tout() && !byteReceived1());
	if(!byteReceived1()) 	return -1;				

    if ( RCSTA1bits.OERR ) /*error occurs*/
	{
        flushUart1();
		return -2;
	}

	return RCREG1;	
}

UINT8 sci_WaitSymbol1(UINT16 to)
{
		timer0(to); // 100 msec for symbol
    while(timer0_tout() && !byteReceived1());
        return byteReceived1()?1:0; 
}
UINT8 sci_WaitSymbol2(UINT16 to)
{
		timer0(to); // 100 msec for symbol
    while(!timer0_tout() && !byteReceived2());
    return byteReceived2()?1:0; 

    
}	
INT16 sci_GetChar2(UINT16 to )
{
	timer0(to); // 100 msec for symbol
    while(!timer0_tout() && !byteReceived2());
	if(!byteReceived2()) 
			return -1;		

    if ( RCSTA2bits.OERR ||RCSTA2bits.FERR) /*error occurs*/
	{
        flushUart2();
		return -2;
	}

	return RCREG2;	
}
UINT16  sci_GetStr1(char *buf)
{
	UINT16 c = 0;
	UINT16 maxlength =MAX_LENGTH;
	while(maxlength--){

		timer0(100); // 100sec for symbol
		while(timer0_tout() && !byteReceived1());

		if(!byteReceived1() ) break;
		if ( RCSTA1bits.OERR ||RCSTA1bits.FERR ) /*error occurs*/
		{
			flushUart1();
			return 0;
		}

			*buf++ = RCREG1;
			 c++;
	}

	return c;
}

UINT16  sci_GetStr2(char *buf)
{
	UINT16 c = 0;
	UINT16 maxlength =MAX_LENGTH;
	while(maxlength--){

		timer0(50); // 50 msec for symbol
		while(timer0_tout() && !byteReceived2());

		if(!byteReceived2() ) break;
		if ( RCSTA2bits.OERR ) /*error occurs*/
		{
			flushUart2();
			return 0;
		}

			*buf++ = RCREG2;
			 c++;
	}

	return c;
}
UINT16  sci_GetLine1(char *buf)
{
	UINT8 c = 0;
	UINT16 maxlength =MAX_LENGTH;
	
	while(maxlength--)
		{
		timer0(100); // 50 msec for symbol
		while(timer0_tout() && !byteReceived1());

		if(!byteReceived1() ) break;
		if ( RCSTA1bits.OERR ) /*error occurs*/
		{
			flushUart1();
			return 0;
		}
			*buf = RCREG1;
			c++;
			if(*(buf-1) == '\r' && *(buf)=='\n')	break;
			buf++;			 
	}

	return c;
}


UINT16  sci_GetLine2(char *buf)
{
	UINT8 c = 0;
	UINT16 maxlength =MAX_LENGTH;
	
	while(maxlength--)
		{
		timer0(100); // 50 msec for symbol
		while(timer0_tout() && !byteReceived2());

		if(!byteReceived2() ) break;
		if ( RCSTA2bits.OERR ) /*error occurs*/
		{
			flushUart2();
			return 0;
		}
			*buf = RCREG2;
			c++;
			if(*(buf-1) == '\r' && *(buf)=='\n')	break;
			buf++;			 
	}

	return c;
}



void  flushUart1(void) {
	if(PIR1bits.RC1IF) { 
		RCSTA1bits.CREN = 0;    
        RCREG1;                 
        RCREG1;                 
        RCSTA1bits.CREN = 1;
        PIR1bits.RC1IF=0;
	}
}                    
void  flushUart2(void)
{   
	if( PIR3bits.RC2IF) { 
		RCSTA2bits.CREN = 0;    
        RCREG2;                 
        RCREG2;                
        RCSTA2bits.CREN = 1;
    	PIR3bits.RC2IF=0;
	}
}

void Uart1On(UINT8 on)
{

	if(on)
	{
		UART1RX_TRIS =1;
		UART1TX_TRIS =1;
		RCSTA1bits.SPEN=1;
	}
	else
	{
		UART1RX_TRIS =0;
		UART1TX_TRIS =0;
		UART1RX_LAT =0;
		UART1TX_LAT =0;
		RCSTA1bits.SPEN=0;

	}
}


void swuart_init(void)
{
	SW_UART_TRIS_TX = 0; //out
	//SW_UART_TRIS_RX = 1; //in
	SW_UART_TX = 0; //set to 0
}	
void swuart_putchar(char c)
{

	UINT8 len;
	swuart_logical1();
	SW_UART_WaitSync();
	SW_UART_WaitSync();
	SW_UART_WaitSync();
	swuart_logical0();//start
	
	len=8;
	while(len--)
	{
		SW_UART_WaitSync();	
		if(c&1) swuart_logical1();
		else swuart_logical0();
		c>>=1;
	}
			
	#ifdef SWUART_STOPBIT1
		SW_UART_WaitSync();
		swuart_logical1();//stop		
	#elif SWUART_STOPBIT2
		SW_UART_WaitSync();
		swuart_logical1();//stop
		SW_UART_WaitSync();
		swuart_logical1();//stop
	#endif
	SW_UART_WaitSync();
	SW_UART_WaitSync();

}	

void swuart_puts(const char * str)
{
	while(*str) swuart_putchar(*str++);
}	

void swuart_putsn(const char * str,unsigned char len)
{	
	while(len--) swuart_putchar(*str++);
}