#define GANG_HOLDSLOT	2
#define GANG_YAKUZA	2

gangs_OnPlayerLogin(playerid){
	if (pData[playerid][gang]==NO_GANG)
		return 0;
	new buf[127];
	if (pData[playerid][gangRank]==GANG_RANK_OWNER) {
		format(buf,sizeof buf,"*** Zalozyciel %s - %s (%d) dolacza do gry.", gData[pData[playerid][gang]][name], GetPlayerProperName(playerid), playerid);
		SendClientMessageToAll(gData[pData[playerid][gang]][gColor] * 256, buf);
	} else if (pData[playerid][gangRank]==GANG_RANK_LEADER) {
		format(buf,sizeof buf,"*** Lider %s - %s (%d) dolacza do gry.", gData[pData[playerid][gang]][name], GetPlayerProperName(playerid), playerid);
		SendClientMessageToAll(gData[pData[playerid][gang]][gColor] * 256, buf);
	}
	return 1;
}

gangs_SetPlayerAttachedObject(playerid){
	if (!pData[playerid][loggedIn]) return;
	switch(pData[playerid][gang]){
		case GANG_YAKUZA:
			SetPlayerAttachedObject( playerid, GANG_HOLDSLOT, 902, 1, 0.193763, -0.067280, 0.000000, 87.878509, 3.261517, 180.000000, 0.100000, 0.100000, 0.100000 ); // Starfish - yakuza
	}

}


gangs_ShowGangsList(playerid){
	new
		szGangList[1024],
		szGangName[8],
		szGangDesc[32],
		szGangColor[7],
		szGangID[4],
		i = 0,
		buffer[1024];
					
	format(buffer, sizeof buffer, "SELECT tag,color,name,id FROM %s", gmData[DB_gangs]);
	mysql_query(buffer);
	mysql_store_result();
					
	while(mysql_fetch_row_data())
	{
		mysql_fetch_field("tag", szGangName);
		mysql_fetch_field("color", szGangColor);
		mysql_fetch_field("name", szGangDesc);
		mysql_fetch_field("id", szGangID);
						
		format(buffer, sizeof buffer, "{%s}%s\t{ffffff}%s\n", szGangColor, szGangName, szGangDesc);
		strcat(szGangList, buffer);
						
		pTemp[playerid][statsGangListID][i++] = StringToInt(szGangID);
	}
					
	strdel(szGangList, strlen(szGangList) - 1, strlen(szGangList));
					
	if (mysql_result_stored()) mysql_free_result();

	return 	ShowPlayerDialog(playerid, DIALOG_STATS_GANG_LIST, DIALOG_STYLE_LIST, "Statystyki > gangi", szGangList, TXT(playerid, 76), TXT(playerid, 77));
}

gangs_saveGangData(bool:force=false){
	new buf[128];
	for(new i=1;i<(MAX_GANGS+1);i++) 
	if (force || random(2)==1) {
		format(buf, sizeof buf, "UPDATE fs_gangs SET respect=%d,spawnpoint='%.2f,%.2f,%.2f,%.2f' WHERE id=%d", gData[i][respect], 
			gData[i][gSpawnPoint][0], gData[i][gSpawnPoint][1], gData[i][gSpawnPoint][2], gData[i][gSpawnPoint][3],i);
		mysql_query(buf);
		
	}
	return;
}


OutputGangChat(playerid, szMsg[])
{
	new
	 buffer[200];
	
	format(buffer, sizeof buffer, " (%s) %s: %s", gData[pData[playerid][gang]][tag], GetPlayerProperName(playerid), szMsg);
	
	foreach(i)
	{
		if(pData[i][gang] == pData[playerid][gang])
		{
			SendClientMessage(i, gData[pData[playerid][gang]][gColor] * 256, buffer);
		}
	}
	
	format(buffer, sizeof buffer, "[GANG %s] %s: %s", gData[pData[playerid][gang]][tag], GetPlayerNick(playerid), szMsg);
	OutputLog(LOG_OCHAT, buffer);
}

gangs_MSG(gangid, szMsg[])
{
	if (gangid==NO_GANG) return;
	new
	 buffer[200];
	
	format(buffer, sizeof buffer, " (%s){ffffff} %s", gData[gangid][tag], szMsg);
	
	foreach(i)
		if(pData[i][gang] == gangid)
			SendClientMessage(i, gData[gangid][gColor] * 256, buffer);

	format(buffer, sizeof buffer, "[GANG %s] %s", gData[gangid][tag], szMsg);
	OutputLog(LOG_OCHAT, buffer);
	return;
}

