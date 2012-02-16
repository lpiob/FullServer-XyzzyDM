/*
yakuza skins: 117,118,120,186
*/
#define MIN_PICKUP_DELAY 2500
#define MIN_TELECP_DELAY 4000

forward Pobiekty_odswiezPomniki();

enum e_telepickup {
	etp_pickupid,
	Float:etp_tPX,	Float:etp_tPY,	Float:etp_tPZ,	Float:etp_tPA,
	etp_tI, etp_tVW,
	bool:etp_pokazopis,
	etp_opis[64],
	Text3D:etp_t3d,
	etp_mapicon,
	etp_restrict_gang,
	etp_restrict_level
}

#define MAX_TELEPICKUPS 400
#define MAX_RETURNPICKUPS 200
#define MAX_TELECHECKPOINTS	100
#define MAX_MISCPICKUPS 250
#define MAX_MAPICONS 100

new TelePickupsCount=0, ReturnPickupsCount=0, TeleCPCount=0, MapIconsCount=0, MiscPickupsCount=0;
//new etp_telepickup:TelePickups[MAX_TELEPICKUPS];
new TelePickups[MAX_TELEPICKUPS][e_telepickup];
new ReturnPickups[MAX_RETURNPICKUPS];
new MiscPickups[MAX_MISCPICKUPS];
new MapIcons[MAX_MAPICONS];

enum e_telecheckpoint {
	etcp_cp[2],
	Float:etcp_P1[4],
	Float:etcp_P2[4],
	etcp_I[2],
	etcp_opis1[64],
	etcp_opis2[64],
	Text3D:etcp_t3d[2]
}

new TeleCheckpoints[MAX_TELECHECKPOINTS][e_telecheckpoint];


enum e_DynamicAreas {
    eDA_andromeda,
	eDA_jail,
	eDA_tuneleKrzyk,	// prostopadloscian o wysokosc 1j, przecinajacy kanaly - po przejsciu przez niego rozlegnie sie krzyk
	eDA_getto
//DA_vip1	//	cala baza vipow
}

enum e_Pickups {
	eP_lv_balkony,
//	eP_powerpickup_baza3,
//	eP_powerpickup_przystanekLS,
//	eP_powerpickup_przystanekLV
	eP_F_policja_LS,
	eP_F_policja_LV,
	eP_F_policja_SF,
	eP_F_wojsko1,
	eP_F_wojsko2,
	eP_F_dilerzy
//	eP_F_wojsko3
}

enum e_Object {
	eO_ls_getto_brama,
//	eO_sf_yakuza_brama
}

enum e_StrefyNoDM {
	ea_warsztat,
	ea_statek,
	ea_kosciol,
	ea_kosciolINT,
	ea_wyspa,
	ea_bankPC,		// palomino creek
//	ea_hacjenda,
	ea_burdel,
	ea_skatepark,
	ea_szpital,
	ea_kart,
	ea_discoLV,	
//	ea_discoCHILIAD,
	ea_klatka,	// klatka KSW/ZTP/WWK
	ea_WWK,	// wlki w klatkach
	ea_KSW	// w onplayerdeath jest specjalny wyjatek aby nie pakowac do paki za zabojstwo tutaj
}

enum e_StrefyFullDM {
	ea_wojsko,
	ea_minigun,
	ea_kopalnia,
	ea_arenasolo1,
	ea_tunele,
	ea_wietnam
}

enum e_PomnikiRodzaje {
	ea_misc1,
	ea_czasgry,
	ea_skill,
	ea_lowskill,
	ea_derby,
	ea_kickban,
	ea_racedist,
	ea_soloklrs,	// solo killers
	ea_fotocash,
	ea_walizki
}

enum e_checkpoints {
	ec_warsztat_tablice,
	ec_warsztatlv_skup,
	ec_warsztatlv_myjnia,
	ec_warsztatlv_drift
}


//enum e_StrefyFullDM { }

new DAs[e_DynamicAreas];	// rozne strefy dynamiczne
new Os[e_Object];
new StrefyNoDM[e_StrefyNoDM];
new StrefyFullDM[e_StrefyFullDM];
new PICKUPs[e_Pickups];		// rozne pickupy
new Text3D:TekstyPomniki[e_PomnikiRodzaje][4];
new Text3D:PPRanking[3];

new Text3D:ArenaRanking[ARENA_MAX][2];	// ranking najlepszych na danej arenie


new Checkpoints[e_checkpoints];
new Fotoradary[9];

new KillZones[11];	// strefy po wejsciu do ktorych nastepuje smierc - np. spadniecie pod arene na derby

new neony[10][2];

