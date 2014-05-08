

#include 	"setup.h"
#include 	"config.h"
#include 	"cdate.h"
#include 	"stdio.h"
#include 	"clock.h"
#include 	"delayt.h"
#include "gps.h"
#include "motor.h"
#include 	"compiler.h"
#include 	"custom_uart.h"
#include 	"debug.h"

	/* configuration word*/
		#pragma config CPUDIV 	= OSC1
		#pragma config PLLDIV 	= 5
	
		#pragma config 	WDTEN 	= ON
		#pragma config 	STVREN 	= ON
		#pragma config 	FOSC 	= HSPLL
		#pragma config 	WDTPS 	= 32768
		#pragma config 	XINST 	= OFF
		#pragma config 	CP0 	= OFF
		#pragma config 	DEBUG 	= OFF
		#pragma config 	CCP2MX 	= DEFAULT
		#pragma config 	MSSPMSK = MSK7


char buffer[512];

enum {
	evReadTimeFromGps=1,
	evButton1=2,
	evTickClock=4,
	ev1Hour=8,
	evCheckWinterTimeSwitch=0x10
	};
unsigned int EVENT;

unsigned int TIMER_1hour;
unsigned char TIMER_1min;
unsigned long int TIMER_24hour;
extern state_t State;	

int msec;
CDateTime date_time;
USES_SWUART;

/**************************************************************************************/
int main(void) {

	unsigned int i=0;
	
  	setup_hardware();
	swuart_init();
	DEBUG("### \r\n restart \r\n");
	DEBUG(__DATE__);
	DEBUG(__TIME__);
	DEBUG(" \r\n### \r\n");
 	init_clock(); 
 	__delayMs(20000);
	//EVENT =	evTickClock|ev1Hour;
	jobRetrieveTimeDate();
	jobRetrieveTimeDate();
	
	///*D*/sys_time.tm_seconds=86300;
    while (1){

	    if(EVENT & evTickClock)
	    {		  			  
		    DEBUG("move-clock\r\n");	
		    jobSetClockTime();
		    EVENT &= ~evTickClock;
		   } 
		if(EVENT & evReadTimeFromGps)	//every 1 hour    
		{
			DEBUG("synchronize:");
			jobRetrieveTimeDate();
			getSystemTime(&date_time);
		  	if((date_time.Date.month==10|| date_time.Date.month==3) )
		  	{
			  	EVENT |= evCheckWinterTimeSwitch;
			  }	
			sprintf(buffer,"time:%u:%02u:%02u\r\ndate:%u.%u.%u\r\n",date_time.Time.hour
							,date_time.Time.min
							,date_time.Time.sec
							,date_time.Date.day
							,date_time.Date.month
							,date_time.Date.year);
		    INFO(buffer);
	    	EVENT &= ~evReadTimeFromGps;
	 	}   
	 	if(EVENT & evCheckWinterTimeSwitch)
	 	{
		 	jobCheckWinterTime();
		 }	
	 	if(EVENT & evButton1)
	 	{
		 //	DEBUG("button1\r\n");
	//	 	mechanical_zero();
		 	EVENT &= ~evButton1;
		 }		
	    
		CLRWDT();

    }//while(1)u

}/*main()*/

/**************************************************************************************/
	
	static void interrupt isr_high(void)
	{
	 /*u seconds delay */
	    if (INTCONbits.TMR0IF && INTCONbits.TMR0IE) 
		{
	        TMR0H = 0xff;
	        TMR0L = -40; /* ~10 usec */
	        INTCONbits.TMR0IF = 0;
	        DELAY_MACRO_USEC;
	    }

    	/*m seconds delay */
  	  	if (PIR1bits.TMR1IF && PIE1bits.TMR1IE) 
	    {
	        PIR1bits.TMR1IF = 0;
	        TMR1H = 0xfd;
	        TMR1L = 0x8e;
	     	if(!BUTTON1 && !(EVENT &evButton1)) EVENT|=evButton1;
	        if (msec) msec--; //count down timer if timer >0
	       	                     
		        if (!msec) {
				addSystemTime(1);//add 1 sec
			
				//timers
				if(TIMER_1hour) TIMER_1hour--;
				else {
					EVENT |= evReadTimeFromGps;
					TIMER_1hour = 10*60;
				}	
				if(TIMER_24hour) TIMER_24hour--;
				else {
					TIMER_24hour = 60*60*24;
				}	
				if(TIMER_1min) TIMER_1min--;
				else {
					TIMER_1min = 30;
					EVENT |= evTickClock;
				}
	            msec = 1000;
	        }// if (!msec)
	        /*delay macro*/
	        DELAY_MACRO_MSEC;
	    }//if (PIR1bits.TMR1IF && PIE1bits.TMR1IE)
	    
	    //For Software uart debug purposes
	     if (PIR2bits.TMR3IF ) 
		     {
			     PIR2bits.TMR3IF=0;
			     TMR3H = 0xf7;
			     TMR3L = 0xdb;
			      SW_UART_SYNC;
		    } 
	}//void YourHighPriorityISRCode(void)
	