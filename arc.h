#ifndef _arc_H

#define _arc_H 1

#include "node.h"
#include "setting.h"

class arc{
public:
	static int size;

	arc();

	arc( int i,int un,int dn,double ms,double mf,double jd,double del );

	int get_id()const{ return id; }
	int get_up_node()const{ return up_node; }
	int get_down_node()const{ return down_node; }
	double get_max_speed()const{ return max_speed; }
	double get_max_flow()const{ return max_flow; }
	double get_jam_density()const{ return jam_density; }
	double get_delta()const{ return delta; }
	double get_length()const{ return length; }
	int get_num_cell()const{ return num_cell; }

	void set_max_speed( double ms ){ max_speed = ms; }
	void set_max_flow( int mf ){ max_flow = mf; }
	void set_jam_density( double jd ){ jam_density = jd; }
	void set_delta( double d ){ delta = d; }
	
	int get_first_cell()const{ return first_cell; }
	int get_last_cell()const{ return last_cell; }
	double get_cell_length()const{ return cell_length; }

	void create_cell();

private:
	int		id;
	int		up_node,down_node;
	double	max_speed,max_flow,jam_density,delta;
	double	length;
	double	cell_length;
	int		num_cell;
	int		first_cell,last_cell;	

};


#endif


