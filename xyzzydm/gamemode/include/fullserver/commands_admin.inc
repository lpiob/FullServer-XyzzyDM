// todo: przetlumaczyc

CMD:autodrift(playerid){
	if (!IsPlayerAdmin(playerid)) return 0;
	new vid=GetPlayerVehicleID(playerid);
	printf("vid: %d", vid);
	if (vid==0 || vid==INVALID_VEHICLE_ID)
		return Msg(playerid,COLOR_ERROR,"Wsiadz do pojazdu");
	if (tVehicles[vid][vo_drift])
		return Msg(playerid,COLOR_ERROR,"Te auto juz jest drifterskie");
	if (TuneCar(GetPlayerVehicleID(playerid))) {
		PlaySound(playerid, 1134);
		tVehicles[vid][vo_drift]=true;
		Msg(playerid,COLOR_INFO,"Zupgradowales auto do driftu");
	} else {
		Msg(playerid,COLOR_ERROR,"W tym aucie nie da sie zainstalowac przerobek drifterskich!");
	}

	return 1;
}

CMD:alogin(playerid,params[]){
	new buf[128];
	if (IsAdmin(playerid))
		return SetPlayerDrunkLevel(playerid,5000);

	if (pData[playerid][allowedLevel]<LEVEL_ADMIN1 || pData[playerid][allowedLevel]>=LEVEL_ADMIN3) {
		TextDrawShowForPlayer(playerid, gTextDraw[TD_WARNING]);
		SendClientMessage(playerid,0xff0000ff,"-------------------------------------------------");
		SendClientMessage(playerid,0xff0000ff,"Uwaga! {ffffff}Wszystkie proby nieautoryzowanego dostepu");
		SendClientMessage(playerid,0xffffffff,"sa rejestrowane i beda scigane z pelna surowoscia.");
		SendClientMessage(playerid,0xffffffff,"Powtarzajace sie przypadki sa zglaszane bezposrednio");
		SendClientMessage(playerid,0xffffffff,"na policje. {ff0000}Zastanow sie trzy razy, zanim zaczniesz.");
		SendClientMessage(playerid,0xff0000ff,"-------------------------------------------------");	
		PlaySound(playerid,1053);	
		SetTimerEx("HidePlayerWarning",10000,false,"d",playerid);

		if (pData[playerid][allowedLevel]<LEVEL_ADMIN1) {	// nieudanych prob logowania hiddenow nie pokazujemy
			format(buf, sizeof buf, "Proba logowania na admin bez autoryzacji przez {b}%s{/b} (%d).", GetPlayerNick(playerid), playerid);
			MSGToAdmins(COLOR_ERROR, buf, true, LEVEL_ADMIN2);
			format(buf, sizeof buf, "{b}%s{/b} (%d) wprowadzil haslo: '{b}%s{/b}'", GetPlayerNick(playerid), playerid, params);
			MSGToAdmins(COLOR_ERROR, buf, true, LEVEL_ADMIN3);
		}
		pTemp[playerid][warningReceived]=GetTickCount();

		return 1;
	}

	if (isnull(params))
		return Msg(playerid,COLOR_INFO,"/alogin {b}<haslo>{/b}");
	else if (strlen(params)<6 || params[strlen(params)-1]!=0x20)
		return Msg(playerid,COLOR_ERROR,"Bledne haslo. Twoja proba zostala zarejestrowana, zastanow sie czy chcesz zgadywac dalej...");

	format(buf,sizeof buf,"%s%s",SALT_ALOGIN,params);
//	new buf2[128];
//	format(buf2, sizeof buf2, "params %s md5 %s gmtemp %s strcmp %d", params, MD5_Hash(buf), gmTemp[AdminPasswordHash], strcmp(MD5_Hash(buf), gmTemp[AdminPasswordHash],true, 32));
//	SendClientMessageToAll(-1,buf2);
	
	if (/*strlen(gmTemp[AdminPasswordHash])==32 &&*/pData[playerid][allowedLevel]>=LEVEL_ADMIN1 && strcmp(MD5_Hash(buf), gmTemp[AdminPasswordHash],true,32)==0) {
		pData[playerid][adminLevel]=pData[playerid][allowedLevel];
		SetPlayerProperColor(playerid);

		new
		 szPlayerName[24];
		
		GetPlayerName(playerid, szPlayerName, sizeof szPlayerName);

		format(buf, sizeof buf, "{b}%s{/b} zalogowal%s sie jako administrator %s", szPlayerName, Kobieta(playerid) ? ("a") : (""), pData[playerid][adminLevel]==LEVEL_ADMIN3?("{b}RCON{/b}"):(""));

		foreach(i)
		{
			Msg(i, (pData[playerid][adminLevel]==LEVEL_ADMIN3 ? COLOR_ADMIN3 : COLOR_ADMIN), buf, false); // "xxx" zalogowa³ siê na admina/moderatora (poziom xxx).
			PlaySound(i, 1133);
		}


	} else {
		if (pData[playerid][allowedLevel]<=LEVEL_ADMIN3) {	// hiddenow nie logujemy
			format(buf, sizeof buf, "Nieudana proba autoryzacji admina: {b}%s{/b} (%d).", GetPlayerNick(playerid), playerid);
			MSGToAdmins(COLOR_ERROR, buf, true, LEVEL_ADMIN2);
			format(buf, sizeof buf, "{b}%s{/b} (%d) wprowadzil haslo: '{b}%s{/b}'", GetPlayerNick(playerid), playerid, params);
			MSGToAdmins(COLOR_ERROR, buf, true, LEVEL_ADMIN3);
		}
		pTemp[playerid][warningReceived]=GetTickCount();

		return Msg(playerid,COLOR_ERROR,"Bledne haslo. Twoja proba zostala zarejestrowana, zastanow sie czy chcesz zgadywac dalej...");
	}
	UpdatePlayerNickTD(playerid);	
	return 1;
}

CMD:ahaslo(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3) || !IsPlayerAdmin(playerid)) return 0;	// nadmierna ostroznosc?

	if (isnull(params))
		return Msg(playerid,COLOR_INFO,"/ahaslo {b}<haslo>{/b}");
	if (strlen(params)<6)
		return Msg(playerid,COLOR_ERROR,"Minimalnie 6 znakow");
	if (strlen(params)>(32-strlen(SALT_ALOGIN)))
		return Msg(playerid,COLOR_ERROR,"Zbyt dlugie!");

	new buf[128],newpass[32];
	format(newpass,sizeof newpass,"%s%s ", SALT_ALOGIN, params);		// !!! SPACJA NA KONCU !!! admin ustawiajac haslo nie musi jej podawac, ale admin logujacy sie tak
	format(buf,sizeof buf,"UPDATE fs_config SET value='%s' WHERE option_name='admin_password'", MD5_Hash(newpass));
	if (mysql_query(buf)==0)
		copy(MD5_Hash(newpass), gmTemp[AdminPasswordHash]);
//		Msg(playerid, COLOR_INFO, gmTemp[AdminPasswordHash]);
	else
		return Msg(playerid,COLOR_ERROR,"Wystapil blad przy aktualizacji hasla. Brak polaczenia z baza danych?");
	return Msg(playerid,COLOR_INFO2,"Haslo zaktualizowane i zapisane");
}

CMD:tpto(playerid,params[]){
    if(!IsAdmin(playerid)) return 0;
    if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));

    new Float:P[3],int,vw;

    if (sscanf(params,"fffI(-1)I(-1)",P[0],P[1],P[2],int,vw))
	return Msg(playerid, COLOR_ERROR, "/tpto <X> <Y> <Z> [interior] [virtualworld]");

    SetPlayerPos(playerid,P[0],P[1],P[2]);
    if (int>-1) 
	SetPlayerInterior(playerid,int);
    if (vw>-1)
	SetPlayerVirtualWorld(playerid,vw);

    SetCameraBehindPlayer(playerid);
    return 1;
}

CMD:getpos(playerid,params[]){
    if(!IsAdmin(playerid)) return 0;
    new Float:P[3],int,vw,Float:fa,str[128],userid,Float:MAz;
    if (sscanf(params,"d",userid))
	userid=playerid;
    else if (!IsPlayerConnected(userid)) 
	return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
    GetPlayerPos(playerid,P[0],P[1],P[2]);
	new vid=GetPlayerVehicleID(playerid);
	if (vid>0) 
		GetVehicleZAngle(vid,fa);
	else
		GetPlayerFacingAngle(playerid,fa);
    int=GetPlayerInterior(playerid);
    vw=GetPlayerVirtualWorld(playerid);

//	printf("%.2f, %.2f, %.2f, %.2f",P[0],P[1],P[2],fa);
    format(str,sizeof(str),"XYZ: {ff0000}%.2f %.2f %.2f {ffffff}%.2f",P[0],P[1],P[2],fa);
    Msg(playerid,0xffffffff,str,false);

	format(str,sizeof str ,"Interior: {ff0000}%d{ffffff}, vw: {ff0000}%d",int,vw);
	Msg(playerid,0xffffffff,str,false);

	if (int==0) {
		MapAndreas_FindZ_For2DCoord(P[0],P[1],MAz);
		format(str,sizeof(str),"Wysokosc mapy w podanym X,Y wg MapAndreas: {b}%.4f{/b}", MAz);
		Msg(playerid,COLOR_INFO2,str,false);
	}
	return 1;
}

CMD:setip(playerid,params[]){
    if(!IsAdmin(playerid)) return 0;
    new int,userid;
    if (sscanf(params,"dd",userid,int))
	return Msg(playerid,COLOR_ERROR,"/setip <id gracza> <interior>");
    else if (!IsPlayerConnected(userid)) 
	return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
    else if (int<0 || int>20) 
	return Msg(playerid,0xffffffff,"Nieprawidlowy numer interioru!");

    SetPlayerInterior(userid,int);
    // todo powiadomienie dla gracza i admina
    return 1;
}

CMD:setworld(playerid,params[]){
    if(!IsAdmin(playerid)) return 0;
    new vw,userid;
    if (sscanf(params,"dd",userid,vw))
	return Msg(playerid,COLOR_ERROR,"/setworld <id gracza> <vw>");
    else if (!IsPlayerConnected(userid)) 
	return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
    else if (vw<0 || vw>9999) 
	return Msg(playerid,0xffffffff,"Nieprawidlowy numer vw!");
    SetPlayerVirtualWorld(userid,vw);
    // todo powiadomienie dla gracza i admina
    return 1;
}

CMD:sethp(playerid,params[]){
    if(!IsAdmin(playerid,LEVEL_ADMIN2)) return 0;
    new hpv,arv,userid;
    if (sscanf(params,"ddD(-1)",userid,hpv,arv))
	return Msg(playerid,COLOR_ERROR,"/sethp <id gracza> <ile hp> [ile ar]");
    else if (!IsPlayerConnected(userid)) 
	return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
	if (arv>-1) SetPlayerArmour(userid,arv);
	SetPlayerHealth(userid,hpv);
    // todo powiadomienie dla gracza i admina
    return 1;
}


CMD:up(playerid,params[]){
    if (!IsAdmin(playerid)) return 0;
    new Float:dist,userid,Float:P[3];

	if (isnull(params))
		return Msg(playerid,COLOR_ERROR,"/up [id] <odleglosc>");

	if (!sscanf(params,"df",userid,dist)) {
		if (!IsPlayerConnected(userid))
			return Msg(playerid,COLOR_ERROR,"Gracz nie jest podlaczony");
		else if (!IsPlayerSpawned(userid))
			return Msg(playerid,COLOR_ERROR,"Gracz nie jest zespawnowany");

	} if (!sscanf(params,"f",dist)) {
		if (pData[playerid][spectating]!=INVALID_PLAYER_ID)
			userid=pData[playerid][spectating];
	    else
	        userid=playerid;
	}

    if (IsPlayerInAnyVehicle(userid)) {
		new vehid;
		vehid=GetPlayerVehicleID(userid);
		GetVehiclePos(vehid,P[0],P[1],P[2]);

		new Float:V[3];
		GetVehicleVelocity(vehid,V[0],V[1],V[2]);

		SetVehiclePos(vehid,P[0],P[1],P[2]+dist);
		SetVehicleVelocity(vehid,V[0],V[1],V[2]);
    } else {
		GetPlayerPos(userid,P[0],P[1],P[2]);
		SetPlayerPos(userid,P[0],P[1],P[2]+dist);
    }

    // todo powiadomienie
    //return cmd_getpos(playerid,IntToString(userid));
    return 1;

}

CMD:uping(playerid,params[]){
    return cmd_up(playerid,params);
}

CMD:downing(playerid,params[]){
    return cmd_up(playerid,IntToString(StringToInt(params)*-1));
}

CMD:explode(playerid,params[]){
    if (!IsAdmin(playerid)) return 0;
	new bool:force;
	if (params[strlen(params)-1]=='!') {
		force=true;
		params[strlen(params)-1]=0;
	}
    new userid;
    if (sscanf(params,"d",userid)) {
		if (pData[playerid][spectating]!=INVALID_PLAYER_ID)
		    userid=pData[playerid][spectating];
		else
	   	    return Msg(playerid,COLOR_ERROR,"/explode [id gracza] [!]");
    }

    if (!IsPlayerConnected(userid)) 
	return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");

	if (!force) {
		new pstate=GetPlayerState(userid);
		switch(pstate){
			case PLAYER_STATE_SPAWNED, PLAYER_STATE_ONFOOT:
				force=true;
			case PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER:
				return Msg(playerid, COLOR_ERROR,"Najpierw wyjmij gracza z pojadu lub uzyj {b}/explode [id] !{/b} aby wymusic.");
			case PLAYER_STATE_SPECTATING:
				if (userid!=playerid)
					return Msg(playerid, COLOR_ERROR, "Wskazany graczy nie jest zespawnowany");
				else
					force=true;
			default:
				return Msg(playerid, COLOR_ERROR, "Wskazany gracz nie jest zespawnowany");
		}
	}

	if (force) {
		if (pTemp[userid][staleTime]>5)
			Msg(playerid, COLOR_INFO2, "Gracz jest AFK.", false);

		new Float:P[3],Float:HP,Float:AR;
		GetPlayerHealth(userid, HP);
		GetPlayerArmour(userid, AR);
		if (HP>100)
			Msg(playerid, COLOR_INFO2,"Gracz posiada goda (ze strony serwera).", false);
		GetPlayerPos(userid,P[0],P[1],P[2]);
		CreateExplosion(P[0],P[1],P[2], 2, 25.0);

		format(gstr, sizeof gstr, "%03.1f HP / %03.1f AR", HP, AR);
		GameTextForPlayer(playerid, gstr, 100, 5);

		explode_After(playerid,userid,2);

	} else {	// shouldn't happen
		Msg(playerid, COLOR_INFO2, "/explode nie wykonany. To sie nie powinno zdarzyc.");
	}
    return 1;
}

CMD:exp(playerid,params[])
	return cmd_explode(playerid,params);

CMD:weather(playerid,params[]){
	if (!IsAdmin(playerid)) return 0;
	new wid;
	if (sscanf(params,"d",wid)) 
		return Msg(playerid,COLOR_ERROR,"/weather {b}<id pogody>{/b}");
	if (wid<0 || wid>45)
		return Msg(playerid,COLOR_ERROR,"Podany id pogody poza dopuszczalnym zakresem (0-45)!");
	gmTemp[currentWeather]=wid;
	SetWeather(gmTemp[currentWeather]);
	gmTemp[lastWeatherChangeTime]=GetTickCount()/1000;
	if (pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN)
		return 1;
	new str[80];
	format(str,sizeof str,"Pogoda zmieniona na {b}%d{/b} przez {b}%s{/b}.", wid, GetPlayerNick(playerid));
	MSGToAdmins(COLOR_INFO, str, false, LEVEL_GM);
	return 1;
}

CMD:zestaw(playerid){
	if (!IsGM(playerid)) return 0;
	ResetPlayerWeapons(playerid);
//	GivePlayerWeapon(playerid,1,1);	// brass knuckles	0
	GivePlayerWeapon(playerid,4,1);	// noz				1
	GivePlayerWeapon(playerid,24,1000);	// deagle		2
	GivePlayerWeapon(playerid,27,1000);	// combat shot	3
	GivePlayerWeapon(playerid,29,1000);	// mp5			4
	GivePlayerWeapon(playerid,31,1000);	// m4 			5
	GivePlayerWeapon(playerid,34,1000);	// sniper		6
	if (pData[playerid][adminLevel]>=LEVEL_ADMIN1)
		GivePlayerWeapon(playerid,38,1000);	// minigun		7
	GivePlayerWeapon(playerid,16,1000);	// granaty		8
	GivePlayerWeapon(playerid,42,1000);	// gasnica		9
//	GivePlayerWeapon(playerid,46,1);	// spadochron	11
//	GivePlayerWeapon(playerid,15,1);	// laska     	12
	SetPlayerArmour(playerid,100.0);
	SetPlayerHealth(playerid,100);
	if (pData[playerid][adminLevel]>=LEVEL_ADMIN1)
		Msg(playerid,COLOR_INFO,"Otrzymales zestaw broni admina");
	else
		Msg(playerid,COLOR_INFO,"Otrzymales zestaw broni GMa");
	return 1;	
}

