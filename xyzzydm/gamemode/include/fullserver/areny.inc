

enum e_arena {
	ea_cmd[16],
	ea_limit,
	ea_cnt
}

new Areny[ARENA_MAX][e_arena]={
	{""},
	{"/ARENASOLO"},
	{""},
	{"/RPG      "},
	{"/MINIGUN  "},
	{"/PRO      "},
	{"/ONEDE    "},
	{"/ONEDE2   "},
	{"/AZTEC    ",10},
	{"/ONESHOOT "},
	{"/SNIPER   "},
	{"/PIER     ",20},
	{"/SO       ",10}
};

CMD:arena(playerid){
	MUST_BE_SPAWNED(playerid);
	if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));

	for (new i=0;i<sizeof Areny;i++)
		Areny[i][ea_cnt]=0;

	foreach(i)
		if (pTemp[i][onArena]>0 && pTemp[i][onArena]<ARENA_MAX)
			Areny[pTemp[i][onArena]][ea_cnt]++;

    new lista[1024];
	for (new i=0;i<sizeof Areny;i++) 
	 if (strlen(Areny[i][ea_cmd])>0) {
		if (Areny[i][ea_limit]<=0)
			format(lista, sizeof lista, "%s%s{3481B9}%s \t{ffffff}%d/-", lista, strlen(lista)>0?("\n"):(""), Areny[i][ea_cmd], Areny[i][ea_cnt]);
		else
			format(lista, sizeof lista, "%s%s{3481B9}%s \t{ffffff}%d/%d", lista, strlen(lista)>0?("\n"):(""), Areny[i][ea_cmd], Areny[i][ea_cnt], Areny[i][ea_limit]);
	 }

	ShowPlayerDialog(playerid,DIALOG_CMDSEL,DIALOG_STYLE_LIST,"Wybierz arene", lista, "OK", "Anuluj");
//"{3481B9}/ARENASOLO\n{ff6090}/AZTEC\n{3481B9}/MINIGUN\n{ffffff}/ONEDE\n{ffffff}/PRO\n{3481B9}/RPG","OK","Anuluj");
	return 1;
}
CMD:areny(playerid)
	return cmd_arena(playerid);


/*CMD:pueblo(playerid){
	return 0;
	pData[playerid][pAttraction] = A_ARENA;
	pTemp[playerid][onArena]=ARENA_PUEBLO;
	Msg(playerid,COLOR_INFO,"Dolaczasz do areny Lost Pueblo");
	SpawnPlayer(playerid);
	return 1;
}*/

CMD:arenaexit(playerid){
	MUST_BE_SPAWNED(playerid);
	if (pTemp[playerid][onArena]==ARENA_NONE)
		return Msg(playerid,COLOR_ERROR,"Nie jestes na zadnej arenie!");
	pData[playerid][pAttraction] = A_NONE;
	pTemp[playerid][onArena]=ARENA_NONE;
//	SetPlayerWorldBounds(playerid, 20000.0, -20000.0, 20000.0, -20000.0);
	Msg(playerid, COLOR_INFO, "Opusciles arene");
	SetPlayerHealth(playerid,100.0);
	SpawnPlayer(playerid);
	return 1;
}
CMD:arenexit(playerid)
	return cmd_arenaexit(playerid);

arena_CheckLimits(playerid){
	switch(pTemp[playerid][onArena]){
		case ARENA_PRODM,ARENA_ONEDE:	{
			if(pData[playerid][averagePing]>100) {
				Msg(playerid,COLOR_ERROR,"Niestety! Twoj sredni ping przekracza {b}100ms{/b} - nie mozesz dolaczyc do tej areny!");
				pData[playerid][pAttraction]=A_NONE;
				pTemp[playerid][onArena]=ARENA_NONE;
				return false;
			}
			
		}
	}
	return true;
}

