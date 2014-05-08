#ifndef DELAY_H
#define DELAY_H

volatile unsigned int _delay_timerMs;
volatile unsigned int _delay_timerUs;

#define timer0_tout() (_delay_timerMs==0)

#define timer0(T)	_delay_timerMs=(T)
    
#define __delayMs(X)	{CLRWDT();_delay_timerMs=(X);while(_delay_timerMs) ;}

#define __setTimerMs(X)		{CLRWDT();_delay_timerMs=(X);}

#define __timeoutMs()		(_delay_timerMs ==0) 

#define DELAY_MACRO_MSEC 	do{ if(_delay_timerMs>0) _delay_timerMs--; }while(0);


/**
 * usec timer
 */

#define __setTimerUs(T)		{_delay_timerUs=(T);}

#define __timerActiveUs()		(_delay_timerUs >0) 

#define __delayUs(Td)	{__setTimerUs((Td));while(__timerActiveUs()) continue;}

#define DELAY_MACRO_USEC 	if(__timerActiveUs()) _delay_timerUs--;

#endif