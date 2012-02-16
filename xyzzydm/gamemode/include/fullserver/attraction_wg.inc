//#define A_WG_WPNSEL


/*
	de snipe
	de combat
	de mp5
	de m4
	de rifle
	de sniper rifle

	de/sil, shotgun, mp5, m4/ak47, sniper rifle/rifle
	
	5	 (8)
	
*/


new Float:aWGWBCube[6],
    Float:aWGWBSphere[4],
	WGPozaStrefa[MAX_QUEUE_WG]={10,...},
	aWGWBmode;

#if defined A_WG_WPNSEL
new Menu:aWGWeaponMenu;

enum e_WGWeaponSelection {
	ws_wid1, ws_wid2,
	ws_combinationName[32]
	
}

new aWGWeaponMenuWPN[][e_WGWeaponSelection]={
	{24, 30,	"deagle + ak47"},
	{24, 29,	"deagle + mp5"},
	{24, 31,	"deagle + m4"},
	{24, 33,	"deagle + rifle"},
	{24, 34,	"deagle + sniper rifle"},
	{24, 25,	"deagle + shotgun"},
	{31, 33,	"m4 + rifle"},
	{31, 25,	"m4 + shotgun"},
	{31, 34,	"m4 + sniper rifle"},
	{29, 34,	"mp5 + sniper rifle"},
	{29, 30,	"mp5 + ak47"},
	{25, 34,	"shotgun + sniper rifle"}
//	{24, 4,		"deagle + noz"},
//	{34, 4,		"sniper rifle + noz"},
//	{25, 4,		"shotgun + noz"},
//	{23, 4,		"silenced + noz"}
};
forward WG_ForceWPNSelection();
#endif

forward WG_Countdown();