CMD:zestaw2(playerid){
	if (!IsGM(playerid)) return 0;
	ResetPlayerWeapons(playerid);
//	GivePlayerWeapon(playerid,1,1);	// brass knuckles	0
	GivePlayerWeapon(playerid,4,1);	// noz				1
	GivePlayerWeapon(playerid,24,1000);	// deagle		2
	GivePlayerWeapon(playerid,27,1000);	// combat shot	3
	GivePlayerWeapon(playerid,29,1000);	// mp5			4
	GivePlayerWeapon(playerid,31,1000);	// m4 			5
	GivePlayerWeapon(playerid,34,1000);	// sniper		6
	GivePlayerWeapon(playerid,16,1000);	// granaty		8

//	GivePlayerWeapon(playerid,46,1);	// spadochron	11
//	GivePlayerWeapon(playerid,15,1);	// laska     	12
	SetPlayerArmour(playerid,100.0);
	SetPlayerHealth(playerid,100);
	if (pData[playerid][adminLevel]>=LEVEL_ADMIN1)
		Msg(playerid,COLOR_INFO,"Otrzymales zestaw broni admina");
	else
		Msg(playerid,COLOR_INFO,"Otrzymales zestaw broni GMa");
	return 1;	
}

CMD:zestaw3(playerid){
	if (!IsAdmin(playerid)) return 0;
	ResetPlayerWeapons(playerid);
//	GivePlayerWeapon(playerid,1,1);	// brass knuckles	0
//	GivePlayerWeapon(playerid,4,1);	// noz				1
	GivePlayerWeapon(playerid,23,1000);	// silenced
	GivePlayerWeapon(playerid,26,1000);	// sawnoff   t	3
	GivePlayerWeapon(playerid,28,1000);	// mp5			4
	GivePlayerWeapon(playerid,30,1000);	// ak 			5
	GivePlayerWeapon(playerid,33,1000);	// sniper		6
	GivePlayerWeapon(playerid,17,1000);	// granaty		8
	GivePlayerWeapon(playerid,35,1000);	// rpg			7
	GivePlayerWeapon(playerid,41,1000);	// spray		
//	GivePlayerWeapon(playerid,46,1);	// spadochron	11
	GivePlayerWeapon(playerid,15,1);	// laska     	10
	SetPlayerArmour(playerid,100.0);
	SetPlayerHealth(playerid,100);
	if (pData[playerid][adminLevel]>=LEVEL_ADMIN1)
		Msg(playerid,COLOR_INFO,"Otrzymales zestaw broni admina");
	else
		Msg(playerid,COLOR_INFO,"Otrzymales zestaw broni GMa");
	return 1;	
}


CMD:god(playerid,params[]){
    if (!IsAdmin(playerid)) return 0;
    new userid,buf[128];
    if (sscanf(params,"d",userid))
	return Msg(playerid,COLOR_ERROR,"/god <id gracza>");
    if (!IsPlayerConnected(userid)) 
	return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
    SetPlayerHealth(userid,9999999.0);
    // todo powiadomienie dla gracza
	if(gmData[artefactOwner] == userid) DropArtefact(userid);
	if(userid!=playerid) {

		format(buf,sizeof buf, "{b}%s (%d){/b} otrzymal%s goda od {b}%s{/b}", GetPlayerProperName(userid), userid, Kobieta(userid)?("a"):(""), GetPlayerProperName(playerid));
		MSGToAdmins(COLOR_INFO, buf, false);
		if (random(2)==1)
			format(buf,sizeof buf, "Administrator%s {b}%s{/b} uczynil Cie niesmierteln%s!", Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(userid)?("a"):("ym"));
		else
			format(buf,sizeof buf, "Otrzymal%s {b}niesmiertelnosc{/b} od admin%s {b}%s{/b}!", Kobieta(userid)?("as"):("es"), Kobieta(playerid)?("ki"):("a"), GetPlayerProperName(playerid));
		Msg(userid, COLOR_ADMIN, buf);
	}
    return 1;
}

CMD:godall(playerid,params[]){
    if (!IsAdmin(playerid)) return 0;
	new cnt,buf[128];
	foreach(i)
	if ((pData[i][pAttraction]==A_NONE || pData[i][pAttraction]==A_ARENA) && pTemp[i][onArena]!=ARENA_SOLO && !pTemp[i][troll]){
		if (random(2)==1)
			format(buf,sizeof buf, "Administrator%s {b}%s{/b} uczynil Cie niesmierteln%s!", Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(i)?("a"):("ym"));
		else
			format(buf,sizeof buf, "Otrzymal%s {b}niesmiertelnosc{/b} od admin%s {b}%s{/b}!", Kobieta(i)?("as"):("es"), Kobieta(playerid)?("ki"):("a"), GetPlayerProperName(playerid));
		Msg(i, COLOR_ADMIN, buf);

	    SetPlayerHealth(i,9999999.0);
		cnt++;
	}
	format(buf,sizeof buf,"{b}%s{/b} daÅ‚a niesmiertelnoÅ›Ä‡ {b}%d %s{/b}", GetPlayerProperName(playerid), cnt, dli(cnt,"graczowi","graczom","graczom"));
	MSGToAdmins(COLOR_INFO, buf, false);
    // todo powiadomienie dla gracza
    return 1;
}




CMD:rspauta(playerid,params[]){
    if (!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
    RespawnVehicles(true);
    return 1;
}

CMD:kick(playerid, params[])
{

	if(!IsAdmin(playerid, LEVEL_GM)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 149)); // U¿ywaj: /kick <gracz> <powód>
	
	new
	 szPlayerName[24],
	 targetplayerid,
	 buffer[128],
	 reason[85];

	sscanf(params, "s[24]s[85]", szPlayerName, reason);
	if(strlen(reason)>80)
		return Msg(playerid,COLOR_ERROR,"Powod kicka jest zbyt dlugi.");
	targetplayerid = FindPlayer(szPlayerName);
	
	if(targetplayerid == ERROR_NOT_FOUND)
	{
		format(buffer, sizeof buffer, TXT(playerid, 27), szPlayerName);
		return Msg(playerid, COLOR_ERROR, buffer);
	}
	if(targetplayerid == ERROR_FOUND_MULTIPLE)
	{
		format(buffer, sizeof buffer, TXT(playerid, 28), szPlayerName);
		return Msg(playerid, COLOR_ERROR, buffer);
	}

	if (!IsAdmin(playerid, LEVEL_ADMIN1) && !(gmTemp[pAdminCount]>0 || (!pData[targetplayerid][loggedIn])))
		return Msg(playerid,COLOR_ERROR,"Mozesz korzystac z {b}/kick{/b} tylko gdy nie ma adminow, lub gracz nie jest zarejestrowany.");
	if (pData[targetplayerid][adminLevel]>pData[playerid][adminLevel] && pData[targetplayerid][adminLevel]!=LEVEL_ADMINHIDDEN)
		return Msg(playerid,COLOR_ERROR,"Nie mozesz zrobic tego wobec osoby wyzszej ranga!");

	if(!strlen(reason)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 149)); // U¿ywaj: /kick <gracz> <powód>
	
	pTemp[playerid][tmpTargetPlayerID] = targetplayerid;
	copy(reason, pTemp[playerid][tmpReason]);
	
	format(buffer, sizeof buffer, TXT(playerid, 150), GetPlayerProperName(targetplayerid), reason);
	ShowPlayerDialog(playerid, DIALOG_KICK_CONFIRM, DIALOG_STYLE_MSGBOX, TXT(playerid, 151), buffer, TXT(playerid, 62), TXT(playerid, 63));
	
	return 1;
}

CMD:b(playerid, params[])
{
	if(!IsAdmin(playerid, LEVEL_ADMIN1)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/b <id/nick> [powod]{/b}");
	new
	 tplayer[24], tplayer_escaped[32],
	 uid,
	 buf[512], reason[86], bool:accountBan, ipaddr[16];
	sscanf(params,"s[24]S(brak powodu)[85]", tplayer, buf);
	if (strlen(buf)>80)
		return Msg(playerid,COLOR_ERROR,"Powod bana jest zbyt dlugi");
	mysql_real_escape_string(buf,reason);

	if (!FindUIDByNameOrID(tplayer,uid,playerid)) return 1;

	if (uid!=playerid && pData[uid][loggedIn] && pData[uid][adminLevel]>pData[playerid][adminLevel] && pData[uid][adminLevel]<LEVEL_ADMINHIDDEN  && pData[playerid][adminLevel]<LEVEL_ADMINHIDDEN) {
		Msg(playerid, COLOR_ERROR, "Nie mozesz zbanowac admina o wyzszym poziomie od swojego!");
		// banujemy sie automatycznie
		format(buf, sizeof buf,"%d do wyjasnienia w gronie administracji", playerid);
		cmd_b(playerid, buf);

		format(buf,sizeof buf,"Admin {b}%s(%d){/b}, poziom: {b}%d{/b}, probowal zbanowac admina {b}%s(%d){/b}, poziom: {b}%d{/b}!",
			GetPlayerNick(playerid), playerid, pData[playerid][adminLevel], GetPlayerNick(uid), uid, pData[uid][adminLevel]);
		MSGToAdmins(COLOR_ADMIN, buf, true, LEVEL_ADMIN2);
		return 1;
	}

	if (pTemp[playerid][bannedPlayersCnt]++>(pData[playerid][adminLevel]==LEVEL_ADMIN1?30:75)  && playerid!=uid) {
		format(buf, sizeof buf,"%d do wyjasnienia w gronie administracji", playerid);
		cmd_b(playerid, buf);
		
		format(buf,sizeof buf,"Admin {b}%s(%d){/b}, zbanowal ponad zbyt duzo graczy w jednej sesji i zostal automatycznie zablokowany!",
			GetPlayerNick(playerid), playerid);
		MSGToAdmins(COLOR_ADMIN, buf, true, LEVEL_ADMIN2);
		return 1;
	}
	
	if (pData[uid][loggedIn] && pData[uid][accountID]>0) {
		format(buf, sizeof buf, "INSERT INTO fs_bans SET player_banned=%d,player_given=%d,date_end=NOW()+INTERVAL 4 DAY,reason='%s'",
				pData[uid][accountID], pData[playerid][accountID], reason);
		mysql_query(buf);

		format(buf, sizeof buf, "UPDATE %s SET ban_count = ban_count + 1 WHERE id = %i", gmData[DB_players], pData[uid][accountID]);
		mysql_query(buf);
		accountBan=true;
	}

	mysql_real_escape_string(GetPlayerNick(uid),tplayer_escaped);
	
	// jesli ma konto, dajemy bana na 30 minut - zeby nie zmienil adresu i nie wlazl pod innym nickiem
	// jesli nie ma konto, ban na 7 dni
	GetPlayerIp(uid, ipaddr, 16);
	// todo poszukiwania innego gracza online z tym adresem

	if (accountBan)
		format(buf, sizeof buf, "INSERT INTO fs_ipbans SET ip='%s',player_given=%d,date_end=NOW()+INTERVAL 30 MINUTE,reason='%s',target_accountid=%d,target_nick='%s'",
					ipaddr, pData[playerid][accountID], reason, pData[uid][accountID],tplayer_escaped);
	else
		format(buf, sizeof buf, "INSERT INTO fs_ipbans SET ip='%s',player_given=%d,date_end=NOW()+INTERVAL 7 DAY,reason='%s',target_nick='%s'",
					ipaddr, pData[playerid][accountID], reason, tplayer_escaped);

	mysql_query(buf);
	

	if (pData[playerid][adminLevel]<LEVEL_ADMINHIDDEN)
		format(buf, sizeof buf, "*** %s(%d) zostal%s zbanowan%s przez admin%s %s. Powod: %s",
			GetPlayerNick(uid), uid, SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : (""), SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : ("y"), SkinKobiecy(GetPlayerSkin(playerid)) ? ("ke") : ("a"),
			GetPlayerNick(playerid), reason);
	else if (pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN)
		format(buf, sizeof buf, "*** %s(%d) zostal%s zbanowan%s. Powod: %s",
			GetPlayerNick(uid), uid, SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : (""), SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : ("y"), reason);

	SendClientMessageToAll(0xFF0000ff, buf);

	if (Audio_IsClientConnected(uid))
		Audio_Play(uid,AUDIOID_BAN, false, false, true);

	KickPlayer(uid);
	return 1;
}


CMD:ban(playerid, params[])
{
	if(!IsAdmin(playerid, LEVEL_ADMIN2)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 26)); // U¿ywaj: /ban <gracz/nazwa konta> <czas> <m/h/d/t/s/*> <powód>
	
	new
	 szPlayerName[24],
	 targetplayerid,
	 banTime,
	 period,
	 buffer[256],
	 reason[128],
	 bool:accountBan = false;

	sscanf(params, "s[24]ics[128]", szPlayerName, banTime, period, reason);
	targetplayerid = FindPlayer(szPlayerName);
	
	if(targetplayerid == ERROR_NOT_FOUND)
	{
		if(AccountExists(szPlayerName))
		{
			accountBan = true;
		}
		else
		{
			format(buffer, sizeof buffer, TXT(playerid, 180), szPlayerName);
			return Msg(playerid, COLOR_ERROR, buffer);
		}
	}
	if(targetplayerid == ERROR_FOUND_MULTIPLE)
	{
		format(buffer, sizeof buffer, TXT(playerid, 28), szPlayerName);
		return Msg(playerid, COLOR_ERROR, buffer);
	}
	
	if(!accountBan && !PlayerAccountExists(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 25)); 
	
	if(banTime < 1 || banTime > 99999999) return Msg(playerid, COLOR_ERROR, TXT(playerid, 31));

	if(period != 'm' && period != 'h' && period != 'd' && period != 't' && period != 's')
	{
		Msg(playerid, COLOR_ERROR, TXT(playerid, 29)); // Nieprawid³owy ostatni parametr komendy.
		return Msg(playerid, COLOR_ERROR, "M - minuty, H - godziny, D - dni, T - tygodnie, S - miesiace");
	}

	if(!strlen(reason)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 26)); // U¿ywaj: /ban <gracz> <czas> <m/g/d/t/s> <powód>
	
	if(!accountBan)
	{
		pTemp[playerid][tmpTargetPlayerID] = targetplayerid;
	}
	else
	{
		pTemp[playerid][tmpTargetPlayerID] = -1;
		copy(szPlayerName, pTemp[playerid][tmpBanAccountName]);
	}
	pTemp[playerid][tmpBanTime] = 			banTime;
	pTemp[playerid][tmpPeriod] = 			period;
	copy(reason, pTemp[playerid][tmpReason]);
	
	format(buffer, sizeof buffer, TXT(playerid, 61), (accountBan) ? szPlayerName : GetPlayerProperName(targetplayerid), banTime, GetPeriodName(playerid, period, banTime), reason);
	ShowPlayerDialog(playerid, DIALOG_BAN_CONFIRM, DIALOG_STYLE_MSGBOX, TXT(playerid, 60), buffer, TXT(playerid, 62), TXT(playerid, 63));
	
	return 1;
}

CMD:unban(playerid, params[])
{
	if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 181)); // U¿ywaj: /unban <nazwa konta>

	new
	 buffer[128];

	if(strlen(params) < 3 || strlen(params) > 24 || !IsNickCorrect(params))
	{
		return Msg(playerid, COLOR_ERROR, TXT(playerid, 128)); // Podana nazwa konta jest niepoprawna.
	}

	if(!AccountExists(params))
	{
		format(buffer, sizeof buffer, TXT(playerid, 129), params);
		return Msg(playerid, COLOR_ERROR, buffer); // Nie znaleziono konta gracza o nazwie "xxx".
	}
	
	copy(params, pTemp[playerid][tmpBanAccountName]);

	format(buffer, sizeof buffer, TXT(playerid, 182), params);
	ShowPlayerDialog(playerid, DIALOG_UNBAN_CONFIRM, DIALOG_STYLE_MSGBOX, TXT(playerid, 183), buffer, TXT(playerid, 62), TXT(playerid, 63));

	return 1;
}

