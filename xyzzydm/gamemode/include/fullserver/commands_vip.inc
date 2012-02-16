forward vip_vbronCB(playerid,bron);

CMD:vsay(playerid, params[])
{
	if(!pData[playerid][vipEnabled] && pData[playerid][adminLevel]<LEVEL_ADMIN3) return Msg(playerid, COLOR_ERROR, TXT(playerid, 414));
	if(pData[playerid][mute] != 0 && pData[playerid][mute] - (GetTickCount() / 1000) > 0 )
		return Msg(playerid,COLOR_ERROR,"Jeste≈õ wyciszony - nie mo≈ºesz pisaƒá na czacie.");
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 413)); // Uøywaj: /vsay <tekst>
	if (gmTemp[chatDisabled]) return Msg(playerid, COLOR_ERROR, "Niestety, czat jest obecnie wylaczony przez administratora");

	if(FilterText(playerid, params) == 0) return 1;
	if(!IsAdmin(playerid)) CensorText(params);
	if(gmData[chatColors]) FilterTextWithColors(params);
	
	new
	 buffer[160];
	
	format(buffer, sizeof buffer, "{FCC000}(VIP) {%06x}%s(%d) {b}%s{/b}", gmData[color_vipSay], GetPlayerProperName(playerid), playerid, params);
	foreach(i)
		Msg(i, COLOR_VIPSAY, buffer);
	
	format(buffer, sizeof buffer, "[VIPSAY] %s %s", GetPlayerNick(playerid), params);
	OutputLog(LOG_OCHAT, buffer);

	return 1;
}

CMD:v(playerid, params[])
{
	if(!pData[playerid][vipEnabled] && !IsAdmin(playerid, LEVEL_GM)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 415)); // Uøywaj: /v <tekst>
	
	if(FilterText(playerid, params) == 0) return 1;
	if(!IsAdmin(playerid)) CensorText(params);
	if(gmData[chatColors]) FilterTextWithColors(params);
	
	OutputVipChat(playerid, params);
	
	return 1;
}

CMD:varmor(playerid,params[])
{
	
	if(!pData[playerid][vipEnabled]  && !IsGM(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 414));
	new tplayer[MAX_PLAYER_NAME],uid,buf[128];
	if (!sscanf(params,"s[24]",tplayer)) {
		FindUIDByNameOrID(tplayer,uid,playerid);
	} else 
		uid=playerid;

	if(!IsPlayerSpawned(uid) || !AttractionAllowed(uid)) {
		format(buf,sizeof buf, "{b}%s{/b} jest obecnie nieosiagaln%s.", GetPlayerProperName(uid), SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : ("y") );
		return Msg(playerid,COLOR_ERROR,buf);
	}
	
	SetPlayerArmour(uid,100.0);
	if (uid==playerid) {
		if (random(2)==1)
			Msg(playerid,COLOR_VIP,Kobieta(playerid)?("Uzupelnilas sobie kamizelke."):("Dodales sobie armora."));
		else
			Msg(playerid,COLOR_VIP,Kobieta(playerid)?("Dodalas sobie armora."):("Uzupelniles sobie kamizelke."));
		return 1;
	}

	format(buf, sizeof buf, "VIP {b}%s{/b}(%d) dodal%s Ci armora", GetPlayerProperName(playerid), playerid, SkinKobiecy(GetPlayerSkin(playerid)) ? ("a") : (""));
	Msg(uid,COLOR_VIP,buf);
	GameTextForPlayer(uid,"~y~AR~w~+", 250,6);
	format(buf, sizeof buf, "Dodal%ss armora graczowi {b}%s{/b}.", SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : ("e"), GetPlayerProperName(uid));
	Msg(playerid,COLOR_INFO, buf, false);
	return 1;
}