arena_SpawnPlayer(playerid){
	if (!arena_CheckLimits(playerid)) return SpawnPlayer(playerid);
	switch(pTemp[playerid][onArena]) {
		case ARENA_SOLO: {
			return cmd_arenasolo(playerid);
		}
		case ARENA_PUEBLO: {
			SetPlayerVirtualWorld(playerid,VW_ARENA);
			SetPlayerArmour(playerid,50.0);
			SetPlayerColor(playerid,pData[playerid][currentColor]*256+32);
			GivePlayerWeapon(playerid, 4, 1);   // noz
			GivePlayerWeapon(playerid, 27, 1000);   // combat shotgun

			new r=random(sizeof spawns_arena_pueblo);
			SetPlayerPos(playerid, spawns_arena_pueblo[r][X], spawns_arena_pueblo[r][Y], spawns_arena_pueblo[r][Z]);
			SetPlayerFacingAngle(playerid, spawns_arena_pueblo[r][angle]);

			SetCameraBehindPlayer(playerid);
		}
		case ARENA_PRODM: {
			SetPlayerArmour(playerid,0.0);
			SetPlayerHealth(playerid,99.0);
			SetPlayerVirtualWorld(playerid,VW_ARENA);
			SetPlayerColor(playerid,pData[playerid][currentColor]*256+96);
			GivePlayerWeapon(playerid, 4, 1);
			GivePlayerWeapon(playerid, 25,	3500);	// shotgun
			GivePlayerWeapon(playerid, 29,	3500);	// mp5
			GivePlayerWeapon(playerid, 31,	3500);	// mp5
			GivePlayerWeapon(playerid, 24,	3500);	// deagle
			GivePlayerWeapon(playerid, 34,	3500);	// sniper
			new r=random(sizeof spawns_arena_prodm);
			SetPlayerPos(playerid, spawns_arena_prodm[r][X], spawns_arena_prodm[r][Y], spawns_arena_prodm[r][Z]);
			SetPlayerFacingAngle(playerid, spawns_arena_prodm[r][angle]);

			SetCameraBehindPlayer(playerid);

		}

		case ARENA_ONEDE: {
			SetPlayerArmour(playerid,0.0);
			SetPlayerHealth(playerid,20.0);
			SetPlayerVirtualWorld(playerid,VW_ARENA);
			SetPlayerColor(playerid,pData[playerid][currentColor]*256+64);
			GivePlayerWeapon(playerid, 24,	3500);	// deagle
			new r=random(sizeof spawns_arena_onede);
			SetPlayerPos(playerid, spawns_arena_onede[r][X], spawns_arena_onede[r][Y], spawns_arena_onede[r][Z]);
			SetPlayerFacingAngle(playerid, spawns_arena_onede[r][angle]);

			SetCameraBehindPlayer(playerid);
		}

		case ARENA_ONEDE2: {
			SetPlayerArmour(playerid,0.0);
			SetPlayerHealth(playerid,20.0);
			SetPlayerVirtualWorld(playerid,VW_ARENA);
			SetPlayerInterior(playerid, 3);
			SetPlayerColor(playerid,pData[playerid][currentColor]*256+64);
			GivePlayerWeapon(playerid, 24,	3500);	// deagle
			new r=random(sizeof spawns_arena_onede2);
			SetPlayerPos(playerid, spawns_arena_onede2[r][X], spawns_arena_onede2[r][Y], spawns_arena_onede2[r][Z]);
			SetPlayerFacingAngle(playerid, spawns_arena_onede2[r][angle]);

			SetCameraBehindPlayer(playerid);
		}

		case ARENA_AZTEC: {
			SetPlayerArmour(playerid,75.0);
			SetPlayerHealth(playerid,99.0);
			SetPlayerVirtualWorld(playerid,VW_ARENA);
			SetPlayerColor(playerid,pData[playerid][currentColor]*256+32);
			GivePlayerWeapon(playerid, 24,	3500);	// deagle
			GivePlayerWeapon(playerid, 16, 4);	// granaty
			GivePlayerWeapon(playerid, 31, 3500);// m4
			new r=random(sizeof spawns_arena_aztec);
			Teleport(T_PLAYER, playerid, spawns_arena_aztec[r][X], spawns_arena_aztec[r][Y], spawns_arena_aztec[r][Z]+1,spawns_arena_aztec[r][angle],
					0, VW_ARENA, true);
			GZ_DrawForPlayer(playerid,gz_arena_aztec)
//			SetCameraBehindPlayer(playerid);
		}


		case ARENA_MINIGUN: {
			new r=random(sizeof spawns_minigun_foot);
			SetPlayerPos(playerid, spawns_minigun_foot[r][0], spawns_minigun_foot[r][1], spawns_minigun_foot[r][2]);
			SetPlayerFacingAngle(playerid, spawns_minigun_foot[r][3]);
			GivePlayerWeapon(playerid,38,1500);
		}
		case ARENA_RPG: {
			new r=random(sizeof spawns_rpg_foot);
			SetPlayerPos(playerid, spawns_rpg_foot[r][0], spawns_rpg_foot[r][1], spawns_rpg_foot[r][2]);
			SetPlayerFacingAngle(playerid, spawns_rpg_foot[r][3]+random(5)-2.5);
			GivePlayerWeapon(playerid,35,300);
		}

		case ARENA_ONESHOOT: {
			SetPlayerArmour(playerid,0.0);
			SetPlayerHealth(playerid,5.0);
			SetPlayerVirtualWorld(playerid,VW_ARENA2);
			SetPlayerColor(playerid,pData[playerid][currentColor]*256+64);
			GivePlayerWeapon(playerid, 33,	3500);	// 34 strzelba
			new r=random(sizeof spawns_arena_oneshoot);
			SetPlayerPos(playerid, spawns_arena_oneshoot[r][X], spawns_arena_oneshoot[r][Y], spawns_arena_oneshoot[r][Z]);
			SetPlayerFacingAngle(playerid, spawns_arena_oneshoot[r][angle]);

			SetCameraBehindPlayer(playerid);
		}

		case ARENA_SNIPER: {
			SetPlayerArmour(playerid,0.0);
			SetPlayerHealth(playerid,20.0);
			SetPlayerVirtualWorld(playerid,VW_ARENA);
			SetPlayerColor(playerid,pData[playerid][currentColor]*256+10);
			GivePlayerWeapon(playerid, 34,	3500);	// sniper
			new r=random(sizeof spawns_arena_sniper);
			SetPlayerPos(playerid, spawns_arena_sniper[r][X], spawns_arena_sniper[r][Y], spawns_arena_sniper[r][Z]);
			SetPlayerFacingAngle(playerid, spawns_arena_sniper[r][angle]);

			SetCameraBehindPlayer(playerid);
		}

		case ARENA_SO: {
			SetPlayerArmour(playerid,0.0);
			SetPlayerHealth(playerid,99.0);
			SetPlayerVirtualWorld(playerid,VW_ARENA2);
			SetPlayerInterior(playerid,9);
			SetPlayerColor(playerid,pData[playerid][currentColor]*256+64);
			GivePlayerWeapon(playerid, 26,	3000);	// sawnoff
			SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, 999);
			new r=random(sizeof spawns_arena_so);
			SetPlayerPos(playerid, spawns_arena_so[r][X], spawns_arena_so[r][Y], spawns_arena_so[r][Z]);
			SetPlayerFacingAngle(playerid, spawns_arena_so[r][angle]);

			SetCameraBehindPlayer(playerid);
		}

		case ARENA_PIER: {
			SetPlayerArmour(playerid,0.0);
			SetPlayerHealth(playerid,99.0);
			SetPlayerVirtualWorld(playerid,VW_ARENA);
			SetPlayerColor(playerid,pData[playerid][currentColor]*256+64);
			GivePlayerWeapon(playerid, 29,	3000);	// mp5
			new r=random(sizeof spawns_arena_pier);
			SetPlayerPos(playerid, spawns_arena_pier[r][X], spawns_arena_pier[r][Y], spawns_arena_pier[r][Z]);
			SetPlayerFacingAngle(playerid, spawns_arena_pier[r][angle]);

			SetCameraBehindPlayer(playerid);
		}



		
	}
	return 1;
}

