#ifndef _output_H

#define _output_H

#include "arc.h"
#include "cell.h"
#include "intersection.h"
#include "coordinate.h"

extern double exist_vehicle[][MAX_CELL];
extern intersection intersections[MAX_INTERSECTION];
extern cell cells[MAX_CELL];
extern setting settings;

void printfile(char namestr[], double delay){
	FILE *out;
	char outputname[256] = "o_";
	strcat(outputname,&namestr[2]);
	strcat(outputname,".txt");
	out = fopen( outputname,"w" );

	fputs("Green Signal Cell Occupation Output File.\n",out);
	fputs("----------------------------------------\n",out);
	fprintf( out,"Output File: \t%s\n",outputname );
	time_t run;
	time(&run);
	localtime(&run);
	fprintf( out,"The Date: \t%s",ctime(&run) );
	fprintf( out,"Total delay: \t %7.2lf vehi*sec (%7.2lf vehi*ticks).\n\n", delay*settings.clock_tick, delay );

	int i,j,k;

	fprintf( out,"%d arcs, %d intersections, %d ticks\n\n",arc::size,intersection::size,settings.get_max_ticks());
	//Output arcs and cells.
	coordinate first,last,interval;
	for( i = 1; i <= arc::size; ++i ){
		first = nodes[arcs[i].get_up_node()].get_pos();
		last = nodes[arcs[i].get_down_node()].get_pos();
		interval.x = (last.x - first.x)/(arcs[i].get_num_cell()+1);
		interval.y = (last.y - first.y)/(arcs[i].get_num_cell()+1);
		
		fprintf( out,"arc %d\n",i );
		fprintf( out,"upnode (%d, %d), downnode (%d, %d)\n",first.x,first.y,last.x,last.y);
		fprintf( out,"%d cells\n",arcs[i].get_num_cell() );
		//Output cell coordinates.		
		for( j=1, k = arcs[i].get_first_cell(); k <=arcs[i].get_last_cell(); k++, j++){
			fprintf( out,"cell %d, coordinate (%d, %d), length %.0lf\n",k, first.x + j*interval.x, first.y + j*interval.y, cells[k].get_length() );
		}
		//Output cell states.
		for( j = 0; j <= settings.get_max_ticks(); ++j ){
			fprintf( out,"#%04d\t",j );
			for( k = arcs[i].get_first_cell(); k <=arcs[i].get_last_cell(); k++){
				fprintf( out,"%6.1lf\t",exist_vehicle[j][k] );
			}
			fprintf( out,"\n" );
		}
		fprintf( out,"\n" );
	}

	//Output intersections.
	coordinate pos;
	for( i = 1; i <= intersection::size; ++i ){
		fprintf( out,"intersection %d\n",i );
		pos = intersections[i].get_pos();
		fprintf( out,"coordinate (%d, %d)\n",pos.x,pos.y );
		//Output phases.
		//for( j = 0, j < intersections[i].get_num_phases(), j++){
		//	intersections[i].phases
		//}
	}


	fclose(out);
}

void printplan( char namestr[] ){
	FILE *out;
	char outputname[256];
	sprintf( outputname,"t_%s.txt",namestr+2 );
	out = fopen( outputname,"w" );

	fputs("Green Signal Time Planning File.\n",out);
	fputs("----------------------------------------\n",out);
	fprintf( out,"Output File: \t%s\n",outputname );
	time_t run;
	time(&run);
	localtime(&run);
	fprintf( out,"The Date: \t%s",ctime(&run) );
	fprintf( out,"%d intersections, %d ticks\n\n",intersection::size,settings.get_max_ticks() );
	for( int i = 1; i <= intersection::size; ++i ){
		fprintf( out,"intersection %d\n",i );
		fprintf( out,"%d phases\n",intersections[i].get_num_phases() );
		fprintf( out,"%%No.  " );
		for( int ip = 1; ip <= intersections[i].get_num_phases(); ++ip ){
			fprintf( out," (%d)",ip );
		}
		fprintf(out,"\n");
		for( int it = 1; it <= settings.get_max_ticks(); ++it ){
			fprintf(out,"@%05d",it );
			for( int ip = 0; ip < intersections[i].get_num_phases(); ++ip ){
				fprintf( out,"%4d",omega[it][i][ip]?1:0 );
			}
			fprintf( out,"\n" );
		}
		fputs( "",out );
	}
	fclose(out);
}

#endif

