/*
DateTime struct
Chikov
*/


#ifndef CTime_H
#define CTime_H
#include "time.h"
#include "compiler.h"

enum {eJan,eFeb,eMar,eApr,eMay,eJun,eJul,eAug,eSep,eOkt,eNov,eDec};
const char MonthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

typedef struct {
	unsigned char day;
	unsigned char month;
	unsigned char year; //format 2XXX till 2255 year
}CDate;

typedef struct {

	unsigned char sec;
	unsigned char min;
	unsigned char hour;
	}CTime;

typedef struct  {
	CDate Date;
	CTime Time;
}CDateTime;


typedef struct {
	unsigned long int tm_weeks;
	unsigned long int tm_seconds;//from this week begin
}ctime_t;



extern ctime_t sys_time;

//eTime;	
UINT32 diffDateTime(CDateTime *dt1,CDateTime *dt2);
UINT16 DayNumber(CDate *d);
void setSystemTimeRaw(UINT32 week,UINT32 sec);
void addSystemTimeRaw(UINT32 addweek,UINT32 addsec);
void addSystemTime(UINT32 addsec);
void getSystemTime(CDateTime *dt_str);
ctime_t * getSystemTimeRaw(void);
char makeTimeDate(UINT32 week,UINT32 sec,CDateTime *dt);
UINT32 getTodaySytemTimeSeconds();
void setSystemTime(CDateTime *dt);

#endif