arena_OnPlayerDeath(playerid,killerid,reason){
	switch(pTemp[playerid][onArena]) {
		case ARENA_SOLO:	solo_OnPlayerDeath(playerid,killerid,reason);
		case ARENA_PRODM,ARENA_MINIGUN,ARENA_AZTEC,ARENA_SO,ARENA_PIER: {
			SetPlayerHealth(killerid, 99.0);
		}
	}
	if (!arena_CheckLimits(playerid)) SpawnPlayer(playerid);
	return 0;
}


CMD:pro(playerid){
	MUST_BE_SPAWNED(playerid);
	if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));
	if(pData[playerid][averagePing]>100)
		return Msg(playerid,COLOR_ERROR,"Niestety! Twoj sredni ping przekracza {b}100ms{/b} - nie mozesz dolaczyc do tej areny!");

	if (IsPlayerInAnyVehicle(playerid)) {
		RemovePlayerFromVehicle(playerid);
		new Float:PP[3];
		GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
		SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
	}
	pData[playerid][pAttraction] = A_ARENA;
	pTemp[playerid][onArena]=ARENA_PRODM;

	pData[playerid][hudSetting][HUD_FPS]=!pData[playerid][hudSetting][HUD_FPS];
	ShowPlayerHudElement(playerid,HUD_FPS,pData[playerid][hudSetting][HUD_FPS]);

	GameTextForPlayer(playerid,"~w~ARENA ~n~~r~PRO DM",1000,1);
	SpawnPlayer(playerid);
	return 1;
}


