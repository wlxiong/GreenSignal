
#include <iostream>
#include <vector>
#include "node.h"
#include "setting.h"
#include "arc.h"
#include "cell.h"
#include "base.h"
#include "update.h"
#include "coordinate.h"
#include "intersection.h"
#include "input.h"
#include "incident.h"
#include "debug.h"
#include "output.h"
#include "nodeopt.h"
#include "localopt.h"


using namespace std;


int present_clock;
setting settings;
node nodes[MAX_NODE];
arc arcs[MAX_ARC];
cell cells[MAX_CELL];
intersection intersections[MAX_INTERSECTION];
incident incidents[MAX_INCIDENT];

vector<int> origin_set,normal_set,diverge_set,merge_set,destination_set;
double exist_vehicle[MAX_CLOCK][MAX_CELL];
double diverge_flow[MAX_CELL][MAX_ADJ_CELL];
bool omega[MAX_CLOCK][MAX_INTERSECTION][MAX_PHASE];		//w -- timing plan

double origin_demand[MAX_CLOCK][MAX_ORIGIN_CELL];
demand temp_origin_demand[MAX_ORIGIN_CELL][MAX_CLOCK];
int temp_origin_demand_size = 0;


debug *Log;

int gg[] = { 5,2,3,4,5 };


double vehicle[MAX_CELL];

int main( int argc,char *argv[] ){
	//The cell length factor.
	settings.cell_length_factor = 2;

	char inputname[256] = "i_Test";

	//if( argc != 2 ){
	//	puts( "Please input the filename:" );
	//	gets( inputname );
	//}
	//else
	//	strcpy( inputname,argv[1] );

	//Input file.
	scanfile( inputname );
	//scanfile( "i_Test" );
	printf( "Input is complete.\n" );
	
	//Simulation.
	initial_control();
	initial_occupation( exist_vehicle[0] );
	double delay = simulate( exist_vehicle[0], vehicle, 0, settings.get_max_ticks() );
	printf( "Total delay: %7.2lf vehi*sec.\n", delay );
	

	//initial_occupation( exist_vehicle[0] );
	//delay = localopt(1);
	//printf( "localopt Total delay: %7.2lf vehi*sec.\n", delay );
	//printplan("localopt");
	//initial_diverge_flow();
	//delay = simulate( exist_vehicle[0], vehicle, 0, settings.get_max_ticks() );
	//printf( "_localopt delay: %7.2lf vehi*sec.\n", delay );


	initial_diverge_flow();
	initial_occupation( exist_vehicle[0] );
	delay = opt(1);
	printplan("opt");
	printf( "opt Total delay: %7.2lf vehi*sec.\n", delay );
	
	//Output occupation file.
	printfile(inputname, delay);
	printplan(inputname);
	initial_diverge_flow();
	delay = simulate( exist_vehicle[0], vehicle, 0, settings.get_max_ticks() );
	printf( "_opt delay: %7.2lf vehi*sec.\n", delay );
	printf( "Output is complete.\n" );
	
	delete Log;
	return 0;
}
/*
clock tick: 5
time 5:40

Input is complete.
1 Total delay: 12500.75 vehi*sec.
localopt Total delay: 12500.75 vehi*sec.
2 Total delay: 12500.75 vehi*sec.
time:001           0.00    0.00    0.00    0.00
time:002           0.00    0.00    0.00    0.00
time:003           0.00    0.00    0.00    0.00
time:004           0.00    0.00    0.00    0.00
time:005          11.75   19.25   11.50   19.00
time:006          35.25   57.75   34.50   57.00
time:007          82.25  133.25   83.50  132.25
time:008         152.75  245.75  158.50  244.75
time:009         210.50  351.50  224.75  351.00
time:010         291.50  486.50  317.25  487.00
time:011         373.50  613.25  397.25  614.75
time:012         481.50  760.75  500.75  763.25
time:013         580.75  859.50  591.50  863.00
time:014         706.25  981.50  705.50  986.00
time:015         819.25 1079.50  820.50 1085.00
time:016         955.75 1203.50  961.50 1211.00
time:017        1079.50 1318.75 1093.75 1328.75
time:018        1226.50 1463.50 1252.25 1472.75
time:019        1374.50 1596.75 1398.25 1604.25
time:020        1548.50 1753.50 1567.75 1761.75
time:021        1713.75 1897.50 1724.50 1906.50
time:022        1905.25 2064.75 1904.50 2074.50
time:023        2084.25 2233.00 2085.50 2244.00
time:024        2286.75 2427.25 2292.50 2440.50
time:025        2476.50 2613.75 2490.75 2628.25
time:026        2689.50 2828.50 2715.25 2842.25
time:027        2903.50 3032.00 2927.25 3043.75
time:028        3143.50 3259.00 3162.75 3268.75
time:029        3374.75 3473.25 3385.50 3483.50
time:030        3632.25 3710.75 3631.50 3721.50
time:031        3877.25 3949.25 3878.50 3962.00
time:032        4145.75 4213.75 4151.50 4228.50
time:033        4401.50 4472.50 4403.25 4486.25
time:034        4680.50 4757.50 4656.75 4770.25
time:035        4960.50 5030.00 4911.25 5041.75
time:036        5266.50 5327.25 5191.75 5336.75
time:037        5156.25 5611.75 5466.50 5543.75
time:038        5401.75 5919.50 5767.50 5810.25
time:039        5670.75 6228.25 6056.00 6078.25
time:040        5948.25 6565.00 6369.25 6372.25
time:041        6226.75 6280.25 6060.25 6094.25
time:042        6439.50 6605.75 6338.75 6375.50
time:043        6675.50 6956.00 6638.00 6694.00
time:044        6935.00 7249.50 6934.25 7049.75
time:045        7043.25 7276.00 6870.25 7160.25
time:046        7339.25 7460.50 7015.50 7469.00
time:047        7658.75 7777.75 7220.75 7783.75
time:048        8001.75 8119.50 7452.00 8113.75
time:049        7469.25 7933.00 7709.25 8083.25
time:050        7676.25 7859.50 7941.50 7963.50
time:051        7906.75 8151.50 8187.00 8260.75
time:052        8155.75 8472.75 8458.50 8584.00
time:053        8413.25 8303.50 8219.25 8432.25
time:054        8552.50 8587.00 8459.25 8718.75
time:055        8783.00 8898.75 8720.00 9031.25
time:056        9037.00 9169.50 8991.75 9251.25
time:057        9076.25 9197.50 9067.50 9305.25
time:058        9349.25 9465.00 9158.50 9519.00
time:059        9630.50 9778.50 9373.50 9833.00
time:060        9932.50 10123.50        9614.50 10173.00
time:061        9654.50 10125.75        9881.50 10119.50
time:062        9858.50 10150.50        10174.50        10018.50
time:063        10086.00        10455.50        10480.00        10309.75
time:064        10337.00        10787.75        10755.75        10638.25
time:065        10611.50        10612.50        10392.50        10480.00
time:066        10707.50        10896.50        10629.50        10761.25
time:067        10937.00        11205.25        10887.25        11072.25
time:068        11190.00        11537.50        11156.00        11406.75
7551
opt Total delay: 11156.00 vehi*sec.
3 Total delay: 11156.00 vehi*sec.
Output is complete.
Press any key to continue . . .

*/

