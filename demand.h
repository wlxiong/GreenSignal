#ifndef _demand_H
#define _demand_H

class demand{
	friend bool operator < ( const demand&,const demand& );
public:
	int clock;
	double traffic;
	demand();
	demand( int cl,double tra );
	
};


#endif