CMD:onede(playerid){
	MUST_BE_SPAWNED(playerid);
	if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));
	if(pData[playerid][averagePing]>100)
		return Msg(playerid,COLOR_ERROR,"Niestety! Twoj sredni ping przekracza {b}100ms{/b} - nie mozesz dolaczyc do tej areny!");

	if (IsPlayerInAnyVehicle(playerid)) {
		RemovePlayerFromVehicle(playerid);
		new Float:PP[3];
		GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
		SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
	}
	pData[playerid][pAttraction] = A_ARENA;
	pTemp[playerid][onArena]=ARENA_ONEDE;
	GameTextForPlayer(playerid,"~w~ARENA ~n~~r~ONE DE",1000,1);

	pData[playerid][hudSetting][HUD_FPS]=!pData[playerid][hudSetting][HUD_FPS];
	ShowPlayerHudElement(playerid,HUD_FPS,pData[playerid][hudSetting][HUD_FPS]);

	Msg(playerid,COLOR_INFO2,"Arena {b}ONEDE{/b} - desert eagle, 1hp");
	SpawnPlayer(playerid);
	return 1;
}

CMD:onede2(playerid){
	MUST_BE_SPAWNED(playerid);
	if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));
	if (IsPlayerInAnyVehicle(playerid)) {
		RemovePlayerFromVehicle(playerid);
		new Float:PP[3];
		GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
		SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
	}
	pData[playerid][pAttraction] = A_ARENA;
	pTemp[playerid][onArena]=ARENA_ONEDE2;
	GameTextForPlayer(playerid,"~w~ARENA ~n~~r~ONE DE ~h~2",1000,1);
	Msg(playerid,COLOR_INFO2,"Arena {b}ONEDE2{/b} - desert eagle, 1hp");
	SpawnPlayer(playerid);
	return 1;
}