CreateObjects()
{
	// neony
	for(new i=0;i<10;i++) {
		neony[i][0]=CreateObject(18647+(i%6),0,0,0,0,0,0,300.0);
		neony[i][1]=CreateObject(18647+(i%6),0,0,0,0,0,0,300.0);
	}

	Os[eO_ls_getto_brama]=CreateDynamicObject(2990, 2423.7770996094, -1774.4482421875, 16.460855484009, 0, 0, 270,0,0,-1);                                                                                                                                                                  
	DAs[eDA_getto]=CreateDynamicRectangle(2419.26, -1779.64,2444, -1769.28,0,0,-1);   
//	Checkpoints[ec_solo_starter_rotunda]=CreateDynamicCP(-1138.52,-1016.10,129.24,3, VW_UNIQUE_ARENASOLO, 9,	-1,150);
//	PICKUPs[eP_powerpickup_baza3]=CreateDynamicPickup(354,2,1925.31,2428.40,15.72,	0,0,-1, 25.0);
//	PICKUPs[eP_powerpickup_przystanekLS]=CreateDynamicPickup(354,2,1165.53,-1720.05,14.23,0,0,-1,150.0);

	PICKUPs[eP_F_policja_LS]=CreateDynamicPickup(1247,2,256.86,69.47,1003.64,0,6,-1,150.0);
	PICKUPs[eP_F_policja_LV]=CreateDynamicPickup(1247,2,229.13,164.49,1003.02,0,3,-1,150.0);
	PICKUPs[eP_F_policja_SF]=CreateDynamicPickup(1247,2,246.79,123.24,1003.21,0,10,-1,150.0);

	PICKUPs[eP_F_wojsko1]=CreateDynamicPickup(1254,2,101.61,1739.23,20.33,0,0,-1,150.0);
	PICKUPs[eP_F_wojsko2]=CreateDynamicPickup(1254,2,223.80,1822.85,6.41,0,0,-1,150.0);

	PICKUPs[eP_F_dilerzy]=CreateDynamicPickup(1241,2,264.65,1395.45,-7.90,0,0,-1,150.0);
//	PICKUPs[eP_F_wojsko3]=CreateDynamicPickup(1254,2,246.79,123.24,1003.21,0,10,-1,150.0);




//	PICKUPs[eP_powerpickup_przystanekLV]=CreateDynamicPickup(354,2,2104.72,  965.66,11.10,0,0,-1,150.0);

	// rankingi przestepcow na posterunkach
	// sf, lv, ls
	PPRanking[0]=CreateDynamic3DTextLabel("Poszukiwani przestepcy\r\n\r\n{ffffff}- brak -",0xff0000ff, 252.9, 113, 1006, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, 10, -1, 50);
	PPRanking[1]=CreateDynamic3DTextLabel("Poszukiwani przestepcy\r\n\r\n{ffffff}- brak -",0xff0000ff, 228.6, 148.5, 1004.6, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, 3, -1, 50);
	PPRanking[2]=CreateDynamic3DTextLabel("Poszukiwani przestepcy\r\n\r\n{ffffff}- brak -",0xff0000ff, 255.18, 62.7, 1005.0, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, 6, -1, 50);



	ArenaRanking[ARENA_ONEDE][0]=CreateDynamic3DTextLabel("Najlepsi na /ONEDE\r\n\r\n{ffffff}-",0xff0000ff, -1352.33, 499.71, 25.23, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, 0, -1, 50);
//	ArenaRanking[ARENA_ONEDE][1]=CreateDynamic3DTextLabel("Najlepsi na /ONEDE\r\nw tym tygodniu\r\n{ffffff}-",0xff0000ff, 228.36, 184.77, 1004.47, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, 3, -1, 50);
  
	ArenaRanking[ARENA_ONEDE2][0]=CreateDynamic3DTextLabel("Najlepsi na /ONEDE2\r\n\r\n{ffffff}-",0xff0000ff, 265.38, 187.46, 1009.47, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, 3, -1, 50);
	ArenaRanking[ARENA_ONEDE2][1]=CreateDynamic3DTextLabel("Najlepsi na /ONEDE2\r\nw tym tygodniu\r\n{ffffff}-",0xff0000ff, 228.36, 184.77, 1004.47, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, 3, -1, 50);



	Checkpoints[ec_warsztat_tablice]=CreateDynamicCP(1040.22,1778.65,11.75,1.5,0,0,-1,20.0);
	Checkpoints[ec_warsztatlv_myjnia]=CreateDynamicCP(1107.62,1762.57,11.40,4.0,0,0,-1,20.0);
	Checkpoints[ec_warsztatlv_drift]=CreateDynamicCP(1048.0,1754.3,12.13,4.0,0,0,-1,20.0);
	CreateDynamic3DTextLabel("Upgrade drifterski", 0xffffffff, 1048.0,1754.3,13.0, 20, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0, -1, 20);

	Checkpoints[ec_warsztatlv_skup]=CreateDynamicCP(1064,1735,12.43,7.0, 0,0,-1,20.0);

	KillZones[0]=CreateDynamicCube(-5489, 485, -10, -5128, 846, 15, VW_DERBY, 0, -1);
	KillZones[1]=CreateDynamicCube(2634,-1902,0,2835,-1656,35, VW_DERBY,0,-1);
	KillZones[3]=CreateDynamicCube(1798,1494,40,2057,1762,65,VW_DERBY,0,-1);	// derby arena 5
	KillZones[4]=CreateDynamicCube(-1450,1150,1000, -1300,1350, 1035,VW_DERBY,16,-1);	// int 118
//	KillZones[5]=CreateDynamicCube(1525,-1287,251,1657,-1161,273, VW_DERBY, 0, -1); // derby obok wiezowiec
	KillZones[5]=CreateDynamicCube(3197,88,-10,3797,788,20, VW_DERBY, 0, -1); // arena classic
	KillZones[6]=CreateDynamicCube(-3837,609,-10,-3409,878,1, VW_DERBY, 0, -1); // arena 10
	KillZones[7]=CreateDynamicCube( 1181,-1537,10, 1335, -1434,20, VW_DERBY, 0, -1); // arena 12
	KillZones[8]=CreateDynamicCube(-2336,1420,1010, -1553,2389,1150,  VW_DERBY, 0, -1); // arena 13
	KillZones[9]=CreateDynamicCube(-2377,1707,600, -2029,2058,950, VW_DERBY,0,-1);// arena 14
	KillZones[10]=CreateDynamicCube(-1243,866,1400, -864,1222,1620, VW_DERBY,0,-1);//arena 15

	Fotoradary[0]=CreateDynamicSphere(2057.50,1303.54,10.93,20,-1,0,-1);
	Fotoradary[1]=CreateDynamicSphere(1217.44,1804.47,7.62,20,-1,0,-1);
	Fotoradary[2]=CreateDynamicSphere(858.23,-1777.20,13.77,20,-1,0,-1);
	Fotoradary[3]=CreateDynamicSphere(2878.24,-1076.36,10.88,20,-1,0,-1);
	Fotoradary[4]=CreateDynamicSphere(-2316.96,1067.26,55.84,30,-1,0,-1);
	Fotoradary[5]=CreateDynamicSphere(-1013.58,1267.92,33.37,40,-1,0,-1);
	Fotoradary[6]=CreateDynamicSphere(91.33, 674.53,5.62,40,-1,0,-1);
	Fotoradary[7]=CreateDynamicSphere(2232.12,-1576.44,2.57,40,-1,0,-1);
	Fotoradary[8]=CreateDynamicSphere(-1904.03,-626.86,40.26,40,-1,0,-1);

	StrefyNoDM[ea_warsztat]=CreateDynamicCube(1014,1716,9,1119,1842,35,	0,0,-1);
	StrefyNoDM[ea_statek]=CreateDynamicCube(1992.69,	1513.90, 9.20,	2007.89, 1574.56, 45.00,	0,0,-1);
	StrefyNoDM[ea_kosciol]=CreateDynamicSphere(-2751,-294,7,65,	0,0,-1);
	StrefyNoDM[ea_kosciolINT]=CreateDynamicSphere(1964,-360,1092,50,	0,1,-1);
	StrefyNoDM[ea_bankPC]=CreateDynamicCube(2299.0,-24.0,20,	2324.0,0.5,30.0);
//	StrefyNoDM[ea_hacjenda]=CreateDynamicSphere(2831.31,-2863.80,11.73,185,	0,0,-1);
	StrefyNoDM[ea_skatepark]=CreateDynamicCube(1862,-1450.5,8,	1976.6,-1351,47);
	StrefyNoDM[ea_szpital]=CreateDynamicSphere(2308,-1211,-14,50,0,1,-1);
	StrefyNoDM[ea_kart]=CreateDynamicCube(-2095,-280,34,-2010,-90, 60,0,0,-1);
	StrefyNoDM[ea_discoLV]=CreateDynamicSphere(2425,1676,557,75,0,0,-1);
	StrefyNoDM[ea_burdel]=CreateDynamicSphere(950,-51,1001,15);
	StrefyNoDM[ea_klatka]=CreateDynamicSphere(1266.90,-815.0,116.0,45, 18,0,-1);
	StrefyNoDM[ea_WWK]=CreateDynamicSphere(1227.60,-787.3,97.0,45, 17,0,-1);
//	StrefyNoDM[ea_KSW]=CreateDynamicSphere(-1356.16,-486.56,14.17,45, 0,6,-1);	stare ksw by lu - wycofane
	StrefyNoDM[ea_KSW]=CreateDynamicSphere(1274.0,-786.4,126.3,45, 15,0,-1);

	StrefyFullDM[ea_wojsko]=CreateDynamicRectangle(-136.32, 1620.74, 418.40,2164.52,	0,	0,-1);
	StrefyFullDM[ea_minigun]=CreateDynamicSphere(2613.0,2729.0,36.5, 170,	0,0,-1);
	StrefyFullDM[ea_kopalnia]=CreateDynamicSphere(606.0,868.0,-40.0, 170,	0,0,-1);
	StrefyFullDM[ea_arenasolo1]=CreateDynamicCircle(-1102.00,-986.00,40.0,	-1,	9,-1);
	StrefyFullDM[ea_tunele]=CreateDynamicSphere( 2464.79,2610.81,-71.56,	300,	-1,	11,-1);
	DAs[eDA_tuneleKrzyk]=CreateDynamicCube(2372, 2478, -68, 2698, 2781, -67,	-1, 11,-1);

	
	// WIETNAM

//	CreateDynamicMapIcon(-631,-1945,	25.88,46,1,	0,0,-1,1200);
	StrefyFullDM[ea_wietnam]=CreateDynamicSphere( -641.98,-1945,25.88,	235,	0,	0,-1);
//	CreateDynamicPickup( 359, 2, -751.25,	-1848.04,	13.23, 0, 0, -1);	// RPG
//	CreateDynamicPickup( 359, 2, -627.16,	-1993.19,	49.55, 0, 0, -1);	// RPG

	// andromeda
	DAs[eDA_andromeda]=CreateDynamicSphere(315,1050,1935,20,-1,5,-1);
	CreateDynamicObject( 18674,     316.00,1035.00,1935.00,   5.00,5.00,0.00,	-1,9,-1); // chmury_andromeda

//-1889.31,	6796.90
//-2005.47,	6857.40	// w kier molo

	DAs[eDA_jail]=CreateDynamicRectangle(-2005,6797.21,-1889.31,	6865.63,VW_JAIL,0,-1);
//				//	Float:minx, Float:miny, Float:maxx, Float:maxy, worldid = -1, interi
//	DAs[eDA_jail]=CreateDynamicSphere(-1942,6835,17,90,VW_JAIL,0,-1);

	// lodz podwodna
//	PICKUPs[eP_submarine_enter]=CreateDynamicPickup(1559,1,-1884.18,1461.72,5.14,	0,0,-1, 25.0);
//	PICKUPs[eP_submarine_exit]=CreateDynamicPickup(1559,1,521.32,-2433.59,1206.15,	0,1,-1, 25.0);

	PICKUPs[eP_lv_balkony]=CreateDynamicPickup(1318,1,2202.52,992.14,10.82,	0,0,-1,100.0);

//	PICKUPs[eP_kosciolSF1]=CreateDynamicPickup(1318,1,-2717.79,-317.58,7.84,        0,0,-1,100.0);
//	PICKUPs[eP_kosciolSF2]=CreateDynamicPickup(1318,1,-2720.93,-320.73,7.84,        0,0,-1,100.0);
//	PICKUPs[eP_kosciolSF_enter]=CreateDynamicPickup(1318,1,-1989.89,1117.91,54.46,	0,0,-1,100.0);

//	PICKUPs[eP_kosciolSF_exit]=CreateDynamicPickup(1318,1,1964.06,-349.63,1092.94,	0,1,-1,100.0);

//	PICKUPs[eP_jaskinia_enter]=CreateDynamicPickup(1318,1,-2009.07,-1524.25,  85.72,	0,0,-1,100.0);
	

	// arena dm
//	CreateDynamicObject( 13623,     -1102.00,-986.00,141.75,          0.00,0.00,0.00, -1,9,-1,300); // arenadmsiatka
//	CreateDynamicObject( 13607,     -1102.00,-986.00,131.75,          0.00,0.00,0.00, -1,9,-1,300); // dol
//	CreateDynamicObject( 13607,     -1102.00,-986.00,131.75,          180.00,0.00,0.00, -1,9,-1,300); // gora


	// GETTO
/*	bramaO=CreateDynamicObject(2990, 2423.7770996094, -1774.4482421875, 16.460855484009, 0, 0, 270,0,0,-1);
	bramaDA=CreateDynamicRectangle(2419.26, -1779.64,2444, -1769.28,0,0,-1);
// pickupy
pickupHP=CreateDynamicPickup(1240,2, 2485.27,	-1813.21,	16.21, 0, 0, -1);
pickupAR=CreateDynamicPickup(1242,2, 2485.27,	-1815.54,	16.21, 0, 0, -1);
pickupINFO=CreateDynamicPickup(1239,1, 2421.21,	-1767.70,	13.53, 0, 0, -1);*/



	// Wyspa	-> 3500x500
//	CreateDynamicMapIcon(3500.00,  500.00,    0.00,44,1, 0,0,-1,1200);
	StrefyNoDM[ea_wyspa]=CreateDynamicSphere(3500,430,0, 80,  0,0,-1);

	CreateDynamicObject(16192, -4706+8206.0215, -1661.8997+2161, -39.7614, 0.8594, 354.8434, 358.2811,	0,0,-1,600);
	CreateDynamicObject(16061, -4706+8215.8184, -1678.8613+2161, 3.1932, 0.0, 0.0, 0.0,	0,0,-1,300);	// drzewo
	CreateDynamicObject(16061, -4706+8236.5693, -1700.8350+2161, 1.4077, 0.0, 1.7189, 289.5262,	0,0,-1,300);
	CreateDynamicObject(16061, -4706+8198.5410, -1683.7899+2161, 2.5793, 0.0, 0.0, 0.0,	0,0,-1,300);
	CreateDynamicObject(739, -4706+8198.6855, -1718.5924+2161, -37.9492, 0.0, 0.0, 0.0,	0,0,-1,300);
	CreateDynamicObject(619, -4706+8234.5820, -1731.2274+2161, 3.4925, 0.0, 0.0, 0.0,	0,0,-1,200);
	CreateDynamicObject(18259, -4706+8215.6787, -1724.3081+2161, 5.9702, 0.0, 0.0, 84.2248,	0,0,-1,300);
	CreateDynamicObject(1481, -4706+8224.1533, -1730.5194+2161, 6.7092, 0.0, 0.0, 0.0,	0,0,-1,150);
	CreateDynamicObject(1495, -4706+8214.9482, -1729.4093+2161, 6.0508, 0.0, 0.0, 208.7390,	0,0,-1,150);
	CreateDynamicObject(14805, -4706+8221.1162, -1725.1243+2161, 6.9429, 0.0, 0.0, 0.0,	0,0,-1,150);
	CreateDynamicObject(2297, -4706+8221.3672, -1720.1305+2161, 6.0501, 0.0, 0.0, 315.3093,	0,0,-1,150);
	CreateDynamicObject(1667, -4706+8221.9932, -1726.3534+2161, 6.8197, 0.0, 0.0, 0.0,	0,0,-1,150);
	CreateDynamicObject(1828, -4706+8222.0391, -1722.8575+2161, 6.0288, 0.0, 0.0, 84.1208,	0,0,-1,150);
	CreateDynamicObject(1829, -4706+8224.0840, -1721.6119+2161, 6.5206, 0.0, 1.7189, 268.0403,	0,0,-1,150);
	CreateDynamicObject(14527, -4706+8221.2822, -1724.7813+2161, 7.0098, 0.0, 0.0, 0.0,	0,0,-1,150);
	CreateDynamicObject(1815, -4706+8223.3389, -1728.3270+2161, 6.0553, 0.0, 0.0, 0.0,	0,0,-1,150);
	CreateDynamicObject(1668, -4706+8224.0420, -1727.6943+2161, 6.7184, 0.0, 0.0, 0.0,	0,0,-1,150);
	CreateDynamicObject(1669, -4706+8224.0439, -1727.8624+2161, 6.7184, 0.0, 0.0, 0.0,	0,0,-1,150);
	CreateDynamicObject(1667, -4706+8223.5752, -1727.6796+2161, 6.6403, 0.0, 0.0, 0.0,	0,0,-1,150);
	CreateDynamicObject(1664, -4706+8224.2734, -1727.8196+2161, 6.6934, 0.0, 0.0, 0.0,	0,0,-1,150);
	CreateDynamicObject(1665, -4706+8223.7666, -1727.9882+2161, 6.5872, 0.0, 0.0, 0.0,	0,0,-1,150);
	CreateDynamicObject(1512, -4706+8224.1240, -1728.0743+2161, 6.7242, 0.0, 0.0, 0.0,	0,0,-1,150);
	CreateDynamicObject(2344, -4706+8218.9346, -1725.0416+2161, 32.8490, 0.0, 0.0, 0.0,	0,0,-1,150);
	CreateDynamicObject(14446, -4706+8210.6387, -1724.0170+2161, 6.6564, 0.0, 0.0, 86.6991,	0,0,-1,150);
	CreateDynamicObject(1742, -4706+8208.6084, -1728.1372+2161, 6.0516, 0.0, 0.0, 83.2614);
	CreateDynamicObject(2190, -4706+8210.1758, -1719.8883+2161, 6.8767, 0.0, 0.0, 0.0);
	CreateDynamicObject(2568, -4706+8209.7500, -1720.0713+2161, 6.0483, 0.0, 0.0, 0.0);
	CreateDynamicObject(1671, -4706+8210.4912, -1720.9667+2161, 6.4644, 0.0, 0.0, 210.4580);
	CreateDynamicObject(643, -4706+8222.7764, -1734.0599+2161, 6.5016, 0.0, 0.0, 0.0);
	CreateDynamicObject(1280, -4706+8222.3604, -1730.1859+2161, 6.4575, 0.0, 0.0, 88.4180);
	CreateDynamicObject(1243, -4706+8185.5571, -1824.7396+2161, -1.9524, 0.0, 0.0, 0.0);
	CreateDynamicObject(1243, -4706+8148.4775, -1789.2531+2161, -2.5265, 0.0, 0.0, 0.0);
	CreateDynamicObject(11496, -4706+8201.7275, -1768.8059+2161, 1.7526, 0.8594, 0.0, 226.7873);

	CreateDynamicObject(1608, -4706+8190.7637, -1793.9025+2161, -1.3131, 0.0, 0.0, 0.0,	0,0,-1,300);		// rekiny
	CreateDynamicObject(1608, -4706+8188.0542, -1778.4365+2161, -1.3246, 4.2972, 0.0, 249.1327,	0,0,-1,300);
	CreateDynamicObject(1608, -4706+8173.2729, -1794.7434+2161, -1.2060, 0.0, 0.0, 0.0,	0,0,-1,300);

	CreateDynamicObject(16097, -4706+8270.3359, -1702.6177+2161, -10.9683, 0.0, 0.0, 0.0,0,0,-1,600);	// lad
	CreateDynamicObject(1458, -4706+8219.5928, -1740.3041+2161, 5.8318, 0.0, 0.0, 0.0);
	CreateDynamicObject(1457, -4706+8232.0752, -1728.2046+2161, 7.2815, 0.0, 0.0, 0.0);
	CreateDynamicObject(3461, -4706+8218.5352, -1759.6958+2161, 3.6359, 0.0, 0.0, 0.0);
	CreateDynamicObject(2035, -4706+8222.0869, -1725.6338+2161, 6.7555, 0.0, 0.0, 0.0);
	CreateDynamicObject(854, -4706+8218.7852, -1759.3384+2161, 5.2185, 0.0, 0.0, 0.0);
	CreateDynamicObject(1280, -4706+8220.2988, -1759.7511+2161, 5.6858, 0.0, 0.0, 0.0);


	// WULKAN 
	KillZones[2]=CreateDynamicSphere(-1093.04,	-993.44,	138.53,	15.0,	0,0,-1);	// lawa

//	CreateDynamicMapIcon(-1093.04,	-993.44,	138.53,43,1,	0,0,-1,1200);
    CreateDynamicObject( 18752, -1090.00,-990.00,116.00,      0.00,0.00,250.00, 0,0,-1,2100); // wulkan
	CreateDynamicObject( 3673,  -1099.00,-992.00,111.00,      0.00,0.00,0.00,	0,0,-1,300); // dym



	// Labirynt
	CreateDynamicObject(974, 1542.3521, -1575.0107, 25.3246, 0.0, 0.0, 180.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1532.4032, -1571.7383, 25.3246, 0.0, 0.0, 270.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1538.4231, -1571.6571, 25.3246, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1541.7977, -1568.3497, 25.3246, 0.0, 0.0, 270.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1535.7205, -1575.0337, 25.3246, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1545.6226, -1568.3002, 25.3246, 0.0, 0.0, 270.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1545.6729, -1561.7369, 25.3246, 0.0, 0.0, 270.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1538.4954, -1561.7454, 25.3246, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1545.6719, -1555.0909, 25.3246, 0.0, 0.0, 270.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1535.7048, -1568.4052, 25.3246, 0.0, 0.0, 180.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1539.0474, -1565.0917, 25.3246, 0.0, 0.0, 270.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1531.9226, -1561.7329, 25.3246, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1542.3376, -1558.4017, 25.3246, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1538.9832, -1555.1039, 25.3246, 0.0, 0.0, 90.0, VW_LABIRYNT,0, -1, 300);
	CreateDynamicObject(974, 1535.1951, -1558.4248, 25.3246, 0.0, 0.0, 90.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1535.6447, -1551.8043, 25.3246, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1532.3263, -1555.0940, 25.3246, 0.0, 0.0, 90.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1542.3726, -1551.8464, 25.3246, 0.0, 0.0, 90.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1545.7322, -1548.4855, 25.3246, 0.0, 0.0, 180.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1538.9792, -1548.4377, 25.3246, 0.0, 0.0, 90.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1538.9825, -1541.9777, 25.3246, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1538.9585, -1535.0912, 25.3246, 0.0, 0.0, 90.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1538.9677, -1528.4279, 25.3246, 0.0, 0.0, 90.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1538.9674, -1528.4657, 25.3246, 0.0, 0.0, 90.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1542.3286, -1545.1782, 25.3246, 0.0, 0.0, 180.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1545.6855, -1541.8887, 25.3246, 0.0, 0.0, 270.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1542.3406, -1538.6388, 25.3246, 0.0, 0.0, 270.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1532.3799, -1541.9833, 25.3246, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1542.3595, -1532.0574, 25.3246, 0.0, 0.0, 270.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1542.2968, -1525.0925, 25.3246, 0.0, 0.0, 180.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1545.6201, -1528.4019, 25.3246, 0.0, 0.0, 90.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1549.0109, -1535.0333, 25.3246, 0.0, 0.0, 180.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1535.6659, -1538.6592, 25.3246, 0.0, 0.0, 270.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1535.5813, -1531.7888, 25.3246, 0.0, 0.0, 0.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1528.9529, -1531.8303, 25.3246, 0.0, 0.0, 0.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1545.6646, -1521.8868, 25.3246, 0.0, 0.0, 0.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1542.2842, -1521.8253, 25.3246, 0.0, 0.0, 0.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1535.6205, -1521.8229, 25.3246, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1535.6990, -1525.1493, 25.3246, 0.0, 0.0, 90.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1532.3751, -1528.4894, 25.3246, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1525.6718, -1531.8445, 25.3246, 0.0, 0.0, 90.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1529.1891, -1525.1876, 25.3246, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1525.8741, -1525.1984, 25.3258, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1522.5953, -1528.4862, 25.3258, 0.0, 0.0, 90.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1522.6183, -1535.0905, 25.3246, 0.0, 0.0, 90.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1529.0139, -1535.1587, 25.3246, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1529.0835, -1538.5983, 25.3246, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1525.7736, -1541.8951, 25.3246, 0.0, 0.0, 90.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1525.7860, -1545.2552, 25.3246, 0.0, 0.0, 90.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1522.4478, -1548.5646, 25.3246, 0.0, 0.0, 180.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1519.1011, -1545.2091, 25.3246, 0.0, 0.0, 270.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1515.8196, -1545.2555, 25.3246, 0.0, 0.0, 90.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1515.8230, -1551.8829, 25.3246, 0.0, 0.0, 90.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1515.8281, -1538.6221, 25.3246, 0.0, 0.0, 90.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1519.1089, -1538.5844, 25.3246, 0.0, 0.0, 270.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1515.8236, -1535.3606, 25.3246, 0.0, 0.0, 90.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1519.2112, -1532.0138, 25.3246, 0.0, 0.0, 180.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1529.1095, -1545.2102, 25.3246, 0.0, 0.0, 180.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1532.2928, -1551.8611, 25.3246, 0.0, 0.0, 90.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1532.3779, -1545.2159, 25.3246, 0.0, 0.0, 180.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1528.5198, -1551.9332, 25.3246, 0.0, 0.0, 90.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1522.6209, -1541.7650, 25.3246, 0.0, 0.0, 90.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1528.9862, -1521.8687, 25.3258, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1522.3267, -1521.8579, 25.3258, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1519.0228, -1525.2212, 25.3258, 0.0, 0.0, 90.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1515.7091, -1528.5531, 25.3258, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1515.6733, -1532.0222, 25.3258, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1512.3469, -1535.3193, 25.3258, 0.0, 0.0, 90.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1512.6003, -1541.9408, 25.3246, 0.0, 0.0, 180.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1509.0295, -1538.6504, 25.3246, 0.0, 0.0, 180.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1505.6814, -1542.0002, 25.3246, 0.0, 0.0, 270.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1509.0219, -1545.3236, 25.3246, 0.0, 0.0, 0.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1512.4440, -1548.6002, 25.3246, 0.0, 0.0, 0.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1509.0890, -1548.6281, 25.3246, 0.0, 0.0, 0.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1502.5472, -1548.6077, 25.3246, 0.0, 0.0, 90.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1467.2406, -1549.9003, 25.3258, 0.0, 0.0, 90.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1467.2423, -1543.3610, 25.3258, 0.0, 0.0, 90.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1467.2168, -1536.7007, 25.3258, 0.0, 0.0, 90.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1467.2292, -1530.1118, 25.3258, 0.0, 0.0, 90.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1467.2412, -1523.4381, 25.3258, 0.0, 0.0, 90.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1467.2636, -1518.3553, 25.3258, 0.0, 0.0, 90.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1502.4938, -1551.9126, 25.3246, 0.0, 0.0, 0.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1505.7478, -1555.2332, 25.3246, 0.0, 0.0, 180.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1499.1150, -1555.2428, 25.3246, 0.0, 0.0, 180.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1495.8224, -1551.9927, 25.3246, 0.0, 0.0, 270.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1495.8093, -1548.6520, 25.3246, 0.0, 0.0, 0.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1499.2008, -1545.2861, 25.3246, 0.0, 0.0, 0.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1492.4713, -1545.3947, 25.3258, 0.0, 0.0, 270.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1502.5918, -1545.1930, 25.3258, 0.0, 0.0, 270.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1502.5950, -1535.4219, 25.3258, 0.0, 0.0, 270.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1499.2122, -1538.6674, 25.3258, 0.0, 0.0, 90.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1495.8981, -1541.9883, 25.3258, 0.0, 0.0, 90.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1492.6071, -1538.6345, 25.3258, 0.0, 0.0, 180.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1499.2139, -1535.3403, 25.3258, 0.0, 0.0, 180.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1492.5970, -1535.3256, 25.3258, 0.0, 0.0, 270.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1492.5907, -1532.0217, 25.3258, 0.0, 0.0, 0.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1489.1124, -1548.7402, 25.3258, 0.0, 0.0, 0.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1482.8412, -1548.7366, 25.3258, 0.0, 0.0, 0.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1476.2571, -1548.7459, 25.3258, 0.0, 0.0, 0.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1469.6105, -1548.7692, 25.3258, 0.0, 0.0, 0.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1489.3923, -1541.9767, 25.3258, 0.0, 0.0, 90.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1489.4459, -1538.6501, 25.3258, 0.0, 0.0, 180.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1512.3174, -1528.6016, 25.3258, 0.0, 0.0, 0.0003, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1508.9943, -1531.9110, 25.3258, 0.0, 0.0, 90.0003, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1502.5959, -1535.3989, 25.3258, 0.0, 0.0, 0.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1486.1500, -1541.9508, 25.3258, 0.0, 0.0, 90.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1482.8809, -1545.3425, 25.3258, 0.0, 0.0, 90.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1482.8223, -1542.0836, 25.3258, 0.0, 0.0, 0.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1505.6859, -1528.6133, 25.3258, 0.0, 0.0, 0.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1502.6919, -1532.1345, 25.3258, 0.0, 0.0, 0.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1499.4734, -1528.8575, 25.3258, 0.0, 0.0, 270.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1505.7883, -1525.3358, 25.3258, 0.0, 0.0, 270.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1505.8625, -1525.3772, 25.3258, 0.0, 0.0, 180.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1502.4342, -1522.0242, 25.3258, 0.0, 0.0, 180.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1495.8796, -1522.0099, 25.3258, 0.0, 0.0, 180.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1495.8394, -1525.3601, 25.3258, 0.0, 0.0, 270.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1492.5830, -1521.9690, 25.3258, 0.0, 0.0, 270.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1492.5447, -1528.6547, 25.3258, 0.0, 0.0, 180.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1489.2456, -1525.3253, 25.3258, 0.0, 0.0, 180.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1485.8824, -1528.6410, 25.3258, 0.0, 0.0, 270.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1485.8365, -1531.9696, 25.3258, 0.0, 0.0, 270.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1485.7964, -1535.3026, 25.3258, 0.0, 0.0, 0.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1482.4838, -1535.3125, 25.3258, 0.0, 0.0, 90.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1479.1299, -1538.6305, 25.3258, 0.0, 0.0, 180.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1475.8295, -1535.3275, 25.3258, 0.0, 0.0, 90.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1475.8500, -1535.2919, 25.3258, 0.0, 0.0, 0.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1495.8928, -1518.6313, 25.3258, 0.0, 0.0, 0.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1502.4065, -1515.5293, 25.3258, 0.0, 0.0, 270.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1489.2261, -1518.6414, 25.3258, 0.0, 0.0, 90.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1485.9114, -1522.0363, 25.3258, 0.0, 0.0, 90.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1485.8369, -1515.3217, 25.3258, 0.0, 0.0, 0.0001, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1509.1982, -1522.0255, 25.3258, 0.0, 0.0, 270.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1519.0155, -1518.4830, 25.3258, 0.0, 0.0, 180.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1515.6964, -1521.8146, 25.3258, 0.0, 0.0, 90.0004, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1509.1403, -1518.6993, 25.3258, 0.0, 0.0, 180.0002, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1525.5430, -1515.0044, 25.3258, 0.0, 0.0, 90.0004, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1512.4391, -1521.9489, 25.3258, 0.0, 0.0, 90.0004, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1538.8873, -1515.0173, 25.3246, 0.0, 0.0, 90.0004, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1535.7004, -1521.8414, 25.3246, 0.0, 0.0, 90.0004, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1532.3534, -1518.5166, 25.3258, 0.0, 0.0, 0.0006, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1482.5197, -1518.6312, 25.3258, 0.0, 0.0, 90.0004, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1482.5692, -1525.3597, 25.3258, 0.0, 0.0, 0.0006, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1479.2651, -1528.6753, 25.3258, 0.0, 0.0, 180.0009, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1475.9590, -1525.3696, 25.3258, 0.0, 0.0, 180.0009, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1472.6544, -1525.4003, 25.3258, 0.0, 0.0, 90.0007, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1469.5690, -1532.0657, 25.3258, 0.0, 0.0, 0.0006, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1472.6122, -1522.0479, 25.3258, 0.0, 0.0, 270.0003, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1479.2556, -1521.8975, 25.3258, 0.0, 0.0, 180.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1473.1891, -1518.7611, 25.3258, 0.0, 0.0, 180.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1467.0192, -1522.0256, 25.3258, 0.0, 0.0, 180.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1472.6602, -1525.3486, 25.3258, 0.0, 0.0, 180.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1472.5425, -1538.6675, 25.3258, 0.0, 0.0, 89.9998, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1472.9302, -1541.9426, 25.3258, 0.0, 0.0, 180.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1475.8702, -1545.4056, 25.3258, 0.0, 0.0, 180.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1472.6029, -1542.0270, 25.3892, 0.0, 0.0, 89.9998, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1472.5935, -1535.3027, 25.3258, 0.0, 0.0, 180.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(974, 1482.5112, -1532.0540, 25.3258, 0.0, 0.0, 89.9998, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(625, 1536.2410, -1565.0746, 23.6026, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);	// drzewa
	CreateDynamicObject(625, 1543.6426, -1516.8820, 23.6026, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(625, 1546.3169, -1516.8896, 23.6026, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);
	CreateDynamicObject(4016, 1497.8185, -1543.8632, 23.0909, 0.0, 0.0, 0.0, VW_LABIRYNT, 0, -1, 600);	// zamkniecie labiryntu od gory

	// Strzelnica (AmmuNation)
	CreateDynamicObject(7191, 280.4880, -115.7953, 1005.0371, 0.0, 180.0, 240.0);
	CreateDynamicObject(7191, 280.3250, -150.6972, 1005.0294, 0.0, 0.0, 300.0);
	CreateDynamicObject(9339, 299.6453, -131.8039, 1005.8615, 0.0, 0.0, 0.0);

	// GETTO
	// mapicon

	// lodz podwodna
	CreateDynamicObject(3117, 538.02746582031, -2431, 1203.025390625, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3117, 534.50219726563, -2431, 1203.025390625, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3117, 530.94000244141, -2431, 1203.025390625, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3117, 527.388671875, -2431, 1203.025390625, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(9820, 533.20812988281, -2434.4289550781, 1203.9852294922, 0, 0, 270,0,1,-1,300);
	CreateDynamicObject(3117, 538.01861572266, -2433.1201171875, 1203.025390625, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3117, 534.501953125, -2433.1201171875, 1203.025390625, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3117, 530.939453125, -2433.1201171875, 1203.025390625, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3117, 527.388671875, -2433.1201171875, 1203.025390625, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3117, 523.837890625, -2433.1201171875, 1203.025390625, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3117, 520.2861328125, -2433.1201171875, 1203.025390625, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 535.32977294922, -2429.962890625, 1207.4162597656, 90, 0, 180,0,1,-1,300);
	CreateDynamicObject(3095, 533.54315185547, -2432.1628417969, 1207.4665527344, 90, 0, 180,0,1,-1,300);
	CreateDynamicObject(3095, 539.5791015625, -2428.357421875, 1205.7175292969, 90, 0, 90,0,1,-1,300);
	CreateDynamicObject(3095, 540.30078125, -2434.3935546875, 1206.4172363281, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 543.3359375, -2435.3017578125, 1205.6912841797, 90, 0, 90,0,1,-1,300);
	CreateDynamicObject(3095, 544.634765625, -2432.837890625, 1205.6922607422, 90, 0, 179.99450683594,0,1,-1,300);
	CreateDynamicObject(3095, 531.34338378906, -2434.4631347656, 1206.4172363281, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 544.28686523438, -2435.7065429688, 1203.1677246094, 180, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 539.814453125, -2438.6484375, 1205.7423095703, 90, 0, 270,0,1,-1,300);
	CreateDynamicObject(3095, 544.1376953125, -2438.8251953125, 1204.4417724609, 90, 0, 0,0,1,-1,300);
	CreateDynamicObject(2944, 539.60577392578, -2434.2741699219, 1204.5452880859, 0, 0, 261.20385742188,0,1,-1,300);
	CreateDynamicObject(3095, 534.77545166016, -2434.1518554688, 1207.4167480469, 90, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 533.5185546875, -2431.5751953125, 1207.4672851563, 90, 0, 0,0,1,-1,300);
	CreateDynamicObject(9819, 542.25457763672, -2437.5654296875, 1203.9002685547, 0, 0, 270.27026367188,0,1,-1,300);
	CreateDynamicObject(1663, 541.26226806641, -2436.9877929688, 1203.6245117188, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 526.58880615234, -2428.4353027344, 1202.9417724609, 90, 0, 270,0,1,-1,300);
	CreateDynamicObject(3095, 522.07385253906, -2432.9611816406, 1202.6665039063, 90, 0, 180.27014160156,0,1,-1,300);
	CreateDynamicObject(3095, 522.36553955078, -2434.474609375, 1206.4172363281, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(16782, 539.89959716797, -2435.6579589844, 1204.6827392578, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 524.52368164063, -2434.1721191406, 1202.7666015625, 90, 0, 0,0,1,-1,300);
	CreateDynamicObject(2944, 530.34967041016, -2434.3896484375, 1204.5452880859, 0, 0, 351.41479492188,0,1,-1,300);
	CreateDynamicObject(3095, 530.46459960938, -2438.6704101563, 1203.1427001953, 179.99450683594, 0, 0,0,1,-1,300);
	CreateDynamicObject(10227, 530.46575927734, -2436.7365722656, 1204.9790039063, 0, 0, 90,0,1,-1,300);
	CreateDynamicObject(3095, 526.03820800781, -2438.9262695313, 1203.2921142578, 90, 0, 270,0,1,-1,300);
	CreateDynamicObject(3095, 534.91082763672, -2438.8146972656, 1207.3671875, 90, 0, 90,0,1,-1,300);
	CreateDynamicObject(3095, 530.50500488281, -2438.7270507813, 1203.2661132813, 90, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 530.658203125, -2439.1062011719, 1205.8416748047, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 524.54742431641, -2434.7482910156, 1203.3907470703, 90, 0, 179.99450683594,0,1,-1,300);
	CreateDynamicObject(3095, 534.77416992188, -2434.7270507813, 1207.4172363281, 90, 0, 179.99450683594,0,1,-1,300);
	CreateDynamicObject(2690, 529.82861328125, -2438.5744628906, 1203.4986572266, 0, 0, 192.54040527344,0,1,-1,300);
	CreateDynamicObject(918, 533.12744140625, -2433.9267578125, 1203.5219726563, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(918, 532.19744873047, -2433.884765625, 1203.5219726563, 0, 0, 300.18017578125,0,1,-1,300);
	CreateDynamicObject(2312, 530.33752441406, -2432.1123046875, 1205.3646240234, 6, 0, 0,0,1,-1,300);
	CreateDynamicObject(2886, 529.6884765625, -2432.1235351563, 1204.5522460938, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(2886, 532.06359863281, -2432.1220703125, 1204.5522460938, 0, 0, 1.9849853515625,0,1,-1,300);
	CreateDynamicObject(2312, 532.68737792969, -2432.107421875, 1205.3646240234, 5.99853515625, 0, 0,0,1,-1,300);
	CreateDynamicObject(2063, 535.6181640625, -2432.4604492188, 1203.9842529297, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(1242, 534.58074951172, -2432.4663085938, 1204.0692138672, 0, 0, 338.1201171875,0,1,-1,300);
	CreateDynamicObject(1242, 534.97930908203, -2432.4438476563, 1204.0692138672, 0, 0, 23.905090332031,0,1,-1,300);
	CreateDynamicObject(1242, 535.40185546875, -2432.4643554688, 1204.0692138672, 0, 0, 351.96069335938,0,1,-1,300);
	CreateDynamicObject(2043, 534.83953857422, -2432.5034179688, 1203.5206298828, 0, 0, 270.27026367188,0,1,-1,300);
	CreateDynamicObject(2043, 535.92388916016, -2432.52734375, 1203.5206298828, 0, 0, 70.869232177734,0,1,-1,300);
	CreateDynamicObject(2969, 536.19653320313, -2432.4033203125, 1203.9854736328, 0, 0, 357.75,0,1,-1,300);
	CreateDynamicObject(2690, 534.41552734375, -2431.447265625, 1203.4986572266, 0, 0, 190.50842285156,0,1,-1,300);
	CreateDynamicObject(2690, 534.86975097656, -2431.4250488281, 1203.4986572266, 0, 0, 250.32806396484,0,1,-1,300);
	CreateDynamicObject(2359, 535.0068359375, -2432.4235839844, 1204.9534912109, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(2359, 536.29071044922, -2432.3913574219, 1204.9534912109, 0, 0, 356.03002929688,0,1,-1,300);
	CreateDynamicObject(2358, 535.44219970703, -2432.48828125, 1204.4558105469, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(1271, 531.34405517578, -2431.2106933594, 1203.5003662109, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(1271, 532.06781005859, -2431.1884765625, 1203.5003662109, 0, 0, 358.78002929688,0,1,-1,300);
	CreateDynamicObject(1271, 531.79602050781, -2431.1955566406, 1204.1760253906, 0, 0, 359.99841308594,0,1,-1,300);
	CreateDynamicObject(3879, 522.79968261719, -2430.4990234375, 1216.5125732422, 0, 180, 180,0,1,-1,300);
	CreateDynamicObject(3095, 520.33489990234, -2430.3913574219, 1202.5418701172, 90, 0, 270,0,1,-1,300);
	CreateDynamicObject(2634, 521.62847900391, -2433.548828125, 1206.4168701172, 270, 0, 270.02026367188,0,1,-1,300);
	CreateDynamicObject(2690, 543.03540039063, -2433.1096191406, 1203.4986572266, 0, 0, 312.13012695313,0,1,-1,300);
	CreateDynamicObject(3111, 524.69171142578, -2434.1579589844, 1204.6546630859, 270, 90, 270,0,1,-1,300);
	CreateDynamicObject(3386, 528.38543701172, -2434.1892089844, 1203.1496582031, 0, 0, 270,0,1,-1,300);
	CreateDynamicObject(12985, 530.96057128906, -2429.4606933594, 1200.5476074219, 0, 0, 270.25,0,1,-1,300);
	CreateDynamicObject(3095, 530.28283691406, -2425.46875, 1207.6424560547, 90, 0, 90,0,1,-1,300);
	CreateDynamicObject(3095, 530.02172851563, -2428.9338378906, 1202.0416259766, 90, 0, 180,0,1,-1,300);
	CreateDynamicObject(3095, 527.61779785156, -2429.9677734375, 1198.6169433594, 90, 0, 359.75,0,1,-1,300);
	CreateDynamicObject(3095, 529.77532958984, -2425.46875, 1206.4172363281, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 535.2802734375, -2425.4484863281, 1202.9177246094, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 533.5009765625, -2425.4375, 1202.0418701172, 90, 0, 90,0,1,-1,300);
	CreateDynamicObject(3095, 533.0244140625, -2434.4638671875, 1198.9937744141, 179.99450683594, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 531.04742431641, -2425.9897460938, 1199.6657714844, 48, 180, 90.25,0,1,-1,300);
	CreateDynamicObject(1800, 528.29656982422, -2431.0593261719, 1198.9904785156, 0, 0, 270,0,1,-1,300);
	CreateDynamicObject(1800, 528.2958984375, -2431.05859375, 1199.8658447266, 0, 0, 270,0,1,-1,300);
	CreateDynamicObject(1800, 528.04467773438, -2434.041015625, 1198.9904785156, 0, 0, 270,0,1,-1,300);
	CreateDynamicObject(1800, 528.322265625, -2437.9799804688, 1199.8656005859, 0, 0, 270,0,1,-1,300);
	CreateDynamicObject(1800, 528.33276367188, -2437.978515625, 1198.9904785156, 0, 0, 270,0,1,-1,300);
	CreateDynamicObject(1800, 531.681640625, -2437.94921875, 1198.9904785156, 0, 0, 270,0,1,-1,300);
	CreateDynamicObject(1800, 531.68212890625, -2437.94921875, 1199.8653564453, 0, 0, 270,0,1,-1,300);
	CreateDynamicObject(3095, 524.11560058594, -2434.4887695313, 1198.9937744141, 179.99450683594, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 529.396484375, -2440.3095703125, 1198.1677246094, 90, 0, 270,0,1,-1,300);
	CreateDynamicObject(3095, 528.82373046875, -2440.3239746094, 1198.1677246094, 90, 0, 90.290161132813,0,1,-1,300);
	CreateDynamicObject(2514, 520.80499267578, -2433.5725097656, 1198.9903564453, 0, 0, 89.730010986328,0,1,-1,300);
	CreateDynamicObject(2739, 521.42553710938, -2433.4611816406, 1198.9903564453, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(2739, 522.27471923828, -2433.4450683594, 1198.9903564453, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(2944, 526.4873046875, -2432.8095703125, 1200.3696289063, 0, 0, 99.55810546875,0,1,-1,300);
	CreateDynamicObject(3095, 533.1416015625, -2434.5673828125, 1201.7415771484, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 524.17797851563, -2434.5625, 1201.7415771484, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3117, 533.58203125, -2430.0791015625, 1202.0744628906, 89.324340820313, 0, 359.74731445313,0,1,-1,300);
	CreateDynamicObject(3095, 527.64892578125, -2430.5646972656, 1197.2668457031, 90, 0, 179.99450683594,0,1,-1,300);
	CreateDynamicObject(2527, 526.66650390625, -2437.2890625, 1198.9655761719, 0, 0, 180,0,1,-1,300);
	CreateDynamicObject(2527, 528.35589599609, -2437.2658691406, 1198.9655761719, 0, 0, 179.99450683594,0,1,-1,300);
	CreateDynamicObject(11631, 534.91815185547, -2430.9252929688, 1200.2359619141, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 538.060546875, -2430.2431640625, 1197.3377685547, 90, 0, 179.99450683594,0,1,-1,300);
	CreateDynamicObject(1663, 535.23059082031, -2431.5083007813, 1199.4191894531, 0, 0, 160.60052490234,0,1,-1,300);
	CreateDynamicObject(2007, 526.5126953125, -2435.2265625, 1198.9904785156, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(2416, 527.42669677734, -2431.1213378906, 1198.8720703125, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(1800, 528.0439453125, -2434.041015625, 1199.6906738281, 0, 0, 270,0,1,-1,300);
	CreateDynamicObject(2690, 532.3095703125, -2438.2602539063, 1199.3232421875, 0, 0, 170.74377441406,0,1,-1,300);
	CreateDynamicObject(2690, 536.43225097656, -2430.5341796875, 1199.3232421875, 0, 0, 335.35363769531,0,1,-1,300);
	CreateDynamicObject(3095, 539.2568359375, -2438.7294921875, 1198.2911376953, 90, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 537.32739257813, -2436.1455078125, 1198.2918701172, 90, 0, 90,0,1,-1,300);
	CreateDynamicObject(2944, 537.42340087891, -2431.7294921875, 1200.3696289063, 0, 0, 324.76245117188,0,1,-1,300);
	CreateDynamicObject(1800, 536.50280761719, -2437.6374511719, 1198.9654541016, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(1800, 536.501953125, -2437.63671875, 1199.8663330078, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3386, 537.34405517578, -2438.6044921875, 1198.9655761719, 0, 0, 319.61853027344,0,1,-1,300);
	CreateDynamicObject(2007, 529.81372070313, -2436.6037597656, 1198.9654541016, 0, 0, 89.684936523438,0,1,-1,300);
	CreateDynamicObject(2007, 536.85040283203, -2433.5270996094, 1198.9654541016, 0, 0, 270.22143554688,0,1,-1,300);
	CreateDynamicObject(2007, 525.03350830078, -2433.8515625, 1198.9654541016, 0, 0, 180.48974609375,0,1,-1,300);
	CreateDynamicObject(3095, 537.94476318359, -2434.2509765625, 1197.2440185547, 179.99450683594, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 546.93872070313, -2434.2585449219, 1197.2440185547, 179.99450683594, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 547.01922607422, -2430.2373046875, 1197.3377685547, 90, 0, 179.99450683594,0,1,-1,300);
	CreateDynamicObject(3095, 548.23669433594, -2438.7329101563, 1198.2906494141, 90, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 542.13323974609, -2434.6682128906, 1201.1927490234, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 550.76525878906, -2434.6833496094, 1198.3181152344, 90, 0, 90,0,1,-1,300);
	CreateDynamicObject(3095, 551.06671142578, -2434.6713867188, 1201.1927490234, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3095, 538.14105224609, -2436.1105957031, 1197.1429443359, 90, 0, 270,0,1,-1,300);
	CreateDynamicObject(3787, 541.83905029297, -2432.0568847656, 1197.7816162109, 0, 0, 89.730010986328,0,1,-1,300);
	CreateDynamicObject(8572, 538.388671875, -2433.0817871094, 1197.7409667969, 0, 0, 89.730041503906,0,1,-1,300);
	CreateDynamicObject(964, 539.80517578125, -2434.8369140625, 1197.2407226563, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(964, 540.00012207031, -2433.5756835938, 1197.2407226563, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(964, 539.96453857422, -2434.0815429688, 1198.1666259766, 0, 0, 352.06005859375,0,1,-1,300);
	CreateDynamicObject(964, 541.27093505859, -2434.435546875, 1197.2407226563, 0, 0, 344.1201171875,0,1,-1,300);
	CreateDynamicObject(964, 540.37884521484, -2432.2858886719, 1197.2407226563, 0, 0, 358.01416015625,0,1,-1,300);
	CreateDynamicObject(964, 540.44445800781, -2430.9887695313, 1197.2407226563, 0, 0, 359.01147460938,0,1,-1,300);
	CreateDynamicObject(964, 540.23645019531, -2431.6215820313, 1198.1666259766, 0, 0, 352.3212890625,0,1,-1,300);
	CreateDynamicObject(3117, 537.99005126953, -2431.9519042969, 1197.8988037109, 89.324340820313, 0, 89.977294921875,0,1,-1,300);
	CreateDynamicObject(3788, 539.72692871094, -2438.126953125, 1197.7603759766, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3788, 545.88702392578, -2432.2800292969, 1197.7600097656, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3788, 545.87335205078, -2434.2255859375, 1197.7600097656, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3788, 545.85906982422, -2436.2758789063, 1197.7600097656, 0, 0, 0,0,1,-1,300);
	CreateDynamicObject(3786, 546.21185302734, -2432.2673339844, 1197.9476318359, 0, 0, 180,0,1,-1,300);
	CreateDynamicObject(3786, 546.21862792969, -2434.2109375, 1197.9476318359, 0, 0, 180,0,1,-1,300);
	CreateDynamicObject(3786, 546.20245361328, -2436.2770996094, 1197.9476318359, 0, 0, 180,0,1,-1,300);
	CreateDynamicObject(14450, 549.74066162109, -2434.4230957031, 1200.4025878906, 0, 0, 270,0,1,-1,300);
	CreateDynamicObject(927, 526.06927490234, -2436.9543457031, 1199.9611816406, 0, 0, 89.730010986328,0,1,-1,300);
	CreateDynamicObject(927, 523.02197265625, -2434.1232910156, 1199.9611816406, 0, 0, 179.70520019531,0,1,-1,300);
	CreateDynamicObject(934, 550.01379394531, -2433.0434570313, 1198.5690917969, 0, 0, 0,0,1,-1,300);

	// kosciol
	CreateDynamicObject(9931,1947.54003906,-368.51269531,1108.01086426,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(church_sfe) (1)
	CreateDynamicObject(3976,1970.65722656,-341.23883057,1100.22949219,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(policest02_lan) (1)
	CreateDynamicObject(9931,1980.55761719,-368.51562500,1108.01086426,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(church_sfe) (2)
	CreateDynamicObject(3976,1962.70117188,-400.46679688,1109.52941895,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(policest02_lan) (2)
	CreateDynamicObject(11472,1964.78710938,-372.02050781,1089.22351074,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(des_swtstairs1) (1)
	CreateDynamicObject(11472,1963.28967285,-372.02139282,1089.22351074,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(des_swtstairs1) (2)
	CreateDynamicObject(11472,1964.04479980,-372.98049927,1089.46386719,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(des_swtstairs1) (3)
	CreateDynamicObject(2896,1959.29248047,-370.91723633,1093.34313965,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(casket_law) (1)
	CreateDynamicObject(11472,1962.66918945,-373.82989502,1087.66955566,90.00000000,180.00000000,270.00000000,0,1,-1,250); //object(des_swtstairs1) (4)
	CreateDynamicObject(11472,1965.41259766,-373.81991577,1087.66992188,90.00000000,180.00000000,90.00000000,0,1,-1,250); //object(des_swtstairs1) (5)
	CreateDynamicObject(970,1970.61437988,-368.20404053,1093.28039551,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (1)
	CreateDynamicObject(970,1968.51074219,-368.20703125,1093.28039551,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (2)
	CreateDynamicObject(970,1957.52185059,-368.25833130,1093.28039551,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (3)
	CreateDynamicObject(970,1959.61914062,-368.25585938,1093.28039551,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
	CreateDynamicObject(6959,1963.97839355,-368.12149048,1072.75439453,90.00000000,0.00000000,0.25000000,0,1,-1,250); //object(vegasnbball1) (2)
	CreateDynamicObject(2960,1964.05883789,-348.98986816,1101.36645508,0.00000000,90.00000000,89.99450684,0,1,-1,250); //object(kmb_beam) (1)
	CreateDynamicObject(2960,1963.98291016,-349.00000000,1101.70666504,90.00000000,0.00000000,179.99450684,0,1,-1,250); //object(kmb_beam) (2)
	CreateDynamicObject(2960,1964.05871582,-348.99044800,1100.00585938,0.00000000,90.00000000,90.00000000,0,1,-1,250); //object(kmb_beam) (3)
	CreateDynamicObject(3872,1964.02685547,-346.40850830,1102.33020020,0.00000000,155.25000000,90.27026367,0,1,-1,250); //object(ws_floodbeams) (1)
	CreateDynamicObject(1667,1962.98828125,-370.41271973,1093.62292480,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(propwineglass1) (1)
	CreateDynamicObject(1664,1962.83520508,-370.42520142,1093.70104980,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(propwinebotl2) (1)
	CreateDynamicObject(2868,1965.95605469,-370.30142212,1093.53430176,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(gb_ornament03) (1)
	CreateDynamicObject(2869,1964.02185059,-373.78527832,1093.66992188,0.00000000,0.00000000,323.75000000,0,1,-1,250); //object(gb_ornament04) (1)
	CreateDynamicObject(2870,1965.26269531,-373.64062500,1093.66992188,0.00000000,0.00000000,350.03002930,0,1,-1,250); //object(gb_ornament05) (1)
	CreateDynamicObject(2270,1965.47998047,-352.47622681,1093.48364258,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(frame_wood_6) (1)
	CreateDynamicObject(2257,1964.05615234,-373.94995117,1095.86132812,0.00000000,0.00000000,180.00000000,0,1,-1,250); //object(frame_clip_4) (1)
	CreateDynamicObject(2271,1962.60437012,-352.58059692,1093.48803711,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(frame_wood_1) (1)
	CreateDynamicObject(2357,1964.05090332,-370.68905640,1093.12402344,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(dunc_dinning) (1)
	CreateDynamicObject(2808,1957.51367188,-364.26171875,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (1)
	CreateDynamicObject(2808,1959.68164062,-364.26171875,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (2)
	CreateDynamicObject(2808,1970.61328125,-364.26171875,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (3)
	CreateDynamicObject(2808,1968.44238281,-364.26171875,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (4)
	CreateDynamicObject(2808,1961.85546875,-364.26171875,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (5)
	CreateDynamicObject(2808,1966.27246094,-364.26171875,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
	CreateDynamicObject(2868,1962.14697266,-370.30624390,1093.53430176,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(gb_ornament03) (4)
	CreateDynamicObject(948,1966.09277344,-374.17285156,1092.72888184,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_10) (1)
	CreateDynamicObject(948,1961.96313477,-374.22888184,1092.72888184,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_10) (2)
	CreateDynamicObject(2894,1963.95080566,-370.94277954,1093.53430176,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(kmb_rhymesbook) (1)
	CreateDynamicObject(3462,1959.30407715,-374.06838989,1094.24255371,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(csrangel_lvs) (1)
	CreateDynamicObject(3462,1968.78125000,-374.06835938,1094.24255371,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(csrangel_lvs) (2)
	CreateDynamicObject(949,1956.02258301,-353.46194458,1095.83190918,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_4) (1)
	CreateDynamicObject(2946,1965.59985352,-349.21301270,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(cr_door_03) (2)
	CreateDynamicObject(2946,1962.47033691,-349.19955444,1091.94543457,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(cr_door_03) (3)
	CreateDynamicObject(2808,1970.60681152,-360.67248535,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
	CreateDynamicObject(2808,1968.43054199,-360.67248535,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
	CreateDynamicObject(2808,1966.25378418,-360.67248535,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
	CreateDynamicObject(2808,1957.52233887,-360.67248535,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
	CreateDynamicObject(2808,1959.69580078,-360.67248535,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
	CreateDynamicObject(2808,1961.86865234,-360.67248535,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
	CreateDynamicObject(2808,1957.51660156,-357.18194580,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
	CreateDynamicObject(2808,1959.69250488,-357.18194580,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
	CreateDynamicObject(2808,1961.86779785,-357.18194580,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
	CreateDynamicObject(2808,1970.61279297,-357.18194580,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
	CreateDynamicObject(2808,1968.43701172,-357.18194580,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
	CreateDynamicObject(2808,1966.26782227,-357.18194580,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
	CreateDynamicObject(14705,1962.68139648,-373.67498779,1093.91894531,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(int2vase) (1)
	CreateDynamicObject(14410,1968.61401367,-349.16000366,1092.01757812,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(carter-stairs03) (1)
	CreateDynamicObject(11472,1971.98046875,-351.97091675,1092.19567871,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(des_swtstairs1) (6)
	CreateDynamicObject(11472,1971.98046875,-353.19616699,1092.19567871,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(des_swtstairs1) (7)
	CreateDynamicObject(14410,1959.47412109,-349.41287231,1092.01660156,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(carter-stairs03) (2)
	CreateDynamicObject(11472,1956.10046387,-351.96487427,1092.19567871,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(des_swtstairs1) (8)
	CreateDynamicObject(11472,1956.10046387,-353.19616699,1092.19567871,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(des_swtstairs1) (9)
	CreateDynamicObject(11472,1965.10974121,-347.94531250,1095.44091797,0.00000000,90.00000000,180.00000000,0,1,-1,250); //object(des_swtstairs1) (10)
	CreateDynamicObject(11472,1962.98315430,-347.94104004,1095.44018555,0.00000000,270.00000000,179.99450684,0,1,-1,250); //object(des_swtstairs1) (11)
	CreateDynamicObject(1698,1962.46606445,-352.53399658,1095.31420898,0.00000000,0.00000000,270.27026367,0,1,-1,250); //object(esc_step8) (1)
	CreateDynamicObject(1698,1962.87316895,-352.53399658,1095.53918457,0.00000000,0.00000000,270.26916504,0,1,-1,250); //object(esc_step8) (2)
	CreateDynamicObject(1698,1963.24682617,-352.53399658,1095.76416016,0.00000000,0.00000000,270.26916504,0,1,-1,250); //object(esc_step8) (3)
	CreateDynamicObject(1698,1965.60021973,-352.53399658,1095.31420898,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(esc_step8) (4)
	CreateDynamicObject(1698,1965.20434570,-352.53399658,1095.53918457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(esc_step8) (5)
	CreateDynamicObject(1698,1964.72375488,-352.53399658,1095.76416016,0.00000000,0.00000000,270.26916504,0,1,-1,250); //object(esc_step8) (6)
	CreateDynamicObject(970,1964.05688477,-353.88497925,1096.74243164,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
	CreateDynamicObject(970,1959.94470215,-353.88497925,1095.74719238,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
	CreateDynamicObject(970,1955.77246094,-353.88589478,1095.74719238,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
	CreateDynamicObject(970,1968.17297363,-353.88497925,1095.74719238,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
	CreateDynamicObject(970,1972.25000000,-353.88589478,1095.74719238,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
	CreateDynamicObject(970,1959.97607422,-351.29064941,1095.74719238,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
	CreateDynamicObject(970,1968.10302734,-351.29064941,1095.74719238,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
	CreateDynamicObject(970,1965.96203613,-349.21618652,1096.74169922,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(fencesmallb) (4)
	CreateDynamicObject(970,1962.13757324,-349.21618652,1096.74243164,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(fencesmallb) (4)
	CreateDynamicObject(2887,1964.04821777,-349.34906006,1094.78955078,90.00000000,0.00000000,0.00000000,0,1,-1,250); //object(a51_spotbulb) (1)
	CreateDynamicObject(949,1971.95068359,-353.46038818,1095.83190918,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_4) (2)
	CreateDynamicObject(948,1965.68554688,-352.45767212,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_10) (1)
	CreateDynamicObject(948,1962.38427734,-352.45767212,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_10) (1)
	CreateDynamicObject(1742,1955.84399414,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (1)
	CreateDynamicObject(1742,1957.28173828,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (2)
	CreateDynamicObject(1742,1958.71166992,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (3)
	CreateDynamicObject(1742,1960.15112305,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (4)
	CreateDynamicObject(1742,1971.77673340,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (5)
	CreateDynamicObject(1742,1970.34277344,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (6)
	CreateDynamicObject(1742,1968.90930176,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (7)
	CreateDynamicObject(1742,1967.47534180,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (8)
	CreateDynamicObject(949,1961.60729980,-354.34103394,1092.58166504,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_4) (3)
	CreateDynamicObject(949,1966.46325684,-354.34805298,1092.58166504,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_4) (4)
	CreateDynamicObject(1720,1970.09338379,-373.59497070,1092.72888184,0.00000000,0.00000000,180.00000000,0,1,-1,250); //object(rest_chair) (1)
	CreateDynamicObject(1720,1970.91198730,-373.58316040,1092.72888184,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(rest_chair) (2)
	CreateDynamicObject(741,1969.29675293,-369.22723389,1093.71252441,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(pot_01) (1)
	CreateDynamicObject(3440,1969.29125977,-370.86264038,1091.08007812,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(arptpillar01_lvs) (1)
	CreateDynamicObject(14455,1955.32812500,-348.54144287,1096.87585449,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(gs_bookcase) (1)
	CreateDynamicObject(14455,1972.78356934,-352.90487671,1096.86743164,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gs_bookcase) (2)
	CreateDynamicObject(2842,1964.51916504,-365.60891724,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (1)
	CreateDynamicObject(2842,1964.51843262,-363.77880859,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (2)
	CreateDynamicObject(2842,1964.51867676,-361.94879150,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (3)
	CreateDynamicObject(2842,1964.52172852,-360.12713623,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (4)
	CreateDynamicObject(2842,1964.52636719,-358.30560303,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (5)
	CreateDynamicObject(2842,1964.52600098,-356.47940063,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (6)
	CreateDynamicObject(2842,1964.52343750,-354.64611816,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (7)
	CreateDynamicObject(2842,1964.52136230,-352.82815552,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (8)
	CreateDynamicObject(2842,1964.51867676,-350.99310303,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (9)
	CreateDynamicObject(2842,1964.52026367,-349.16046143,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (10)
	CreateDynamicObject(2833,1963.55749512,-369.65057373,1092.72888184,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(gb_livingrug02) (1)
	CreateDynamicObject(2834,1968.81579590,-372.52481079,1092.72888184,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(gb_livingrug03) (1)
	CreateDynamicObject(6959,1951.26586914,-360.22650146,1095.56030273,310.00000000,180.00000000,90.00000000,0,1,-1,250); //object(vegasnbball1) (1)
	CreateDynamicObject(6959,1976.87695312,-356.75421143,1095.51965332,310.00000000,180.00000000,270.00000000,0,1,-1,250); //object(vegasnbball1) (3)
	CreateDynamicObject(6959,1958.95715332,-378.23306274,1096.33813477,309.99572754,179.99450684,180.00000000,0,1,-1,250); //object(vegasnbball1) (4)

















	// pomniki
	CreateDynamicMapIcon(-2359,-1637,482.70,33,1,	0,0,-1,1200);			// chiliad
	CreateDynamicObject(11489, -2359,-1637,482.70,	0,0,90,	0,0,-1,300);
	TekstyPomniki[ea_czasgry][0]=CreateDynamic3DTextLabel("Najdluzej grajace osoby",0xFF5219FF,-2359,-1637,482.7+1,	100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0,0,0);
	TekstyPomniki[ea_czasgry][1]=CreateDynamic3DTextLabel("_",0xffffffff,-2359,-1637,482.7+3,	28, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0,0,0);
	TekstyPomniki[ea_czasgry][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0,-2359+2, -1637+5 ,	482.7+2, 28, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0,0,0);
	TekstyPomniki[ea_czasgry][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0,-2359+2, -1637-5 ,	482.7+2, 28, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0,0,0);

	CreateDynamicMapIcon(2191,1004,9.80,33,1,	0,0,-1,1200);			// parking lv
	CreateDynamicObject(11489, 2191,1004,9.80,	0,0,270,	0,0,-1,300);
	TekstyPomniki[ea_skill][0]=CreateDynamic3DTextLabel("Urodzeni zabojcy\ngracze z najwiekszym skillem",0xFF5219FF, 2191, 1004,9.8+1,	100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_skill][1]=CreateDynamic3DTextLabel("_",0xffffffff, 2191, 1004,9.8+3,	30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_skill][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0, 2191-2,  1004+5 ,	  9.8+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_skill][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0, 2191-2,  1004-5 ,	  9.8+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);

	CreateDynamicMapIcon(1083,-1813,12.50,33,1,	0,0,-1,1200);			// parking lv
	CreateDynamicObject(11489, 1083,-1813,12.50,	0,0,270,	0,0,-1,300);
	TekstyPomniki[ea_walizki][0]=CreateDynamic3DTextLabel("Poszukiwacze bonusow",0xFF5219FF, 1083,-1813,12.50+1,	100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_walizki][1]=CreateDynamic3DTextLabel("_",0xffffffff, 1083,-1813,12.5+3,	30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_walizki][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0, 1083-2, -1813+5 ,	 12.5+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_walizki][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0, 1083-2, -1813-5 ,	 12.5+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);





	CreateDynamicMapIcon(-2225,1804,-62.50,33,1,	0,0,-1,1200);			// woda kolo /sf
	CreateDynamicObject(11489,-2225,1804,-62.50,	0,0,0,	0,0,-1,300);
	TekstyPomniki[ea_lowskill][0]=CreateDynamic3DTextLabel("Gracze z najnizszym skillem",0xFF5219FF,-2225, 1804,-62.50+1,	100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_lowskill][1]=CreateDynamic3DTextLabel("_",0xffffffff,-2225, 1804,-62.50+3,	30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_lowskill][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0,-2225+5,  1804-2,	-62.50+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_lowskill][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0,-2225-5,  1804-2 ,-62.50+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);

	CreateDynamicMapIcon(-1739,56,2.50,33,1,	0,0,-1,1200);			// sf suchy dok
	CreateDynamicObject(11489,-1739,56,2.5,	0,0,90,	0,0,-1,300);
	TekstyPomniki[ea_derby][0]=CreateDynamic3DTextLabel("Mistrzowie /derby",0xFF5219FF,-1739, 56,2.50+1,	100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_derby][1]=CreateDynamic3DTextLabel("_",0xffffffff,-1739, 56,2.50+3,	30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_derby][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0,-1739+2,  56+5,2.50+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_derby][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0,-1739+2,  56-5,2.50+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);

	CreateDynamicMapIcon(-2960,-2927,-58.29,33,1,	0,0,-1,1200);			// gdzies w rogu mapy w wodzie
	CreateDynamicObject(11489,-2960,-2927,-58.29,	0,0,90,	0,0,-1,300);
	TekstyPomniki[ea_kickban][0]=CreateDynamic3DTextLabel("Najczesciej wykopywani",0xFF5219FF,-2960,-2927,-58.29+1,	100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_kickban][1]=CreateDynamic3DTextLabel("_",0xffffffff,-2960,-2927,-58.29+3,	30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_kickban][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0,-2960+2,-2927+5,-58.29+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_kickban][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0,-2960+2,-2927-5,-58.29+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);

	// lvlot racedist
	CreateDynamicMapIcon(1632,1678,9.80,33,1,0,0,-1,1200);
	CreateDynamicObject(11489,1632,1678,9.80,	0,0,90,	0,0,-1,300);
	TekstyPomniki[ea_racedist][0]=CreateDynamic3DTextLabel("Mistrzowie /race\nNajwiecej przejechanych km.",0xFF5219FF, 1632,1678,  9.80+1,	100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_racedist][1]=CreateDynamic3DTextLabel("_",0xffffffff, 1632, 1678,  9.80+3,	30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_racedist][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0, 1632+2, 1678+5,  9.80+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_racedist][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0, 1632+2, 1678-5,  9.80+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	
	// arenasolo ilosc wygranych
	CreateDynamicMapIcon(-1154.50,-1025,128.2,33,1,VW_UNIQUE_ARENASOLO,9,-1,1200);
	CreateDynamicObject(11489,-1154.50,-1025,128.2,	0,0,90,	VW_UNIQUE_ARENASOLO, 9, -1, 300);
	TekstyPomniki[ea_soloklrs][0]=CreateDynamic3DTextLabel("Mistrzowie /solo\nNajwiecej wygranych solowek.",0xFF5219FF,-1154.50,-1025,128.20+1,	100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,VW_UNIQUE_ARENASOLO,9);
	TekstyPomniki[ea_soloklrs][1]=CreateDynamic3DTextLabel("_",0xffffffff,-1154.50,-1025,128.20+3,	30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,VW_UNIQUE_ARENASOLO,9);
	TekstyPomniki[ea_soloklrs][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0,-1154.50+2,-1025+5,128.20+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,VW_UNIQUE_ARENASOLO,9);
	TekstyPomniki[ea_soloklrs][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0,-1154.50+2,-1025-5,128.20+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,VW_UNIQUE_ARENASOLO,9);

	// kwota mandatow
	CreateDynamicMapIcon(625.0,-590.0,15.5,33,1,0,0,-1,1200);
	CreateDynamicObject(11489,625,-590,15.5,	0,0,90,	0,0, -1, 300);
	TekstyPomniki[ea_fotocash][0]=CreateDynamic3DTextLabel("Najwieksi piraci drogowi\nNajwieksze mandaty za predkosc",0xFF5219FF,625.00,-590,15.50+1,	100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_fotocash][1]=CreateDynamic3DTextLabel("_",0xffffffff,625.00,-590,15.50+3,	30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_fotocash][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0,625.00+2,-590+5,15.50+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
	TekstyPomniki[ea_fotocash][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0,625.0+2,-590-5,15.50+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);


	obiekty_odswiezPomniki(true);
	SetTimer("Pobiekty_odswiezPomniki",1000*60*1,true);
		
}

new Float:spawns_lv_balkony[][3]={
	{2179.02,1037.36,32.76},
	{2180.89,1031.51,32.76},
	{2176.75,1043.13,32.76},
	{2179.02,1037.36,29.40},
	{2180.89,1031.51,29.40},
	{2176.75,1043.13,29.40}
};

obiekty_OPPickUpDynamicPickup(playerid,pickupid){
	new bool:pickupDelayActive=false;
	if (GetTickCount()-pTemp[playerid][pickupDelay] < MIN_PICKUP_DELAY)
		pickupDelayActive=true;

	// rozdzielic na dwie tablice dla roznych interiorow TODO
	if (!pickupDelayActive) {
			for(new i=0;i<TelePickupsCount;i++)
				if(pickupid==TelePickups[i][etp_pickupid]) {
						if (TelePickups[i][etp_restrict_gang]>0 && 
							(TelePickups[i][etp_restrict_gang]!=pData[playerid][gang])) {
							new buf[128];
							format(buf, sizeof buf, "Tylko dla czlonkow klanu {b}%s{/b}!", gData[TelePickups[i][etp_restrict_gang]][name]);
							Msg(playerid,COLOR_ERROR,buf);
							return 1;
						}
						if (TelePickups[i][etp_restrict_level]>0 && pData[playerid][adminLevel]<TelePickups[i][etp_restrict_level] && pData[playerid][adminLevel]!=LEVEL_ADMINHIDDEN) {
							// telepickup dla wyzszego poziomu
							switch(TelePickups[i][etp_restrict_level]) {
								case 1:
									Msg(playerid,COLOR_ERROR,"Wejscie tylko dla {b}moderatorow{/b}!");
								case 2,3:
									Msg(playerid,COLOR_ERROR,"Wejscie tylko dla {b}administratorow{/b}!");
								case 4:
									Msg(playerid,COLOR_ERROR,"Wejscie tylko dla {b}RCONow{/b}!");
							}
							return 1;
						}
							
						if (TelePickups[i][etp_tPX]==0 && TelePickups[i][etp_tPY]==0)
							return Msg(playerid, COLOR_INFO2, "Zamkniete.");
						SaveReturnPath(playerid,TelePickups[i][etp_tI]);
						Teleport(T_PLAYER,playerid,TelePickups[i][etp_tPX],TelePickups[i][etp_tPY],TelePickups[i][etp_tPZ],TelePickups[i][etp_tPA],TelePickups[i][etp_tI],TelePickups[i][etp_tVW],true);
						return 1;
				}
	}


	if (GetPlayerInterior(playerid)==0) {
		if (pickupid==PICKUPs[eP_lv_balkony]){
			new r=random(sizeof spawns_lv_balkony);
			Teleport(T_PLAYER,playerid,spawns_lv_balkony[r][0],spawns_lv_balkony[r][1],spawns_lv_balkony[r][2],70+random(90),0,0,true);
			return 1;
		} /*else if (pickupid==PICKUPs[eP_powerpickup_baza3] || pickupid==PICKUPs[eP_powerpickup_przystanekLS] || pickupid==PICKUPs[eP_powerpickup_przystanekLV]) {
			Msg(playerid,COLOR_INFO,"Dostales {b}mega zestaw broni{/b}!");
			ResetPlayerWeapons(playerid);
			GivePlayerWeapon(playerid, 24,3000);
			GivePlayerWeapon(playerid, 28,3000);
			GivePlayerWeapon(playerid, 31,3000);
			GivePlayerWeapon(playerid, 26,3000);
			GivePlayerWeapon(playerid, 34,3000);
			SetPlayerHealth(playerid, 100);
			SetPlayerArmour(playerid,100);
			PlaySound(playerid, 1039);
			
			return 1;
		}*/ 
		else if (pickupid==PICKUPs[eP_F_wojsko1] || pickupid==PICKUPs[eP_F_wojsko2]) {
			factions_PlayerJoin(playerid,FACTION_ARMY);
			return 1;
		} else if (pickupid==PICKUPs[eP_F_dilerzy]) {
			factions_PlayerJoin(playerid,FACTION_DILERZY);
			return 1;
		}
	} else { // interior inny niz 0
		if (pickupid==PICKUPs[eP_F_policja_LS] || pickupid==PICKUPs[eP_F_policja_LV] || pickupid==PICKUPs[eP_F_policja_LV]) {
			factions_PlayerJoin(playerid,FACTION_POLICE);
			return 1;
		} else 
		if (!pickupDelayActive)
		for(new i=0;i<ReturnPickupsCount;i++){
				if(pickupid==ReturnPickups[i]) {
					UseReturnPath(playerid);
					return 1;
				}
		}	
	}
	return 0;
}

obiekty_OPEnterDynamicCP(playerid,checkpointid){
	if (GetPlayerVirtualWorld(playerid)==0) {

	 if (checkpointid==Checkpoints[ec_warsztat_tablice]) {
			warsztat_zmianaTablicy(playerid);
			return 1;
	 } else if (checkpointid==Checkpoints[ec_warsztatlv_skup])
			return warsztat_CPskup(playerid);
	 else if (checkpointid==Checkpoints[ec_warsztatlv_myjnia])
			return warsztat_CPmyjnia(playerid);
	 else if (checkpointid==Checkpoints[ec_warsztatlv_drift])
			return warsztat_CPdrift(playerid);

	 if (GetTickCount()-pTemp[playerid][teleCPDelay] > MIN_TELECP_DELAY)
		 for(new i=0; i < TeleCPCount; i++)
			if(TeleCheckpoints[i][etcp_cp][0]==checkpointid || TeleCheckpoints[i][etcp_cp][1]==checkpointid) 
				return obiekty_OPETeleCheckpoint(playerid,checkpointid,i);
		
		

	 if (!IsPlayerInAnyVehicle(playerid))
		for(new i = 0; i < sizeof ATMS; i++)
			if(ATMS[i][eatm_cp]==checkpointid) {
				if(!pData[playerid][loggedIn]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 110)); // Komenda dostêpna tylko dla zarejestrowanych u¿ytkowników.
				ShowPlayerDialog(playerid, DIALOG_BANK_MAIN, DIALOG_STYLE_LIST, "Bankomat", GetDialogList(playerid, DIALOG_BANK_MAIN), TXT(playerid, 76), TXT(playerid, 54));
				return 1;
			}
	}
	return 0;
}

CreateATMs()
{
	for(new i = 0; i < sizeof DATA_ATM; i++)
	{
		CreateDynamicObject(2618, DATA_ATM[i][X], DATA_ATM[i][Y], DATA_ATM[i][Z], DATA_ATM[i][rX], DATA_ATM[i][rY], DATA_ATM[i][rZ]);
		ATMS[i][eatm_mapicon]=CreateDynamicMapIcon(DATA_ATM[i][X], DATA_ATM[i][Y], DATA_ATM[i][Z], 52, 6, 0, DATA_ATM[i][atmInterior], -1, 300.0);
		Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, ATMS[i][eatm_mapicon], E_STREAMER_STYLE, 2);
//		Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, ATMS[i][eatm_mapicon], E_STREAMER_SIZE, random(10));	// testy!
		ATMS[i][eatm_cp]=CreateDynamicCP(DATA_ATM[i][X], DATA_ATM[i][Y], DATA_ATM[i][Z], 1.3, 0,DATA_ATM[i][atmInterior],-1,15.0);
	}
}

CreatePickups()
{
//	for(new i=0;i<sizeof DATA_AMMUNATIONS;i++){
//		PICKUPs_AMMUNATIONS[i]=CreateDynamicPickup(1318,1,DATA_AMMUNATIONS[i][0],DATA_AMMUNATIONS[i][1],DATA_AMMUNATIONS[i][2],0,0,-1,100.0);
//	}
//	for(new i=0;i<sizeof DATA_RETURNDOORS;i++){
//		PICKUPs_RETURNS[i]=CreateDynamicPickup(1318,1,DATA_RETURNDOORS[i][0],DATA_RETURNDOORS[i][1],DATA_RETURNDOORS[i][2],floatround(DATA_RETURNDOORS[i][4]),floatround(DATA_RETURNDOORS[i][3]),-1,100.0);
//	}
}


obiekty_odswiezTelePickups(bool:tdebug=false){
	mysql_query("SELECT id,pX,pY,pZ,pi,pvw,pickupid,tX,tY,tZ,ti,tvw,tA,pokazopis,IFNULL(opis,'-'),mapicon,mapicon_type,opis_color,restrict_gang,restrict_level FROM fs_telepickups");
	mysql_store_result();
	new Float:pP[3], pI, pVW, pickupid, buffer[255], mapicon, id, mapicon_type, opis_color;
	new i=0;
	while(mysql_fetch_row(buffer,"|")){
		if (IsValidDynamicPickup(TelePickups[i][etp_pickupid]))
			DestroyDynamicPickup(TelePickups[i][etp_pickupid]);
		if (IsValidDynamic3DTextLabel(TelePickups[i][etp_t3d]))
			DestroyDynamic3DTextLabel(TelePickups[i][etp_t3d]);
		if (IsValidDynamicMapIcon(TelePickups[i][etp_mapicon]))
			DestroyDynamicMapIcon(TelePickups[i][etp_mapicon]);

		if (sscanf(buffer,"p<|>dfffdddfffddfds[64]ddddd", id,
				pP[0], pP[1], pP[2], pI, pVW, pickupid, 
				TelePickups[i][etp_tPX],
				TelePickups[i][etp_tPY],TelePickups[i][etp_tPZ], TelePickups[i][etp_tI],TelePickups[i][etp_tVW], TelePickups[i][etp_tPA], 
				TelePickups[i][etp_pokazopis], TelePickups[i][etp_opis], mapicon, mapicon_type, opis_color, TelePickups[i][etp_restrict_gang], TelePickups[i][etp_restrict_level])) continue;

		TelePickups[i][etp_pickupid]=CreateDynamicPickup(pickupid, 1, pP[0], pP[1], pP[2]+0.25, pVW, pI, -1);

		if (TelePickups[i][etp_pokazopis]) {
			if (tdebug) format(TelePickups[i][etp_opis], 64, "%s[%d]", TelePickups[i][etp_opis], id);
			TelePickups[i][etp_t3d]=CreateDynamic3DTextLabel(TelePickups[i][etp_opis],opis_color*256+0xff, pP[0], pP[1], pP[2]+1, 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, pVW, pI);
		} else if (tdebug) {
			format(TelePickups[i][etp_opis], 64, "[%d]", id);
			TelePickups[i][etp_t3d]=CreateDynamic3DTextLabel(TelePickups[i][etp_opis],opis_color*256+0xff, pP[0], pP[1], pP[2]+1, 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, pVW, pI);
		}
		if (mapicon>0) {
			TelePickups[i][etp_mapicon]=CreateDynamicMapIcon(pP[0], pP[1], pP[2], mapicon, 1, pVW, pI, -1, 1200.0);
			Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, TelePickups[i][etp_mapicon], E_STREAMER_STYLE, mapicon_type);
		}
		i++;
	}
	TelePickupsCount=i;
	for (;i<MAX_TELEPICKUPS;i++) {
		if (IsValidDynamicPickup(TelePickups[i][etp_pickupid]))
			DestroyDynamicPickup(TelePickups[i][etp_pickupid]);
		if (IsValidDynamic3DTextLabel(TelePickups[i][etp_t3d]))
			DestroyDynamic3DTextLabel(TelePickups[i][etp_t3d]);
		if (IsValidDynamicMapIcon(TelePickups[i][etp_mapicon]))
			DestroyDynamicMapIcon(TelePickups[i][etp_mapicon]);

	}


	mysql_free_result();
	return TelePickupsCount;
}

obiekty_odswiezReturnPickups(){
	mysql_query("SELECT pX,pY,pZ,pi,pvw,pickupid FROM fs_returnpickups");
	mysql_store_result();
	new Float:pP[3], pI, pVW, pickupid, buffer[255];
	new i=0;
	while(mysql_fetch_row(buffer,"|")){
		if (IsValidDynamicPickup(ReturnPickups[i]))
			DestroyDynamicPickup(ReturnPickups[i]);

		if (sscanf(buffer,"p<|>fffddd", 
				pP[0], pP[1], pP[2], pI, pVW, pickupid)) continue;

		ReturnPickups[i]=CreateDynamicPickup(pickupid, 1, pP[0], pP[1], pP[2], pVW, pI, -1);
		
		i++;
	}
	mysql_free_result();
	ReturnPickupsCount=i;
	for (;i<MAX_RETURNPICKUPS;i++)
		if (IsValidDynamicPickup(ReturnPickups[i]))
			DestroyDynamicPickup(ReturnPickups[i]);


	return ReturnPickupsCount;
}

obiekty_odswiezMapIcons(){
	mysql_query("SELECT pX,pY,pZ,pi,pvw,mapicon,type,loadingdistance,opis FROM fs_mapicons");
	mysql_store_result();
	new Float:pP[3], pI, pVW, mapicon, type, Float:dist, opis[65], buffer[255];
	new i=0;
	while(mysql_fetch_row(buffer,"|")){
		
		if (IsValidDynamicMapIcon(MapIcons[i]))
			DestroyDynamicMapIcon(MapIcons[i]);

		if (sscanf(buffer,"p<|>fffddddfs[64]", 
				pP[0], pP[1], pP[2], pI, pVW, mapicon, type, dist, opis)) continue;

		if (dist<=0) dist=1200;

		if (pI<=0)	{ 	// mapikonki nie dzialaja w interiorach
			MapIcons[i]=CreateDynamicMapIcon(pP[0], pP[1], pP[2], mapicon, 1, pVW, pI, -1, dist);
			Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, MapIcons[i], E_STREAMER_STYLE, type);
		}
		i++;
	}
	mysql_free_result();
	MapIconsCount=i;
	for (;i<MAX_MAPICONS;i++)
		if (IsValidDynamicMapIcon(MapIcons[i]))
			DestroyDynamicMapIcon(MapIcons[i]);

	return MapIconsCount;	
}

obiekty_odswiezTeleCheckpoints(){
	mysql_query("SELECT fPOS,IFNULL(fOPIS,'-'),dPOS,IFNULL(dOPIS,'-'),rozmiar FROM fs_telecheckpoints WHERE aktywny=1");
	mysql_store_result();
	new buffer[255];
	new i=0;
	while(mysql_fetch_row(buffer,"|")){
		new fPOS[64], dPOS[64], cpSize;
		if (IsValidDynamicCP(TeleCheckpoints[i][etcp_cp][0]))
			DestroyDynamicCP(TeleCheckpoints[i][etcp_cp][0]);
		if (IsValidDynamicCP(TeleCheckpoints[i][etcp_cp][1]))
			DestroyDynamicCP(TeleCheckpoints[i][etcp_cp][1]);

		if (IsValidDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][0]))
			DestroyDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][0]);

		if (IsValidDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][1]))
			DestroyDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][1]);



		if (sscanf(buffer,"p<|>s[64]s[64]s[64]s[64]d", 
				fPOS, TeleCheckpoints[i][etcp_opis1], dPOS, TeleCheckpoints[i][etcp_opis2], cpSize)) continue;
		if (sscanf(fPOS, "p<,>ffffd",
			TeleCheckpoints[i][etcp_P1][0], TeleCheckpoints[i][etcp_P1][1], TeleCheckpoints[i][etcp_P1][2], TeleCheckpoints[i][etcp_P1][3], TeleCheckpoints[i][etcp_I][0]))
				continue;
		if (sscanf(dPOS, "p<,>ffffd",
			TeleCheckpoints[i][etcp_P2][0], TeleCheckpoints[i][etcp_P2][1], TeleCheckpoints[i][etcp_P2][2], TeleCheckpoints[i][etcp_P2][3], TeleCheckpoints[i][etcp_I][1]))
				continue;
//		SendClientMessageToAll(-1, "Dodawanie TCP");

		TeleCheckpoints[i][etcp_cp][0]=CreateDynamicCP(TeleCheckpoints[i][etcp_P1][0], TeleCheckpoints[i][etcp_P1][1], TeleCheckpoints[i][etcp_P1][2], cpSize, 0, TeleCheckpoints[i][etcp_I][0], -1);
		TeleCheckpoints[i][etcp_cp][1]=CreateDynamicCP(TeleCheckpoints[i][etcp_P2][0], TeleCheckpoints[i][etcp_P2][1], TeleCheckpoints[i][etcp_P2][2], cpSize, 0, TeleCheckpoints[i][etcp_I][1], -1);

		if (strlen(TeleCheckpoints[i][etcp_opis1])>2)
			TeleCheckpoints[i][etcp_t3d][0]=CreateDynamic3DTextLabel(TeleCheckpoints[i][etcp_opis1],0xFFFFFFAA, TeleCheckpoints[i][etcp_P1][0], TeleCheckpoints[i][etcp_P1][1], TeleCheckpoints[i][etcp_P1][2]+1, 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, TeleCheckpoints[i][etcp_I][0]);
		if (strlen(TeleCheckpoints[i][etcp_opis2])>2)
			TeleCheckpoints[i][etcp_t3d][1]=CreateDynamic3DTextLabel(TeleCheckpoints[i][etcp_opis2],0x0E073CFF, TeleCheckpoints[i][etcp_P2][0], TeleCheckpoints[i][etcp_P2][1], TeleCheckpoints[i][etcp_P2][2]+1, 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, TeleCheckpoints[i][etcp_I][1]);

		i++;
	}
	mysql_free_result();
	TeleCPCount=i;
	for (;i<MAX_TELECHECKPOINTS;i++) {
		if (IsValidDynamicCP(TeleCheckpoints[i][etcp_cp][0]))
			DestroyDynamicCP(TeleCheckpoints[i][etcp_cp][0]);
		if (IsValidDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][0]))
			DestroyDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][0]);
		
		if (IsValidDynamicCP(TeleCheckpoints[i][etcp_cp][1]))
			DestroyDynamicCP(TeleCheckpoints[i][etcp_cp][1]);

		if (IsValidDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][1]))
			DestroyDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][1]);


	}


	return TeleCPCount;
}

obiekty_odswiezMiscPickups(){
	mysql_query("SELECT pX,pY,pZ,interior,vw,pickupid FROM fs_miscpickups");
	mysql_store_result();
	new Float:pP[3], pI, pVW, pickupid, buffer[255];
	new i=0;
	while(mysql_fetch_row(buffer,"|")){
		if (IsValidDynamicPickup(MiscPickups[i]))
			DestroyDynamicPickup(MiscPickups[i]);

		if (sscanf(buffer,"p<|>fffddd", 
				pP[0], pP[1], pP[2], pI, pVW, pickupid)) continue;

		MiscPickups[i]=CreateDynamicPickup(pickupid, 2, pP[0], pP[1], pP[2], pVW, pI, -1);
		
		i++;
	}
	mysql_free_result();
	MiscPickupsCount=i;
	for (;i<MAX_MISCPICKUPS;i++)
		if (IsValidDynamicPickup(MiscPickups[i]))
			DestroyDynamicPickup(MiscPickups[i]);


	return MiscPickupsCount;
}



obiekty_OPETeleCheckpoint(playerid,checkpointid,i){
	if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));

	pTemp[playerid][teleCPDelay]=GetTickCount();

	if (TeleCheckpoints[i][etcp_cp][0]==checkpointid) {
		Teleport(T_BOTH, playerid, TeleCheckpoints[i][etcp_P2][0] , TeleCheckpoints[i][etcp_P2][1], TeleCheckpoints[i][etcp_P2][2], TeleCheckpoints[i][etcp_P2][3]+random(10)-5, TeleCheckpoints[i][etcp_I][1], VW_DEFAULT);
		if (strlen(TeleCheckpoints[i][etcp_opis2])>2)
			GameTextForPlayer(playerid, TeleCheckpoints[i][etcp_opis2], 1000, 1);

	}
	else if (TeleCheckpoints[i][etcp_cp][1]==checkpointid) {
		Teleport(T_BOTH, playerid, TeleCheckpoints[i][etcp_P1][0] , TeleCheckpoints[i][etcp_P1][1], TeleCheckpoints[i][etcp_P1][2], TeleCheckpoints[i][etcp_P1][3]+random(10)-5, TeleCheckpoints[i][etcp_I][0], VW_DEFAULT);
		if (strlen(TeleCheckpoints[i][etcp_opis1])>2)
			GameTextForPlayer(playerid, TeleCheckpoints[i][etcp_opis1], 1000, 1);

	} 

	return 1;
	
}

CMD:tepireload(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
//	if (!IsGM(playerid)) return 0;
	new buf[128];
	new cnt=obiekty_odswiezTelePickups(!isnull(params));
	format(buf, sizeof buf, "TelePickups: Odswiezono %d elementow", cnt);
	Msg(playerid, COLOR_INFO, buf);

	cnt=obiekty_odswiezReturnPickups();
	format(buf, sizeof buf, "ReturnPickups: Odswiezono %d elementow", cnt);
	Msg(playerid, COLOR_INFO, buf);

	cnt=obiekty_odswiezMapIcons();
	format(buf, sizeof buf, "MapIcons: Odswiezono %d elementow", cnt);
	Msg(playerid, COLOR_INFO, buf);


	cnt=obiekty_odswiezTeleCheckpoints();
	format(buf, sizeof buf, "TeleCheckpoints: Odswiezono %d elementow", cnt);
	Msg(playerid, COLOR_INFO, buf);

	cnt=obiekty_odswiezMiscPickups();
	format(buf, sizeof buf, "MiscPickups: Odswiezono %d elementow", cnt);
	return Msg(playerid, COLOR_INFO, buf);
}

CMD:addtepi(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
	if (IsPlayerInAnyVehicle(playerid)) return Msg(playerid,COLOR_ERROR,"Musisz byc pieszo!");
	if (isnull(params)) return Msg(playerid,COLOR_ERROR,"/addtepi <nazwa>");
	new Float:PP[3], int;
	GetPlayerPos(playerid, PP[0], PP[1], PP[2]);
	int=GetPlayerInterior(playerid);
	new buf[256],desc[64];
	mysql_real_escape_string(params,desc);
	format(buf,sizeof buf,"INSERT INTO fs_telepickups SET pX=%.3f,pY=%.3f,pZ=%.3f,pi=%d,pvw=0,pickupid=1318,tX=0,tY=0,tZ=5,ti=0,tA=0,opis='%s'",
				PP[0], PP[1], PP[2], int, desc);
//	SendClientMessageToAll(-1, buf);
	mysql_query(buf);
	format(buf, sizeof buf, "Dodano telepickup, ID: %d", mysql_insert_id());
	Msg(playerid,COLOR_INFO,buf);
	return 1;
}

CMD:addrepi(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
	if (IsPlayerInAnyVehicle(playerid)) return Msg(playerid,COLOR_ERROR,"Musisz byc pieszo!");
	if (GetPlayerInterior(playerid)==0) return Msg(playerid,COLOR_ERROR,"Pickupy powrotne nie moga byc w interiorze 0");
	new Float:PP[3], int;
	GetPlayerPos(playerid, PP[0], PP[1], PP[2]);
	int=GetPlayerInterior(playerid);
	new buf[256],desc[64];
	mysql_real_escape_string(params,desc);
	format(buf,sizeof buf,"INSERT INTO fs_returnpickups SET pX=%.3f,pY=%.3f,pZ=%.3f,pi=%d,pvw=-1,pickupid=1318,opis='%s'",
				PP[0], PP[1], PP[2], int, desc);
//	SendClientMessageToAll(-1, buf);
	mysql_query(buf);
	format(buf, sizeof buf, "Dodano returnpickup, ID: %d", mysql_insert_id());
	Msg(playerid,COLOR_INFO,buf);
	return 1;
}

CMD:addmipi(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
//	if (!IsGM(playerid)) return 0;
	if (IsPlayerInAnyVehicle(playerid)) return Msg(playerid,COLOR_ERROR,"Musisz byc pieszo!");
	new Float:PP[3], int, pickupid, opis[65];

	if (sscanf(params,"ds[64]",pickupid, opis))
		return Msg(playerid, COLOR_ERROR, "/addmipi <pickupid> <opis>");

	GetPlayerPos(playerid, PP[0], PP[1], PP[2]);
	int=GetPlayerInterior(playerid);
	new buf[256],desc[70];
	mysql_real_escape_string(opis,desc);
	format(buf,sizeof buf,"INSERT INTO fs_miscpickups SET pX=%.3f,pY=%.3f,pZ=%.3f,interior=%d,vw=0,pickupid=%d,descr='%s',addedby=%d",
				PP[0], PP[1], PP[2], int, pickupid, desc, pData[playerid][accountID]);
//	SendClientMessageToAll(-1, buf);
	mysql_query(buf);
	format(buf, sizeof buf, "Dodano miscpickup, ID: %d", mysql_insert_id());
	Msg(playerid,COLOR_INFO,buf);
	return 1;
}


CMD:tepitu(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
	new Float:PP[4], int, pid;

	if (sscanf(params,"d",pid))
		return Msg(playerid,COLOR_INFO2,"/tepitu [tepi id]");

	if (IsPlayerInAnyVehicle(playerid)) return Msg(playerid,COLOR_ERROR,"Musisz byc pieszo!");

	GetPlayerPos(playerid, PP[0], PP[1], PP[2]);
	int=GetPlayerInterior(playerid);
	GetPlayerFacingAngle(playerid,PP[3]);

	new buf[256];
	format(buf,sizeof buf, "UPDATE fs_telepickups SET tX=%.3f,tY=%.3f,tZ=%.3f,ti=%d,tA=%.1f WHERE id=%d",
			PP[0], PP[1], PP[2], int, PP[3], pid);
//	SendClientMessageToAll(-1,buf);
	mysql_query(buf);
	Msg(playerid,COLOR_INFO,"Zrobione");
	return 1;	
}

CMD:addmapicon(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
//	if (!IsGM(playerid)) return 0;
	if (IsPlayerInAnyVehicle(playerid)) return Msg(playerid,COLOR_ERROR,"Musisz byc pieszo!");

	new Float:PP[3], int, mapicon, opis[64]="-";

	if (sscanf(params,"ds[63]",mapicon,opis))
		return Msg(playerid,COLOR_INFO2,"/addmapicon <id-ikonki> [opis]");

	if (mapicon<0 || mapicon>63)
		return Msg(playerid,COLOR_INFO2,"Prawidlowy zakres id ikonki to 1-63. Lista pod http://wiki.sa-mp.com/wiki/MapIcons");
	GetPlayerPos(playerid, PP[0], PP[1], PP[2]);
	int=GetPlayerInterior(playerid);
	new buf[256],desc[64];
	mysql_real_escape_string(opis,desc);
	format(buf,sizeof buf,"INSERT INTO fs_mapicons SET pX=%.3f,pY=%.3f,pZ=%.3f,pi=%d,pvw=0,mapicon=%d,opis='%s'",
				PP[0], PP[1], PP[2], int, mapicon, desc);
	mysql_query(buf);
	format(buf, sizeof buf, "Dodano mapicon, ID: %d", mysql_insert_id());
	Msg(playerid,COLOR_INFO,buf);
	return 1;
}

public Pobiekty_odswiezPomniki()
	obiekty_odswiezPomniki();

obiekty_odswiezPomniki(bool:force=false){
 new pid, pval, pval2, pnick[32], buffer[255], prespect,i;
 if (force || random(3)==1) {
	mysql_query("SELECT id,session,nick,respect FROM `fs_players` ORDER BY session DESC LIMIT 3;");
	mysql_store_result();
	i=0;
//	UpdateDynamic3DTextLabelText(TekstyPomniki[ea_czasgry][0], 0xFF5219FF, "Najdluzej grajace osoby:");
	if (mysql_num_rows())
	while (mysql_fetch_row(buffer,"|")) {
		i++;
		if(sscanf(buffer,"p<|>dds[32]d", pid, pval, pnick, prespect))
				format(buffer,sizeof buffer, "-");
		else
			format(buffer,sizeof buffer, "%d. %s\n{ffffff}%dh\n{d0d0d0}Respekt: %d", i, pnick, floatround(pval/3600), prespect);
		UpdateDynamic3DTextLabelText(TekstyPomniki[ea_czasgry][i], 0xFF5219FF, buffer);
	}
	mysql_free_result();
 }

 if (force || random(3)==1) {
	// skill
	mysql_query("SELECT id,skill,nick,respect FROM `fs_players` ORDER BY skill DESC LIMIT 3;");
	mysql_store_result();
	i=0;
//	UpdateDynamic3DTextLabelText(TekstyPomniki[ea_skill][0], 0xFF5219FF, "Gracze z najwiekszym skillem");
	if (mysql_num_rows())
	while (mysql_fetch_row(buffer,"|")) {
		i++;
		if(sscanf(buffer,"p<|>dds[32]d", pid, pval, pnick, prespect))
				format(buffer,sizeof buffer, "-");
		else
			format(buffer,sizeof buffer, "%d. %s\n{ffffff}Skill: %d\n{d0d0d0}Respekt: %d", i, pnick, pval, prespect);
		UpdateDynamic3DTextLabelText(TekstyPomniki[ea_skill][i], 0xFF5219FF, buffer);
	}
	mysql_free_result();
 }

	// bonusy
 if (force || random(3)==1) {
	mysql_query("select p.nick,pa.score from fs_players_achievements pa join fs_players p on pa.playerid=p.id where pa.shortname='WALIZKI' ORDER BY score DESC limit 3");
	mysql_store_result();
	i=0;
//	UpdateDynamic3DTextLabelText(TekstyPomniki[ea_skill][0], 0xFF5219FF, "Gracze z najwiekszym skillem");
	if (mysql_num_rows())
	while (mysql_fetch_row(buffer,"|")) {
		i++;
		if(sscanf(buffer,"p<|>s[32]d", pnick, pval))
				format(buffer,sizeof buffer, "-");
		else
			format(buffer,sizeof buffer, "%d. %s\n{ffffff}%d %s", i, pnick, pval, dli(pval,"znaleziony bonus","znalezione bonusy","znalezionych bonusow"));
		UpdateDynamic3DTextLabelText(TekstyPomniki[ea_walizki][i], 0xFF5219FF, buffer);
	}
	mysql_free_result();
 }




 if (force || random(3)==1) {
	// niski skill
	mysql_query("SELECT id,skill,nick,respect FROM `fs_players` ORDER BY skill ASC LIMIT 3;");
	mysql_store_result();
	i=0;
//	UpdateDynamic3DTextLabelText(TekstyPomniki[ea_lowskill][0], 0xFF5219FF, "Gracze z najnizszym skillem");
	if (mysql_num_rows())
	while (mysql_fetch_row(buffer,"|")) {
		i++;
		if(sscanf(buffer,"p<|>dds[32]d", pid, pval, pnick, prespect))
				format(buffer,sizeof buffer, "-");
		else
			format(buffer,sizeof buffer, "%d. %s\n{ffffff}Skill: %d\n{d0d0d0}Respekt: %d", i, pnick, pval, prespect);
		UpdateDynamic3DTextLabelText(TekstyPomniki[ea_lowskill][i], 0xFF5219FF, buffer);
	}
	mysql_free_result();
 }


 if (force || random(3)==1) {
	// mistrzowie /derby
	mysql_query("SELECT pa.playerid,p.nick,pa.score derbywon,floor(pa.score/pa2.score*100) procent FROM fs_players_achievements pa JOIN fs_players_achievements pa2 ON pa2.playerid=pa.playerid AND pa2.shortname='DERBYFAN' JOIN fs_players p on pa.playerid=p.id WHERE pa.shortname='DERBYWIN' ORDER BY pa.score DESC limit 3;");
	mysql_store_result();
	i=0;
//	UpdateDynamic3DTextLabelText(TekstyPomniki[ea_derby][0], 0xFF5219FF, "Mistrzowie /derby");
	if (mysql_num_rows())
	while (mysql_fetch_row(buffer,"|")) {
		i++;
		if(sscanf(buffer,"p<|>ds[32]dd", pid, pnick, pval, pval2))
				format(buffer,sizeof buffer, "-");
		else
			format(buffer,sizeof buffer, "%d. %s\n{ffffff}Wygranych: %d\n{d0d0d0}Wygrywa %01d%% pojedynkow", i, pnick, pval, pval2);
		UpdateDynamic3DTextLabelText(TekstyPomniki[ea_derby][i], 0xFF5219FF, buffer);
	}
	mysql_free_result();
 }

 if (force || random(3)==1) {
	// mandaty
	mysql_query("SELECT pa.playerid,p.nick,pa.score mandaty FROM fs_players_achievements pa JOIN fs_players p on pa.playerid=p.id WHERE pa.shortname='FOTOCASH' ORDER BY pa.score DESC limit 3;");
	mysql_store_result();
	i=0;
//	UpdateDynamic3DTextLabelText(TekstyPomniki[ea_derby][0], 0xFF5219FF, "Mistrzowie /derby");
	if (mysql_num_rows())
	while (mysql_fetch_row(buffer,"|")) {
		i++;
		if(sscanf(buffer,"p<|>ds[32]d", pid, pnick, pval))
				format(buffer,sizeof buffer, "-");
		else
			format(buffer,sizeof buffer, "%d. %s\n{ffffff}%d$", i, pnick, pval);
		UpdateDynamic3DTextLabelText(TekstyPomniki[ea_fotocash][i], 0xFF5219FF, buffer);
	}
	mysql_free_result();
 }


 if (force || random(5)==1) {
	// Kick/ban
	mysql_query("SELECT nick,kick_count,ban_count from fs_players order by kick_count desc limit 3;");
	mysql_store_result();
	i=0;
//	UpdateDynamic3DTextLabelText(TekstyPomniki[ea_kickban][0], 0xFF5219FF, "Najczesciej wykopywani");
	if (mysql_num_rows())
	while (mysql_fetch_row(buffer,"|")) {
		i++;
		if(sscanf(buffer,"p<|>s[32]dd", pnick, pval, pval2))
				format(buffer,sizeof buffer, "-");
		else
			format(buffer,sizeof buffer, "%d. %s\n{ffffff}%d %s\n{d0d0d0}%d %s.", i, pnick, pval, dli(pval,"kick","kicki","kickow"), pval2, dli(pval2,"ban","bany","banow"));
		UpdateDynamic3DTextLabelText(TekstyPomniki[ea_kickban][i], 0xFF5219FF, buffer);
	}
	mysql_free_result();
 }

 if (force || random(3)==1) {
	// mistrzowie /race
	// select pa.playerid,p.nick,pa.score racedist,pa2.score racefcnt from fs_players_achievements pa JOIN fs_players p ON p.id=pa.playerid JOIN fs_players_achievements pa2 ON pa2.playerid=pa.playerid AND pa2.shortname='RACEFCNT' WHERE pa.shortname='RACEDIST' order by pa.score DESC limit 3;
	mysql_query("select p.nick,pa.score racedist,pa2.score racefcnt from fs_players_achievements pa JOIN fs_players p ON p.id=pa.playerid JOIN fs_players_achievements pa2 ON pa2.playerid=pa.playerid AND pa2.shortname='RACEFCNT' WHERE pa.shortname='RACEDIST' order by pa.score DESC limit 3;");
	mysql_store_result();
	i=0;
//	UpdateDynamic3DTextLabelText(TekstyPomniki[ea_racedist][0], 0xFF5219FF, "Mistrzowie /race\nNajwiecej przejechanych km.");
	if (mysql_num_rows())
	while (mysql_fetch_row(buffer,"|")) {
		i++;
		if(sscanf(buffer,"p<|>s[32]dd", pnick, pval, pval2))
				format(buffer,sizeof buffer, "-");
		else
			format(buffer,sizeof buffer, "%d. %s\n{ffffff}Przejechanych %dKM\n{d0d0d0}w %d %s.", i, pnick, pval, pval2, dli(pval2,"wyscigu","wyscigach","wyscigach"));//
		//dli(pval2,"wyscigu","wyscigach","wyscigach") );
		UpdateDynamic3DTextLabelText(TekstyPomniki[ea_racedist][i], 0xFF5219FF, buffer);
	}
	mysql_free_result();
 }

 if (force || random(5)==1) {	// bardzo czasochlonne
	// solo killers
	mysql_query("select p.nick,count(sm.id) pojedynkow,round(avg(fightlen)) sredniczas from fs_solo_matches sm join fs_players p ON p.id=sm.killerid GROUP by sm.killerid order by pojedynkow desc LIMIT 3;");
	mysql_store_result();
	i=0;
//	UpdateDynamic3DTextLabelText(TekstyPomniki[ea_soloklrs][0], 0xFF5219FF, "Mistrzowie /solo\nNajwiecej wygranych solowek.");
	if (mysql_num_rows())
	while (mysql_fetch_row(buffer,"|")) {
		i++;
		if(sscanf(buffer,"p<|>s[32]dd", pnick, pval, pval2))
				format(buffer,sizeof buffer, "-");
		else
			format(buffer,sizeof buffer, "%d. %s\n{ffffff}%d %s\n{d0d0d0}Sredni czas walki: %dm%ds.", i, pnick, pval, dli(pval,"wygrany pojedynek","wygrane pojedynki","wygranych pojedynkow"),floatround(pval2/60,floatround_floor),pval2%60);
		UpdateDynamic3DTextLabelText(TekstyPomniki[ea_soloklrs][i], 0xFF5219FF, buffer);
	}
	mysql_free_result();

 }

 new longbuf[1023];
  // posterunek policji
  // select nick,hitman_prize  from fs_players WHERE timediff(NOW(),datetime_last)<'00:30:00' and hitman_prize>0 order by hitman_prize desc limit 5;
 mysql_query("select nick,hitman_prize  from fs_players WHERE timediff(NOW(),datetime_last)<'01:00:00' and hitman_prize>0 order by hitman_prize desc limit 5;");
 mysql_store_result();
 i=0;
 format(longbuf, sizeof longbuf, "Poszukiwani przestepcy\r\n\r\n");
//	UpdateDynamic3DTextLabelText(TekstyPomniki[ea_soloklrs][0], 0xFF5219FF, "Mistrzowie /solo\nNajwiecej wygranych solowek.");
 if (mysql_num_rows())
  while (mysql_fetch_row(buffer,"|")) {
		i++;
		if(!sscanf(buffer,"p<|>s[32]d", pnick, pval))
			format(longbuf,sizeof longbuf, "%s\r\n{ffffff}%d. %s {60ff60}%d$", longbuf, i, pnick, pval);

 }
 mysql_free_result();
 for (i=0;i<3;i++)
 	UpdateDynamic3DTextLabelText(PPRanking[i], 0xFF0000FF, longbuf);

 if (force || random(3)==1) {	// najlepsi na ONEDE
	format(longbuf, sizeof longbuf, "Najlepsi na /ONEDE\r\n\r\n");
	mysql_query("select p.nick,pa.kills from fs_players_arenascore pa JOIN fs_players p on p.id=pa.id_player WHERE pa.id_arena=6 ORDER BY pa.kills DESC limit 5;");
	mysql_store_result();
	i=0;
	if (mysql_num_rows())
		while (mysql_fetch_row(buffer,"|")) {
		i++;
		if(!sscanf(buffer,"p<|>s[32]d", pnick, pval))
			format(longbuf,sizeof longbuf, "%s\r\n{ffffff}%d. %s {60ff60}%d %s", longbuf, i, pnick, pval, dli(pval,"frag","fragi","fragow"));
		}
	mysql_free_result();
	UpdateDynamic3DTextLabelText(ArenaRanking[ARENA_ONEDE][0], 0xFF0000FF, longbuf);
 }

 if (force || random(3)==1) {	// najlepsi na ONEDE2
	format(longbuf, sizeof longbuf, "Najlepsi na /ONEDE2\r\n\r\n");
	mysql_query("select p.nick,pa.kills from fs_players_arenascore pa JOIN fs_players p on p.id=pa.id_player WHERE pa.id_arena=7 ORDER BY pa.kills DESC limit 5;");
	mysql_store_result();
	i=0;
	if (mysql_num_rows())
		while (mysql_fetch_row(buffer,"|")) {
		i++;
		if(!sscanf(buffer,"p<|>s[32]d", pnick, pval))
			format(longbuf,sizeof longbuf, "%s\r\n{ffffff}%d. %s {60ff60}%d %s", longbuf, i, pnick, pval, dli(pval,"frag","fragi","fragow"));
		}
	mysql_free_result();
	UpdateDynamic3DTextLabelText(ArenaRanking[ARENA_ONEDE2][0], 0xFF0000FF, longbuf);
 }





}

bool:IsPlayerInNoDMArea(playerid){
	if (!IsPlayerConnected(playerid)) return false;
	new pstate=GetPlayerState(playerid);
	if (pstate==PLAYER_STATE_NONE || pstate==PLAYER_STATE_SPECTATING) return false;

	for(new i=0;i<sizeof StrefyNoDM;i++) 
		if (StrefyNoDM[e_StrefyNoDM:i]!=StrefyNoDM[ea_KSW] && IsPlayerInDynamicArea(playerid,StrefyNoDM[e_StrefyNoDM:i]))
			return true;
	return false;
}

bool:IsPlayerInFullDMArea(playerid){
	if (!IsPlayerConnected(playerid)) return false;
	for(new i=0;i<sizeof StrefyFullDM;i++) 
		if (IsPlayerInDynamicArea(playerid,StrefyFullDM[e_StrefyFullDM:i]))
			return true;
	return false;
}


obiekty_OPEnterDynamicArea(playerid,areaid){
	if (areaid==StrefyNoDM[ea_kosciolINT]) {
		Audio_PlayBGSound(playerid, AUDIOID_FXKOSCIOL);
	} else if (areaid==StrefyFullDM[ea_tunele]) {
		Audio_PlayBGSound(playerid, AUDIOID_FXTUNEL1);
	} else if (areaid==DAs[eDA_tuneleKrzyk]) {
		Audio_Play(playerid, AUDIOID_FXTUNEL2, false, false, true);	return 1;
	}

	for(new i=0;i<sizeof KillZones;i++)
		if (areaid==KillZones[i]) {
	    	new Float:P[6];
		    GetPlayerPos(playerid,P[0],P[1],P[2]);
			GetPlayerVelocity(playerid,P[3],P[4],P[5]);

		    CreateExplosion(P[0],P[1],P[2], 2, 35.0);
		    CreateExplosion(P[0]+random(2)-1,P[1]+random(2)-1,P[2]+random(2)-1, 2, 35.0);
		    CreateExplosion(P[0]+random(2)-1,P[1]+random(2)-1,P[2]+random(2)-1, 2, 35.0);

			CreateExplosion(P[0]+P[3],P[1]+P[4],P[2]+P[5], 2, 35.0);
			CreateExplosion(P[0]+(P[3]*2),P[1]+(P[4]*2),P[2]+(P[5]*2), 2, 35.0);
			SetPlayerHealth(playerid,0);

			return 1;
		}
	for(new i=0;i<sizeof StrefyNoDM;i++) 
		if (areaid==StrefyNoDM[e_StrefyNoDM:i])
		{
//			if(e_StrefyNoDM:i==ea_hacjenda)
//				Audio_PlayBGSound(playerid,AUDIOID_FXHACJENDA,false,false,true);
			if (e_StrefyNoDM:i==ea_discoLV) {
				new str=Audio_PlayBGStream(playerid,"http://www.polskastacja.pl/play/aac_hot100.pls",false,true,true);
				Audio_Set3DPosition(playerid, str, 2404.4, 1695.5, 557.71, 150.0);
			}/* else if (e_StrefyNoDM:i==ea_discoCHILIAD) {
				new str=Audio_PlayBGStream(playerid,"http://www.polskastacja.pl/play/aac_drumbass.pls",false,true,true);
				Audio_Set3DPosition(playerid, str,-2371.9,-1721.3, 844.5, 75.0);
			}*/

//			Msg(playerid,COLOR_INFO2,"Wkraczasz do wolnej strefy, obowiazuje zakaz zabijania");

			TextDrawHideForPlayer(playerid,gTextDraw[TD_AREAFULLDM]);
			TextDrawShowForPlayer(playerid,gTextDraw[TD_AREANODM]);
			SetPlayerArmedWeapon(playerid,0);	// piesci
			pTemp[playerid][wStrefieNODM]=true;
			return 1;
		}
	for(new i=0;i<sizeof StrefyFullDM;i++)
		if (areaid==StrefyFullDM[e_StrefyFullDM:i])
		{
			new pstate=GetPlayerState(playerid);			
			if (pstate==PLAYER_STATE_WASTED || pstate==PLAYER_STATE_SPECTATING || pstate==PLAYER_STATE_NONE) return 1;

			if(e_StrefyFullDM:i==ea_wojsko)												// w petli zle brzmia
				Audio_PlayBGSound(playerid,AUDIOID_FXAFGANISTAN,false,false,true);
			else if(e_StrefyFullDM:i==ea_wietnam)
				Audio_PlayBGSound(playerid,AUDIOID_FXWIETNAM,false,false,true);
			else if(e_StrefyFullDM:i==ea_kopalnia)
				pTemp[playerid][onArena]=ARENA_RPG;
			else if(e_StrefyFullDM:i==ea_minigun)
				pTemp[playerid][onArena]=ARENA_MINIGUN;
			pTemp[playerid][wStrefieFULLDM]=true;
			TextDrawHideForPlayer(playerid,gTextDraw[TD_AREANODM]);
			TextDrawShowForPlayer(playerid,gTextDraw[TD_AREAFULLDM]);
			return 1;
		}
	for (new i=0;i<sizeof Fotoradary;i++)
		if (Fotoradary[i]==areaid) 
			return obiekty_Fotokomorka(playerid);
	if (areaid==DAs[eDA_getto]) {
		MoveDynamicObject(Os[eO_ls_getto_brama],2423.7770996094, -1790.4482421875, 16.460855484009,5);
		return 1;
	}



	return 0;
}

obiekty_OPLeaveDynamicArea(playerid,areaid){
	for(new i=0;i<sizeof StrefyNoDM;i++) 
		if (areaid==StrefyNoDM[e_StrefyNoDM:i]) {
			Audio_StopBGSound(playerid);
			if (IsPlayerInNoDMArea(playerid)) return 1;
			pTemp[playerid][wStrefieNODM]=false;
			return TextDrawHideForPlayer(playerid,gTextDraw[TD_AREANODM]);
		}

	for(new i=0;i<sizeof StrefyFullDM;i++)
		if (areaid==StrefyFullDM[e_StrefyFullDM:i]) {
			Audio_StopBGSound(playerid);

			if	((pTemp[playerid][onArena]==ARENA_RPG && e_StrefyFullDM:i==ea_kopalnia) ||
				 (pTemp[playerid][onArena]==ARENA_MINIGUN && e_StrefyFullDM:i==ea_minigun))
				pTemp[playerid][onArena]=ARENA_NONE;
			new pstate=GetPlayerState(playerid);

			if (pstate==PLAYER_STATE_WASTED || pstate==PLAYER_STATE_SPECTATING || pstate==PLAYER_STATE_NONE) return 1;
			if (IsPlayerInFullDMArea(playerid)) return 1;
			else if (pData[playerid][pAttraction]==A_NONE) RemoveDangerousWeapon(playerid);
			pTemp[playerid][wStrefieFULLDM]=false;
			return TextDrawHideForPlayer(playerid,gTextDraw[TD_AREAFULLDM]);
		}

	if (areaid==DAs[eDA_andromeda]) {
		new count = GetTickCount()/3000;
		new x= (count % 3500);
		new y= (count/x)%3500;
		x=x-1850+random(150);
		y=y-1850+random(150);
//		TogglePlayerControllable(playerid,false);
		SetPlayerInterior(playerid,0);
		SetPlayerPos(playerid,x,y,1500.0);
//		GivePlayerWeapon(playerid,46,1);
//		TogglePlayerControllable(playerid,true);
		return 1;
	}
	else if (areaid==DAs[eDA_getto]) {
		MoveDynamicObject(Os[eO_ls_getto_brama],2423.7770996094, -1774.4482421875, 16.460855484009,5);
		return 1;
	}
//	} else if (areaid==DAs[eDA_jail] && pData[playerid][jail]>0 && GetPlayerState(playerid)>0 && random(2)==1) {
//		Msg(playerid, COLOR_INFO2, TXT(playerid, 223));
//		MoveToJail(playerid);
//	}

	return 0;
}

obiekty_Fotokomorka(playerid){
	if (!IsPlayerInAnyVehicle(playerid)) return 1;
	new vid=GetPlayerVehicleID(playerid);
	new spd=GetVehicleSpeed(vid);
	new Float:VV[3];
	GetVehicleVelocity(vid,VV[0],VV[1],VV[2]);
	if (spd>110 && floatabs(VV[2])<(floatabs(VV[0])+floatabs(VV[1]))) {
			new buf[128];
			FlashScreen(playerid);
			format(buf,sizeof buf,"_~n~_~n~_~n~_~n~_~n~~r~%d KM/h~n~~w~Jedziesz zbyt szybko!", spd);
			GameTextForPlayer(playerid, buf, 4000, 6);
			if (Audio_IsClientConnected(playerid))
				Audio_Play(playerid,50,false,false,true);

			if (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return 1;
			if (pTemp[playerid][faction]==FACTION_POLICE || pTemp[playerid][faction]==FACTION_ARMY) return 1;	// policjanci i wojskowi nie dostaja mandatow

			format(buf,sizeof buf,"[FOTORADAR] {ff5050}Przekroczyles dopuszczalna predkosc! {50ff50}Mandat: {ffffff}%d$", floatround(spd/4));
			SendClientMessage(playerid,-1,buf);
			avt_record(playerid,e_fotocash,floatround(spd/4),ART_ADD);
			IncreasePlayerHitman(playerid, floatround(spd/4));
			

	}
	return 1;
}
