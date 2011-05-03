#include "soc.h"
#include "derivative.h"
#include "defines.h"


unsigned int TENSION [1][61];
unsigned int CAPACITE_60 [1][61];
unsigned int CAPACITE_40 [1][61];
unsigned int CAPACITE_25 [1][61];
unsigned int CAPACITE_0 [1][61];
unsigned int CAPACITE__10 [1][61];
unsigned int CAPACITE__20 [1][61]; 
unsigned int CAPACITE_MAX [401];
int NB_CYCLE=0;

extern int T_moyenne;
extern unsigned int lastSOC;
extern unsigned int presSOC;
extern unsigned int glowestV;
extern int gCellTemp[N_MOD_MAX][N_CELL_SLV_MAX];

//*************
//  Functions
//*************


void initTables(void)
{
   //Tension 

   TENSION [0][0] = 2700;
   TENSION [0][1] = 2725;
   TENSION [0][2] = 2750;
   TENSION [0][3] = 2775;
   TENSION [0][4] = 2800;
   TENSION [0][5] = 2825;
   TENSION [0][6] = 2850;
   TENSION [0][7] = 2875;
   TENSION [0][8] = 2900;
   TENSION [0][9] = 2925;
   TENSION [0][10] = 2950;
   TENSION [0][11] = 2975;
   TENSION [0][12] = 3000;
   TENSION [0][13] = 3025;
   TENSION [0][14] = 3050;
   TENSION [0][15] = 3075;
   TENSION [0][16] = 3100;
   TENSION [0][17] = 3125;
   TENSION [0][18] = 3150;
   TENSION [0][19] = 3175;
   TENSION [0][20] = 3200;
   TENSION [0][21] = 3225;
   TENSION [0][22] = 3250;
   TENSION [0][23] = 3275;
   TENSION [0][24] = 3300;
   TENSION [0][25] = 3325;
   TENSION [0][26] = 3350;
   TENSION [0][27] = 3375;
   TENSION [0][28] = 3400;
   TENSION [0][29] = 3425;
   TENSION [0][30] = 3450;
   TENSION [0][31] = 3475;
   TENSION [0][32] = 3500;
   TENSION [0][33] = 3525;
   TENSION [0][34] = 3550;
   TENSION [0][35] = 3575;
   TENSION [0][36] = 3600;
   TENSION [0][37] = 3625;
   TENSION [0][38] = 3650;
   TENSION [0][39] = 3675;
   TENSION [0][40] = 3700;
   TENSION [0][41] = 3725;
   TENSION [0][42] = 3750;
   TENSION [0][43] = 3775;
   TENSION [0][44] = 3800;
   TENSION [0][45] = 3825;
   TENSION [0][46] = 3850;
   TENSION [0][47] = 3875;
   TENSION [0][48] = 3900;
   TENSION [0][49] = 3925;
   TENSION [0][50] = 3950;
   TENSION [0][51] = 3975;
   TENSION [0][52] = 4000;
   TENSION [0][53] = 4025;
   TENSION [0][54] = 4050;
   TENSION [0][55] = 4075;
   TENSION [0][56] = 4100;
   TENSION [0][57] = 4125;
   TENSION [0][58] = 4150;
   TENSION [0][59] = 4175;
   TENSION [0][60] = 4200;

   //Capacité des cellules en fonction de V_min à 60deg
   CAPACITE_60 [0][0]=11041.67;
   CAPACITE_60 [0][1]=11041.67;
   CAPACITE_60 [0][2]=11041.67;
   CAPACITE_60 [0][3]=11020.83;
   CAPACITE_60 [0][4]=11000;
   CAPACITE_60 [0][5]=11000;
   CAPACITE_60 [0][6]=10979.16;
   CAPACITE_60 [0][7]=10963.53;
   CAPACITE_60 [0][8]=10958.33;
   CAPACITE_60 [0][9]=10937.5;
   CAPACITE_60 [0][10]=10921.87;
   CAPACITE_60 [0][11]=10906.25;
   CAPACITE_60 [0][12]=10890.63;
   CAPACITE_60 [0][13]=10875;
   CAPACITE_60 [0][14]=10859.37;
   CAPACITE_60 [0][15]=10843.75;
   CAPACITE_60 [0][16]=10812.5;
   CAPACITE_60 [0][17]=10796.88;
   CAPACITE_60 [0][18]=10750;
   CAPACITE_60 [0][19]=10718.75;
   CAPACITE_60 [0][20]=10687.5;
   CAPACITE_60 [0][21]=10656.25;
   CAPACITE_60 [0][22]=10625;
   CAPACITE_60 [0][23]=10531.25;
   CAPACITE_60 [0][24]=10468.75;
   CAPACITE_60 [0][25]=10421.875;
   CAPACITE_60 [0][26]=10375;
   CAPACITE_60 [0][27]=10250;
   CAPACITE_60 [0][28]=9968.75;
   CAPACITE_60 [0][29]=9687.5;
   CAPACITE_60 [0][30]=9375;
   CAPACITE_60 [0][31]=8937.5;
   CAPACITE_60 [0][32]=8312.5;
   CAPACITE_60 [0][33]=8000;
   CAPACITE_60 [0][34]=7625;
   CAPACITE_60 [0][35]=7187.5;
   CAPACITE_60 [0][36]=6687.5;
   CAPACITE_60 [0][37]=5906.25;
   CAPACITE_60 [0][38]=5250;
   CAPACITE_60 [0][39]=4718.75;
   CAPACITE_60 [0][40]=4281.25;
   CAPACITE_60 [0][41]=3875;
   CAPACITE_60 [0][42]=3562.5;
   CAPACITE_60 [0][43]=3312.5;
   CAPACITE_60 [0][44]=3093.75;
   CAPACITE_60 [0][45]=2750;
   CAPACITE_60 [0][46]=2531.25;
   CAPACITE_60 [0][47]=2281.25;
   CAPACITE_60 [0][48]=2000;
   CAPACITE_60 [0][49]=1781.25;
   CAPACITE_60 [0][50]=1531.25;
   CAPACITE_60 [0][51]=1312.5;
   CAPACITE_60 [0][52]=1093.75;
   CAPACITE_60 [0][53]=875;
   CAPACITE_60 [0][54]=656.25; 
   CAPACITE_60 [0][55]=468.75;
   CAPACITE_60 [0][56]=250;
   CAPACITE_60 [0][57]=0;
   CAPACITE_60 [0][58]=0;
   CAPACITE_60 [0][59]=0;
   CAPACITE_60 [0][60]=0;
                             
                              
   //Capacité des cellules en fonction de V_min à 40deg

   CAPACITE_40 [0][0] =10333.34;
   CAPACITE_40 [0][1] =10333.34;
   CAPACITE_40 [0][2] =10333.34;
   CAPACITE_40 [0][3] =10317.72;
   CAPACITE_40 [0][4] =10291.63;
   CAPACITE_40 [0][5] =10291.6;
   CAPACITE_40 [0][6] =10291.53;
   CAPACITE_40 [0][7] =10265.62;
   CAPACITE_40 [0][8] =10265.2;
   CAPACITE_40 [0][9] =10257.1;
   CAPACITE_40 [0][10] =10250;
   CAPACITE_40 [0][11] =10250;
   CAPACITE_40 [0][12] =10234.8;
   CAPACITE_40 [0][13] =10226.6;
   CAPACITE_40 [0][14] =10218.5;
   CAPACITE_40 [0][15] =10203.3;
   CAPACITE_40 [0][16] =10187.5;
   CAPACITE_40 [0][17] =10156.25;
   CAPACITE_40 [0][18] =10125;
   CAPACITE_40 [0][19] =10109.375;
   CAPACITE_40 [0][20] =10093.75;
   CAPACITE_40 [0][21] =10062.5;
   CAPACITE_40 [0][22] =10031.25;
   CAPACITE_40 [0][23] =9984.38;
   CAPACITE_40 [0][24] =9937.5;
   CAPACITE_40 [0][25] =9875;
   CAPACITE_40 [0][26] =9781.25;
   CAPACITE_40 [0][27] =9625;
   CAPACITE_40 [0][28] =9468.75;
   CAPACITE_40 [0][29] =9187.5;
   CAPACITE_40 [0][30] =8812.5;
   CAPACITE_40 [0][31] =8375;
   CAPACITE_40 [0][32] =7843.75;
   CAPACITE_40 [0][33] =7437.5;
   CAPACITE_40 [0][34] =7031.25;
   CAPACITE_40 [0][35] =6531.25;
   CAPACITE_40 [0][36] =5968.75;
   CAPACITE_40 [0][37] =5281.25;
   CAPACITE_40 [0][38] =4750;
   CAPACITE_40 [0][39] =4281.25;
   CAPACITE_40 [0][40] =3906.25;
   CAPACITE_40 [0][41] =3500;
   CAPACITE_40 [0][42] =3218.75;
   CAPACITE_40 [0][43] =2968.75;
   CAPACITE_40 [0][44] =2718.75;
   CAPACITE_40 [0][45] =2500;
   CAPACITE_40 [0][46] =2250;
   CAPACITE_40 [0][47] =1937.5;
   CAPACITE_40 [0][48] =1750;
   CAPACITE_40 [0][49] =1500;
   CAPACITE_40 [0][50] =1281.25;
   CAPACITE_40 [0][51] =1031.25;
   CAPACITE_40 [0][52] =843.75;
   CAPACITE_40 [0][53] =625;
   CAPACITE_40 [0][54] =437.5;
   CAPACITE_40 [0][55] =218.75;
   CAPACITE_40 [0][56] =0;
   CAPACITE_40 [0][57] =0;
   CAPACITE_40 [0][58] =0;
   CAPACITE_40 [0][59] =0;
   CAPACITE_40 [0][60] =0; 

   //Capacité des cellules en fonction de V_min à 25deg

   CAPACITE_25 [0][0] =10000;
   CAPACITE_25 [0][1] =10000;
   CAPACITE_25 [0][2] =9979;
   CAPACITE_25 [0][3] =9968;
   CAPACITE_25 [0][4] =9958;
   CAPACITE_25 [0][5] =9958;
   CAPACITE_25 [0][6] =9958;
   CAPACITE_25 [0][7] =9957;
   CAPACITE_25 [0][8] =9936;
   CAPACITE_25 [0][9] =9936;
   CAPACITE_25 [0][10] =9936;
   CAPACITE_25 [0][11] =9936;
   CAPACITE_25 [0][12] =9936;
   CAPACITE_25 [0][13] =9920;
   CAPACITE_25 [0][14] =9905;
   CAPACITE_25 [0][15] =9900;
   CAPACITE_25 [0][16] =9896;
   CAPACITE_25 [0][17] =9892;
   CAPACITE_25 [0][18] =9888;
   CAPACITE_25 [0][19] =9884;
   CAPACITE_25 [0][21] =9875;
   CAPACITE_25 [0][22] =9859;
   CAPACITE_25 [0][23] =9843;
   CAPACITE_25 [0][24] =9828;
   CAPACITE_25 [0][25] =9875;
   CAPACITE_25 [0][26] =9859;
   CAPACITE_25 [0][27] =9843;
   CAPACITE_25 [0][28] =9828;
   CAPACITE_25 [0][29] =9812;
   CAPACITE_25 [0][30] =9796;
   CAPACITE_25 [0][31] =9781;
   CAPACITE_25 [0][32] =9765;
   CAPACITE_25 [0][33] =9748;
   CAPACITE_25 [0][34] =7031.25;
   CAPACITE_25 [0][35] =6531.25;
   CAPACITE_25 [0][36] =5968.75;
   CAPACITE_25 [0][37] =5281.25;
   CAPACITE_25 [0][38] =4750;
   CAPACITE_25 [0][39] =4281.25;
   CAPACITE_25 [0][40] =3906.25;
   CAPACITE_25 [0][41] =3500;
   CAPACITE_25 [0][42] =3218.75;
   CAPACITE_25 [0][43] =2968.75;
   CAPACITE_25 [0][44] =2718.75;
   CAPACITE_25 [0][45] =2500;
   CAPACITE_25 [0][46] =2250;
   CAPACITE_25 [0][47] =1937.5;
   CAPACITE_25 [0][48] =1750;
   CAPACITE_25 [0][49] =1500;
   CAPACITE_25 [0][50] =1281.25;
   CAPACITE_25 [0][51] =1031.25;
   CAPACITE_25 [0][52] =843.75;
   CAPACITE_25 [0][53] =625;
   CAPACITE_25 [0][54] =437.5;
   CAPACITE_25 [0][55] =218.75;
   CAPACITE_25 [0][56] =0;
   CAPACITE_25 [0][57] =0;
   CAPACITE_25 [0][58] =0;
   CAPACITE_25 [0][59] =0;
   CAPACITE_25 [0][60] =0; 
   //Capacité des cellules en fonction de V_min à -20deg

   CAPACITE__20[0][0]=7406.25;
   CAPACITE__20[0][1]=7375;
   CAPACITE__20[0][2]=7343.75;
   CAPACITE__20[0][3]=7312.5;
   CAPACITE__20[0][4]=7250;
   CAPACITE__20[0][5]=7187.5;
   CAPACITE__20[0][6]=8140.63;
   CAPACITE__20[0][7]=8125;
   CAPACITE__20[0][8]=7000;
   CAPACITE__20[0][9]=0;
   CAPACITE__20[0][10]=0;
   CAPACITE__20[0][11]=0;
   CAPACITE__20[0][12]=0;
   CAPACITE__20[0][13]=0;
   CAPACITE__20[0][14]=0;
   CAPACITE__20[0][15]=0;
   CAPACITE__20[0][16]=0;
   CAPACITE__20[0][17]=0;
   CAPACITE__20[0][18]=0;
   CAPACITE__20[0][19]=0;
   CAPACITE__20[0][20]=0;
   CAPACITE__20[0][21]=0;
   CAPACITE__20[0][22]=0;
   CAPACITE__20[0][23]=0;
   CAPACITE__20[0][24]=0;
   CAPACITE__20[0][25]=0;
   CAPACITE__20[0][26]=0;
   CAPACITE__20[0][27]=0;
   CAPACITE__20[0][28]=0;
   CAPACITE__20[0][29]=0;
   CAPACITE__20[0][30]=0;
   CAPACITE__20[0][31]=0;
   CAPACITE__20[0][32]=0;
   CAPACITE__20[0][33]=0;
   CAPACITE__20[0][34]=0;
   CAPACITE__20[0][35]=0;
   CAPACITE__20[0][36]=0;
   CAPACITE__20[0][37]=0;
   CAPACITE__20[0][38]=0;
   CAPACITE__20[0][39]=0;
   CAPACITE__20[0][40]=0;
   CAPACITE__20[0][41]=0;
   CAPACITE__20[0][42]=0;
   CAPACITE__20[0][43]=0;
   CAPACITE__20[0][44]=0;
   CAPACITE__20[0][45]=0;
   CAPACITE__20[0][46]=0;
   CAPACITE__20[0][47]=0;
   CAPACITE__20[0][48]=0;
   CAPACITE__20[0][49]=0;
   CAPACITE__20[0][50]=0;
   CAPACITE__20[0][51]=0;
   CAPACITE__20[0][52]=0;
   CAPACITE__20[0][53]=0;
   CAPACITE__20[0][54]=0;
   CAPACITE__20[0][55]=0;
   CAPACITE__20[0][56]=0;
   CAPACITE__20[0][57]=0;
   CAPACITE__20[0][58]=0;
   CAPACITE__20[0][59]=0;
   CAPACITE__20[0][60]=0;
   //Capacité des cellules en fonction de V_min à -10deg

   CAPACITE__10[0][0]=8343.75;
   CAPACITE__10[0][1]=8342.75;
   CAPACITE__10[0][2]=8312.5;
   CAPACITE__10[0][3]=8296.87;
   CAPACITE__10[0][4]=8281.25;
   CAPACITE__10[0][5]=8265.62;
   CAPACITE__10[0][6]=8234.37;
   CAPACITE__10[0][7]=8218.75;
   CAPACITE__10[0][8]=8187.5;
   CAPACITE__10[0][9]=0;
   CAPACITE__10[0][10]=0;
   CAPACITE__10[0][11]=0;
   CAPACITE__10[0][12]=0;
   CAPACITE__10[0][13]=0;
   CAPACITE__10[0][14]=0;
   CAPACITE__10[0][15]=0;
   CAPACITE__10[0][16]=0;
   CAPACITE__10[0][17]=0;
   CAPACITE__10[0][18]=0;
   CAPACITE__10[0][19]=0;
   CAPACITE__10[0][20]=0;
   CAPACITE__10[0][21]=0;
   CAPACITE__10[0][22]=0;
   CAPACITE__10[0][23]=0;
   CAPACITE__10[0][24]=0;
   CAPACITE__10[0][25]=0;
   CAPACITE__10[0][26]=0;
   CAPACITE__10[0][27]=0;
   CAPACITE__10[0][28]=0;
   CAPACITE__10[0][29]=0;
   CAPACITE__10[0][30]=0;
   CAPACITE__10[0][31]=0;
   CAPACITE__10[0][32]=0;
   CAPACITE__10[0][33]=0;
   CAPACITE__10[0][34]=0;
   CAPACITE__10[0][35]=0;
   CAPACITE__10[0][36]=0;
   CAPACITE__10[0][37]=0;
   CAPACITE__10[0][38]=0;
   CAPACITE__10[0][39]=0;
   CAPACITE__10[0][40]=0;
   CAPACITE__10[0][41]=0;
   CAPACITE__10[0][42]=0;
   CAPACITE__10[0][43]=0;
   CAPACITE__10[0][44]=0;
   CAPACITE__10[0][45]=0;
   CAPACITE__10[0][46]=0;
   CAPACITE__10[0][47]=0;
   CAPACITE__10[0][48]=0;
   CAPACITE__10[0][49]=0;
   CAPACITE__10[0][50]=0;
   CAPACITE__10[0][51]=0;
   CAPACITE__10[0][52]=0;
   CAPACITE__10[0][53]=0;
   CAPACITE__10[0][54]=0;
   CAPACITE__10[0][55]=0;
   CAPACITE__10[0][56]=0;
   CAPACITE__10[0][57]=0;
   CAPACITE__10[0][58]=0;
   CAPACITE__10[0][59]=0;
   CAPACITE__10[0][60]=0;
   //Capacité des cellules en fonction de V_min à 25deg
   //doit etre remplacer par celui de 0 deg
                                
   CAPACITE_25[0][0]=10000;
   CAPACITE_25[0][1]=10000;
   CAPACITE_25[0][2]=9979.17;
   CAPACITE_25[0][3]=9968.75;
   CAPACITE_25[0][4]=9958.33;
   CAPACITE_25[0][5]=9958.33;
   CAPACITE_25[0][6]=9958.33;
   CAPACITE_25[0][7]=9957.33;
   CAPACITE_25[0][8]=9936.49;
   CAPACITE_25[0][9]=0;
   CAPACITE_25[0][10]=0;
   CAPACITE_25[0][11]=0;
   CAPACITE_25[0][12]=0;
   CAPACITE_25[0][13]=0;
   CAPACITE_25[0][14]=0;
   CAPACITE_25[0][15]=0;
   CAPACITE_25[0][16]=0;
   CAPACITE_25[0][17]=0;
   CAPACITE_25[0][18]=0;
   CAPACITE_25[0][19]=0;
   CAPACITE_25[0][20]=0;
   CAPACITE_25[0][21]=0;
   CAPACITE_25[0][22]=0;
   CAPACITE_25[0][23]=0;
   CAPACITE_25[0][24]=0;
   CAPACITE_25[0][25]=0;
   CAPACITE_25[0][26]=0;
   CAPACITE_25[0][27]=0;
   CAPACITE_25[0][28]=0;
   CAPACITE_25[0][29]=0;
   CAPACITE_25[0][30]=0;
   CAPACITE_25[0][31]=0;
   CAPACITE_25[0][32]=0;
   CAPACITE_25[0][33]=0;
   CAPACITE_25[0][34]=0;
   CAPACITE_25[0][35]=0;
   CAPACITE_25[0][36]=0;
   CAPACITE_25[0][37]=0;
   CAPACITE_25[0][38]=0;
   CAPACITE_25[0][39]=0;
   CAPACITE_25[0][40]=0;
   CAPACITE_25[0][41]=0;
   CAPACITE_25[0][42]=0;
   CAPACITE_25[0][43]=0;
   CAPACITE_25[0][44]=0;
   CAPACITE_25[0][45]=0;
   CAPACITE_25[0][46]=0;
   CAPACITE_25[0][47]=0;
   CAPACITE_25[0][48]=0;
   CAPACITE_25[0][49]=0;
   CAPACITE_25[0][50]=0;
   CAPACITE_25[0][51]=0;
   CAPACITE_25[0][52]=0;
   CAPACITE_25[0][53]=0;
   CAPACITE_25[0][54]=0;
   CAPACITE_25[0][55]=0;
   CAPACITE_25[0][56]=0;
   CAPACITE_25[0][57]=0;
   CAPACITE_25[0][58]=0;
   CAPACITE_25[0][59]=0;
   CAPACITE_25[0][60]=0;

   CAPACITE_MAX [0] = 10000;
   CAPACITE_MAX [1] = 10000;
   CAPACITE_MAX [2] = 10000;
   CAPACITE_MAX [3] = 10000;
   CAPACITE_MAX [4] = 10000;
   CAPACITE_MAX [5] = 10000;
   CAPACITE_MAX [6] = 10000;
   CAPACITE_MAX [7] = 10000;
   CAPACITE_MAX [8] = 10000;
   CAPACITE_MAX [9] = 10000;
   CAPACITE_MAX [10] = 10000;
   CAPACITE_MAX [11] = 10000;
   CAPACITE_MAX [12] = 10000;
   CAPACITE_MAX [13] = 10000;
   CAPACITE_MAX [14] = 10000;
   CAPACITE_MAX [15] = 10000;
   CAPACITE_MAX [16] = 10000;
   CAPACITE_MAX [17] = 10000;
   CAPACITE_MAX [18] = 10000;
   CAPACITE_MAX [19] = 10000;
   CAPACITE_MAX [20] = 10000;
   CAPACITE_MAX [21] = 10000;
   CAPACITE_MAX [22] = 10000;
   CAPACITE_MAX [23] = 10000;
   CAPACITE_MAX [24] = 10000;
   CAPACITE_MAX [25] = 10000;
   CAPACITE_MAX [26] = 9999;
   CAPACITE_MAX [27] = 9998;
   CAPACITE_MAX [28] = 9997;
   CAPACITE_MAX [29] = 9996;
   CAPACITE_MAX [30] = 9996;
   CAPACITE_MAX [31] = 9995;
   CAPACITE_MAX [32] = 9994;
   CAPACITE_MAX [33] = 9993;
   CAPACITE_MAX [34] = 9992;
   CAPACITE_MAX [35] = 9992;
   CAPACITE_MAX [36] = 9991;
   CAPACITE_MAX [37] = 9990;
   CAPACITE_MAX [38] = 9989;
   CAPACITE_MAX [39] = 9988;
   CAPACITE_MAX [40] = 9988;
   CAPACITE_MAX [41] = 9987;
   CAPACITE_MAX [42] = 9986;
   CAPACITE_MAX [43] = 9985;
   CAPACITE_MAX [44] = 9984;
   CAPACITE_MAX [45] = 9984;
   CAPACITE_MAX [46] = 9983;
   CAPACITE_MAX [47] = 9982;
   CAPACITE_MAX [48] = 9981;
   CAPACITE_MAX [49] = 9980;
   CAPACITE_MAX [50] = 9980;
   CAPACITE_MAX [51] = 9980;
   CAPACITE_MAX [52] = 9980;
   CAPACITE_MAX [53] = 9980;
   CAPACITE_MAX [54] = 9980;
   CAPACITE_MAX [55] = 9980;
   CAPACITE_MAX [56] = 9980;
   CAPACITE_MAX [57] = 9980;
   CAPACITE_MAX [58] = 9980;
   CAPACITE_MAX [59] = 9980;
   CAPACITE_MAX [60] = 9980; 
   CAPACITE_MAX [61] = 9980;
   CAPACITE_MAX [62] = 9980;
   CAPACITE_MAX [63] = 9980;
   CAPACITE_MAX [64] = 9979;
   CAPACITE_MAX [65] = 9979;
   CAPACITE_MAX [66] = 9979;
   CAPACITE_MAX [67] = 9979;
   CAPACITE_MAX [68] = 9979;
   CAPACITE_MAX [69] = 9979;  
   CAPACITE_MAX [70] = 9979;
   CAPACITE_MAX [71] = 9979;
   CAPACITE_MAX [72] = 9979;
   CAPACITE_MAX [73] = 9979;
   CAPACITE_MAX [74] = 9979;
   CAPACITE_MAX [75] = 9979;
   CAPACITE_MAX [76] = 9978;
   CAPACITE_MAX [77] = 9978;
   CAPACITE_MAX [78] = 9978;
   CAPACITE_MAX [79] = 9978;
   CAPACITE_MAX [80] = 9978;
   CAPACITE_MAX [81] = 9978;
   CAPACITE_MAX [82] = 9978;
   CAPACITE_MAX [83] = 9978;
   CAPACITE_MAX [84] = 9978;
   CAPACITE_MAX [85] = 9978;
   CAPACITE_MAX [86] = 9978;
   CAPACITE_MAX [87] = 9977;
   CAPACITE_MAX [88] = 9977;
   CAPACITE_MAX [89] = 9977; 
   CAPACITE_MAX [90] = 9977;
   CAPACITE_MAX [91] = 9977;
   CAPACITE_MAX [92] = 9977;
   CAPACITE_MAX [93] = 9977;
   CAPACITE_MAX [94] = 9977;
   CAPACITE_MAX [95] = 9977;
   CAPACITE_MAX [96] = 9977;
   CAPACITE_MAX [97] = 9977;
   CAPACITE_MAX [98] = 9976;
   CAPACITE_MAX [99] = 9976;                    
   CAPACITE_MAX [100] = 9976;
   CAPACITE_MAX [101] = 9976;
   CAPACITE_MAX [102] = 9976;
   CAPACITE_MAX [103] = 9976;
   CAPACITE_MAX [104] = 9976;
   CAPACITE_MAX [105] = 9976;
   CAPACITE_MAX [106] = 9976;
   CAPACITE_MAX [107] = 9976;
   CAPACITE_MAX [108] = 9976;
   CAPACITE_MAX [109] = 9976;
   CAPACITE_MAX [110] = 9976;
   CAPACITE_MAX [111] = 9975;
   CAPACITE_MAX [112] = 9975;
   CAPACITE_MAX [113] = 9975;
   CAPACITE_MAX [114] = 9975;
   CAPACITE_MAX [115] = 9975;
   CAPACITE_MAX [116] = 9975;
   CAPACITE_MAX [117] = 9975;
   CAPACITE_MAX [118] = 9975;
   CAPACITE_MAX [119] = 9975;
   CAPACITE_MAX [120] = 9975;
   CAPACITE_MAX [121] = 9975;
   CAPACITE_MAX [122] = 9975;
   CAPACITE_MAX [123] = 9975;
   CAPACITE_MAX [124] = 9975;
   CAPACITE_MAX [125] = 9975;
   CAPACITE_MAX [126] = 9975;
   CAPACITE_MAX [127] = 9975;
   CAPACITE_MAX [128] = 9975;
   CAPACITE_MAX [129] = 9975;
   CAPACITE_MAX [130] = 9975;
   CAPACITE_MAX [131] = 9975;
   CAPACITE_MAX [132] = 9975;
   CAPACITE_MAX [133] = 9975;
   CAPACITE_MAX [134] = 9975;
   CAPACITE_MAX [135] = 9975;
   CAPACITE_MAX [136] = 9975;
   CAPACITE_MAX [137] = 9975;
   CAPACITE_MAX [138] = 9975;
   CAPACITE_MAX [139] = 9975;
   CAPACITE_MAX [140] = 9975;
   CAPACITE_MAX [141] = 9975;
   CAPACITE_MAX [142] = 9975;
   CAPACITE_MAX [143] = 9975;
   CAPACITE_MAX [144] = 9975;
   CAPACITE_MAX [145] = 9975;
   CAPACITE_MAX [146] = 9975;
   CAPACITE_MAX [147] = 9975;
   CAPACITE_MAX [148] = 9975;
   CAPACITE_MAX [149] = 9975;
   CAPACITE_MAX [150] = 9975;
   CAPACITE_MAX [151] = 9975;
   CAPACITE_MAX [152] = 9975;
   CAPACITE_MAX [153] = 9975;
   CAPACITE_MAX [154] = 9975;
   CAPACITE_MAX [155] = 9975;
   CAPACITE_MAX [156] = 9975;
   CAPACITE_MAX [157] = 9975;
   CAPACITE_MAX [158] = 9975;
   CAPACITE_MAX [159] = 9975;
   CAPACITE_MAX [160] = 9975; 
   CAPACITE_MAX [161] = 9975;
   CAPACITE_MAX [162] = 9975;
   CAPACITE_MAX [163] = 9975;
   CAPACITE_MAX [164] = 9975;
   CAPACITE_MAX [165] = 9975;
   CAPACITE_MAX [166] = 9975;
   CAPACITE_MAX [167] = 9975;
   CAPACITE_MAX [168] = 9975;
   CAPACITE_MAX [169] = 9975;  
   CAPACITE_MAX [170] = 9975;
   CAPACITE_MAX [171] = 9975;
   CAPACITE_MAX [172] = 9975;
   CAPACITE_MAX [173] = 9975;
   CAPACITE_MAX [174] = 9975;
   CAPACITE_MAX [175] = 9975;
   CAPACITE_MAX [176] = 9975;
   CAPACITE_MAX [177] = 9975;
   CAPACITE_MAX [178] = 9975;
   CAPACITE_MAX [179] = 9975;
   CAPACITE_MAX [180] = 9975;
   CAPACITE_MAX [181] = 9975;
   CAPACITE_MAX [182] = 9975;
   CAPACITE_MAX [183] = 9975;
   CAPACITE_MAX [184] = 9975;
   CAPACITE_MAX [185] = 9975;
   CAPACITE_MAX [186] = 9975;
   CAPACITE_MAX [187] = 9975;
   CAPACITE_MAX [188] = 9975;
   CAPACITE_MAX [189] = 9975; 
   CAPACITE_MAX [190] = 9975;
   CAPACITE_MAX [191] = 9975;
   CAPACITE_MAX [192] = 9975;
   CAPACITE_MAX [193] = 9975;
   CAPACITE_MAX [194] = 9975;
   CAPACITE_MAX [195] = 9975;
   CAPACITE_MAX [196] = 9975;
   CAPACITE_MAX [197] = 9975;
   CAPACITE_MAX [198] = 9975;
   CAPACITE_MAX [199] = 9975; 
   CAPACITE_MAX [200] = 9975;
   CAPACITE_MAX [201] = 9975;
   CAPACITE_MAX [202] = 9975;
   CAPACITE_MAX [203] = 9975;
   CAPACITE_MAX [204] = 9975;
   CAPACITE_MAX [205] = 9975;
   CAPACITE_MAX [206] = 9975;
   CAPACITE_MAX [207] = 9975;
   CAPACITE_MAX [208] = 9975;
   CAPACITE_MAX [209] = 9975;
   CAPACITE_MAX [210] = 9975;
   CAPACITE_MAX [211] = 9975;
   CAPACITE_MAX [212] = 9975;
   CAPACITE_MAX [213] = 9975;
   CAPACITE_MAX [214] = 9975;
   CAPACITE_MAX [215] = 9975;
   CAPACITE_MAX [216] = 9975;
   CAPACITE_MAX [217] = 9975;
   CAPACITE_MAX [218] = 9975;
   CAPACITE_MAX [219] = 9975;
   CAPACITE_MAX [220] = 9975;
   CAPACITE_MAX [221] = 9975;
   CAPACITE_MAX [222] = 9975;
   CAPACITE_MAX [223] = 9975;
   CAPACITE_MAX [224] = 9975;
   CAPACITE_MAX [225] = 9975;
   CAPACITE_MAX [226] = 9975;
   CAPACITE_MAX [227] = 9975;
   CAPACITE_MAX [228] = 9975;
   CAPACITE_MAX [229] = 9975;
   CAPACITE_MAX [230] = 9975;
   CAPACITE_MAX [231] = 9975;
   CAPACITE_MAX [232] = 9975;
   CAPACITE_MAX [233] = 9975;
   CAPACITE_MAX [234] = 9975;
   CAPACITE_MAX [235] = 9975;
   CAPACITE_MAX [236] = 9975;
   CAPACITE_MAX [237] = 9975;
   CAPACITE_MAX [238] = 9975;
   CAPACITE_MAX [239] = 9975;
   CAPACITE_MAX [240] = 9975;
   CAPACITE_MAX [241] = 9975;
   CAPACITE_MAX [242] = 9975;
   CAPACITE_MAX [243] = 9975;
   CAPACITE_MAX [244] = 9975;
   CAPACITE_MAX [245] = 9975;
   CAPACITE_MAX [246] = 9975;
   CAPACITE_MAX [247] = 9975;
   CAPACITE_MAX [248] = 9975;
   CAPACITE_MAX [249] = 9975;
   CAPACITE_MAX [250] = 9975;
   CAPACITE_MAX [251] = 9975;
   CAPACITE_MAX [252] = 9975;
   CAPACITE_MAX [253] = 9975;
   CAPACITE_MAX [254] = 9975;
   CAPACITE_MAX [255] = 9975;
   CAPACITE_MAX [256] = 9975;
   CAPACITE_MAX [257] = 9975;
   CAPACITE_MAX [258] = 9975;
   CAPACITE_MAX [259] = 9975;
   CAPACITE_MAX [260] = 9975; 
   CAPACITE_MAX [261] = 9975;
   CAPACITE_MAX [262] = 9975;
   CAPACITE_MAX [263] = 9975;
   CAPACITE_MAX [264] = 9975;
   CAPACITE_MAX [265] = 9975;
   CAPACITE_MAX [266] = 9975;
   CAPACITE_MAX [267] = 9975;
   CAPACITE_MAX [268] = 9975;
   CAPACITE_MAX [269] = 9975;  
   CAPACITE_MAX [270] = 9975;
   CAPACITE_MAX [271] = 9975;
   CAPACITE_MAX [272] = 9975;
   CAPACITE_MAX [273] = 9975;
   CAPACITE_MAX [274] = 9975;
   CAPACITE_MAX [275] = 9975;
   CAPACITE_MAX [276] = 9975;
   CAPACITE_MAX [277] = 9975;
   CAPACITE_MAX [278] = 9975;
   CAPACITE_MAX [279] = 9975;
   CAPACITE_MAX [280] = 9975;
   CAPACITE_MAX [281] = 9975;
   CAPACITE_MAX [282] = 9975;
   CAPACITE_MAX [283] = 9975;
   CAPACITE_MAX [284] = 9975;
   CAPACITE_MAX [285] = 9975;
   CAPACITE_MAX [286] = 9975;
   CAPACITE_MAX [287] = 9975;
   CAPACITE_MAX [288] = 9975;
   CAPACITE_MAX [289] = 9975; 
   CAPACITE_MAX [290] = 9975;
   CAPACITE_MAX [291] = 9975;
   CAPACITE_MAX [292] = 9975;
   CAPACITE_MAX [293] = 9975;
   CAPACITE_MAX [294] = 9975;
   CAPACITE_MAX [295] = 9975;
   CAPACITE_MAX [296] = 9975;
   CAPACITE_MAX [297] = 9975;
   CAPACITE_MAX [298] = 9975;
   CAPACITE_MAX [299] = 9975; 
   CAPACITE_MAX [300] = 9975;
   CAPACITE_MAX [301] = 9975;
   CAPACITE_MAX [302] = 9975;
   CAPACITE_MAX [303] = 9975;
   CAPACITE_MAX [304] = 9975;
   CAPACITE_MAX [305] = 9975;
   CAPACITE_MAX [306] = 9975;
   CAPACITE_MAX [307] = 9975;
   CAPACITE_MAX [308] = 9975;
   CAPACITE_MAX [309] = 9975;
   CAPACITE_MAX [310] = 9975;
   CAPACITE_MAX [311] = 9975;
   CAPACITE_MAX [312] = 9975;
   CAPACITE_MAX [313] = 9975;
   CAPACITE_MAX [314] = 9975;
   CAPACITE_MAX [315] = 9975;
   CAPACITE_MAX [316] = 9975;
   CAPACITE_MAX [317] = 9975;
   CAPACITE_MAX [318] = 9975;
   CAPACITE_MAX [319] = 9975;
   CAPACITE_MAX [320] = 9975;
   CAPACITE_MAX [321] = 9975;
   CAPACITE_MAX [322] = 9975;
   CAPACITE_MAX [323] = 9975;
   CAPACITE_MAX [324] = 9975;
   CAPACITE_MAX [325] = 9975;
   CAPACITE_MAX [326] = 9975;
   CAPACITE_MAX [327] = 9975;
   CAPACITE_MAX [328] = 9975;
   CAPACITE_MAX [329] = 9975;
   CAPACITE_MAX [330] = 9975;
   CAPACITE_MAX [331] = 9975;
   CAPACITE_MAX [332] = 9975;
   CAPACITE_MAX [333] = 9975;
   CAPACITE_MAX [334] = 9975;
   CAPACITE_MAX [335] = 9975;
   CAPACITE_MAX [336] = 9975;
   CAPACITE_MAX [337] = 9975;
   CAPACITE_MAX [338] = 9975;
   CAPACITE_MAX [339] = 9975;
   CAPACITE_MAX [340] = 9975;
   CAPACITE_MAX [341] = 9975;
   CAPACITE_MAX [342] = 9975;
   CAPACITE_MAX [343] = 9975;
   CAPACITE_MAX [344] = 9975;
   CAPACITE_MAX [345] = 9975;
   CAPACITE_MAX [346] = 9975;
   CAPACITE_MAX [347] = 9975;
   CAPACITE_MAX [348] = 9975;
   CAPACITE_MAX [349] = 9975;
   CAPACITE_MAX [350] = 9975;
   CAPACITE_MAX [351] = 9975;
   CAPACITE_MAX [352] = 9975;
   CAPACITE_MAX [353] = 9975;
   CAPACITE_MAX [354] = 9975;
   CAPACITE_MAX [355] = 9975;
   CAPACITE_MAX [356] = 9975;
   CAPACITE_MAX [357] = 9975;
   CAPACITE_MAX [358] = 9975;
   CAPACITE_MAX [359] = 9975;
   CAPACITE_MAX [360] = 9975; 
   CAPACITE_MAX [361] = 9975;
   CAPACITE_MAX [362] = 9975;
   CAPACITE_MAX [363] = 9975;
   CAPACITE_MAX [364] = 9975;
   CAPACITE_MAX [365] = 9975;
   CAPACITE_MAX [366] = 9975;
   CAPACITE_MAX [367] = 9975;
   CAPACITE_MAX [368] = 9975;
   CAPACITE_MAX [369] = 9975;  
   CAPACITE_MAX [370] = 9975;
   CAPACITE_MAX [371] = 9975;
   CAPACITE_MAX [372] = 9975;
   CAPACITE_MAX [373] = 9975;
   CAPACITE_MAX [374] = 9975;
   CAPACITE_MAX [375] = 9975;
   CAPACITE_MAX [376] = 9975;
   CAPACITE_MAX [377] = 9975;
   CAPACITE_MAX [378] = 9975;
   CAPACITE_MAX [379] = 9975;
   CAPACITE_MAX [380] = 9975;
   CAPACITE_MAX [381] = 9975;
   CAPACITE_MAX [382] = 9975;
   CAPACITE_MAX [383] = 9975;
   CAPACITE_MAX [384] = 9975;
   CAPACITE_MAX [385] = 9975;
   CAPACITE_MAX [386] = 9975;
   CAPACITE_MAX [387] = 9975;
   CAPACITE_MAX [388] = 9975;
   CAPACITE_MAX [389] = 9975; 
   CAPACITE_MAX [390] = 9975;
   CAPACITE_MAX [391] = 9975;
   CAPACITE_MAX [392] = 9975;
   CAPACITE_MAX [393] = 9975;
   CAPACITE_MAX [394] = 9975;
   CAPACITE_MAX [395] = 9975;
   CAPACITE_MAX [396] = 9975;
   CAPACITE_MAX [397] = 9975;
   CAPACITE_MAX [398] = 9975;
   CAPACITE_MAX [399] = 9975; 
   CAPACITE_MAX [400] = 9975;  
}

      
/*
* calcul_avec_tension:  trouve la cellule la moins chargé et garde son voltage. 
* Parameters: V_min         : le voltage de la cellul la moins chargée
* Return :    T_moyenne     : Température moyenne des cellules
*/

