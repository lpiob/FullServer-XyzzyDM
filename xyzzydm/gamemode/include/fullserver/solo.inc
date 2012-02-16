enum e_soloinvitation {
	bool:esi_sent,
	esi_targetplayerid,
	esi_arenaid,
	esi_weapon1,
	esi_weapon2,
	esi_when,
	esi_start
}

new soloinv[MAX_PLAYERS][e_soloinvitation];

CMD:solo(playerid,params[]){
	MUST_BE_SPAWNED(playerid);
	if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, "Nie mozesz teraz tego zrobic!");
	new targetid;
	if (isnull(params))
		return Msg(playerid,COLOR_ERROR,"/solo {b}<id/nick>{/b}");

	FindUIDByNameOrID(params,targetid,playerid);

	if (targetid==playerid) return Msg(playerid, COLOR_INFO3, "Fap, fap, fap... {b}Przegrales{/b}");

	if(!pTemp[targetid][accepts][eac_solo])
		return Msg(playerid, COLOR_ERROR, "Wskazany gracz nie akceptuje zaproszen do SOLO.");

	if(!IsPlayerConnected(targetid)) return Msg(playerid, COLOR_ERROR, "Wskazana osoba nie jest polaczona");
	if(!TeleportAllowed(targetid)) return Msg(playerid, COLOR_ERROR, "Wskazana osoba nie moze teraz stoczyc pojedynku");
	
	if (soloinv[targetid][esi_targetplayerid]==playerid && soloinv[targetid][esi_sent] ) {	// dwustronnie zaakceptowany pojedynek
		soloinv[playerid][esi_targetplayerid]=targetid;
		solo_startSolo(playerid, targetid, soloinv[targetid][esi_arenaid]);
		return 1;
	}

	// wybor areny

	soloinv[playerid][esi_targetplayerid]=targetid;
	soloinv[playerid][esi_sent]=false;

	return solo_wybierzArene(playerid);
}

CMD:solooff(playerid)
	return Msg(playerid,COLOR_INFO,"Uzyj komendy {b}/soloacc{/b} aby zarzadzac zezwoleniami na zaproszenia do /solo");

CMD:soloacc(playerid){
	if (pTemp[playerid][accepts][eac_solo]==ACCEPTS_NONE) {
		pTemp[playerid][accepts][eac_solo]=ACCEPTS_ALL;
		Msg(playerid,COLOR_INFO,"{b}Akceptujesz{/b} zaproszenia do pojedynkow solo.");
	} else {
 		pTemp[playerid][accepts][eac_solo]=ACCEPTS_NONE;
		Msg(playerid,COLOR_INFO,"{b}Nie akceptujesz{/b} zaproszen do pojedynkow solo.");
	}

	return 1;
}

solo_wybierzArene(playerid)
	return ShowPlayerDialog(playerid, DIALOG_ARENASOLO_SELECT, DIALOG_STYLE_LIST, "Wybierz arene", GetDialogList(playerid, DIALOG_ARENASOLO_SELECT), TXT(playerid, 76), TXT(playerid, 54));

solo_wybranaArena(playerid,arenaid){
	// mamy wybrana arene
	soloinv[playerid][esi_arenaid]=arenaid;
	soloinv[playerid][esi_sent]=true;

	// wybieramy bron1
	return ShowWeaponSelectMenu(playerid, "Wybierz pierwsza bron na solo", "solo_wybranaBron1");

}
forward solo_wybranaBron1(playerid,bron1);
public solo_wybranaBron1(playerid,bron1){
	if (bron1<0)
		return solo_wybierzArene(playerid);
	soloinv[playerid][esi_weapon1]=bron1;
	return ShowWeaponSelectMenu(playerid, "Wybierz druga bron na solo", "solo_wybranaBron2", weaponSlots[bron1]);
}
forward solo_wybranaBron2(playerid,bron2);
public solo_wybranaBron2(playerid,bron2){

	soloinv[playerid][esi_weapon2]=bron2;
	// wysylamy zaproszenie do gracza
	new buf[255];
	format(buf,sizeof buf,"{b}%s{/b} zaprasza Cie do pojedynku: arena {b}%s{/b}, bron: {b}%s, %s{/b}.", GetPlayerProperName(playerid),
				DATA_arenysolo[soloinv[playerid][esi_arenaid]][eas_nazwa], 	weaponName[soloinv[playerid][esi_weapon1]], weaponName[soloinv[playerid][esi_weapon2]]
				);
	
	Msg(soloinv[playerid][esi_targetplayerid],COLOR_INFO,buf);
	format(buf,sizeof buf,"Aby zaakceptowac, wpisz {b}/solo %d{/b}", playerid);
	Msg(soloinv[playerid][esi_targetplayerid],COLOR_INFO,buf ,false);

	Msg(playerid,COLOR_INFO,"Wyslano zaproszenie do pojedynku.");

	return 1;
	
}


