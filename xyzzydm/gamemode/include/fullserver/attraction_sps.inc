#define SPS_OX	(1882.0)
#define	SPS_OY	(-1395.0)
#define	SPS_OZ	(500.0)

enum e_HayPiece {
	ehp_object,
	bool:ehp_active
}
new HayArray[10][10][e_HayPiece];
new LeftHayCount;

CMD:sps(playerid)
{
//	Msg(playerid,COLOR_ERROR,"Atrakcja jest obecnie wylaczona przez administratora");
//	return 1;
	MUST_BE_SPAWNED(playerid);
	if(pData[playerid][aSPS]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 253));
	if(GetAttractionPlayerCount(A_SPS) > MAX_QUEUE_SPS) return Msg(playerid, COLOR_ERROR, TXT(playerid, 423));
	
	pData[playerid][aSPS] = true;
	Msg(playerid, COLOR_INFO, TXT(playerid, 261));

	return 1;
}

CMD:spsexit(playerid){
	if (pData[playerid][pAttraction]==A_SPS) {
		pData[playerid][pAttraction] = A_NONE;
		SpawnPlayer(playerid);
		Msg(playerid,COLOR_INFO,"Opusciles SPS.");
	} else if (pData[playerid][aSPS]) {
		pData[playerid][aSPS]=false;	// zapisy
		Msg(playerid,COLOR_INFO,"Zrezygnowales/as z zapisow na /sps");
	}
	return 1;
}

SPS_Cleanup()
{
	for (new x=0;x<10;x++)
		for (new y=0;y<10;y++)
			if (HayArray[x][y][ehp_active])
				DestroyDynamicObject(HayArray[x][y][ehp_object]);
	LeftHayCount=0;
}


SPS_Start()
{
	new
	 aPlayers[MAX_QUEUE_SPS],
	 aMaxPlayers = 0;

	foreach(playerid)
	{
		if(pData[playerid][aSPS])
		{
			if(AttractionAllowed(playerid))
			{
				aPlayers[aMaxPlayers++] = playerid;
				if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
			}
			else
			{
				pData[playerid][aSPS] = false;
				Msg(playerid, COLOR_ERROR, TXT(playerid, 285));
			}
		}
	}

	if(aMaxPlayers < 1)
	{
		foreach(playerid)
			Msg(playerid, COLOR_INFO3, TXT(playerid, 270));
		aData[A_SPS][aState] = A_STATE_OFF;
		return;
	}
	SPS_Cleanup();

	for (new x=0;x<10;x++)
		for (new y=0;y<10;y++) {
			HayArray[x][y][ehp_active]=true;
			HayArray[x][y][ehp_object]=CreateDynamicObject(3374,SPS_OX+(x*4),SPS_OY+(y*4),SPS_OZ,	floatsin(y)*5,0,(floatsin(y)*floatcos(x)*90),	VW_SPS,0,-1,150);
		}
	LeftHayCount=100;

	for(new i = 0; i < aMaxPlayers; i++)
	if (pTemp[aPlayers[i]][staleTime]<5) {
		pData[aPlayers[i]][pAttraction] = A_SPS;
		pData[aPlayers[i]][aSPS] = false;
		new x=random(10);
		new y=random(10);

		Streamer_UpdateEx(aPlayers[i],SPS_OX+(x*4),SPS_OY+(y*4),SPS_OZ+2.0);
		TogglePlayerControllable(aPlayers[i], false);
		ResetPlayerWeapons(aPlayers[i]);
		GivePlayerWeapon(aPlayers[i],46,1);
		
		Teleport(T_PLAYER, aPlayers[i], SPS_OX+(x*4),SPS_OY+(y*4),SPS_OZ+2.0, (floatsin(y)*floatcos(x)*90), 0, VW_SPS);
		Streamer_Update(aPlayers[i]);
	}

	gmTemp[aSPSPlayers] = aPlayers;
	gmTemp[aSPSMaxPlayers] = aMaxPlayers;
	gmTemp[aSPSCount] = 5;
	gmTemp[timerSPSCountdown] = SetTimer("SPS_Countdown", 700, true);
	gmTemp[aSPSStartTick] = GetTickCount();
}

forward SPS_Countdown();
public SPS_Countdown()
{
	new
	 buffer[64];
	
	if(gmTemp[aSPSCount] <= 0)
	{
		format(buffer, sizeof buffer, "~g~GO!");
		KillTimer(gmTemp[timerSPSCountdown]);
		SetTimer("HayMovement",1024,false);
	}
	else
	{
		format(buffer, sizeof buffer, "~r~%i", gmTemp[aSPSCount]);
	}
	
	for(new playerid = 0; playerid < gmTemp[aSPSMaxPlayers]; playerid++)
	{
		GameTextForPlayer(gmTemp[aSPSPlayers][playerid], buffer, 1000, 6);
		
		if(gmTemp[aSPSCount] == 0)
		{
			PlaySound(gmTemp[aSPSPlayers][playerid], 1057);
			TogglePlayerControllable(gmTemp[aSPSPlayers][playerid], true);
		}
		else
		{
			PlaySound(gmTemp[aSPSPlayers][playerid], 1056);
		}
	}
	
	gmTemp[aSPSCount]--;
}

