
new	aDerbyVehicles[MAX_QUEUE_DERBY]={INVALID_VEHICLE_ID,...};
new aDerbyVehiclesPos[MAX_QUEUE_DERBY][e_Vectors];

Derby_Start()
{
	new
	 aPlayers[MAX_QUEUE_DERBY],
	 aMaxPlayers = 0, nitro=0;

	for(new i=0; i<MAX_QUEUE_DERBY;i++)
		aPlayers[i]=INVALID_PLAYER_ID;

	foreach(playerid)
	{
		if(pData[playerid][aDerby])
		{
			if(AttractionAllowed(playerid))
			{
				aPlayers[aMaxPlayers++] = playerid;
				if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
			}
			else
			{
				pData[playerid][aDerby] = false;
				Msg(playerid, COLOR_ERROR, TXT(playerid, 285));
			}
		}
	}

	if (aMaxPlayers<MIN_QUEUE_DERBY) {
		if (aMaxPlayers==0)
			SendClientMessageToAll(-1,"Derby nie wystartowaly, nikt z zapisanych nie mogl dolaczyc");
		else
			SendClientMessageToAll(-1,"Derby nie wystartowaly, nie udalo sie skompletowac minimalnej ilosci graczy.");
		aData[A_DERBY][aState] = A_STATE_OFF;
		Derby_Cleanup();
		return 0;
	}


	// losujemy arene
	// select id,vehicle,interior from fs_derby_arena where minPlayers<=3<=maxPlayers ORDER by rand() LIMIT 1;
	new buffer[127];
	if (gmTemp[aDerbyForceArena]>0) { // wybieramy wymuszona
		format(buffer,sizeof buffer,"select id,vehicle,interior,nitro,descr from fs_derby_arena where id=%d LIMIT 1;",gmTemp[aDerbyForceArena]);
		gmTemp[aDerbyForceArena]=0;
	} else // losujemy
		format(buffer,sizeof buffer,"select id,vehicle,interior,nitro,descr from fs_derby_arena where minPlayers<=%d AND %d<=maxPlayers ORDER by rand() LIMIT 1;",aMaxPlayers,aMaxPlayers);
	mysql_query(buffer);
	mysql_store_result();
	if (!mysql_num_rows()) {	// nie znaleziono odpowiedniej areny
		SendClientMessageToAll(-1,"Derby nie wystartowaly - nie znaleziono odpowiedniej areny");

		for(new i = 0; i < aMaxPlayers; i++)	{
			pData[aPlayers[i]][pAttraction] = A_DERBY;
			pData[aPlayers[i]][aDerby] = false;
		}
		aData[A_DERBY][aState] = A_STATE_OFF;
		Derby_Cleanup();
		if (mysql_result_stored()) 	mysql_free_result();
		return 1;
	}

	mysql_fetch_row(buffer," ");
	
	if (sscanf(buffer,"ddddS(Derby)[32]",gmTemp[aDerbyArena],gmTemp[aDerbyVehicle],gmTemp[aDerbyInterior],nitro, gmTemp[aDerbyArenaDescr])) {
		SendClientMessageToAll(-1,"Derby nie wystartowaly - nie znaleziono odpowiedniej areny");

		for(new i = 0; i < aMaxPlayers; i++)	{
			pData[aPlayers[i]][pAttraction] = A_NONE;//A_DERBY;
			pData[aPlayers[i]][aDerby] = false;
		}
		aData[A_DERBY][aState] = A_STATE_OFF;
		Derby_Cleanup();
		if (mysql_result_stored()) mysql_free_result();
		return 1;
	}
	mysql_free_result();

	// pobieramy dane spawnow
	// select X,Y,Z,angle from fs_derby_arena_sp where aid=1 order by rand() limit 4
	format(buffer,sizeof buffer,"select X,Y,Z,angle from fs_derby_arena_sp where aid=%d order by rand() limit %d",gmTemp[aDerbyArena],aMaxPlayers);
	mysql_query(buffer);
	mysql_store_result();
	for (new i=0;i<mysql_num_rows();i++){
		mysql_fetch_row(buffer," ");
		new Float:P[4];
		if (!sscanf(buffer,"ffff",P[0],P[1],P[2],P[3])) {

			pData[aPlayers[i]][pAttraction] = A_DERBY;
			pData[aPlayers[i]][aDerby] = false;
		
			TogglePlayerControllable(aPlayers[i], false);
			SetPlayerArmour(aPlayers[i], 0.0);
			SetPlayerHealth(aPlayers[i], 100.0);
			ResetPlayerWeapons(aPlayers[i]);
		
			Teleport(T_PLAYER, aPlayers[i], P[0], P[1], P[2]+3, P[3], gmTemp[aDerbyInterior], VW_DERBY);
			Streamer_Update(aPlayers[i]);
			// monster
			aDerbyVehicles[i]=CreateVehicle(gmTemp[aDerbyVehicle],P[0], P[1], P[2], P[3],random(253),random(253),1);
			aDerbyVehiclesPos[i][X]=P[0];
			aDerbyVehiclesPos[i][Y]=P[1];
			aDerbyVehiclesPos[i][Z]=P[2];
			tVehicles[aDerbyVehicles[i]][vo_used]=true;
			tVehicles[aDerbyVehicles[i]][vo_occupied]=true;
			if (nitro>0){
				AddVehicleComponent(aDerbyVehicles[i], 1010);
				vehicleHasNitro[aDerbyVehicles[i]]=true;
				tVehicles[aDerbyVehicles[i]][vo_used]=true;
			}

			SetVehicleVirtualWorld(aDerbyVehicles[i],VW_DERBY);
			LinkVehicleToInterior(aDerbyVehicles[i],gmTemp[aDerbyInterior]);
			if (gmTemp[aDerbyArena]==9)	// ugly ale jebac to
				SetVehicleHealth(aDerbyVehicles[i],999999);

			TogglePlayerControllable(aPlayers[i], false);

//			PutPlayerInVehicle(aPlayers[i],aDerbyVehicles[i],0);
		} else {
			pData[aPlayers[i]][pAttraction] = A_NONE;
			pData[aPlayers[i]][aDerby] = false;
		}
	}

	mysql_free_result();

    gmTemp[aDerbyPlayers] = aPlayers;
    gmTemp[aDerbyMaxPlayers] = aMaxPlayers;
    gmTemp[aDerbyCount] = 5;
	KillTimer(gmTemp[timerDerbyCountdown]);
    gmTemp[timerDerbyCountdown] = SetTimer("Derby_Countdown", 700, true);
    gmTemp[aDerbyStartTick] = GetTickCount();
	format(buffer,sizeof buffer,"~w~Arena_~n~~y~~h~%s", gmTemp[aDerbyArenaDescr]);
	for (new i=0;i<gmTemp[aDerbyMaxPlayers];i++)
		GameTextForPlayer(aPlayers[i], buffer, 6000, 1);

	return 1;
}

