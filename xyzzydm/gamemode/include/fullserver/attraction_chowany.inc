forward chowany_Countdown();

new Float:aCHWBCube[6],
    	Float:aCHWBSphere[4],
		CHPozaStrefa[MAX_QUEUE_CHOWANY]={10,...},
		aCHWBmode,
		aCH_lastHidding,aCH_lastSearching;

Chowany_Start()
{
	new
	 aPlayers[MAX_QUEUE_CHOWANY],
	 aMaxPlayers = 0,
	 buffer[512],
	 minSearchers,
	 tmpMaxSearchers = 0;
	
	new	wb_sphere[80],
		wb_cube[80];

	foreach(playerid)
	{
		if(pData[playerid][aChowany])
		{
			if(AttractionAllowed(playerid))
			{
				aPlayers[aMaxPlayers++] = playerid;
				if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
			}
			else
			{
				pData[playerid][aChowany] = false;
				Msg(playerid, COLOR_ERROR, TXT(playerid, 285));
			}
		}
	}
	
	if(aMaxPlayers < 3)
	{
		foreach(playerid)
		{
			Msg(playerid, COLOR_INFO3, TXT(playerid, 269));
		}
		
		aData[A_CHOWANY][aState] = A_STATE_OFF;
		
		return 0;
	}
	
	if (gmTemp[aChowanyForceArena]>0) {
	    format(buffer,sizeof buffer,"select id,descr,interior,IFNULL(wb_sphere,'-'),IFNULL(wb_cube,'-'),(wb_mode='oraz') from fs_chowany_arena where id=%d LIMIT 1;",gmTemp[aChowanyForceArena]);
		gmTemp[aChowanyForceArena]=0;
	} else
	    format(buffer,sizeof buffer,"select id,descr,interior,IFNULL(wb_sphere,'-'),IFNULL(wb_cube,'-'),(wb_mode='oraz') from fs_chowany_arena where minplayers<=%d AND %d<=maxplayers ORDER by rand() LIMIT 1;",aMaxPlayers,aMaxPlayers);
    mysql_query(buffer);
    mysql_store_result();
    if (!mysql_num_rows()) {    // nie znaleziono odpowiedniej areny
        SendClientMessageToAll(0xffffffff,"Chowany nie wystartowal - nie znaleziono odpowiedniej areny");
		
		CH_Finish();
		if (mysql_result_stored()) 	mysql_free_result();
        return 0;
    }

    mysql_fetch_row(buffer,"|");
	mysql_free_result();
	
    if (sscanf(buffer,"p<|>ds[64]ds[64]s[64]d",gmTemp[aChowanyArena],gmTemp[aChowanyDescr],gmTemp[aChowanyInterior], wb_sphere,wb_cube,aCHWBmode)) {
        SendClientMessageToAll(0xffffffff,"Chowany nie wystartowal - nie znaleziono odpowiedniej areny");
		CH_Finish();
        return 0;
	}

	aCHWBSphere[0]=FLOAT_NAN; 
	sscanf(wb_sphere,"p<,>ffff", aCHWBSphere[0],aCHWBSphere[1],aCHWBSphere[2],aCHWBSphere[3]);


	aCHWBCube[0]=FLOAT_NAN;
	sscanf(wb_cube,"p<,>ffffff", aCHWBCube[0],aCHWBCube[1],aCHWBCube[2],aCHWBCube[3],aCHWBCube[4],aCHWBCube[5]);
	
	switch(aMaxPlayers)
	{
		case 0..4: 		minSearchers = 1;
		case 5..8: 		minSearchers = 2;
		case 9..13: 	minSearchers = 3;
		case 14..17: 	minSearchers = 4;
		case 18..21:	minSearchers = 5;
		case 22..24:	minSearchers = 6;
		default: 		minSearchers = 8;
	}
		
	while(tmpMaxSearchers < minSearchers)
	{
		for(new i = 0; i < aMaxPlayers; i++)
		if (pData[aPlayers[i]][aChowany])
		{
			if(random(2) == 0 && tmpMaxSearchers < minSearchers)
			{
				pTemp[aPlayers[i]][aChowanySide] = A_CHOWANY_SEARCHING;
				tmpMaxSearchers++;
//				SendClientMessageToAll(0xFFFFFFFF, "start: s++");
			}
			else
			{
				pTemp[aPlayers[i]][aChowanySide] = A_CHOWANY_HIDDING;
//				SendClientMessageToAll(0xFFFFFFFF, "start: h++ %d - %d", i, aPlayers[i]);
			}
		}
	}

	// pobieramy chowajacych sie
	format(buffer,sizeof buffer,"SELECT X,Y,Z,A FROM fs_chowany_arena_sp WHERE aid=%d AND team=0 ORDER BY RAND() LIMIT %d", gmTemp[aChowanyArena], aMaxPlayers);
		// pobiera kilku wiecej, ale ciul w to
	mysql_query(buffer);
	mysql_store_result();
	if (mysql_num_rows()<aMaxPlayers) {
		mysql_free_result();
		SendClientMessageToAll(-1,"Chowany nie wystartowal - nie znaleziono odpowiedniej areny");
		CH_Finish();
		return 0;
	}


	new p=0,pc=0;
	while (p<aMaxPlayers) {
//	printf("P1: p<a %d<%d", p, aMaxPlayers);
	if (pData[aPlayers[p]][aChowany] && pTemp[aPlayers[p]][aChowanySide] == A_CHOWANY_HIDDING) {
		if (!mysql_fetch_row(buffer,"|")) {
			mysql_free_result();
			SendClientMessageToAll(-1,"Chowany nie wystartowal - nie znaleziono odpowiedniej areny");
			CH_Finish();
			return 0;
		}
		
//		printf("Ustawianie gracza %d jako chowajacego sie, %d/%d", aPlayers[p], p, aMaxPlayers);

		new Float:PX,Float:PY,Float:PZ,Float:PA;
		if (sscanf(buffer,"p<|>ffff", PX, PY, PZ, PA)) 
			continue;
		

		pData[aPlayers[p]][pAttraction] = A_CHOWANY;
		pData[aPlayers[p]][aChowany] = false;
		pTemp[aPlayers[p]][aCHDead]=false;
		CHPozaStrefa[p]=10;

		SetPlayerArmour(aPlayers[p], 0.0);
		SetPlayerHealth(aPlayers[p], 99.0);	// specjalnie dajemy 99 zeby mozna bylo szybko wykryc czity na /chowanym!

		SetPlayerColor(aPlayers[p],pData[aPlayers[p]][currentColor]*256+0);	// chowajacy sie niewidoczny przed startem

		ResetPlayerWeapons(aPlayers[p]);
		
		Teleport(T_PLAYER, aPlayers[p], PX,PY,PZ,PA, gmTemp[aChowanyInterior], VW_CHOWANY,true);
		
//		TextDrawSetString(pTextDraw[PTD_INFOBOX][aPlayers[p]], "Chowasz sie...");
//		ShowElement(aPlayers[p], TDE_INFOBOX, true);
		pc++;
	}
//	printf("DONE");
	p++;
	}

	aCH_lastHidding=pc;

	mysql_free_result();

	// pobieramy szukajacyh
	format(buffer,sizeof buffer,"SELECT X,Y,Z,A FROM fs_chowany_arena_sp WHERE aid=%d AND team=1 ORDER BY RAND() LIMIT %d", gmTemp[aChowanyArena], minSearchers);
	mysql_query(buffer);
	mysql_store_result();
	if (mysql_num_rows()<minSearchers) {
		mysql_free_result();
		SendClientMessageToAll(-1,"Chowany nie wystartowal - nie znaleziono odpowiedniej areny");
		CH_Finish();
		return 0;
	}



	p=0;	pc=0;
	while (p<aMaxPlayers) {
	if (pData[aPlayers[p]][aChowany] && pTemp[aPlayers[p]][aChowanySide] == A_CHOWANY_SEARCHING) {
		if (!mysql_fetch_row(buffer,"|")) {
			mysql_free_result();
			SendClientMessageToAll(-1,"Chowany nie wystartowal - nie znaleziono odpowiedniej areny");
			CH_Finish();
			return 0;
		}

//		printf("Ustawianie gracza %d jako szukajacego", aPlayers[p]);
		new Float:PX,Float:PY,Float:PZ,Float:PA;
		if (sscanf(buffer,"p<|>ffff",PX, PY, PZ, PA)) 
//			SendClientMessageToAll(-1,"No sscanf searching");
			continue;
		
		pData[aPlayers[p]][pAttraction] = A_CHOWANY;
		pData[aPlayers[p]][aChowany] = false;
		pTemp[aPlayers[p]][aCHDead]=false;
		CHPozaStrefa[p]=10;

		TogglePlayerControllable(aPlayers[p], 0);

		SetPlayerColor(aPlayers[p], 0x0A043900+127);	// ciemnogranatowy

		SetPlayerArmour(aPlayers[p], 0.0);
		SetPlayerHealth(aPlayers[p], 999999.0);
		ResetPlayerWeapons(aPlayers[p]);
		
		Teleport(T_PLAYER, aPlayers[p], PX, PY, PZ, PA, gmTemp[aChowanyInterior], VW_CHOWANY,true);
		
//		TextDrawSetString(pTextDraw[PTD_INFOBOX][aPlayers[p]], "Szukasz!");
//		ShowElement(aPlayers[p], TDE_INFOBOX, true);
		
		SetPlayerCameraPos(aPlayers[p], PX,PY,PZ + 2.5);
		SetPlayerCameraLookAt(aPlayers[p], PX,PY,PZ);
		GivePlayerWeapon(aPlayers[p], 38, 1);
		pc++;
	}
	
	p++;
	}
	aCH_lastSearching=pc;

	mysql_free_result();

	
	gmTemp[aChowanyPlayers] = aPlayers;
	gmTemp[aChowanyMaxPlayers] = aMaxPlayers;
	gmTemp[aChowanyCount] = 25;
	if (aMaxPlayers>10)
		gmTemp[aChowanyCount]+= floatround((aMaxPlayers-5)/4);

	KillTimer(gmTemp[timerChowanyCountdown]);
	gmTemp[timerChowanyCountdown] = SetTimer("chowany_Countdown", 1000, true);
	gmTemp[aChowanyStartTick] = GetTickCount();
//	aData[A_CHOWANY][aState]= A_STATE_ON;
	return 1;
}


