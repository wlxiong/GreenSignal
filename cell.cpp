#include "cell.h"
#include "setting.h"
#include <cfloat>
#include <climits>
#include "debug.h"
#include <assert.h>
//extern const int MAX_CELL;
//extern const int MAX_CLOCK;
extern arc arcs[];
extern double exist_vehicle[MAX_CLOCK][MAX_CELL];
extern int present_clock;
extern setting settings;
extern cell cells[MAX_CELL];
extern double diverge_flow[MAX_CELL][MAX_ADJ_CELL];
//extern int signals[MAX_CLOCK][MAX_CELL];
extern debug *Log;

using Cell::merge;

cell::cell():id(0),on_arc(0),in_flow(-1.0),out_flow(-1.0){
	if( !signals.empty() ) signals.clear();
	if( !previous_cell.empty() ) previous_cell.clear();
	if( !next_cell.empty() ) next_cell.clear();
	if( !turning.empty() ) turning.clear();
	if( !diverge_coeff.empty() ) diverge_coeff.clear();
	temp_origin_demand_id = -1;
	num_demand = 0;
}

cell::cell( int i,int arc,cell_type t,double len ):
	id(i),type(t),on_arc(arc),length(len){
		assert(size<MAX_CELL);
		char str[256];
		size++;
		sprintf( str,"Create Cell#%03d successfully",size );
		Log->process( str );
		max_speed = arcs[arc].get_max_speed();
		max_flow = arcs[arc].get_max_flow()*settings.clock_tick;
		jam_density = arcs[arc].get_jam_density();
		delta = arcs[arc].get_delta();
		if( !signals.empty() ) signals.clear();
		if( !previous_cell.empty() ) previous_cell.clear();
		if( !next_cell.empty() ) next_cell.clear();
		if( !turning.empty() ) turning.clear();
		if( !diverge_coeff.empty() ) diverge_coeff.clear();
		max_vehicle = length*jam_density;
		temp_origin_demand_id = -1;
		num_demand = 0;
	}


void cell::set_out_flow( double out, int next_cell_id){ 
	int i;
	if(type == diverge){
		for(i=0;i < (int)next_cell.size();i++)
			if(next_cell_id == next_cell[i])
				break;
		diverge_flow[id][i] -= out;
		diverge_flow[id][i] += in_flow*diverge_coeff[i];
		out_flow += out;
	}
	else
		out_flow = out; 
}


void cell::add_demand( int clock,double traffic ){
	char str[256];
	sprintf( str,"Origin Cell#%03d Added Demand(Start Time:%03d, Traffic:%5.2lf)",id,clock,traffic );
	Log->process( str );
	if( temp_origin_demand_id < 0 ) temp_origin_demand_id = temp_origin_demand_size++;
	temp_origin_demand[temp_origin_demand_id][num_demand++] = demand( clock,traffic );
}

void cell::add_next_cell( int i ){
	if( i < 0 ){
		Log->throws("In cell::add_next_cell( int i ) : the i out range." );
		exit(1);
	}
	for( int ii = 0; ii < (int)next_cell.size(); ++ii ) 
		if( next_cell[ii] == i ) return;
	next_cell.push_back(i);
	if( get_type() == normal && next_cell.size() > 1 ){
		set_type( diverge );
	}
	if( signals.size() < next_cell.size() ) 
		signals.push_back(1);
}


void cell::add_previous_cell( int i ){
	if( i < 0 ){
		Log->throws("In cell::add_previous_cell( int i ) : the i out range." );
		exit(1);
	}
	for( int ii = 0; ii < (int)previous_cell.size(); ++ii ) 
		if( previous_cell[ii] == i ) return;
	previous_cell.push_back(i);

	if( get_type() == normal && previous_cell.size() > 1 ){
		set_type( Cell::merge );
	}
}


void cell::add_diverge_coeff( double g ){
	diverge_coeff.push_back(g);
	if( get_type() == normal && next_cell.size() > 1 ){
		set_type( diverge );
	}
	if( signals.size() < diverge_coeff.size() )
		signals.push_back(1);
}

void cell::add_next_cell( int i,double g ){
	next_cell.push_back(i);
	if( get_type() == normal && next_cell.size() > 1 ){
		set_type( diverge );
	}
	diverge_coeff.push_back(g);
	if( signals.size() < next_cell.size() ) 
		signals.push_back(1);
}