CMD:aztec(playerid){
	MUST_BE_SPAWNED(playerid);
	if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));
	if (IsPlayerInAnyVehicle(playerid)) {
		RemovePlayerFromVehicle(playerid);
		new Float:PP[3];
		GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
		SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
	}
	pData[playerid][pAttraction] = A_ARENA;
	pTemp[playerid][onArena]=ARENA_AZTEC;
	GameTextForPlayer(playerid,"~w~ARENA ~n~~b~AZTEC",1000,1);
	SpawnPlayer(playerid);
	return 1;
}


CMD:minigun(playerid)
{
	MUST_BE_SPAWNED(playerid);
	if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));
	new r=random(sizeof spawns_minigun_foot);
	Teleport(T_PLAYER, playerid, spawns_minigun_foot[r][0]+random(1)-0.5, spawns_minigun_foot[r][1]+random(1)-0.5, spawns_minigun_foot[r][2], spawns_minigun_foot[r][3]+random(5)-2.5,0, VW_DEFAULT);
	GivePlayerWeapon(playerid,38,1500);
	GameTextForPlayer(playerid, "~b~Arena ~h~Minigun",1000,1);
	return 1;
}

CMD:rpg(playerid){
	MUST_BE_SPAWNED(playerid);
	if(!TeleportAllowed(playerid))
		return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic.");
	GivePlayerWeapon(playerid,35,40);
	Teleport(T_PLAYER, playerid, 608+random(100)-50, 850+random(100)-50, -43+400+random(150), FLOAT_NAN, 0, VW_DEFAULT);
	GivePlayerWeapon(playerid,46,1);
	GameTextForPlayer(playerid, "~r~Arena ~h~RPG", 1000, 1);
	
	return 1;
}

CMD:sniper(playerid){
	MUST_BE_SPAWNED(playerid);
	if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));
	if (IsPlayerInAnyVehicle(playerid)) {
		RemovePlayerFromVehicle(playerid);
		new Float:PP[3];
		GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
		SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
	}
	pData[playerid][pAttraction] = A_ARENA;
	pTemp[playerid][onArena]=ARENA_SNIPER;
	GameTextForPlayer(playerid,"~w~ARENA ~n~~r~SNIPER ~h~I",1000,1);
	SpawnPlayer(playerid);
	return 1;
}

CMD:oneshoot(playerid){
	MUST_BE_SPAWNED(playerid);
	if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));
	if (IsPlayerInAnyVehicle(playerid)) {
		RemovePlayerFromVehicle(playerid);
		new Float:PP[3];
		GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
		SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
	}
	pData[playerid][pAttraction] = A_ARENA;
	pTemp[playerid][onArena]=ARENA_ONESHOOT;
	GameTextForPlayer(playerid,"~w~ARENA ~n~~r~One~h~shoot",1000,1);
	SpawnPlayer(playerid);
	return 1;
}


CMD:so(playerid){
	MUST_BE_SPAWNED(playerid);
	if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));
	if (IsPlayerInAnyVehicle(playerid)) {
		RemovePlayerFromVehicle(playerid);
		new Float:PP[3];
		GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
		SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
	}
	pData[playerid][pAttraction] = A_ARENA;
	pTemp[playerid][onArena]=ARENA_SO;
	GameTextForPlayer(playerid,"~w~ARENA ~n~~p~SAWN-OFF",1000,1);
	SpawnPlayer(playerid);
	return 1;
}

CMD:pier(playerid){
	MUST_BE_SPAWNED(playerid);
	if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));
	if (IsPlayerInAnyVehicle(playerid)) {
		RemovePlayerFromVehicle(playerid);
		new Float:PP[3];
		GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
		SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
	}
	pData[playerid][pAttraction] = A_ARENA;
	pTemp[playerid][onArena]=ARENA_PIER;
	GameTextForPlayer(playerid,"~w~ARENA ~n~~y~PIER",1000,1);
	SpawnPlayer(playerid);
	return 1;
}