CH_Update()
{
	if (aData[A_CHOWANY][aState] == A_STATE_OFF)
		return;
	new
	 aSearchingPlayers = 0,
	 aHiddingPlayers = 0,
	 bool:aBreak = false,
	 aLastHidding,
	 buf[128];
	
	for(new playerid = 0; playerid < gmTemp[aChowanyMaxPlayers]; playerid++)
	{
		new rpid=gmTemp[aChowanyPlayers][playerid];
		if(rpid == INVALID_PLAYER_ID) continue;

		if(pData[rpid][pAttraction] != A_CHOWANY || GetPlayerVirtualWorld(rpid)!=VW_CHOWANY) {
			gmTemp[aChowanyPlayers][playerid]=INVALID_PLAYER_ID;
			continue;
		}
		if (pTemp[rpid][staleTime]>5) {
			Msg(rpid,COLOR_ERROR,"Zostajesz wyrzucony z {b}chowanego{/b} za bycie {b}AFK{/b}");
			SetPlayerHealth(rpid,0);
			gmTemp[aChowanyPlayers][playerid]=INVALID_PLAYER_ID;
			pData[rpid][pAttraction] = A_NONE;
			continue;
		}
		new bool:pozaS=false, bool:pozaC=false;
		
		if (aCHWBSphere[0]==aCHWBSphere[0]) {
			new Float:PP[3];
			GetPlayerPos(rpid, PP[0], PP[1], PP[2]);
			if (GetDistanceBetweenPoints3D(PP[0],PP[1],PP[2], aCHWBSphere[0], aCHWBSphere[1], aCHWBSphere[2])> aCHWBSphere[3]) {
				pozaS=true;
			}	
		} else if (!aCHWBmode) pozaS=true;
		
		if (aCHWBCube[0]==aCHWBCube[0]) {
			if (!IsPlayerInCube(rpid,
					aCHWBCube[3], aCHWBCube[4], aCHWBCube[5],
					aCHWBCube[0], aCHWBCube[1], aCHWBCube[2] )) {
				pozaC=true;

			}
		} else if (!aCHWBmode) pozaC=true;

		if (aCHWBCube[0]==aCHWBCube[0] || aCHWBSphere[0]==aCHWBSphere[0])
		if (gmTemp[aChowanyCount]<=0 && 		//aqq
				(
					(aCHWBmode && (pozaS || pozaC))				// oraz!
					||
					(!aCHWBmode && pozaS && pozaC)				// lub
				))
			 {

			if(CHPozaStrefa[playerid]>0) {
				format(buf, sizeof buf,"~r~Wracaj na arene!~n~~w~ %d s", CHPozaStrefa[playerid]);
				CHPozaStrefa[playerid]--;
				GameTextForPlayer(rpid, buf,1000,3);
				FlashScreen(rpid);
				PlaySound(rpid, 1135);
			} else {
				SetPlayerHealth(rpid,0);
				pData[rpid][pAttraction] = A_NONE;
				gmTemp[aChowanyPlayers][playerid] = INVALID_PLAYER_ID;
			}

		} else if (CHPozaStrefa[playerid]<10)
			CHPozaStrefa[playerid]++;

//		printf(" liczenie graczy %d:%d -> %d", playerid,rpid,pTemp[rpid][aChowanySide]);

		if(pTemp[rpid][aChowanySide] == A_CHOWANY_SEARCHING) {
			aSearchingPlayers++;
//			printf("Szuka");
		}
		else if(pTemp[rpid][aChowanySide] == A_CHOWANY_HIDDING)
		{
			aHiddingPlayers++;
			aLastHidding = gmTemp[aChowanyPlayers][playerid];
//			printf("chowa sie");
		}

		if (gmTemp[aChowanyCount]>0)
			continue;

/*		if (aSearchingPlayers<aCH_lastSearching && aSearchingPlayers>0) {
			if (pTemp[rpid][aChowanySide] == A_CHOWANY_SEARCHING && aSearchingPlayers==1) {
					format(buf,sizeof buf, "~w~Szukasz sam%s!", Kobieta(gmTemp[aChowanyPlayers][playerid])?("a"):(""));
					GameTextForPlayer(rpid, buf, 6000, 1);
			} else {
					format(buf,sizeof buf, "~w~%d %s", aSearchingPlayers, dli(aSearchingPlayers, "szukajacy", "szukajacych", "szukajacych"));
					GameTextForPlayer(rpid, buf, 6000, 1);
			}


			
		} else if (aHiddingPlayers<aCH_lastHidding && aHiddingPlayers>1) {
			if (pTemp[rpid][aChowanySide] == A_CHOWANY_SEARCHING) {
				format(buf,sizeof buf, "~w~Jeszcze ~n~%d %s", aHiddingPlayers, dli(aHiddingPlayers, "osoba", "osoby", "osob"));
				GameTextForPlayer(rpid, buf, 6000, 1);
			} else {
				format(buf,sizeof buf, "~w~Jeszcze ~n~%d %s sie", aHiddingPlayers, dli(aHiddingPlayers, "chowajacy", "chowajacych", "chowajacych"));
				GameTextForPlayer(rpid, buf, 6000, 1);
			}
		}*/

		new alpha=floatround(GetPlayerSpeed(rpid)*255);

		if (pTemp[gmTemp[aChowanyPlayers][playerid]][aChowanySide] == A_CHOWANY_SEARCHING)
			alpha=floatround(alpha/0.75);

		if (alpha>255) alpha=255;
		pTemp[gmTemp[aChowanyPlayers][playerid]][tmpAlpha]=floatround((pTemp[gmTemp[aChowanyPlayers][playerid]][tmpAlpha]+alpha)/2);
		if(pTemp[gmTemp[aChowanyPlayers][playerid]][aChowanySide] == A_CHOWANY_SEARCHING)
			SetPlayerColor(rpid,0x0A043900+ pTemp[gmTemp[aChowanyPlayers][playerid]][tmpAlpha]);
		else
			SetPlayerColor(rpid,0xD0E0D000+ pTemp[gmTemp[aChowanyPlayers][playerid]][tmpAlpha]);	
	}
	
	if (gmTemp[aChowanyCount]<=0) {
		aCH_lastHidding=aHiddingPlayers;
		aCH_lastSearching=aSearchingPlayers;
	}

	if(aHiddingPlayers < 1)
	{
		foreach(playerid)
			Msg(gmTemp[aChowanyPlayers][playerid], COLOR_ERROR, TXT(playerid, 442));
		
		aBreak = true;
	}
	else if(aSearchingPlayers < 1)
	{
		foreach(playerid)
			Msg(gmTemp[aChowanyPlayers][playerid], COLOR_ERROR, TXT(playerid, 441));
		aBreak = true;
	}
	else if(aHiddingPlayers == 1 && gmTemp[aChowanyCount]<=0)
	{
		new
		 playerid = aLastHidding;
		new score=floatround( 5+(gmTemp[aChowanyMaxPlayers]/2));
		format(buf, sizeof buf, "Chowanego wygrywa {b}%s{/b} i otrzymuje {b}%d respektu{/b}!", GetPlayerProperName(playerid), score);
		foreach(i)
			if(i != playerid)
				Msg(i, COLOR_INFO3, buf); // "xxx" wygrywa chowanego!

		format(buf, sizeof buf, "Wygrywasz chowanego i otrzymujesz {b}%d respektu{/b}!", score);
		Msg(playerid, COLOR_INFO2, buf); // Wygrywasz chowanego! +10 respekt
		GivePlayerScore(playerid, score, "Wygrana w Chowanym");
		
		aBreak = true;
	}

	if(aBreak)
		CH_Finish();
}