WG_Start(){
	new
	 aPlayers[MAX_QUEUE_WG],
	 aMaxPlayers = 0,
	 buffer[255],
	 _skill[2],
	 _teamsize[2],
	 wb_sphere[80],
	 wb_cube[80];
	
	foreach(playerid)
	{
		if(pData[playerid][aWG])
		{
			if(AttractionAllowed(playerid))
			{
				_teamsize[aMaxPlayers%2]++;
				aPlayers[aMaxPlayers++] = playerid;
				if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
			}
			else
			{
				pData[playerid][aWG] = false;
				Msg(playerid, COLOR_ERROR, TXT(playerid, 285));
			}
		}
	}
	


//	format(buffer,sizeof buffer,"aMaxPlayers: %d", aMaxPlayers);
//	SendClientMessageToAll(0xff0000ff,buffer);

//	if (aMaxPlayers>2 && floatabs(Float:(_skill[0]-_skill[1]))>
//	TODO kod do wyrownywania druzyn

	if(aMaxPlayers < MIN_QUEUE_WG)
	{
		foreach(playerid)
		{
			Msg(playerid, COLOR_INFO3, TXT(playerid, 275));
			pData[playerid][aWG] = false;
		}
		
		aData[A_WG][aState] = A_STATE_OFF;
		return 0;
	}

	gmTemp[aWGPBar]=CreateProgressBar(453.00, 356.00, 166.50, 4.50, 16777215, aMaxPlayers*100);

	// losowanie areny
	if (gmTemp[aWGForceArena]>0) {
	    format(buffer,sizeof buffer,"select id,descr,interior,IFNULL(wb_sphere,'-'),IFNULL(wb_cube,'-'),(wb_mode='oraz') from fs_wg_arena where id=%d LIMIT 1;",gmTemp[aWGForceArena]);
		gmTemp[aWGForceArena]=0;
	} else
	    format(buffer,sizeof buffer,"select id,descr,interior,IFNULL(wb_sphere,'-'),IFNULL(wb_cube,'-'),(wb_mode='oraz') from fs_wg_arena where minplayers<=%d AND %d<=maxplayers ORDER by rand() LIMIT 1;",aMaxPlayers,aMaxPlayers);
    mysql_query(buffer);
    mysql_store_result();
    if (!mysql_num_rows()) {    // nie znaleziono odpowiedniej areny
        SendClientMessageToAll(0xffffffff,"WG nie wystartowalo - nie znaleziono odpowiedniej areny");

		WG_Finish();
		if (mysql_result_stored()) 	mysql_free_result();
        return 0;
    }

    mysql_fetch_row(buffer,"|");
	mysql_free_result();
	
    if (sscanf(buffer,"p<|>ds[63]ds[63]s[63]d",gmTemp[aWGArena],gmTemp[aWGDescr],gmTemp[aWGInterior], wb_sphere,wb_cube,aWGWBmode)) {
        SendClientMessageToAll(0xffffffff,"WG nie wystartowalo - nie znaleziono odpowiedniej areny");
		WG_Finish();
        return 0;
	}
//	sscanf(aWorldBounds,"p<,>F(20000)F(-20000)F(20000)F(-20000)",gmTemp[aWGWorldBounds][0],gmTemp[aWGWorldBounds][1],gmTemp[aWGWorldBounds][2],gmTemp[aWGWorldBounds][3]);
	aWGWBSphere[0]=FLOAT_NAN; 
	sscanf(wb_sphere,"p<,>ffff", aWGWBSphere[0],aWGWBSphere[1],aWGWBSphere[2],aWGWBSphere[3]);

	aWGWBCube[0]=FLOAT_NAN;
	sscanf(wb_cube,"p<,>ffffff", aWGWBCube[0],aWGWBCube[1],aWGWBCube[2],aWGWBCube[3],aWGWBCube[4],aWGWBCube[5]);

//	format(buffer,sizeof buffer,"Arena %d - %s.... %s : %.2f", gmTemp[aWGArena],gmTemp[aWGDescr],wb_sphere, aWGWBSphere[0]);
//	SendClientMessageToAll(0xff0000ff,buffer);

	TextDrawSetString(gTextDraw[TD_AWGINFO],"~g~-   ~w~vs   ~b~-");

	format(buffer,sizeof buffer,"(SELECT team,X,Y,Z,A FROM fs_wg_arena_sp WHERE aid=%d AND team=0 ORDER BY RAND() LIMIT %d) UNION (SELECT team,X,Y,Z,A FROM fs_wg_arena_sp WHERE aid=%d AND team=1 ORDER BY rand() LIMIT %d)", gmTemp[aWGArena], _teamsize[0], gmTemp[aWGArena], _teamsize[1]);
	mysql_query(buffer);
	mysql_store_result();
	

	new p=0;	
	while (mysql_fetch_row(buffer,"|") && p<aMaxPlayers) {
		new team,Float:PX,Float:PY,Float:PZ,Float:PA;
		WGPozaStrefa[p]=10;
		sscanf(buffer,"p<|>dffff",team, PX, PY, PZ, PA);

		pTemp[aPlayers[p]][aWGTeam]=team;
		pTemp[aPlayers[p]][aWGDead]=false;
		_skill[team]+=pData[aPlayers[p]][skill];

		SetPlayerHealth(aPlayers[p],99.0);
		SetPlayerArmour(aPlayers[p],0.0);
		Streamer_UpdateEx(aPlayers[p],PX,PY,PZ);

		SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_PISTOL, 0);
		SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_PISTOL_SILENCED, 0);
		SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_SAWNOFF_SHOTGUN, 0);
		SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_MICRO_UZI, 0);

		TogglePlayerControllable(aPlayers[p],false);
		ResetPlayerWeapons(aPlayers[p]);
		SetPlayerPos(aPlayers[p],PX,PY,PZ);
		Streamer_Update(aPlayers[p]);
		SetPlayerFacingAngle(aPlayers[p],PA);
		SetPlayerTeam(aPlayers[p], team);
		SetPlayerVirtualWorld(aPlayers[p], VW_WG);
		SetPlayerInterior(aPlayers[p], gmTemp[aWGInterior]);
//		SetPlayerWorldBounds(aPlayers[p], gmTemp[aWGWorldBounds][0],gmTemp[aWGWorldBounds][1],gmTemp[aWGWorldBounds][2],gmTemp[aWGWorldBounds][3]);

		pData[aPlayers[p]][pAttraction] = A_WG;
		pData[aPlayers[p]][aWG] = false;

		TextDrawShowForPlayer(aPlayers[p],gTextDraw[TD_AWGINFO]);
		ShowProgressBarForPlayer(aPlayers[p],gmTemp[aWGPBar]);
		TextDrawShowForPlayer(aPlayers[p],gTextDraw[TD_AWGINFOHP]);

		TogglePlayerControllable(aPlayers[p], false);
		GivePlayerWeapon(aPlayers[p],4,1);
		GivePlayerWeapon(aPlayers[p],24,1000);
		GivePlayerWeapon(aPlayers[p],25,200);
		GivePlayerWeapon(aPlayers[p],34,1000);

		

		if (team==A_WG_TEAM_GREEN) 
			SetPlayerColor(aPlayers[p],0x00FF00FF);	
		else
			SetPlayerColor(aPlayers[p],0x0000FFFF);
		p++;
	}

	mysql_free_result();

	gmTemp[aWGPlayers] = aPlayers;
	gmTemp[aWGMaxPlayers] = aMaxPlayers;
	gmTemp[aWGCount] = 3;
	gmTemp[timerWGCountdown] = SetTimer("WG_Countdown", 1000, true);
