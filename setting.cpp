#include "setting.h"
#include <cmath>

setting::setting():start_time(0),end_time(0),clock_tick(1),
		epsilon(0.01),initial_occupation(0),initial_control(0){}

setting::setting( int st,int et,int ct,double ep,int io,int ic ):
		start_time(st),end_time(et),clock_tick(ct),
		epsilon(ep),initial_occupation(io),initial_control(ic){}

