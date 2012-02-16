/* kod nieuzywany - nie przeniesiony do konca z filterscriptu */

new mg_spedycja_cel[MAX_PLAYERS]={-1,...};
new mg_spedycja_naczepa[MAX_PLAYERS]={INVALID_VEHICLE_ID, ... };
new mg_spedycja_mapicon[MAX_PLAYERS];

enum e_BazaTransportowa {
	Float:ebt_loc[4],
	ebt_nazwa[32],
	ebt_mapicon,
	ebt_cp,
	
}

new BazyTransportowe[17][e_BazaTransportowa]={
	{	{-302.51,1508.92,75.35,20.0}, "Pustynna baza transportowa"	},
	{	{-490.30,-556.37,25.52,10.0}, "Lesna baza transportowa"	},
	{	{2228.13,-2222.40,13.54,10.0}, "Baza Ocean Docks"	},
	{	{-170.75,-324.31,1.42,10.0},	"Baza Red County"	},
	{	{2791.18,912.81,10.75,10.0},"Magazyn Rockshore LV"	},
	{	{-1530.28,463.64,7.18,10.0},"Easter Basin SF"	},
	{	{-2126.96,-88.05,35.32,10.0},"Doherty SF"	},
	{	{-1987.60,-1034.48,32.02,10.0},"Foster Valley SF"	},
	{	{-2465.56,2238.66,4.79,10.0}, "Bayside Marina"	},
	{	{-1967.44,-2436.49,30.62,10.0}, "Angel Pine"	},
	{	{2597.02,-2426.39,13.49,10.0},	"Ocean Docks II LS"	},
	{	{1545.91,1670.92,10.82,10.0},	"Lotnisko LV"	},
	{	{2072.34,-2284.84,13.10,10.0},	"Lotnisko LS"	},
	{	{-1240.57,-93.24,14.14,10.0}, "Lotnisko SF"	},
	{	{848.88,-1208.38,16.97,10.0},	"Studio filmowe LS"},
	{	{-1005.66,-1662.45,76.37,10.0},	"Forteca"},
	{	{2370.08,2740.72,10.82,10.0},	"Spinybed LV"}
};


mg_tiry_Init(){
	return;
/*	for(new i=0;i<sizeof BazyTransportowe;i++) {
		BazyTransportowe[i][ebt_cp]=CreateDynamicCP( BazyTransportowe[i][ebt_loc][0], BazyTransportowe[i][ebt_loc][1],BazyTransportowe[i][ebt_loc][2],BazyTransportowe[i][ebt_loc][3], 0, 0, -1);
		BazyTransportowe[i][ebt_mapicon]=CreateDynamicMapIcon(BazyTransportowe[i][ebt_loc][0], BazyTransportowe[i][ebt_loc][1],BazyTransportowe[i][ebt_loc][2], 51, 1, 0,0,-1,100);
//		Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, BazyTransportowe[i][ebt_mapicon], E_STREAMER_STYLE, 0);
	}*/
}