#if defined A_WG_WPNSEL
	gmTemp[timerWGWeapSel] = SetTimer("WG_ForceWPNSelection", 15000, false);
#endif
	gmTemp[aWGStartTick] = GetTickCount();
	WG_Update();
	return 1;
}

WG_Finish(){
	if (_:gmTemp[aWGPBar]!=-1) {
		HideProgressBarForAll(gmTemp[aWGPBar]);
		DestroyProgressBar(gmTemp[aWGPBar]);
	}
	TextDrawHideForAll(gTextDraw[TD_AWGINFO]);
	TextDrawHideForAll(gTextDraw[TD_AWGINFOHP]);
	aData[A_WG][aState] = A_STATE_OFF;

	for(new i = 0; i < gmTemp[aWGMaxPlayers]; i++) {
		if (gmTemp[aWGPlayers][i]==INVALID_PLAYER_ID) continue;
		if (!IsPlayerConnected(gmTemp[aWGPlayers][i])) {
			gmTemp[aWGPlayers][i]=INVALID_PLAYER_ID;
			continue;
		}

		pData[gmTemp[aWGPlayers][i]][pAttraction] = A_NONE;
		SetPlayerTeam(gmTemp[aWGPlayers][i], gmTemp[aWGPlayers][i]);
//		SetPlayerWorldBounds(gmTemp[aWGPlayers][i], 20000.0, -20000.0, 20000.0, -20000.0);
		if (GetPlayerState(gmTemp[aWGPlayers][i])!=PLAYER_STATE_WASTED && !pTemp[gmTemp[aWGPlayers][i]][aWGDead]) {
			SetPlayerHealth(gmTemp[aWGPlayers][i],100.0);
			SpawnPlayer(gmTemp[aWGPlayers][i]);
		}
		gmTemp[aWGPlayers][i]=INVALID_PLAYER_ID;
	}
	aData[A_WG][aState] = A_STATE_OFF;
	gmTemp[aWGMaxPlayers]=0;
	return 1;
}

#if defined A_WG_WPNSEL
public WG_ForceWPNSelection()
{
	for(new playerid = 0; playerid < gmTemp[aWGMaxPlayers]; playerid++) 
	if (pData[gmTemp[aWGPlayers][playerid]][pAttraction]==A_WG && !pTemp[gmTemp[aWGPlayers][playerid]][aWGDead])
	{
		HideMenuForPlayer(aWGWeaponMenu, playerid);
		TogglePlayerControllable(playerid, true);
	}
}
#endif
public WG_Countdown()
{
	new
	 buffer[64];
	
	if(gmTemp[aWGCount] <= 0)
	{
		format(buffer, sizeof buffer, "~g~GO!");
		KillTimer(gmTemp[timerWGCountdown]);
	}
	else
	{
		format(buffer, sizeof buffer, "~r~%i", gmTemp[aWGCount]);
	}
	
	for(new playerid = 0; playerid < gmTemp[aWGMaxPlayers]; playerid++) {
		GameTextForPlayer(gmTemp[aWGPlayers][playerid], buffer, 1000, 6);
#if defined A_WG_WPNSEL
		if(gmTemp[aWGCount]==3)
			ShowMenuForPlayer(aWGWeaponMenu, gmTemp[aWGPlayers][playerid]);
#endif
		if(gmTemp[aWGCount] == 0)
		{
#if defined A_WG_WPNSEL
#else
			TogglePlayerControllable(gmTemp[aWGPlayers][playerid], 1);
#endif
			PlaySound(gmTemp[aWGPlayers][playerid], 1057);
		} else
			PlaySound(gmTemp[aWGPlayers][playerid], 1056);
		
		
	}
	
	gmTemp[aWGCount]--;
}

