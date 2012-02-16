
#define MYJNIA_KOSZT	4000
#define DRIFTUPGRADE_KOSZT	10000

forward warsztat_streambackvehicle(vid, Float:vP0, Float:vP1, Float: vP2, Float: vP3);
warsztat_zmianaTablicy(playerid, tablica[]=""){
	
	new vid=GetNearestVehicleFromXYZ(1042.68,1782.55,11.52,5,true);
	if (vid==INVALID_VEHICLE_ID)
		return Msg(playerid,COLOR_ERROR,"Najpierw zaparkuj samochod w garazu.");
	if (strlen(tablica)==0)
		return ShowPlayerDialog(playerid,DIALOG_WARSZTAT_TABLICE, DIALOG_STYLE_INPUT, "Zmiana tablicy", "Wprowadz tresc nowe tablicy rejestracyjnej", "OK", "Anuluj");
	else if (!IsNickCorrect(tablica))
		return ShowPlayerDialog(playerid,DIALOG_WARSZTAT_TABLICE, DIALOG_STYLE_INPUT, "Zmiana tablicy", "Wprowadz tresc nowe tablicy rejestracyjnej.\n{ff3030}Uzyles niedozwolonych znakow. Sprobuj ponownie.", "OK", "Anuluj");
	
	SetVehicleNumberPlate(vid,tablica);
	tVehicles[vid][vo_licensePlateSet]=true;
	
	Msg(playerid,COLOR_INFO,"Tablica ustawiona");

	new Float:vP[4];
	GetVehiclePos(vid,vP[0],vP[1],vP[2]);
	LinkVehicleToInterior(vid,17);
	SetVehicleVirtualWorld(vid,17);

	SetTimerEx("warsztat_streambackvehicle",  1000, false, "dffff", vid, vP[0],vP[1],vP[2], vP[3]);
	
	return 1;
}


public warsztat_streambackvehicle(vid,Float:vP0, Float:vP1, Float: vP2, Float: vP3){
	SetVehiclePos(vid,vP0,vP1,vP2);
	LinkVehicleToInterior(vid,0);
	SetVehicleVirtualWorld(vid,0);
}

warsztat_DialogResponse(playerid, dialogid, response, listitem, inputtext[]){
#pragma unused listitem
	if (dialogid==DIALOG_WARSZTAT_TABLICE) {
		if (!response) return 1;
		warsztat_zmianaTablicy(playerid, inputtext);
		return 1;
	}
	return 0;
}

warsztat_CPmyjnia(playerid){
	GameTextForPlayer(playerid,"~y~Myjnia",500,0);
	if (!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid,COLOR_ERROR,"Musisz tu przyjechac pojazdem :-).");
	if (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER)
		return 1;

	new vid,Float:VZ;
	vid=GetPlayerVehicleID(playerid);
	GetVehicleZAngle(vid,VZ);
	if (VZ<269 && VZ>90)
		return Msg(playerid,COLOR_ERROR,"Wjechales ze zlej strony!");

	if (GetPlayerMoney(playerid)<MYJNIA_KOSZT)
		return Msg(playerid,COLOR_ERROR,"Nie stac Cie na myjnie!");

	cmd_lock(playerid);
	GivePlayerMoney(playerid,-MYJNIA_KOSZT);

	RepairVehicle(vid);
	SetVehicleHealth(vid,VEHICLE_DEFAULT_HP);

	return 1;
}

warsztat_CPskup(playerid){
	GameTextForPlayer(playerid,"~y~Skup pojazdow",2000,3);
	if (!IsPlayerInAnyVehicle(playerid)){
		GameTextForPlayer(playerid,"Mozesz tu przyholowywac pojazdy do skupu/utylizacji", 2000,5);
		return 1;
	}
	new vid=GetPlayerVehicleID(playerid);
	if (GetVehicleModel(vid)!=525)
		return GameTextForPlayer(playerid,"Pojazdy musza byc przyholowane!", 2000,5);

	if (!IsTrailerAttachedToVehicle(vid))
		return GameTextForPlayer(playerid,"Przyholuj pojazd do skupu", 2000,5);
	new tvid=GetVehicleTrailer(vid);
	DetachTrailerFromVehicle(vid);
	new buf[128],cash;
//	new panels,doors,lights,tires;
//	GetVehicleDamageStatus(tvid,panels,doors,lights,tires);
//	format(buf, sizeof buf, "p %d %d %d %d", panels, doors, lights, tires);
//	printf(buf);
	RespawnVehicle(tvid);
	cash=4000+random(9000);

	format(buf,sizeof buf,"Pojazd przyjety do skupu~n~~g~~h~+~g~%d~h~$", cash);
	GameTextForPlayer(playerid,buf,2000,5);
	GivePlayerMoney(playerid,cash);
	PlayRandomMusic(playerid,6000);
	
	return 1;
}

warsztat_CPdrift(playerid){

	if (!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid,COLOR_ERROR,"Musisz tu przyjechac pojazdem :-).");
	if (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER)
		return 1;

	new vid;
	vid=GetPlayerVehicleID(playerid);

	switch(GetVehicleModel(vid)){
		case 562,402,451,560,602,475: { }
		default: { 
			GameTextForPlayer(playerid,"~r~Tego auta nie mozna zupgradowac",500,0);
			return 1;
			}
			// elegy, buffalo, turismo, sultan, alpha, sabre, 
	}

	GameTextForPlayer(playerid,"~y~Upgrade drifterski",500,0);

	if (GetPlayerMoney(playerid)<DRIFTUPGRADE_KOSZT)
		return Msg(playerid,COLOR_ERROR,"Nie stac Cie na myjnie!");

	GivePlayerMoney(playerid,-DRIFTUPGRADE_KOSZT);

	TuneCar(vid);
	PlaySound(playerid, 1134);
	tVehicles[vid][vo_drift]=true;
	Msg(playerid,COLOR_INFO,"Zupgradowales auto do driftu");
	Msg(playerid,COLOR_INFO,"Mozesz je szybko naprawiac/odwracac wciskajac 2",false);
	Msg(playerid,COLOR_INFO2,"Zabronione jest uzywanie auta drifterskiego do DT/DB!",false);
	SetPlayerArmedWeapon(playerid,0);

	return 1;
}