CMD:banip(playerid, params[])
{
	if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
	if(isnull(params))
	{
		Msg(playerid, COLOR_ERROR, TXT(playerid, 185), false); // U¿ywaj: /banip <gracz/adres IP> <czas> <m/g/d/t/s/*> <powód>
		return Msg(playerid, COLOR_ERROR, TXT(playerid, 186)); // PodpowiedŸ: w pierwszym parametrze komendy wprowadŸ adres IP (zakres lub pe³ny) albo nick/ID gracza bêd¹cego online.
	}
	
	new
	 szIP[24],
	 targetplayerid,
	 banTime,
	 period,
	 buffer[256],
	 reason[128];

	sscanf(params, "s[24]ics[128]", szIP, banTime, period, reason);
	
	if(!IsCorrectIP(szIP))
	{
		targetplayerid = FindPlayer(szIP);
		
		if(targetplayerid == ERROR_NOT_FOUND)
		{
			format(buffer, sizeof buffer, TXT(playerid, 190), szIP);
			Msg(playerid, COLOR_ERROR, buffer, false);
			return Msg(playerid, COLOR_ERROR, TXT(playerid, 186)); // PodpowiedŸ: w pierwszym parametrze komendy wprowadŸ adres IP (zakres lub pe³ny) albo nick/ID gracza bêd¹cego online.
		}
		if(targetplayerid == ERROR_FOUND_MULTIPLE)
		{
			format(buffer, sizeof buffer, TXT(playerid, 28), szIP);
			return Msg(playerid, COLOR_ERROR, buffer);
		}
		
		GetPlayerIp(targetplayerid, szIP, sizeof szIP);
	}
	
	if(banTime < 1 || banTime > 99999999) return Msg(playerid, COLOR_ERROR, TXT(playerid, 31));

	if(period != 'm' && period != 'g' && period != 'd' && period != 't' && period != 's')
	{
		Msg(playerid, COLOR_ERROR, TXT(playerid, 29)); // Nieprawid³owy ostatni parametr komendy.
		return Msg(playerid, COLOR_ERROR, TXT(playerid, 30)); // PodpowiedŸ: m - minuty, g - godziny, d - dni, t - tygodnie, s - miesi¹ce.
	}

	if(!strlen(reason)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 185)); // U¿ywaj: /banip <gracz/adres IP> <czas> <m/g/d/t/s/*> <powód>
	
	copy(szIP, pTemp[playerid][tmpBanIP]);
	pTemp[playerid][tmpBanTime] = 	banTime;
	pTemp[playerid][tmpPeriod] = 	period;
	copy(reason, pTemp[playerid][tmpReason]);
	
	format(buffer, sizeof buffer, TXT(playerid, 187), szIP, banTime, GetPeriodName(playerid, period, banTime), reason);
	ShowPlayerDialog(playerid, DIALOG_BANIP_CONFIRM, DIALOG_STYLE_MSGBOX, TXT(playerid, 188), buffer, TXT(playerid, 62), TXT(playerid, 63));
	
	return 1;
}

CMD:unbanip(playerid, params[])
{
	if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 191)); // U¿ywaj: /unbanip <adres IP>

	new
	 buffer[128];

	if(!IsCorrectIP(params))
	{
		return Msg(playerid, COLOR_ERROR, TXT(playerid, 192)); // Podany adres IP jest nieprawid³owy.
	}
	
	copy(params, pTemp[playerid][tmpBanIP]);

	format(buffer, sizeof buffer, TXT(playerid, 194), params);
	ShowPlayerDialog(playerid, DIALOG_UNBANIP_CONFIRM, DIALOG_STYLE_MSGBOX, TXT(playerid, 193), buffer, TXT(playerid, 62), TXT(playerid, 63));

	return 1;
}

CMD:spec(playerid, params[])
{
	if(!IsAdmin(playerid)) return 0;
//	MUST_BE_SPAWNED(playerid);
	if(!SpecAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 411)); // Nie mo¿esz w tym momencie obserwowaæ graczy.
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 106)); // U¿ywaj: /spec <gracz>
	
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

	if(playerid == targetplayerid) return Msg(playerid, COLOR_ERROR, TXT(playerid, 64)); // Nie mo¿esz obserwowaæ siebie.
	if(!IsPlayerSpawned(targetplayerid) && !IsPlayerNPC(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 379));
	
	StartSpectating(playerid, targetplayerid);
	
	format(buffer, sizeof buffer, TXT(playerid, 107), GetPlayerNick(targetplayerid), targetplayerid ); // Obserwujesz gracza "%s" (%i).
	Msg(playerid, COLOR_INFO, buffer);
	
	return 1;
}

CMD:specoff(playerid)
{
	if(!IsAdmin(playerid, LEVEL_ADMIN1)) return 0;
	if(pData[playerid][spectating] == INVALID_PLAYER_ID) return Msg(playerid, COLOR_ERROR, TXT(playerid, 65));

	StopSpectating(playerid);
	
	return 1;
}
CMD:ann(playerid, params[])
{
	if(!IsAdmin(playerid)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 177)); // U¿ywaj: /ann <tekst>
	if(!CheckTildes(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 205)); // Iloœæ tyld "~" musi byæ parzysta.
	
	new	buffer[512];
	format(buffer, sizeof buffer, "%s: %s", GetPlayerNick(playerid), params);
	OutputLog(LOG_ANN, buffer);
	ShowAnnouncement(params);
	if (pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN) return 1;
	if (strlen(params)>64) params[64]=0;	
	format(buffer,sizeof buffer,"Ann: {b}%s%s{/b}, autor: {b}%s{/b}", params, strlen(params)==64?("..."):(""), GetPlayerNick(playerid));
	MSGToAdmins(COLOR_INFO, buffer, false, LEVEL_GM);
	return 1;
}

CMD:ann2(playerid, params[])
{
	if(!IsGM(playerid)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, "/ann2 {b}<tekst>{/b}"); // U¿ywaj: /ann <tekst>
	if(!CheckTildes(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 205)); // Iloœæ tyld "~" musi byæ parzysta.
		
	new buffer[255];
	new tdtext[1023];
	format(buffer, sizeof buffer, "%s: %s", GetPlayerNick(playerid), params);
	OutputLog(LOG_ANN, buffer);
	if (pData[playerid][adminLevel]<LEVEL_ADMINHIDDEN)
		format(tdtext,sizeof tdtext, "%s ~r~--_~h~%s", params, GetPlayerProperName(playerid));
	else
		format(tdtext,sizeof tdtext, "%s", params);
	ShowAnnouncement2(tdtext);
	return 1;
}

CMD:r(playerid,params[]){
	if(!IsAdmin(playerid)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, "/r {b}<tekst>{/b}");
	new offset=0;
	while (offset<strlen(params)) {
		new line[90];
		strmid(line, params, offset, offset+70);
		format(line, sizeof line, "%s{ff1010}%s", ((offset>0) ? ("   ") : ">> "),line);
		SendClientMessageToAll(0xffffffff, line);
		offset+=70;
	}
	return 1;
}

CMD:ann3(playerid, params[])
{
	if(!IsAdmin(playerid)) return 0;
	new txt[80];
	if (sscanf(params,"s[80]",txt))
		return Msg(playerid, COLOR_ERROR, "/ann3 {b}<tekst>{/b}, maksymalnie 80 znakow"); // U¿ywaj: /ann <tekst>
	if(!CheckTildes(txt)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 205)); // Iloœæ tyld "~" musi byæ parzysta.
		
	new	buffer[140];
	format(buffer, sizeof buffer, "%s: %s", GetPlayerNick(playerid), txt);
	OutputLog(LOG_ANN, buffer);
	format(buffer,sizeof buffer,"~n~%s",txt);
	GameTextForAll(buffer,6000,5);

	if (pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN) return 1;
	if (strlen(txt)>64) txt[64]=0;	
	format(buffer,sizeof buffer,"Ann: {b}%s%s{/b}, autor: {b}%s{/b}", txt, strlen(txt)==64?("..."):(""), GetPlayerNick(playerid));
	MSGToAdmins(COLOR_INFO, buffer, false, LEVEL_GM);


	return 1;
}

CMD:makedrunk(playerid,params[]){
	if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	new userid;
	if (sscanf(params,"d",userid))
		return Msg(playerid,COLOR_ERROR,"/makedrunk {b}<id>{/b}");
	if (!IsPlayerConnected(userid))
		return Msg(playerid,COLOR_ERROR,"Gracz nie jest aktywny");
	SetPlayerDrunkLevel(userid,10000);
	return Msg(playerid,COLOR_ERROR,"Uzyj {b}/makesober <id>{/b} aby cofnac efekt u gracza");
}

CMD:makesober(playerid,params[]){
	if(!IsAdmin(playerid)) return 0;
	new userid;
	if (sscanf(params,"d",userid))
		return Msg(playerid,COLOR_ERROR,"/makesober {b}<id>{/b}");
	if (!IsPlayerConnected(userid))
		return Msg(playerid,COLOR_ERROR,"Gracz nie jest aktywny");
	SetPlayerDrunkLevel(userid,0);
	return Msg(playerid,COLOR_ERROR,"Gracz 'otrzezwiony'");
}



CMD:jail(playerid, params[])
{
	if(!IsAdmin(playerid, LEVEL_GM)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 211)); // U¿ywaj: /jail <gracz> <czas w minutach> <powód>
	
	new
	 szPlayerName[24],
	 jTime,
	 targetplayerid,
	 szReason[128],
	 buffer[128];
	
	sscanf(params, "s[24]is[128]", szPlayerName, jTime, szReason);
	
	targetplayerid = FindPlayer(szPlayerName);
	
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
	
	if(!IsPlayerSpawned(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 379));
	if(pData[targetplayerid][jail] >= 0) return Msg(playerid, COLOR_ERROR, TXT(playerid, 221)); // Ten gracz jest aktualnie w wiêzieniu.
	
	if(jTime<1) return Msg(playerid,COLOR_ERROR,"Minimalnie 1 minuta");
	switch(pData[playerid][adminLevel]){
		case LEVEL_GM:
			if (jTime>2 && gmTemp[pAdminCount]>0)
				return Msg(playerid,COLOR_ERROR,"Mozesz wsadzic na maksymalnie 2 minuty.");
			else if (jTime>5)
				return Msg(playerid,COLOR_ERROR,"Mozesz wsadzic na maksymalnie 5 minut.");
		case LEVEL_ADMIN3,LEVEL_ADMINHIDDEN: {
			}
		default:	// admin2 i admin1
			if (jTime>5)
				return Msg(playerid,COLOR_ERROR,"Mozesz wsadzic na maksymalnie 5 minut.");
					
	}
	if(jTime < 1 || jTime > 20) return Msg(playerid, COLOR_ERROR, TXT(playerid, 212)); // Wartoœæ czasu musi siê mieœciæ w przedziale 1 - 20.
	
	if(!strlen(szReason)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 211)); // U¿ywaj: /jail <gracz> <czas w minutach> <powód>
	
	JailPlayer(targetplayerid, jTime);
	
	format(buffer, sizeof buffer, TXT(playerid, 213), GetPlayerNick(targetplayerid), jTime, GetPeriodName(playerid, 'm', jTime), szReason);
	foreach(i)
		Msg(i, COLOR_INFO, buffer); // Gracz "xxx" zosta³ zamkniêty w wiêzeniu na xxx minut z powodu: xxx.
	
	format(buffer, sizeof buffer, TXT(playerid, 214), jTime, GetPeriodName(playerid, 'm', jTime), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
	Msg(targetplayerid, COLOR_INFO2, buffer); // Zosta³eœ zamkniêty w wiêzieniu na xxx minut przez admina "xxx".
	format(buffer, sizeof buffer, TXT(playerid, 50), szReason);
	Msg(targetplayerid, COLOR_INFO2, buffer); // Powód: xxx
	
	return 1;
}

CMD:unwiezall(playerid){
	new buffer[128], cnt;
	if(!IsAdmin(playerid, LEVEL_ADMIN2)) return 0;
	foreach(i)
		if (pData[i][jail]>=0) {
				format(buffer,sizeof buffer,"%d", i);
				cmd_unwiez(playerid,buffer);
				cnt++;
		}

	format(buffer,sizeof buffer,"Wypuszczono z wiezienia %d %s", cnt, dli(cnt,"gracza","graczy","graczy"));
	Msg(playerid,COLOR_INFO,buffer);
	return 1;

}

CMD:unwiez(playerid, params[])
{
	
	new
	 targetplayerid,
	 buffer[128];

	if(!IsAdmin(playerid, LEVEL_GM)) return 0;
	if(isnull(params)) {
		new sec=GetTickCount()/1000;
		new remainTime;
		foreach(i)
			if (pData[i][jail]>=0) {
				remainTime = ((pData[i][jail] - sec)/60) + 1;
				format(buffer, sizeof buffer, "%3d {b}%s{/b} - %02dm", i, GetPlayerNick(i), remainTime);
				Msg(playerid,COLOR_INFO, buffer, false);
			}
		Msg(playerid,COLOR_ERROR,"/unwiez <id/nick>");
		return 1;
	}
	
	targetplayerid = FindPlayer(params);
	
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
	
	if(pData[targetplayerid][jail] == -1) return Msg(playerid, COLOR_ERROR, TXT(playerid, 218)); // Podany gracz nie jest w wiêzieniu.
	
	UnjailPlayer(targetplayerid);
	
	format(buffer, sizeof buffer, TXT(playerid, 216), GetPlayerNick(targetplayerid));
	foreach(i)
		if (pData[i][pAttraction]==A_NONE && i!=targetplayerid)
			Msg(i, COLOR_INFO, buffer); // Gracz "xxx" zosta³ usuniêty z wiêzienia.

	
	format(buffer, sizeof buffer, TXT(playerid, 217), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
	Msg(targetplayerid, COLOR_INFO2, buffer); // Zosta³eœ usuniêty z wiêzienia przez admina "xxx".
	
	return 1;
}
CMD:mute(playerid, params[])
{
	if(!IsAdmin(playerid, LEVEL_GM)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 243)); // U¿ywaj: /mute <gracz> <czas w minutach> <powód>
	
	new
	 szPlayerName[24],
	 mTime,
	 targetplayerid,
	 szReason[128],
	 buffer[128];
	
	sscanf(params, "s[24]is[128]", szPlayerName, mTime, szReason);
	
	targetplayerid = FindPlayer(szPlayerName);
	
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
	
	if(pData[targetplayerid][mute] - (GetTickCount() / 1000) > 0 && pData[targetplayerid][mute] != 0) return Msg(playerid, COLOR_ERROR, TXT(playerid, 244)); // Ten gracz jest ju¿ wyciszony.


	switch(pData[playerid][adminLevel]){
		case LEVEL_GM:
			if (mTime>2 && gmTemp[pAdminCount]>0)
				return Msg(playerid,COLOR_ERROR,"Mozesz uciszyc na maksymalnie 2 minuty.");
			else if (mTime>5)
				return Msg(playerid,COLOR_ERROR,"Mozesz uciszyc na maksymalnie 5 minut.");
		case LEVEL_ADMIN3,LEVEL_ADMINHIDDEN: {
			}
		default:	// admin2 i admin1
			if (mTime>5)
				return Msg(playerid,COLOR_ERROR,"Mozesz uciszyc na maksymalnie 5 minut.");
					
	}
	if(mTime < 1 || mTime > 20) return Msg(playerid, COLOR_ERROR, TXT(playerid, 212)); // Wartoœæ czasu musi siê mieœciæ w przedziale 1 - 20.

	
	if(!strlen(szReason)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 243)); // U¿ywaj: /mute <gracz> <czas w minutach> <powód>
	
	pData[targetplayerid][mute] = (GetTickCount() / 1000) + (mTime * 60);
	
	format(buffer, sizeof buffer, TXT(playerid, 245), GetPlayerNick(targetplayerid), mTime, GetPeriodName(playerid, 'm', mTime), szReason);
	foreach(i)
		Msg(i, COLOR_INFO, buffer); // Gracz "xxx" zosta³ wyciszony na xxx minut z powodu: xxx.
	
	format(buffer, sizeof buffer, TXT(playerid, 246), mTime, GetPeriodName(playerid, 'm', mTime), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
	Msg(targetplayerid, COLOR_INFO2, buffer); // Zosta³eœ wyciszony na xxx minut przez admina "xxx".
	format(buffer, sizeof buffer, TXT(playerid, 50), szReason);
	Msg(targetplayerid, COLOR_INFO2, buffer); // Powód: xxx

	return 1;
}

CMD:unmute(playerid, params[])
{
	if(!IsAdmin(playerid, LEVEL_GM)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 247)); // U¿ywaj: /unmute <gracz>
	
	new
	 targetplayerid,
	 buffer[128];
	
	targetplayerid = FindPlayer(params);
	
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

	if(pData[targetplayerid][mute] - (GetTickCount() / 1000) <= 0) return Msg(playerid, COLOR_ERROR, TXT(playerid, 248)); // Podany gracz nie jest wyciszony.
	
	pData[targetplayerid][mute] = 0;
	
	format(buffer, sizeof buffer, TXT(playerid, 249), GetPlayerNick(targetplayerid));
	Msg(playerid, COLOR_INFO, buffer); // Wyciszenie gracza "xxx" zosta³o wy³¹czone.
	
	format(buffer, sizeof buffer, TXT(playerid, 250), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
	Msg(targetplayerid, COLOR_INFO2, buffer); // Twoje wyciszenie zosta³o wy³¹czone przez admina "xxx".
	
	return 1;
}


