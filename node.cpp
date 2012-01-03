#include "node.h"
#include "debug.h"

extern debug *Log;


node::node():id(0),type(0),x(0),y(0),pos(x,y){ }
node::node( int i,int t,int _x,int _y,int ia ):id(i),type(t),
		x(_x),y(_y),pos(x,y),adj_arc(ia){ 
			size++;
}


int node::size = 0;
