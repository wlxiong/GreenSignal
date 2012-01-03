#ifndef _cell_H

#define _cell_H

#include "base.h"
#include "arc.h"
#include "demand.h"
#include <set>
#include <vector>
#include "setting.h"


extern setting settings;
extern double origin_demand[MAX_CLOCK][MAX_ORIGIN_CELL];
extern demand temp_origin_demand[MAX_ORIGIN_CELL][MAX_CLOCK];
extern int temp_origin_demand_size;
extern int present_clock;

using std::set;
using std::vector;
namespace Cell{
	enum cell_type{
		origin = 1,destination,diverge,normal,merge
	};
};

using Cell::cell_type;
using Cell::merge;
using Cell::origin;
using Cell::destination;
using Cell::diverge;
using Cell::normal;


class cell{
public:
	
	cell();
	cell( int i,int arc,cell_type t,double len );
	static int size;

	int			get_id()			const{ return id;			}
	cell_type	get_type()			const{ return type;			}
	int			get_on_arc()		const{ return on_arc;		}
	double		get_max_speed()		const{ return max_speed;	}
	double		get_max_flow()		const{ return max_flow;		}
	double		get_jam_density()	const{ return jam_density;	}
	double		get_delta()			const{ return delta;		}
	double		get_length()		const{ return length;		}
	double		get_in_flow()		const{ return in_flow;		}
	double		get_out_flow()		const{ return out_flow;		}
	double		get_max_vehicle()	const{ return max_vehicle;	}
	int			get_num_demand()	const{ return num_demand;	}
	int			get_temp_origin_demand_id()const{ return temp_origin_demand_id; }

	void origin_update_flow(){	in_flow = origin_demand[present_clock][temp_origin_demand_id];    }

	void set_type(cell_type t){	type = t;	}
	void set_max_speed( double ms ){ max_speed = ms; }
	void set_max_flow( double mf ){ max_flow = mf*settings.clock_tick; }
	void set_jam_density( double jd ){ jam_density = jd; }
	void set_delta( double del ){ delta = del; }
	void set_length( double len ){ length = len; }
	void set_in_flow( double in ){ in_flow = in; }

	void set_out_flow( double out, int next_cell_id = -1);
	void add_previous_cell( int i );
	void add_next_cell( int i );
	void add_diverge_coeff( double g );

	void add_next_cell( int i,double g );
	void add_next_cell( int i,double g,int t );

	void add_demand( int clock,double traffic );
	double get_demand();

	void diverge_update_flow();
	void merge_update_flow();
	void destination_update_flow();
	void normal_update_flow();

	double send_flow(int cn = 0);
	double receive_flow();

	void set_signals_on( int next_cell_id );
	void set_signals_off( int next_cell_id );
	double move_vehicle();

private:

	int			id;
	cell_type	type;
	int			on_arc;
	double		length;
	double		max_speed,
				max_flow,
				jam_density,delta;
	
	vector<int>		previous_cell,next_cell,signals,turning;
	vector<double>	diverge_coeff;

	double	in_flow,
			out_flow;

	double	max_vehicle;

	int temp_origin_demand_id,num_demand;

};

#endif


