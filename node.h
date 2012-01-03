#ifndef _node_H

#define _node_H

#include "coordinate.h"

class node{
public:
	static int size;

	node();
	node( int i,int t,int _x,int _y,int ia = -1);

	int get_id()const{ return id; }
	int get_type()const{ return type; }
	int get_x()const{ return x; }
	int get_y()const{ return y; }
	int get_arc()const{ return adj_arc; }
	void set_arc( int i ){	adj_arc = i;	}
	coordinate get_pos()const{ return pos; }

private:
	int id;
	int type;
	int x,y;
	coordinate pos;
	int adj_arc;
};

#endif

