#ifndef _localopt_H

#define _localopt_H

#include "update.h"
#include "base.h"
#include "cell.h"
#include "arc.h"
#include "demand.h"
#include "node.h"
#include "setting.h"
#include "intersection.h"
#include "debug.h"
#include "nodeopt.h"

extern int			present_clock;
extern setting		settings;
extern node			nodes[MAX_NODE];
extern arc			arcs[MAX_ARC];
extern cell			cells[MAX_CELL];
extern vector<int>	origin_set,normal_set,diverge_set,merge_set,destination_set;
extern double		exist_vehicle[MAX_CLOCK][MAX_CELL];
extern double		diverge_flow[MAX_CELL][MAX_ADJ_CELL];
extern intersection intersections[MAX_INTERSECTION];
extern incident		incidents[MAX_INCIDENT];
extern bool			omega[MAX_CLOCK][MAX_INTERSECTION][MAX_PHASE];
extern debug		*Log;

nodeopt inode[MAX_CLOCK][MAX_PHASE];

void initial_diverge_flow(){
	for( int i = 0; i < MAX_CELL; ++i ){
		for( int  j = 0; j < MAX_PHASE; ++j ) diverge_flow[i][j] = 0.0;
	}
}

double localopt( int id ){ // intersection id

	inode[0][0].pre_nodeopt = NULL;
	inode[0][0].clock = 0;
	inode[0][0].delay = 0.0;
	for( int  i = 1; i <= intersections[id].get_max_green_ticks(); ++i ){
		for( int ip = 0; ip < intersections[id].get_num_phases(); ++ip ){

			modify_control( id, 1, i, ip );
			//initial_occupation( inode[i][ip].vehicle );

			initial_diverge_flow();

			memcpy( inode[i][ip].vehicle,exist_vehicle[0],sizeof(inode[i][ip].vehicle) );

			inode[i][ip].delay = simulate( inode[i][ip].vehicle,inode[i][ip].vehicle, 0, i ) ;

			inode[i][ip].phase = ip;
			inode[i][ip].clock = i;
			inode[i][ip].pre_nodeopt = &inode[0][0];
			memcpy( inode[i][ip].diverge_flow,diverge_flow,sizeof(inode[i][ip].diverge_flow) );

		}
	}
	double vehicle[MAX_CELL];
	long start = clock();
	for( int  i = intersections[id].get_min_green_ticks(); i <= settings.get_max_ticks(); ++i ){
		int st = max( 0,i-intersections[id].get_max_green_ticks() );
		int et = i-intersections[id].get_min_green_ticks();
		if( st == et ) continue;
		for( int ip = 0; ip < intersections[id].get_num_phases(); ++ip ){
			inode[i][ip].delay = MAX_DELAY;
			//if( st == et ) continue;
			inode[i][ip].phase = ip;
			inode[i][ip].clock = i;

			for( int  j = st; j <= et; ++j ){

				modify_control( id, j+1, i, ip );
				

				for( int jp = 0; jp < intersections[id].get_num_phases(); ++jp ){
					if( ip == jp ) continue;

					//memcpy( diverge_flow,inode[j][jp].diverge_flow,sizeof(diverge_flow) );
					for( int d = 1; d <= cell::size; ++d ){
						for( int p = 0; p < MAX_ADJ_CELL; ++p ){
							diverge_flow[d][p] = inode[j][jp].diverge_flow[d][p];
						}
					}
					double tmpdelay = inode[j][jp].delay+simulate( inode[j][jp].vehicle,vehicle,j,i );

					if( tmpdelay < inode[i][ip].delay ){
						inode[i][ip].delay = tmpdelay;
						//memcpy( inode[i][ip].vehicle,vehicle,sizeof(inode[i][ip].vehicle) );
						for( int in = 1; in <= cell::size; ++in ){
							inode[i][ip].vehicle[in] = vehicle[in];
						}
						inode[i][ip].pre_nodeopt = &inode[j][jp];
						//memcpy( inode[i][ip].diverge_flow,diverge_flow,sizeof(inode[i][ip].diverge_flow) );
						for( int d = 1; d <= cell::size; ++d ){
							for( int p = 0; p < MAX_ADJ_CELL; ++p ){
								inode[i][ip].diverge_flow[d][p] = diverge_flow[d][p];
							}
						}
					}
				}

			}   
		}
		printf( "%ld\n",clock()-start );
	}
	printf( "%ld\n",clock()-start );
	int best_phase = 0;
	for( int i = settings.get_max_ticks(),ip = 1; ip < intersections[id].get_num_phases(); ++ip ){
		if( inode[i][ip].delay < inode[i][best_phase].delay ){
			best_phase = ip;
		}
	}
	
	for( nodeopt *p = &inode[settings.get_max_ticks()][best_phase]; p->pre_nodeopt; p = p->pre_nodeopt ){
		modify_control( id, p->pre_nodeopt->clock+1, p->clock, p->phase );
	}
	return inode[settings.get_max_ticks()][best_phase].delay;
}