void calcul_avec_tension (void){

   int i;

                          

   if (T_moyenne <= -150) {
   
      if(TENSION[0][0]>=glowestV) {
         lastSOC= (10000-CAPACITE__20[1][0]);
      } 
      else if(TENSION[0][60]<=glowestV && glowestV<=4300)
      {
         lastSOC= (10000- CAPACITE__20[1][60]);
      } 
      else
      {  
         for (i=0;i<=60; i++){       
         // si la valeur de la tension est comprise entre la valeur 
         //i et i+2 du tableau, soc égale à soc de i+1.

            if(TENSION[0][i]<glowestV && TENSION[0][i+2]>glowestV ) {                   
               lastSOC= (10000-CAPACITE__20[1][i+1]);
            }
         }
      }
   }

   else if (T_moyenne>-150 && T_moyenne <= -50)
   {
      if(TENSION[0][0]>=glowestV) {
          lastSOC= (10000-CAPACITE__10[1][0]);
        
      } 
      else if(TENSION[0][60]<=glowestV && glowestV<=4300){
          lastSOC= (10000-CAPACITE__10[1][60]);
          
      } 
      else
      {  
          for (i=0;i<=60; i++){   
          // si la valeur de la tension est comprise entre la valeur 
         //i et i+2 du tableau, soc égale à soc de i+1    
              if(TENSION[0][i]<glowestV && TENSION[0][i+2]>glowestV ) {
                lastSOC= (10000-CAPACITE__10[1][i+1]);
               
              }
          }
      }
   }

   else if (T_moyenne>-50 && T_moyenne <= 100) {
     if(TENSION[0][0]>=glowestV) {
         lastSOC= (10000-CAPACITE_0[1][0]);
         
     } 
     else if(TENSION[0][60]<=glowestV && glowestV<=4300){
         lastSOC= (10000-CAPACITE_0[1][60]);
         
     } 
     else  {  
         for (i=0;i<=60; i++){    
         // si la valeur de la tension est comprise entre la valeur 
         //i et i+2 du tableau, soc égale à soc de i+1   
            if(TENSION[0][i]<glowestV && TENSION[0][i+2]>glowestV ) {
               
               lastSOC= (10000-CAPACITE_0[1][i+1]);
          
             
            }
            else {
                //erreur la tension est plus grande que Vmax
            }
          }
      }
   }

   else if (T_moyenne> 100 && T_moyenne <= 350) {
      if(TENSION[0][0]>=glowestV) {
         lastSOC= (10000-CAPACITE_25[1][0]); 
      }
      else if(TENSION[0][60]<=glowestV && glowestV<=4300){
         lastSOC= (10000-CAPACITE_25[1][60]); 
      
      }
      else {

         for (i=0;i<=60; i++){       
         // si la valeur de la tension est comprise entre la valeur 
         //i et i+2 du tableau, soc égale à soc de i+1
            if(TENSION[0][i]<glowestV && TENSION[0][i+2]>glowestV ) {
                           
                lastSOC= (10000-CAPACITE_25[1][i+1]);
               
              }

            else {
                 //erreur la tension est plus grande que Vmax
            }

         }
       }
   }

   else if (T_moyenne> 350 && T_moyenne <= 500) {
            if(TENSION[0][0]>=glowestV) {
               lastSOC= (10000-CAPACITE_40[1][0]);
                
            } 
            else if(TENSION[0][60]<=glowestV && glowestV<=4300){
               lastSOC= (CAPACITE_40[1][60]-10000);
                
            } 
         else  {  
            for (i=0;i<=60; i++){       
            // si la valeur de la tension est comprise entre la valeur 
               //i et i+2 du tableau, soc égale à soc de i+1
                if(TENSION[0][i]<glowestV && TENSION[0][i+2]>glowestV ) {
                    if(CAPACITE_40[1][i] >=10000) {
                       lastSOC= (CAPACITE_40[1][i+1]-10000);
                    } 
                    else {
                       lastSOC= (10000-CAPACITE_40[1][i+1]);
                    }
    
                   
                 }
                 else {
                     //erreur la tension est plus grande que Vmax
                 }
            }
        }
   }

   else  {
         if(TENSION[0][0]>=glowestV) {
             lastSOC= (10000-CAPACITE_60[1][0]);
              
          } 
         else if(TENSION[0][60]<=glowestV && glowestV<=4300){
             lastSOC= (CAPACITE_60[1][60]-10000);
               
          } 
         else 
          {  
             for (i=0;i<=60; i++){    
             // si la valeur de la tension est comprise entre la valeur 
               //i et i+2 du tableau, soc égale à soc de i+1   
                 if(TENSION[0][i]<glowestV && TENSION[0][i+2]>glowestV ) {
                     if(CAPACITE_60[1][i] >=10000) {
                        lastSOC= (CAPACITE_60[1][i+1]-10000);
                        } 
                     else {
                         lastSOC= (10000-CAPACITE_60[1][i+1]);
                        }
    
                
                 }
                  else {
                      //erreur la tension est plus grande que Vmax
                 }
              }
           }
   } 
}
//*************
//  Functions
//*************      
/*
* calcul_voltage_min:  trouve la cellule la moins chargé et garde son voltage. 
* Parameters: CellVolt [] : tableaux contenant les voltages de toutes les cellules
* Return :    voltage_min
*/
/*
unsigned int  calcul_V_min (unsigned int *CellVolt [], unsigned int V_min) {
  
     int j;
     int i;
     
     V_min = CellVolt [0][0];
     
      for (j=0 ; j <= N_CELL_SLV_MAX ; j++) 
         {
           for(i=0 ; N_MOD_MAX<= 1 ; i++) 
             {
               if(CellVolt[i][j]< V_min)
                 {
                   V_min = CellVolt[i][j];
                 }    
              }     
          }
     
     return (V_min);  
    }
    */ 
     