solo_startSolo(playerid,targetid,arenaid) {
	new buf[128];
//	new aid=DATA_arenysolo[arenaid][eas_code];

	if(!TeleportAllowed(playerid) || !TeleportAllowed(targetid)) {
		Msg(playerid, COLOR_ERROR, "Solo nie moglo sie rozpoczac.");
		Msg(targetid, COLOR_ERROR, "Solo nie moglo sie rozpoczac.");
		return 1;
	}

	soloinv[playerid][esi_weapon1]=soloinv[targetid][esi_weapon1];
	soloinv[playerid][esi_weapon2]=soloinv[targetid][esi_weapon2];
	soloinv[playerid][esi_arenaid]=soloinv[targetid][esi_arenaid];

	soloinv[targetid][esi_sent]=false;

	SetPlayerHealth(playerid,99.0);
	SetPlayerArmour(playerid,DATA_arenysolo[arenaid][eas_ar]);		ResetPlayerWeapons(playerid);	

	new ammo1=500, ammo2=500;
	if (soloinv[targetid][esi_weapon1]==0)
		ammo1=0;
	else if (soloinv[targetid][esi_weapon1]<=15 || soloinv[targetid][esi_weapon1]==46)
		ammo1=1;

	if (soloinv[targetid][esi_weapon2]==0)
		ammo2=0;
	else if (soloinv[targetid][esi_weapon2]<=15 || soloinv[targetid][esi_weapon2]==46)
		ammo2=1;




	if (ammo1>0)
		GivePlayerWeapon(playerid,soloinv[targetid][esi_weapon1],ammo1); 
	if (ammo2>0)
		GivePlayerWeapon(playerid,soloinv[targetid][esi_weapon2],ammo2);

	SetPlayerHealth(targetid,99.0);
	SetPlayerArmour(targetid,DATA_arenysolo[arenaid][eas_ar]);		ResetPlayerWeapons(targetid);

	if (ammo1>0)
		GivePlayerWeapon(targetid,soloinv[targetid][esi_weapon1],ammo1); 
	if (ammo2>0)
		GivePlayerWeapon(targetid,soloinv[targetid][esi_weapon2],ammo2);

	soloinv[playerid][esi_start]=GetTickCount();
	soloinv[targetid][esi_start]=soloinv[playerid][esi_start];

	TogglePlayerClock(playerid,0);
	SetPlayerWeather(playerid,0);
	SetPlayerTime(playerid,12,0);
	TogglePlayerClock(targetid,0);
	SetPlayerWeather(targetid,0);
	SetPlayerTime(targetid,12,0);

	Teleport(T_PLAYER, playerid, 
			DATA_arenysolo[arenaid][eas_spawn1][0], DATA_arenysolo[arenaid][eas_spawn1][1],DATA_arenysolo[arenaid][eas_spawn1][2], DATA_arenysolo[arenaid][eas_spawn1][3], 
			DATA_arenysolo[arenaid][eas_interior], VW_UNIQUE_ARENASOLO+playerid+1, true);

	Teleport(T_PLAYER, targetid, 
			DATA_arenysolo[arenaid][eas_spawn2][0], DATA_arenysolo[arenaid][eas_spawn2][1],DATA_arenysolo[arenaid][eas_spawn2][2], DATA_arenysolo[arenaid][eas_spawn2][3], 
			DATA_arenysolo[arenaid][eas_interior], VW_UNIQUE_ARENASOLO+playerid+1, true);

	pData[playerid][pAttraction] = A_ARENA;
    pTemp[playerid][onArena]=ARENA_SOLO;

	pData[targetid][pAttraction] = A_ARENA;
    pTemp[targetid][onArena]=ARENA_SOLO;


	if(pData[playerid][hudSetting][HUD_DATE]) ShowElement(playerid, TDE_DATETIME, false);
	if(pData[playerid][hudSetting][HUD_STATUSBAR]) ShowElement(playerid, TDE_STATS, false);
	if(pData[playerid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(playerid, TDE_ATTRACTIONBOX, false);


	if(pData[targetid][hudSetting][HUD_DATE]) ShowElement(targetid, TDE_DATETIME, false);
	if(pData[targetid][hudSetting][HUD_STATUSBAR]) ShowElement(targetid, TDE_STATS, false);
	if(pData[targetid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(targetid, TDE_ATTRACTIONBOX, false);

	format(buf, sizeof buf, "Arena solo - %s, {3030ff}%s{ffffff} vs {3030ff}%s{ffffff}. Bronie: {306090}%s, %s", 
			DATA_arenysolo[soloinv[targetid][esi_arenaid]][eas_nazwa], GetPlayerProperName(playerid), GetPlayerProperName(targetid),
			weaponName[soloinv[targetid][esi_weapon1]], weaponName[soloinv[targetid][esi_weapon2]]);

	if (gmTemp[pPlayerCount]<=10)
		SendClientMessageToAll(0xff0000ff, buf);
	else
		solo_MSG(0xff0000ff, buf);
	
	return 1;
}

solo_OnPlayerDeath(playerid,killerid,reason){
	#pragma unused reason
	
	new buf[128], Float:hp, Float: ar;
	GetPlayerHealth(killerid,hp);
	GetPlayerArmour(killerid,ar);
	

//	czas=floatround(GetTickCount()/1000)-soloinv[playerid][esi_start];
	new roznica=floatround((GetTickCount()-soloinv[playerid][esi_start])/1000,floatround_floor);
//	SendClientMessageToAll(0xeeeeeeff,"Koniec solo (TODO USUNAC)");
//	printf("Roznica %d", roznica);

#if defined GAMETIMESYNCTOREAL
	TogglePlayerClock(playerid,1);
#endif
	if(pData[playerid][hudSetting][HUD_DATE]) ShowElement(playerid, TDE_DATETIME, true);
	if(pData[playerid][hudSetting][HUD_STATUSBAR]) ShowElement(playerid, TDE_STATS, true);
	if(pData[playerid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(playerid, TDE_ATTRACTIONBOX, true);
	SyncPlayerGameTime(playerid);

#if defined GAMETIMESYNCTOREAL
	TogglePlayerClock(killerid,1);
#endif
	if(pData[killerid][hudSetting][HUD_DATE]) ShowElement(killerid, TDE_DATETIME, true);
	if(pData[killerid][hudSetting][HUD_STATUSBAR]) ShowElement(killerid, TDE_STATS, true);
	if(pData[killerid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(killerid, TDE_ATTRACTIONBOX, true);
	SyncPlayerGameTime(killerid);


	format(buf, sizeof buf, "%s {eeeeee}wygrywa solo z {6060d0}%s{eeeeee}, arena %s.",  GetPlayerProperName(killerid), GetPlayerProperName(playerid),
			DATA_arenysolo[soloinv[killerid][esi_arenaid]][eas_nazwa]
			);
	if (gmTemp[pPlayerCount]<=10) 
		SendClientMessageToAll(0x6060d0ff, buf);
	else
		solo_MSG(0x6060d0ff, buf);



	format(buf, sizeof buf, "Bronie: %s, %s, zwyciezcy zostalo %03.0fHP, %03.0fAR, czas: {ffb648}%02dm%02ds{eeeeee}.",// zwyciezcy zostalo {ffb648}%fHP{eeeeee}.", 
		 	weaponName[soloinv[killerid][esi_weapon1]], weaponName[soloinv[killerid][esi_weapon2]],
			hp, ar,
			floatround(roznica/60, floatround_floor), roznica%60);

	if (gmTemp[pPlayerCount]<=10) 
		SendClientMessageToAll(0x6060d0ff,buf);
	else
		solo_MSG(0x6060d0ff,buf);

	pData[playerid][pAttraction] = A_NONE;
    pTemp[playerid][onArena]=ARENA_NONE;

	pData[killerid][pAttraction] = A_NONE;
    pTemp[killerid][onArena]=ARENA_NONE;

	SpawnPlayer(killerid);

	if (killerid>=0 && killerid!=INVALID_PLAYER_ID && IsPlayerConnected(killerid)) {
		format(buf, sizeof buf, "INSERT INTO fs_solo_matches SET arenaid=%d,weapon1=%d,weapon2=%d,playerid=%d,killerid=%d,fightlen=%d,hp=%.0f,ar=%.0f,ts=NOW();",
			DATA_arenysolo[soloinv[playerid][esi_arenaid]][eas_code], soloinv[playerid][esi_weapon1], soloinv[playerid][esi_weapon2], 
			pData[playerid][accountID], pData[killerid][accountID], roznica, hp, ar);
		mysql_query(buf);
	}


	return 1;
}

solo_MSG(color, txt[]){
	foreach(i)
		if ((pData[i][pAttraction]==A_ARENA && pTemp[i][onArena]==ARENA_SOLO) || (GetPlayerVirtualWorld(i)==VW_UNIQUE_ARENASOLO))
			SendClientMessage(i, color, txt);
	return 1;
}