gangs_isPlayerGangOwnerOrLeader(playerid) {
	// select 1 from fs_players_in_gangs where rank in ('owner','leader') AND id_player=239;
	if (!pData[playerid][loggedIn]) return false;
	if (pData[playerid][gang]==NO_GANG) return false;
	new buf[128], bool:res=false;
	format(buf, sizeof buf, "select 1 from fs_players_in_gangs where rank in ('owner','leader') AND id_player=%d;", pData[playerid][accountID]);
	mysql_query(buf);
	mysql_store_result();
	if (mysql_num_rows() && mysql_fetch_int()==1) res=true;
	mysql_free_result();
	return res;
}

CMD:gangprzyjmij(playerid,params[]){
	if (!gangs_isPlayerGangOwnerOrLeader(playerid))
		return Msg(playerid, COLOR_ERROR, "Funkcja tylko dla liderow/wlascicieli klanow");

	new userid,tplayer[24];
	if (isnull(params) && pData[playerid][spectating]!=INVALID_PLAYER_ID)
		userid=pData[playerid][spectating];
	else if (!sscanf(params,"s[24]",tplayer))
		FindUIDByNameOrID(tplayer,userid,playerid);
	else
		return Msg(playerid,COLOR_ERROR,"/gangprzyjmij [id/nick]");

	if (!IsPlayerConnected(userid)) 
		return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
	if (!pData[userid][loggedIn])
		return Msg(playerid,COLOR_ERROR,"Gracz nie jest zarejestrowany!");
	if (pData[userid][gang]==pData[playerid][gang])
		return Msg(playerid,COLOR_ERROR,"Gracz jest juz czlonkiem Twojego klanu");
	if (pData[userid][gang]!=NO_GANG)
		return Msg(playerid,COLOR_ERROR,"Gracz nalezy juz do innego klanu");

	pData[userid][gang]=pData[playerid][gang];
	SetPlayerProperColor(userid);
	new buf[128];
	format(buf,sizeof buf,"INSERT INTO fs_players_in_gangs SET id_gang=%d,id_player=%d,rank='member'", pData[playerid][gang], pData[userid][accountID]);
	mysql_query(buf);
	format(buf, sizeof buf, "_~n~_~n~_~n~~w~Witaj w~n~~b~%s", gData[pData[playerid][gang]][name]);
	GameTextForPlayer(userid,buf,5000,6);
	PlayRandomMusic(userid,5000);

	if (SkinKobiecy(GetPlayerSkin(userid)))
		format(buf,sizeof buf,"%s (%d) dolaczyla do klanu %s", GetPlayerProperName(userid), userid, gData[pData[playerid][gang]][name]);
	else
		format(buf,sizeof buf,"%s (%d) dolaczyl do klanu %s", GetPlayerProperName(userid), userid, gData[pData[playerid][gang]][name]);

	gangs_MSG(pData[playerid][gang], buf);
	if (TeleportAllowed(userid))
		gangs_TeleportPlayerToGangSpawn(userid);

	UpdatePlayerFactionName(userid);
	
	return 1;
}

CMD:gangwyrzuc(playerid,params[]){
	if (!gangs_isPlayerGangOwnerOrLeader(playerid))
		return Msg(playerid, COLOR_ERROR, "Funkcja tylko dla liderow/wlascicieli klanow");

	new userid,tplayer[24];
	if (isnull(params) && pData[playerid][spectating]!=INVALID_PLAYER_ID)
		userid=pData[playerid][spectating];
	else if (!sscanf(params,"s[24]",tplayer))
		FindUIDByNameOrID(tplayer,userid,playerid);
	else
		return Msg(playerid,COLOR_ERROR,"/gangwyrzuc [id/nick]");

	if (!IsPlayerConnected(userid)) 
		return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
	if (!pData[userid][loggedIn])
		return Msg(playerid,COLOR_ERROR,"Gracz nie jest zarejestrowany!");
	if (pData[userid][gang]==NO_GANG)
		return Msg(playerid,COLOR_ERROR,"Gracz nie nalezy do zadnego klanu");
	if (pData[userid][gang]!=pData[playerid][gang])
		return Msg(playerid,COLOR_ERROR,"Gracz nie jest czlonkiem!");

	return gangs_RemovePlayerFromGang(userid);
/*	new buf[128];
	format(buf,sizeof buf,"DELETE FROM fs_players_in_gangs WHERE id_gang=%d AND id_player=%d AND rank='member' LIMIT 1", pData[playerid][gang], pData[userid][accountID]);
	mysql_query(buf);
	pData[userid][gang]=NO_GANG;
	SetPlayerProperColor(userid);

	if (SkinKobiecy(GetPlayerSkin(userid)))
		format(buf,sizeof buf,"%s (%d) zostala wyproszona z klanu %s", GetPlayerProperName(userid), userid, gData[pData[playerid][gang]][name]);
	else
		format(buf,sizeof buf,"%s (%d) zostal wyproszony z klanu %s", GetPlayerProperName(userid), userid, gData[pData[playerid][gang]][name]);

	gangs_MSG(pData[playerid][gang], buf);
	Msg(userid, COLOR_INFO2, buf);
	
	return 1;*/
}

