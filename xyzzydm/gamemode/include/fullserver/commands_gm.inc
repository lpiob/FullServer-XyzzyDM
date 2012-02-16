CMD:ann100(playerid,params[]){
	if (!IsGM(playerid)) return 0;

	if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uzywaj: {b}/ann100 <tekst>{/b}"); // U¿ywaj: /ann <tekst>
	if(!CheckTildes(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 205)); // Iloœæ tyld "~" musi byæ parzysta.

	new
	 pVector1[e_Vectors],
	 pVector2[e_Vectors],
	 buffer[160],
	 pCount = 0,
	 int, vw;
	
	GetPlayerPosition(playerid, pVector1[X], pVector1[Y], pVector1[Z]);
	int=GetPlayerInterior(playerid);
	vw=GetPlayerVirtualWorld(playerid);

	foreach(i)
	if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
	{
		GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
		if(GetDistanceBetweenPoints3D(pVector1[X], pVector1[Y], pVector1[Z], pVector2[X], pVector2[Y], pVector2[Z]) <= 100.0)
		{
			GameTextForPlayer(i,params,11000,5);
			pCount++;
		}
	}
	
	if (pCount) {
		format(buffer, sizeof buffer, "Komunikat wyslany do {b}%d %s{/b}.", pCount, dli(pCount,"gracza","graczy","graczy"));
		Msg(playerid, COLOR_INFO, buffer);
	} else 
		Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");
	
	
	return 1;

}

CMD:da50(playerid) {
	Msg(playerid,COLOR_INFO2,"{b}/da50{/b} jest przestarzale i wkrotce bedzie wycofane. Uzywaj {b}/da <0-100>{/b}.");
	return cmd_da(playerid,"50");
}

