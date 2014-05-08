#ifndef gps_h
	#define gps_h
	
	#include "cdate.h"
	#include "compiler.h"

		typedef struct {
			UINT8 a;
			UINT8 b;
		}ck_t;
			
	typedef struct {
		char 	class;
		char 	id;
		UINT16 len;
		char 	data[0x34];
	}in_packet_t;	

	void SetupGPS();
	char ReadTime();
	 char makeTimeDate(UINT32 week,UINT32 sec,CDateTime *dt);
	char calcCheckSum(const char *buf,UINT8 len,ck_t *ck);
#endif