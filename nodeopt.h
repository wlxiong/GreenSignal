#ifndef _nodeopt_H

#define _nodeopt_H
#include "base.h"

class nodeopt{
public:
	double delay;
	int clock;
	nodeopt* pre_nodeopt;
	int phase;
	double vehicle[MAX_CELL];
	double diverge_flow[MAX_CELL][MAX_ADJ_CELL];

};

#endif