CH_Finish() {
//	printf("Konczenie chowanego!");

	KillTimer(gmTemp[timerChowanyCountdown]);
		
	for(new i = 0; i < gmTemp[aChowanyMaxPlayers]; i++)
	{
		if(gmTemp[aChowanyPlayers][i] == INVALID_PLAYER_ID) continue;

		if (!IsPlayerConnected(gmTemp[aChowanyPlayers][i])) {
			gmTemp[aChowanyPlayers][i]=INVALID_PLAYER_ID;
			continue;
		}
		
		pData[gmTemp[aChowanyPlayers][i]][pAttraction] = A_NONE;

		if (GetPlayerState(gmTemp[aChowanyPlayers][i])!=PLAYER_STATE_WASTED && !pTemp[gmTemp[aChowanyPlayers][i]][aCHDead]) {
				SetPlayerHealth(gmTemp[aChowanyPlayers][i],100.0);
				SpawnPlayer(gmTemp[aChowanyPlayers][i]);
		}
			
//		ShowElement(gmTemp[aChowanyPlayers][i], TDE_INFOBOX, false);
		gmTemp[aChowanyPlayers][i] = INVALID_PLAYER_ID;
	}
	aData[A_CHOWANY][aState] = A_STATE_OFF;
	gmTemp[aChowanyMaxPlayers]=0;
	gmTemp[aChowanyCount]=0;
}