CMD:cenz(playerid, params[])
{
	if(!IsAdmin(playerid, LEVEL_ADMIN2)) return 0;
	new buffer[128];
	if(isnull(params)) {
		Msg(playerid,COLOR_INFO,"Gracze objeci cenzura:",false);
		new cnt;
		foreach(i)
			if (pTemp[i][cenzura]) {
				cnt++;
				format(buffer,sizeof buffer, "%s%s (%d)   ", buffer, GetPlayerNick(i), i);
				if (strlen(buffer)>=94) {
					Msg(playerid,COLOR_INFO2, buffer,false);
					buffer[0]=0;
				}
			}		
		if (buffer[0]!=0)
			Msg(playerid,COLOR_INFO2, buffer,false);					

		format(buffer, sizeof buffer, "Lacznie: {b}%d %s{/b}", cnt, dli(cnt,"osoba","osoby","osob"));
		Msg(playerid,COLOR_INFO, buffer, false);
		return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/cenz [id/nick]{/b}. Ponowne uzycie komendy wylaczy cenzure.",false);
	}

	new
	 targetplayerid;
	
	targetplayerid = FindPlayer(params);
	
	if(targetplayerid == ERROR_NOT_FOUND)
	{
		format(buffer, sizeof buffer, TXT(playerid, 27), params);
		return Msg(playerid, COLOR_ERROR, buffer);
	}
	else if(targetplayerid == ERROR_FOUND_MULTIPLE)
	{
		format(buffer, sizeof buffer, TXT(playerid, 28), params);
		return Msg(playerid, COLOR_ERROR, buffer);
	} else if (targetplayerid==playerid)
		return Msg(playerid, COLOR_ERROR, "Nie mozesz ocenzurowac siebie.");

	if(pData[targetplayerid][adminLevel]>0 && pData[targetplayerid][adminLevel]!=LEVEL_ADMINHIDDEN && pData[targetplayerid][adminLevel]>pData[playerid][adminLevel]) {
		format(buffer,sizeof buffer,"Admin %s (%d) probowal Cie o(d)cenzurowac, ale niestety jest nizszego poziomu niz Ty.");
		Msg(targetplayerid,COLOR_ADMIN, buffer);
		return Msg(playerid, COLOR_ERROR, "Nie mozesz o(d)cenzurowac admina wyzszego poziomu!");
	}
	if(pTemp[targetplayerid][cenzura]) {
		new bool:kobieta=Kobieta(playerid),
			bool:kobietat=Kobieta(targetplayerid);
		pTemp[targetplayerid][cenzura]=false;
		format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} odcenzurowal%s Cie - mozesz juz uzywac komend.", 
			kobieta?("ka"):(""), pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN?(""):GetPlayerProperName(playerid), kobieta?("a"):(""));
		Msg(targetplayerid, COLOR_ADMIN, buffer, false);
		PlaySound(targetplayerid, 1137);
		GameTextForPlayer(targetplayerid, kobietat?("~r~zostalas odcenzurowana"):("~r~zostales odcenzurowany!"), 4000, 1);
		format(buffer,sizeof buffer,"{b}%s (%d){/b} zostal%s odcenzurowan%s przez {b}%s{/b}.", GetPlayerProperName(targetplayerid), targetplayerid, kobietat?("a"):(""),
			kobietat?("a"):("y"), GetPlayerProperName(playerid));
		foreach(i)
			if (i!=targetplayerid)
				Msg(i,COLOR_ADMIN, buffer, false);
	} else {
		new bool:kobieta=Kobieta(playerid),
			bool:kobietat=Kobieta(targetplayerid);

		if (pData[targetplayerid][adminLevel]!=LEVEL_ADMINHIDDEN)
			pTemp[targetplayerid][cenzura]=true;

		format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} nalozyl%s na Ciebie cenzure - nie mozesz uzywac komend.", 
			kobieta?("ka"):(""), pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN?(""):GetPlayerProperName(playerid), kobieta?("a"):(""));
		Msg(targetplayerid, COLOR_ADMIN, buffer, false);

		PlaySound(targetplayerid, 1137);
		GameTextForPlayer(targetplayerid, kobietat?("~r~zostalas ocenzurowana"):("~r~zostales ocenzurowany!"), 4000, 1);
		format(buffer,sizeof buffer,"{b}%s (%d){/b} zostal%s ocenzurowan%s przez {b}%s{/b}.", GetPlayerProperName(targetplayerid), targetplayerid, kobietat?("a"):(""),
			kobietat?("a"):("y"), GetPlayerProperName(playerid));
		foreach(i)
			if (i!=targetplayerid)
				Msg(i,COLOR_ADMIN, buffer, false);

		if (pData[targetplayerid][adminLevel]==LEVEL_ADMINHIDDEN)
			Msg(targetplayerid, COLOR_ADMIN, "Z racji Twojego poziomu admina, cenzura nie zostala faktycznie wlaczona, ale nikt o tym nie wie.");

	}

	return 1;
}


CMD:prot(playerid, params[])
{
	if(!IsAdmin(playerid, LEVEL_GM)) return 0;
	new buffer[128];
	if(isnull(params)) {
		Msg(playerid,COLOR_INFO,"Gracze z protem:",false);
		new cnt;
		foreach(i)
			if (pTemp[i][protkill]) {
				cnt++;
				format(buffer,sizeof buffer, "%s%s (%d)   ", buffer, GetPlayerNick(i), i);
				if (strlen(buffer)>=94) {
					Msg(playerid,COLOR_INFO2, buffer,false);
					buffer[0]=0;
				}
			}		
		if (buffer[0]!=0)
			Msg(playerid,COLOR_INFO2, buffer,false);					

		format(buffer, sizeof buffer, "Lacznie: {b}%d %s{/b}", cnt, dli(cnt,"osoba","osoby","osob"));
		Msg(playerid,COLOR_INFO, buffer, false);
		return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/prot [id/nick]{/b}. Ponowne uzycie komendy wylaczy prota.",false);
	}

	new
	 targetplayerid;
	
	targetplayerid = FindPlayer(params);
	
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
	if(pTemp[targetplayerid][protkill]) {
		new bool:kobieta=Kobieta(playerid),
			bool:kobietat=Kobieta(targetplayerid);
		pTemp[targetplayerid][protkill]=false;
		format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} odebral%s Ci immunitet.", 
			kobieta?("ka"):(""), pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN?(""):GetPlayerProperName(playerid), kobieta?("a"):(""));
		Msg(targetplayerid, COLOR_ADMIN, buffer, false);
		PlaySound(targetplayerid, 1137);
		GameTextForPlayer(targetplayerid, "~g~immunitet odebrany", 4000, 1);
		format(buffer,sizeof buffer,"{b}%s (%d){/b} zostal%s pozbawion%s immunitetu przez {b}%s{/b}.", GetPlayerProperName(targetplayerid), targetplayerid, kobietat?("a"):(""),
			kobietat?("a"):("y"), GetPlayerProperName(playerid));
		foreach(i)
			if (i!=targetplayerid)
				Msg(i,COLOR_ADMIN, buffer, false);
	} else {
		new bool:kobieta=Kobieta(playerid),
			bool:kobietat=Kobieta(targetplayerid);

		pTemp[targetplayerid][protkill]=true;

		format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} dal%s Ci immunitet.", 
			kobieta?("ka"):(""), pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN?(""):GetPlayerProperName(playerid), kobieta?("a"):(""));

		Msg(targetplayerid, COLOR_ADMIN, buffer, false);
		PlaySound(targetplayerid, 1137);
		GameTextForPlayer(targetplayerid, "~g~otrzymano immunitet", 4000, 1);
		format(buffer,sizeof buffer,"{b}%s (%d){/b} otrzymal%s immunitet od {b}%s{/b}.", GetPlayerProperName(targetplayerid), targetplayerid, kobietat?("a"):(""),
			GetPlayerProperName(playerid));
		foreach(i)
			if (i!=targetplayerid)
				Msg(i,COLOR_ADMIN, buffer, false);
	}

	return 1;
}

CMD:protping(playerid, params[])
{
	if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
	new buffer[128];
	if(isnull(params)) {
		Msg(playerid,COLOR_INFO,"Gracze z protem na pinga:",false);
		new cnt;
		foreach(i)
			if (pTemp[i][protping]) {
				cnt++;
				format(buffer,sizeof buffer, "%s%s (%d)   ", buffer, GetPlayerNick(i), i);
				if (strlen(buffer)>=94) {
					Msg(playerid,COLOR_INFO2, buffer,false);
					buffer[0]=0;
				}
			}		
		if (buffer[0]!=0)
			Msg(playerid,COLOR_INFO2, buffer,false);					

		format(buffer, sizeof buffer, "Lacznie: {b}%d %s{/b}", cnt, dli(cnt,"osoba","osoby","osob"));
		Msg(playerid,COLOR_INFO, buffer, false);
		return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/protping [id/nick]{/b}. Ponowne uzycie komendy wylaczy prota.",false);
	}

	new
	 targetplayerid;
	
	targetplayerid = FindPlayer(params);
	
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
	if(pTemp[targetplayerid][protping]) {
		new bool:kobieta=Kobieta(playerid);
//			bool:kobietat=Kobieta(targetplayerid);
		pTemp[targetplayerid][protping]=false;
		format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} odebral%s Ci zezwolenie na wyzszy ping.", 
			kobieta?("ka"):(""), pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN?(""):GetPlayerProperName(playerid), kobieta?("a"):(""));
		Msg(targetplayerid, COLOR_ADMIN, buffer, false);
		PlaySound(targetplayerid, 1137);
//		GameTextForPlayer(targetplayerid, "~g~protping odebrany", 4000, 1);
//		format(buffer,sizeof buffer,"{b}%s (%d){/b} zostal%s pozbawion%s immunitetu przez {b}%s{/b}.", GetPlayerProperName(targetplayerid), targetplayerid, kobietat?("a"):(""),
//			kobietat?("a"):("y"), GetPlayerProperName(playerid));
//		foreach(i)
//			if (i!=targetplayerid)
//				Msg(i,COLOR_ADMIN, buffer, false);
	} else {
		new bool:kobieta=Kobieta(playerid);
//			bool:kobietat=Kobieta(targetplayerid);

		pTemp[targetplayerid][protping]=true;

		format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} dal%s Ci zezwolenie na wyzszy ping.", 
			kobieta?("ka"):(""), pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN?(""):GetPlayerProperName(playerid), kobieta?("a"):(""));

		Msg(targetplayerid, COLOR_ADMIN, buffer, false);
		PlaySound(targetplayerid, 1137);
//		GameTextForPlayer(targetplayerid, "~g~otrzymano immunitet", 4000, 1);
//		format(buffer,sizeof buffer,"{b}%s (%d){/b} otrzymal%s immunitet od {b}%s{/b}.", GetPlayerProperName(targetplayerid), targetplayerid, kobietat?("a"):(""),
//			GetPlayerProperName(playerid));

	}

	return 1;
}


CMD:troll(playerid, params[])
{
	if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
	new buffer[128];
	if(isnull(params)) {
		Msg(playerid,COLOR_INFO,"Trolle na serwerze:",false);
		new cnt;
		foreach(i)
			if (pTemp[i][troll]) {
				cnt++;
				format(buffer,sizeof buffer, "%s%s (%d)   ", buffer, GetPlayerNick(i), i);
				if (strlen(buffer)>=94) {
					Msg(playerid,COLOR_INFO2, buffer,false);
					copy("", buffer);
				}
			}
		if (strlen(buffer)>0)
			Msg(playerid,COLOR_INFO2, buffer,false);					

		format(buffer, sizeof buffer, "Lacznie: {b}%d %s{/b}", cnt, dli(cnt,"troll","trolle","trolli"));
		Msg(playerid,COLOR_INFO, buffer, false);
		return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/troll [id/nick]{/b}. Ponowne uzycie komendy wylaczy ograniczenie.",false);
	}

	new
	 targetplayerid;
	
	targetplayerid = FindPlayer(params);
	
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
	if(pTemp[targetplayerid][troll]) {
//		new bool:kobieta=Kobieta(playerid),
//			bool:kobietat=Kobieta(targetplayerid);
		pTemp[targetplayerid][troll]=false;
		Msg(playerid,COLOR_INFO, "Zdjeles ograniczenie z gracza");
//		format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} odebral%s Ci immunitet.", 
//			kobieta?("ka"):(""), pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN?(""):GetPlayerProperName(playerid), kobieta?("a"):(""));
//		Msg(targetplayerid, COLOR_ADMIN, buffer, false);
//		PlaySound(targetplayerid, 1137);
//		GameTextForPlayer(targetplayerid, "~g~immunitet odebrany", 4000, 1);
//		format(buffer,sizeof buffer,"{b}%s (%d){/b} zostal%s pozbawion%s immunitetu przez {b}%s{/b}.", GetPlayerProperName(targetplayerid), targetplayerid, kobietat?("a"):(""),
//			kobietat?("a"):("y"), GetPlayerProperName(playerid));
//		foreach(i)
//			if (i!=targetplayerid)
//				Msg(i,COLOR_ADMIN, buffer, false);
	} else {
//		new bool:kobieta=Kobieta(playerid),
//			bool:kobietat=Kobieta(targetplayerid);

		pTemp[targetplayerid][troll]=true;

		Msg(playerid, COLOR_INFO,"Nalozyles na gracza ograniczenie");
//		format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} dal%s Ci immunitet.", 
//			kobieta?("ka"):(""), pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN?(""):GetPlayerProperName(playerid), kobieta?("a"):(""));

//		Msg(targetplayerid, COLOR_ADMIN, buffer, false);
//		PlaySound(targetplayerid, 1137);
//		GameTextForPlayer(targetplayerid, "~g~otrzymano immunitet", 4000, 1);
//		format(buffer,sizeof buffer,"{b}%s (%d){/b} otrzymal%s immunitet od {b}%s{/b}.", GetPlayerProperName(targetplayerid), targetplayerid, kobietat?("a"):(""),
//			GetPlayerProperName(playerid));
//		foreach(i)
//			if (i!=targetplayerid)
//				Msg(i,COLOR_ADMIN, buffer, false);
	}

	return 1;
}


CMD:configupdate(playerid)
{
	if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
	
	LoadConfig();
	TextDrawSetString(gTextDraw[TD_WELCOMETEXT], GetServerConfig("welcome_text"));
	
	foreach(i)
	{
		SetPlayerProperColor(i);
	}
	
	Msg(playerid, COLOR_INFO, TXT(playerid, 302));
	
	return 1;
}

CMD:jetpack(playerid,params[]){
	printf(":jetpack");
    if(pData[playerid][adminLevel]<LEVEL_GM) return 0;
	printf("Jetpack ok");

	new tplayer[24],userid;

    if (pData[playerid][adminLevel]==LEVEL_GM)
		userid=playerid;
	else if (!sscanf(params,"s[24]",tplayer))
		FindUIDByNameOrID(tplayer,userid,playerid);
	else
		return Msg(playerid,COLOR_ERROR,"Uzyj: /jetpack [id/nick]");



//	new Float:P[3]; 
//	GetPlayerPos(userid,P[0],P[1],P[2]);
	SetPlayerVelocity(userid,0,0,1);

	SetPlayerSpecialAction(userid, 2);
	if(gmData[artefactOwner] == userid) DropArtefact(userid);
	// todo powiadomienie
	return 1;
}

CMD:resetaudio(playerid,params[]){
    if(!IsAdmin(playerid)) return 0;
	Audio_SetPack("fullserver", true);
	return 1;
}

CMD:kratka(playerid,params[]){
    if(!IsGM(playerid)) return 0;
    new userid;
    if (sscanf(params,"d",userid))
	return Msg(playerid,COLOR_ERROR,"/kratka <id gracza>");
    else if (!IsPlayerConnected(userid)) 
	return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");

    new Float:P[3],int,vw;
    GetPlayerPos(userid,P[0],P[1],P[2]);
    int=GetPlayerInterior(userid);
    vw=GetPlayerVirtualWorld(userid);

    if (IsValidDynamicObject(oKratki[userid]))
	DestroyDynamicObject(oKratki[userid]);
    oKratki[userid] = CreateDynamicObject(18856, P[0], 	P[1], 		P[2]+0.5, 0.0, 0.0, 0.0, vw,int,-1,300);
    SetPlayerPos(userid,P[0],P[1],P[2]);
	Streamer_Update(userid);
    PlayerPlaySound(userid, 1137, 0,0,0);
    return 1;

}
CMD:unkratka(playerid,params[]){
    if(!IsGM(playerid)) return 0;
    new userid;
    if (sscanf(params,"d",userid))
	return Msg(playerid,COLOR_ERROR,"/unkratka <id gracza>");
    if (IsValidDynamicObject(oKratki[userid]))
	    DestroyDynamicObject(oKratki[userid]);
	Streamer_Update(userid);
    return 1;
}