SPS_Update()
{
//	printf("SPS UPDATE");
	if (aData[A_SPS][aState] != A_STATE_ON || gmTemp[aSPSCount]>0)
		return;
//	printf("SPS dalej");
	new activePlayers=0,lastPlayer=INVALID_PLAYER_ID;
	for(new i = 0; i < gmTemp[aSPSMaxPlayers]; i++)
		if (gmTemp[aSPSPlayers][i]!=INVALID_PLAYER_ID) {
			if (!IsPlayerConnected(gmTemp[aSPSPlayers][i]) || GetPlayerVirtualWorld(gmTemp[aSPSPlayers][i])!=VW_SPS || pTemp[gmTemp[aSPSPlayers][i]][staleTime]>5) {
				SetPlayerVirtualWorld(gmTemp[aSPSPlayers][i],0);
				pData[gmTemp[aSPSPlayers][i]][pAttraction] = A_NONE;
				gmTemp[aSPSPlayers][i]=INVALID_PLAYER_ID;
				continue;
			}
			new Float:PP[3];
			GetPlayerPos(gmTemp[aSPSPlayers][i],PP[0],PP[1],PP[2]);
			if(floatabs(PP[2]-SPS_OZ)>50) {
				Msg(gmTemp[aSPSPlayers][i],COLOR_INFO,"Opuszczasz SPS");
				SetPlayerVirtualWorld(gmTemp[aSPSPlayers][i],0);
				pData[gmTemp[aSPSPlayers][i]][pAttraction] = A_NONE;
				gmTemp[aSPSPlayers][i]=INVALID_PLAYER_ID;
				continue;
			}
			if (++activePlayers==1) lastPlayer=i;
		}
//	printf("ActivePlayers %d lastPlayer %d",activePlayers,lastPlayer);
	if (activePlayers==1) {
		new resp=5+floatround(gmTemp[aSPSMaxPlayers]/2);
		format(gstr,sizeof gstr, " *** SPS wygrywa, jako ostatni%s - {ffff00}%s{ffffff} - otrzymuje %d %s.", 
			Kobieta(gmTemp[aSPSPlayers][lastPlayer])?("a"):(""), GetPlayerProperName(gmTemp[aSPSPlayers][lastPlayer]), resp, dli(resp,"punkt respektu","punkty respektu","punktow respektu"));
		SendClientMessageToAll(-1,gstr);

		GivePlayerScore(gmTemp[aSPSPlayers][lastPlayer], resp, "SPS");

		pData[gmTemp[aSPSPlayers][lastPlayer]][pAttraction] = A_NONE;
		SpawnPlayer(gmTemp[aSPSPlayers][lastPlayer]);
		gmTemp[aSPSPlayers][lastPlayer]=INVALID_PLAYER_ID;
		aData[A_SPS][aState]=A_STATE_OFF;
		return;
	}
	if (activePlayers==0) {
//		SendClientMessageToAll(-1,"Konczenie SPS");
		aData[A_SPS][aState]=A_STATE_OFF;
	}
	return;
}

SPS_Finish(){
	aData[A_SPS][aState]=A_STATE_OFF;
	for(new i = 0; i < gmTemp[aSPSMaxPlayers]; i++)
		if (gmTemp[aSPSPlayers][i]!=INVALID_PLAYER_ID) {
			SetPlayerVirtualWorld(gmTemp[aSPSPlayers][i],0);
			pData[gmTemp[aSPSPlayers][i]][pAttraction] = A_NONE;
			gmTemp[aSPSPlayers][i]=INVALID_PLAYER_ID;
		}
	return;
}

forward HayMovement();
public HayMovement(){
	if (aData[A_SPS][aState] != A_STATE_ON || gmTemp[aSPSCount]>0)
		return;
	new mtime,ttime,rh,rh2,hm,hi;
	rh=random(LeftHayCount);
	rh2=random(LeftHayCount);
	LeftHayCount=0;
	hm=random(6);

	for (new x=0;x<10;x++)
	 for (new y=0;y<10;y++)
		if (HayArray[x][y][ehp_active]) {
			if (rh==hi++ || rh2==hi) {
				HayArray[x][y][ehp_active]=false;
				SetTimerEx("SPS_DestroyHayBlock",MoveDynamicObject(HayArray[x][y][ehp_object],SPS_OX+(x*4),SPS_OY+(y*4),SPS_OZ-100,	5),false,"dd",x,y);
			} else {
				LeftHayCount++;

				switch(hm){
					case 0: ttime=MoveDynamicObject(HayArray[x][y][ehp_object],SPS_OX+(x*4),SPS_OY+(y*4),SPS_OZ,	0.5);
					case 1: ttime=MoveDynamicObject(HayArray[x][y][ehp_object],SPS_OX+(x*4),SPS_OY+(y*4),SPS_OZ+floatsin(y)+floatcos(x),	0.5);
					case 2: ttime=MoveDynamicObject(HayArray[x][y][ehp_object],SPS_OX+(x*4),SPS_OY+(y*4),SPS_OZ+floatcos(y)+floatsin(x),	0.5);
					case 3: ttime=MoveDynamicObject(HayArray[x][y][ehp_object],SPS_OX+(x*4),SPS_OY+(y*4),SPS_OZ+floatsin(y),	0.5);
					case 4: ttime=MoveDynamicObject(HayArray[x][y][ehp_object],SPS_OX+(x*4),SPS_OY+(y*4),SPS_OZ+floatcos(x),	0.5);
					case 5: ttime=MoveDynamicObject(HayArray[x][y][ehp_object],SPS_OX+(x*4),SPS_OY+(y*4),SPS_OZ+(floatcos(x)*floatsin(y)),	0.5);
				}
				if (mtime<ttime) mtime=ttime;
			}
		}
	if (mtime==0) mtime=random(500)+500;
	if (LeftHayCount>0)
		SetTimer("HayMovement",mtime,false);
}

forward SPS_DestroyHayBlock(x,y);
public SPS_DestroyHayBlock(x,y){
	DestroyDynamicObject(HayArray[x][y][ehp_object]);
}