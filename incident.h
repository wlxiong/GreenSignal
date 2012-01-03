#ifndef _incident_H

#define _incident_H

class incident{
public:
	static int size;
	incident();
	incident( int aa,double dist,int st,int et,double mf );
	void occur();
private:
	int id;
	int at_arc;
	double distance;
	int at_cell;
	int start_time,end_time;
	double max_flow;
	int start_clock,end_clock;
};


#endif