CMD:unkratkaall(playerid){
    if(!IsAdmin(playerid)) return 0;
    for (new i=0; i<GetMaxPlayers();i++)
        if (IsValidDynamicObject(oKratki[i]))
	    DestroyDynamicObject(oKratki[i]);
    return 1;	
}

CMD:a(playerid, params[])
{
	if(!IsAdmin(playerid)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 133)); // U¿ywaj: /a <tekst>
	
	if(FilterText(playerid, params) == 0) return 1;
	if(!IsAdmin(playerid)) CensorText(params);
	if(gmData[chatColors]) FilterTextWithColors(params);
	
	OutputAdminChat(playerid, params);
	
	return 1;
}

CMD:aa(playerid, params[])
{
	if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 134)); // U¿ywaj: /aa <tekst>
	
	if(FilterText(playerid, params) == 0) return 1;
	if(gmData[chatColors]) FilterTextWithColors(params);
	
	OutputAdmin3Chat(playerid, params);
	
	return 1;
}

CMD:tt(playerid, params[])
{
	MUST_BE_SPAWNED(playerid);
	if(!IsGM(playerid)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 354)); // U¿ywaj: /tt <gracz>
	
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
	
	new
	 pVector[e_Vectors],
	 _interior,
	 _vw;
	
	GetPlayerPosition(targetplayerid, pVector[X], pVector[Y], pVector[Z], pVector[A]);
	_interior = GetPlayerInterior(targetplayerid);
	_vw = GetPlayerVirtualWorld(targetplayerid);
	
	Teleport(T_PLAYER, playerid, pVector[X] + 1.0, pVector[Y], pVector[Z], pVector[A], _interior, _vw, (Streamer_CountVisibleItems(targetplayerid, STREAMER_TYPE_OBJECT)>0)?true:false);
	
	format(buffer, sizeof buffer, TXT(playerid, 355), GetPlayerNick(targetplayerid));
	Msg(playerid, COLOR_INFO, buffer);

	if (pData[playerid][adminLevel] != LEVEL_ADMINHIDDEN) {
//		format(buffer, sizeof buffer, TXT(playerid, 356),  GetPlayerNick(playerid));
		format(buffer, sizeof buffer, "%s%s {b}%s{/b} przeteleportowal%s sie do Twojej pozycji", pData[playerid][adminLevel]==LEVEL_GM?("Moderator"):("Administrator"), Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""));
		Msg(targetplayerid, COLOR_INFO2, buffer);
	}
	
	return 1;
}

CMD:th(playerid, params[])
{
	MUST_BE_SPAWNED(playerid);
	if(!IsGM(playerid)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 357)); // U¿ywaj: /th <gracz>
	
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
	if(!TeleportAllowed(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 376));
	
	new
	 pVector[e_Vectors],
	 _interior,
	 _vw;
	
	GetPlayerPosition(playerid, pVector[X], pVector[Y], pVector[Z], pVector[A]);
	_interior = GetPlayerInterior(playerid);
	_vw = GetPlayerVirtualWorld(playerid);
	
	Teleport(T_PLAYER, targetplayerid, pVector[X] + 1.0, pVector[Y], pVector[Z], pVector[A], _interior, _vw, (Streamer_CountVisibleItems(playerid, STREAMER_TYPE_OBJECT)>0)?true:false);
	
	format(buffer, sizeof buffer, TXT(playerid, 358), GetPlayerNick(targetplayerid));
	Msg(playerid, COLOR_INFO, buffer);
	if (pData[playerid][adminLevel] != LEVEL_ADMINHIDDEN) {
		format(buffer, sizeof buffer, TXT(playerid, 359),  GetPlayerNick(playerid));
		Msg(targetplayerid, COLOR_INFO2, buffer);
	}
	
	return 1;
}

CMD:freeze(playerid, params[])
{
	MUST_BE_SPAWNED(playerid);
	if(!IsAdmin(playerid)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 367)); // U¿ywaj: /freeze <gracz>
	
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
	
	if(!IsPlayerSpawned(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 379));
	
	TogglePlayerControllable(targetplayerid, false);
	
	format(buffer, sizeof buffer, TXT(playerid, 368), GetPlayerNick(targetplayerid));
	Msg(playerid, COLOR_INFO, buffer); // Zamrozi³eœ gracza xxx.
	
	if(targetplayerid == playerid) return 1;
	
	format(buffer, sizeof buffer, TXT(playerid, 369), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
	Msg(targetplayerid, COLOR_INFO2, buffer); // Zosta³eœ zamro¿ony przez admina xxx.
	
	return 1;
}

CMD:unfreeze(playerid, params[])
{
	MUST_BE_SPAWNED(playerid);
	if(!IsGM(playerid)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 370)); // U¿ywaj: /unfreeze <gracz>
	
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
	
	if(!IsPlayerSpawned(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 379));
	
	TogglePlayerControllable(targetplayerid, true);
	
	format(buffer, sizeof buffer, TXT(playerid, 371), GetPlayerNick(targetplayerid));
	Msg(playerid, COLOR_INFO, buffer); // Odmrozi³eœ gracza xxx.
	
	if(targetplayerid == playerid) return 1;
	
	format(buffer, sizeof buffer, TXT(playerid, 372), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
	Msg(targetplayerid, COLOR_INFO2, buffer); // Zosta³eœ odmro¿ony przez admina xxx.
	
	return 1;
}

CMD:disarm(playerid, params[])
{
	MUST_BE_SPAWNED(playerid);
	if(!IsAdmin(playerid)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 373)); // U¿ywaj: /disarm <gracz>
	
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
	
	if(!IsPlayerSpawned(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 379));
	
	ResetPlayerWeapons(targetplayerid);
	
	format(buffer, sizeof buffer, TXT(playerid, 374), GetPlayerNick(targetplayerid));
	Msg(playerid, COLOR_INFO, buffer); // Rozbroi³eœ gracza xxx.
	
	if(targetplayerid == playerid) return 1;
	
	format(buffer, sizeof buffer, TXT(playerid, 375), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
	Msg(targetplayerid, COLOR_INFO2, buffer); // Zosta³eœ rozbrojony przez admina xxx.
	
	return 1;
}

CMD:remove(playerid, params[])
{
//	MUST_BE_SPAWNED(playerid);
	if(!IsAdmin(playerid)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 380)); // U¿ywaj: /remove <gracz>
	
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
	
	if(!IsPlayerInAnyVehicle(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 381));
	
	SetVehicleVelocity(GetPlayerVehicleID(targetplayerid), 0.0, 0.0, 0.0);
	RemovePlayerFromVehicle(targetplayerid);
	
	format(buffer, sizeof buffer, TXT(playerid, 382), GetPlayerNick(targetplayerid));
	Msg(playerid, COLOR_INFO, buffer); // Wyrzuci³eœ gracza xxx z pojazdu.
	
	if(targetplayerid == playerid) return 1;
	
	format(buffer, sizeof buffer, TXT(playerid, 383), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
	Msg(targetplayerid, COLOR_INFO2, buffer); // Admin xxx wyrzuci³ Ciê z pojazdu.
	
	return 1;
}

