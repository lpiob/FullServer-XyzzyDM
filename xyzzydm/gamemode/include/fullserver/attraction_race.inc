#define RACE_MAXCPS	130

forward race_RemovePlayer(playerid);
//forward race_CameraFall(rpid, Float:cX,Float:cY,Float:cZ,step);

new aRaceVehicles[MAX_QUEUE_RACE]={ INVALID_VEHICLE_ID, ... };
new aRaceVehiclesPos[MAX_QUEUE_RACE][e_Vectors];
new aRaceCP[RACE_MAXCPS];
new aRaceCPV[RACE_MAXCPS][e_Vectors];
new aRaceMI[RACE_MAXCPS];
new aRacePosition[MAX_QUEUE_RACE]={ -1, ... };
new aRaceCheckpoint[MAX_QUEUE_RACE]={ 0, ... };
new Float:aRaceLastCPZAngle[MAX_QUEUE_RACE];	// kat Z, w ktorym byl pojazd jak wjechal do ostatniego CP

enum e_rclt {
	erclt_racerid,
	erclt_ts
}

new aRaceCheckpointLastTime[RACE_MAXCPS][e_rclt];	// czasy dotarcia danego gracza do danego CP

new aRaceCPET[MAX_QUEUE_RACE]={ 0, ... };	// checkpoint entry time

new aRaceCameraPos[e_Vectors];

