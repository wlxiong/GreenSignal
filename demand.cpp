#include "demand.h"

demand::demand():clock(0),traffic(-1.0){}

demand::demand( int cl,double tra ):clock(cl),traffic(tra){}

bool operator < ( const demand& a,const demand& b ){
	return a.clock < b.clock;
}

