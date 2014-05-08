#include "cdate.h"
#include "config.h"
#include "stdio.h"
#include "debug.h"

char buffer3[100];
ctime_t sys_time;
//----------------------------------------------------------------------
//----------------------------------------------------------------------	

UINT16 DayNumber(CDate *d)
{
	char i;
	UINT16 days;
	for(days=0,i=0;i<(d->month-1);i++)
		days+=MonthDays[i];
	days+=d->day-1;
	return days;
}	

UINT32 compDateTime(CDateTime *dt1,CDateTime *dt2)
{
	UINT32 difference;
//	UINT32	addweeks;
//	ctime_t
//	addweeks = dt1.Date.year
	return difference;	
}	

void setSystemTimeRaw(UINT32 week,UINT32 sec)
{
	sys_time.tm_weeks = week;
	sys_time.tm_seconds = sec;	  
	addSystemTime((UINT32)2*60*60);//add gmt hours
}	
void setSystemTime(CDateTime *dt)
{
	char i;
	UINT32 days;
	days = (UINT32)(dt->Date.year/4) *((UINT32)366 +(UINT32)365*365*365);
	for(i=0;i<(dt->Date.month-1);i++)
		days+=MonthDays[i];
	days+=dt->Date.day-1;
	sys_time.tm_weeks = days/7;
	sys_time.tm_seconds = (days%7) *(UINT32)24*60*60;
	
}	
void addSystemTimeRaw(UINT32 addweek,UINT32 addsec)
{
	sys_time.tm_weeks += addweek;
	sys_time.tm_seconds += addsec;
}	

void addSystemTime(UINT32 addsec)
{
	UINT32 tmp = sys_time.tm_seconds +addsec;	
	sys_time.tm_seconds = tmp%((UINT32)7*24*60*60);	
	sys_time.tm_weeks += 	tmp/((UINT32)7*24*60*60);	
}	

void getSystemTime(CDateTime *dt_str)
{
	makeTimeDate(sys_time.tm_weeks,sys_time.tm_seconds,dt_str);
}	
ctime_t * getSystemTimeRaw(void)
{
	return &sys_time;
}	
UINT32 getTodaySytemTimeSeconds()
{
	UINT32 tmp = (UINT32)24*60*60;
	
	return (UINT32)sys_time.tm_seconds%tmp;
}	

char makeTimeDate(UINT32 week,UINT32 sec,CDateTime *dt)
{
    const UINT32 weeks_in_year=(365/7);//~= week in year
    char leap ;
    UINT32 cur,rest;
    UINT32 current_week ;
    UINT32 current_day ;
    cur=week/weeks_in_year;
    dt->Date.year =  (UINT8)cur-20;//from year 1980 move to 2000
    leap = ((dt->Date.year)&3==0);
    current_week = week-(cur/4)*(366 +3*365)/7;
    current_day = current_week*7;
    rest = current_day+sec/((UINT32)60*60*24);
    cur = eJan;//{31,28,31,30,31,30,31,31,30,31,30,31};
    

    
    while(1)
    {
        UINT32 cdm = MonthDays[cur];//days in this month
        if(cur==eFeb && leap) rest++;
        if(rest <= cdm)
        {
            dt->Date.month = (UINT8)cur+1;
            dt->Date.day = (UINT8)rest;
            break;
        }
        rest-=cdm;
        if(cur++ == eDec) return 0;
    }
    rest=sec%((UINT32)24*60*60);
    dt->Time.hour= (UINT8)(rest/((UINT32)60*60));
    rest%=(UINT32)60*60;
    dt->Time.min = (UINT8)(rest/60);
    dt->Time.sec = (UINT8)(rest%60);
    return 1;
}