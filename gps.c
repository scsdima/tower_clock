#include "gps.h"
#include "cdate.h"
#include "custom_uart.h"
#include "setup.h"
#include "config.h"
#include "delayt.h"
#include "debug.h"



//const char setup_string[]=
//{"\xB5\x62\x06\x31\x20\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x1B\xB7\x00\x00\x1B\xB7\x00\x99\x99\x99\x19\x99\x99\x99\x19\x00\x00\x00\x00\x67\x00\x00\x00\x2A\x31"};

char gps_buffer[100];
char buffer2[100];
ck_t ck;
	
void SetupGPS()
{					
	GPS_ON=1;
	__delayMs(1000);
}

char ReadTime()
{
	char n,i,retry;
	retry=100;
	in_packet_t *packet=(in_packet_t *)gps_buffer;

	while(retry--){
		
	if(sci_GetChar1(100)==0xb5 && sci_GetChar1(100)==0x62 &&
		sci_GetChar1(100)==0x01 && sci_GetChar1(100)==0x06 &&
		sci_GetChar1(100)==0x34 && sci_GetChar1(100)==0x00)
		{
			LED3_GREEN=1;
			i=0;
			packet->class=0x01;
			packet->id=0x06;
			packet->len=0x0034;			
			for(n=0;n<0x34;n++)	
			{
				packet->data[n]=(char)sci_GetChar1(100);
			}	
			ck.a=(char)sci_GetChar1(100);
			ck.b=(char)sci_GetChar1(100);			
			LED3_GREEN=0;
			if(calcCheckSum((char*)packet,0x38,&ck))
			{	
				DEBUG("get-gps-time\r\n");	
				DEBUG(packet->data);	
				setSystemTimeRaw((UINT32)*((UINT16*)&packet->data[8]),(UINT32)*((UINT32*)&packet->data[0])/1000);				
				LED3_GREEN=0;
				return 1;
			}
			
		}	
	}
	DEBUG("ERROR");
	LED3_GREEN=0;
	return 0;
}



char calcCheckSum(const char *buf,UINT8 len,ck_t *ck)
{
	UINT8 i;
	UINT8 CK_A = 0;
	UINT8 CK_B = 0;
	for(i=0;i<len;i++)
	{
		CK_A = CK_A + buf[i];
		CK_B = CK_B + CK_A;
	}
	if(ck->a==CK_A && ck->b==CK_B)		return 1;
	return 0;
}