Derby_Cleanup(){
	KillTimer(gmTemp[timerDerbyCountdown]);

	for (new i=0;i<gmTemp[aDerbyMaxPlayers];i++) {
		if (gmTemp[aDerbyPlayers][i]!=INVALID_PLAYER_ID && GetPlayerVirtualWorld( gmTemp[aDerbyPlayers][i] )==VW_DERBY) {
//			SpawnPlayer( gmTemp[aDerbyPlayers][i]);
		}
		gmTemp[aDerbyPlayers][i]=INVALID_PLAYER_ID;
	}
	
/*	for (new i=0;i<MAX_QUEUE_DERBY;i++)
		if (aDerbyVehicles[i]>0 && aDerbyVehicles[i]>0 && aDerbyVehicles[i]!=INVALID_VEHICLE_ID && gmTemp[aDerbyVehicle]>0 && GetVehicleModel(aDerbyVehicles[i])==gmTemp[aDerbyVehicle]) {
			RespawnVehicle(aDerbyVehicles[i]);
			aDerbyVehicles[i]=INVALID_VEHICLE_ID;
		}*/
	return;
}

Derby_RemovePlayer(playerid,bool:force=false){
	if (force || (pData[playerid][pAttraction]==A_DERBY && GetPlayerVirtualWorld(playerid)==VW_DERBY)) {
//		new buf[128];
//		format(buf,sizeof buf,"Usuwanie gracza %d z derb.",playerid);
//		SendClientMessageToAll(-1,buf);
		if (IsPlayerInAnyVehicle(playerid)) RemovePlayerFromVehicle(playerid);
		pData[playerid][pAttraction] = A_NONE;
		pData[playerid][aDerby] = false;
		if (GetPlayerState(playerid)!=PLAYER_STATE_WASTED)
			SpawnPlayer(playerid);
		Derby_Recount();
		avt_record(playerid,e_derbyfan, 1, ART_ADD);
	}
	return 1;
}

