
#define DRIFT_MAXCPS	20

forward drift_RemovePlayer(playerid);
forward drift_loop(step);

#define DRIFT_TD_x (320.0)
#define DRIFT_TD_y (250.0)

new Text:Drift_TD[MAX_QUEUE_DRIFT];
new Text:Drift_TDs[MAX_QUEUE_DRIFT];
new Bar:Drift_bar[MAX_QUEUE_DRIFT];
new Float:DriftPoints[MAX_QUEUE_DRIFT];
new Float:Drift_lastvhp[MAX_QUEUE_DRIFT];


new aDriftVehicles[MAX_QUEUE_DRIFT]={ INVALID_VEHICLE_ID, ... };
new aDriftVehiclesPos[MAX_QUEUE_DRIFT][e_Vectors];
new aDriftCP[DRIFT_MAXCPS];
new aDriftCPV[DRIFT_MAXCPS][e_Vectors];
new aDriftMI[DRIFT_MAXCPS];
//new aDriftPosition[MAX_QUEUE_DRIFT]={ -1, ... };
new aDriftScore[MAX_QUEUE_DRIFT]={0, ... };
new aDriftCheckpoint[MAX_QUEUE_DRIFT]={ 0, ... };
new Float:aDriftLastCPZAngle[MAX_QUEUE_DRIFT];	// kat Z, w ktorym byl pojazd jak wjechal do ostatniego CP

static drift_tablice[][16]={
	"DRIFT",
	"2FAST4U",
	"SPEED",
	"I <3 DR"
};

enum e_drlt {
	edrlt_racerid,
	edrlt_ts
}

new aDriftCheckpointLastTime[DRIFT_MAXCPS][e_drlt];	// czasy dotarcia danego gracza do danego CP

new aDriftCPET[MAX_QUEUE_DRIFT]={ 0, ... };	// checkpoint entry time

new aDriftCameraPos[e_Vectors];