CMD:clearchat(playerid)
{
//	MUST_BE_SPAWNED(playerid);
	if(!IsAdmin(playerid)) return 0;
//	if(IsGM(playerid) && gmTemp[pAdminCount]>0)
//		Msg(playerid,COLOR_ERROR,"Nie masz dostepu do komend admina");

	new
	 buffer[128];
	
	for(new i = 1; i <= 100; i++)
		SendClientMessageToAll(0, " ");
	
	foreach(i)
	{
		format(buffer, sizeof buffer, TXT(i, 384), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
		Msg(i, COLOR_INFO2, buffer); // Czat zosta³ wyczyszczony przez admina xxx.
	}
	
	return 1;
}
CMD:nearvehid(playerid){
	MUST_BE_SPAWNED(playerid);
	if(!IsAdmin(playerid)) return 0;

	new vid=GetNearestVehicleToPlayer(playerid);
	if (vid==INVALID_VEHICLE_ID)
		return Msg(playerid,COLOR_ERROR,"Nie znaleziono zadnego pojazdu");

	new str[127];
	new Float:VP[3];
	GetVehiclePos(vid,VP[0],VP[1],VP[2]);
	format(str,sizeof str, "Najblizszy pojazd: %d, model: %d, polozenie %.1f %.1f %.1f", vid, GetVehicleModel(vid), VP[0],VP[1],VP[2]);
	return Msg(playerid,COLOR_INFO2,str);

}

CMD:startvote(playerid,params[]){
	return cmd_glosowanie(playerid,params);
}

CMD:stopvote(playerid){
	return cmd_glosowanie(playerid,"stop");
}

CMD:glosowanie(playerid,params[]){
	if(!IsGM(playerid)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uzycie: {b}/glosowanie <pytanie/STOP>{/b}"); // U¿ywaj: /ann <tekst>
	if (strcmp(params,"stop",false)==0) {	// przerywamy glosowaine!
		if (gmTemp[voteTimeleft]==0)
			return Msg(playerid,COLOR_ERROR,"Obecnie nie ma zadnego glosowania!");

		gmTemp[voteTimeleft]=0;
		TextDrawHideForAll(gTextDraw[TD_VOTING]);
		foreach (i)
			Msg(i,COLOR_INFO,TXT(playerid,460));
		return 1;
	}
	if(!CheckTildes(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 205)); // Iloœæ tyld "~" musi byæ parzysta.
	if (gmTemp[voteTimeleft]!=0) 
		return Msg(playerid,COLOR_ERROR, "Obecnie trwa juz jakies glosowanie!");

	new str[1024];

	gmTemp[voteCount][0]=0;
	gmTemp[voteCount][1]=0;
	gmTemp[voteAuthor]=playerid;
	gmTemp[voteTimeleft]=15;

	foreach(i)
		pTemp[i][hasVoted]=false;

	copy(params,gmTemp[voteQuestion]);
//	format(str,sizeof str,"%s ~n~ ~n~~y~%2d       ~l~%d   ~w~/TAK   |   /NIE   ~l~%d",gmTemp[voteQuestion],gmTemp[voteTimeleft],gmTemp[voteCount][0],gmTemp[voteCount][1]);

	format(str,sizeof str,"%s %s~n~~y~%2d       ~g~%d   ~w~/TAK   ~r~%d   ~w~/NIE",
					gmTemp[voteQuestion],	(strlen(gmTemp[voteQuestion])<35) ? ("~n~ ") : (" "),
					gmTemp[voteTimeleft],gmTemp[voteCount][0],gmTemp[voteCount][1]);

//	TextDrawBoxColor(gTextDraw[TD_VOTING],-855664316);//0x031f033f);// 1f
	TextDrawSetString(gTextDraw[TD_VOTING],str);
	TextDrawShowForAll(gTextDraw[TD_VOTING]);	// dorobic sprawdzanie

	if (pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN) return 1;
	format(gstr, sizeof gstr, "Autor glosowania: {b}%s{/b}", GetPlayerProperName(playerid));
	MSGToAdmins(COLOR_INFO2, gstr, false, LEVEL_GM);
	return 1;
}

CMD:rrsp(playerid,params[]){
	if(!IsAdmin(playerid)) return 0;
    new userid;
    if (sscanf(params,"d",userid))
		userid=playerid;
	else if (!IsPlayerConnected(userid)) 
		return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
	SpawnPlayer(userid);
	return 1;
}

CMD:nocheats(playerid,params[]){
	if(!IsGM(playerid)) return 0;

	new tplayer[24],userid;

	if (!sscanf(params,"s[24]",tplayer))
		FindUIDByNameOrID(tplayer,userid,playerid);
	else
		return Msg(playerid,COLOR_ERROR,"Uzyj: /nocheats <id/nick>");
	ShowPlayerDialog(userid,DIALOG_PLAYER_INFO,DIALOG_STYLE_MSGBOX,"Ostrzezenie/Warning","Na tym serwerze nie mozna uzywac czitow zadnego rodzaju.\r\nDostosuj sie, albo zostaniesz zbanowany!\r\n\r\n---\r\n\r\nCheats are not allowed on this server\r\nDon't use them or get banned!\r\n\r\n---\r\n\r\nObman zdes' ne dopuskaet sya.\r\nNe obmanut' ili poluchit' ban.","OK","");
	Msg(playerid,COLOR_INFO,"Wyslano informacje do gracza");
	
	return 1;
	
}


CMD:warn(playerid,params[]){

	if(!IsAdmin(playerid)) return 0;

	new tplayer[24],userid,reason[127],bool:kickPlayer=false;

	if (!sscanf(params,"s[24]s[127]",tplayer,reason))
		FindUIDByNameOrID(tplayer,userid,playerid);
	else
		return Msg(playerid,COLOR_ERROR,"Uzyj: /warn <id/nick> <powod>");

	if (GetPlayerCurrentSession(userid)<=5)
		return Msg(playerid, COLOR_ERROR, "Ten gracz dopiero sie polaczyl.");

	if (strlen(reason)>90)
		return Msg(playerid,COLOR_ERROR,"Powod warna jest zbyt dlugi");

	if (pTemp[userid][warningReceived]==0)	// pierwsze ostrzezenie
		pTemp[userid][warningReceived]=GetTickCount();
	else if (GetTickCount()-pTemp[userid][warningReceived]<15000)
		return Msg(playerid,0xff6060ff,"Ten gracz przed chwila otrzymal ostrzezenie.");
	else if (pTemp[userid][warningReceived]>0)
		kickPlayer=true;
	

	new buf[255];
	format(buf,sizeof buf,TXT(playerid,463),GetPlayerProperName(userid),reason);
	foreach(i)
		Msg(i,COLOR_ADMIN,buf);

	TextDrawShowForPlayer(userid, gTextDraw[TD_WARNING]);
	SendClientMessage(userid,-1," ");
	SendClientMessage(userid,0xff0000ff,"-------------------------------------------------");
	SendClientMessage(userid,0xff0000ff,"OTRZYMANO OSTRZEZENIE OD ADMINISTRATORA!");
	SendClientMessage(userid,0xffffffff,reason);
	SendClientMessage(userid,0xff0000ff,"-------------------------------------------------");	
	SendClientMessage(userid,-1," ");

	if (kickPlayer)
		return KickPlayerWithReason(userid,playerid,TXT(userid,465));		// user ignores warnings

	PlaySound(userid,1053);	
	if (Audio_IsClientConnected(userid))
		Audio_Play(userid,AUDIOID_WARN_MALE,false,false,true);
	
	SetTimerEx("HidePlayerWarning",5000,false,"d",userid);
	return 1;
}

CMD:unwarn(playerid,params[]){
	if(!IsAdmin(playerid)) return 0;

	new tplayer[24],userid;

	if (!sscanf(params,"s[24]",tplayer))
		FindUIDByNameOrID(tplayer,userid,playerid);
	else
		return Msg(playerid,COLOR_ERROR,"Uzyj: /unwarn <id/nick>");

	if (pTemp[userid][warningReceived]==0)
		return Msg(playerid,COLOR_INFO,"Gracz nie ma zadnych ostrzezen");
	pTemp[userid][warningReceived]=0;
	Msg(userid,COLOR_INFO,TXT(userid,464));
	return Msg(playerid,COLOR_INFO,"Ostrzezenia zostaly usuniete");
}

CMD:ach(playerid,params[]){
	if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
    new userid,reason[128];
    if (sscanf(params,"ds[128]",userid,reason))
		return Msg(playerid,0xffffffff,"/ach {b}<id> <powod>{/b}");
	if (!IsPlayerConnected(userid)) 
		return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");

	ShowPlayerAchievement(userid,reason);
	
	return 1;

}

CMD:raceend(playerid){
	if(!IsAdmin(playerid)) return 0;
	Race_Cleanup();
	aData[A_RACE][aState] = A_STATE_OFF;
	return 1;
}

CMD:driftend(playerid){
	if(!IsAdmin(playerid)) return 0;
	SendClientMessage(playerid,-1,"Konczenie /drifting");
	aData[A_DRIFT][aState] = A_STATE_OFF;
	Drift_Cleanup();
	aData[A_DRIFT][aState] = A_STATE_OFF;
	return 1;
}




CMD:wgend(playerid){
	if(!IsAdmin(playerid)) return 0;
	WG_Finish();
	aData[A_WG][aState] = A_STATE_OFF;
	return 1;
}

CMD:chend(playerid){
	if(!IsAdmin(playerid)) return 0;
	CH_Finish();
//	aData[A_CHOWANY][aState] = A_STATE_OFF;
	return 1;
}

CMD:derbyend(playerid){
	if(!IsAdmin(playerid)) return 0;
	Derby_Cleanup();
	aData[A_DERBY][aState] = A_STATE_OFF;
	return 1;
}

/*CMD:sudo(playerid,params[]){
	if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
	new userid,cmd[16],eparams[255],buf[255];
	if (sscanf(params,"ds[16]S[255]",userid,cmd,eparams))
		return Msg(playerid,0xffffffff,"/sudo {b}<id> <cmd> [argumenty]{/b}");
	if (!IsPlayerConnected(userid)) 
		return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
	if (cmd[0]=='/') return Msg(playerid,COLOR_ERROR,"Pomin / na poczatku");
	format(cmd,sizeof cmd,"cmd_%s",cmd);
	format(buf,sizeof buf,"Wykonywanie {b}/%s %s{/b} jako {b}%s{/b}", cmd, eparams, GetPlayerNick(userid));
	return CallLocalFunction(cmd,"ds",userid,eparams);
}*/

CMD:givescore(playerid,params[]){
	if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
    new userid,amount,reason[128];
    if (sscanf(params,"dds[128]",userid,amount,reason))
		return Msg(playerid,COLOR_INFO,"/givescore {b}<id> <ilosc> <powod>{/b}");
	if (amount>150)
		return Msg(playerid,COLOR_ERROR,"Maksymalnie 150! Wieksze ilosci mozna dodawac tylko przez baze danych.");
	if (!IsPlayerConnected(userid)) 
		return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");

	if (!pData[playerid][loggedIn])
		return Msg(playerid,COLOR_ERROR,"Ten gracz nie jest zarejestrowany!");
	else
		GivePlayerScore(userid,amount,reason);
	
	return 1;
}

CMD:givescoreall(playerid,params[]){
	if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
    new amount,reason[64], reason2[128];
    if (sscanf(params,"ds[64]",amount,reason))
		return Msg(playerid,COLOR_INFO,"/givescoreall {b}<ilosc> <powod>{/b}");
	if (amount>150)
		return Msg(playerid,COLOR_ERROR,"Maksymalnie 150! Wieksze ilosci mozna dodawac tylko przez baze danych.");

	format(reason2, sizeof reason2, "Administrator {b}%s{/b} dal wszystkim {b}%d %s{/b} z powodu: {b}%s{/b}", GetPlayerProperName(playerid), amount, 
			dli(amount, "punkt respektu", "punkty respektu", "punktow respektu"), reason);

	foreach(i) {
		GivePlayerScore(i, amount,reason);
		Msg(i, COLOR_INFO, reason2);
	}
	
	return 1;
}

CMD:givecashall(playerid,params[]){
	if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
    new amount,reason[64], reason2[128];
    if (sscanf(params,"ds[64]",amount,reason))
		return Msg(playerid,COLOR_INFO,"/givecashall {b}<ilosc> <powod>{/b}");
	if (amount>150000)
		return Msg(playerid,COLOR_ERROR,"Maksymalnie 150k! Wieksze ilosci mozna dodawac tylko przez baze danych.");

	format(reason2, sizeof reason2, "Administrator {b}%s{/b} dal wszystkim {b}%d %s{/b} z okazji: {b}%s{/b}", GetPlayerProperName(playerid), amount, 
			dli(amount, "dolara", "dolary", "dolarow"), reason);

	foreach(i) {
		GivePlayerMoney(i, amount);
		Msg(i, COLOR_INFO, reason2);
	}
	
	return 1;
}


CMD:tp(playerid, params[])
{
	MUST_BE_SPAWNED(playerid);
	if(!IsAdmin(playerid)) return 0;
	if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 360)); // U¿ywaj: /tp <gracz 1> <gracz 2>
	
	new
	 playerid1,
	 playerid2,
	 szPlayerName1[24],
	 szPlayerName2[24],
	 buffer[128];
	
	sscanf(params, "s[24]s[24]", szPlayerName1, szPlayerName2);
	playerid1 = FindPlayer(szPlayerName1);
	
	if(playerid1 == ERROR_NOT_FOUND)
	{
		format(buffer, sizeof buffer, TXT(playerid, 27), szPlayerName1);
		return Msg(playerid, COLOR_ERROR, buffer);
	}
	if(playerid1 == ERROR_FOUND_MULTIPLE)
	{
		format(buffer, sizeof buffer, TXT(playerid, 28), szPlayerName1);
		return Msg(playerid, COLOR_ERROR, buffer);
	}
	
	if(!TeleportAllowed(playerid1)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 377));
	
	playerid2 = FindPlayer(szPlayerName2);
	
	if(playerid2 == ERROR_NOT_FOUND)
	{
		format(buffer, sizeof buffer, TXT(playerid, 27), szPlayerName2);
		return Msg(playerid, COLOR_ERROR, buffer);
	}
	if(playerid2 == ERROR_FOUND_MULTIPLE)
	{
		format(buffer, sizeof buffer, TXT(playerid, 28), szPlayerName2);
		return Msg(playerid, COLOR_ERROR, buffer);
	}
	
	if(!TeleportAllowed(playerid2)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 378));
	if(playerid1 == playerid2) return Msg(playerid, COLOR_ERROR, TXT(playerid, 385));
	
	new
	 pVector[e_Vectors],
	 _interior,
	 _vw;
	
	GetPlayerPosition(playerid2, pVector[X], pVector[Y], pVector[Z], pVector[A]);
	_interior = GetPlayerInterior(playerid2);
	_vw = GetPlayerVirtualWorld(playerid2);
	
	Teleport(T_PLAYER, playerid1, pVector[X] + 1.0, pVector[Y], pVector[Z], pVector[A], _interior, _vw);
	
	format(buffer, sizeof buffer, TXT(playerid, 361), GetPlayerNick(playerid1), GetPlayerNick(playerid2));
	Msg(playerid, COLOR_INFO, buffer); // Teleportowa³eœ gracza xxx do pozycji gracza xxx.
	
	format(buffer, sizeof buffer, TXT(playerid, 362), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), GetPlayerNick(playerid2));
	Msg(playerid1, COLOR_INFO2, buffer); // Admin xxx teleportowa³ Ciê do pozycji gracza xxx.
	
	format(buffer, sizeof buffer, TXT(playerid, 363), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), GetPlayerNick(playerid1));
	Msg(playerid2, COLOR_INFO2, buffer); // Admin xxx teleportowa³ gracza xxx do Twojej pozycji.
	
	return 1;
}
CMD:info(playerid,params[]){
	if(!IsAdmin(playerid)) return 0;

	new tplayer[24],uid;

	if (isnull(params) && pData[playerid][spectating]!=INVALID_PLAYER_ID)
		uid=pData[playerid][spectating];
	else if (!sscanf(params,"s[24]",tplayer))
		FindUIDByNameOrID(tplayer,uid,playerid);
	else
		return Msg(playerid,COLOR_ERROR,"/info [id/nick]");

	if (!IsPlayerConnected(uid))
		return Msg(playerid,COLOR_ERROR,"Wskazany gracz nie jest podlaczony.");
		
	SendClientMessage(playerid,0xffffffff,"");
	new buf[128],Float:hp,Float:ar;
	GetPlayerHealth(uid,hp);
	GetPlayerArmour(uid,ar);
	if (IsPlayerNPC(uid)) {
		format(buf,sizeof buf,"NICK {b}%s{/b}, ID: {b}%d{/b} - NPC/BOT", GetPlayerNick(uid), uid);
		Msg(playerid,COLOR_INFO2,buf);
		
	} else {
		format(buf,sizeof buf,"NICK {b}%s{/b}, ID: {b}%d{/b} HP: {b}%03.1f{/b}, AR: {b}%03.1f{/b}", GetPlayerNick(uid), uid, hp,ar);
		if (pTemp[uid][staleTime]>1) format(buf, sizeof buf, "%s, AFK od %ds", buf, pTemp[uid][staleTime]);
		Msg(playerid,COLOR_INFO,buf,false);

		if (pData[uid][pAttraction] == A_WG && hp>99)
			Msg(playerid, COLOR_ERROR, "Gracz ma 100HP, chociaz max to 99. Prawdopodobny HH lub lost");
		if (pData[uid][pAttraction] == A_WG && ar>0)
			Msg(playerid, COLOR_ERROR, "Gracz ma 100AR, chociaz powinien miec 0. Prawdopodobny AH lub lost");

		format(buf,sizeof buf,"IP: %s. PING aktualny: {b}%d{/b}ms, sredni: {b}%d{/b}ms", GetPlayerIP(uid), GetPlayerPing(uid), pData[uid][averagePing]);
		Msg(playerid,COLOR_INFO,buf,false);

		format(buf,sizeof buf,"%s. Respekt: {b}%d{/b}, Skill: {b}%d{/b}, Gotowka: %d, Gang: %d, killStreak: {b}%d{/b}",
				((pData[uid][loggedIn]) ? ("ZALOGOWANY") : "NIEZALOGOWANY"), pData[uid][respect], pData[uid][skill], GetPlayerMoney(uid), pData[uid][gang], pTemp[uid][killStreak] );
		Msg(playerid,COLOR_INFO,buf,false);
	}
	new playerState = GetPlayerState(uid);
	switch (playerState){
		case PLAYER_STATE_NONE:
			format(buf,sizeof buf,"Stan: {b}NONE{/b}");
		case PLAYER_STATE_ONFOOT:
			format(buf,sizeof buf,"Stan: {b}PIESZY{/b}");
		case PLAYER_STATE_DRIVER:
			format(buf,sizeof buf,"Stan: {b}KIEROWCA{/b}");
		case PLAYER_STATE_PASSENGER:
			format(buf,sizeof buf,"Stan: {b}PASAZER{/b}");
		case 4:
			format(buf,sizeof buf,"Stan: {b}WYSIADA Z POJAZDU{/b}");
		case 5:
			format(buf,sizeof buf,"Stan: {b}WSIADA JAKO KIEROWCA{/b}");
		case 6:
			format(buf,sizeof buf,"Stan: {b}WSIADA JAKO PASAZER{/b}");
		case PLAYER_STATE_WASTED:
			format(buf,sizeof buf,"Stan: {b}MARTWY{/b}");
		case PLAYER_STATE_SPAWNED:
			format(buf,sizeof buf,"Stan: {b}ZESPAWNOWANY{/b}");
		case PLAYER_STATE_SPECTATING:
			format(buf,sizeof buf,"Stan: {b}SPEC{/b}");
	}
	new specact=GetPlayerSpecialAction(uid);
	new aniidx=GetPlayerAnimationIndex(uid);
	
	new animlib[32]="-";
	new animname[32]="-";
	if (aniidx) 
		GetAnimationName(aniidx,animlib,32,animname,32);

	format(buf,sizeof buf,"%s, akcja: {b}%d{/b}, animacja {b}%s:%s (%d){/b}",buf,specact, animlib,animname, aniidx);
	Msg(playerid,COLOR_INFO,buf, false);

	format(buf,sizeof buf,"bonusHours: %d, accountid: %d, jail: %d", 
			pTemp[uid][bonusHours], pData[uid][accountID], pData[uid][jail]);

	if (pData[uid][pAttraction]>=0)
		format(buf,sizeof buf, "%s atrakcja: %d (%s)", buf, pData[uid][pAttraction], aData[pData[uid][pAttraction]][aName]);
	
	
	Msg(playerid,COLOR_INFO,buf, false);

	new cursess=GetPlayerCurrentSession(uid);
	format(buf,sizeof buf, "W grze od {b}%dm%02ds{/b}.", floatround(cursess/60, floatround_floor), cursess%60 );

	if (gmData[artefactOwner]==uid) {
		format(buf, sizeof buf, "%sNosi artefakt od {b}%ds{/b}", buf, ((GetTickCount() / 1000) - pTemp[gmData[artefactOwner]][artefactStartTime]));
	}

	Msg(playerid,COLOR_INFO,buf,false);

	format(buf,sizeof buf,"%d", uid);	// lista broni
	cmd_cwep(playerid,buf);

	if(IsAdmin(playerid,LEVEL_ADMIN3) && pData[uid][logonDialog])
		Msg(playerid,COLOR_INFO,"Gracz w trakcie logowania",false);
	
	if (IsPlayerInAnyVehicle(uid)){
		new vid=GetPlayerVehicleID(uid);
		format(buf, sizeof buf, "Pojazd ID %d.", vid);
		Msg(playerid,COLOR_INFO,buf,false);
	}


	return 1;
}

CMD:restartaudio(playerid,params[]){
    if(!IsAdmin(playerid)) return 0;
    new userid;
    if (sscanf(params,"d",userid))
        userid=playerid;
    else if (!IsPlayerConnected(userid))
        return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
//	Audio_RemovePlayer(userid);
	Audio_SetPack("fullserver", true);
//	Audio_RetransferPack(userid);
	return 1;
}

CMD:wiezienie(playerid,params[]){
	if(!IsGM(playerid)) return 0;
	MoveToJail(playerid);
	return 1;
}

CMD:streamerinfo(playerid){
	if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	new buf[255];
	format(buf,sizeof buf,"Wszystkich obiektow: {b}%d{/b}", CountDynamicObjects());
	Msg(playerid,COLOR_INFO,buf);
	format(buf,sizeof buf,"Widocznych przez Ciebie obiektow: {b}%d{/b}",  Streamer_CountVisibleItems(playerid, STREAMER_TYPE_OBJECT));
	Msg(playerid,COLOR_INFO,buf);
	return 1;
}

CMD:streamtr(playerid,params[]){
	if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	new tr;
	if (sscanf(params,"d",tr) || tr<=0 || tr>100)
		return Msg(playerid,COLOR_ERROR,"/streamertr {b}0-100{/b}, domyslnie jest 50");
	Streamer_TickRate(tr);
	Msg(playerid,COLOR_INFO,"Ustawione");
	return 1;
}

CMD:makevip(playerid,params[]){
	if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	new userid,tplayer[24];
	if (isnull(params) && pData[playerid][spectating]!=INVALID_PLAYER_ID)
		userid=pData[playerid][spectating];
	else if (!sscanf(params,"s[24]",tplayer))
		FindUIDByNameOrID(tplayer,userid,playerid);
	else
		return Msg(playerid,COLOR_ERROR,"/makevip [id/nick]");

	if (!IsPlayerConnected(userid)) 
		return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
	if (!pData[userid][loggedIn])
		return Msg(playerid,COLOR_ERROR,"Gracz nie jest zarejestrowany!");

	pData[userid][vipEnabled]=true;
	Msg(playerid,COLOR_INFO2,"Aktywowales graczowi VIPa do wylogowania.");
	new buf[255];
	format(buf,sizeof buf,"Otrzymal%s {b}uprawnienia VIPa{/b} od administratora {b}%s{/b}. Bedzie on aktywny do wylogowania sie.", 
			(SkinKobiecy(GetPlayerSkin(userid))) ? ("es") : ("as"), GetPlayerProperName(playerid));

	Msg(userid,COLOR_INFO2,buf);
	SetPlayerProperColor(userid);
	UpdatePlayerNickTD(userid);
	return 1;
}

CMD:makegm(playerid,params[]){
	if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	new userid,tplayer[24];
	if (isnull(params) && pData[playerid][spectating]!=INVALID_PLAYER_ID)
		userid=pData[playerid][spectating];
	else if (!sscanf(params,"s[24]",tplayer))
		FindUIDByNameOrID(tplayer,userid,playerid);
	else
		return Msg(playerid,COLOR_ERROR,"/makegm [id/nick]");

	if (!IsPlayerConnected(userid)) 
		return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
	if (!pData[userid][loggedIn])
		return Msg(playerid,COLOR_ERROR,"Gracz nie jest zarejestrowany!");

	if (pData[userid][adminLevel]>LEVEL_GM)
		return Msg(playerid, COLOR_ERROR, "Ten gracz ma juz wyzsze uprawnienia!");

	pData[userid][adminLevel]=LEVEL_GM;
	Msg(playerid,COLOR_INFO2,"Aktywowales graczowi GM do wylogowania.");
	new buf[255];
	format(buf,sizeof buf,"Otrzymal%s {b}uprawnienia GM{/b} od administratora {b}%s{/b}. Beda one aktywne do wylogowania sie.", 
			(SkinKobiecy(GetPlayerSkin(userid))) ? ("es") : ("as"), GetPlayerProperName(playerid));

	Msg(userid,COLOR_INFO2,buf);
	SetPlayerProperColor(userid);
	UpdatePlayerNickTD(userid);
	return 1;
}