public chowany_Countdown()
{
	new
	 buffer[128],buffer2[128];
//	printf("CHcountdown: %d", gmTemp[aChowanyCount]);



	if(gmTemp[aChowanyCount] <= 0)
	{
		format(buffer, sizeof buffer, "~g~GO!");
		KillTimer(gmTemp[timerChowanyCountdown]);
	}


	if(gmTemp[aChowanyCount]>0)
	{
		format(buffer, sizeof buffer, "~r~%i", gmTemp[aChowanyCount]);
	}

	if (gmTemp[aChowanyCount]==20)
		format(buffer2, sizeof buffer2, "~w~%d %s_~n~%d %s sie", aCH_lastSearching, dli(aCH_lastSearching,"szukajacy","szukajacych","szukajacych"), aCH_lastHidding,
				dli(aCH_lastHidding,"chowajacy","chowajacych","chowajacych"));
	
	for(new playerid = 0; playerid < gmTemp[aChowanyMaxPlayers]; playerid++) {
		if (gmTemp[aChowanyPlayers][playerid]==INVALID_PLAYER_ID || pData[gmTemp[aChowanyPlayers][playerid]][pAttraction]!=A_CHOWANY) continue;

		GameTextForPlayer(gmTemp[aChowanyPlayers][playerid], buffer, 1000, 6);

		if (gmTemp[aChowanyCount]==20)
			GameTextForPlayer(gmTemp[aChowanyPlayers][playerid], buffer2, 6000, 1);

		if (gmTemp[aChowanyCount]<=0) {
			PlaySound(gmTemp[aChowanyPlayers][playerid], 1057);

			if(pTemp[gmTemp[aChowanyPlayers][playerid]][aChowanySide] == A_CHOWANY_SEARCHING)
			{
				GivePlayerWeapon(gmTemp[aChowanyPlayers][playerid], 38, 0xFFFF);
				TogglePlayerControllable(gmTemp[aChowanyPlayers][playerid], 1);
				SetCameraBehindPlayer(gmTemp[aChowanyPlayers][playerid]);
//				format(buffer,sizeof buffer, "~w~do znalezienia ~n~%d %s", aCH_lastHidding, dli(aCH_lastHidding, "osoba", "osoby", "osob"));
//				GameTextForPlayer(gmTemp[aChowanyPlayers][playerid], buffer, 6000, 1);

//			} else {
//				format(buffer,sizeof buffer, "~w~%d %s", aCH_lastHidding, dli(aCH_lastHidding, "szukajacy", "szukajacych", "szukajacych"));
//				GameTextForPlayer(gmTemp[aChowanyPlayers][playerid], buffer, 6000, 1);
			}
		} else {
			PlaySound(gmTemp[aChowanyPlayers][playerid], 1056);
			if(pTemp[gmTemp[aChowanyPlayers][playerid]][aChowanySide] == A_CHOWANY_SEARCHING)
				TogglePlayerControllable(gmTemp[aChowanyPlayers][playerid], 0);
		}
	}
	
	
	gmTemp[aChowanyCount]--;
}
