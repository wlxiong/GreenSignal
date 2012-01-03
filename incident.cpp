#include "incident.h"
#include "setting.h"
#include "cell.h"
#include "arc.h"
#include "base.h"

extern cell cells[MAX_CELL];
extern arc	arcs[MAX_ARC];
extern int present_clock;
extern setting settings;

incident::incident():id(0),distance(0.0),start_time(0),end_time(0),max_flow(0.0){}

incident::incident( int aa,double dist,int st,int et,double mf ):
at_arc(aa),distance(dist),start_time(st),end_time(et),max_flow(mf){

	double len = arcs[at_arc].get_cell_length();
	at_cell = (int)ceil(distance/len)+arcs[at_arc].get_first_cell()-1;
	start_clock = (int)floor( (start_time-settings.start_time )*1.0/settings.clock_tick ) + 1;
	end_clock =	 (int)floor( (end_time-settings.start_time)*1.0/settings.clock_tick ) + 1;
	id = size+1;
	size++;

}

void incident::occur(){
	if( present_clock == start_clock )
		cells[at_cell].set_max_flow(max_flow);
	if( present_clock == end_clock )
		cells[at_cell].set_max_flow( arcs[at_arc].get_max_flow() );
}

int incident::size = 0;