void cell::add_next_cell( int i,double g,int t ){
	if( i < 0 ){
		Log->throws("In cell::add_previous_cell( int i,double g,int t ) : the i out range." );
		exit(1);
	}
	next_cell.push_back(i);
	diverge_coeff.push_back(g);
	if( get_type() == normal && next_cell.size() > 1 ){
		set_type( diverge );
	}
	turning.push_back(t);
	if( signals.size() < next_cell.size() ) 
		signals.push_back(1);
}

double cell::send_flow(int cn){ 
	double S;
	int i;
	S = std::min( max_flow,exist_vehicle[present_clock - 1][id] );
	if(type != diverge)
		return S;
	else{
		for(i=0;i<(int)next_cell.size();i++)
			if(next_cell[i] == cn)
				break;
		return std::min( max_flow*diverge_coeff[i],diverge_flow[id][i] * signals[i] );
	}
}

double cell::receive_flow(){
	double R;
	R = std::min( max_flow,delta*(max_vehicle-exist_vehicle[present_clock - 1][id]) );
	return R;
}


void cell::diverge_update_flow(){
	double pre_send_flow;
	//In flow.
	pre_send_flow = cells[previous_cell[0]].send_flow(id);
	in_flow = std::min( pre_send_flow, receive_flow() );
	cells[previous_cell[0]].set_out_flow(in_flow,id);
	//Diverge flow.
	/*for( int i = 0; i < (int)next_cell.size(); ++i )
		remainder_flow += diverge_flow[id][i];
	may_send_flow = std::min(max_flow, exist_vehicle[present_clock-1][id] );
	new_diverge_flow = may_send_flow - remainder_flow;

	for( int i = 0; i < (int)next_cell.size(); ++i ){
		diverge_flow[id][i] += new_diverge_flow * diverge_coeff[i];
	}*/
	out_flow = 0.0;
}


void cell::merge_update_flow(){

	double sum_send_flow = 0.0, pre_send_flow, this_receive_flow, part_in_flow;

	for( int i = 0; i < (int)previous_cell.size(); ++i ){
		assert( previous_cell[i]>=0 );
		sum_send_flow += cells[previous_cell[i]].send_flow(id);
	}
	in_flow = 0.0;
	for( int i = 0; i < (int)previous_cell.size(); ++i ){
		pre_send_flow = cells[previous_cell[i]].send_flow(id);
		this_receive_flow = pre_send_flow/sum_send_flow * receive_flow();
		part_in_flow = std::min( pre_send_flow, this_receive_flow );
		in_flow += part_in_flow;
		//Set flow for previous cell.
		cells[previous_cell[i]].set_out_flow( part_in_flow, id);
	}
}


void cell::destination_update_flow(){
	in_flow = cells[previous_cell[0]].send_flow();
	out_flow = send_flow();
	cells[previous_cell[0]].set_out_flow(in_flow,id);
}

void cell::normal_update_flow(){
	double pre_send_flow;
	assert( previous_cell[0]>=0 );
	pre_send_flow = cells[previous_cell[0]].send_flow(id);
	in_flow = std::min( pre_send_flow, receive_flow() );
	cells[previous_cell[0]].set_out_flow(in_flow,id);
}

void cell::set_signals_on( int next_cell_id ){
	for( int i = 0; i < (int)next_cell.size(); ++i ){
		if( next_cell[i] == next_cell_id ){
			signals[i] = 1; break;
		}
	}
}

void cell::set_signals_off( int next_cell_id ){
	for( int i = 0; i < (int)next_cell.size(); ++i ){
		if( next_cell[i] == next_cell_id ){
			signals[i] = 0; break;
		}
	}
}


double cell::move_vehicle(){

	double delay = exist_vehicle[present_clock - 1][id] - get_out_flow();
	exist_vehicle[present_clock][id] = delay + get_in_flow();
	//Log flows and states.
	sprintf(Log->get_str(),"Cell#%04d: In Flow(%7.2lf ), Out Flow(%7.2lf ), Exist vehicles (%7.2lf ), Delay (%7.2lf )",
		id,get_in_flow(),get_out_flow(), exist_vehicle[present_clock][id], delay );
	Log->process(Log->get_str());
	if(get_in_flow() - max_flow > settings.epsilon )
		Log->warning("Exceed maximum flow! ^^^^^^");
	if(get_out_flow() - max_flow > settings.epsilon )
		Log->warning("Exceed maximum flow!\t\t   ^^^^^^");
	if(exist_vehicle[present_clock][id] - max_vehicle > settings.epsilon)
		Log->warning("Exceed maximum vehicle!\t\t\t\t\t    ^^^^^^");
	
	return delay;
}

int cell::size = 0;