Drifting_Start()
{
	new
	 aPlayers[MAX_QUEUE_DRIFT],
	 aPlayersR[MAX_PLAYERS] = { INVALID_PLAYER_ID, ... },	// odwrotne mapowanie playerid (0..200,500) na id graczy w wyscigu (0..MAX_QUEUE_DRIFT)
	 aMaxPlayers = 0, nitro=0, aLimitRand=0;

	for(new i=0; i<MAX_QUEUE_DRIFT;i++) {
		aPlayers[i]=INVALID_PLAYER_ID;
	}

	foreach(playerid)
	{
		if(pData[playerid][aDrift])
		{
			if(AttractionAllowed(playerid))
			{
				aPlayersR[playerid]=aMaxPlayers;

				aPlayers[aMaxPlayers++] = playerid;
				if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
			}
			else
			{
				pData[playerid][aDrift] = false;
				Msg(playerid, COLOR_ERROR, TXT(playerid, 285));
			}
		}
	}

	if (aMaxPlayers<MIN_QUEUE_DRIFT) {
		if (aMaxPlayers==0)
			SendClientMessageToAll(0xff0000ff,"Drifting nie wystartowal, nikt z zapisanych nie mogl dolaczyc");
		else
			SendClientMessageToAll(0xff0000ff,"Drifting nie wystartowal, nie udalo sie skompletowac minimalnej ilosci graczy.");
		aData[A_DRIFT][aState] = A_STATE_OFF;
		Drift_Cleanup();
		return 1;
	}

	new buffer[255],buffer2[255];
	if (gmTemp[aDriftForceCircuit]>0) {
		format(buffer,sizeof buffer,"select id,limitrand,vehicle,interior,nitro,scx,scy,scz,distance,partime,descr from fs_drift_tracks where id=%d ORDER by rand() LIMIT 1;",gmTemp[aDriftForceCircuit]);
		gmTemp[aDriftForceCircuit]=0;
	} else	// losujemy arene
		format(buffer,sizeof buffer,"select id,limitrand,vehicle,interior,nitro,scx,scy,scz,distance,partime,descr from fs_drift_tracks where active=1 AND minPlayers<=%d AND %d<=maxPlayers ORDER by rand() LIMIT 1;",aMaxPlayers,aMaxPlayers);
	
	
	mysql_query(buffer);
	mysql_store_result();
	if (!mysql_num_rows()) {	// nie znaleziono odpowiedniej areny
		SendClientMessageToAll(0xff0000ff,"Drifting nie wystartowal - nie znaleziono odpowiedniej trasy");

		for(new i = 0; i < aMaxPlayers; i++)	{
			pData[aPlayers[i]][pAttraction] = A_NONE;//A_DRIFT;
			pData[aPlayers[i]][aDrift] = false;
		}
		aData[A_DRIFT][aState] = A_STATE_OFF;
		Drift_Cleanup();

		return 1;
	}

	mysql_fetch_row(buffer,"|");
	
	if (sscanf(buffer,"p<|>dddddffffds[255]",
				gmTemp[aDriftArena],aLimitRand,gmTemp[aDriftVehicle],gmTemp[aDriftInterior],nitro,
				aDriftCameraPos[X],aDriftCameraPos[Y],aDriftCameraPos[Z],gmTemp[aDriftDistance],gmTemp[aDriftParTime],gmTemp[aDriftDescr])) {
		SendClientMessageToAll(0xff0000ff,"Drifting nie wystartowal - nie znaleziono odpowiedniej trasy");

		for(new i = 0; i < aMaxPlayers; i++)	{
			pData[aPlayers[i]][pAttraction] = A_NONE;//A_DRIFT;
			pData[aPlayers[i]][aDrift] = false;
		}
		aData[A_DRIFT][aState] = A_STATE_OFF;
		Drift_Cleanup();
		mysql_free_result();
		return 1;
	}
	if (mysql_result_stored()) mysql_free_result();


	// pobieramy dane spawnow
	format(buffer,sizeof buffer,"select X,Y,Z,angle from fs_drift_tracks_sp where aid=%d order by rand() limit %d",gmTemp[aDriftArena],aMaxPlayers);
//	printf("%s",buffer);
	mysql_query(buffer);
	mysql_store_result();
	for (new i=0;i<mysql_num_rows();i++){
//		printf("5.1");
		mysql_fetch_row(buffer," ");
//		printf("buf: %s", buffer);
		new Float:P[4];
		if (!sscanf(buffer,"ffff",P[0],P[1],P[2],P[3])) {
//			printf("ub: %s, P: %f %f %f %f", buffer,P[0],P[1],P[2],P[3]);
			pData[aPlayers[i]][pAttraction] = A_DRIFT;
			pData[aPlayers[i]][aDrift] = false;
//			EnableStuntBonusForPlayer(aPlayers[i], 1);
			TogglePlayerControllable(aPlayers[i], false);
			SetPlayerArmour(aPlayers[i], 0.0);
			SetPlayerHealth(aPlayers[i], 100.0);
			ResetPlayerWeapons(aPlayers[i]);
		
			Teleport(T_PLAYER, aPlayers[i], P[0], P[1], P[2], P[3]-2.5, gmTemp[aDriftInterior], VW_DRIFT);	// przenosimy gracza pod ulice, w przypadku pojazdow RC inaczej widac skin przy starcie.
			Streamer_Update(aPlayers[i]);


			aDriftScore[i]=0;
			Drift_TD[i]=TextDrawCreate(DRIFT_TD_x, DRIFT_TD_y, "~b~.");
			TextDrawShowForPlayer(aPlayers[i],Drift_TD[i]);
			Drift_bar[i]=CreateProgressBar(296.00, 271.00, 55.50, 3.20, 269549376, 100.0);
			SetProgressBarValue(Drift_bar[i], 0.0);
			ShowProgressBarForPlayer(aPlayers[i], Drift_bar[i]);
			UpdateProgressBar(Drift_bar[i], aPlayers[i]);

			Drift_TDs[i] = TextDrawCreate(321.000000, 278.000000, "_");
			TextDrawAlignment(Drift_TDs[i], 2);
			TextDrawBackgroundColor(Drift_TDs[i], 255);
			TextDrawFont(Drift_TDs[i], 3);
			TextDrawLetterSize(Drift_TDs[i], 0.300000, 1.000000);
			TextDrawColor(Drift_TDs[i], 0xFFFFFFFF);
			TextDrawSetOutline(Drift_TDs[i], 1);
			TextDrawSetProportional(Drift_TDs[i], 1);
			TextDrawShowForPlayer(aPlayers[i],Drift_TDs[i]);





			if (gmTemp[aDriftVehicle]>0) {
				aDriftVehicles[i]=CreateVehicle(gmTemp[aDriftVehicle],P[0], P[1], P[2], P[3],random(253),random(253),60);
				TuneCar(aDriftVehicles[i]);

				SetVehicleNumberPlate(aDriftVehicles[i],drift_tablice[random(sizeof drift_tablice)]);
				tVehicles[aDriftVehicles[i]][vo_licensePlateSet]=true;
				SetVehicleToRespawn(aDriftVehicles[i]);
				if (nitro>0){
					AddVehicleComponent(aDriftVehicles[i], 1010);
					vehicleHasNitro[aDriftVehicles[i]]=true;
				}
				SetVehicleVirtualWorld(aDriftVehicles[i],VW_DRIFT);
				LinkVehicleToInterior(aDriftVehicles[i],gmTemp[aDriftInterior]);

			}

			aDriftVehiclesPos[i][X]=P[0];
			aDriftVehiclesPos[i][Y]=P[1];
			aDriftVehiclesPos[i][Z]=P[2];
			aDriftVehiclesPos[i][A]=P[3];
			TogglePlayerControllable(aPlayers[i], false);
			if (gmTemp[aDriftVehicle]>0)
				PutPlayerInVehicle(aPlayers[i],aDriftVehicles[i],0);
			SetPlayerCameraPos(aPlayers[i],aDriftCameraPos[X],aDriftCameraPos[Y],aDriftCameraPos[Z]);
			SetPlayerCameraLookAt(aPlayers[i],aDriftVehiclesPos[i][X], aDriftVehiclesPos[i][Y], aDriftVehiclesPos[i][Z]);

		} else {
			pData[aPlayers[i]][pAttraction] = A_NONE;
			pData[aPlayers[i]][aDrift] = false;
		}
	}
	mysql_free_result();
	if (aLimitRand>0 && aLimitRand<100)
		format(buffer,sizeof buffer,"select X,Y,Z,type,size from fs_drift_tracks_cp where aid=%d AND (type=1 OR ( rand()*100<%d )) order by so,id ASC LIMIT %d",gmTemp[aDriftArena],aLimitRand,DRIFT_MAXCPS);
	else
		format(buffer,sizeof buffer,"select X,Y,Z,type,size from fs_drift_tracks_cp where aid=%d order by so,id ASC LIMIT %d",gmTemp[aDriftArena],DRIFT_MAXCPS);
	mysql_query(buffer);

//	printf("6");

	mysql_store_result();
	gmTemp[aDriftNumCP]=mysql_num_rows();
//	printf("mysql num rows: %d numcp %d", mysql_num_rows(),gmTemp[aDriftNumCP]);
	new Float:P[4],Float:L[4],type,oldtype;
	new Float:totaldistance;

	if (!mysql_num_rows()) {	// nie znaleziono odpowiedniej areny
		SendClientMessageToAll(0xff0000ff,"Drifting nie wystartowal - blad podczas wczytywania trasy");

		for(new i = 0; i < aMaxPlayers; i++)	{
			pData[aPlayers[i]][pAttraction] = A_DRIFT;
			pData[aPlayers[i]][aDrift] = false;
		}
		aData[A_DRIFT][aState] = A_STATE_OFF;
		Drift_Cleanup();

		return 1;
	}

	for (new i=0;i<mysql_num_rows();i++){
		mysql_fetch_row(buffer," ");
		if (!sscanf(buffer,"fffdf",P[0],P[1],P[2],type,P[3])) {
			aDriftCPV[i][X] = P[0]; aDriftCPV[i][Y] = P[1]; aDriftCPV[i][Z] = P[2]; 
			if (i>0) {
				new Float:pointdist=GetDistanceBetweenPoints3D(L[0], L[1], L[2],P[0],P[1],P[2]);
				aDriftCP[i-1]=CreateDynamicRaceCP(oldtype, L[0], L[1], L[2],P[0],P[1],P[2],P[3],VW_DRIFT, gmTemp[aDriftInterior], -1, pointdist*2+300);
				aDriftMI[i-1]=CreateDynamicMapIcon(L[0],L[1],L[2], 56, 1,VW_DRIFT,gmTemp[aDriftInterior],-1,pointdist*2+300);
//				printf("cp %d t:%d    %f,%f,%f -> %f,%f,%f, DIST: %f", i, oldtype, L[0], L[1], L[2],P[0],P[1],P[2],	GetDistanceBetweenPoints3D(L[0], L[1], L[2],P[0],P[1],P[2]));
				totaldistance+=pointdist;
			}

			L[0]=P[0]; L[1]=P[1]; L[2]=P[2]; L[3]=P[3]; oldtype=type;
		}	
	}
	mysql_free_result();
	aDriftCP[gmTemp[aDriftNumCP]-1]=CreateDynamicRaceCP(1, P[0], P[1], P[2],0,0,0,P[3],VW_DRIFT, gmTemp[aDriftInterior], -1, totaldistance);
	aDriftMI[gmTemp[aDriftNumCP]-1]=CreateDynamicMapIcon(P[0],P[1],P[2], 53, 1,VW_DRIFT,gmTemp[aDriftInterior],-1,totaldistance);
	Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, aDriftMI[gmTemp[aDriftNumCP]-1], E_STREAMER_STYLE, 1);