CMD:vheal(playerid,params[])
{

	if(!pData[playerid][vipEnabled] && !IsGM(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 414));
	new tplayer[MAX_PLAYER_NAME],uid,buf[128];
	if (!sscanf(params,"s[24]",tplayer)) {
		FindUIDByNameOrID(tplayer,uid,playerid);
	} else 
		uid=playerid;

	if(!IsPlayerSpawned(uid) || !AttractionAllowed(uid)) {
		format(buf,sizeof buf, "{b}%s{/b} jest obecnie nieosiagaln%s.", GetPlayerProperName(uid), SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : ("y") );
		return Msg(playerid,COLOR_ERROR,buf);
	}
	
	SetPlayerHealth(uid,100.0);

	if (uid==playerid) {
		if (random(2)==1)
			Msg(playerid,COLOR_VIP,Kobieta(playerid)?("Uleczylas sie."):("Uleczyles sie."));
		else
			Msg(playerid,COLOR_VIP,Kobieta(playerid)?("Uzupelnilas sobie punkty HP."):("Uzupelniles sobie punkty HP."));
		return 1;
	}
	
	format(buf, sizeof buf, "VIP {b}%s{/b}(%d) uzupelnil%s Twoj poziom HP.", GetPlayerProperName(playerid), playerid, SkinKobiecy(GetPlayerSkin(playerid)) ? ("a") : (""));
	Msg(uid,COLOR_VIP,buf);
	GameTextForPlayer(uid,"~g~HP~w~+", 250,6);
	format(buf, sizeof buf, "Uleczyl%ss gracza {b}%s{/b}.", SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : ("e"), GetPlayerProperName(uid));
	Msg(playerid,COLOR_INFO, buf, false);
	return 1;
}

CMD:vnapraw(playerid,params[])
{
	
	if(!pData[playerid][vipEnabled]  && !IsGM(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 414));
	new tplayer[MAX_PLAYER_NAME],uid,buf[128];
	if (!sscanf(params,"s[24]",tplayer)) {
		FindUIDByNameOrID(tplayer,uid,playerid);
	} else 
		uid=playerid;

	if(!IsPlayerSpawned(uid) || !AttractionAllowed(uid)) {
		format(buf,sizeof buf, "{b}%s{/b} jest obecnie nieosiagaln%s.", GetPlayerProperName(uid), SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : ("y") );
		return Msg(playerid,COLOR_ERROR,buf);
	}
	if (!IsPlayerInAnyVehicle(uid)) {
		format(buf,sizeof buf, "{b}%s{/b} nie jest obecnie w zadnym pojezdzie.", GetPlayerProperName(uid));
		return Msg(playerid,COLOR_ERROR, buf);
	}
	RepairVehicle(GetPlayerVehicleID(uid));
	SetVehicleHealth(GetPlayerVehicleID(uid), VEHICLE_DEFAULT_HP);

	if (uid==playerid) 
		return	Msg(playerid,COLOR_VIP,Kobieta(playerid)?("Naprawilas swoj pojazd."):("Naprawiles swoj pojazd."));

	format(buf, sizeof buf, "VIP {b}%s{/b}(%d) naprawil%s Ci pojazd", GetPlayerProperName(playerid), playerid, SkinKobiecy(GetPlayerSkin(playerid)) ? ("a") : (""));
	Msg(uid,COLOR_VIP,buf);
	GameTextForPlayer(uid,"~y~VRP~w~+", 250,6);
	format(buf, sizeof buf, "Naprawil%ss pojazd gracza {b}%s{/b}.", SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : ("e"), GetPlayerProperName(uid));
	Msg(playerid,COLOR_INFO, buf, false);
	return 1;
}


CMD:vpomoc(playerid)
{
	pTemp[playerid][ept_dialogid]=DIALOG_HELP_CMD;
	OnDialogResponse(playerid, DIALOG_HELP_CMD, BUTTON_NEXT, f_h_CMD_VIP, "");
	return 1;
}

