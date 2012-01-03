#include "intersection.h"
#include "base.h"
#include "debug.h"
#include "setting.h"
#include <assert.h>

extern cell cells[];
extern arc arcs[];
extern setting settings;

extern bool omega[MAX_CLOCK][MAX_INTERSECTION][MAX_PHASE];
extern int present_clock;
extern debug *Log;

phase::phase( int i ):id(i){
	if( !from_arc.empty() ) from_arc.clear();
	if( !to_arc.empty() ) to_arc.clear();
}

void phase::add_flow(int _from, int _to){
	from_arc.push_back( _from );
	to_arc.push_back( _to );
}

void phase::set_on(){
	for( int i = 0; i < (int)from_arc.size(); ++i ){
		int from_cell,to_cell;
		from_cell = arcs[from_arc[i]].get_last_cell();
		to_cell =	arcs[to_arc[i]].get_first_cell();
		cells[from_cell].set_signals_on( to_cell );
	}
}

void phase::set_off(){
	for( int i = 0; i < (int)from_arc.size(); ++i ){
		int from_cell,to_cell;
		from_cell = arcs[from_arc[i]].get_last_cell();
		to_cell =	arcs[to_arc[i]].get_first_cell();
		cells[from_cell].set_signals_off( to_cell );
	}
}

intersection::intersection():id(0),type(0),pos(0,0),
		min_green(0),max_green(0),right_turning(true),num_phases(0){
			if( !connected_nodes.empty() ) connected_nodes.clear();
			if( !phases.empty() ) phases.clear();
}


intersection::intersection( int i,int t,int px,int py ):id(i),type(t),pos(px,py){
			size++;
			if( !connected_nodes.empty() ) connected_nodes.clear();
			if( !phases.empty() ) phases.clear();
}

intersection::intersection( int i,int t,int px,int py,
		int min_g,int max_g,int rturn,int num_p ):id(i),type(t),pos(px,py),
		min_green(min_g),max_green(max_g),right_turning(rturn!=0),num_phases(num_p){
			size++;

			min_green_ticks = (int)ceil(min_green*1.0/settings.clock_tick);
			max_green_ticks = (int)floor(max_green*1.0/settings.clock_tick);

			if( !connected_nodes.empty() ) connected_nodes.clear();
			if( !phases.empty() ) phases.clear();
			while( num_phases > (int)phases.size() ) phases.push_back( phase() );
			char str[256];
			sprintf(str,"phases::size %03d",phases.size() );
			Log->process( str );
}

void intersection::add_node( int i ){
	assert( i >= 0 );
	connected_nodes.push_back( i );
}

void intersection::add_phase( int i,int from_arc,int to_arc ){
	assert( i>0 && from_arc>=0 && to_arc>=0 );
	--i;
	phases[i].add_flow( from_arc,to_arc );
}

void intersection::update_signal(){
	for( int i = 0; i < (int)phases.size(); ++i ){
		if( omega[present_clock][id][i] ){
			phases[i].set_on();
		}
		else phases[i].set_off();
	}
}


int intersection::size = 0;