CMD:da(playerid, params[])
{
    if (!IsGM(playerid)) return 0;
	MUST_BE_SPAWNED(playerid);

	
	new
	 zasieg,
	 pVector1[e_Vectors],
	 pVector2[e_Vectors],
	 int,vw,buf[128],pcount;

	sscanf(params,"D(50)", zasieg);
	if (zasieg>100 && pData[playerid][adminLevel]==LEVEL_GM)
		return Msg(playerid,COLOR_ERROR,"Dopuszczalny zasieg: {b}0-100{/b}");
	
	int=GetPlayerInterior(playerid);
	vw=GetPlayerVirtualWorld(playerid);
//	if (vw>0) return Msg(playerid,COLOR_ERROR,"{b}/da{/b} mozesz uzyc tylko w VW 0");

	GetPlayerPosition(playerid, pVector1[X], pVector1[Y], pVector1[Z]);

	switch(pData[playerid][adminLevel]){
		case LEVEL_GM:
			format(buf, sizeof buf, "Moderator%s {b}%s{/b} rozbroil%s wszystkich w promieniu {b}%d %s{/b}.", 
				Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid),	Kobieta(playerid)?("a"):(""), zasieg, dli(zasieg,"metra","metrow","metrow"));
		case LEVEL_ADMINHIDDEN:
			if (random(2)==1)
				format(buf, sizeof buf, "Nagle zauwazasz ze nie masz zadnej broni.");
			else
				format(buf, sizeof buf, "Niespodziewanie Twoje bronie znikaja.");
		default:
			format(buf, sizeof buf, "Administator%s {b}%s{/b} rozbroil%s wszystkich w promieniu {b}%d %s{/b}.", 
				Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid),	Kobieta(playerid)?("a"):(""), zasieg, dli(zasieg,"metra","metrow","metrow"));

	}

	foreach(i)
	if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
	{
		GetPlayerPos(i, pVector2[X], pVector2[Y], pVector2[Z]);
		if(playerid != i && GetDistanceBetweenPoints3D(pVector1[X], pVector1[Y], pVector1[Z], pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
		{		
			ResetPlayerWeapons(i);
			Msg(i,COLOR_INFO,buf,false);			
			pcount++;
		}
	}
	format(buf,sizeof buf,"Rozbrojono {b}%d{/b} %s", pcount, dli(pcount, "osob", "osoby", "osob"));
	Msg(playerid,COLOR_INFO,buf,false);

	return 1;
}

CMD:gmlogin(playerid){
	new buf[128];
	if (pData[playerid][adminLevel]>=LEVEL_GM)
		return SetPlayerDrunkLevel(playerid,5000);

	if (pData[playerid][allowedLevel]<LEVEL_GM) {
		TextDrawShowForPlayer(playerid, gTextDraw[TD_WARNING]);
		SendClientMessage(playerid,0xff0000ff,"-------------------------------------------------");
		SendClientMessage(playerid,0xff0000ff,"Uwaga! {ffffff}Wszystkie proby nieautoryzowanego dostepu");
		SendClientMessage(playerid,0xffffffff,"sa rejestrowane i beda scigane z pelna surowoscia.");
		SendClientMessage(playerid,0xffffffff,"Powtarzajace sie przypadki sa zglaszane bezposrednio");
		SendClientMessage(playerid,0xffffffff,"na policje. {ff0000}Zastanow sie trzy razy, zanim zaczniesz.");
		SendClientMessage(playerid,0xff0000ff,"-------------------------------------------------");	

		PlaySound(playerid,1053);
		SetTimerEx("HidePlayerWarning",10000,false,"d",playerid);
		return 1;
	}

//	if (strlen(params)<6 || isnull(params))
//		return Msg(playerid,COLOR_INFO,"/gmlogin");

	if (pData[playerid][allowedLevel]==LEVEL_GM) {
		pData[playerid][adminLevel]=pData[playerid][allowedLevel];
		SetPlayerProperColor(playerid);

		new
		 szPlayerName[24];
		
		GetPlayerName(playerid, szPlayerName, sizeof szPlayerName);
		
		foreach(i)
		{
			format(buf, sizeof buf, TXT(i, 241), szPlayerName);
			Msg(i, COLOR_INFO2, buf, false); // "xxx" zalogowa³ siê na admina/moderatora (poziom xxx).
//			PlaySound(i, 1133);
			PlayerPlaySound(i,1133, 10,10,10);	
		}
	}
	UpdatePlayerNickTD(playerid);
	return 1;
}

CMD:heal(playerid,params[]){
    if (!IsGM(playerid)) return 0;
    new userid=INVALID_PLAYER_ID;
    if (sscanf(params,"d",userid))
		if (pData[playerid][spectating]!=INVALID_PLAYER_ID)
	 		userid=pData[playerid][spectating];

	if (userid==INVALID_PLAYER_ID)
	return Msg(playerid,COLOR_ERROR,"/heal <id gracza>");

    if (!IsPlayerConnected(userid)) 
	return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
    SetPlayerHealth(userid,100.0);
	if (pData[playerid][adminLevel]==LEVEL_GM)
		format(gstr, sizeof gstr, "Zostal%s uleczon%s przez moderator%s {b}%s{/b}.", Kobieta(userid)?("as"):("es"), Kobieta(userid)?("a"):("y"), Kobieta(playerid)?("ke"):("a"), GetPlayerProperName(playerid));
	else if (pData[playerid][adminLevel]<LEVEL_ADMINHIDDEN)
		format(gstr, sizeof gstr, "Zostal%s uleczon%s przez administrator%s {b}%s{/b}.", Kobieta(userid)?("as"):("es"), Kobieta(userid)?("a"):("y"), Kobieta(playerid)?("ke"):("a"), GetPlayerProperName(playerid));
	Msg(userid, COLOR_INFO, gstr);
    // todo powiadomienie dla gracza
    return 1;
}

CMD:armor(playerid,params[]){
    if (!IsGM(playerid)) return 0;
    new userid,Float:ar=100;
    if (sscanf(params,"df(100)",userid,ar)) {
	if (pData[playerid][spectating]!=INVALID_PLAYER_ID) {
	    userid=pData[playerid][spectating];
	    ar=100;
	} else {
	    userid=playerid;
	    ar=100;
	}
    }
    if (!IsPlayerConnected(userid)) 
	return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
    SetPlayerArmour(userid,ar);
    // todo powiadomienie

    return 1;
}

CMD:vehgod(playerid,params[]){
    if (!IsGM(playerid)) return 0;
    new userid,vehicleid;
    if (sscanf(params,"d",userid)) {
	if (IsPlayerInAnyVehicle(playerid))
	    userid=playerid;
	else if (pData[playerid][spectating]!=INVALID_PLAYER_ID && IsPlayerInAnyVehicle(pData[playerid][spectating]))
	    userid=pData[playerid][spectating];
    } else if (!IsPlayerInAnyVehicle(userid))
	    return Msg(playerid,0xffffffff,"Gracz nie jest w pojezdzie.");

    if (!IsPlayerConnected(userid)) 
	return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");

    vehicleid=GetPlayerVehicleID(userid);
    SetVehicleHealth(vehicleid, 1000000);
    // todo powiadomienie
    return 1;
}

CMD:gun50(playerid,params[]){
	if (!IsGM(playerid)) return 0;
//	new wepid,ammo;
//	printf("par %s",params);
//	if (sscanf(params,"d",wepid))
	ShowWeaponSelectMenu(playerid, "Daj bron wszystkim w promieniu 50m", "GM_gun50CB");
	return 1;

}

forward GM_gun50CB(playerid,wepid);
public GM_gun50CB(playerid,wepid){
	new ammo;
	for (new i=0; i<sizeof quickbuyWeapons;i++)
		if (quickbuyWeapons[i][ewd_id]==wepid) {	ammo=quickbuyWeapons[i][ewd_baseAmmo]; break; }
	if (ammo==0)
		return Msg(playerid, COLOR_ERROR,"Niewlasciwy wybor");

	new
	 pVector1[e_Vectors],
	 pVector2[e_Vectors],
	 buffer[160],
	 pCount = 0,
	 int, vw;
	
	GetPlayerPosition(playerid, pVector1[X], pVector1[Y], pVector1[Z]);
	int=GetPlayerInterior(playerid);
	vw=GetPlayerVirtualWorld(playerid);

//	format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} uleczyl%s Cie.", Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""));
	
	foreach(i)
	if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
	{
		GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
		if(playerid != i && GetDistanceBetweenPoints3D(pVector1[X], pVector1[Y], pVector1[Z], pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
		{		
			GivePlayerWeapon(i,wepid,ammo);
			pCount++;
		}
	}
	
	if (pCount) {
		format(buffer, sizeof buffer, "Rozdano bron {b}%d %s{/b}.", pCount, dli(pCount,"graczu","graczom","graczom"));
		Msg(playerid, COLOR_INFO, buffer);
	} else 
		Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");

	return 1;

}

CMD:heal50(playerid,params[]){
	if (!IsGM(playerid)) return 0;

	new
	 pVector1[e_Vectors],
	 pVector2[e_Vectors],
	 buffer[160],
	 pCount = 0,
	 int, vw;
	
	GetPlayerPosition(playerid, pVector1[X], pVector1[Y], pVector1[Z]);
	int=GetPlayerInterior(playerid);
	vw=GetPlayerVirtualWorld(playerid);

	format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} uleczyl%s Cie.", Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""));
	
	foreach(i)
	if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
	{
		GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
		if(playerid != i && GetDistanceBetweenPoints3D(pVector1[X], pVector1[Y], pVector1[Z], pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
		{		
			SetPlayerHealth(i,100.0);
			Msg(i, COLOR_INFO, buffer, true);
			pCount++;
		}
	}
	
	if (pCount) {
		format(buffer, sizeof buffer, "Uleczyles {b}%d %s{/b}.", pCount, dli(pCount,"gracza","graczy","graczy"));
		Msg(playerid, COLOR_INFO, buffer);
	} else 
		Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");

	
	
	return 1;
}


CMD:armor50(playerid,params[]){
	if (!IsGM(playerid)) return 0;

	new
	 pVector1[e_Vectors],
	 pVector2[e_Vectors],
	 buffer[160],
	 pCount = 0,
	 int, vw;
	
	GetPlayerPosition(playerid, pVector1[X], pVector1[Y], pVector1[Z]);
	int=GetPlayerInterior(playerid);
	vw=GetPlayerVirtualWorld(playerid);

	format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} uzupelnil%s Ci armora.", Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""));
	
	foreach(i)
	if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
	{
		GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
		if(playerid != i && GetDistanceBetweenPoints3D(pVector1[X], pVector1[Y], pVector1[Z], pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
		{		
			SetPlayerArmour(i,100.0);
			Msg(i, COLOR_INFO, buffer, true);
//			SendClientMessage(i, gmData[color_chatIC]*256, buffer);
			pCount++;
		}
	}
	
	if (pCount) {
		format(buffer, sizeof buffer, "Dales/as armora {b}%d %s{/b}.", pCount, dli(pCount,"graczowi","graczom","graczom"));
		Msg(playerid, COLOR_INFO, buffer);
	} else 
		Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");

	
	
	return 1;
}

CMD:unarmor50(playerid,params[]){
	if (!IsGM(playerid)) return 0;

	new
	 pVector1[e_Vectors],
	 pVector2[e_Vectors],
	 buffer[160],
	 pCount = 0,
	 int, vw;
	
	GetPlayerPosition(playerid, pVector1[X], pVector1[Y], pVector1[Z]);
	int=GetPlayerInterior(playerid);
	vw=GetPlayerVirtualWorld(playerid);

	format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} zabral%s Ci armor.", Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""));
	
	foreach(i)
	if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
	{
		GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
		if(playerid != i && GetDistanceBetweenPoints3D(pVector1[X], pVector1[Y], pVector1[Z], pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
		{		
			SetPlayerArmour(i,0.0);
			Msg(i, COLOR_INFO, buffer, true);
//			SendClientMessage(i, gmData[color_chatIC]*256, buffer);
			pCount++;
		}
	}
	
	if (pCount) {
		format(buffer, sizeof buffer, "Odebrano armora {b}%d %s{/b}.", pCount, dli(pCount,"graczowi","graczom","graczom"));
		Msg(playerid, COLOR_INFO, buffer);
	} else 
		Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");

	
	
	return 1;
}





CMD:freeze50(playerid,params[]){
	if (!IsGM(playerid)) return 0;

	new
	 pVector1[e_Vectors],
	 pVector2[e_Vectors],
	 buffer[160],
	 pCount = 0,
	 int, vw;
	
	GetPlayerPosition(playerid, pVector1[X], pVector1[Y], pVector1[Z]);
	int=GetPlayerInterior(playerid);
	vw=GetPlayerVirtualWorld(playerid);
	
	format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} zamrozil%s Cie.", Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""));

	foreach(i)
	if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
	{
		GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
		if(playerid != i && GetDistanceBetweenPoints3D(pVector1[X], pVector1[Y], pVector1[Z], pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
		{		
//			SendClientMessage(i, gmData[color_chatIC]*256, buffer);
			Msg(i, COLOR_INFO, buffer, true);
			TogglePlayerControllable(i,0);
			pCount++;
		}
	}
	
	if (pCount) {
		format(buffer, sizeof buffer, "Zamroziles/as {b}%d %s{/b}.", pCount, dli(pCount,"gracza","graczy","graczy"));
		Msg(playerid, COLOR_INFO, buffer);
	} else 
		Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");

	
	
	return 1;
}

CMD:unfreeze50(playerid,params[]){
	if (!IsGM(playerid)) return 0;

	new
	 pVector1[e_Vectors],
	 pVector2[e_Vectors],
	 buffer[160],
	 pCount = 0,
	 int, vw;
	
	GetPlayerPosition(playerid, pVector1[X], pVector1[Y], pVector1[Z]);
	int=GetPlayerInterior(playerid);
	vw=GetPlayerVirtualWorld(playerid);
	
	format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} odmrozil%s Cie.", Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""));

	foreach(i)
	if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
	{
		GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
		if(playerid != i && GetDistanceBetweenPoints3D(pVector1[X], pVector1[Y], pVector1[Z], pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
		{		
//			SendClientMessage(i, gmData[color_chatIC]*256, buffer);
			Msg(i, COLOR_INFO, buffer, true);
			TogglePlayerControllable(i,1);
			pCount++;
		}
	}
	
	if (pCount) {
		format(buffer, sizeof buffer, "Odmroziles/as {b}%d %s{/b}.", pCount, dli(pCount,"gracza","graczy","graczy"));
		Msg(playerid, COLOR_INFO, buffer);
	} else 
		Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");

	
	
	return 1;
}






CMD:delcar(playerid){
	if (!IsGM(playerid)) return 0;
	new vehicleid=GetPlayerVehicleID(playerid);
	if (vehicleid==0 || vehicleid==INVALID_VEHICLE_ID)
		vehicleid=GetNearestVehicleToPlayer(playerid,4);
	if (vehicleid==0 || vehicleid==INVALID_VEHICLE_ID) {
		Msg(playerid,COLOR_ERROR,"Wsiadz albo stan obok pojazdu do usuniecia");
		return 1;
	}

	RespawnVehicle(vehicleid);
	return 1;
}

CMD:tenwoz(playerid){
	if (!IsGM(playerid)) return 0;
//	if (IsPlayerAdmin(playerid) || IsFSAdmin(playerid)) {
	if (!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid,-1,"Musisz byc w pojezdzie");
	new vid=GetPlayerVehicleID(playerid);
	new engine,lights,alarm,doors,bonnet,boot,objective;
	GetVehicleParamsEx(vid,engine,lights,alarm,doors,bonnet,boot,objective);
	SetVehicleParamsEx(vid,1,lights,alarm,doors,bonnet,boot,(objective>0?0:1));
//	} else 
//		return 0;
	return 1;
}

CMD:me(playerid, params[])	//gracze nie potrafia z tego skorzystac :/
{
	if (!IsGM(playerid)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 144)); // U¿ywaj: /me <tekst>
	
	if(FilterText(playerid, params) == 0) return 1;
	if(!IsAdmin(playerid)) CensorText(params);
	if(gmData[chatColors]) FilterTextWithColors(params);
	
	new
	 buffer[160];
	
	format(buffer, sizeof buffer, "%s {%s}%s", GetPlayerProperName(playerid), IntToHex(gmData[color_chatME]), params);
	SendClientMessageToAll(gmData[color_chatME_HL] * 256, buffer);
	
	format(buffer, sizeof buffer, "[ME] %s %s", GetPlayerNick(playerid), params);
	OutputLog(LOG_OCHAT, buffer);

	return 1;
}


CMD:rspa(playerid,params[]){
    if (!IsGM(playerid)) return 0;
    RespawnVehiclesInRangeOf(playerid);
    return 1;
}
