CMD:zaloz(playerid,params[]){

	if(pData[playerid][respect]>=15000) {
		if (!strcmp(params,"koparka",true))
			return SetPlayerAttachedObject( playerid, 0, 18248, 2, 0.315063, 0.321018, -0.221677, 258.909912, 96.667274, 277.171569, 0.050388, 0.050388, 0.050388 ); // cuntwjunk01 - koparka od 10000 respa
		else if (!strcmp(params,"lom",true))
			return SetPlayerAttachedObject( playerid, 0, 18634, 5, 0.106173, 0.030468, -0.015657, 258.909912, 269.149536, 14.354298, 1.163060, 1.163060, 1.163060 ); // GTASACrowbar1 - ³om od 15k respa
		else if (!strcmp(params,"wigwam",true))
			return SetPlayerAttachedObject( playerid, 0, 3243, 1, -1.057782, -0.016429, -0.009905, 0.000000, 85.779541, 354.995361, 0.308334, 0.308334, 0.308334 ); // tepee_room_ - wigwam 15000 respa
	}
	if(pData[playerid][respect]>=5000) {
		if (!strcmp(params,"boombox",true))
			return SetPlayerAttachedObject( playerid, 0, 2226, 1, 0.000000, -0.203173, -0.080502, 0.000000, 22.671218, 0.000000, 1.000000, 1.000000, 1.000000 ); // LOW_HI_FI_3 - magnetofon 5000 respa
		else if (!strcmp(params,"mikolaj",true))
			return SetPlayerAttachedObject( playerid, 0, 19065, 2, 0.113131, 0.018596, 0.000000, 94.697052, 90.118164, 0.000000, 1.226595, 1.226595, 1.226595 ); // SantaHat2 - czapkamikolaja 5000 respa
		else if (!strcmp(params,"mlotek",true))
			return SetPlayerAttachedObject( playerid, 0, 18635, 2, -0.250711, -0.464394, 0.159861, 258.909912, 45.141162, 5.301577, 1.920960, 1.920960, 1.920960 ); // GTASAHammer1 - mlotek od 2500 respa
		else if (!strcmp(params,"srubokret",true))
			return SetPlayerAttachedObject( playerid, 0, 18644, 2, 0.151123, 0.345173, 0.019663, 275.692535, 10.100483, 357.343811, 1.924180, 1.924180, 1.924180 ); // Screwdriver1 - srubokret od 2500 respa
	}
	if(pData[playerid][respect]>=2500) {
		if (!strcmp(params,"spoiler",true))
			return SetPlayerAttachedObject( playerid, 0, 1060, 1, 0.239907, -0.077574, 0.000871, 2.348074, 87.804405, 357.280487, 0.652846, 0.652846, 0.652846 ); // spl_c_st_r - spojler2 2500 respa
	}
	if(pData[playerid][respect]>=500) {
		if(!strcmp(params,"afro",true))
			return SetPlayerAttachedObject( playerid, 0, 18975, 2, 0.086262, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000, 1.000000, 1.000000 ); // Hair2 - peruka od 500 respa

		else if (!strcmp(params,"grabie",true))
			return SetPlayerAttachedObject( playerid, 0, 18890, 5, 0.037106, 0.096538, 0.019960, 337.156921, 4.156515, 252.433853, 1.000000, 1.000000, 1.000000 ); // Rake1 - grabie od 500 respa
		else if (!strcmp(params,"kurczak",true))
			return SetPlayerAttachedObject( playerid, 0, 19137, 2, 0.092990, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.076031, 1.076031, 1.076031 ); // CluckinBellHat1 - kurczak 1000 respa

		else if (!strcmp(params,"telefon",true))
			return SetPlayerAttachedObject( playerid, 0, 18873, 1, -0.012269, 0.021618, 0.112074, 255.530456, 177.323989, 63.659469, 4.356958, 4.356958, 4.356958 ); // MobilePhone9 - telefon id 0 respa 500

	}
	if (!strcmp(params,"cygaro",true))
		return SetPlayerAttachedObject( playerid, 0, 1485, 2, 0.102237, 0.094420, -0.038719, 61.115173, 0.000000, 140.122970 ); // CJ_CIGGY - cygaro
	else if (!strcmp(params,"kapelusz",true))
		return SetPlayerAttachedObject( playerid, 0, 18962, 2, 0.146981, 0.013410, 0.000000, 0.000000, 0.000000, 0.000000, 1.223083, 1.223083, 1.223083 ); // CowboyHat2 - kapelusz
	else if (!strcmp(params,"katana",true))
		return SetPlayerAttachedObject( playerid, 0, 339, 2, 0.178890, 0.004188, 0.130645, 346.583984, 188.009109, 12.984556 ); 
	else if (!strcmp(params,"pacholek",true))
		return SetPlayerAttachedObject( playerid, 0, 1238, 2, 0.435544, -0.001684, 0.000000, 0.000000, 90.000000, 285.874969 );
	else if (!strcmp(params,"rycerz",true))
		return SetPlayerAttachedObject( playerid, 0, 1549, 1, -0.173378, 0.021022, -0.012099, 3.122609, 87.657714, 355.966491, 1.000000, 1.000000, 1.000000 ); // CJ_ASHTRAY_b - Smietnik
	else if (!strcmp(params,"siano",true))
		return SetPlayerAttachedObject( playerid, 0, 1453, 1, -0.202300, 0.010707, -0.029694, 0.737283, 267.933135, 0.000000, 1.000000, 1.000000, 1.000000 ); // DYN_H_BALE1 - Siano
	else if (!strcmp(params,"swietlik",true))
		return SetPlayerAttachedObject( playerid, 0, 3785, 1, 0.073756, -0.125172, 0.002187, 30.842874, 88.859939, 234.537429, 1.000000, 1.000000, 1.000000 ); // bulkheadlight - 9
	else if (!strcmp(params,"telefon",true))
		return SetPlayerAttachedObject( playerid, 0, 18873, 1, -0.012269, 0.021618, 0.112074, 255.530456, 177.323989, 63.659469, 4.356958, 4.356958, 4.356958 ); // MobilePhone9 - telefon id 0 respa 500
	else if (!strcmp(params,"torba",true))
		return SetPlayerAttachedObject( playerid, 0, 2663, 2, 0.233547, 0.028803, 0.017788, 271.072326, 88.000000, 0.000000 );	    
	else if (!strcmp(params,"zolwik",true))
		return SetPlayerAttachedObject( playerid, 0, 1609, 1, 0.041243, -0.101723, 0.016250, 89.706207, 349.184875, 272.334106, 0.190168, 0.190168, 0.190168 ); // turtle - plecaczek


	SendClientMessage(playerid,-1,"Uzyj: /zaloz <nazwa>");
	SendClientMessage(playerid,-1,"-> cygaro, kapelusz, katana, pacholek, rycerz, siano, swietlik, telefon, torba, zolwik");
	SendClientMessage(playerid,0xe0e0e0ff,"Od 500 respektu: {ffffff}afro, grabie, kurczak, telefon");
	SendClientMessage(playerid,0xd0e0d0ff,"Od 2.5k respektu: {ffffff}spoiler");
	SendClientMessage(playerid,0xd0e0d0ff,"Od 5k respektu: {ffffff}boombox, mikolaj, mlotek, srubokret");
	SendClientMessage(playerid,0xc0e0c0ff,"Od 15k respektu: {ffffff}koparka, lom, wigwam");
	
	return 1;
}