gangs_RemovePlayerFromGang(playerid){
	if (!IsPlayerConnected(playerid)) 
		return printf("DEBUG gangs_RemovePlayerFromGang, ale gracz nei jest polaczony!");
	if (!pData[playerid][loggedIn])
		return printf("DEBUG gangs_RemovePlayerFromGang, ale gracz nei jest zalogowany!");
	new gangid=pData[playerid][gang];
	if (gangid==NO_GANG) return printf("DEBUG gangs_RemovePlayerFromGang, ale gracz nei jest w gangu!");

	new buf[255];

	// kasujemy dom!!! - jesli jest gangowy
	new hid=domy_findHouseByOwnerID(pData[playerid][accountID]);
	if(hid>=0 && FSDomy[hid][ed_restrict_gang]!=NO_GANG && FSDomy[hid][ed_restrict_gang]==pData[playerid][gang]) {
		format(buf,sizeof buf,"UPDATE fs_houses set paidTo='0000-00-00', ownerid=0 WHERE ownerid=%d AND id=%d AND restrict_gang=%d", pData[playerid][accountID], hid, pData[playerid][gang]);
		mysql_query(buf);
		pTemp[playerid][e_houseid]=-1;
		domy_Reload();
	}

	
	format(buf,sizeof buf,"DELETE FROM fs_players_in_gangs WHERE id_gang=%d AND id_player=%d", pData[playerid][gang], pData[playerid][accountID]);
	mysql_query(buf);
	pData[playerid][gang]=NO_GANG;
	SetPlayerProperColor(playerid);

	if (SkinKobiecy(GetPlayerSkin(playerid)))
		format(buf,sizeof buf,"%s (%d) opuscila gang %s", GetPlayerProperName(playerid), playerid, gData[gangid][name]);
	else
		format(buf,sizeof buf,"%s (%d) opuscil gang %s", GetPlayerProperName(playerid), playerid, gData[gangid][name]);

	gangs_MSG(gangid, buf);
	Msg(playerid, COLOR_INFO2, buf);

	UpdatePlayerFactionName(playerid);

	return 1;
}

gangs_SetGangSpawn(playerid){
	if (pData[playerid][gang]==NO_GANG)
		return Msg(playerid, COLOR_ERROR, "Nie nalezysz do zadnego gangu!");
	if(!TeleportAllowed(playerid))
		return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));

	if(GetPlayerVirtualWorld(playerid)>0)
		return Msg(playerid, COLOR_ERROR, "Nie mozesz tutaj ustawic spawnu gangu!");

	if (GetPlayerInterior(playerid)!=0)
		return Msg(playerid, COLOR_ERROR, "Spawn gangu moze byc zapisany tylko na zewnatrz");
	
	new gangid=pData[playerid][gang];
	switch(GetPlayerState(playerid)) {
		case PLAYER_STATE_ONFOOT: {
			GetPlayerPos(playerid, gData[gangid][gSpawnPoint][0], gData[gangid][gSpawnPoint][1], gData[gangid][gSpawnPoint][2]);
			GetPlayerFacingAngle(playerid, gData[gangid][gSpawnPoint][3]);
		}
		case PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER: {
			new vid=GetPlayerVehicleID(playerid);
			GetVehiclePos(vid, gData[gangid][gSpawnPoint][0], gData[gangid][gSpawnPoint][1], gData[gangid][gSpawnPoint][2]);
			GetVehicleZAngle(vid, gData[gangid][gSpawnPoint][3]);
		}
		default:
			return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));
	}
	new buf[127];
	format(buf, sizeof buf, "%s ustanowil%s nowy spawn gangu.", GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""));
	gangs_MSG(gangid, buf);
	return 1;
	
}