Race_Start()
{
	
	new
	 aPlayers[MAX_QUEUE_RACE],
	 aPlayersR[MAX_PLAYERS] = { INVALID_PLAYER_ID, ... },	// odwrotne mapowanie playerid (0..200,500) na id graczy w wyscigu (0..MAX_QUEUE_RACE)
	 aMaxPlayers = 0, nitro=0, aLimitRand=0;

	for(new i=0; i<MAX_QUEUE_RACE;i++)
		aPlayers[i]=INVALID_PLAYER_ID;

	foreach(playerid)
	{
		if(pData[playerid][aRace])
		{
			if(AttractionAllowed(playerid))
			{
				aPlayersR[playerid]=aMaxPlayers;
				aPlayers[aMaxPlayers++] = playerid;
				if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
			}
			else
			{
				pData[playerid][aRace] = false;
				Msg(playerid, COLOR_ERROR, TXT(playerid, 285));
			}
		}
	}


	if (aMaxPlayers<MIN_QUEUE_RACE) {
		if (aMaxPlayers==0)
			SendClientMessageToAll(0xff0000ff,"Wyscig nie wystartowal, nikt z zapisanych nie mogl dolaczyc");
		else
			SendClientMessageToAll(0xff0000ff,"Wyscig nie wystartowal, nie udalo sie skompletowac minimalnej ilosci graczy.");
		aData[A_RACE][aState] = A_STATE_OFF;
		Race_Cleanup();
		return 1;
	}
		


	new buffer[255],buffer2[255];
	if (gmTemp[aRaceForceCircuit]>0) {
		format(buffer,sizeof buffer,"select id,limitrand,vehicle,interior,nitro,scx,scy,scz,distance,descr from fs_races where id=%d ORDER by rand() LIMIT 1;",gmTemp[aRaceForceCircuit]);
		gmTemp[aRaceForceCircuit]=0;
	} else	// losujemy arene
		format(buffer,sizeof buffer,"select id,limitrand,vehicle,interior,nitro,scx,scy,scz,distance,descr from fs_races where active=1 AND minPlayers<=%d AND %d<=maxPlayers ORDER by rand() LIMIT 1;",aMaxPlayers,aMaxPlayers);
	
	

	mysql_query(buffer);
	mysql_store_result();
	if (!mysql_num_rows()) {	// nie znaleziono odpowiedniej areny
		SendClientMessageToAll(0xff0000ff,"Wyscig nie wystartowal - nie znaleziono odpowiedniej trasy");

		for(new i = 0; i < aMaxPlayers; i++)	{
			pData[aPlayers[i]][pAttraction] = A_NONE;//A_RACE;
			pData[aPlayers[i]][aRace] = false;
		}
		aData[A_RACE][aState] = A_STATE_OFF;
		Race_Cleanup();

		return 1;
	}

	mysql_fetch_row(buffer,"|");
	
	if (sscanf(buffer,"p<|>dddddffffs[255]",
				gmTemp[aRaceArena],aLimitRand,gmTemp[aRaceVehicle],gmTemp[aRaceInterior],nitro,
				aRaceCameraPos[X],aRaceCameraPos[Y],aRaceCameraPos[Z],gmTemp[aRaceDistance],gmTemp[aRaceDescr])) {
		SendClientMessageToAll(0xff0000ff,"Wyscig nie wystartowal - nie znaleziono odpowiedniej trasy");

		for(new i = 0; i < aMaxPlayers; i++)	{
			pData[aPlayers[i]][pAttraction] = A_NONE;//A_RACE;
			pData[aPlayers[i]][aRace] = false;
		}
		aData[A_RACE][aState] = A_STATE_OFF;
		Race_Cleanup();
		mysql_free_result();
		return 1;
	}
	if (mysql_result_stored()) mysql_free_result();


	// pobieramy dane spawnow
	format(buffer,sizeof buffer,"select X,Y,Z,angle from fs_races_sp where aid=%d order by rand() limit %d",gmTemp[aRaceArena],aMaxPlayers);
	mysql_query(buffer);
	mysql_store_result();
	for (new i=0;i<mysql_num_rows();i++){
		mysql_fetch_row(buffer," ");
		new Float:P[4];
		if (!sscanf(buffer,"ffff",P[0],P[1],P[2],P[3])) {
//			printf("ub: %s, P: %f %f %f %f", buffer,P[0],P[1],P[2],P[3]);
			pData[aPlayers[i]][pAttraction] = A_RACE;
			pData[aPlayers[i]][aRace] = false;
//			EnableStuntBonusForPlayer(aPlayers[i], 1);
			TogglePlayerControllable(aPlayers[i], false);
			SetPlayerArmour(aPlayers[i], 0.0);
			SetPlayerHealth(aPlayers[i], 100.0);
			ResetPlayerWeapons(aPlayers[i]);
		
			Teleport(T_PLAYER, aPlayers[i], P[0], P[1], P[2], P[3]-2.5, gmTemp[aRaceInterior], VW_RACE);	// przenosimy gracza pod ulice, w przypadku pojazdow RC inaczej widac skin przy starcie.
			Streamer_Update(aPlayers[i]);

			if (gmTemp[aRaceVehicle]>0) {
				aRaceVehicles[i]=CreateVehicle(gmTemp[aRaceVehicle],P[0], P[1], P[2], P[3],random(253),random(253),60);
			
				SetVehicleNumberPlate(aRaceVehicles[i],"WYSCIG");
				tVehicles[aRaceVehicles[i]][vo_licensePlateSet]=true;
				SetVehicleToRespawn(aRaceVehicles[i]);
				if (nitro>0){
					AddVehicleComponent(aRaceVehicles[i], 1010);
					vehicleHasNitro[aRaceVehicles[i]]=true;
				}
				SetVehicleVirtualWorld(aRaceVehicles[i],VW_RACE);
				LinkVehicleToInterior(aRaceVehicles[i],gmTemp[aRaceInterior]);

			}

			aRaceVehiclesPos[i][X]=P[0];
			aRaceVehiclesPos[i][Y]=P[1];
			aRaceVehiclesPos[i][Z]=P[2];
			aRaceVehiclesPos[i][A]=P[3];
			TogglePlayerControllable(aPlayers[i], false);
			if (gmTemp[aRaceVehicle]>0)
				PutPlayerInVehicle(aPlayers[i],aRaceVehicles[i],0);
			SetPlayerCameraPos(aPlayers[i],aRaceCameraPos[X],aRaceCameraPos[Y],aRaceCameraPos[Z]);
			SetPlayerCameraLookAt(aPlayers[i],aRaceVehiclesPos[i][X], aRaceVehiclesPos[i][Y], aRaceVehiclesPos[i][Z]);

		} else {
			pData[aPlayers[i]][pAttraction] = A_NONE;
			pData[aPlayers[i]][aRace] = false;
		}
	}
	mysql_free_result();
	if (aLimitRand>0 && aLimitRand<100)
		format(buffer,sizeof buffer,"select X,Y,Z,type,size from fs_races_cp where aid=%d AND (type=1 OR ( rand()*100<%d )) order by so,id ASC LIMIT %d",gmTemp[aRaceArena],aLimitRand,RACE_MAXCPS);
	else
		format(buffer,sizeof buffer,"select X,Y,Z,type,size from fs_races_cp where aid=%d order by so,id ASC LIMIT %d",gmTemp[aRaceArena],RACE_MAXCPS);
	mysql_query(buffer);
	mysql_store_result();
	gmTemp[aRaceNumCP]=mysql_num_rows();
//	printf("mysql num rows: %d numcp %d", mysql_num_rows(),gmTemp[aRaceNumCP]);
	new Float:P[4],Float:L[4],type,oldtype;
	new Float:totaldistance;

	if (!mysql_num_rows()) {	// nie znaleziono odpowiedniej areny
		SendClientMessageToAll(0xff0000ff,"Wyscig nie wystartowal - blad podczas wczytywania trasy");

		for(new i = 0; i < aMaxPlayers; i++)	{
			pData[aPlayers[i]][pAttraction] = A_RACE;
			pData[aPlayers[i]][aRace] = false;
		}
		aData[A_RACE][aState] = A_STATE_OFF;
		Race_Cleanup();

		return 1;
	}


	for (new i=0;i<mysql_num_rows();i++){
		mysql_fetch_row(buffer," ");
		if (!sscanf(buffer,"fffdf",P[0],P[1],P[2],type,P[3])) {
			aRaceCPV[i][X] = P[0]; aRaceCPV[i][Y] = P[1]; aRaceCPV[i][Z] = P[2]; 
			if (i>0) {
				new Float:pointdist=GetDistanceBetweenPoints3D(L[0], L[1], L[2],P[0],P[1],P[2]);
				aRaceCP[i-1]=CreateDynamicRaceCP(oldtype, L[0], L[1], L[2],P[0],P[1],P[2],P[3],VW_RACE, gmTemp[aRaceInterior], -1, pointdist*2+300);
				aRaceMI[i-1]=CreateDynamicMapIcon(L[0],L[1],L[2], 56, 1,VW_RACE,gmTemp[aRaceInterior],-1,pointdist*2+300);
//				printf("cp %d t:%d    %f,%f,%f -> %f,%f,%f, DIST: %f", i, oldtype, L[0], L[1], L[2],P[0],P[1],P[2],	GetDistanceBetweenPoints3D(L[0], L[1], L[2],P[0],P[1],P[2]));
				totaldistance+=pointdist;
			}

			L[0]=P[0]; L[1]=P[1]; L[2]=P[2]; L[3]=P[3]; oldtype=type;
		}	
	}
	mysql_free_result();
	aRaceCP[gmTemp[aRaceNumCP]-1]=CreateDynamicRaceCP(type, P[0], P[1], P[2],0,0,0,P[3],VW_RACE, gmTemp[aRaceInterior], -1, totaldistance);
	aRaceMI[gmTemp[aRaceNumCP]-1]=CreateDynamicMapIcon(P[0],P[1],P[2], 53, 1,VW_RACE,gmTemp[aRaceInterior],-1,totaldistance);
	Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, aRaceMI[gmTemp[aRaceNumCP]-1], E_STREAMER_STYLE, 1);

