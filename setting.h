
#ifndef _setting_H

#define _setting_H

#include <math.h>

class setting{
public:
	
	setting();
	setting( int,int,int,double,int,int );

	// start_time or end_time [ 0,24*60*60 ]
	int start_time,end_time;
	// clock_tick [ 1,10 ]
	int clock_tick;
	// epsilon [ 0.0001,0.01 ]
	double epsilon;
	// initial cell occupation [ 0,100 ]   in percentage
	int initial_occupation;
	// initial control strategy { 0,1 
	int initial_control;
	
	double cell_length_factor;

	int max_ticks;
	int get_max_ticks(){	return max_ticks; }
};

#endif