/*

Input is complete.
1 Total delay: 3056.01 vehi*sec.
localopt Total delay: 3056.01 vehi*sec.
2 Total delay: 3056.01 vehi*sec.
time:001           0.00    0.00    0.00    0.00
time:002           0.00    0.00    0.00    0.00
time:003           0.00    0.00    0.00    0.00
time:004           0.00    0.00    0.00    0.00
time:005           0.00    0.00    0.00    0.00
time:006           0.00    0.00    0.00    0.00
time:007           0.00    0.00    0.00    0.00
time:008           0.00    0.00    0.00    0.00
time:009           0.00    0.00    0.00    0.00
time:010           0.00    0.00    0.00    0.00
time:011           0.00    0.00    0.00    0.00
time:012           0.00    0.00    0.00    0.00
time:013           0.00    0.00    0.00    0.00
time:014           0.00    0.00    0.00    0.00
time:015           0.00    0.00    0.00    0.00
time:016           0.00    0.00    0.00    0.00
time:017           0.00    0.00    0.00    0.00
time:018           0.00    0.00    0.00    0.00
time:019           0.00    0.00    0.00    0.00
time:020           0.00    0.00    0.00    0.00
time:021           0.00    0.00    0.00    0.00
time:022           0.00    0.00    0.00    0.00
time:023           0.00    0.00    0.00    0.00
time:024           0.47    0.77    0.46    0.76
time:025           1.41    2.31    1.38    2.28
time:026           2.82    4.62    2.76    4.56
time:027           4.70    7.70    4.60    7.60
time:028           7.05   11.55    6.90   11.40
time:029           9.87   16.17    9.66   15.96
time:030          13.16   21.56   12.88   21.28
time:031          17.39   28.43   17.14   28.09
time:032          22.56   36.78   22.44   36.39
time:033          28.67   46.61   28.78   46.18
time:034          35.72   57.92   36.16   57.46
time:035          43.71   70.71   44.58   70.23
time:036          52.64   84.98   54.04   84.49
time:037          62.51  100.73   64.54  100.24
time:038          73.32  117.96   76.08  117.48
time:039          85.07  136.67   88.66  136.21
time:040          97.76  156.86  102.28  156.43
time:041         111.39  178.53  116.94  178.14
time:042         125.96  201.68  132.64  201.34
time:043         141.47  226.31  149.38  226.03
time:044         150.71  244.91  160.17  244.71
time:045         160.88  264.68  172.01  264.58
time:046         171.98  285.62  184.90  285.64
time:047         184.01  307.73  198.84  307.89
time:048         196.97  331.01  213.83  331.33
time:049         210.86  355.46  229.87  355.96
time:050         225.68  381.08  246.96  381.78
time:051         236.54  402.39  258.03  403.33
time:052         248.44  422.96  270.16  424.16
time:053         261.73  444.52  283.23  445.93
time:054         276.06  467.02  297.24  468.64
time:055         291.43  490.46  312.19  492.29
time:056         307.84  514.84  328.08  516.88
time:057         325.29  540.16  344.91  542.41
time:058         344.02  566.50  362.80  568.90
time:059         363.91  593.82  381.87  596.37
time:060         384.84  622.08  402.13  624.88
time:061         406.81  651.28  423.33  654.33
time:062         430.17  681.47  445.47  684.72
time:063         454.57  712.60  468.55  716.05
time:064         472.32  730.26  484.64  733.91
time:065         491.24  748.89  501.66  752.70
time:066         511.45  768.53  519.61  772.48
time:067         532.71  789.10  538.49  793.19
time:068         555.02  810.60  558.30  814.83
time:069         578.38  833.03  579.29  837.50
time:070         602.79  856.39  601.21  861.10
time:071         620.46  870.84  618.67  875.74
time:072         639.19  886.33  637.17  891.42
time:073         658.86  903.33  657.06  908.53
time:074         679.47  921.37  677.99  926.70
time:075         701.02  940.45  699.96  945.91
time:076         723.51  960.57  722.97  966.16
time:077         746.94  981.73  747.02  987.45
time:078         771.43 1004.17  772.35 1010.12
time:079         797.10 1027.77  798.84 1033.95
time:080         823.96 1052.54  826.37 1058.84
time:081         851.76 1078.39  854.94 1084.81
time:082         880.50 1105.63  884.90 1112.17
time:083         910.18 1133.91  915.90 1140.57
time:084         932.87 1155.54  940.25 1162.32
time:085         956.49 1178.34  965.77 1185.24
time:086         981.04 1202.43  992.58 1209.45
time:087        1006.52 1227.61 1020.44 1234.81
time:088        1032.93 1253.92 1049.35 1261.28
time:089        1060.52 1281.33 1079.31 1288.80
time:090        1089.04 1309.79 1110.32 1317.37
time:091        1113.10 1331.51 1134.59 1339.20
time:092        1138.20 1354.29 1159.92 1362.09
time:093        1164.69 1378.01 1186.19 1385.92
time:094        1192.22 1402.67 1213.40 1410.69
time:095        1220.79 1428.39 1241.55 1436.44
time:096        1250.40 1455.05 1270.64 1463.25
time:097        1281.05 1482.65 1300.67 1491.00
time:098        1312.98 1511.36 1331.76 1519.81
time:099        1346.07 1541.25 1364.03 1549.80
time:100        1380.20 1572.33 1397.49 1580.98
time:101        1415.37 1604.35 1431.89 1613.10
time:102        1451.93 1637.39 1467.23 1646.18
time:103        1489.53 1671.41 1503.51 1680.24
time:104        1520.48 1698.44 1532.80 1707.31
time:105        1552.60 1726.40 1563.02 1735.41
time:106        1586.01 1755.29 1594.17 1764.44
time:107        1620.47 1785.16 1626.25 1794.40
time:108        1655.98 1815.96 1659.26 1825.29
time:109        1692.54 1847.98 1693.45 1857.36
time:110        1730.15 1881.01 1728.57 1890.42
time:111        1761.02 1909.58 1759.23 1919.02
time:112        1792.95 1939.19 1790.93 1948.66
time:113        1825.82 1970.19 1824.02 1979.69
time:114        1859.63 2002.23 1858.15 2011.86
time:115        1894.38 2035.31 1893.32 2045.07
time:116        1930.07 2069.48 1929.53 2079.32
time:117        1966.70 2104.81 1966.78 2114.65
time:118        2004.39 2141.42 2005.31 2151.28
time:119        2043.26 2179.19 2045.00 2189.07
time:120        2083.32 2218.00 2085.73 2227.90
235404
opt Total delay: 2083.32 vehi*sec.
3 Total delay: 2083.32 vehi*sec.
Output is complete.
Press any key to continue . . .

*/