//*************
//  Functions
//*************     
/*
* calcul_temperature_voltage: Calcul la temperature moyenne. 
* Parameters: CellTemp [] : tableau  conteant les températures de toutes les cellules
*             temperature_moyenne : température moyenne des cellules 
* Return :    temperature_moyenne
*/
void  calcul_temperature_moyenne (void){
   
     int j=0;
     int i=0;
     int NB_Cell=0;
     int T_Tot=0;
     
    for( j=0 ; j< N_CELL_SLV_MAX ; j++) 
       {
         for(i=1 ; i < N_MOD_MAX; i++) 
            { 
              T_Tot= T_Tot + gCellTemp[i][j];
              NB_Cell = NB_Cell++;
            }         
       }
        T_moyenne = T_Tot / NB_Cell;   
  
}  
    
    

//*************
//  Functions
//*************  
      
/*
* calcul_avec_courant: calcul le soc en integrant le courant entrant. 
* 
*
* Parameters: Cellcurent  : vecteur conteant les 100 derniéres mesures de courant
*             lasrpres    : la derniere valeur de soc calculée 
*             soc         : noeveau soc 
* Return : soc
*/

/*
unsigned int calcul_avec_courant (long int * gCurrentMsr,unsigned int presSOC,unsigned int lastSOC){
  
  int i;
  long  int somme_integral = 0;
  long  int tang = 0;
  long  int result_integral =0;
  long  int premiere_valeur=0;
  long  int deuxieme_valeur=0;
  
   
  presSOC=lastSOC;
    for (i=0;i<= 10;i++) {   
        if (premiere_valeur>0) {
              if( deuxieme_valeur>0) {
                 premiere_valeur = gCurrentMsr[i];
                 deuxieme_valeur=gCurrentMsr[i+1];
                 tang=((deuxieme_valeur)-(premiere_valeur));
                 tang=tang*100*1500/2;
                 result_integral= ((tang/100)+(premiere_valeur *1500));
                 somme_integral = somme_integral + result_integral;
                 lastSOC = lastSOC + (100*somme_integral/CAPACITE_MAX[NB_CYCLE]);
                 lastSOC=result_integral;
                 } 
             else {
                
              i++;
                 premiere_valeur=gCurrentMsr[i];
                 deuxieme_valeur =gCurrentMsr[i+1];
                    if(deuxieme_valeur<0) {
                       tang=((deuxieme_valeur)-(premiere_valeur));
                       result_integral= (((tang/2)*1500)+(premiere_valeur *1500));
                       somme_integral = somme_integral + result_integral;
                       lastSOC = (lastSOC + 100*somme_integral)*100/CAPACITE_MAX[NB_CYCLE];
                    } 
                    else {
                    }
              }
        } 
        else  {
              if( deuxieme_valeur<0) {
                
                 tang=((deuxieme_valeur)-(premiere_valeur))/1500;
                 result_integral= (((tang/2)*1500*1500)+(premiere_valeur *1500));
                 somme_integral = somme_integral + result_integral;
                 lastSOC = (lastSOC + 100*somme_integral)*100/CAPACITE_MAX[NB_CYCLE];
              } 
              else {
                
              i++;
                 premiere_valeur=gCurrentMsr[i];
                 deuxieme_valeur =gCurrentMsr[i+1];
                    if(deuxieme_valeur>0) {
                       tang=((deuxieme_valeur)-(premiere_valeur))/1500;
                       result_integral= (((tang/2)*1500*1500)+(premiere_valeur *1500));
                       somme_integral = somme_integral + result_integral;
                       lastSOC = (lastSOC + 100*somme_integral)*100/CAPACITE_MAX[NB_CYCLE];
                    } 
                    else {
                    }
        
        }
       
    }         
     return lastSOC;


}
}
*/

int nb_cycle_cellules (unsigned int lastSOC,unsigned int presSOC, int NB_CYCLE) {
  
  if (lastSOC>  presSOC & presSOC <=1000) {
     NB_CYCLE++;
  }
  return NB_CYCLE;
}

