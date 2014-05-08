#ifndef SETUP_H
	#define SETUP_H
	
	#include "config.h"
	
	#ifdef __PICC18
		#include "pic18.h"
		#include "htc.h"
	#endif

       //interface
            #define     LED1_GREEN    LATCbits.LATC1
            #define     LED2_RED    LATCbits.LATC4
            #define     LED3_GREEN    LATAbits.LATA4
            #define     LED4_RED    LATFbits.LATF2                     
            
            #define 	ZVON1_ON	TRISBbits.TRISB2=0;LATBbits.LATB2=0;
            #define 	ZVON2_ON	TRISBbits.TRISB5=0;LATBbits.LATB5=0;
            #define 	ZVON1_OFF	TRISBbits.TRISB2=1;
            #define 	ZVON2_OFF	TRISBbits.TRISB5=1;
            
            #define MA_en	LATGbits.LATG0
			#define MA_DIR1	LATFbits.LATF5
			#define MA_DIR2	LATEbits.LATE5
			
			#define MB_en	LATFbits.LATF6
			#define MB_DIR1	LATGbits.LATG1
			#define MB_DIR2	LATGbits.LATG2
			
			#define GPS_ON	LATGbits.LATG4
			
			#define POS1	PORTDbits.RD3
			#define POS2	PORTEbits.RE7
			#define BUTTON1	PORTBbits.RB6
			
			#define     UART_SW     LATDbits.LATD5 //UART_SW
			#define 	UART1TX_TRIS		TRISCbits.TRISC7
			#define 	UART1TX_LAT		LATCbits.LATC7
			#define 	UART1RX_TRIS		TRISCbits.TRISC6
			#define 	UART1RX_LAT		LATCbits.LATC6
			
				            
			#define 	SW_UART_TRIS_TX         TRISBbits.RB7
			#define 	SW_UART_TX				LATBbits.LATB7
		//	#define 	SW_UART_BAUD			2400	
            



	#define F8MHz (7<<4)
	#define F4MHz (6<<4)
	#define F2MHz (5<<4)
	#define F1MHz (4<<4)
	#define F500kHz (3<<4)
	#define F250kHz (2<<4)
	#define F125kHz (1<<4)
	#define F31kHz (0<<4)
	

 	
	#define InterruptsOFF() INTCONbits.GIEL=0;INTCONbits.GIEH=0;
	#define InterruptsON()	INTCONbits.GIEL=1;INTCONbits.GIEH=1;

	void setup_hardware(void);
    void setOscillator(UINT8);
#endif