gangs_TeleportPlayerToGangSpawn(playerid){
	if (pData[playerid][gang]==NO_GANG)
		return Msg(playerid, COLOR_ERROR, "Nie nalezysz do zadnego gangu!");
	if(!TeleportAllowed(playerid))
		return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));
	new gangid=pData[playerid][gang];

	Teleport(T_BOTH, playerid, gData[gangid][gSpawnPoint][0]+random(2)-1, gData[gangid][gSpawnPoint][1]+random(2)-1, gData[gangid][gSpawnPoint][2]+random(2)-1, gData[gangid][gSpawnPoint][3]+random(10)-5, 0, VW_DEFAULT, true);
	GameTextForPlayer(playerid, "~y~Spawn gangu", 1000, 1);
	return 1;
}

gangs_TeleportPlayerToBaseSpawn(playerid,bool:force=false){

	
	if (pData[playerid][gang]==NO_GANG)
		return Msg(playerid, COLOR_ERROR, "Nie nalezysz do zadnego gangu!");
	if(!force && !TeleportAllowed(playerid))
		return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));
	new gangid=pData[playerid][gang];

	Teleport(T_BOTH, playerid, gData[gangid][gBaseSpawn][0], gData[gangid][gBaseSpawn][1], gData[gangid][gBaseSpawn][2], gData[gangid][gBaseSpawn][3], 0, VW_DEFAULT, true);
	GameTextForPlayer(playerid, "~y~Baza gangu", 1000, 1);
	return 1;
}


CMD:gang(playerid){
	if (pData[playerid][gang]==NO_GANG)
		return gangs_ShowGangsList(playerid);

	new lista[1024];

	strcat(lista,"{000000}B {ffffff}Baza gangu\n");
	strcat(lista,"{000000}S {d0d0d0}Spawn gangu\n");

	if (pData[playerid][gangRank]==GANG_RANK_LEADER || pData[playerid][gangRank]==GANG_RANK_OWNER) {
		strcat(lista,"{000000}U {ffffff}Ustaw spawn\n");
	}
//	if (pData[playerid][gangRank]!=GANG_RANK_LEADER && pData[playerid][gangRank]!=GANG_RANK_OWNER) {
	else {
		strcat(lista,"{000000}E {ffffff}Opusc ");
		format(lista,sizeof lista, "%s{%06x}%s\n", lista, gData[pData[playerid][gang]][gColor], gData[pData[playerid][gang]][tag]);
	}
	
	strcat(lista,"{000000}L {ffffff}Lista klanow na serwerze");
	return ShowPlayerDialog(playerid, DIALOG_GANG_MAINMENU, DIALOG_STYLE_LIST, "Gang", lista, TXT(playerid, 76), TXT(playerid, 77));
}

gangs_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]){
#pragma unused listitem
	switch(dialogid){
		case DIALOG_GANG_MAINMENU: {
			if(!response) return 1;
			switch(inputtext[0]){
				case 'B':	return gangs_TeleportPlayerToBaseSpawn(playerid);
				case 'U':	return gangs_SetGangSpawn(playerid);
				case 'S':	return gangs_TeleportPlayerToGangSpawn(playerid);
				case 'E':	{
					if (pData[playerid][loggedIn]) {
						new hid=domy_findHouseByOwnerID(pData[playerid][accountID]);
						if(hid>=0 && FSDomy[hid][ed_restrict_gang]!=NO_GANG && FSDomy[hid][ed_restrict_gang]==pData[playerid][gang])
							return ShowPlayerDialog(playerid, DIALOG_GANG_CONFIRMLEAVING, DIALOG_STYLE_MSGBOX, "Gang > opusc", "Czy na pewno chcesz opuscic ten gang?\nZ odejsciem wiaze sie utrata domku klanowego!", TXT(playerid, 76), TXT(playerid, 77));
					}

					return ShowPlayerDialog(playerid, DIALOG_GANG_CONFIRMLEAVING, DIALOG_STYLE_MSGBOX, "Gang > opusc", "Czy na pewno chcesz opuscic ten gang?", TXT(playerid, 76), TXT(playerid, 77));
				}
				case 'L':	return gangs_ShowGangsList(playerid);
			}

		}
		case DIALOG_GANG_CONFIRMLEAVING: {
			if(!response) return cmd_gang(playerid);
			gangs_RemovePlayerFromGang(playerid);
		}
		
			
	}
	return 1;
}

