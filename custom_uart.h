#ifndef UART_H
    #define UART_H
    #include "compiler.h"
	#include "delayt.h"

    #define ERROR_NOERROR 	0x00
    #define ERROR_TIMEOUT 	0x02
    #define ERROR_OVERSPEED 0x04


    /* define for TXSTAx***/
    #define _ASYNCH_MODE 0x80
    #define _SYNCH_MODE 0x00
    #define _9_BIT	0x40
    #define _8_BIT	0x00
    #define _TRANSMIT_ENABLE 0x20
    #define _SYNCH_BREAK	0x10
    #define _HIGH_SPEED     0x04
    #define _LOW_SPEED     0x00

    /* define for RCSTA***/
    #define _PORT_ENABLE	0x80
    #define	_SINGLE_RECEIVE 0x20
    #define	_RECEIVE_ENABLE 0x10
    #define _CONTINIOUS_RECEIVE 0x10
    #define _ADDRESS_DETECTION	0x8

    /* define BAUDCONx***/
    #define _RECEIVE_IDLE 0x40
    #define _DATA_INVERTED 0x20
    #define _BRG16	0x8
    #define	_WAKE_UP	0x4
    #define _AUTOBAUD_DETECT 0x1

    //#define setBaudRate()
    #define stopUart1()	RCSTA1 &= ~_PORT_ENABLE
    #define startUart1()	RCSTA1 |= _PORT_ENABLE

    /*uart device speed*/
    #define BAUD14400_20M	345
    #define BAUD19200_20M	259
    #define BAUD57600_20M	86
    #define BAUD115200_20M	42
    #define BAUD230400_20M  20 //or 10
    #define BAUD460800_20M  9 //or 10

    #define MAX_LENGTH 200

    #define setBaudRate1(B)	{ \
                    SPBRG1 =(B)&0x00ff; \
                    SPBRGH1 = ((B)>>8)&0x00ff;\
                    }

    #define setBaudRate2(B)	{ \
                    SPBRG2 =(B)&0x00ff; \
                    SPBRGH2 = ((B)>>8)&0x00ff;\
                    }
    



    /*
     * EUSART1
     */
    char sci_PutStrn1(ROM char *,UINT16 );

   	INT16 sci_GetChar1(UINT16 to );

    void sci_PutChar1( char);

    #define byteReceived1()	PIR1bits.RC1IF

    


    /*
     * EUSART2
     */

    char sci_PutStrn2(ROM char *,UINT16 );

    INT16 sci_GetChar2(UINT16 to);

    void sci_PutChar2( char);

	UINT8 sci_WaitSymbol1(UINT16 to);
	
	UINT8 sci_WaitSymbol2(UINT16 to);
	
    void flushUART2();

    #define byteReceived2()	PIR3bits.RC2IF
    
	UINT16  sci_GetLine1(char *buf);
	
    UINT16  sci_GetStr1(char *buf);

	UINT16  sci_GetLine2(char *buf);
	
    UINT16  sci_GetStr2(char *buf);

    void sci_PutStr2(ROM char *str);
    void sci_PutStr1(ROM char *str);

    UINT8 sci_ReadLine2(char * dst, UINT8 n);

	void  flushUart1(void);
	void  flushUart2(void);

	void Uart1On(UINT8);




	/************************************************************
							SW UArt
	*************************************************************/
	#define USES_SWUART extern _SW_UART_STATUS_t sw_uart_status
	#define SWUART_STOPBIT1
		
	typedef struct {
	unsigned sync:1;
		}_SW_UART_STATUS_t;
		
	void swuart_init(void);
	void swuart_putchar(char c);
		
	#define swuart_logical1()	SW_UART_TX=1
	#define swuart_logical0()	SW_UART_TX=0
	
	#define SW_UART_SYNC		do{ sw_uart_status.sync =1;}while(0);
	#define SW_UART_WaitSync()		while(!sw_uart_status.sync);	sw_uart_status.sync =0;														

	void swuart_puts(const char * str);	
	void swuart_putsn(const char * str,unsigned char len);
	
#endif 