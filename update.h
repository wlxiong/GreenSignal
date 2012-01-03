#ifndef _update_H

#define _update_H

#include <vector>
#include <ctime>
#include <cstdio>

using std::vector;

#include "base.h"
#include "setting.h"
#include "arc.h"
#include "cell.h"
#include "demand.h"
#include "intersection.h"
#include "node.h"
#include "coordinate.h"
#include "incident.h"
#include "debug.h"

extern int			present_clock;
extern setting		settings;
extern node			nodes[MAX_NODE];
extern arc			arcs[MAX_ARC];
extern cell			cells[MAX_CELL];
extern vector<int>	origin_set,normal_set,diverge_set,merge_set,destination_set;
extern double		exist_vehicle[MAX_CLOCK][MAX_CELL];
extern double		diverge_flow[MAX_CELL][MAX_ADJ_CELL];
extern intersection intersections[MAX_INTERSECTION];
extern incident		incidents[MAX_INCIDENT];
extern bool			omega[MAX_CLOCK][MAX_INTERSECTION][MAX_PHASE];
extern debug		*Log;


/*
	initial the omega
*/

void initial_occupation( double *vehicle ){
	for( int i = 1; i <= cell::size; ++i ){
		vehicle[i] = settings.initial_occupation/100.0*cells[i].get_max_vehicle();
	}
}

void initial_control(){

	memset(omega,false,sizeof(omega) );
	char str[256],_str[256];
	int istr = 0;
	for( int i = 1; i <= intersection::size; ++i ){
		int min_green = (int)ceil(intersections[i].get_min_green()*1.0/settings.clock_tick);
		int g=0,p=0;
		for( int j = 1; j <= settings.get_max_ticks(); j++ ){
			omega[j][i][p] = true;
			if(++g == min_green){
				g = 0;
				p = ++p % intersections[i].get_num_phases();
			}
		}
	}

	//Output initial timing plan.
	Log->process("Initial Timing Plan...");
	for( int i = 1; i <= intersection::size; ++i ){
		sprintf(_str,"Intersection %03d",i);
		Log->process(_str);
		for( int j = 1; j <= settings.get_max_ticks(); ++j ){
			memset( str,0,sizeof(str) );
			istr = 0;
			sprintf( str,"#%05d ",j );
			istr = strlen(str);
			for( int k = 0; k < intersections[i].get_num_phases(); ++k ){
				sprintf( str+istr,"%d ",omega[j][i][k]?1:0 );
				istr+=2;
			}
			Log->process(str);
		}
	}
}

void modify_control(int I, int t0, int t1, int P){
	for(int i=t0;i<=t1;i++){
		omega[i][I][P]=true;
		for( int j = 0; j < intersections[I].get_num_phases(); ++j ){
			if( j == P ) continue;
			omega[i][I][j] = false;
		}
	}
}

void initial_vehicle(){
	for( int i = 1; i <= cell::size; ++i ){
		cells[i].set_in_flow(-1.0);
		cells[i].set_out_flow(-1.0);
	}
}

void update_event(){
	for( int i = 1; i <= incident::size; ++i ){
		incidents[i].occur();
	}
}

void update_signal() {
	for( int i = 1; i <= intersection::size; ++i ){
		intersections[i].update_signal();
	}
}


void update_flow( ) {
	cell *cl = cells;

	for( int i = 0; i < (int)origin_set.size(); ++i )
		cl[origin_set[i]].origin_update_flow();

	for( int i = 0; i < (int)diverge_set.size(); ++i )
		cl[diverge_set[i]].diverge_update_flow();

	for( int i = 0; i < (int)merge_set.size(); ++i )
		cl[merge_set[i]].merge_update_flow();

	for( int i = 0; i < (int)normal_set.size(); ++i )
		cl[normal_set[i]].normal_update_flow();

	for( int i = 0; i < (int)destination_set.size(); ++i )
		cl[destination_set[i]].destination_update_flow();

}

double update_occupation( ) {
	double delay = 0.0;
	for( int i = 1; i <= cell::size; ++i )
		delay += cells[i].move_vehicle();
	return delay;
}

double simulate( double* vehicle,double *result,int st,int et ){
	//Start simulation.
	char time1[256],time2[256],time3[256];
	time_t run1;
	time(&run1);
	struct tm *t1 = localtime(&run1);
	sprintf( time1,"Simulation begins at time: %dhour %dmin %dsec.",t1->tm_hour,t1->tm_min,t1->tm_sec);
	Log->process(time1);

	//Vehicle delay during time interval [st,et].
	double delay = 0.0;

	for( int i = 1; i <= cell::size; ++i ) 
		exist_vehicle[st][i] = vehicle[i];

	for( int it = st+1; it <= et; ++it ){

		present_clock = it;
		update_event();
		update_signal();
		update_flow();

		double d = update_occupation();
		delay += d;
	}
	for( int i = 1; i <= cell::size; ++i )
		result[i] = exist_vehicle[et][i];

	//Simulation ends.
	time_t run2;
	time(&run2);
	struct tm *t2 = localtime(&run2);
	sprintf( time2,"Simulation is complete at time: %dhour %dmin %dsec.",t2->tm_hour,t2->tm_min,t2->tm_sec);
	sprintf( time3,"Simulation elapsed: %dsec.",run2 - run1);
	Log->process(time2);
	Log->process(time3);

	return delay*settings.clock_tick;
}

#endif