WG_Update(){
//	printf("WG UPDATE"); //dbg
	new _hp[2],Float:tmphp,_cnt[2],buf[128];
	for(new playerid = 0; playerid < gmTemp[aWGMaxPlayers]; playerid++)
	if (!IsPlayerConnected(gmTemp[aWGPlayers][playerid])) {
			gmTemp[aWGPlayers][playerid]=INVALID_PLAYER_ID;
			continue;
	} else if (!pTemp[gmTemp[aWGPlayers][playerid]][aWGDead] && GetPlayerVirtualWorld(gmTemp[aWGPlayers][playerid])==VW_WG)
	{
		if (pTemp[gmTemp[aWGPlayers][playerid]][staleTime]>5) {
			Msg(gmTemp[aWGPlayers][playerid],COLOR_ERROR,"Zostajesz wyrzucony z {b}WG{/b} za bycie {b}AFK{/b}");
			SetPlayerHealth(gmTemp[aWGPlayers][playerid],0);
			pData[gmTemp[aWGPlayers][playerid]][pAttraction]=A_NONE;
			gmTemp[aWGPlayers][playerid]=INVALID_PLAYER_ID;
			continue;
		}
		new bool:pozaS=false, bool:pozaC=false;
		
		if (aWGWBSphere[0]==aWGWBSphere[0] ) {
			new Float:PP[3];
			GetPlayerPos(gmTemp[aWGPlayers][playerid], PP[0], PP[1], PP[2]);
			if (GetDistanceBetweenPoints3D(PP[0],PP[1],PP[2], aWGWBSphere[0], aWGWBSphere[1], aWGWBSphere[2])> aWGWBSphere[3]) {
				pozaS=true;
			}	
		} else if (!aWGWBmode) pozaS=true;
		
		if (aWGWBCube[0]==aWGWBCube[0]) {
			if (!IsPlayerInCube(gmTemp[aWGPlayers][playerid], 
					aWGWBCube[3], aWGWBCube[4], aWGWBCube[5],
					aWGWBCube[0], aWGWBCube[1], aWGWBCube[2] )) {
				pozaC=true;

			}
		} else if (!aWGWBmode) pozaC=true;

//		new buf2[128];
//		format(buf2, sizeof buf2,"WS: %d, WC: %d, mode %d", pozaS, pozaC, aWGWBmode);
//		SendClientMessageToAll(-1, buf2);

		if (aWGWBCube[0]==aWGWBCube[0] || aWGWBSphere[0]==aWGWBSphere[0])
		if (gmTemp[aWGCount]<=0 && 
				(
					(aWGWBmode && (pozaS || pozaC))				// oraz!
					||
					(!aWGWBmode && pozaS && pozaC)				// lub
				))
			 {
			if(WGPozaStrefa[playerid]>0) {
				format(buf, sizeof buf,"~r~Wracaj na arene!~n~~w~ %d s", WGPozaStrefa[playerid]);
				WGPozaStrefa[playerid]--;
				GameTextForPlayer(gmTemp[aWGPlayers][playerid], buf,1000,3);
				FlashScreen(gmTemp[aWGPlayers][playerid]);
				PlaySound(gmTemp[aWGPlayers][playerid], 1135);

				
			} else {
				SendClientMessage(gmTemp[aWGPlayers][playerid], -1, "Znajdowales sie poza arena zbyt dlugo.");
				SetPlayerHealth(gmTemp[aWGPlayers][playerid],0);
			}

		} else if (WGPozaStrefa[playerid]<10)
			WGPozaStrefa[playerid]++;
		

		GetPlayerHealth(gmTemp[aWGPlayers][playerid], tmphp);
		_hp[pTemp[gmTemp[aWGPlayers][playerid]][aWGTeam]]+=floatround(tmphp);
		_cnt[pTemp[gmTemp[aWGPlayers][playerid]][aWGTeam]]++;
	}
	
	SetProgressBarMaxValue(gmTemp[aWGPBar],_hp[0]+_hp[1]);
	SetProgressBarValue(gmTemp[aWGPBar],_hp[0]);
	SetProgressBarColor(gmTemp[aWGPBar], 0x00000000+ (floatround( 255* _hp[0] / (_hp[0]+_hp[1]))<<16) + (floatround( 255* _hp[1] / (_hp[0]+_hp[1]))<<8) + 0xff);


	format(buf,sizeof buf,"~g~%d   ~w~vs   ~b~%d", _cnt[0], _cnt[1]);
	TextDrawSetString(gTextDraw[TD_AWGINFO], buf);
	format(buf,sizeof buf,"~g~%dHP               ~r~%dHP~n~~n~~w~%s_(%d)", _hp[0],_hp[1],gmTemp[aWGDescr],gmTemp[aWGArena]);
	TextDrawSetString(gTextDraw[TD_AWGINFOHP], buf);

	for(new playerid = 0; playerid < gmTemp[aWGMaxPlayers]; playerid++)
		if (gmTemp[aWGPlayers][playerid]!=INVALID_PLAYER_ID && pData[gmTemp[aWGPlayers][playerid]][pAttraction]==A_WG)
			UpdateProgressBar(gmTemp[aWGPBar],	gmTemp[aWGPlayers][playerid]);

	// Team BLUE wins
	if(_cnt[A_WG_TEAM_GREEN] < 1 && _cnt[A_WG_TEAM_BLUE] >= 1)
	{
		foreach(playerid)
		{
			if(pData[playerid][pAttraction] == A_WG && pTemp[playerid][aWGTeam] == A_WG_TEAM_BLUE)
			{
				Msg(playerid, COLOR_INFO3, TXT(playerid, 433)); // Twoja dru¿yna wygrywa wojnê gangów! Respekt +5
				GivePlayerScore(playerid,(5+floatround(gmTemp[aWGMaxPlayers]/2))*((pTemp[playerid][aWGDead]) ? 2 : 1),"WG");
			}
			else
				Msg(playerid, COLOR_INFO3, TXT(playerid, 432)); // Niebiescy wygrywaj¹ wojnê gangów!

		}
		WG_Finish();
	}
			
	// Team GREEN wins
	else if(_cnt[A_WG_TEAM_BLUE] < 1 && _cnt[A_WG_TEAM_GREEN] >= 1)
	{
		foreach(playerid)
		{
			if(pData[playerid][pAttraction] == A_WG && pTemp[playerid][aWGTeam] == A_WG_TEAM_GREEN)
			{
				Msg(playerid, COLOR_INFO3, TXT(playerid, 433)); // Twoja dru¿yna wygrywa wojnê gangów
				GivePlayerScore(playerid,(5+floatround(gmTemp[aWGMaxPlayers]/2))*((pTemp[playerid][aWGDead]) ? 2 : 1),"WG");
			}
			else
				Msg(playerid, COLOR_INFO3, TXT(playerid, 431)); // Zieloni wygrywaj¹ wojnê gangów!
		}
		WG_Finish();
	}
						
	// Noone wins
	else if(_cnt[A_WG_TEAM_BLUE] < 1 && _cnt[A_WG_TEAM_GREEN] < 1)
	{
		foreach(playerid)
			Msg(playerid, COLOR_INFO3, TXT(playerid, 429)); // WG zosta³o przerwane z powodu braku graczy.
		WG_Finish();
	}

	return;
}
#if defined A_WG_WPNSEL
wg_OnPlayerSelectedMenuRow(playerid,row){

	new buf[128];
	if (pTemp[playerid][aWGDead])
		return 1;

	if (SkinKobiecy(GetPlayerSkin(playerid)))
		format(buf,sizeof buf,"{b}%s{/b} wybrala {b}%s{/b}",GetPlayerProperName(playerid),aWGWeaponMenuWPN[row][ws_combinationName]);
	else
		format(buf,sizeof buf,"{b}%s{/b} wybral {b}%s{/b}",GetPlayerProperName(playerid), aWGWeaponMenuWPN[row][ws_combinationName]);
	ResetPlayerWeapons(playerid);

//	GivePlayerWeapon(playerid,4, 1);
	GivePlayerWeapon(playerid,aWGWeaponMenuWPN[row][ws_wid1], 1500);
	GivePlayerWeapon(playerid,aWGWeaponMenuWPN[row][ws_wid2], 1500);
	
	
	new team=pTemp[playerid][aWGTeam];
	for(new i = 0; i < gmTemp[aWGMaxPlayers]; i++)
		if (pTemp[gmTemp[aWGPlayers][i]][aWGTeam]==team)
			Msg(gmTemp[aWGPlayers][i], COLOR_INFO, buf);

	TogglePlayerControllable(playerid, true);

	return 1;
}
#endif
#if defined A_WG_WPNSEL
wg_OnPlayerExitedMenu(playerid){
	if (pTemp[playerid][aWGDead] || pData[playerid][pAttraction]!=A_WG)
		return 1;
	if (GetTickCount()-gmTemp[aWGStartTick]<9000) {
		Msg(playerid,COLOR_INFO,"Wybierz zestaw broni, z ktorym chcesz rozpoczac walke!");
		return ShowMenuForPlayer(aWGWeaponMenu, playerid);
	} 
	TogglePlayerControllable(playerid, true);
	// wybieramy losowy
	Msg(playerid,COLOR_INFO2,"Zostaje Ci przydzielony losowy zestaw broni");
	return wg_OnPlayerSelectedMenuRow(playerid,random(sizeof aWGWeaponMenuWPN));
}
#endif