Derby_Recount(){
//	printf("DERBY RECOUNT");//DBG
	if (gmTemp[aDerbyCount]>0) return 1;

	new pozostalo,lastPlayer=INVALID_PLAYER_ID;

	for (new i=0;i<gmTemp[aDerbyMaxPlayers];i++) 
		if (gmTemp[aDerbyPlayers][i]==INVALID_PLAYER_ID) continue;
		else if (pData[gmTemp[aDerbyPlayers][i]][pAttraction]==A_DERBY && GetPlayerVirtualWorld(gmTemp[aDerbyPlayers][i])==VW_DERBY) {
			if (pTemp[gmTemp[aDerbyPlayers][i]][staleTime]>4) {
				Msg(gmTemp[aDerbyPlayers][i],COLOR_ERROR,"Zostales usuniety z {b}/derby{/b} za {b}AFK{/b}!");
				Derby_RemovePlayer(gmTemp[aDerbyPlayers][i]);
			} if (GetPlayerVehicleID(gmTemp[aDerbyPlayers][i])==aDerbyVehicles[i]) {
				pozostalo++;
				lastPlayer=gmTemp[aDerbyPlayers][i];
			} else
				Derby_RemovePlayer(gmTemp[aDerbyPlayers][i]);
			
		}

	if (pozostalo<=1) {
		aData[A_DERBY][aState] = A_STATE_OFF;

		if (lastPlayer!=INVALID_PLAYER_ID) 	{
			pData[lastPlayer][pAttraction]=A_NONE;
//			ClearAnimations(lastPlayer);			// wyjmuje z pojazdow rc
//			TogglePlayerControllable(lastPlayer,true);	// zabiera jetpacka
//			RemovePlayerFromVehicle(lastPlayer);	// 
			new vid=GetPlayerVehicleID(lastPlayer);
			RemovePlayerFromVehicle(lastPlayer);
			RespawnVehicle(vid);

			new Float:P[3];
			GetPlayerPos(lastPlayer,P[0],P[1],P[2]);
			SetPlayerPos(lastPlayer,P[0],P[1],P[2]);
//			SetPlayerVirtualWorld(lastPlayer,0);
//			SetPlayerInterior(lastPlayer,0);

			if (GetPlayerState(lastPlayer)!=PLAYER_STATE_WASTED)
				SpawnPlayer(lastPlayer);
			new str[127];
			format(str,sizeof(str),"Derby wygrywa %s. Otrzymuje %d respektu.",GetPlayerProperName(lastPlayer),5+gmTemp[aDerbyMaxPlayers]);
			GivePlayerScore(lastPlayer, 5+gmTemp[aDerbyMaxPlayers], "wygrane Derby");
			avt_record(lastPlayer,e_derbywin, 1, ART_ADD);
			avt_record(lastPlayer,e_derbyfan, 1, ART_ADD);

			SendClientMessageToAll(-1,str);
		}
		Derby_Cleanup();
	}
			
	return 1;
}

forward Derby_Countdown();
public Derby_Countdown()
{
	new
	 buffer[64];
	
	if(gmTemp[aDerbyCount] <= 0)
	{
		format(buffer, sizeof buffer, "~g~GO!");
		KillTimer(gmTemp[timerDerbyCountdown]);
	}
	else
	{
		format(buffer, sizeof buffer, "~r~%i", gmTemp[aDerbyCount]);
	}
	
	for(new playerid = 0; playerid < gmTemp[aDerbyMaxPlayers]; playerid++)
	{
		GameTextForPlayer(gmTemp[aDerbyPlayers][playerid], buffer, 1000, 6);
		
		switch(gmTemp[aDerbyCount]){
			case 0: {
				PlaySound(gmTemp[aDerbyPlayers][playerid], 1057);
				TogglePlayerControllable(gmTemp[aDerbyPlayers][playerid], true);
			}
			case 1: {
				PlaySound(gmTemp[aDerbyPlayers][playerid], 1057);
				PutPlayerInVehicle(gmTemp[aDerbyPlayers][playerid],aDerbyVehicles[playerid],0);
			}
			case 5: {
//				TogglePlayerControllable(gmTemp[aDerbyPlayers][playerid], true);
//				Streamer_Update(gmTemp[aDerbyPlayers][playerid]);
//				TogglePlayerControllable(gmTemp[aDerbyPlayers][playerid], false);
				PlaySound(gmTemp[aDerbyPlayers][playerid], 1056);
			}
			default: {
				PlaySound(gmTemp[aDerbyPlayers][playerid], 1056);
				SetVehiclePos(aDerbyVehicles[playerid],aDerbyVehiclesPos[playerid][X],aDerbyVehiclesPos[playerid][Y],aDerbyVehiclesPos[playerid][Z]);
			}
		}
	}
	
	gmTemp[aDerbyCount]--;
}