
#include "arc.h"
#include "cell.h"
#include "node.h"
#include "debug.h"
#include <math.h>
//#include <iostream>

extern node nodes[];
extern setting settings;
extern cell cells[];
//extern int gg[];
extern debug* Log;
//#include "arc.h"

arc::arc():id(0),up_node(0),down_node(0),
		max_speed(0.0),max_flow(0.0),jam_density(0.0),delta(0.0){
			//std::cout<<gg[0]<<std::endl;
}

arc::arc( int i,int un,int dn,double ms,double mf,double jd,double del = -1.0):
		id(i),up_node(un),down_node(dn),max_speed(ms),
		max_flow(mf),jam_density(jd),delta(del){
			if( delta < 0.0 ){
				delta = max_flow*settings.clock_tick/
					(jam_density*max_speed - settings.clock_tick*max_flow);
			}
			size++;
			nodes[un].set_arc(i);
			nodes[dn].set_arc(i);
			length = nodes[up_node].get_pos().dist(	nodes[down_node].get_pos() );
			cell_length = max_speed*settings.clock_tick;
			num_cell = (int)floor( length/cell_length );
			//create_cell();
	}

void arc::create_cell(){
	double LL = length;
	first_cell = cell::size + 1;
	last_cell = first_cell + num_cell - 1;
	sprintf(Log->get_str(),"Arc#%03d: First Cell #%03d, Last Cell #%03d",id,first_cell,last_cell );
	Log->process(Log->get_str());
	int tmp_cell_size;
	while( LL >= 2*cell_length ){
		LL -= cell_length;
		//int id = cell::size+1;
		//double max_speed,max_flow,jam_density,delta;
		tmp_cell_size = cell::size + 1;
		cells[tmp_cell_size] = cell( tmp_cell_size,id,normal,cell_length );
	}
	if( nodes[down_node].get_type() == 2 ){
		tmp_cell_size = cell::size + 1;
		cells[tmp_cell_size] = cell( tmp_cell_size,id,destination,LL );
	}
	else{
		tmp_cell_size = cell::size + 1;
		cells[tmp_cell_size] = cell( tmp_cell_size,id,normal,LL );
	}
	if( nodes[up_node].get_type() == 1 ){
		cells[first_cell].set_type( origin );
	}

	for( int i = first_cell + 1; i <= last_cell; ++i ){
		cells[i].add_previous_cell( i-1 );
	}
	for( int i = first_cell; i < last_cell; ++i ){
		cells[i].add_next_cell( i+1 );
	}
}

int arc::size = 0;
