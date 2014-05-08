#ifndef __COMPILER_H
	#define __COMPILER_H

		#include <htc.h>
		typedef unsigned char 	UINT8;
		typedef unsigned int 	UINT16;
		typedef unsigned long 	UINT32;
		
		typedef signed char 	INT8;
		typedef signed int 	INT16;
		typedef signed long 	INT32;
	
	    #define ROM                 	const
		#define rom
        #ifndef NOP()
        	#define NOP()               	asm("NOP");
        #endif
        #ifndef CLRWDT()
        	#define CLRWDT()				asm("CLRWDT");
        #endif
        #ifndef RESET()
        	#define RESET()					asm("RESET");
        #endif
        #ifndef SLEEP()
       	 #define SLEEP()					asm("SLEEP");
        #endif
        
        #define INTERRUPT   static void interrupt
 

#endif