CMD:makeadmin(playerid,params[]){
	if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	new userid,tplayer[24];
	if (isnull(params) && pData[playerid][spectating]!=INVALID_PLAYER_ID)
		userid=pData[playerid][spectating];
	else if (!sscanf(params,"s[24]",tplayer))
		FindUIDByNameOrID(tplayer,userid,playerid);
	else
		return Msg(playerid,COLOR_ERROR,"/makegm [id/nick]");

	if (!IsPlayerConnected(userid)) 
		return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
	if (!pData[userid][loggedIn])
		return Msg(playerid,COLOR_ERROR,"Gracz nie jest zarejestrowany!");

	if (pData[userid][adminLevel]!=LEVEL_GM)
		return Msg(playerid, COLOR_ERROR, "Najpierw nadaj tej osobie uprawnienia GM!");
	if (pData[userid][adminLevel]>LEVEL_ADMIN1)
		return Msg(playerid, COLOR_ERROR, "Ten gracz ma juz wyzsze uprawnienia!");

	pData[userid][adminLevel]=LEVEL_ADMIN1;
	Msg(playerid,COLOR_INFO2,"Aktywowales graczowi ADMINa do wylogowania.");
	new buf[255];
	format(buf,sizeof buf,"Otrzymal%s {b}uprawnienia admina{/b} od {b}%s{/b}. Beda one aktywne do wylogowania sie.", 
			(SkinKobiecy(GetPlayerSkin(userid))) ? ("es") : ("as"), GetPlayerProperName(playerid));

	Msg(userid,COLOR_INFO2,buf);
	SetPlayerProperColor(userid);
	UpdatePlayerNickTD(userid);
	return 1;
}



/*
CMD:setwepskill(playerid,params[]){
	if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	new userid,wepid,wskill;
    if (sscanf(params,"ddd",userid,wepid,wskill))
		return Msg(playerid,COLOR_INFO,"/setwepskill {b}<id gracza> <id broni> <0-1000>{/b}");
	if (!IsPlayerConnected(userid)) 
		return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
//	if (wskill>1000) wskill=1000;
	SetPlayerSkillLevel(userid,wepid,wskill);
	new buf[255];
	format(buf, sizeof buf, "Admin %s ustawil Ci skilla broni %d na %d%%", GetPlayerProperName(playerid), wepid, floatround(wskill/10));
	Msg(userid,COLOR_INFO,buf);
	Msg(playerid,COLOR_INFO,"Ustawiono");
	return 1;
}
*/

CMD:givegun(playerid,params[]){
	if(!IsAdmin(playerid,LEVEL_GM)) return 0;
	new userid,wepid,ammo;
    if (sscanf(params,"ddd",userid,wepid,ammo))
		return Msg(playerid,COLOR_INFO,"/givegun {b}<id gracza> <id broni> <amunicja>{/b}");
	if (!IsPlayerConnected(userid)) 
		return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
	if (wepid<0 || wepid>43 || wepid==19 || wepid==20 || wepid==21 || wepid==13 || wepid==36 || wepid==44 || wepid==45)
		return Msg(playerid,COLOR_ERROR,"Bledne ID broni");
	if (wepid<=15 && ammo>1)
		ammo=1;
	if (ammo>10000 || ammo<0)
		ammo=10000;
	
	GivePlayerWeapon(userid, wepid, ammo);
	if (wepid<=15)
		SetPlayerAmmo(userid,wepid,1);

	return Msg(playerid,COLOR_INFO,"Done.");
}

CMD:givegunall(playerid) {
	if(!IsAdmin(playerid)) return 0;
	return Msg(playerid,COLOR_ERROR,"Uzyj: {b}/gga{/b}. Ta komenda zostanie wkrotce usunieta.");
}

CMD:gga(playerid,params[]){
	if(!IsAdmin(playerid)) return 0;
	new wepid,ammo;
    if (sscanf(params,"dD(5000)",wepid,ammo))
		return Msg(playerid,COLOR_INFO,"/givegunall {b}<id broni> [amunicja]{/b}");

	if (wepid<0 || wepid>43 || wepid==19 || wepid==20 || wepid==21 || wepid==13 || wepid==36 || wepid==44 || wepid==45)
		return Msg(playerid,COLOR_ERROR,"Bledne ID broni");
	if (wepid<=15 && ammo>1)
		ammo=1;
	if (ammo>50000 || ammo<0)
		ammo=50000;
	new cnt;
	new buffer[128];

	format(buffer,sizeof buffer,"Administartor%s rozdal%a wszystkim bron: {b}%s{/b}.", Kobieta(playerid)?("ka"):(""), Kobieta(playerid)?("a"):(""), weaponName[wepid]);
	foreach(i) 
	if ((pData[i][pAttraction]==A_NONE || pData[i][pAttraction]==A_ARENA) && pTemp[i][onArena]!=ARENA_SOLO && pTemp[i][weaponsAllowed] && !pTemp[i][troll])
	{
		GivePlayerWeapon(i, wepid, ammo);
		if (wepid<=15)
			SetPlayerAmmo(i,wepid,1);
		else if (ammo==50000)
			SetPlayerAmmo(i,wepid,ammo);
		Msg(i,COLOR_INFO,buffer,true);
		cnt++;
	}

	format(buffer,sizeof buffer,"Bron otrzymalo {b}%d %s{/b}.", cnt, dli(cnt,"gracz","graczy","graczy"));
	Msg(playerid,COLOR_INFO,buffer);

	return Msg(playerid,COLOR_INFO,"Done.");
}

CMD:setcash(playerid,params[]){
	if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	new tplayer[24],amount,uid;
	if (!sscanf(params,"s[24]d",tplayer,amount)) {
		FindUIDByNameOrID(tplayer,uid,playerid);
	} else
		return Msg(playerid,COLOR_ERROR,"/setcash <id/nick> <kwota>");

	if (amount<0 || amount>50000000)
		return Msg(playerid,COLOR_ERROR,"Podaj kwote w zakresie 0-50mln");

	ResetPlayerMoney(uid);
	GivePlayerMoney(uid,amount);

	return Msg(playerid,COLOR_INFO,"Ustawione");
}

CMD:getcash(playerid,params[]){
	if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	new amount;
    if (sscanf(params,"d",amount))
		return Msg(playerid,COLOR_INFO,"/getcash {b}<kwota>{/b}");
	new curcash=GetPlayerMoney(playerid);

	if (curcash>=9999999)
		return Msg(playerid,COLOR_ERROR,"Masz juz maksymalna ilosc gotowki");
	if (curcash+amount>99999999)
		return Msg(playerid,COLOR_ERROR,"Za duzo!");
	SetPlayerMoney(playerid,curcash+amount);
	return Msg(playerid,COLOR_INFO,"Done.");
}


CMD:pomnikireload(playerid){
	if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	obiekty_odswiezPomniki();
	return Msg(playerid,COLOR_INFO,"DONE");
}

CMD:inserttoveh(playerid,params[]){
    if (!IsPlayerAdmin(playerid)) return 0;
    new userid,vehid;
    if (sscanf(params,"dd",userid,vehid))
        return Msg(playerid,COLOR_ERROR,"/inserttoveh <playerid> <vehicleid>");
    PutPlayerInVehicle(userid,vehid,0);
    return 1;
}

CMD:pigula(playerid,params[]){
	if (!IsPlayerAdmin(playerid)) return 0;
	new uid,Float:P[3];
	if (sscanf(params,"d",uid))
		return Msg(playerid,COLOR_ERROR,"/pigula id");
	if (IsPlayerConnected(uid)) {
			SetPlayerDrunkLevel(uid,10000);
			if (!IsPlayerInAnyVehicle(uid)) {
				GetPlayerPos(uid,P[0],P[1],P[2]);
				new p=CreateDynamicPickup(1241,3,P[0],P[1],P[2],-1,-1,uid);
				Streamer_Update(uid);
				SetPlayerPos(uid,P[0],P[1],P[2]+1);
				SetTimerEx("UsunPickup",1000,false,"d",p);
			}
		}
	return 1;
}

CMD:odtrucie(playerid,params[]){
	if (!IsPlayerAdmin(playerid)) return 0;
	new uid;
	if (sscanf(params,"u",uid))
		return Msg(playerid,COLOR_ERROR,"/odtrucie id");
	if (IsPlayerConnected(uid)) 
		SetPlayerDrunkLevel(uid,0);
	return 1;	
}

CMD:getdist(playerid,params[]){
	if (!IsPlayerAdmin(playerid)) return 0;
	new u1,u2;
	if (sscanf(params,"uu",u1,u2))
		return Msg(playerid,COLOR_ERROR,"/getdist user1 user2");
	new Float:FP[6];
	GetPlayerPos(u1,FP[0],FP[1],FP[2]);
	GetPlayerPos(u2,FP[3],FP[4],FP[5]);
	new Float:dist=GetDistanceBetweenPoints3D(FP[0],FP[1],FP[2],FP[3],FP[4],FP[5]);
	new buf[32];
	format(buf, sizeof buf,"Odleglosc: %f", dist);
	return Msg(playerid,COLOR_INFO, buf);

}

CMD:forcearena(playerid,params[]){
	if (!IsAdmin(playerid)) return 0;
	new atrakcja[6],arenaid;
	if (sscanf(params,"s[6]d",atrakcja,arenaid))
		return Msg(playerid,COLOR_INFO2,"/forcearena <derby/race/drift/wg/ch/chowany> <id>");
	if (strcmp(atrakcja,"derby")==0)
		gmTemp[aDerbyForceArena]=arenaid;
	else if (strcmp(atrakcja,"race")==0)
		gmTemp[aRaceForceCircuit]=arenaid;
	else if (strcmp(atrakcja,"drift")==0)
		gmTemp[aDriftForceCircuit]=arenaid;
	else if (strcmp(atrakcja,"wg")==0)
		gmTemp[aWGForceArena]=arenaid;
	else if (strcmp(atrakcja,"ch")==0 || strcmp(atrakcja,"chowany")==0)
		gmTemp[aChowanyForceArena]=arenaid;
	else
		return Msg(playerid,COLOR_ERROR,"Nieprawidlowa nazwa atrakcji");
	Msg(playerid,COLOR_INFO,"Ustawione");
	return 1;
}

CMD:cwep(playerid,params[]){
	if (!IsAdmin(playerid)) return 0;
	new uid,tplayer[24];
	if (sscanf(params,"s[24]",tplayer))
		return Msg(playerid,COLOR_INFO2,"/cwep <id/nick>");

	FindUIDByNameOrID(tplayer,uid,playerid);

	GetPlayerWeaponList(playerid,uid);
	return 1;
}

CMD:cgod(playerid){
	if (!IsAdmin(playerid)) return 0;
	new Float:AR, Float:HP,score;
	foreach(i) 
	 if (IsPlayerSpawned(i)) {
		GetPlayerHealth(i,HP);
		GetPlayerArmour(i,AR);
		score=0;
		new buf[127];
		if ((pData[i][loggedIn] && pTemp[i][killStreak]>10) || (!pData[i][loggedIn] && pTemp[i][killStreak]>5)) {
			score++;
			format(buf, sizeof buf, ", duzo zabic pod rzad");
		}
		if (HP>100 && !(pData[i][pAttraction]==A_CHOWANY && pTemp[i][aChowanySide] == A_CHOWANY_SEARCHING)) {
			score++;
			format(buf, sizeof buf, "%s, god otrzymany od admina/serwera", buf);
		}
		if (HP>99 && 
				((pData[i][pAttraction]==A_CHOWANY && pTemp[i][aChowanySide] == A_CHOWANY_HIDDING) || (pData[i][pAttraction]==A_WG))) {
			score++;
			format(buf, sizeof buf, "%s, prawdopodobny HH", buf);
		}
		if (score>0) {
			format(gstr, sizeof gstr, "%s(%d) - HP: %3.0f, AR: %3.0f, zabic pod rzad: %d%s", GetPlayerNick(i), i, HP, AR, pTemp[i][killStreak], buf);
			SCM(playerid, -1, gstr);
		}
	 }
	SCM(playerid, -1, "--");
	return 1;

}

CMD:settime(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	new h;
	if (sscanf(params,"u",h))
		return Msg(playerid,COLOR_INFO2,"/settime <h:0-23>");
	h=h%24;
	new oh=floatround((GetTickCount()/1000)/60);
	gmTemp[gametime_offset]=(h-oh)*60;
	foreach(i)
		SyncPlayerGameTime(i);
	return 1;
}

CMD:killplayer(playerid, params[])
{
	if (!IsAdmin(playerid)) return 0;

	if (isnull(params)) {
		SetPlayerHealth(playerid,0.0);
		return Msg(playerid, COLOR_INFO, "Zabiles/-as sie", false);
	}
	
	
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
	
	if(!IsPlayerSpawned(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 379));
	
	SetPlayerHealth(targetplayerid, 0.0);
	
	format(buffer, sizeof buffer, TXT(playerid, 365), GetPlayerNick(targetplayerid));
	Msg(playerid, COLOR_INFO, buffer);
	
	if(targetplayerid == playerid) return 1;
	
	format(buffer, sizeof buffer, TXT(playerid, 366), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
	Msg(targetplayerid, COLOR_INFO2, buffer);
	
	return 1;
}

CMD:playall(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN2)) return 0;
	new audioid;
	if (sscanf(params,"d",audioid))
		Msg(playerid, COLOR_ERROR, "/playall <id>");

	if (audioid==0 || (pData[playerid][adminLevel]<=LEVEL_ADMIN2 && (audioid<200 || audioid>300))) {
		SCM(playerid, -1, "200 miota nim 201 meeee, 202 onajestpedalem");
		return 1;
	}

	if (pData[playerid][adminLevel]<=LEVEL_ADMIN2 && GetTickCount()-gmTemp[playallLast]<(60*1000*3))
		return Msg(playerid,COLOR_ERROR,"Musza minac przynajmniej {b}3 minuty{/b} od ostatniego odtwarzania.");
	gmTemp[playallLast]=GetTickCount();

	new cnt=0;
	foreach (i)
		if (Audio_IsClientConnected(i)){
			Audio_Play(i,audioid,false,false,true);
			cnt++;
		}
	new buf[128];
	format(buf,sizeof buf,"Odtworzono u graczy: %d", cnt);
	return 1;
}

CMD:resetgangscore(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	new gid;
	if (sscanf(params,"d",gid) || gid<=0 || gid>MAX_GANGS)
		return Msg(playerid, COLOR_ERROR, "/resetgangscore <gid>");
	gData[gid][respect]=0;
	Msg(playerid,COLOR_INFO, "Respekt wyzerowany");
	return 1;
}

CMD:vehinfo(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	new vid;
	if (sscanf(params,"d",vid))
		return Msg(playerid,COLOR_ERROR, "/vehinfo <vid>");
	new vmodel, Float:vP[4];
	vmodel=GetVehicleModel(vid);
	GetVehiclePos(vid,vP[0],  vP[1],  vP[2]);
	GetVehicleZAngle(vid, vP[3]);
	new buf[128];
	format(buf,sizeof buf,"model: %d, XYZA: %.2f %.2f %.2f %.2f", vmodel, vP[0], vP[1], vP[2], vP[3]);
	Msg(playerid,COLOR_INFO,buf,false);
	format(buf,sizeof buf,"static:%d/%d destroyIfEmpty:%d plateset:%d priv:%d, houseid {b}%d{/b}, owningplayerid %d, colors:%d,%d", 
			(vid<=staticVehicleCount), tVehicles[vid][vo_static], 
			tVehicles[vid][vo_destroyIfEmpty], tVehicles[vid][vo_licensePlateSet], tVehicles[vid][vo_private], tVehicles[vid][vo_houseid], tVehicles[vid][vo_owningPlayerId],
			tVehicles[vid][vo_color][0], tVehicles[vid][vo_color][1]);
	Msg(playerid,COLOR_INFO2,buf,false);

	format(buf,sizeof buf,"nitro: %d, turbo: %d, kierowca: %d, drift: %s", vehicleHasNitro[vid], tVehicles[vid][vo_hasTurbo], tVehicles[vid][vo_driver], tVehicles[vid][vo_drift]?("TAK"):("NIE"));
	Msg(playerid,COLOR_INFO2,buf,false);

	return 1;
}

CMD:findvehmodel(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	new vm,buf[128];
	if (sscanf(params,"d",vm)) return Msg(playerid,COLOR_ERROR,"/findvehmodel <id>");
	for (new i=0; i<=MAX_VEHICLES; i++)
		if (GetVehicleModel(i)==vm) {
			format(buf, sizeof buf, "vid:{b}%d{/b}, P:{b}%.1f %.1f %.1f{/b}");
			Msg(playerid,COLOR_INFO,buf,false);
		}
	return 1;

}

CMD:visibility(playerid,params[]){
	if (!IsAdmin(playerid)) return 0;

	new tplayer[24],vis,uid;
	if (!sscanf(params,"s[24]d",tplayer,vis)) {
		FindUIDByNameOrID(tplayer,uid,playerid);
	} else if (!sscanf(params,"d",vis)) {
		uid=playerid;
	} else
		return Msg(playerid,COLOR_ERROR,"/visibility [id gracza] <widocznosc 0-100>");

	if (vis<0 || vis>100)
		return Msg(playerid,COLOR_ERROR,"Podaj widocznosc w zakresie od 0 do 100");

	pTemp[uid][playerColorAlpha]=floatround(0xFF*vis/100,floatround_floor);
	SetPlayerProperColor(uid);
	return Msg(playerid,COLOR_INFO,"Ustawione");
}