gangs_ShowGangDetails(playerid, gangid){
	new buffer[2048],buf2[512];
	new czlonkow, czlonkow_24h, czlonkow_7d,tmpresult[32];

//	printf("Showing for %d",gangid);

	format(buffer, sizeof buffer, "{%06x}%s \t\t {A9C4E4}tag: {FFFFFF}%s\n{A9C4E4}Zalozyciel: {FFFFFF}%s\n{A9C4E4}Data rejestracji: {FFFFFF}%s\n\n{A9C4E4}Liderzy:\t {FFFFFF}",
				gData[gangid][gColor], gData[gangid][name], gData[gangid][tag],
				GetGangOwner(gangid),
				GetGangData(gangid, "datetime_created")
			);

//	printf("a1");
	format(buf2, sizeof buf2, "select p.nick from fs_players_in_gangs pig JOIN fs_players p ON p.id=pig.id_player AND pig.rank='leader' where pig.id_gang=%d ORDER BY nick ASC;", gangid);
	mysql_query(buf2);
	mysql_store_result();
//	printf("wykonano buf2");
	while (mysql_fetch_row(tmpresult))
			if(strlen(tmpresult)>0) {
				strcat(buffer, " ");
				strcat(buffer, tmpresult);
			}

	mysql_free_result();

//	printf("done");

	format(buf2, sizeof buf2, "select count(pig.id_player) czlonkow from fs_players_in_gangs pig where pig.id_gang=%d", gangid);
	sscanf(SQLGetResult(buf2), "D(0)", czlonkow);

	format(buf2, sizeof buf2, "select count(pig.id_player) czlonkow from fs_players_in_gangs pig JOIN fs_players p ON p.id=pig.id_player AND DATEDIFF(NOW(),p.datetime_last)<=1 where pig.id_gang=%d", gangid);
	
	sscanf(SQLGetResult(buf2), "D(0)", czlonkow_24h);
	
	format(buf2, sizeof buf2, "select count(pig.id_player) czlonkow from fs_players_in_gangs pig JOIN fs_players p ON p.id=pig.id_player AND DATEDIFF(NOW(),p.datetime_last)<=7 where pig.id_gang=%d", gangid);
	sscanf(SQLGetResult(buf2), "D(0)", czlonkow_7d);
	
	format(buf2, sizeof buf2, "\n\nRespekt:\t %d\nCzlonkow:\t %d (online w ciagu 24h: %d, w ciagu 7d: %d)", 
				gData[gangid][respect],
				czlonkow, czlonkow_24h, czlonkow_7d
		);
	strcat(buffer, buf2);
	
	new
	 szTitle[32];
			
	format(szTitle, sizeof szTitle, TXT(playerid, 239), gData[gangid][tag]);
		
	return	ShowPlayerDialog(playerid, DIALOG_STATS_GANG_INFO, DIALOG_STYLE_MSGBOX, szTitle, buffer, TXT(playerid, 54), TXT(playerid, 77));
}

CMD:reloadgangs(playerid){
	if (!IsPlayerAdmin(playerid)) return 0;
	SendClientMessageToAll(-1,"Przeladowywanie danych gangow...");
	gangs_saveGangData(true);
	gangs_LoadGangData();
	return 1;
}

gangs_LoadGangData(){
	new buffer[127];
	for(new i = 1; i < MAX_GANGS; i++)
	{ 
		copy(GetGangData(i, "name"), gData[i][name]);
		copy(GetGangData(i, "tag"), gData[i][tag]);
		copy(GetGangData(i, "color"), buffer);
		gData[i][gColor] = HexToInt(buffer);
		gData[i][respect] = StringToInt(GetGangData(i, "respect"));

		copy(GetGangData(i, "basespawn"), buffer);
		sscanf(buffer,"p<,>ffff",gData[i][gBaseSpawn][0], gData[i][gBaseSpawn][1], gData[i][gBaseSpawn][2], gData[i][gBaseSpawn][3]);

		copy(GetGangData(i, "spawnpoint"), buffer);
		sscanf(buffer,"p<,>ffff",gData[i][gSpawnPoint][0], gData[i][gSpawnPoint][1], gData[i][gSpawnPoint][2], gData[i][gSpawnPoint][3]);
	}
}