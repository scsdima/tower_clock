#ifndef clock_h
	#define clock_h

	#define ROUND_TOTAL_STEPS 5930
	#define ROUND_CONST_SECONDS 86400
	
	
	typedef struct 
	{
	struct {
		unsigned day_zero:1;
		unsigned hour_zero:1;
		unsigned found_center:1;
		unsigned makebell:1;
		unsigned bell_has_made:1;
		
	}flags;
	unsigned step_center;

	struct{
		unsigned timeshift:1; 
		unsigned timeshift_off:1;
		unsigned time_winter:1;
	}options;
	}state_t;

	void init_clock();
	char set_mechanical_time(double clock_pos);

	int mechanical_zero();
	void jobSetClockTime();
	void	jobRetrieveTimeDate();
	void jobCheckWinterTime();
	char CheckTimeShift();
	void zvon(char);
#endif