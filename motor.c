#include "motor.h"
#include "setup.h"
#include "delayt.h"
#include "config.h"



#ifdef POSITIVE_LOGIC
	#define mON 1
	#define mOFF 1
#else
	#define mON 0
	#define mOFF 1
#endif
typedef struct {
	unsigned A1:1;
	unsigned B1:1;
	unsigned A2:1;
	unsigned B2:1;
}t_MotStepPattern;

 char step;

const t_MotStepPattern mot_pattern[4] ={
										{	mON		,mON	,mOFF	,mOFF	},
										{	mOFF	,mON	,mON	,mOFF	},
										{	mOFF	,mOFF	,mON	,mON	},
										{	mON		,mOFF	,mOFF	,mON	},	
										};
void MotorStep( int n,char dir)
{
	while(n--)
	{
		if(dir == STEP_LEFT) {
			step= ++step %4;
		}
		else 	step= --step %4;
	
		MA_DIR1 = mot_pattern[step].A1;
		MA_DIR2	= mot_pattern[step].A2;
		MB_DIR1 = mot_pattern[step].B1;
		MB_DIR2 = mot_pattern[step].B2;
		
		MA_en = mON;
		MB_en =mON;
		__delayMs(30);	
		#ifdef HOLD_MOTOR
			MB_en =mON;
			MA_en = mON;
		#else
			MB_en =mOFF;
			MA_en = mOFF;
		#endif
	}	
}

void MotorCenter()
{	
	MA_DIR1 =mON;	
	MB_DIR1 =mON;
	MA_DIR2 =mOFF;	
	MB_DIR2 =mOFF;
	
	MA_en = mON;
	MB_en =mON;
	__delayMs(300);	
	MB_en =mOFF;
	MA_en = mOFF;
}