CMD:vtt(playerid,params[]){
	if(!pData[playerid][vipEnabled] && !IsGM(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 414));
	MUST_BE_SPAWNED(playerid);
	if(!TeleportAllowed(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie mozesz teraz tego zrobic!");

	if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uzywaj: {b}/vtt <id/nick>{/b}");

	new
	 targetplayerid = FindPlayer(params),
	 buffer[128];
	
	if(targetplayerid == ERROR_NOT_FOUND)
	{
		format(buffer, sizeof buffer, TXT(playerid, 27), params);
		return Msg(playerid, COLOR_ERROR, buffer);
	}
	if(targetplayerid == ERROR_FOUND_MULTIPLE)
	{
		format(buffer, sizeof buffer, TXT(playerid, 28), params);
		return Msg(playerid, COLOR_ERROR, buffer);
	}
	
	if(targetplayerid == playerid) return 1;
	new tpstate=GetPlayerState(targetplayerid);
	if (tpstate==PLAYER_STATE_NONE || tpstate==PLAYER_STATE_WASTED || tpstate==PLAYER_STATE_SPECTATING)
		return Msg(playerid,COLOR_ERROR,"Gracz nie jest obecnie zespawnowany");
	
	pTemp[targetplayerid][TPInv]=playerid+1;
	
	format(buffer, sizeof buffer, "%s(%d) chce sie przeniesc {b}do Ciebie{/b}. Wpisz {b}/vtak{/b} aby wyrazic zgode.", GetPlayerProperName(playerid), playerid);
	Msg(targetplayerid, COLOR_VIPSAY, buffer);

	Msg(playerid, COLOR_INFO2, "Wyslano zaproszenie do teleportacji.");

	GameTextForPlayer(targetplayerid,"~g~TP", 500,6);
	
	return 1;

}


CMD:vth(playerid,params[]){
	if(!pData[playerid][vipEnabled]  && !IsGM(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 414));
	MUST_BE_SPAWNED(playerid);
	if(!TeleportAllowed(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie mozesz teraz tego zrobic!");

	if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uzywaj: {b}/vtt <id/nick>{/b}");

	new
	 targetplayerid = FindPlayer(params),
	 buffer[128];
	
	if(targetplayerid == ERROR_NOT_FOUND)
	{
		format(buffer, sizeof buffer, TXT(playerid, 27), params);
		return Msg(playerid, COLOR_ERROR, buffer);
	}
	if(targetplayerid == ERROR_FOUND_MULTIPLE)
	{
		format(buffer, sizeof buffer, TXT(playerid, 28), params);
		return Msg(playerid, COLOR_ERROR, buffer);
	}
	
	if(targetplayerid == playerid) return 1;
	new tpstate=GetPlayerState(targetplayerid);
	if (tpstate==PLAYER_STATE_NONE || tpstate==PLAYER_STATE_WASTED || tpstate==PLAYER_STATE_SPECTATING)
		return Msg(playerid,COLOR_ERROR,"Gracz nie jest obecnie zespawnowany");
	

	pTemp[targetplayerid][TPInv]=-1 * (playerid+1);
	
	format(buffer, sizeof buffer, "(VIP) %s(%d) chce Cie przeniesc {b}do siebie{/b}. Wpisz {b}/vtak{/b} aby wyrazic zgode.", GetPlayerProperName(playerid), playerid);
	Msg(targetplayerid, COLOR_VIPSAY, buffer);

	if (!TeleportAllowed(targetplayerid)) return Msg(playerid,COLOR_INFO2, "Wskazany gracz nie moze sie teraz teleportowac, zostalo do niego wyslane zaproszenie z ktorego bedzie mogl skorzystac", false);
	else Msg(playerid, COLOR_INFO2, "Wyslano zaproszenie do teleportacji.", false);

	GameTextForPlayer(targetplayerid,"~g~TP", 500,6);
	
	return 1;

}

CMD:vtak(playerid){
	MUST_BE_SPAWNED(playerid);
	if(!TeleportAllowed(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie mozesz teraz sie nigdzie przeniesc.");
	new buf[128];
//	format(buf, sizeof buf, "tpinv %d", pTemp[playerid][TPInv]);
//	SendClientMessageToAll(-1, buf);

	if (pTemp[playerid][TPInv]==0)
		return Msg(playerid,COLOR_ERROR,"Nie masz zadnych aktywnych zaproszen do teleportacji.");
	new uid,bool:ktt;
	uid=pTemp[playerid][TPInv];
	if (uid<0) 
		uid*=-1;	
	else
		ktt=true;

	uid--;

	if (!IsPlayerConnected(uid))
		return Msg(playerid,COLOR_ERROR,"Nie masz zadnych aktywnych zaproszen do teleportacji.");
	if (!TeleportAllowed(uid))
		return Msg(playerid,COLOR_ERROR,"Niestety, nie mozesz sie obecnie przeniesc do tego gracza.");
	format(buf, sizeof buf, "{b}%s(%d){/b} zgodzil%s sie na teleportacje", GetPlayerProperName(playerid), playerid, SkinKobiecy(GetPlayerSkin(playerid)) ? ("a") : (""));
	Msg(uid, COLOR_INFO, buf);

	new Float:pP[4];
	if (ktt) {
		GetPlayerPos(playerid,pP[0], pP[1], pP[2]);
		SetPlayerPos(uid, pP[0], pP[1], pP[2]);
		SetPlayerInterior(uid, GetPlayerInterior(playerid));
		SetPlayerVirtualWorld(uid, GetPlayerVirtualWorld(playerid));
	} else {
		GetPlayerPos(uid,pP[0], pP[1], pP[2]);
		SetPlayerPos(playerid, pP[0], pP[1], pP[2]);
		SetPlayerInterior(playerid, GetPlayerInterior(uid));
		SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(uid));
	}
	pTemp[playerid][TPInv]=0;
	return 1;
}

CMD:vpozostalo(playerid){
	new buffer[127];
	if (!pData[playerid][vipEnabled])
		return Msg(playerid,COLOR_ERROR,"Nie posiadasz uprawnien {b}VIP{/b}. Aby je uzyskac, odwiedz {b}http://fullserver.eu/doladowania/{/b}");
	format(buffer, sizeof buffer, "Twoje konto {b}VIP{/b} jest aktywne do {b}%s{/b} (jeszcze przez {b}%d %s{/b})", pData[playerid][vipToDate],
			pData[playerid][vipDaysLeft], dli(pData[playerid][vipDaysLeft], "dzien", "dni", "dni"));
	return Msg(playerid,COLOR_VIP, buffer);
}

CMD:vbron(playerid) {
	if (!pData[playerid][vipEnabled])
		return Msg(playerid,COLOR_ERROR,"Nie posiadasz uprawnien {b}VIP{/b}. Aby je uzyskac, odwiedz {b}http://fullserver.eu/doladowania/{/b}");
	if (pData[playerid][pAttraction]!=A_NONE)
		return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic!");

	return ShowWeaponSelectMenu(playerid, "Wybierz bron", "vip_vbronCB", -1);
}


public vip_vbronCB(playerid,bron){
	if (pData[playerid][pAttraction]!=A_NONE)
		return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz zakupic broni!");
	new wi;
	for (new i=0; i<sizeof quickbuyWeapons;i++)
		if (quickbuyWeapons[i][ewd_id]==bron) {	wi=i; break; }

	GivePlayerWeapon(playerid,quickbuyWeapons[wi][ewd_id],quickbuyWeapons[wi][ewd_baseAmmo]);

	return 1;
}

CMD:vzestaw(playerid){
	if (!pData[playerid][vipEnabled])
        return Msg(playerid,COLOR_ERROR,"Nie posiadasz uprawnien {b}VIP{/b}. Aby je uzyskac, odwiedz {b}http://fullserver.eu/doladowania/{/b}");
	if (pData[playerid][pAttraction]!=A_NONE)
		return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic!");

	

	GivePlayerWeapon(playerid, 24, 100);
	GivePlayerWeapon(playerid, 31, 100);
	GivePlayerWeapon(playerid, 34, 100);
	GivePlayerWeapon(playerid, 9, 1);
	SetPlayerAmmo(playerid,9,1);
	GivePlayerWeapon(playerid, 28, 100);

	new vid=GetPlayerVehicleID(playerid);
	if (vid!=0 && vid!=INVALID_VEHICLE_ID && tVehicles[vid][vo_drift])	// w pojezdzie drifterskim nie wolno miec broni
		SetPlayerArmedWeapon(playerid,0);
	else if (GetPlayerState(playerid)==PLAYER_STATE_PASSENGER)		// nie zmieniamy pasazerowi broni
		SetPlayerArmedWeapon(playerid,0);

	
	return Msg(playerid,COLOR_VIP,"Otrzymujesz {b}zestaw broni VIPa{/b}");
}