//	printf("cp %d t:%d    %f,%f,%f -> 0,0,0", gmTemp[aDriftNumCP], type, P[0],P[1],P[2]);

//	printf("Total distance: %f",totaldistance/21.645);
	if (gmTemp[aDriftDistance]==100) {
			gmTemp[aDriftDistance]=totaldistance/216;
			format(buffer,sizeof buffer,"UPDATE fs_drift_tracks SET distance=%.2f WHERE id=%d", gmTemp[aDriftDistance], gmTemp[aDriftArena]);
			mysql_query(buffer);
	}

	format(buffer,sizeof buffer,"Trasa: {b}%s{/b}. Dlugosc: {b}%.1fkm{/b}. Czas: {b}%02dm%02ds{/b}, Ilosc punktow kontrolnych: {b}%d{/b}.",	gmTemp[aDriftDescr], gmTemp[aDriftDistance],gmTemp[aDriftParTime]/60,gmTemp[aDriftParTime]%60, gmTemp[aDriftNumCP]);

	for(new i = 0; i < aMaxPlayers; i++)    {
            TogglePlayerAllDynamicRaceCPs(aPlayers[i],0);
			TogglePlayerDynamicRaceCP(aPlayers[i], aDriftCP[0],1);
//			TogglePlayerDynamicRaceCP(aPlayers[i], aDriftCP[1],1);
//			TogglePlayerDynamicRaceCP(aPlayers[i], aDriftCP[3],1);
			aDriftCheckpoint[i]=-1;
			aDriftScore[i]=0;
			DriftPoints[i]=0;
			aDriftCPET[i]=0;
			SendClientMessage(aPlayers[i],0xffffffff,"");
			SendClientMessage(aPlayers[i],0xffffffff,"");
			Msg(aPlayers[i],COLOR_INFO,buffer);
			SendClientMessage(aPlayers[i],0xffffffff,"");
			if (gmTemp[aDriftVehicle]>0) {
				Msg(aPlayers[i],COLOR_INFO2,"Wcisnij {b}2{/b} aby naprawic/obrocic swoj pojazd i wrocic do ostatniego CP na trasie!");
				SendClientMessage(aPlayers[i],0xffffffff,"");
			}
	}

	// TODO zrobic wiecej rekordow!
	format(buffer,sizeof buffer,"SELECT rr.finalscore,p.nick,DATE(rr.ts) kiedy FROM fs_drift_records rr JOIN fs_players p on p.id=rr.playerid where raceid=%d ORDER BY finalscore DESC,recordtime ASC LIMIT 1", gmTemp[aDriftArena]);
	mysql_query(buffer);
	mysql_store_result();
	format(buffer2,sizeof buffer2,"Brak rekordow na tej trasie");
	if (mysql_num_rows()==1 && mysql_fetch_row(buffer,"|")) {
		
		new recordscore;
		new recordowner[MAX_PLAYER_NAME];
		new recordts[11];
		if (!sscanf(buffer,"p<|>ds[25]s[11]", recordscore, recordowner, recordts)) {
			format(buffer2,sizeof buffer2, "{b}%d{b}p - %s, {b}%s{/b}", recordscore, recordowner, recordts);
			for(new i = 0; i < aMaxPlayers; i++) {
				Msg(aPlayers[i],COLOR_INFO,"Rekord trasy:");
				Msg(aPlayers[i],COLOR_INFO,buffer2);
			}
			format(buffer2,sizeof buffer2, "~n~Rekord trasy:~n~~n~~w~%s ~b~%s - ~y~%dp", recordts, recordowner, recordscore);
		}
	}
	mysql_free_result();
	format(buffer,sizeof buffer,"~n~~w~%s (%d)    ~y~%.1f~h~km~n~%s",gmTemp[aDriftDescr],gmTemp[aDriftArena],gmTemp[aDriftDistance], buffer2);

	TextDrawSetString(gTextDraw[TD_ADRIFTBOX], buffer);

    gmTemp[aDriftPlayers] = aPlayers;
	gmTemp[aDriftPlayersR] = aPlayersR;
    gmTemp[aDriftMaxPlayers] = aMaxPlayers;
    gmTemp[aDriftCount] = 5;
	KillTimer(gmTemp[timerDriftCountdown]);
    gmTemp[timerDriftCountdown] = SetTimer("Drift_Countdown", 700, true);
    gmTemp[aDriftStartTick] = GetTickCount();
	return 1;
}