double opt( int id ){
	inode[0][0].pre_nodeopt = NULL;
	inode[0][0].clock = 0;
	inode[0][0].delay = 0.0;
	//Initialize.
	double vehicle[MAX_CELL];
	for( int i = 0; i <= settings.get_max_ticks(); ++i ){
		for( int  ip = 0; ip < intersections[id].get_num_phases(); ++ip ){
			inode[i][ip].delay = MAX_DELAY;
			inode[i][ip].clock = i;
			inode[i][ip].phase = ip;
			inode[i][ip].pre_nodeopt = NULL;
		}
	}

	//First phase.
	for( int op = 0; op < intersections[id].get_num_phases(); ++op ){

		modify_control( id,0,intersections[id].get_max_green_ticks(),op );
		double tmpdelay = 0.0;
		initial_occupation( vehicle );
		initial_diverge_flow();
		for( int i = 1; i <= cell::size; ++i ) 
			vehicle[i] = exist_vehicle[0][i];

		for( int t = 1; t <= intersections[id].get_max_green_ticks(); ++t ){
			
			tmpdelay += simulate( vehicle,vehicle,t-1,t );
			inode[t][op].delay = tmpdelay;
			for( int in = 1; in <= cell::size; ++in ){
				inode[t][op].vehicle[in] = vehicle[in];
			}
			inode[t][op].pre_nodeopt = &inode[0][op];
			for( int d = 1; d <= cell::size; ++d ){
				for( int p = 0; p < MAX_ADJ_CELL; ++p ){
					inode[t][op].diverge_flow[d][p] = diverge_flow[d][p];
				}
			}	
		}
	}
	
	long start = clock();
	for( int  i = 1; i <= settings.get_max_ticks(); ++i ){
		int et = std::min( i+intersections[id].get_max_green_ticks(),settings.get_max_ticks() );
		int st = i+intersections[id].get_min_green_ticks();
		if( st > settings.get_max_ticks() ) break;
		
		for( int  ip = 0; ip < intersections[id].get_num_phases(); ++ip ){
			for( int jp = 0; jp < intersections[id].get_num_phases(); ++jp ){
				if( ip == jp ) continue;
				modify_control( id, i, et, jp );
				//Load diverge flow.
				for( int d = 1; d <= cell::size; ++d ){
					for( int p = 0; p <= MAX_ADJ_CELL; ++p ){
						diverge_flow[d][p] = inode[i][ip].diverge_flow[d][p];
					}
				}

				double tmpdelay = simulate( inode[i][ip].vehicle,vehicle,i,st-1 )+inode[i][ip].delay;
				for( int t = st; t <= et; ++t ){
					tmpdelay += simulate( vehicle,vehicle,t-1,t );
					if( tmpdelay < inode[t][jp].delay ){
						inode[t][jp].delay = tmpdelay;
						for( int in = 1; in <= cell::size; ++in ){
							inode[t][jp].vehicle[in] = vehicle[in];
						}
						inode[t][jp].pre_nodeopt = &inode[i][ip];
						for( int d = 1; d <= cell::size; ++d ){
							for( int p = 0; p < MAX_ADJ_CELL; ++p ){
								inode[t][jp].diverge_flow[d][p] = diverge_flow[d][p];
							}
						}
					}
				}
			}
		}
	}
	for( int i = 1; i <= settings.get_max_ticks(); ++i ){
		printf( "time:%03d",i );
		for( int ip = 0; ip < intersections[id].get_num_phases(); ++ip ){
			printf( "\t%7.2lf",inode[i][ip].delay );
		}
		putchar('\n');
	}
	printf( "%ld\n",clock()-start );

	int best_phase = 0;
	for( int i = settings.get_max_ticks(),ip = 1; ip < intersections[id].get_num_phases(); ++ip ){
		if( inode[i][ip].delay < inode[i][best_phase].delay ){
			best_phase = ip;
		}
	}
	
	for( nodeopt *p = &inode[settings.get_max_ticks()][best_phase]; p->pre_nodeopt; p = p->pre_nodeopt ){
		modify_control( id, p->pre_nodeopt->clock+1, p->clock, p->phase );
	}
	return inode[settings.get_max_ticks()][best_phase].delay;
}


#endif

