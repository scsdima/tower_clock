#ifndef __DEBUG_H
	#define __DEBUG_H	
	
	#include "compiler.h"
	#include "custom_uart.h"
	#include 	"stdio.h"
			#define INFOC(_chr_) do{swuart_putchar((const char)_chr_);}while(0)
		 	#define INFO(_txt_) do{swuart_puts((const char*)_txt_);}while(0)
		 	#define INFON(_txt_,_n_) do{swuart_putsn((_txt_),(_n_));}while(0)
		 	
	#ifdef __DEBUG
			
			#define DEBUGC(_chr_)	do{swuart_putchar((const char)_chr_);}while(0)
			#define DEBUG(_txt_)	do{swuart_puts((const char*)_txt_);}while(0)
			#define DEBUGN(_txt_,_n_) do{swuart_putsn((_txt_),(_n_));}while(0)
		 
		#else

			#define DEBUGC 	NOP();
			#define DEBUG	NOP();
			#define DEBUGN 	NOP();
	#endif
	
	
#endif