Drift_Cleanup(){
	KillTimer(gmTemp[timerDriftCountdown]);
	for (new i=0;i<MAX_QUEUE_DRIFT;i++) {
		if (aDriftVehicles[i]>0 && aDriftVehicles[i]!=INVALID_VEHICLE_ID && gmTemp[aDriftVehicle]>0 && gmTemp[aDriftVehicle]==GetVehicleModel(aDriftVehicles[i]))
			DestroyVehicle(aDriftVehicles[i]);



	}
	for(new i=0; i<DRIFT_MAXCPS;i++) {
		if (IsValidDynamicRaceCP(aDriftCP[i]))
			DestroyDynamicRaceCP(aDriftCP[i]);
		if (IsValidDynamicMapIcon(aDriftMI[i]))
			DestroyDynamicMapIcon(aDriftMI[i]);
		aDriftCheckpointLastTime[i][edrlt_racerid]=INVALID_PLAYER_ID;
		aDriftCheckpointLastTime[i][edrlt_ts]=0;
	}

	for(new i = 0; i < gmTemp[aDriftMaxPlayers]; i++) {
		new playerid=gmTemp[aDriftPlayers][i];

		TextDrawHideForPlayer(playerid,Drift_TD[i]);
		TextDrawHideForPlayer(playerid,Drift_TDs[i]);

		TextDrawDestroy(Drift_TD[i]);
		TextDrawDestroy(Drift_TDs[i]);

		HideProgressBarForPlayer(playerid,Drift_bar[i]);
		DestroyProgressBar(Drift_bar[i]);

		if (pData[playerid][pAttraction]==A_DRIFT && GetPlayerVirtualWorld(playerid)==VW_DRIFT) {

			TextDrawHideForPlayer(playerid,pTextDraw[PTD_ADRIFTBOX][i]);
			TextDrawHideForPlayer(playerid,gTextDraw[TD_ADRIFTBOX]);

			if(pData[playerid][hudSetting][HUD_DATE]) ShowElement(playerid, TDE_DATETIME, true);
			if(pData[playerid][hudSetting][HUD_STATUSBAR]) ShowElement(playerid, TDE_STATS, true);
			if(pData[playerid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(playerid, TDE_ATTRACTIONBOX, true);
		
			pData[playerid][pAttraction] = A_NONE;
			SpawnPlayer(playerid);
		}
	}

	
	return;
}

public drift_RemovePlayer(playerid){
	if (pData[playerid][pAttraction]==A_DRIFT) {
		pData[playerid][pAttraction] = A_NONE;

		TextDrawHideForPlayer(playerid,pTextDraw[PTD_ADRIFTBOX][gmTemp[aDriftPlayersR][playerid]]);
		TextDrawHideForPlayer(playerid,gTextDraw[TD_ADRIFTBOX]);
		TextDrawHideForPlayer(playerid,Drift_TDs[gmTemp[aDriftPlayersR][playerid]]);
		TextDrawHideForPlayer(playerid,Drift_TD[gmTemp[aDriftPlayersR][playerid]]);
		TextDrawDestroy(Drift_TDs[gmTemp[aDriftPlayersR][playerid]]);
		TextDrawDestroy(Drift_TD[gmTemp[aDriftPlayersR][playerid]]);

		HideProgressBarForPlayer(playerid,Drift_bar[gmTemp[aDriftPlayersR][playerid]]);
		if(pData[playerid][hudSetting][HUD_DATE]) ShowElement(playerid, TDE_DATETIME, true);
		if(pData[playerid][hudSetting][HUD_STATUSBAR]) ShowElement(playerid, TDE_STATS, true);
		if(pData[playerid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(playerid, TDE_ATTRACTIONBOX, true);

		gmTemp[aDriftPlayers][gmTemp[aDriftPlayersR][playerid]]=INVALID_PLAYER_ID;
		gmTemp[aDriftPlayersR][playerid] = INVALID_PLAYER_ID;

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
	Drift_Recount();
	return 1;
}

Drift_Recount(){
	new pozostalo,lastPlayer=INVALID_PLAYER_ID;
//	printf("Drift recount");

	for (new i=0;i<gmTemp[aDriftMaxPlayers];i++) 
		if (gmTemp[aDriftPlayers][i]==INVALID_PLAYER_ID) continue;
		else if (pData[gmTemp[aDriftPlayers][i]][pAttraction]==A_DRIFT) {
			if (pTemp[gmTemp[aDriftPlayers][i]][staleTime]>5) {
				Msg(gmTemp[aDriftPlayers][i],COLOR_ERROR,"Zostales usuniety z wyscigu z powodu {b}AFK{/b}.");
				drift_RemovePlayer(gmTemp[aDriftPlayers][i]);
			}
			if (GetPlayerVirtualWorld(gmTemp[aDriftPlayers][i])==VW_DRIFT) {
				pozostalo++;
				lastPlayer=gmTemp[aDriftPlayers][i];
			} else
				drift_RemovePlayer(gmTemp[aDriftPlayers][i]);
		}

	if (pozostalo<=0) {
		aData[A_DRIFT][aState] = A_STATE_OFF;
		Drift_Cleanup();
		if (lastPlayer!=INVALID_PLAYER_ID) {
			pData[lastPlayer][pAttraction]=A_NONE;
			SetPlayerVirtualWorld(lastPlayer,0);
			SetPlayerInterior(lastPlayer,0);
			SpawnPlayer(lastPlayer);
//			new str[127];
//			format(str,sizeof(str),"Drift wygrywa %s. Otrzymuje %d respektu.",GetPlayerProperName(lastPlayer),5+gmTemp[aDriftMaxPlayers]);
//			GivePlayerScore(lastPlayer,5+gmTemp[aDriftMaxPlayers],"wygrany wyscig");
//			SendClientMessageToAll(0xff0000ff,str);
		}
	}
			
	return 1;
}

forward Drift_Countdown();
public Drift_Countdown()
{
	new
	 buffer[255];
	
	if(gmTemp[aDriftCount] <= 0)
	{
		format(buffer, sizeof buffer, "~g~GO!");
		KillTimer(gmTemp[timerDriftCountdown]);
		drift_loop(0);	// glowna petla driftu!
	}
	else
	{
		format(buffer, sizeof buffer, "~r~%i", gmTemp[aDriftCount]);
	}


	
	for(new playerid = 0; playerid < gmTemp[aDriftMaxPlayers]; playerid++)
	{
		GameTextForPlayer(gmTemp[aDriftPlayers][playerid], buffer, 1000, 6);
		
		switch(gmTemp[aDriftCount]){
			
			case 0: {
				PlaySound(gmTemp[aDriftPlayers][playerid], 1057);
				TogglePlayerControllable(gmTemp[aDriftPlayers][playerid], true);
				SetCameraBehindPlayer(gmTemp[aDriftPlayers][playerid]);
			}
			default: {
				TogglePlayerControllable(gmTemp[aDriftPlayers][playerid], true);
				Streamer_Update(gmTemp[aDriftPlayers][playerid]);
				TogglePlayerControllable(gmTemp[aDriftPlayers][playerid], false);
				PlaySound(gmTemp[aDriftPlayers][playerid], 1056);

				SetPlayerCameraPos(gmTemp[aDriftPlayers][playerid],aDriftCameraPos[X],aDriftCameraPos[Y],aDriftCameraPos[Z]);
				SetPlayerCameraLookAt(gmTemp[aDriftPlayers][playerid],aDriftVehiclesPos[playerid][X], aDriftVehiclesPos[playerid][Y], aDriftVehiclesPos[playerid][Z]);
				
				Drift_RefreshPTD(playerid);

				TextDrawShowForPlayer(gmTemp[aDriftPlayers][playerid],pTextDraw[PTD_ADRIFTBOX][playerid]);
				TextDrawShowForPlayer(gmTemp[aDriftPlayers][playerid],gTextDraw[TD_ADRIFTBOX]);
				if(pData[gmTemp[aDriftPlayers][playerid]][hudSetting][HUD_DATE]) ShowElement(gmTemp[aDriftPlayers][playerid], TDE_DATETIME, false);
				if(pData[gmTemp[aDriftPlayers][playerid]][hudSetting][HUD_STATUSBAR]) ShowElement(gmTemp[aDriftPlayers][playerid], TDE_STATS, false);
				if(pData[gmTemp[aDriftPlayers][playerid]][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(gmTemp[aDriftPlayers][playerid], TDE_ATTRACTIONBOX, false);

				if (gmTemp[aDriftVehicle]>0) {
					SetVehiclePos(aDriftVehicles[playerid],aDriftVehiclesPos[playerid][X],aDriftVehiclesPos[playerid][Y],aDriftVehiclesPos[playerid][Z]);
					SetVehicleZAngle(aDriftVehicles[playerid],aDriftVehiclesPos[playerid][A]);
				} else {
					SetPlayerPos(gmTemp[aDriftPlayers][playerid], aDriftVehiclesPos[playerid][X],aDriftVehiclesPos[playerid][Y],aDriftVehiclesPos[playerid][Z]);
					SetPlayerFacingAngle(gmTemp[aDriftPlayers][playerid], aDriftVehiclesPos[playerid][A]);
				}

			}
/*			default: {
				PlaySound(gmTemp[aDriftPlayers][playerid], 1056);
			}*/
		}
	}
	
	gmTemp[aDriftCount]--;
}

drift_OPEDRCP(playerid, checkpointid){
	new rpid=gmTemp[aDriftPlayersR][playerid];

	if (rpid==INVALID_PLAYER_ID) return 0;
	for (new i=((aDriftCheckpoint[rpid]<0)?(0):(aDriftCheckpoint[rpid])); i<gmTemp[aDriftNumCP]; i++) 
		if (aDriftCP[i]==checkpointid && gmTemp[aDriftNumCP]>(aDriftCheckpoint[rpid]+1)) {

			aDriftCheckpoint[rpid]=i;
			aDriftCPET[rpid]=GetTickCount();

/*			if (aDriftCheckpointLastTime[i][edrlt_racerid]!=rpid && aDriftCheckpointLastTime[i][edrlt_racerid]!=INVALID_PLAYER_ID && aDriftCheckpointLastTime[i][edrlt_ts]>0) {
				// mamy informacje o czasie odwiedzenia tego CP przez poprzedniego uczestnika wyscigu
				new str[32];
				new Float:roznica=float(aDriftCheckpointLastTime[i][edrlt_ts]-aDriftCPET[rpid]);
//				printf(" %d %d %.2f ", aDriftCheckpointLastTime[i][edrlt_ts], aDriftCPET[rpid], roznica/1000);

//				format(str, sizeof str, "_~n~_~n~_~n~%.3f ~g~sek.", -1*(roznica/1000));
//				GameTextForPlayer(gmTemp[aDriftPlayers][aDriftCheckpointLastTime[i][erclt_racerid]] , str,600,6);

				format(str, sizeof str, "_~n~_~n~%.3f ~b~sek.", -1*roznica/1000);
				GameTextForPlayer(playerid, str,1000,6);
				// TODO: powyzsze wyniki sa prawidlowe, jesli roznica odleglosci pomiedzy graczami wynosi dokladnie 1CP.
				// dopisac petle ktora zsumuje roznice czasowe dla wszystkich CP!
				// edit: da sie chyba bez petli
			}*/

			aDriftCheckpointLastTime[i][edrlt_racerid]=rpid;
			aDriftCheckpointLastTime[i][edrlt_ts]=aDriftCPET[rpid];
			
			PlaySound(playerid, 1056);


			if ((i+1)==gmTemp[aDriftNumCP]) { 
					drift_DropCamera(rpid);

					// obliczamy premie/kare za czas i wyswietlamy podsumowanie
					new podsumowanie[1023],ptime;
					ptime=floatround(float(aDriftCPET[rpid]-gmTemp[aDriftStartTick])/1000);
					format(podsumowanie, sizeof podsumowanie, "~y~Twoj czas: %02d:%02d~n~~h~czas trasy: %02d:%02d~n~", ptime/60,ptime%60, gmTemp[aDriftParTime]/60, gmTemp[aDriftParTime]%60);
					new roznica=(gmTemp[aDriftParTime]-ptime)*100;	// w sekunde mozna na drifcie zarobic maksymalnie 100 punktow wiec jest to odpowiednia wartosc
					if (roznica>0) roznica/=10;
					if (roznica>0)	format(podsumowanie, sizeof podsumowanie, "%s~w~bonus za czas: ~g~+%d~y~p", podsumowanie, roznica);
					else if (roznica<0)	format(podsumowanie, sizeof podsumowanie," %s~w~kara za czas: ~r~%d~y~p", podsumowanie, roznica);
					aDriftScore[rpid]+=roznica;
					if (aDriftScore[rpid]<0)	aDriftScore[rpid]=0;
					format(podsumowanie, sizeof podsumowanie,"%s~n~_ ~n~Wynik: ~g~%dp",podsumowanie, aDriftScore[rpid]);
					Drift_RefreshTD();
					Drift_RefreshPTD(rpid);
					GameTextForPlayer(playerid, podsumowanie, 6000,0);
					format(podsumowanie,sizeof podsumowanie,"Twoj czas: {b}%02d:%02d{/b}. Twoj wynik: {b}%d{/b}p.", ptime/60,ptime%60, aDriftScore[rpid]);
					Msg(playerid,COLOR_INFO,podsumowanie);

//					if (gmTemp[aDriftMaxPlayers]==1)	// sam jechal
					GivePlayerScore(playerid,2,"wygrany drifting");
//					else
//						GivePlayer
//						GivePlayerScore(playerid,floatround((5+gmTemp[aDriftMaxPlayers])/aDriftPosition[rpid])+1,"wygrany wyscig");

//					avt_record(playerid,e_driftdist,floatround(gmTemp[aDriftDistance]),ART_ADD);
//					avt_record(playerid,e_racefcnt,1,ART_ADD);

					SetTimerEx("drift_RemovePlayer",3500, false, "d", playerid); 
					drift_RecordResult(rpid);
					return 1; 
					}

			Drift_RefreshTD();
			Drift_RefreshPTD(rpid);


			if (gmTemp[aDriftVehicle]>0)
				GetVehicleZAngle(aDriftVehicles[rpid],aDriftLastCPZAngle[rpid]);
			else
				GetPlayerFacingAngle(playerid,aDriftLastCPZAngle[rpid]);



			TogglePlayerDynamicRaceCP(playerid, checkpointid,0);
			TogglePlayerDynamicRaceCP(playerid, aDriftCP[i+1],1);
//			Streamer_UpdateEx(playerid, aDriftCPV[i][X],aDriftCPV[i][Y],aDriftCPV[i][Z]);

			Streamer_RemoveArrayData(STREAMER_TYPE_MAP_ICON, aDriftMI[i], E_STREAMER_PLAYER_ID, playerid);
//			TogglePlayerDynamicRaceCP(playerid, aDriftCP[i+2],1);



			return 1;
		}
	return 0;
}

drift_Napraw(playerid){
	if (gmTemp[aDriftVehicle]==0) return 1;
	new rpid=gmTemp[aDriftPlayersR][playerid];
	new rcpv=aDriftCheckpoint[rpid];

	DriftPoints[rpid]=0;
	
	if (rcpv<0) {	// nie dojechal jeszcze do pierwszego CP
		PlaySound(playerid, 1056);
		SetVehiclePos(aDriftVehicles[rpid],aDriftVehiclesPos[rpid][X],aDriftVehiclesPos[rpid][Y],aDriftVehiclesPos[rpid][Z]);
		SetVehicleZAngle(aDriftVehicles[rpid],aDriftVehiclesPos[rpid][A]);
		RepairVehicle(aDriftVehicles[gmTemp[aDriftPlayersR][playerid]]);
	} else if (rcpv>=0 && rcpv<sizeof aDriftCP) {
		aDriftCPET[rpid]=GetTickCount();	// gracz cofa sie w wyscigu!

		PlaySound(playerid, 1056);
		SetVehiclePos(aDriftVehicles[gmTemp[aDriftPlayersR][playerid]],aDriftCPV[rcpv][X],aDriftCPV[rcpv][Y],aDriftCPV[rcpv][Z]+1.5);
		SetVehicleZAngle(aDriftVehicles[rpid],aDriftLastCPZAngle[rpid]+random(10)-5);
		RepairVehicle(aDriftVehicles[gmTemp[aDriftPlayersR][playerid]]);
	}
	return 1;
}

Drift_RefreshTD(){
	//printf("RECHECKING");
/*	new pozycja=1;
	if (gmTemp[aDriftMaxPlayers]==1)	// tylko jeden scigajacy sie
		aDriftPosition[0]=pozycja++;
	else
		for(new i=gmTemp[aDriftNumCP]; i>0; i--) {
			new wtymcp[MAX_QUEUE_DRIFT]={-1,...};
			new i3=0;
			for(new i2=0; i2<gmTemp[aDriftMaxPlayers]; i2++)
				if (aDriftCheckpoint[i2]==(i-1)) wtymcp[i3++]=i2;
		
			if (i3==1)	// jeden pojazd w tym CP
				aDriftPosition[wtymcp[0]]=pozycja++;
			else if (i3>1) { // kilka pojazdow w tym CP
				new i4=i3;
				while (i4>0) {
//					print("i4: %d");
					new najszybszy=-1;
					for(new i5=0;i5<i4;i5++)
						if (wtymcp[i5]==-1) continue;
						else if (najszybszy==-1) najszybszy=i5;
						else if (aDriftCPET[wtymcp[najszybszy]]>aDriftCPET[wtymcp[i5]]) najszybszy=i5;
					if (najszybszy==-1) break;	// nie powinno sie nigdy zdarzyc, ale robimy na wypadek wystapienia petli
//					printf("najszybszy %d -> %d",najszybszy, wtymcp[najszybszy]);
					aDriftPosition[wtymcp[najszybszy]]=pozycja++;
					wtymcp[najszybszy]=-1;
					i4--;
				}
			}
		}




	for(new npozycja=1; npozycja<=(pozycja>6?6:pozycja); npozycja++)*/

	new buffer[1023],npozycja=1;
	format(buffer,sizeof buffer,"~n~~w~%s (%d)    ~y~%.1f~h~km~n~~b~",gmTemp[aDriftDescr],gmTemp[aDriftArena],gmTemp[aDriftDistance]);
	for(new i=0;i<gmTemp[aDriftMaxPlayers];i++)
		if (gmTemp[aDriftPlayers][i]!=INVALID_PLAYER_ID) {
			format(buffer,sizeof buffer,"%s~w~%d.~b~ %s ~h~%d~n~",buffer, npozycja++, GetPlayerProperName(gmTemp[aDriftPlayers][i]), aDriftScore[i]);
			Drift_RefreshPTD(i);
		}

	TextDrawSetString(gTextDraw[TD_ADRIFTBOX], buffer);

}

Drift_RefreshPTD(rpid){
	new str[64];
	format(str,sizeof str, "~b~%d~h~~h~p~b~    ~h~%d/%d", aDriftScore[rpid], aDriftCheckpoint[rpid]+1, gmTemp[aDriftNumCP]);
	TextDrawSetString(pTextDraw[PTD_ADRIFTBOX][rpid], str);
}

drift_DropCamera(rpid){
	new Float:PF[6];

	if (gmTemp[aDriftVehicle]>0) {
		GetVehiclePos(aDriftVehicles[rpid],PF[0],PF[1],PF[2]);
		GetVehicleVelocity(aDriftVehicles[rpid],PF[3],PF[4],PF[5]);
	} else {
		GetPlayerPos(gmTemp[aDriftPlayers][rpid],PF[0],PF[1],PF[2]);
		GetPlayerVelocity(gmTemp[aDriftPlayers][rpid],PF[3],PF[4],PF[5]);
	}
	PF[3]=-1*PF[3]*3+PF[0];
	PF[4]=-1*PF[4]*3+PF[1];
	PF[5]=-1*PF[5]*3+PF[2]+random(2)-1;

	SetPlayerCameraPos(gmTemp[aDriftPlayers][rpid],PF[3],PF[4],PF[5]);
	SetPlayerCameraLookAt(gmTemp[aDriftPlayers][rpid],PF[0],PF[1],PF[2]);
//	TogglePlayerControllable(gmTemp[aDriftPlayers][rpid],false);
//	race_CameraFall(rpid, PF[3],PF[4],PF[5],1);
}

drift_RecordResult(rpid){
	if (pData[gmTemp[aDriftPlayers][rpid]][accountID]>0) {
		new buf[256];
		format(buf,sizeof buf,"INSERT INTO fs_drift_records SET playerid=%d,raceid=%d,ts=NOW(),recordtime=%.2f,opponents=%d,finalscore=%d",
			pData[gmTemp[aDriftPlayers][rpid]][accountID], gmTemp[aDriftArena], float(aDriftCPET[rpid]-gmTemp[aDriftStartTick])/1000, gmTemp[aDriftMaxPlayers], aDriftScore[rpid]);
		mysql_query(buf);
		avt_record(gmTemp[aDriftPlayers][rpid],e_driftpts,aDriftScore[rpid],ART_ADD);
	}
	return;
}


public drift_loop(step){
	static Float:VA,Float:VV[3],Float:SPD;
	static Float:VC,Float:VD,Float:vhp;
//	static gstr[16];

	if (step++>10) step=0;

	for(new i = 0; i < gmTemp[aDriftMaxPlayers]; i++) {
		new PP=gmTemp[aDriftPlayers][i];
		if (PP==INVALID_PLAYER_ID) continue;
		if (gmTemp[aDriftNumCP]==(aDriftCheckpoint[i]+1)) continue;	// dojechal do ostatniego CP!

		if (aDriftVehicles[i]==0 || aDriftVehicles[i]==INVALID_VEHICLE_ID) continue;
		
		if (step%2) {
			GetVehicleHealth(aDriftVehicles[i],vhp);	// uderzenie = zerowanie punktow
			if (vhp<Drift_lastvhp[i]) DriftPoints[i]=0;
			Drift_lastvhp[i]=vhp;
		}

		if (step%10==0) {
			TextDrawColor(Drift_TDs[i], 0xFFFFFFFF);
			if (DriftPoints[i]>=5) {
				format(gstr,sizeof gstr,"+%d", floatround(DriftPoints[i]/5)*5);
				TextDrawSetString(Drift_TDs[i],gstr);
				aDriftScore[i]+=floatround(DriftPoints[i]/5)*5;
				Drift_RefreshPTD(i);
			} else 
				TextDrawSetString(Drift_TDs[i],"");
		} else { 
			TextDrawColor(Drift_TDs[i], 0xFFFFFFFF - (step%10*24));
		}
		TextDrawShowForPlayer(PP,Drift_TDs[i]);

		GetVehicleZAngle(aDriftVehicles[i],VA);
		GetVehicleVelocity(aDriftVehicles[i],VV[0],VV[1],VV[2]);
		SetProgressBarValue(Drift_bar[i], DriftPoints[i]);
		UpdateProgressBar(Drift_bar[i], PP);
		SPD=(VV[0]*VV[0])*(VV[1]*VV[1])*200;

		if ((floatabs(VV[0])<0.05 && floatabs(VV[1])<0.05)) {
				if (DriftPoints[i]>=1) DriftPoints[i]-=1.0;
				TextDrawHideForPlayer(PP,Drift_TD[i]);
				continue;
		}
		new Float:DP[2];

		if (VV[1]==0) 
			VV[1]+=0.001;
		VC=atan(-1*VV[0]/VV[1]);
		if (VV[1]<0)	VC+=180.0;

		VC=floatround(VC)%360;
		VD=floatabs(VA-VC);

		GetDotXY(DRIFT_TD_x,DRIFT_TD_y,DP[0],DP[1],(VA-VC),64);
		TextDrawDestroy(Drift_TD[i]);
		Drift_TD[i]=TextDrawCreate(DP[0], DP[1], 
			VD<3 ? ("~g~.") : ( VD>70 ? ("~r~.") : ("~y~.") )
		);
		TextDrawLetterSize(Drift_TD[i], 0.73, -2.60);
		TextDrawSetOutline(Drift_TD[i], 0);
		TextDrawSetShadow(Drift_TD[i], 1);

		TextDrawShowForPlayer(PP,Drift_TD[i]);

		if (DriftPoints[i]>=1) DriftPoints[i]-=1.0;

		if (VD>=100) {
			// za bardzo obrocil lub jedzie do tylu
			if (DriftPoints[i]>10) DriftPoints[i]-=2.0;
			else if (DriftPoints[i]>=1) DriftPoints[i]-=1.0;
			continue;
		}
	    if (VD<4) continue;
		if (VD>20) {
//		GameTextForPlayer(PP,"~g~drift+",100,3);
			DriftPoints[i]+=3.0*SPD;
		} else {
//		GameTextForPlayer(PP,"~g~~h~drift",100,3);
			DriftPoints[i]+=2.0*SPD;
		}
		if (DriftPoints[i]>100.0) DriftPoints[i]=100.0;
	}
	
	if (aData[A_DRIFT][aState] == A_STATE_ON)
		SetTimer("drift_loop",100,false);
	return 1;
}