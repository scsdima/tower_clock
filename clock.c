#include "clock.h"
#include "motor.h"
#include "setup.h"
#include "cdate.h"
#include "gps.h"
#include "stdio.h"
#include "debug.h"

extern CDateTime date_time;

#define onZeroPosition_24h() (POS1==0 && POS2==0) 	//when both hall detectors are under magnets
#define onZeroPosition_12h() (POS1==0 ) 	//when clock shows0
#define swTimeShift()	

UINT16 clock_counter; //current position of clock
double	last_clock_pos;//for time zero pass detection

char buffer2[200];

const unsigned char TimeShiftTableWinter[20]=//oktober
{	28,27,26,25,30,29,28,27,25,31,30,29,27,26,25,31,29,28,27};	
const unsigned char TimeShiftTableSummer[20]=//oktober
{	0, 31,30,29,27,26,25,31,29,28,27,26,31,30,29,28,26,25,31};

state_t State;	

void zvon(char k)
{
	while(k--)
	{
	 	DEBUG("zvon:");DEBUGC(k+'0');
 		ZVON1_ON;
 		__delayMs(4800);
		ZVON1_OFF;
		__delayMs(100);
	}	
}	
void init_clock()
{
	GPS_ON=1;
	SetupGPS();	
	State.flags.day_zero=0;
	State.flags.hour_zero=0;
	State.options.timeshift=0;
	State.options.timeshift_off=1;
	State.options.time_winter=1;;
	clock_counter=0;
	MotorCenter();
	mechanical_zero();
	
}
#define ABS(_X_) 	((_X_)<0?-(_X_):(_X_))
char  set_mechanical_time(double clock_pos)	//i0..5930
{	
	char dir,res=0;
	double pos_offs;
	int offs_move;
	if(last_clock_pos > clock_pos )
	 //time zero passed
	{
		DEBUG("zero time passed\n");
		mechanical_zero();
		last_clock_pos = clock_pos;
		return 1;
	}	
	last_clock_pos = clock_pos;
	
	dir=STEP_RIGHT;
	pos_offs = clock_pos-(double)clock_counter;
	if(pos_offs<0) 
	{
		DEBUG("left<<\n");
		dir = STEP_LEFT;
		offs_move =(int)ABS(pos_offs);
	}	
	else if(pos_offs>4000)
		{
			DEBUG("left<<\n");
			dir = STEP_LEFT;
			offs_move =5930-(int)pos_offs;
		}
		else {
		DEBUG("right>>\n");
		offs_move =(int)pos_offs;
	}
	
	sprintf(buffer2,"round=%d offset=%d\r\n",clock_counter,offs_move);
	DEBUG(buffer2);	
	
	while(offs_move--)
	{
		DEBUGC('*');
		MotorStep(1,dir);	
		if(dir==STEP_RIGHT)
		{
			if(!State.flags.day_zero && onZeroPosition_24h() ) 
			{
				State.step_center=5;
				DEBUG(">>center 24h<<\n");	
			}
			if(!State.flags.hour_zero && onZeroPosition_12h() ) 
			{
				DEBUG(">>center 12h<<\n");	
			}
			if(State.step_center-- ==1)	
			{
				DEBUG("0 zero 0\n");	
				clock_counter=0;	
				return 1;			
			}
		}	
		State.flags.hour_zero=onZeroPosition_12h();
		State.flags.day_zero=onZeroPosition_24h();
	}
	clock_counter = (int)clock_pos;

 	return 0;
	
}	
//find mechanical absolute zero
int mechanical_zero()
{
	int i;
	DEBUG("set zero\r\n");

	while(onZeroPosition_24h()) MotorStep(1,STEP_LEFT); //if allready on zero position
	for(i=0;!onZeroPosition_24h();i++)  //finding zero position 		
		{			
			MotorStep(1,STEP_RIGHT);			
		}
	clock_counter=0;
	State.flags.day_zero=0;
	State.flags.found_center=1;
	MotorStep(3,STEP_RIGHT);
	return i;
}	
//****JOBS******

void	jobRetrieveTimeDate()
{
	ReadTime();
//	if(!State.options.timeshift_off && State.options.timeshift ) 
//		addSystemTime(-(UINT32)60*60);//add -1 hour
		
	addSystemTime(-(UINT32)60*60);//add -1 hour
}
	
void jobCheckWinterTime()
{
	if(!State.options.timeshift_off)
			{
				State.options.timeshift = CheckTimeShift()==1;
			}	
	else State.options.timeshift=0;
}
			
void jobSetClockTime()
{	
	double sec_v;
	long int min_v;
	sec_v = (double) getTodaySytemTimeSeconds();
	
	//detecting if need to make bell
	getSystemTime(&date_time);
	
	if(date_time.Time.min==0)
	{
		State.flags.makebell=1;
		/*I*/DEBUG("makebell");
	}
	else if(date_time.Time.min>3)
		{
			State.flags.bell_has_made=0;								
			State.flags.makebell=0;
		}	
			
	sec_v/=(double)ROUND_CONST_SECONDS/ROUND_TOTAL_STEPS/*+HOUR_OFFSET*/;//ROUND_CONST_SECONDS=86400 , ROUND_TOTAL_STEPS=steps per cycle
	/*I*/ sprintf(buffer2,"position=%f\r\n",sec_v);DEBUG(buffer2);	
	if(State.flags.makebell &&!State.flags.bell_has_made ) 
	{
		zvon(date_time.Time.hour);
		State.flags.bell_has_made=1;
	}
	while(set_mechanical_time(sec_v)==1);
	
}
	
char CheckTimeShift()
{
	CDate wd ,sd;
	UINT16 cur_day_n,curw,curs;
	wd.day = TimeShiftTableWinter[date_time.Date.year-12];
	wd.month = 10;
	sd.day = TimeShiftTableSummer[date_time.Date.year-12];
	sd.month = 3;
	curw=DayNumber(&wd);
	curs=DayNumber(&sd);
	cur_day_n = DayNumber(&date_time.Date);
	if(cur_day_n > curw) return 1;
	if((cur_day_n == curw) &&date_time.Time.hour >= 3) return 1;
	if(cur_day_n < curs) return 1;
	if((cur_day_n == curs)	&& date_time.Time.hour < 2) return 1;
	return 0;
}