//	printf("cp %d t:%d    %f,%f,%f -> 0,0,0", gmTemp[aRaceNumCP], type, P[0],P[1],P[2]);

//	printf("Total distance: %f",totaldistance/21.645);
	if (gmTemp[aRaceDistance]==100) {
			gmTemp[aRaceDistance]=totaldistance/216;
			format(buffer,sizeof buffer,"UPDATE fs_races SET distance=%.2f WHERE id=%d", gmTemp[aRaceDistance], gmTemp[aRaceArena]);
			mysql_query(buffer);
	}

	format(buffer,sizeof buffer,"Trasa: {b}%s{/b}. Dlugosc: {b}%.1fkm{/b}. Ilosc punktow kontrolnych: {b}%d{/b}.",	gmTemp[aRaceDescr], gmTemp[aRaceDistance],gmTemp[aRaceNumCP]);

	for(new i = 0; i < aMaxPlayers; i++)    {
            TogglePlayerAllDynamicRaceCPs(aPlayers[i],0);
			TogglePlayerDynamicRaceCP(aPlayers[i], aRaceCP[0],1);
//			TogglePlayerDynamicRaceCP(aPlayers[i], aRaceCP[1],1);
//			TogglePlayerDynamicRaceCP(aPlayers[i], aRaceCP[3],1);
			aRaceCheckpoint[i]=-1;
			aRacePosition[i]=-1;
			aRaceCPET[i]=0;
			SendClientMessage(aPlayers[i],0xffffffff,"");
			SendClientMessage(aPlayers[i],0xffffffff,"");
			Msg(aPlayers[i],COLOR_INFO,buffer);
			SendClientMessage(aPlayers[i],0xffffffff,"");
			if (gmTemp[aRaceVehicle]>0) {
				Msg(aPlayers[i],COLOR_INFO2,"Wcisnij {b}2{/b} aby naprawic/obrocic swoj pojazd i wrocic do ostatniego CP na trasie!");
				SendClientMessage(aPlayers[i],0xffffffff,"");
			}
	}

	// TODO zrobic wiecej rekordow!
	format(buffer,sizeof buffer,"SELECT rr.recordtime,p.nick,DATE(rr.ts) kiedy FROM fs_races_records rr JOIN fs_players p on p.id=rr.playerid where raceid=%d ORDER BY recordtime ASC LIMIT 1", gmTemp[aRaceArena]);
	mysql_query(buffer);
	mysql_store_result();
	format(buffer2,sizeof buffer2,"Brak rekordow na tej trasie");
	if (mysql_num_rows()==1 && mysql_fetch_row(buffer,"|")) {
		
		new Float:recordtime;
		new recordowner[MAX_PLAYER_NAME];
		new recordts[11];
		if (!sscanf(buffer,"p<|>fs[25]s[11]", recordtime, recordowner, recordts)) {
			format(buffer2,sizeof buffer2, "{b}%.2f{b}s - %s, {b}%s{/b}", recordtime, recordowner, recordts);
			for(new i = 0; i < aMaxPlayers; i++) {
				Msg(aPlayers[i],COLOR_INFO,"Rekord trasy:");
				Msg(aPlayers[i],COLOR_INFO,buffer2);
			}
			format(buffer2,sizeof buffer2, "~n~Rekord trasy:~n~~n~~w~%s ~b~%s - ~y~%.2fs", recordts, recordowner, recordtime);
		}
	}
	mysql_free_result();
	format(buffer,sizeof buffer,"~n~~w~%s (%d)    ~y~%.1f~h~km~n~%s",gmTemp[aRaceDescr],gmTemp[aRaceArena],gmTemp[aRaceDistance], buffer2);
	TextDrawSetString(gTextDraw[TD_ARACEBOX], buffer);

    gmTemp[aRacePlayers] = aPlayers;
	gmTemp[aRacePlayersR] = aPlayersR;
    gmTemp[aRaceMaxPlayers] = aMaxPlayers;
    gmTemp[aRaceCount] = 5;
	KillTimer(gmTemp[timerRaceCountdown]);
    gmTemp[timerRaceCountdown] = SetTimer("Race_Countdown", 700, true);
    gmTemp[aRaceStartTick] = GetTickCount();
	return 1;
}