CMD:invisible(playerid){
	if (!IsGM(playerid)) return 0;

	pTemp[playerid][playerColorAlpha]=0;
	SetPlayerProperColor(playerid);

	return Msg(playerid,COLOR_INFO,"Jestes niewidzialny na radarze!");
}

CMD:visible(playerid){
	if (!IsGM(playerid)) return 0;

	pTemp[playerid][playerColorAlpha]=PLAYER_COLOR_ALPHA;
	SetPlayerProperColor(playerid);

	return Msg(playerid,COLOR_INFO,"Jestes widzialny na radarze!");
}



CMD:bench(playerid){
	if (!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	gmTemp[performBenchmark]=10;
	return 1;
}


CMD:podgladpm(playerid){
	return Msg(playerid, COLOR_INFO2, "Uzyj {b}/toggle podgladpm{/b}");
/*	if (!IsAdmin(playerid,LEVEL_ADMIN2)) return 0;

	return 1;*/
}

CMD:cwh(playerid){
	if (!IsAdmin(playerid,LEVEL_ADMIN2)) return 0;
	new buf[128];
	foreach(i){
	    new wepid,ammo, podejrzany=0;
	    for (new slot=0; slot<=12;slot++) {
			wepid=0; ammo=0;
	        GetPlayerWeaponData(i,slot,wepid,ammo);
			if (wepid==0 && ammo==0) continue;
			if (wepid>=0 && wepid<=15 && ammo>1) podejrzany+=(ammo<=3?1:2);
			if (ammo==198) podejrzany+=2;
			if (wepid==36 || wepid==44 || wepid==45) podejrzany+=2;
			if (pData[i][pAttraction]!=A_ARENA && ((wepid==38 && pData[i][pAttraction]!=A_CHOWANY) || wepid==37)) podejrzany++;
			if ((ammo<0 && pData[i][pAttraction]!=A_CHOWANY) || ammo>9000) podejrzany++;
    	}
		if (podejrzany>1) {
			format(buf, sizeof buf, "%s(%d) - {b}%d{/b} %s", GetPlayerNick(i), i, podejrzany, dli(podejrzany,"punkt","punkty","puntkow"));
			Msg(playerid,COLOR_INFO,buf,false);
			format(buf, sizeof buf, "%d", i);
			cmd_cwep(playerid,buf);
		}	
	}
	return 1;
}

CMD:toggle(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN2)) return 0;
//	new buf[128];

	if (!strcmp(params,"chat",true)) {
		if (!IsAdmin(playerid, LEVEL_ADMIN3)) return Msg(playerid, COLOR_ERROR, "Tylko dla RCONow");
		gmTemp[chatDisabled]=(gmTemp[chatDisabled] ? false : true);
		SendClientMessageToAll(-1," ");
		if (gmTemp[chatDisabled])
			SendClientMessageToAll(0xFF6060,"Czat zostal wylaczony przez administratora.");
		else
			SendClientMessageToAll(0xFF6060,"Czat zostal wlaczony.");
		SendClientMessageToAll(-1," ");
		
		return 1;
	} else if (!strcmp(params,"podgladpm",true)) {
		gmTemp[snoopPM]=((gmTemp[snoopPM]+1)%3);
		switch (gmTemp[snoopPM]) {
			case 0:
				MSGToAdmins(COLOR_INFO2, "Podglad wiadomosci PM zostal {b}wylaczony{/b}", true, LEVEL_ADMIN2);
			case 1: {
				MSGToAdmins(COLOR_INFO2, "Podglad wiadomosci PM zostal {b}wlaczony, wiadomosci graczy <1000 respektu{/b}", true, LEVEL_ADMIN2);
			}
			case 2:
				MSGToAdmins(COLOR_INFO2, "Podglad wiadomosci PM zostal {b}wlaczony, dla wszystkich wiadomosci{/b}", true, LEVEL_ADMIN2);
		}
		
		return 1;
	} else if (!strcmp(params,"join",true)) {
		gmTemp[showJoins]=((gmTemp[showJoins]+1)%3);
		switch (gmTemp[showJoins]) {
			case 0:
				MSGToAdmins(COLOR_INFO2, "Informacje o wejsciu/wyjsciu z serwera {b}wylaczone{/b}", true, LEVEL_ADMIN2);
			case 1:
				MSGToAdmins(COLOR_INFO2, "Informacje o wejsciu/wyjsciu z serwera {b}wlaczone, widoczne dla adminow{/b}", true, LEVEL_ADMIN2);
			case 2:
				MSGToAdmins(COLOR_INFO2, "Informacje o wejsciu/wyjsciu z serwera {b}wlaczone, widoczne dla wszystkich graczy{/b}", true, LEVEL_ADMIN2);
		}
		return 1;
	} else if (!strcmp(params,"protall",true)) {
		gmTemp[protAll]=(gmTemp[protAll] ? false : true);
		foreach(i) {
			SendClientMessage(i,-1, " ");
			if (gmTemp[protAll]) {
				Msg(i,COLOR_ERROR,"WSZYSCY GRACZE OTRZYMALI IMMUNITET.");
				GameTextForPlayer(i, "~r~Wszyscy gracze ~n~~h~otrzymali immunitet", 2000, 1);
			}
			else  {
				Msg(i,COLOR_ERROR,"WSZYSCY GRACZE UTRACILI IMMUNITET.");
				GameTextForPlayer(i, "~r~Wszyscy gracze ~n~~h~utracili immunitet", 2000, 1);
			}

			SendClientMessage(i,-1, " ");
		}
		return 1;
	}



	if (IsAdmin(playerid, LEVEL_ADMIN3))
		return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/toggle <chat|podgladpm|protall|join>{/b}");
	else
		return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/toggle <podgladpm|protall|join>{/b}");

}

CMD:objectsreload(playerid){
	if (pData[playerid][accountID]!=55 && !IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	SendRconCommand("reloadfs objectloader");
	return 1;
}

CMD:svall(playerid){
	if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
	SendClientMessageToAll(-1," ");
	SendClientMessageToAll(0xFB00FBFF,"ZAPISYWANIE WSZYSTKICH DANYCH GRACZY...");
	foreach(i)
		if(pData[i][loggedIn])
			UpdatePlayerAccountData(i,true);
	gangs_saveGangData(true);
	SendClientMessageToAll(0xFfffffFF,"   ... gotowe");
	return 1;
}

CMD:atrakcje(playerid){
	if(!IsAdmin(playerid)) return 0;
	for(new i = 0; i < MAX_ATTRACTIONS; i++)
		if(aData[i][aState] != A_STATE_OFF)
        {
			SendClientMessage(playerid,0x506070ff, aData[i][aName]);
			new uczestnicy[127];
			foreach(pp)
			if(pData[pp][pAttraction]==i)
			{
				format(uczestnicy,sizeof uczestnicy,"%s %s(%d)", uczestnicy, GetPlayerNick(pp),pp);
				if(strlen(uczestnicy)>96) {
					SendClientMessage(playerid,0xd0ccffff,uczestnicy);
					copy("",uczestnicy);
				}
				
			}	
			SendClientMessage(playerid,0xccccccff,uczestnicy);
		}
	return 1;
}

CMD:acolor(playerid,params[]){
	if(!IsGM(playerid)) return 0;
	
	new playerSz[25],color,buffer[128];
	if(sscanf(params,"s[24]D(-1)",playerSz,color))
		return Msg(playerid,COLOR_ERROR,"Uzyj: {b}/acolor <id/nick> [id koloru]. Nie podawaj id koloru aby zresetowac do domyslnego dla gracza.");

	new
	 targetplayerid;
	
	targetplayerid = FindPlayer(playerSz);
	
	if(targetplayerid == ERROR_NOT_FOUND)
	{
		format(buffer, sizeof buffer, TXT(playerid, 27), params);
		return Msg(playerid, COLOR_ERROR, buffer);
	}
	else if(targetplayerid == ERROR_FOUND_MULTIPLE)
	{
		format(buffer, sizeof buffer, TXT(playerid, 28), params);
		return Msg(playerid, COLOR_ERROR, buffer);
	}

	if ((color == 5 || color==11) && pData[playerid][adminLevel]==LEVEL_GM)
		return Msg(playerid, COLOR_ERROR, "Kolor bialy i kolor czarny moze byc nadany tylko przez administratora");
	
	new nkolory[]={
		0xAFAFAF,	// 1 - szary
		0x24FF0a,
		0xAA3333,
		0xFFFF00,	// 4
		0xFFFFFF,	// 	5
		0x0000BB,
		0xFF9900,
		0xFF66FF,// ping 8
		0x39484B,
		0x9955DE,
		0x000000,	// czarny	11
		0x8F4747,	// brown
		0x33ccff,
		0x33aa33,
		0xFF8080,	// krem
		0x303030	// prawie czarny
		};		
	color--;
	if (color>=0 && color<=sizeof nkolory) {
		pData[targetplayerid][currentColor]=nkolory[color];
		SetPlayerColor(targetplayerid, pData[targetplayerid][currentColor]*256+pTemp[targetplayerid][playerColorAlpha]);
		SendClientMessage(playerid, pData[targetplayerid][currentColor]*256+0xff,"Zmieniono kolor gracza na taki jak ten");
		format(gstr,sizeof gstr, "%s%s %s zmienil%s Ci kolor na taki jak ten.", pData[playerid][adminLevel]==LEVEL_GM ? ("Moderator"):("Administrator"), Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""));
		SendClientMessage(targetplayerid, pData[targetplayerid][currentColor]*256+0xff, gstr);
	} else {
		SetPlayerProperColor(targetplayerid);
		Msg(playerid,COLOR_INFO,"Kolor gracza zostal zresetowany.");
	}

	return 1;
}
/*
CMD:remove(playerid,params[]){
	if(!IsAdmin(playerid)) return 0;
	
	new buffer[128];
	if(isnull(params))
		return Msg(playerid,COLOR_ERROR,"Uzyj: {b}/remove <id/nick>{/b}.");
	new
	 targetplayerid;
	
	targetplayerid = FindPlayer(params);
	
	if(targetplayerid == ERROR_NOT_FOUND)
	{
		format(buffer, sizeof buffer, TXT(playerid, 27), params);
		return Msg(playerid, COLOR_ERROR, buffer);
	}
	else if(targetplayerid == ERROR_FOUND_MULTIPLE)
	{
		format(buffer, sizeof buffer, TXT(playerid, 28), params);
		return Msg(playerid, COLOR_ERROR, buffer);
	}
	
	RemovePlayerFromVehicle(playerid);
	Msg(playerid,COLOR_INFO,"Wyrzuciles gracza z pojazdu");
	
	return 1;
}*/
CMD:healall(playerid,params[]){
	if (!IsAdmin(playerid)) return 0;

	new
	 buffer[160],
	 pCount = 0;


	format(buffer,	sizeof buffer,"Administrator%s %s uzdrowila wszystkich graczy.", Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid));
	foreach(i)
	if (IsPlayerSpawned(i) && AttractionAllowed(i)) {
		SetPlayerHealth(i,100.0); pCount++;
		Msg(i, COLOR_INFO, buffer);
		GameTextForPlayer(i,"~g~HP~w~+", 500,6);
	}
	
	format(buffer, sizeof buffer, "Uleczyles {b}%d %s{/b}.", pCount, dli(pCount,"gracza","graczy","graczy"));
	Msg(playerid, COLOR_INFO, buffer);
	
	return 1;
}

CMD:armorall(playerid){
	if (!IsAdmin(playerid)) return 0;

	new
	 buffer[127],
	 pCount = 0;

	format(buffer,	sizeof buffer,"Administrator%s {b}%s{/b} uzupelnil%s wszystkim graczom armora.", Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""));
	foreach(i)
	if (IsPlayerSpawned(i) && AttractionAllowed(i)) {
		SetPlayerArmour(i,100.0); pCount++;
		Msg(i, COLOR_INFO, buffer);
		GameTextForPlayer(i,"~y~AR~w~+", 500,6);
	}
	
	format(buffer, sizeof buffer, "Armor otrzymalo {b}%d %s{/b}.", pCount, dli(pCount,"gracza","graczy","graczy"));
	Msg(playerid, COLOR_INFO, buffer);
	
	return 1;

}

CMD:saveconfig(playerid){
	if (!IsAdmin(playerid,LEVEL_ADMIN3) || !IsPlayerAdmin(playerid)) return 0;	// nadmierna ostroznosc?
	SaveConfig();
	return 1;

}

CMD:podepnijneon(playerid,params[]){
	if(!IsPlayerAdmin(playerid)) return 0;
	new vid=GetPlayerVehicleID(playerid);
	if (vid==0 || vid==INVALID_VEHICLE_ID) return Msg(playerid,COLOR_ERROR,"Wsiadz do pojazdu");
	new nr,Float:ox,Float:oy,Float:oz,Float:ro[3];
	sscanf(params,"dfffF(0)F(0)F(0)",nr,ox,oy,oz,ro[0],ro[1],ro[2]);
	if(nr>sizeof neony) return Msg(playerid,COLOR_ERROR,"Nie ma takiego neonu!");
	AttachObjectToVehicle(neony[nr][0],vid,ox,oy,oz,ro[0],ro[1],ro[2]);
	AttachObjectToVehicle(neony[nr][1],vid,-ox,oy,oz,ro[0],ro[1],ro[2]);
//	printf("case %d: { ox=%.2f; oy=%.2f; oz=%.2f; }", GetVehicleModel(vid),ox,oy,oz);
	return 1;
}

CMD:raporty(playerid)
{

	if (!IsAdmin(playerid))
		return 0;
	new
	 szList[3000],
	 _players = 0,
	 buffer[127];
	
	for(new i = 0; i < MAX_REPORTS; i++)
	{
		if(!gReports[i][rOccupiedIndex]) continue;

		format(buffer, sizeof buffer, "{%06x}%s{ffffff}(%i) - %s\n", pData[gReports[i][rPlayerId]][currentColor], gReports[i][rPlayerName], gReports[i][rPlayerId], gReports[i][rReason]);
		strcat(szList, buffer);
		
		pTemp[playerid][reportSync][_players++] = i;
	}
	
	if(_players)
	{
		new
		 length = strlen(szList);

		strdel(szList, length - 1, length);
	}

	ShowPlayerDialog(playerid, DIALOG_REPORTS, DIALOG_STYLE_LIST, TXT(playerid, 406), (_players > 0) ? szList : TXT(playerid, 408), TXT(playerid, 76), TXT(playerid, 54));

	return 1;
}

CMD:xd(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3))
		return 0;
	// pola monola + coca cola
	new buf[127];
	if (strlen(params)<=16)
		for(new i=0;i<=strlen(params);i++)
			format(buf,sizeof buf, "%s{%06x}%c", buf, random(256*256*256), params[i]);
	else if (strlen(params)<24)
		for(new i=0;i<=strlen(params)/2;i++)
			format(buf,sizeof buf, "%s{%06x}%c%c", buf, random(256*256*256), params[i*2], params[i*2+1]);
	else
		return SendClientMessage(playerid,-1,"Podany tekst jest zbyt dlugi");
	SendClientMessageToAll(-1,buf);
	return 1;
}

CMD:fakeafk(playerid){
	if (!IsAdmin(playerid,LEVEL_ADMIN3))
		return 0;
	if (pTemp[playerid][fakeAFK]) {
		pTemp[playerid][fakeAFK]=false;
		Msg(playerid,COLOR_INFO,"FAKEAFK wylaczony.");
	} else {
		pTemp[playerid][fakeAFK]=true;
		Msg(playerid,COLOR_INFO,"FAKEAFK wlaczony.");
	}
	return 1;
}

CMD:netstat(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN2))
		return 0;
	new tplayer[MAX_PLAYER_NAME],userid;
	if (sscanf(params,"s[24]",tplayer))
		return Msg(playerid,COLOR_ERROR,"uzyj: {b}/netstat <id/nick>{/b}");
	FindUIDByNameOrID(tplayer,userid,playerid);


	new netstat[1023],Float:ploss,dup;
	format(netstat,sizeof netstat,"Statystki gracza {b}%s (%d){/b}", GetPlayerNick(userid), userid);
	Msg(playerid,COLOR_INFO2,netstat,false);
	format(netstat,sizeof netstat,"IP: %s. PING aktualny: {b}%d{/b}ms, sredni: {b}%d{/b}ms", GetPlayerIP(userid), GetPlayerPing(userid), pData[userid][averagePing]);
	Msg(playerid,COLOR_INFO,netstat,false);


	GetPlayerNetworkStats(userid,netstat,sizeof netstat);

	if (sscanf(netstat,"p<%>'Packetloss: 'f'Duplicate acks received: 'p<\n>d",ploss,dup))
		return Msg(playerid,COLOR_ERROR,"Nie udalo sie pobrac statystyk");



	format(netstat,sizeof netstat,"Utrata pakietow: {b}%.1f%%{/b}, duplikaty: {b}%d{/b}.",ploss,dup);
	Msg(playerid,COLOR_INFO,netstat,false);

	return 1;
}