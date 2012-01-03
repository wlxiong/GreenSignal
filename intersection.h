#ifndef _intersection_H

#define _intersection_H

#include "coordinate.h"
#include <vector>
#include "cell.h"
#include "arc.h"

using std::vector;

class phase{
public:
	int id;
	vector<int>  from_arc,to_arc;
	//phase();
	explicit phase( int i = 0 );
	void add_flow( int _from,int _to );
	void set_on();
	void set_off();
};

class intersection{
public:

	static int size;
	intersection();
	intersection( int i,int t,int px,int py );
	intersection( int i,int t,int px,int py,int min_g,int max_g,int rturn,int num_p );

	int get_id()const{			return id; }
	int get_type()const{		return type; }
	coordinate get_pos()const{	return pos; }
	int get_min_green()const{	return min_green; }
	int get_max_green()const{	return max_green; }
	int get_min_green_ticks()const{ return min_green_ticks; }
	int get_max_green_ticks()const{ return max_green_ticks; }
	bool get_right_turning()const{	return right_turning; }
	int get_num_phases()const{		return num_phases; }

	void add_node( int i );
	void add_phase( int i,int from_arc,int to_arc );
	void update_signal();

private:
	int id;
	int type;
	coordinate pos;
	int min_green,max_green;
	bool right_turning;
	int num_phases;
	vector<int> connected_nodes;
	vector<phase> phases;
	int min_green_ticks,max_green_ticks;
};

#endif

