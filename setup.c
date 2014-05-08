#include "setup.h"
#include "compiler.h"
#include "custom_uart.h"
/*
111 = 8 MHz (INTOSC drives clock directly)
110 = 4 MHz(3)
101 = 2 MHz
100 = 1 MHz
011 = 500 kHz
010 = 250 kHz
001 = 125 kHz
000 = 31 kHz (from either INTOSC/256 or INTRC directly)(4)
*/


void setOscillator(UINT8 osccon)
{
	OSCCON &= 0b10001100;
	OSCCON |= osccon;
}

void setup_hardware()
	{
   
	//due to uart incompatibility
		OSCTUNEbits.PLLEN=0;
		/*tuning ports*/                
		TRISA=0;
		TRISB = 0xff; //RA4 -led
		TRISC	=	0;
		TRISG=0;
		TRISE=0b10000000;
		LATA=0;
		LATB=0;
		LATC=0;		
		TRISAbits.TRISA4=1;
		TRISD=0b00011110;
		TRISF=0b10000000;

		/*EUSART1 GPS*/
		TRISCbits.TRISC6=1;
		TRISCbits.TRISC7=1;
		setBaudRate1(BAUD57600_20M); //57600
		BAUDCON1 = _BRG16;
		TXSTA1 = _ASYNCH_MODE | _8_BIT | _TRANSMIT_ENABLE |_HIGH_SPEED;
		RCSTA1 =  _PORT_ENABLE |_RECEIVE_ENABLE ;

		/*EUSART2 gsm*/
		//setBaudRate2(BAUD115200_20M);
		//BAUDCON2 = _BRG16;
		//TXSTA2 = _ASYNCH_MODE | _8_BIT | _TRANSMIT_ENABLE |_HIGH_SPEED;
	//	RCSTA2 = _PORT_ENABLE | _RECEIVE_ENABLE ;

		/*ADC*/
		WDTCONbits.ADSHR =1;
		ANCON0 = 0xff; //AN0 analog input
		ANCON1 = 0xff;
		WDTCONbits.ADSHR = 0;
		ADCON0 = 0b00000000; //ADON
		ADCON1 = 0b00000000;

		CM1CON =0;
		CM2CON =0;
//		CREF =0;


	/*Timer 0 enable*/
		T0CON = 0b10011000;	/*TMR0ON,16Bit,Internal CLKO,1/2 prescaler*/
		INTCONbits.TMR0IE = 1;
		/* Timer 1 */
		T1CON = 0b10110101;
//		TMR1 = -1500;
		PIE1bits.TMR1IE = 1;
		RBPU=0;	
		/*Timer 3*/
		T3CON = (1<<7)|(1<<2)|1; //timer on /16 bit /int clock
		PIE2bits.TMR3IE = 1;		
		ZVON1_OFF;
		ZVON2_OFF;
        InterruptsON();		

}