Race_Cleanup(){
	KillTimer(gmTemp[timerRaceCountdown]);
	for (new i=0;i<MAX_QUEUE_RACE;i++) {
		if (aRaceVehicles[i]>0 && aRaceVehicles[i]!=INVALID_VEHICLE_ID && gmTemp[aRaceVehicle]>0 && gmTemp[aRaceVehicle]==GetVehicleModel(aRaceVehicles[i]))
			DestroyVehicle(aRaceVehicles[i]);


	}
	for(new i=0; i<RACE_MAXCPS;i++) {
		if (IsValidDynamicRaceCP(aRaceCP[i]))
			DestroyDynamicRaceCP(aRaceCP[i]);
		if (IsValidDynamicMapIcon(aRaceMI[i]))
			DestroyDynamicMapIcon(aRaceMI[i]);
		aRaceCheckpointLastTime[i][erclt_racerid]=INVALID_PLAYER_ID;
		aRaceCheckpointLastTime[i][erclt_ts]=0;
	}

	for(new i = 0; i < gmTemp[aRaceMaxPlayers]; i++) {
		new playerid=gmTemp[aRacePlayers][i];
		if (pData[playerid][pAttraction]==A_RACE && GetPlayerVirtualWorld(playerid)==VW_RACE) {

			TextDrawHideForPlayer(playerid,pTextDraw[PTD_ARACEBOX][i]);
			TextDrawHideForPlayer(playerid,gTextDraw[TD_ARACEBOX]);

			if(pData[playerid][hudSetting][HUD_DATE]) ShowElement(playerid, TDE_DATETIME, true);
			if(pData[playerid][hudSetting][HUD_STATUSBAR]) ShowElement(playerid, TDE_STATS, true);
			if(pData[playerid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(playerid, TDE_ATTRACTIONBOX, true);
		
			pData[playerid][pAttraction] = A_NONE;
			SpawnPlayer(playerid);
		}
	}
	
	return;
}

public race_RemovePlayer(playerid){
	if (pData[playerid][pAttraction]==A_RACE) {
		pData[playerid][pAttraction] = A_NONE;

		TextDrawHideForPlayer(playerid,pTextDraw[PTD_ARACEBOX][gmTemp[aRacePlayersR][playerid]]);
		TextDrawHideForPlayer(playerid,gTextDraw[TD_ARACEBOX]);
		if(pData[playerid][hudSetting][HUD_DATE]) ShowElement(playerid, TDE_DATETIME, true);
		if(pData[playerid][hudSetting][HUD_STATUSBAR]) ShowElement(playerid, TDE_STATS, true);
		if(pData[playerid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(playerid, TDE_ATTRACTIONBOX, true);

		gmTemp[aRacePlayers][gmTemp[aRacePlayersR][playerid]]=INVALID_PLAYER_ID;
		gmTemp[aRacePlayersR][playerid] = INVALID_PLAYER_ID;

		if (IsPlayerInAnyVehicle(playerid)) {
			new Float:FP[3];
			new vid=GetPlayerVehicleID(playerid);
			GetVehiclePos(vid,FP[0],FP[1],FP[2]);
			RemovePlayerFromVehicle(playerid);
			SetPlayerPos(playerid,FP[0],FP[1],FP[2]-1);
			DestroyVehicle(vid);
		}

	}

	SpawnPlayer(playerid);
	Race_Recount();
	return 1;
}

Race_Recount(){
	new pozostalo,lastPlayer=INVALID_PLAYER_ID;
//	printf("Race recount");

	for (new i=0;i<gmTemp[aRaceMaxPlayers];i++) 
		if (gmTemp[aRacePlayers][i]==INVALID_PLAYER_ID) continue;
		else if (pData[gmTemp[aRacePlayers][i]][pAttraction]==A_RACE) {
			if (pTemp[gmTemp[aRacePlayers][i]][staleTime]>5) {
				Msg(gmTemp[aRacePlayers][i],COLOR_ERROR,"Zostales usuniety z wyscigu z powodu {b}AFK{/b}.");
				race_RemovePlayer(gmTemp[aRacePlayers][i]);
			}
			if (GetPlayerVirtualWorld(gmTemp[aRacePlayers][i])==VW_RACE) {
				pozostalo++;
				lastPlayer=gmTemp[aRacePlayers][i];
			} else
				race_RemovePlayer(gmTemp[aRacePlayers][i]);
		}

	if (pozostalo<=0) {
		aData[A_RACE][aState] = A_STATE_OFF;
		Race_Cleanup();
		if (lastPlayer!=INVALID_PLAYER_ID) {
			pData[lastPlayer][pAttraction]=A_NONE;
			SetPlayerVirtualWorld(lastPlayer,0);
			SetPlayerInterior(lastPlayer,0);
			SpawnPlayer(lastPlayer);
//			new str[127];
//			format(str,sizeof(str),"Race wygrywa %s. Otrzymuje %d respektu.",GetPlayerProperName(lastPlayer),5+gmTemp[aRaceMaxPlayers]);
//			GivePlayerScore(lastPlayer,5+gmTemp[aRaceMaxPlayers],"wygrany wyscig");
//			SendClientMessageToAll(0xff0000ff,str);
		}
	}
			
	return 1;
}

forward Race_Countdown();
public Race_Countdown()
{
	new
	 buffer[255];
	
	if(gmTemp[aRaceCount] <= 0)
	{
		format(buffer, sizeof buffer, "~g~GO!");
		KillTimer(gmTemp[timerRaceCountdown]);
	}
	else
	{
		format(buffer, sizeof buffer, "~r~%i", gmTemp[aRaceCount]);
	}


	
	for(new playerid = 0; playerid < gmTemp[aRaceMaxPlayers]; playerid++)
	{
		GameTextForPlayer(gmTemp[aRacePlayers][playerid], buffer, 1000, 6);
		
		switch(gmTemp[aRaceCount]){
			
			case 0: {
				PlaySound(gmTemp[aRacePlayers][playerid], 1057);
				TogglePlayerControllable(gmTemp[aRacePlayers][playerid], true);
				SetCameraBehindPlayer(gmTemp[aRacePlayers][playerid]);
			}
			default: {
				TogglePlayerControllable(gmTemp[aRacePlayers][playerid], true);
				Streamer_Update(gmTemp[aRacePlayers][playerid]);
				TogglePlayerControllable(gmTemp[aRacePlayers][playerid], false);
				PlaySound(gmTemp[aRacePlayers][playerid], 1056);

				SetPlayerCameraPos(gmTemp[aRacePlayers][playerid],aRaceCameraPos[X],aRaceCameraPos[Y],aRaceCameraPos[Z]);
				SetPlayerCameraLookAt(gmTemp[aRacePlayers][playerid],aRaceVehiclesPos[playerid][X], aRaceVehiclesPos[playerid][Y], aRaceVehiclesPos[playerid][Z]);
				
				Race_RefreshPTD(playerid);

				TextDrawShowForPlayer(gmTemp[aRacePlayers][playerid],pTextDraw[PTD_ARACEBOX][playerid]);
				TextDrawShowForPlayer(gmTemp[aRacePlayers][playerid],gTextDraw[TD_ARACEBOX]);
				if(pData[gmTemp[aRacePlayers][playerid]][hudSetting][HUD_DATE]) ShowElement(gmTemp[aRacePlayers][playerid], TDE_DATETIME, false);
				if(pData[gmTemp[aRacePlayers][playerid]][hudSetting][HUD_STATUSBAR]) ShowElement(gmTemp[aRacePlayers][playerid], TDE_STATS, false);
				if(pData[gmTemp[aRacePlayers][playerid]][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(gmTemp[aRacePlayers][playerid], TDE_ATTRACTIONBOX, false);

				if (gmTemp[aRaceVehicle]>0) {
					SetVehiclePos(aRaceVehicles[playerid],aRaceVehiclesPos[playerid][X],aRaceVehiclesPos[playerid][Y],aRaceVehiclesPos[playerid][Z]);
					SetVehicleZAngle(aRaceVehicles[playerid],aRaceVehiclesPos[playerid][A]);
				} else {
					SetPlayerPos(gmTemp[aRacePlayers][playerid], aRaceVehiclesPos[playerid][X],aRaceVehiclesPos[playerid][Y],aRaceVehiclesPos[playerid][Z]);
					SetPlayerFacingAngle(gmTemp[aRacePlayers][playerid], aRaceVehiclesPos[playerid][A]);
				}

			}
/*			default: {
				PlaySound(gmTemp[aRacePlayers][playerid], 1056);
			}*/
		}
	}
	
	gmTemp[aRaceCount]--;
}

race_OPEDRCP(playerid,checkpointid){
	new rpid=gmTemp[aRacePlayersR][playerid];
//	printf("playerid %d rpid %d cpid %d racecp %d NUMCP %d", playerid,rpid, checkpointid, aRaceCheckpoint[rpid], gmTemp[aRaceNumCP]);
	if (rpid==INVALID_PLAYER_ID) return 0;
	for (new i=((aRaceCheckpoint[rpid]<0)?(0):(aRaceCheckpoint[rpid])); i<gmTemp[aRaceNumCP]; i++) 
		if (aRaceCP[i]==checkpointid && gmTemp[aRaceNumCP]>(aRaceCheckpoint[rpid]+1)) {
//			printf("Enter: i: %d sizeof racecp %d, playerid %d - %d, pozycja %d",i, sizeof aRaceCP, playerid,checkpointid,rpid, aRaceCheckpoint[ rpid ]);
			aRaceCheckpoint[rpid]=i;
			aRaceCPET[rpid]=GetTickCount();

			if (aRaceCheckpointLastTime[i][erclt_racerid]!=rpid && aRaceCheckpointLastTime[i][erclt_racerid]!=INVALID_PLAYER_ID && aRaceCheckpointLastTime[i][erclt_ts]>0) {
				// mamy informacje o czasie odwiedzenia tego CP przez poprzedniego uczestnika wyscigu
				new str[32];
				new Float:roznica=float(aRaceCheckpointLastTime[i][erclt_ts]-aRaceCPET[rpid]);
//				printf(" %d %d %.2f ", aRaceCheckpointLastTime[i][erclt_ts], aRaceCPET[rpid], roznica/1000);

//				format(str, sizeof str, "_~n~_~n~_~n~%.3f ~g~sek.", -1*(roznica/1000));
//				GameTextForPlayer(gmTemp[aRacePlayers][aRaceCheckpointLastTime[i][erclt_racerid]] , str,600,6);

				format(str, sizeof str, "_~n~_~n~%.3f ~b~sek.", -1*roznica/1000);
				GameTextForPlayer(playerid, str,1000,6);
				// TODO: powyzsze wyniki sa prawidlowe, jesli roznica odleglosci pomiedzy graczami wynosi dokladnie 1CP.
				// dopisac petle ktora zsumuje roznice czasowe dla wszystkich CP!
				// edit: da sie chyba bez petli
			}
			aRaceCheckpointLastTime[i][erclt_racerid]=rpid;
			aRaceCheckpointLastTime[i][erclt_ts]=aRaceCPET[rpid];
			
			PlaySound(playerid, 1056);

			if ((i+1)==gmTemp[aRaceNumCP]) { 
					race_DropCamera(rpid);

					if (gmTemp[aRaceMaxPlayers]==1)	// sam jechal
						GivePlayerScore(playerid,2,"wygrany wyscig solo");
					else
						GivePlayerScore(playerid,floatround((5+gmTemp[aRaceMaxPlayers])/aRacePosition[rpid])+1,"wygrany wyscig");

					avt_record(playerid,e_racedist,floatround(gmTemp[aRaceDistance]),ART_ADD);
					avt_record(playerid,e_racefcnt,1,ART_ADD);
					SetTimerEx("race_RemovePlayer",3500, false, "d", playerid); 
					race_RecordResult(rpid);
					return 1; 
					}
			if (gmTemp[aRaceVehicle]>0)
				GetVehicleZAngle(aRaceVehicles[rpid],aRaceLastCPZAngle[rpid]);
			else
				GetPlayerFacingAngle(playerid,aRaceLastCPZAngle[rpid]);

			Race_RefreshTD();

			TogglePlayerDynamicRaceCP(playerid, checkpointid,0);
			TogglePlayerDynamicRaceCP(playerid, aRaceCP[i+1],1);
//			Streamer_UpdateEx(playerid, aRaceCPV[i][X],aRaceCPV[i][Y],aRaceCPV[i][Z]);

			Streamer_RemoveArrayData(STREAMER_TYPE_MAP_ICON, aRaceMI[i], E_STREAMER_PLAYER_ID, playerid);
//			TogglePlayerDynamicRaceCP(playerid, aRaceCP[i+2],1);

//			Race_RefreshPTD(rpid);

			return 1;
		}
	return 0;
}

race_Napraw(playerid){
	if (gmTemp[aRaceVehicle]==0) return 1;
	new rpid=gmTemp[aRacePlayersR][playerid];
	new rcpv=aRaceCheckpoint[rpid];
	
	if (rcpv<0) {	// nie dojechal jeszcze do pierwszego CP
		PlaySound(playerid, 1056);
		SetVehiclePos(aRaceVehicles[rpid],aRaceVehiclesPos[rpid][X],aRaceVehiclesPos[rpid][Y],aRaceVehiclesPos[rpid][Z]);
		SetVehicleZAngle(aRaceVehicles[rpid],aRaceVehiclesPos[rpid][A]);
		RepairVehicle(aRaceVehicles[gmTemp[aRacePlayersR][playerid]]);
	} else if (rcpv>=0 && rcpv<sizeof aRaceCP) {
		aRaceCPET[rpid]=GetTickCount();	// gracz cofa sie w wyscigu!

		PlaySound(playerid, 1056);
		SetVehiclePos(aRaceVehicles[gmTemp[aRacePlayersR][playerid]],aRaceCPV[rcpv][X],aRaceCPV[rcpv][Y],aRaceCPV[rcpv][Z]+1.5);
		SetVehicleZAngle(aRaceVehicles[rpid],aRaceLastCPZAngle[rpid]+random(10)-5);
		RepairVehicle(aRaceVehicles[gmTemp[aRacePlayersR][playerid]]);
	}
	return 1;
}

Race_RefreshTD(){
	//printf("RECHECKING");
	new pozycja=1;
	if (gmTemp[aRaceMaxPlayers]==1)	// tylko jeden scigajacy sie
		aRacePosition[0]=pozycja++;
	else
		for(new i=gmTemp[aRaceNumCP]; i>0; i--) {
			new wtymcp[MAX_QUEUE_RACE]={-1,...};
			new i3=0;
			for(new i2=0; i2<gmTemp[aRaceMaxPlayers]; i2++)
				if (aRaceCheckpoint[i2]==(i-1)) wtymcp[i3++]=i2;
		
			if (i3==1)	// jeden pojazd w tym CP
				aRacePosition[wtymcp[0]]=pozycja++;
			else if (i3>1) { // kilka pojazdow w tym CP
				new i4=i3;
				while (i4>0) {
//					print("i4: %d");
					new najszybszy=-1;
					for(new i5=0;i5<i4;i5++)
						if (wtymcp[i5]==-1) continue;
						else if (najszybszy==-1) najszybszy=i5;
						else if (aRaceCPET[wtymcp[najszybszy]]>aRaceCPET[wtymcp[i5]]) najszybszy=i5;
					if (najszybszy==-1) break;	// nie powinno sie nigdy zdarzyc, ale robimy na wypadek wystapienia petli
//					printf("najszybszy %d -> %d",najszybszy, wtymcp[najszybszy]);
					aRacePosition[wtymcp[najszybszy]]=pozycja++;
					wtymcp[najszybszy]=-1;
					i4--;
				}
			}
		}

	new buffer[1023];

	format(buffer,sizeof buffer,"~n~~w~%s (%d)    ~y~%.1f~h~km~n~~b~",gmTemp[aRaceDescr],gmTemp[aRaceArena],gmTemp[aRaceDistance]);
	for(new npozycja=1; npozycja<=(pozycja>6?6:pozycja); npozycja++)
		for(new i=0;i<gmTemp[aRaceMaxPlayers];i++)
			if (aRacePosition[i]==npozycja && gmTemp[aRacePlayers][i]!=INVALID_PLAYER_ID) {
				format(buffer,sizeof buffer,"%s%d. %s~n~~h~",buffer, npozycja, GetPlayerProperName(gmTemp[aRacePlayers][i]));
				Race_RefreshPTD(i);
				break;
			}

	TextDrawSetString(gTextDraw[TD_ARACEBOX], buffer);

//	for(new i = 0; i < gmTemp[aRaceMaxPlayers]; i++) {
//		if (gmTemp[aRacePlayers][i]!=INVALID_PLAYER_ID && IsPlayerConnected(gmTemp[aRacePlayers][i]))
//	}
//	printf("DONE");
}

Race_RefreshPTD(rpid){
	new str[25];
	if (aRacePosition[rpid]<0 || aRaceCheckpoint[rpid]<0)
		format(str,sizeof str, "~b~-/%d    ~h~-/%d", gmTemp[aRaceMaxPlayers], gmTemp[aRaceNumCP]);
	else
		format(str,sizeof str, "~b~%d/%d    ~h~%d/%d", aRacePosition[rpid], gmTemp[aRaceMaxPlayers], aRaceCheckpoint[rpid]+1, gmTemp[aRaceNumCP]);
	TextDrawSetString(pTextDraw[PTD_ARACEBOX][rpid], str);
}

race_DropCamera(rpid){
	new Float:PF[6];

	if (gmTemp[aRaceVehicle]>0) {
		GetVehiclePos(aRaceVehicles[rpid],PF[0],PF[1],PF[2]);
		GetVehicleVelocity(aRaceVehicles[rpid],PF[3],PF[4],PF[5]);
	} else {
		GetPlayerPos(gmTemp[aRacePlayers][rpid],PF[0],PF[1],PF[2]);
		GetPlayerVelocity(gmTemp[aRacePlayers][rpid],PF[3],PF[4],PF[5]);
	}
	PF[3]=-1*PF[3]*3+PF[0];
	PF[4]=-1*PF[4]*3+PF[1];
	PF[5]=-1*PF[5]*3+PF[2]+random(2)-1;

	SetPlayerCameraPos(gmTemp[aRacePlayers][rpid],PF[3],PF[4],PF[5]);
	SetPlayerCameraLookAt(gmTemp[aRacePlayers][rpid],PF[0],PF[1],PF[2]);
//	TogglePlayerControllable(gmTemp[aRacePlayers][rpid],false);
//	race_CameraFall(rpid, PF[3],PF[4],PF[5],1);
}

race_RecordResult(rpid){
	if (pData[gmTemp[aRacePlayers][rpid]][accountID]>0) {
		new buf[256];
		format(buf,sizeof buf,"INSERT INTO fs_races_records SET playerid=%d,raceid=%d,ts=NOW(),recordtime=%.2f,opponents=%d,finalposition=%d",
			pData[gmTemp[aRacePlayers][rpid]][accountID], gmTemp[aRaceArena], float(aRaceCPET[rpid]-gmTemp[aRaceStartTick])/1000, gmTemp[aRaceMaxPlayers], aRacePosition[rpid]);
		mysql_query(buf);
	}
	return;
}

