#ifndef _coordinate_H

#define _coordinate_H

class coordinate{
public:
	int x,y;
	coordinate();
	coordinate( int _x,int _y);
	double dist( coordinate a );
};


#endif



