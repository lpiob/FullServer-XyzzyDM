#include <a_samp>
#tryinclude <a_http>

#include "YSI/y_timers"

#include "fullserver/version"
#include "fullserver/license"
#include "fullserver/fs_header"
#include "fullserver/dialogs"
#include <sscanf2>
#include <streamer>
#include <mysql>
#include <dini>
#include <mapandreas>

//#define USE_OPSP
#if defined USE_OPSP
#include <OPSP>
#endif

#include "fullserver/logging"
#include "fullserver/gangzones"
#include "fullserver/npc"
#include "fullserver/regexp"	// wyrazenia regularne
#include "fullserver/audio"	// zmodyfikowany audio.inc od incognito - konflikt zmiennej name
#include "fullserver/md5"
#include "fullserver/zcmd"
#include "fullserver/atms"
#include "fullserver/utility_functions"
#include "fullserver/audio_functions"

#include "fullserver/money"
#include "fullserver/factions"
#include "fullserver/spawns"
#include "fullserver/areny"
#include "fullserver/objects"
#include "fullserver/domy"

#include "fullserver/scripting_functions"
#include "fullserver/timers"

#include "fullserver/vehicles"
#include "fullserver/solo"
#include "fullserver/gangs"
#include "fullserver/minigames"
#include "fullserver/artefact"

#include "fullserver/commands"
#include "fullserver/textdraws"
#include "fullserver/score"
#include "fullserver/prezenty"
#include "fullserver/attraction_derby"
#include "fullserver/attraction_race"
#include "fullserver/attraction_drifting"
//#include "fullserver/attraction_ctf"
#include "fullserver/attraction_sps"
#include "fullserver/attraction_wg"
#include "fullserver/attraction_labirynt"
#include "fullserver/attraction_chowany"
#include "fullserver/jail"

#include "fullserver/warsztat"
#include "progress"

#include "fullserver/exports"

main()
{
}

public OnGameModeInit()
{
	if (iz7ex6ie<0) OnGameModeInit();
	new
	 startTime = GetTickCount(),
	 buffer[MAX_LOG_STRING_LENGTH];

	SendRconCommand("unbanip *.*.*.*");

	CreateConfigDBIfNotExists();
	LoadConfig();
//	RandomizeWeather();
	LoadDatabaseConfig();
	SetTimer("license_Verify",5000,false);

	Streamer_TickRate(30);
	
	if(!strlen(gmData[DB_hostname]) || !strlen(gmData[DB_username]) || !strlen(gmData[DB_database]))
	{
		printf("Nie udalo sie wczytac konfiguracji bazy danych. Wylaczanie gamemode");
		SendRconCommand("exit");
	}
	printf("Inicjacja i laczenie z baza MySQL...");
	
	hMySQL = mysql_init(LOG_ONLY_ERRORS, 1);
//	hMySQL = mysql_init(LOG_ALL, 1);
	new rConn = mysql_connect(gmData[DB_hostname], gmData[DB_username], gmData[DB_password], gmData[DB_database], hMySQL, 1);
	if(rConn == 0)
	{
		printf("NIEUDANE");
		SendRconCommand("exit");
		return 1;
	}

	printf( "\n\n");
	printf( "d88888b db    db db      db      .d8888. d88888b d8888b. db    db d88888b d8888b. ");
	printf( "88'     88    88 88      88      88'  YP 88'     88  `8D 88    88 88'     88  `8D ");
	printf( "88ooo   88    88 88      88      `8bo.   88ooooo 88oobY' Y8    8P 88ooooo 88oobY' ");
	printf( "88~~~   88    88 88      88        `Y8b. 88~~~~~ 88`8b   `8b  d8' 88~~~~~ 88`8b   ");
	printf( "88      88b  d88 88booo. 88booo. db   8D 88.     88 `88.  `8bd8'  88.     88 `88. ");
	printf( "YP      ~Y8888P' Y88888P Y88888P `8888Y' Y88888P 88   YD    YP    Y88888P 88   YD\n\n");
	printf( "        FullServer XyzzyDM v%s, %s\n\n", GMVERSION, GMCOMPILED);
	format(buffer,sizeof buffer,"   Fullserver XyzzyDM v%s", GMVERSION);

	printf("Uruchamianie FullServer XyzzyDM:");
	printf(" Ladowanie ustawien i elementow glownych ...");

	GetServerVarAsString("bind", gmData[serverIP], sizeof gmData[serverIP]);
	// zabezpieczenie przeciwko odpalaniu na innym adresie ip
	if (strcmp(gmData[serverIP],GMHOST,false)!=0)
		SendRconCommand("exit");

	SetGameModeText("�� XyzzyDM ��");
	SendRconCommand("mapname �� Full Andreas ��");

	UsePlayerPedAnims();
	AllowAdminTeleport(1);
	DisableInteriorEnterExits();
	EnableStuntBonusForAll(false);
	ShowPlayerMarkers(PLAYER_MARKERS_MODE_GLOBAL);
	
	gmTemp[lastHour] = -1;
	gmTemp[lastDay] = -1;
	gmData[artefactOwner] = INVALID_PLAYER_ID;
	
	LoadCensoredWords();
	
	
	printf(" Ladowanie elementow wizualnych ...");
	
	CreateTextDraws();
	
	printf(" Ladowanie jezykow ...");
	LoadLanguages();

	printf(" Ladowanie klas graczy ...");
	
	AddPlayerClass(292, -2876.87, 2807.71, 252.25, 45.0, 0, 0, 0, 0, 0, 0);	// gracze zarejestrowani zobacza zamiast niego swoj wlasny skin

	for(new skinid = 0; skinid <= 299; skinid++)	// bylo 288
		switch(skinid)
		{
			case 1..6, 8, 42, 65, 74, 86, 119, 149, 208, 239, 265..273, 289:
				continue;
			default:
				if (!SkinKobiecy(skinid))
				AddPlayerClass(skinid, -2876.87, 2807.71, 252.25, 45.0, 0, 0, 0, 0, 0, 0);
		}

	for(new skinid = 300; skinid >0; skinid--)	// bylo 288
		if (SkinKobiecy(skinid))
			AddPlayerClass(skinid, -2876.87, 2807.71, 252.25, 45.0, 0, 0, 0, 0, 0, 0);
	
	printf(" Ladowanie pojazdow ...");
	
    for(new i = 0; i < sizeof DATA_vehicles; i++)
	{
		if (random(3)==1) continue;	// co 3ci pojazd nie jest dodawany
		switch(DATA_vehicles[i][model])
		{
			case 407, 416, 427, 448, 490, 497, 599, 598, 597, 596, 523, 420, 438, 528, 442, 409:
			{
				new v=AddStaticVehicle(DATA_vehicles[i][model], DATA_vehicles[i][X], DATA_vehicles[i][Y], DATA_vehicles[i][Z], DATA_vehicles[i][angle], 0, 0);
				tVehicles[v][vo_color][0]=0;
				tVehicles[v][vo_color][1]=0;
				tVehicles[v][vo_static]=true;

			}
			default:
			{
				new color1=random(127);
				new color2=random(127);

				new v=AddStaticVehicle(DATA_vehicles[i][model], DATA_vehicles[i][X], DATA_vehicles[i][Y], DATA_vehicles[i][Z], DATA_vehicles[i][angle], color1, color2);
				tVehicles[v][vo_static]=true;
				tVehicles[v][vo_color][0]=color1;
				tVehicles[v][vo_color][1]=color2;
				SetVehicleHealth(v, VEHICLE_DEFAULT_HP);
			}
		}
		staticVehicleCount++;
	}
	new File:olist = fopen("pojazdy.txt", io_read);
	new cnt=0, lnum=0, line[255],v=0;
	while(fread(olist, line)) { 
		lnum++;
		if (line[0]!='/' && strfind(line,"AddStaticVehicle",true)!=-1) {
			new modelid, Float:vX,Float:vY,Float:vZ,	Float:vA, color1, color2;
			if (sscanf(line,"p<,>'('iffffdp<)>d",modelid,vX,vY,vZ,vA,color1,color2)) {
				continue;
			}
			if (color1<0)
				color1=random(127);
			if (color2<0)
				color2=random(127);

			if (modelid<400 || (vX==0 && vY==0 && vZ==0)) continue;
			v=AddStaticVehicle(modelid,vX,vY,vZ,vA,color1,color2);
			tVehicles[v][vo_static]=true;
			tVehicles[v][vo_color][0]=color1;
			tVehicles[v][vo_color][1]=color2;
			SetVehicleHealth(v, VEHICLE_DEFAULT_HP);
			cnt++;
		}
	}
	fclose(olist);
	staticVehicleCount+=cnt;
	if (v>staticVehicleCount) staticVehicleCount=v;

	printf(" Ladowanie obiektow ...");
	
	CreateObjects();
	CreateATMs();
	
	CreateObject(1433, 371.3275, -1797.0743, 23.5606, 0.0, 0.0, 180.0, 300.0);

	minigames_Init();
	
	printf(" Pobieranie danych gangow ...");
	gangs_LoadGangData();
	


	printf(" Pobieranie danych domow");
	domy_Reload();

	printf(" Ladowanie pickup-ow, checkpointow, teleporting pickups, returning pickups, i co tam jeszcze XJL wymyslil ...");

	CreatePickups();
	CreateArtefact();//1982.1211, 1587.8715, 22.7734);
	obiekty_odswiezTeleCheckpoints();
	obiekty_odswiezReturnPickups();
	obiekty_odswiezTelePickups();
	obiekty_odswiezMapIcons();
	obiekty_odswiezMiscPickups();

	gmTemp[snoopPM]=1;
	gmTemp[showJoins]=2;

	printf(" Inicjalizowanie GangZones:");
	GZ_Init();
	
	printf(" Wybor paczki dzwiekowej...");
	Audio_SetPack("fullserver", true);
	
	printf("Inicjalizowanie MapAndreas...");
	MapAndreas_Init(MAP_ANDREAS_MODE_FULL);

	printf(" Aktualizacja danych i przygotowywanie do uruchomienia ...");
	
//	printf("WELCOMETEXT: %s", GetServerConfig("welcome_text"));
//	TextDrawSetString(gTextDraw[TD_WELCOMETEXT], GetServerConfig("welcome_text"));

	// laczymy NPC
//	if (gmNPC[gmnt_fullserver]==INVALID_PLAYER_ID || !IsPlayerNPC(gmNPC[gmnt_fullserver]))
//		ConnectNPC("FullServer","hydra");

	telpos[tpX]=FLOAT_NAN;
	evtp[tpX]=FLOAT_NAN;
	printf(" Pobieranie nazw pojazdow z bazy danych");
	UpdateVehicleNames();

#if defined A_WG_WPNSEL
	aWGWeaponMenu=CreateMenu("Wybierz bron",1,20.0,150.0,180.0,110.0);
	for (new i=0;i<sizeof aWGWeaponMenuWPN;i++)
		AddMenuItem(aWGWeaponMenu, 0, aWGWeaponMenuWPN[i][ws_combinationName]);
#endif
	printf("Odswiezanie konfiguracji");
	RefreshConfiguration();

	printf("Uruchamianie petli glownej");
	SetTimer("UpdateTimer", UPDATE_TIMER_TIME, true);

	TimeSync();

	printf("FullServer DM zaladowany pomyslnie (%0.3f sekund)\r\n", float(GetTickCount() - startTime) / 1000);

	return 1;
}

public OnGameModeExit()
{
	printf("Wylaczanie gamemode. Zapisywanie danych graczy");
	foreach(playerid)
		if(pData[playerid][loggedIn])
			UpdatePlayerAccountData(playerid,true);
	gangs_saveGangData(true);
	printf("Dane graczy zapisane");
	mysql_close(hMySQL);

	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	if (pData[playerid][logonDialog]) return 0;
	SyncPlayerGameTime(playerid);

	ShowElement(playerid, TDE_DATETIME, false);
	ShowElement(playerid, TDE_STATS, false);
	ShowElement(playerid, TDE_ATTRACTIONBOX, false);
	ShowElement(playerid, TDE_WYBIERALKA, true);

	if (pTemp[playerid][firstClassSelection]) {
		if (pData[playerid][lastUsedSkin]>0 || pData[playerid][accountID]>0)
			Msg(playerid,COLOR_INFO2,"Wcisnij {b}shift lub enter{/b}, aby wybrac swoj ostatni skin, lub wybierz nowy.", false);
		else 
			Msg(playerid,COLOR_INFO,"Wybierz skin, ktorego chcesz uzywac w grze. ");
		Msg(playerid,COLOR_INFO2,"Przewijaj w lewo aby przegladac skiny kobiece, w prawo aby przegladac skiny meskie.", false);
		Msg(playerid,COLOR_INFO,"Jesli znasz ID skina, mozesz wpisac {b}/skin id{/b}, aby wybrac go od razu", false);
		pTemp[playerid][firstClassSelection]=false;
	}
	if (classid==0) {
		if (pData[playerid][lastUsedSkin]>0)
			SetPlayerSkin(playerid, pData[playerid][lastUsedSkin]);
		GameTextForPlayer(playerid,"~n~~n~~n~~n~~n~~n~~n~~n~<< kobiety  |  mezczyzni >>",2500,5);
	} else {
		new buf[44];
		format(buf,sizeof buf,"~n~~n~~n~~n~~n~~n~~n~~n~Skin ~y~%d", GetPlayerSkin(playerid));
		GameTextForPlayer(playerid,buf,3000,5);
	}

	FlashScreen(playerid);

	pData[playerid][citySelecting] = false;
	pData[playerid][classSelecting] = true;

/*	SetPlayerPos(playerid, 371.3095, -1797.1166, 24.9386);	// plaza
	SetPlayerFacingAngle(playerid, 180.0);
	SetPlayerInterior(playerid, 0);
	
	SetPlayerDrunkLevel(playerid, 2);
	
	new
		 Float:pvector[3] = {371.3275, -1800.0743, 25.5306};
	
	pvector[0] += 7.0 * floatsin(float(random(60) - 30), degrees);
	pvector[1] -= float(random(150)) / 150.0;
	pvector[2] += 4.0 * floatsin(float(random(30) - 15), degrees);
	
	SetPlayerCameraPos(playerid, pvector[0], pvector[1], pvector[2]);
	SetPlayerCameraLookAt(playerid, 371.3275, -1797.0743, 25.2306);*/

/*
	// tama
	SetPlayerPos(playerid,-755.29,2037.16,77.89);
	SetPlayerFacingAngle(playerid,198.18);
	SetPlayerCameraPos(playerid,-754,2033,79);
	SetPlayerCameraLookAt(playerid,-762.6,2047,77);
	SetPlayerDrunkLevel(playerid, 2);
	SetPlayerInterior(playerid,0);*/

	// most sf-lv
	SetPlayerPos(playerid,-1531.59,687.47,71.91);
	SetPlayerFacingAngle(playerid,131.7);
	SetPlayerCameraPos(playerid,-1533.6,685.1,72.55);
	SetPlayerCameraLookAt(playerid,-1532.09,687.47,72.21);
	SetPlayerDrunkLevel(playerid, 2);
	SetPlayerInterior(playerid,0);

	switch(random(8))
	{
		case 0: ApplyAnimation(playerid,"DANCING", "DAN_Down_A", 4.000000, 1, 1, 1, 1, 1); // Taichi
		case 1: ApplyAnimation(playerid, "DANCING", "DAN_Left_A", 4.000000, 1, 1, 1, 1, 1); // Dilujesz
		case 2: ApplyAnimation(playerid, "DANCING", "DAN_Right_A", 4.000000, 1, 1, 1, 1, 1); // R�ce
		case 3: ApplyAnimation( playerid,"DANCING", "DAN_Up_A", 4.000000, 1, 1, 1, 1, 1); // Fuck
		case 4: ApplyAnimation(playerid, "DANCING", "dnce_M_a", 4.000000, 1, 1, 1, 1, 1); // Lookout
		case 5: ApplyAnimation(playerid, "RAPPING", "Laugh_01", 4.0, 1, 0, 0, 0, 0); // Laugh
		case 6: ApplyAnimation(playerid, "RAPPING", "RAP_B_Loop", 4.0,1,0,0,0,0); // Rapujesz
		case 7: ApplyAnimation(playerid, "DANCING", "DAN_Right_A", 4.000000, 1, 1, 1, 1, 1); //Taniec
	}

	
	PlaySound(playerid, 1132);
	
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	if(IsPlayerNPC(playerid)) return 1;
	if(pData[playerid][logonDialog])
	{
		return 0;
	}
	
	pData[playerid][citySelecting] = true;
	pData[playerid][classSelecting] = false;
	
	SetPlayerProperColor(playerid);
	TogglePlayerSpectating(playerid, true);
	SetPlayerDrunkLevel(playerid, 0);
	
//	UpdateCityPlayers(CITY_LV);
	
	pData[playerid][lastUsedSkin] = GetPlayerSkin(playerid);
//	SetPlayerINIFileValue(playerid, "last_skin", pData[playerid][lastUsedSkin]);
	
//	if(pData[playerid][accountID]  <1)
//	{
//		SetTimerEx("InitPlayerLanguageSelecting", 200, false, "i", playerid);
//	}
//	else
//	{
//		ShowElement(playerid, TDE_HINT_CITYSELECT, true);
	ShowElement(playerid, TDE_WELCOMEBOX, true);
	ShowElement(playerid, TDE_WYBIERALKA, false);
//	printf("RequiestSpawn welcome iniplayer");
	SetTimerEx("InitPlayerCitySelecting", 100, false, "i", playerid);
		
//	}
	
	return 1;
}

public OnPlayerConnect(playerid)
{
	if (playerid>MAX_SERVER_PLAYERS) {
		SendClientMessage(playerid,-1,"Limit graczy osiagniety");
		Kick(playerid);
		return 1;
	}
	if (IsPlayerNPC(playerid)){
			Kick(playerid);
/*
			new pip[16];
			GetPlayerIp(playerid,pip,sizeof pip);
			if(strcmp(pip, "127.0.0.1") && strcmp(pip, gmData[serverIP])) {
				Kick(playerid);
				return 1;
			}
			pData[playerid][citySelecting] = false;
			pData[playerid][classSelecting] = false;
			gmNPC[gmnt_fullserver]=playerid;
			SetupNPC(gmnt_fullserver);
*/
			return 1;
	}

	if (gmTemp[highestPlayerID]<playerid)
		gmTemp[highestPlayerID]=playerid;
	
	SendClientMessage(playerid,-1," ");
	SendClientMessage(playerid,0xfaf439ff,"Wersja: " #GMVERSION " " #GMCOMPILED);
	new
	 buffer[160];
	
	format(buffer, sizeof buffer, "SELECT TIMESTAMPDIFF(SECOND, NOW(), date_end) FROM %s \
		WHERE '%s' LIKE ip \
		AND date_end > NOW() \
		AND date_created < NOW() \
		LIMIT 1", 
	gmData[DB_ipbans], GetPlayerIP(playerid));
	
	if (mysql_query(buffer))
		mysql_ping();
	else {
		mysql_store_result();
	
		if(mysql_num_rows())
		{
			new
			 banExpireTime,
			 tempPeriod;

			mysql_fetch_row(buffer);
			banExpireTime = StringToInt(buffer);

			GetOptimalTimeUnit(banExpireTime, tempPeriod); 

			format(buffer, sizeof buffer, TXT(playerid, 196), banExpireTime, GetPeriodName(playerid, tempPeriod, banExpireTime));
			Msg(playerid, 0x900A00FF, buffer); // Tw�j adres IP jest zbanowany na xxx minut/godzin.

			Msg(playerid, COLOR_INFO2, TXT(playerid, 197)); // Je�eli uwa�asz, �e to pomy�ka, zg�o� to do administratora lub napisz pro�b� o odbanowanie na FullServer.eu
			KickPlayer(playerid);
			if (mysql_result_stored()) mysql_free_result();
			return 1;
		}
		if (mysql_result_stored()) mysql_free_result();
	}
	pData[playerid][accountID]=0;
	pData[playerid][pmoney]=50000;		// domyslna kasa dla nowych graczy
	pData[playerid][selectedSpawn] = 2;	// Full Andreas
	pData[playerid][citySelecting] = false;
	pData[playerid][loggedIn] = false;
	pTemp[playerid][loginAttemps] = 0;
	pData[playerid][vipEnabled] = false;
	pData[playerid][vipDaysLeft]=-1;
	format(pData[playerid][vipToDate], 12, "0000-00-00");
	pData[playerid][session] = GetTickCount();
	pTemp[playerid][lastSessionSaveTick] = GetTickCount();
	pTemp[playerid][firstCitySelect] = true;
	pData[playerid][gang] = NO_GANG;
	pData[playerid][gangRank] = GANG_RANK_NONE;
	pData[playerid][allowedLevel]=0;
	pData[playerid][adminLevel]=0;
	pData[playerid][spectating] = INVALID_PLAYER_ID;
	pTemp[playerid][firstSelectingScreen] = true;
	pData[playerid][destroyMyVehicle] = false;
	pData[playerid][currentColor] = gmData[color_normalUser];
	pTemp[playerid][shipIdle] = 0;	// todo usunac
	pTemp[playerid][lastMsgTick] = 0;
	pTemp[playerid][spamCount] = 0;
	pData[playerid][kills] = 0;
	pData[playerid][teamkills] = 0;
	pData[playerid][deaths] = 0;
	pData[playerid][suicides] = 0;
	pData[playerid][averagePing] = 0;
	pTemp[playerid][pingChecks] = 0;
	pTemp[playerid][pingSum] = 0;
	pTemp[playerid][pingWarningCount] = 0;
//	pTemp[playerid][lastRespect] = -1;
	pData[playerid][respect]=0;
	pData[playerid][skill] = 0;
	pTemp[playerid][specPosReturn] = false;
	pData[playerid][pAttraction] = A_NONE;
	pData[playerid][aChowany] = false;
	pData[playerid][aSPS] = false;
	pData[playerid][aDerby] = false;
	pData[playerid][aLabirynt] = false;
	pData[playerid][aRace] = false;
	pData[playerid][aDrift] = false;
	pData[playerid][aWG] = false;
	pData[playerid][aStrzelnica] = false;
	pData[playerid][statsShowed] = false;
	pData[playerid][lastUsedSkin] = 0;
	pTemp[playerid][performingAnim] = false;
	pTemp[playerid][hasVoted]=false;
	pTemp[playerid][warningReceived]=0;
	pTemp[playerid][firstClassSelection]=true;
	pTemp[playerid][onArena]=ARENA_NONE;
	pTemp[playerid][pickupDelay]=0;
	pTemp[playerid][bonusHours] = 0;
	pTemp[playerid][miniGame]=MINIGAME_NONE;
	pTemp[playerid][lastPlayerKilled]=INVALID_PLAYER_ID;
	pTemp[playerid][killStreak]=0;
	pTemp[playerid][e_houseid]=-1;
	pTemp[playerid][drunkLevel]=0;
	pTemp[playerid][drunkLevelL]=0;
	pTemp[playerid][playerColorAlpha]=PLAYER_COLOR_ALPHA;
	pTemp[playerid][tmpAlpha]=PLAYER_COLOR_ALPHA;
//	pTemp[playerid][dirtyHack2Reconnect]=false;
	pTemp[playerid][TPInv]=0;
	pTemp[playerid][disableWeaponCheck]=false;

	pTemp[playerid][wStrefieNODM]=false;
	pTemp[playerid][wStrefieFULLDM]=false;

	pTemp[playerid][curPos]=1;
	pTemp[playerid][lastPos]=-1;

	pTemp[playerid][cenzura]=false;
	pTemp[playerid][protkill]=false;
	pTemp[playerid][protping]=false;
	pTemp[playerid][troll]=false;
	pTemp[playerid][lockedPos]=false;
	pTemp[playerid][weaponsAllowed]=true;

	pTemp[playerid][weaponSkill_pistol]=0;
	pTemp[playerid][weaponSkill_silenced]=0;
	pTemp[playerid][weaponSkill_sawnoff]=0;
	pTemp[playerid][weaponSkill_uzi]=0;
	pTemp[playerid][audio_vehicle]=-1;
	pTemp[playerid][vehicleSpecialLastUsed]=GetTickCount();

	pTemp[playerid][bannedPlayersCnt]=0;
	pTemp[playerid][bannedPlayersRS]=0;
	pTemp[playerid][faction]=FACTION_NONE;
	pTemp[playerid][FactionName]=CreateDynamic3DTextLabel(" ", 0xffffffff, 0, 0, 1, 30, playerid, INVALID_VEHICLE_ID, 1, 0); 
	pTemp[playerid][fakeAFK]=false;

	for(new i=0;i<ARENA_MAX;i++)
		pTemp[playerid][arenaScore][i]=0;

	privpos[playerid][tpX]=FLOAT_NAN;
	privpos2[playerid][tpX]=FLOAT_NAN;

	
	SetPlayerColor(playerid,0x606060FF);
	if (!LoadBasicPlayerData(playerid)) return 0;
//	if (pData[playerid][accountID]==0)	// nie znaleziono gracza
//		if (!LoadBasicPlayerData(playerid,true)) return 0;	

	
	
	if(IsPlayerAdmin(playerid) && pData[playerid][allowedLevel]<LEVEL_ADMIN3)	// zalogowany na rcona bez rcona w bazie danych!
	{
		format(buffer,sizeof buffer,"Nieautoryzowane logowanie na admina RCON przez %s (%d)! Wykopany.", GetPlayerNick(playerid), playerid);
		KickPlayer(playerid,false);
		MSGToAdmins(COLOR_ERROR, buffer, false);

		OutputLog(LOG_SYSTEM, buffer);
		return 0;

	}
	else if (IsPlayerAdmin(playerid) && pData[playerid][allowedLevel]>=LEVEL_ADMIN3)
		pTemp[playerid][isRcon] = true;
	
	
	ShowElement(playerid, TDE_WIDE, true);
	ShowElement(playerid, TDE_FULLSERVERLOGO, true);

	
	
	for(new i = TD_STARS_START; i <= TD_STARS_END; i++)
	{
		TextDrawHideForPlayer(playerid, gTextDraw[i]);
	}

//	if (gmTemp[pPlayerCount]<75)	gmTemp[pPlayerCount]

	if (gmTemp[showJoins]==2 || (gmTemp[showJoins]==1 && gmTemp[pAbsAdminCount]>0))
	foreach(i)
	{
		if(playerid == i) continue;
		
		new
		 szPlayerName[24];
		
		GetPlayerName(playerid, szPlayerName, sizeof szPlayerName);
		
		if(IsAdmin(i))
		{
			format(buffer, sizeof buffer, SkinKobiecy(pData[playerid][lastUsedSkin]) ? TXT(i, 69) : TXT(i, 68), szPlayerName, playerid, GetPlayerIP(playerid));
			Msg(i, COLOR_JOININFO, buffer, ((gmTemp[pPlayerCount]<5 && pData[playerid][allowedLevel]<=0) ? true : false) );
		}
		else if (gmTemp[showJoins]==2)
		{
			format(buffer, sizeof buffer, (SkinKobiecy(pData[playerid][lastUsedSkin])) ? TXT(i, 67) : TXT(i, 66), szPlayerName, playerid);
			Msg(i, COLOR_JOININFO, buffer, ((gmTemp[pPlayerCount]<5 && pData[playerid][allowedLevel]<=0) ? true : false) );
		}
		
		
	}
	
	new
	 pOnline = GetPlayerCount();
	
	if(pOnline > StringToInt(GetServerStat("most_online")))
	{
		SetServerStatInt("most_online", pOnline);
		SetServerStatString("most_online_date", "NOW()", true);
		
		format(buffer, sizeof buffer, "Nowy rekord graczy na serwerze!~n~~n~~r~~h~%i!", pOnline);
		ShowAnnouncement(buffer);
	}
	
	SetServerStatString("join_count", "value + 1", true);
	
	if(PlayerAccountExists(playerid))
	{
		pData[playerid][logonDialog] = true;
		ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_INPUT, TXT(playerid, 51), TXT(playerid, 52), TXT(playerid, 53), TXT(playerid, 54));
		pData[playerid][classSelecting] = false;
	}
	else
	{
		pData[playerid][logonDialog] = false;
		pData[playerid][classSelecting] = true;
	}
	
	
	new
	 szPlayerName[24],
	 str[160];
	
	GetPlayerName(playerid, szPlayerName, sizeof szPlayerName);

	format(str,sizeof str," ~r~%s ~w~(~l~%d~w~) ",szPlayerName,playerid);
	TextDrawSetString(pTextDraw[PTD_STAT][playerid], str);
	
	// TODO: informacje

//	if (!Audio_IsClientConnected(playerid))	// audio klient po polaczeniu odtwarza dzwiek intro
//		PlayRandomMusic(playerid);			// COTOKURWA JEST ZA WIEJSKA MELODYJKA PRZY POLACZENIU! ;]
	PlaySound(playerid, 1185);	// standardowa muza fs

#if defined GAMETIMESYNCTOREAL
	TogglePlayerClock(playerid,0);
#else
	TogglePlayerClock(playerid,1);
#endif
	SyncPlayerGameTime(playerid);

	pTemp[playerid][p3d_status] = CreateDynamic3DTextLabel("", COLOR_3DTEXT_HITMAN, 0.0, 0.0, 0.7, 50.0, playerid, INVALID_VEHICLE_ID, 1);
//	Attach3DTextLabelToPlayer(pTemp[playerid][p3d_status], playerid, 0.0, 0.0, 0.5);

	pTemp[playerid][accepts][eac_solo]=ACCEPTS_ALL;
	pTemp[playerid][accepts][eac_pm]=ACCEPTS_ALL;

	if (gmTemp[protAll])
		Msg(playerid, COLOR_ERROR,"Uwaga! Obecnie wszyscy gracze na serwerze posiadaja imunitet!");

	UpdatePlayerNickTD(playerid);

	return 1;
}

OnPlayerLogin(playerid)
{
	new
	 buffer[160];

	pData[playerid][accountID] = GetAccountID(GetPlayerNick(playerid));
	
	format(buffer, sizeof buffer, "SELECT TIMESTAMPDIFF(SECOND, NOW(), date_end) pozostalo,reason FROM %s \
		WHERE player_banned = %i \
		AND date_end > NOW() \
		AND date_created < NOW() \
		LIMIT 1", 
	gmData[DB_bans], pData[playerid][accountID]);
	
	mysql_query(buffer);
	mysql_store_result();
	
	if(mysql_num_rows())
	{
		new
		 banExpireTime,
		 tempPeriod,
		 banReason[100];
		
		mysql_fetch_row(buffer,"|");// aqq
		sscanf(buffer,"p<|>dS(brak powodu)[99]", banExpireTime, banReason);
//		banExpireTime = StringToInt(buffer);

		GetOptimalTimeUnit(banExpireTime, tempPeriod); 
		format(buffer, sizeof buffer, TXT(playerid, 20), banExpireTime, GetPeriodName(playerid, tempPeriod, banExpireTime));
		Msg(playerid, COLOR_INFO, buffer); // Twoje konto jest zbanowane na xxx minut/godzin.
		format(buffer, sizeof buffer, "Powod: {b}%s{/b}", banReason);
		Msg(playerid, COLOR_INFO, buffer); // Twoje konto jest zbanowane na xxx minut/godzin.
		
		Msg(playerid, COLOR_INFO2, "Jesli uwazasz ze ban byl niesluszny, badz tez pragniesz byc odbanowany przed uplywem podanego czasu");
		Msg(playerid, COLOR_INFO2, "zloz {b}podanie o odbanowanie{/b} na stronie {b}WWW.FULLSERVER.EU{/b}.");

		format(buffer, sizeof buffer, "Gracz {b}%s{/b} nie dolaczyl z powodu aktywnego bana {b}%s:+%d %s", GetPlayerNick(playerid), banReason, banExpireTime, GetPeriodName(playerid, tempPeriod, banExpireTime));
		MSGToAdmins(COLOR_INFO2, buffer, false, LEVEL_ADMIN1);

		KickPlayer(playerid);
		if (mysql_result_stored()) mysql_free_result();
		return 0;
	}
	
	if (mysql_result_stored()) mysql_free_result();

	FetchPlayerAccountData(playerid);

	avt_getPlayerAchievements(playerid);	// pobiera wszystkie osiagniecia gracza i jego rankingi

	if(IsPlayerAdmin(playerid) && pData[playerid][adminLevel]<LEVEL_ADMIN3)	// logowanie na rcona bez rcona w bazie danych!
	{
		format(buffer,sizeof buffer,"Nieautoryzowane logowanie na admina RCON przez %s (%d)! Wykopany.", GetPlayerNick(playerid), playerid);
		KickPlayer(playerid,false);
		foreach(i)
			if(IsAdmin(i, LEVEL_ADMIN1))
				Msg(i, COLOR_ERROR, buffer);
		OutputLog(LOG_SYSTEM, buffer);
		return 0;
	}	
	else if(IsPlayerAdmin(playerid) && pData[playerid][allowedLevel] == LEVEL_ADMIN3)
	{
		pData[playerid][adminLevel] = LEVEL_ADMIN3;
	}
	
	SetPlayerScore(playerid, pData[playerid][respect]);
	
	LogConnection(playerid);	
	
	SetPlayerProperColor(playerid);

	
	pData[playerid][loggedIn] = true;
	pData[playerid][logonDialog] = false;
	
/*	if(pData[playerid][allowedLevel] == LEVEL_GM && pData[playerid][adminLevel] != LEVEL_ADMINHIDDEN)		// automatyczne logowanie na admina
	{
		new
		 szPlayerName[24];
		
		GetPlayerName(playerid, szPlayerName, sizeof szPlayerName);
		
		foreach(i)
		{
			if(pData[playerid][adminLevel] == LEVEL_GM)
			{
				format(buffer, sizeof buffer, TXT(i, 241), szPlayerName);
			}
			else
			{
				format(buffer, sizeof buffer, TXT(i, 147), szPlayerName, pData[playerid][adminLevel] - 1);
			}
			
			Msg(i, COLOR_INFO2, buffer, false); // "xxx" zalogowa� si� na admina/moderatora (poziom xxx).
			PlaySound(i, 1133);
		}
	}*/
	
	new
	 szName[24];
		
	GetPlayerName(playerid, szName, sizeof szName);
	
/*	if(pData[playerid][gang] != NO_GANG)
	{
		if(strcmp(GetPlayerTag(playerid), gData[pData[playerid][gang]][tag], true) != 0 && GangExists(GetPlayerTag(playerid)))
		{
			format(buffer, sizeof buffer, TXT(playerid, 209), GetPlayerTag(playerid), gData[pData[playerid][gang]][tag]);
			Msg(playerid, COLOR_ERROR, buffer); // Nie nale�ysz do gangu "xxx", tylko do "xxx". Zmie� sw�j tag i wr�� na serwer.
//			KickPlayer(playerid);
			
			return 0;
		}
		
		new
		 szTagPlayerName[30];
		
		format(szTagPlayerName, sizeof szTagPlayerName, "[%s]%s", gData[pData[playerid][gang]][tag], GetPlayerProperName(playerid));

		if(strfind(szName, gData[pData[playerid][gang]][tag],false)==-1)
//		if(strcmp(szName, szTagPlayerName, true, strlen(gData[pData[playerid][gang]][tag])) != 0)
		{
//			if((strlen(szName) + strlen(gData[pData[playerid][gang]][tag]) + 2) > 24)
//			{
//				Msg(playerid, COLOR_ERROR, TXT(playerid, 207));  // Tw�j nick jest zbyt d�ugi aby dopisa� do niego tag Twojego gangu. Prosimy zg�osi� ten problem do administratora.
//				KickPlayer(playerid, true);
//				
//				return 0;
//			}
			
//			format(buffer, sizeof buffer, "[%s]%s", gData[pData[playerid][gang]][tag], szName);
//			SetPlayerName(playerid, buffer);
//			UpdatePlayerStatsTD(playerid);
						
//			format(buffer, sizeof buffer, TXT(playerid, 206), gData[pData[playerid][gang]][tag], szName);
//			Msg(playerid, COLOR_INFO2, buffer); // Tw�j nick zosta� automatycznie poprawiony na "[xxx]xxx".
			format(buffer, sizeof buffer, "Nalezysz do gangu {b}%s{/b}, czym predzej dolacz tag {b}%s{/b} do swojego nicku!", gData[pData[playerid][gang]][name], gData[pData[playerid][gang]][tag]);
			Msg(playerid, COLOR_INFO2, buffer);
		}
	}
	else
	{
		if(GangExists(GetPlayerTag(playerid)))
		{
			format(buffer, sizeof buffer, "Nie nalezysz do gangu {b}%s{/b}, usun ich tag z nicku, albo zalatw dopisanie do listy czlonkow u lidera gangu.", GetPlayerTag(playerid));
			Msg(playerid, COLOR_ERROR, buffer); // Nie nale�ysz do gangu "%s", usu� tag z nicku i wr�� na serwer.
			Msg(playerid, COLOR_ERROR, "Ignorujac te ostrzezenie narazasz sie na zbanowanie konta!");
			format(buffer, sizeof buffer, "Gracz %s(%d) posiada w nicku tag gangu %s, chociaz do niego nie nalezy", GetPlayerNick(playerid), playerid, GetPlayerTag(playerid));
			MSGToAdmins(COLOR_INFO, buffer, false, LEVEL_ADMIN1);
		}
	}*/
	
	if(pData[playerid][hitman] > 0)
	{
//		format(buffer, sizeof buffer, "%i{006600}$", pData[playerid][hitman]);
//		pTemp[playerid][hitman3DTextLabel] = CreateDynamic3DTextLabel(buffer, COLOR_3DTEXT_HITMAN, 0.0, 0.0, 1.2, 40.0, playerid);
		IncreasePlayerHitman(playerid, 0);
	}
	// wczytujemy informacje o domu
	if (pData[playerid][accountID]>0)
		pTemp[playerid][e_houseid]=domy_findHouseByOwnerID(pData[playerid][accountID]);
	else
		pTemp[playerid][e_houseid]=-1;

	if (pTemp[playerid][e_houseid]>=0)
		domy_OnHouseOwnerLogin(playerid);

	if (pData[playerid][gang]!=NO_GANG)
		pData[playerid][selectedSpawn]=1;
	else if (pTemp[playerid][e_houseid]>=0)
		pData[playerid][selectedSpawn]=0;


	if (pData[playerid][vipEnabled]) {
		if (gmTemp[pPlayerCount]<150) {
			format(buffer, sizeof buffer, "({FACB00}VIP{A0A0A0}) Do gry dolacza {FACB00}%s{A0A0A0}.", GetPlayerProperName(playerid));
			SendClientMessageToAll(0xA0A0A0, buffer);
		}
		cmd_vpozostalo(playerid);
	}

	if (pData[playerid][gang]!=NO_GANG)
		gangs_OnPlayerLogin(playerid);

//	SendDeathMessage(playerid, INVALID_PLAYER_ID, 200);
	if(pData[playerid][accountID]==55) {
		format(buffer,sizeof buffer,"*** %s zalogowal sie jako maper.", GetPlayerNick(playerid));
		SendClientMessageToAll(0x636dfb, buffer);
		foreach(i)
			PlaySound(i, 1133);
	}
	// aqq

	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	for (new i=gmTemp[highestPlayerID]; i>=0; i--)
		if ((IsPlayerConnected(i) && i!=playerid) || i==0) {
			gmTemp[highestPlayerID]=i; break;
		}
	DestroyDynamic3DTextLabel(pTemp[playerid][FactionName]);
	DestroyDynamic3DTextLabel(pTemp[playerid][p3d_status]);
	pData[playerid][pAttraction] = A_NONE;
	pData[playerid][aChowany] = false;
	pData[playerid][aSPS] = false;
	pData[playerid][aDerby] = false;
	pData[playerid][aLabirynt] = false;
	pData[playerid][aRace] = false;
	pData[playerid][aDrift] = false;
	pData[playerid][aWG] = false;
	pData[playerid][aStrzelnica] = false;



	if (gmTemp[aRacePlayersR][playerid]!=INVALID_PLAYER_ID) {
		gmTemp[aRacePlayers][gmTemp[aRacePlayersR][playerid]]=INVALID_PLAYER_ID;
		gmTemp[aRacePlayersR][playerid] = INVALID_PLAYER_ID;
	}

	if (aData[A_WG][aState] != A_STATE_OFF) 
		for(new i = 0; i < gmTemp[aWGMaxPlayers]; i++)
    	    if (gmTemp[aWGPlayers][i]==playerid) {
				gmTemp[aWGPlayers][i]=INVALID_PLAYER_ID;
				break;
			}

	if (aData[A_CHOWANY][aState] != A_STATE_OFF)
		for(new i = 0; i < gmTemp[aChowanyMaxPlayers]; i++)
    	    if (gmTemp[aChowanyPlayers][i]==playerid) {
				gmTemp[aChowanyPlayers][i]=INVALID_PLAYER_ID;
				break;
			}

	pTemp[playerid][isRcon] = false;

	avt_zeroPlayerAchievements(playerid);	// zerujemy zapisy o osiagnieciach, na wypadek gdyby na ten id wrocil ktos niezarejestrowany (i te nie zostana pobrane).
	if(pData[playerid][reported]) RemovePlayerFromReportList(playerid);

	if(pTemp[playerid][e_houseid]>=0)
		domy_OnHouseOwnerDisconnects(playerid);

	for(new i = 0; i < gmTemp[aStrzelnicaMaxPlayers]; i++)
	{
		if(playerid != gmTemp[aStrzelnicaPlayers][i]) continue;
		
		gmTemp[aStrzelnicaPlayers][i] = INVALID_PLAYER_ID;
	}

	SavePlayerData(playerid);

	if(gmData[artefactOwner] == playerid)
		DropArtefact(playerid);

/*	else if(GetPlayerCount() < 2)
	{
		if(gmData[artefactOwner] != INVALID_PLAYER_ID)
			DropArtefact(gmData[artefactOwner]);
	}*/

//	if (IsValidDynamic3DTextLabel(pTemp[playerid][hitman3DTextLabel]))
//		DestroyDynamic3DTextLabel(pTemp[playerid][hitman3DTextLabel]);

/*	if(pTemp[playerid][dirtyHack2Reconnect]){
		new string[80],IP[16];
        GetPlayerIp(playerid,IP,sizeof(IP));
		format(string,sizeof(string),"unbanip %s",IP);
		SendRconCommand(string);
		format(string,sizeof(string),"%s (%d) %s", GetPlayerNick(playerid), playerid, IP);
		OutputLog(LOG_SYSTEM, "Automatyczne odbanowywanie dla reconnectu - ", true, false);
		OutputLog(LOG_SYSTEM, string,false, true);
	}*/

	if(pData[playerid][mute] - (GetTickCount() / 1000) > 0 && pData[playerid][mute] != 0)
	{
//		SetPlayerINIFileValue(playerid, "mute", pData[playerid][mute] - (GetTickCount() / 1000));	TODO zamienic na zapis do bazy danych, ale nie w 20 zapytaniach!
	}
	else
	{
//		SetPlayerINIFileValue(playerid, "mute", 0);
	}
	
	if(pData[playerid][jail] >= 0)
	{
//		SetPlayerINIFileValue(playerid, "jail", pData[playerid][jail] - (GetTickCount() / 1000));
//		SetPlayerAccountDataInt(playerid, "jail", pData[playerid][jail] - (GetTickCount() / 1000));
	}
	else
	{
//		SetPlayerINIFileValue(playerid, "jail", -1);
		pData[playerid][jail]=-1;
		SetPlayerAccountDataInt(playerid, "jail", -1);
	}
	
	if (gmTemp[showJoins]>0)
	foreach(i)
	{
		if(pData[i][spectating] == playerid)
		{
			StopSpectating(i);
		}
//		if (gmTemp[showJoins]==0) continue;
		if(i == playerid || reason == LEAVE_REASON_KICKBAN) continue;
		


		new
		 buffer[128],
		 szPlayerName[24],
		 playedSeconds = GetPlayerCurrentSession(playerid),
		 tempPeriod;
			
		copy(GetPlayerProperName(playerid), szPlayerName);
		GetOptimalTimeUnit(playedSeconds, tempPeriod); 

		if (gmTemp[showJoins]==2 || IsAdmin(i))
		{
			format(buffer, sizeof buffer, (SkinKobiecy(pData[playerid][lastUsedSkin])) ? TXT(i, 71) : TXT(i, 70), szPlayerName, playerid, playedSeconds, GetPeriodName(playerid, tempPeriod, playedSeconds), (reason == LEAVE_REASON_TIMEOUT) ? TXT(i, 230) : "");
			Msg(i, COLOR_LEAVEINFO, buffer, ((gmTemp[pPlayerCount]<8) ? true : false));
		}
//		format(buffer, sizeof buffer, (SkinKobiecy(pData[playerid][lastUsedSkin])) ? TXT(i, 71) : TXT(i, 70), szPlayerName, playerid, playedSeconds, GetPeriodName(playerid, tempPeriod, playedSeconds), (reason == LEAVE_REASON_TIMEOUT) ? TXT(i, 230) : "");
//		Msg(i, COLOR_LEAVEINFO, buffer, ((gmTemp[pPlayerCount]<8) ? true : false));
	}
	pData[playerid][loggedIn]=false;
	pData[playerid][vipEnabled]=false;
	pData[playerid][adminLevel]=0;
	pData[playerid][allowedLevel]=0;
	pData[playerid][session]=0;
	pData[playerid][accountID]=0;
	pTemp[playerid][curPos]=1;
	pTemp[playerid][lastPos]=-1;

	pTemp[playerid][cenzura]=false;
	pTemp[playerid][protkill]=false;
	pTemp[playerid][protping]=false;
	pTemp[playerid][troll]=false;
	pTemp[playerid][lockedPos]=false;
	pTemp[playerid][weaponsAllowed]=true;

	pTemp[playerid][weaponSkill_pistol]=0;
	pTemp[playerid][weaponSkill_silenced]=0;
	pTemp[playerid][weaponSkill_sawnoff]=0;
	pTemp[playerid][weaponSkill_uzi]=0;
	pTemp[playerid][faction]=FACTION_NONE;
	UpdatePlayerStatsTD(playerid);

	return 1;
}

public OnPlayerSpawn(playerid)
{
	if (pTemp[playerid][onArena]==ARENA_SOLO) {
//		new buf[128];
//		format(buf, sizeof buf, "pic arena solo %d vs %d", playerid, soloinv[playerid][esi_targetplayerid]);
//		SendClientMessageToAll(-1, buf);
		solo_OnPlayerDeath(playerid,soloinv[playerid][esi_targetplayerid],54);
	}
	pTemp[playerid][dead]=false;


	if(pData[playerid][citySelecting])
	{
		SetPlayerInterior(playerid, 0);
		return 1;
	}

	if(pTemp[playerid][specPosReturn])
	{
		Teleport(T_PLAYER, playerid, pTemp[playerid][specPosition][0], pTemp[playerid][specPosition][1], pTemp[playerid][specPosition][2], pTemp[playerid][specPosition][3], pTemp[playerid][specInterior], pTemp[playerid][specVirtualWorld]);
		SetCameraBehindPlayer(playerid);
		pTemp[playerid][specPosReturn] = false;
		
		return 1;
	}


	if (pTemp[playerid][onArena]>0)
		return arena_SpawnPlayer(playerid);
	
	new
	 randArg;

	if (pData[playerid][selectedSpawn]==1 && pData[playerid][gang]!=NO_GANG)
		gangs_TeleportPlayerToBaseSpawn(playerid,true);
	else if (pData[playerid][selectedSpawn]==0 && pTemp[playerid][e_houseid]>=0)
		domy_tpto(playerid);
	else 
	switch (random(3)) {
//	switch(pData[playerid][mainCity])
//	{
		case 0://CITY_LS:
		{
			randArg = random(sizeof DATA_spawns_LS);
			Teleport(T_PLAYER, playerid, DATA_spawns_LS[randArg][X], DATA_spawns_LS[randArg][Y], DATA_spawns_LS[randArg][Z], DATA_spawns_LS[randArg][angle], 0, VW_DEFAULT);
		}
		case 1://CITY_SF:
		{
			randArg = random(sizeof DATA_spawns_SF);
			Teleport(T_PLAYER, playerid, DATA_spawns_SF[randArg][X], DATA_spawns_SF[randArg][Y], DATA_spawns_SF[randArg][Z], DATA_spawns_SF[randArg][angle], 0, VW_DEFAULT);
		}
		case 2://CITY_LV:
		{
			randArg = random(sizeof DATA_spawns_LV);
			Teleport(T_PLAYER, playerid, DATA_spawns_LV[randArg][X], DATA_spawns_LV[randArg][Y], DATA_spawns_LV[randArg][Z], DATA_spawns_LV[randArg][angle], 0, VW_DEFAULT);
		}
	}
	SetPlayerProperColor(playerid);//,pData[playerid][currentColor]*256+PLAYER_COLOR_ALPHA);
	// synchronizujemy zegarek z czasem w grze
	SyncPlayerGameTime(playerid);
	SyncPlayerWeather(playerid);
	
	ShowElement(playerid, TDE_VEHICLEBOX, false);


	// BRONIE DLA GRACZY
	new skin=GetPlayerSkin(playerid);

	if (pTemp[playerid][troll]) {
		if (random(2)==1)
			SetPlayerVirtualWorld(playerid, VW_TROLLE);
		if (random(2)==1)
			SetPlayerInterior(playerid,random(10)+1);
		return 1;
	}

	if (pData[playerid][respect]<10000) {
		if (SkinKobiecy(skin)) {
			GivePlayerWeapon(playerid, 14,1);	// kwiaty 10
			SetPlayerAmmo(playerid, 14, 1);
		} else {
			GivePlayerWeapon(playerid, 1, 1);	// kastet 0
			SetPlayerAmmo(playerid, 1, 1);
		}
	} else {
		GivePlayerWeapon(playerid,4,1);			// noz, slot 1
		SetPlayerAmmo(playerid, 4, 1);
	}
	
	// KASA
	GivePlayerMoney(playerid, 2000 + floatround(pData[playerid][respect]/2));
		
	if (pTemp[playerid][weaponSkill_pistol]>500)
		GivePlayerWeapon(playerid, 22+random(1), 1000);	// desert eagle
	else
		GivePlayerWeapon(playerid, 24, 1000);	// desert eagle

	if (pTemp[playerid][weaponSkill_uzi]>500)
		GivePlayerWeapon(playerid, 32, 1000); 	// uzi
	else
		GivePlayerWeapon(playerid, 29, 1000); 	// mp5

	if (pData[playerid][respect]>=1000) {
	
		if (pTemp[playerid][weaponSkill_sawnoff]>500)
			GivePlayerWeapon(playerid, 26, 200);	// shotgun
		else
			GivePlayerWeapon(playerid, 25, 200);	// shotgun

	}
	if (pData[playerid][respect]>=2000)
		GivePlayerWeapon(playerid, 31,	3000);	// m4
	if (pData[playerid][respect]>=3500)
		GivePlayerWeapon(playerid, 31,	3000);	// m4
	if (pData[playerid][respect]>=7000)
		GivePlayerWeapon(playerid, 34,	300);	// sniperka

	// armor
	if (pData[playerid][respect]>=1500 && pData[playerid][respect]<3500)
		SetPlayerArmour(playerid,25.0);
	if (pData[playerid][respect]>=3500 && pData[playerid][respect]<5000)
		SetPlayerArmour(playerid,50.0);
	else if (pData[playerid][respect]>=5000 && pData[playerid][respect]<15000) 
		SetPlayerArmour(playerid,75.0);
	else if (pData[playerid][respect]>=15000)
		SetPlayerArmour(playerid,100.0);

	// molotovy/granaty
	if (pData[playerid][respect]>=75000)
		GivePlayerWeapon(playerid, 16,	200);	// granat
	else if (pData[playerid][respect]>=40000)
		GivePlayerWeapon(playerid, 18,	200);	// molotov

	


	// 2 -1000
	// 31 -1000	// m4
	// 24 25 34 27
	// 22 26 28 32 inne

	SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL, pTemp[playerid][weaponSkill_pistol]);
	SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL_SILENCED, pTemp[playerid][weaponSkill_silenced]);
	SetPlayerSkillLevel(playerid,WEAPONSKILL_SAWNOFF_SHOTGUN, pTemp[playerid][weaponSkill_sawnoff]);
	SetPlayerSkillLevel(playerid,WEAPONSKILL_MICRO_UZI, pTemp[playerid][weaponSkill_uzi]);

	if (pData[playerid][gang]!=NO_GANG && pData[playerid][loggedIn])
		gangs_SetPlayerAttachedObject(playerid);

	if (gmData[artefactOwner] == playerid)
		artefact_SetPlayerHolding(playerid);
	if (pTemp[playerid][faction]!=FACTION_NONE)
		factions_SetPlayerAttObject(playerid);

	SetCameraBehindPlayer(playerid);
	
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	new
	 buffer[128];
//	printf("PDEATH: %d %d %d", playerid, killerid, reason);

	pTemp[playerid][dead]=true;

	if(gmData[artefactOwner] == playerid)
		DropArtefact(playerid);

	pTemp[playerid][killStreak]=0;
	if (pData[playerid][pAttraction] == A_NONE || pData[playerid][pAttraction] == A_ARENA || pData[playerid][pAttraction] == A_WG) {
		pData[playerid][deaths]++;
		if (killerid!=INVALID_PLAYER_ID) {


			if (pTemp[killerid][lastPlayerKilled]!=playerid) {
				pData[killerid][skill]++;
				if (pData[playerid][skill]>0) pData[playerid][skill]--;
				if (pData[killerid][loggedIn] && pData[killerid][kills]++%10==0)
					avt_record(killerid,e_kills,pData[killerid][kills],ART_UPDATE);

			}


			
			pTemp[killerid][killStreak]++;
			pTemp[killerid][lastPlayerKilled]=playerid;		// nie zliczamy podwojnych zabic


		}
	}

	if(killerid!=INVALID_PLAYER_ID && (pData[killerid][gang] != NO_GANG && pData[playerid][gang]!=NO_GANG) && (pData[playerid][pAttraction] == A_NONE || pData[playerid][pAttraction] == A_ARENA)) {
		//  --- Team kill ---
		if(pData[playerid][gang] == pData[killerid][gang] && pTemp[playerid][onArena]!=ARENA_SOLO) // Team-kill
		{
				pData[killerid][teamkills]++;
				if (gData[pData[killerid][gang]][respect]>0)  gData[pData[killerid][gang]][respect]--;
		} else if (pData[playerid][gang] != pData[killerid][gang]) { // Other Team-kill
				gData[pData[killerid][gang]][respect]++;
				if (gData[pData[playerid][gang]][respect]>0) gData[pData[playerid][gang]][respect]--;
		}
	}

	if (killerid!=INVALID_PLAYER_ID && pTemp[killerid][onArena]!=ARENA_NONE)
//		printf("Zabicie na arenie %d przez %d", pTemp[killerid][onArena], killerid);
		pTemp[killerid][arenaScore][pTemp[killerid][onArena]]++;
	



	if(pData[playerid][pAttraction] == A_NONE)
	{

		

		if (killerid==playerid || killerid==INVALID_PLAYER_ID)
			pData[playerid][suicides]++;

		if(gmData[artefactOwner] == playerid)
			DropArtefact(playerid);
		new bool:dropmoney=true;

		//  --- Normal kill ---
		if(!gmTemp[protAll] && killerid!=INVALID_PLAYER_ID && (!pTemp[killerid][protkill] && pData[killerid][adminLevel]<LEVEL_GM))
		{
			if(IsPlayerInNoDMArea(playerid)) {
				Msg(killerid, COLOR_INFO2, "Zabijanie w strefie bez DM jest zakazane.");
				JailPlayer(killerid, 4);
				dropmoney=false;
			} else if (!IsPlayerInFullDMArea(playerid)) {
				if (GetPlayerState(killerid)==PLAYER_STATE_DRIVER) { // || pstate==PLAYER_STATE_PASSENGER) 
					dropmoney=false;
					switch(GetVehicleModel(GetPlayerVehicleID(killerid))){
						case 537,538,	// pociagi
							460,511,512,513,519,553,593: { }		// samoloty oprocz andromedy, at-400, hydry, rustlera
						default: {
							Msg(killerid,COLOR_INFO2,"Zabijanie z uzyciem pojazdu jest zakazane.");
							JailPlayer(killerid, 1);
						}
		
					}

				}
			}

		}

		new
		 pMoney = GetPlayerMoney(playerid);
		
		if(dropmoney && pMoney >= 2)// && (killerid==playerid || killerid==INVALID_PLAYER_ID))
		{
			GivePlayerMoney(playerid, -(pMoney / 2));
			
			new
			 pVector[e_Vectors];
			
			GetPlayerPos(playerid, pVector[X], pVector[Y], pVector[Z]);
		
			if(gmTemp[moneyPickupIdx] >= MAX_MONEY_PICKUPS)
				gmTemp[moneyPickupIdx] = 0;
			
			if(gMoneyPickup[gmTemp[moneyPickupIdx]][gmpTime] != 0)
			{
				if (IsValidDynamicPickup(gMoneyPickup[gmTemp[moneyPickupIdx]][gmpPickupID]))
					DestroyDynamicPickup(gMoneyPickup[gmTemp[moneyPickupIdx]][gmpPickupID]);
			
				if (IsValidDynamic3DTextLabel(gMoneyPickup[gmTemp[moneyPickupIdx]][gmp3DText]))
					DestroyDynamic3DTextLabel(gMoneyPickup[gmTemp[moneyPickupIdx]][gmp3DText]);
				gMoneyPickup[gmTemp[moneyPickupIdx]][gmpTime]=0;
			}
		
			format(buffer, sizeof buffer, "$%i", pMoney / 2);
		
			gMoneyPickup[gmTemp[moneyPickupIdx]][gmpPickupID] = CreateDynamicPickup(1212, 1, pVector[X], pVector[Y], pVector[Z], GetPlayerVirtualWorld(playerid),GetPlayerInterior(playerid));
			gMoneyPickup[gmTemp[moneyPickupIdx]][gmpMoney] = pMoney / 2;
			gMoneyPickup[gmTemp[moneyPickupIdx]][gmpTime] = GetTickCount() / 1000;
			gMoneyPickup[gmTemp[moneyPickupIdx]][gmp3DText] = CreateDynamic3DTextLabel(buffer, 0x539C0090, pVector[X], pVector[Y], pVector[Z] + 0.3, 10.0, INVALID_PLAYER_ID,INVALID_VEHICLE_ID,0, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid),-1, 10.0);
		
			gmTemp[moneyPickupIdx]++;
		}
	} else if (pData[playerid][pAttraction] == A_WG) {
		pTemp[playerid][aWGDead]=true;
		pData[playerid][pAttraction]=A_NONE;
		WG_Update();
	} else if (pData[playerid][pAttraction] == A_CHOWANY) {
		pTemp[playerid][aCHDead]=true;
		pData[playerid][pAttraction]=A_NONE;
		CH_Update();
	}
	
		
	if(killerid != INVALID_PLAYER_ID && pData[playerid][hitman] > 0)
	{
		foreach(i)
		{
			format(buffer, sizeof buffer, TXT(i, 294), GetPlayerNick(killerid), GetPlayerNick(playerid), pData[playerid][hitman]);
			Msg(i, COLOR_INFO3, buffer);
		}
	
		GivePlayerMoney(killerid, pData[playerid][hitman]);
		pData[playerid][hitman] = 0;
		
		if(pData[playerid][loggedIn])
		{
			SetPlayerAccountDataInt(playerid, "hitman_prize", 0);
		}
		
		PlaySound(killerid, 1057);
//		if (IsValidDynamic3DTextLabel(pTemp[playerid][hitman3DTextLabel]))
//			DestroyDynamic3DTextLabel(pTemp[playerid][hitman3DTextLabel]);
		IncreasePlayerHitman(playerid, 0);
	}
	
	SetServerStatString("death_count", "value + 1", true);
		
	if(killerid != INVALID_PLAYER_ID)
	{
			SetServerStatString("kill_count", "value + 1", true);
	}
		
	foreach(i)
	{
		if(pData[i][spectating] == playerid)
		{
			StopSpectating(i);
		}
	}
	
	if(reason == 51)
	{
		reason = pTemp[killerid][lastWeaponUsed];
	}
	
	SendDeathMessage(killerid, playerid, reason);
	
	//  --- ATTRACTION STUFF ---
	
	if(pData[playerid][pAttraction] != A_NONE)
	{
		switch(pData[playerid][pAttraction])
		{
			case A_STRZELNICA: pData[playerid][pAttraction] = A_NONE;
			case A_LABIRYNT: pData[playerid][pAttraction] = A_NONE;
			case A_DERBY: Derby_RemovePlayer(playerid,true);
//			case A_WG: pData[playerid][pAttraction] = A_NONE;
			case A_CHOWANY:
			{
				if(killerid != INVALID_PLAYER_ID)
				{
					for(new i = 0; i < gmTemp[aChowanyMaxPlayers]; i++)
					{
						if(gmTemp[aChowanyPlayers][i] == INVALID_PLAYER_ID) continue;
						
						format(buffer, sizeof buffer, TXT(i, 439), GetPlayerNick(killerid), GetPlayerNick(playerid));
						Msg(i, COLOR_INFO3, buffer);
					}
				}
			
				pData[playerid][pAttraction] = A_NONE;
				ShowElement(playerid, TDE_INFOBOX, false);
				CH_Update();
			}
			case A_ARENA:
				if (arena_OnPlayerDeath(playerid,killerid,reason)==1) return 1;
		}
	}
	
	for(new i = 0; i < gmTemp[aStrzelnicaMaxPlayers]; i++)
	{
		if(playerid != gmTemp[aStrzelnicaPlayers][i]) continue;
		
		gmTemp[aStrzelnicaPlayers][i] = INVALID_PLAYER_ID;
	}
	
	for(new i = 0; i < gmTemp[aLabiryntMaxPlayers]; i++)
	{
		if(playerid != gmTemp[aLabiryntPlayers][i]) continue;
		
		gmTemp[aLabiryntPlayers][i] = INVALID_PLAYER_ID;
	}
	
/*	for(new i = 0; i < gmTemp[aWGMaxPlayers]; i++)
	{
		if(playerid != gmTemp[aWGPlayers][i]) continue;
		gmTemp[aWGPlayers][i] = INVALID_PLAYER_ID;
	}*/
	
	for(new i = 0; i < gmTemp[aChowanyMaxPlayers]; i++)
	{
		if(playerid != gmTemp[aChowanyPlayers][i]) continue;
		
		gmTemp[aChowanyPlayers][i] = INVALID_PLAYER_ID;
	}
	
	SetPlayerTeam(playerid, playerid);
	SetPlayerWorldBounds(playerid, 20000.0, -20000.0, 20000.0, -20000.0);
	
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	tVehicles[vehicleid][vo_used]=false;
	tVehicleUsed[vehicleid]=false;
	tVehicles[vehicleid][vo_occupied]=false;
	tVehicles[vehicleid][vo_driver]=INVALID_PLAYER_ID;

	if(tVehicles[vehicleid][vo_private]) {
			if (tVehicles[vehicleid][vo_owningPlayerId]!=INVALID_PLAYER_ID) {
				Msg(tVehicles[vehicleid][vo_owningPlayerId],COLOR_INFO,"Twoj pojazd wrocil pod Twoj dom.");
				// w tej funkcji nastapi zniszczenie aktualnego pojazdu i zespawnowanie go pod domem gracza
				domy_SpawnVehicle(tVehicles[vehicleid][vo_owningPlayerId]);
			} else {	// wlasciciel sie rozlaczyl a pojazd ulegl respawnowi
				tVehicles[vehicleid][vo_private]=false;
				tVehicles[vehicleid][vo_houseid]=-1;
				FSDomy[tVehicles[vehicleid][vo_houseid]][ehv_id]=INVALID_VEHICLE_ID;
				DestroyVehicle(vehicleid);
			}
			return 1;
	}
	if(vehicleid > staticVehicleCount) tVehicleSpawned[vehicleid]=true;

	new vmodel=GetVehicleModel(vehicleid); 
	tVehicles[vehicleid][vo_paintjob]=0;
	vehicleHasNitro[vehicleid]=false;
	tVehicles[vehicleid][vo_hasTurbo]=false;


	if (tVehicles[vehicleid][vo_static]) {
		vehicleDoorState[vehicleid] = DOOR_OPENED;
		SetVehicleParamsEx(vehicleid, 1, 1, random(2), 0, 0, 0, 0);
		switch(vmodel){
			case 400,401,404,405,410,415,418,420,421,422,426,436,439,477,478,489,491,492,496,500,505,516,517,518,527,529,534,
				535,536,540,542,546,547,549,550,551,558,559,560,561,562,565,567,575,576,580,585,589,600,603: {
				if (random(3)==1)
					TuneCar(vehicleid);
				if (random(3)==1) {
					tVehicles[vehicleid][vo_paintjob]=random(3);
					ChangeVehiclePaintjob(vehicleid, tVehicles[vehicleid][vo_paintjob]);
				}
			}			
		}
	}


//	if (vmodel==432) SetVehicleHealth(vehicleid, 1000.0);		// rhino
	SetVehicleHealth(vehicleid, VEHICLE_DEFAULT_HP);

	if (!tVehicles[vehicleid][vo_licensePlateSet]) {
		new string[50];
		format(string,sizeof(string),"{000000}%c%c%c %i%i%i",(65+random(26)),(65+random(26)),(65+random(26)),random(10),random(10),random(10));
		SetVehicleNumberPlate(vehicleid,string);
	}
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	tVehicles[vehicleid][vo_used]=false;
	tVehicles[vehicleid][vo_occupied]=false;
	tVehicles[vehicleid][vo_driver]=INVALID_PLAYER_ID;


	if(tVehicles[vehicleid][vo_private]) {
		if (tVehicles[vehicleid][vo_owningPlayerId]!=INVALID_PLAYER_ID) {
			Msg(tVehicles[vehicleid][vo_owningPlayerId],COLOR_INFO,"Twoj pojazd wrocil pod Twoj dom.");
			return domy_SpawnVehicle(tVehicles[vehicleid][vo_owningPlayerId]);
		} else {
			tVehicles[vehicleid][vo_private]=false;
			tVehicles[vehicleid][vo_houseid]=-1;
			FSDomy[tVehicles[vehicleid][vo_houseid]][ehv_id]=INVALID_VEHICLE_ID;
		}
		return 1;
	}

	vehicleHasNitro[vehicleid]=false;
	tVehicles[vehicleid][vo_hasTurbo]=false;
	vehicleDoorState[vehicleid] = DOOR_OPENED;
	vehicleDoorOwner[vehicleid] = INVALID_PLAYER_ID;
	tVehicles[vehicleid][vo_licensePlateSet]=false;
	tVehicles[vehicleid][vo_drift]=false;
	return 1;
}

public OnPlayerText(playerid, text[])
{
	if(pData[playerid][logonDialog] || pData[playerid][classSelecting]) {
		Msg(playerid,COLOR_ERROR,"Najpierw musisz dolaczyc do gry.");
		return 0;
	}

	pTemp[playerid][lastPos]=-1;

	if(pData[playerid][jail]>0) {
		Msg(playerid, COLOR_ERROR, "Niestety - siedzisz w paczce.");
		return 0;
	}
	
	if((strlen(GetPlayerNick(playerid)) + strlen(text) + 6) > 128)
	{
		Msg(playerid, COLOR_ERROR, TXT(playerid, 399));
		return 0;
	}
	if(pData[playerid][mute] != 0 && pData[playerid][mute] - (GetTickCount() / 1000) > 0 )
	{
		new
		 buffer[128],
		 period,
		 muteTimeLeft = pData[playerid][mute] - (GetTickCount() / 1000);
		
		GetOptimalTimeUnit(muteTimeLeft, period); 
		if(period == 'm') muteTimeLeft += 1;
		
		format(buffer, sizeof buffer, TXT(playerid, 251), muteTimeLeft, GetPeriodName(playerid, period, muteTimeLeft));
		Msg(playerid, COLOR_ERROR, buffer);
	
		return 0;
	}
	
	if (FilterText(playerid, text, true) == 0) return 0;
//	copy(gmTemp[chatLastText], text);
//	if(!IsAdmin(playerid)) 
	if (gmTemp[lastHour]>6 && gmTemp[lastHour]<23)	// cenzura nieaktywna w nocy
		CensorText(text);		// jak cenzura to dla wszystkich
	
	//  --- Game moderator chat ---
	
	if(text[0] == '$') {
		if (IsGM(playerid))
			OutputModeratorChat(playerid, text[1]);
		else 
			Msg(playerid,COLOR_ERROR,"Nie jestes GM-em!");
		return 0;
		
	}
	
	//  --- Master & Normal admin chat ---
	
	if(text[0] == '@' && text[1]=='@'){
		if (IsAdmin(playerid, LEVEL_ADMIN3))
			OutputAdmin3Chat(playerid, text[2]);
		else 
			Msg(playerid,COLOR_ERROR,"Nie jestes adminem!");
		return 0;
	} else if(text[0]=='@'){
		if (IsAdmin(playerid))
			OutputAdminChat(playerid, text[1]);
		else 
			Msg(playerid,COLOR_ERROR,"Nie jestes Adminem!");
		return 0;
	}
	
	//  --- Faction chat ---
	
	if (text[0]=='#') {
		if (pTemp[playerid][faction]!=FACTION_NONE)
			OutputFactionChat(playerid, text[1]);
		else
			Msg(playerid,COLOR_ERROR,"Nie nalezysz do zadnej frakcji!");
		return 0;
	}
	
	//  --- Gang chat ---
	
	if(text[0] == '!') {
		if (pData[playerid][loggedIn] && pData[playerid][gang] != NO_GANG)
			OutputGangChat(playerid, text[1]);
		else
			Msg(playerid,COLOR_ERROR,"Nie jestes w gangu!");
		return 0;
	}


	
	new
	 buffer[160];
	
//	format(buffer, sizeof buffer, "%s (%i): %s", GetPlayerNick(playerid), playerid, text);
//	OutputLog(LOG_CHAT, buffer);
	
//	if(gmData[chatColors]) FilterTextWithColors(text);
	
	if (gmTemp[chatLastMSGSender]!=playerid) {
		gmTemp[chatLastMSGSender]=playerid;
		gmTemp[chatMSGCount]=(gmTemp[chatMSGCount]+1)%2;
	}

	

	if (gmTemp[chatMSGCount]==0)
		format(buffer, sizeof buffer, "%i {%06x}%s: {FFFFFF}%s", playerid, pData[playerid][currentColor], GetPlayerNick(playerid), text);
	else
		format(buffer, sizeof buffer, "%i {%06x}%s: {EAEAEA}%s", playerid, pData[playerid][currentColor], GetPlayerNick(playerid), text);

	if (gmTemp[chatDisabled] && pData[playerid][adminLevel]<LEVEL_ADMIN2) {
		Msg(playerid, COLOR_ERROR, "Czat jest obecnie wylaczony.");
		return 0;
	}

	if (ContainsIP(text)) {
		SendClientMessage(playerid, pData[playerid][adminLevel]==LEVEL_ADMIN3 ? 0xA01010FF : 0x606060FF, buffer);
		MSGToAdmins(COLOR_ADMIN, buffer, true, LEVEL_ADMIN2);
		format(buffer, sizeof buffer, "Ninjabanowana proba reklamy przez {b}%s (%d){/b}.", GetPlayerNick(playerid), playerid);
		MSGToAdmins(COLOR_ADMIN, buffer, false, LEVEL_ADMIN2);
		return 0;
	}
	
	SendClientMessageToAll(pData[playerid][adminLevel]==LEVEL_ADMIN3 ? 0xA01010FF : 0x606060FF, buffer);
	
	return 0;
}

/*public OnPlayerCommandText(playerid, cmdtext[])
{
	return 0;
}*/

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger) // wykonywane nawet jak gracz zrezygnuje z akcji
{
	if (pTemp[playerid][troll])
		RemovePlayerFromVehicle(playerid);

	if (tVehicles[vehicleid][vo_private] && tVehicles[vehicleid][vo_owningPlayerId]!=INVALID_PLAYER_ID && tVehicles[vehicleid][vo_owningPlayerId]!=playerid) {
		new buf[128];
		format(buf,sizeof buf,"To jest prywatny pojazd {b}%s{/b}(%d).", GetPlayerProperName(tVehicles[vehicleid][vo_owningPlayerId]), tVehicles[vehicleid][vo_owningPlayerId]);
		Msg(playerid,COLOR_INFO,buf,false);
		if (!ispassenger || !tVehicles[vehicleid][vo_occupied]) {
			RemovePlayerFromVehicle(playerid);
			return 0;
		}
	}

	tVehicles[vehicleid][vo_used]=true;

//	if (!ispassenger)
//		tVehicles[vehicleid][vo_driver]=playerid;

	tVehicles[vehicleid][vo_occupied]=true;

	tVehicleUsed[vehicleid]=true;

/*	if (!ispassenger) {
		new buf[1024];
		strcat(buf,"_~n~_~n~_");
		new vmodel=GetVehicleModel(vehicleid);
		new godzina=floatround( ((GetTickCount()/1000)+gmTemp[gametime_offset])/60)%24;
		switch(vmodel) {
			case 525: {
				strcat(buf,"~n~~r~~k~~VEHICLE_TURRETUP~~w~/~r~~k~~VEHICLE_TURRETDOWN~~w~ sterowanie hakiem");

				if (godzina<7 || godzina>22)
					strcat(buf,"~n~~r~~k~~TOGGLE_SUBMISSIONS~~y~ swiatla");
				else
					GameTextForPlayer(playerid,"_~n~_~n~~r~~k~~VEHICLE_TURRETUP~~w~/~r~~k~~VEHICLE_TURRETDOWN~~w~ sterowanie hakiem",1500,5);
			} default: {

				if (tVehicles[vehicleid][vo_private])
					strcat(buf,"~n~~r~~k~~VEHICLE_TURRETUP~~w~ zaplon");

				new r=GetVehicleFlags(vehicleid);
				if (r&VF_NATATORIAL!=VF_NATATORIAL && r&VF_AIRBORNE!=VF_AIRBORNE && r&VF_MILITARY!=VF_MILITARY && r&VF_RC!=VF_RC && r&VF_TRAILER!=VF_TRAILER && r&VF_RAILROAD!=VF_RAILROAD && r&VF_BIKES!=VF_BIKES)
					strcat(buf,"~n~~r~~k~~VEHICLE_TURRETDOWN~~w~ maska/bagaznik");
				
				if (godzina<7 || godzina>22)
					strcat(buf,"~n~~r~~k~~TOGGLE_SUBMISSIONS~~y~ swiatla");

			}
		}
		if (strlen(buf)>10)
			GameTextForPlayer(playerid,buf,3500,5);

	}	*/
	if (tVehicles[vehicleid][vo_drift]) {
		if (!ispassenger)
			Msg(playerid,COLOR_INFO,"Wsiadles do pojazdu drifterskiego, mozesz je w kazdej chwili naprawic/odwrocic wciskajac {b}2{/b}.",false);
		SetPlayerArmedWeapon(playerid,0);
	}
	
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	if (!engine) engine=1;
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);


	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	if (pData[playerid][pAttraction]==A_DERBY) {
		RespawnVehicle(vehicleid);
		return Derby_RemovePlayer(playerid);
	} else if (pData[playerid][pAttraction]==A_RACE) {
		RespawnVehicle(vehicleid);
		return race_RemovePlayer(playerid);
	} else if (pData[playerid][pAttraction]==A_DRIFT) {
		RespawnVehicle(vehicleid);
		return drift_RemovePlayer(playerid);
	}

	tVehicles[vehicleid][vo_used]=true;
	if (GetPlayerVehicleSeat(playerid)==0) {	// wysiadl kierowca
		tVehicles[vehicleid][vo_occupied]=false;
//		if (tVehicles[vehicleid][vo_driver]==playerid)
//			tVehicles[vehicleid][vo_driver]=INVALID_PLAYER_ID;
	}
	
	tVehicleUsed[vehicleid]=true;
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
//	if (playerid==gmNPC[gmnt_fullserver] && random(5)==1)
//		return SetupNPC(gmnt_fullserver);

	SyncPlayerGameTime(playerid);

	// Enter vehicle
	if((newstate == 2 && oldstate == 1) || (newstate == 3 && oldstate == 1))
	{

		pData[playerid][lastVehicle]=GetPlayerVehicleID(playerid);
		if (newstate==PLAYER_STATE_DRIVER) {
				if (tVehicles[pData[playerid][lastVehicle]][vo_private] && 
						tVehicles[pData[playerid][lastVehicle]][vo_owningPlayerId]!=INVALID_PLAYER_ID && 
						tVehicles[pData[playerid][lastVehicle]][vo_owningPlayerId]!=playerid) {
						RemovePlayerFromVehicle(playerid);
						return 0;
				}
				tVehicles[pData[playerid][lastVehicle]][vo_driver]=playerid;
		} else if (newstate==PLAYER_STATE_PASSENGER) {
			// pojazd bez kierowcy lub pojazd drifterski
			if (tVehicles[pData[playerid][lastVehicle]][vo_driver]==INVALID_PLAYER_ID || tVehicles[pData[playerid][lastVehicle]][vo_drift])
				SetPlayerArmedWeapon(playerid,0);	// ustawiamy na piesci
		}
						
		
		if (vehicleDoorState[pData[playerid][lastVehicle]] == DOOR_CLOSED)
			TextDrawBoxColor(pTextDraw[PTD_VEHICLEINFO][playerid],0xcc000030);
		else
			TextDrawBoxColor(pTextDraw[PTD_VEHICLEINFO][playerid],3145776);

		if(pData[playerid][hudSetting][HUD_VEHICLEBOX]) ShowElement(playerid, TDE_VEHICLEBOX, true);

		
		
		foreach(i)
		{
			if(pData[i][spectating] == playerid)
			{
				if (tVehicles[pData[playerid][lastVehicle]][vo_drift])
					Msg(i,COLOR_INFO,"Gracz wsiadl do pojazdu drifterskiego (szybkie naprawianie/odwracanie)");
				PlayerSpectateVehicle(i, pData[playerid][lastVehicle]);
			}
		}

	}
	
	// Leave vehicle
	else if((newstate == 1 && oldstate == 2) || (newstate == 1 && oldstate == 3))
	{
		if (oldstate==PLAYER_STATE_DRIVER)
			tVehicles[pData[playerid][lastVehicle]][vo_driver]=INVALID_PLAYER_ID;


		ShowElement(playerid, TDE_VEHICLEBOX, false);
		if(pData[playerid][destroyMyVehicle] && pData[playerid][lastVehicle]!=INVALID_VEHICLE_ID)
		{
			DestroyVehicle(pData[playerid][lastVehicle]);
			pData[playerid][destroyMyVehicle] = false;
		}
		
		foreach(i)
		{
			if(pData[i][spectating] == playerid)
			{
				PlayerSpectatePlayer(i, playerid);
			}
		}

		


	}

	// dzwieki w pojazdach
	if (newstate==2 || oldstate==2)
		Audio_OnPlayerStateChange(playerid,newstate,oldstate);

	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	new
	 buffer[128];

	if(aData[A_LABIRYNT][aState] == A_STATE_ON && pData[playerid][pAttraction] == A_LABIRYNT)
	{
		foreach(i)
		{
			if(playerid == i) continue;
	
			format(buffer, sizeof buffer, TXT(i, 425), GetPlayerNick(playerid));
			Msg(i, COLOR_INFO3, buffer); // "xxx" wygrywa labirynt!

			if(pData[i][pAttraction] == A_LABIRYNT)
			{
				pData[i][pAttraction] = A_NONE;
				DisablePlayerSounds(i);
				DisablePlayerCheckpoint(i);
				SpawnPlayer(i);
			}

			
		}
				
		Msg(playerid, COLOR_INFO2, TXT(playerid, 426)); // Wygrywasz labirynt! +5 respekt
		pData[playerid][pAttraction] = A_NONE;
		DisablePlayerSounds(playerid);
		DisablePlayerCheckpoint(playerid);
		SpawnPlayer(playerid);
		GivePlayerScore(playerid,5,"Labirynt");
				
		aData[A_LABIRYNT][aState] = A_STATE_OFF;
	}

	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}


public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpDynamicPickup(playerid, pickupid)
{
	new
	 buffer[128];



/*	if(pickupid == gPickup[ship])	// wycofujemy calkiem statek
	{
		Msg(playerid, COLOR_INFO, TXT(playerid, 108));
		DestroyDynamic3DTextLabel(pTemp[playerid][p3D_ship]);
		pTemp[playerid][p3D_ship] = CreateDynamic3DTextLabel(TXT(playerid, 108), COLOR_3DTEXT_INFORMATION, 2000.6306, 1538.3073, 14.6000, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0,playerid);
	
		return 1;
	}*/	

	if(pickupid == gPickup[artefact] && gmData[artefactOwner] == INVALID_PLAYER_ID)
	{
		artefact_OnPlayerPickup(playerid);
		return 1;
	}


	if (prezenty_OPPickUpDynamicPickup(playerid,pickupid))	return 1;
	if (obiekty_OPPickUpDynamicPickup(playerid,pickupid)) return 1;
//	if (domy_OPPickUpDynamicPickup(playerid,pickupid)) return 1;
	
	
	for(new i = 0; i < MAX_MONEY_PICKUPS; i++)
	{
		if(gMoneyPickup[i][gmpTime]>0 && gMoneyPickup[i][gmpPickupID] == pickupid)
		{
			DestroyDynamicPickup(gMoneyPickup[i][gmpPickupID]);
			DestroyDynamic3DTextLabel(gMoneyPickup[i][gmp3DText]);
			gMoneyPickup[i][gmpTime] = 0;
			
			GivePlayerMoney(playerid, gMoneyPickup[i][gmpMoney]);
			
			format(buffer, sizeof buffer, TXT(playerid, 4), gMoneyPickup[i][gmpMoney]);
			Msg(playerid, COLOR_INFO, buffer);
			
			return 1;
		}
	}
	
	return 0;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	tVehicles[vehicleid][vo_paintjob]=paintjobid;
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{	
	tVehicles[vehicleid][vo_color][0]=color1;
	tVehicles[vehicleid][vo_color][1]=color2;
	return 1;
}

#if defined A_WG_WPNSEL
public OnPlayerSelectedMenuRow(playerid, row)
{
	new Menu:current = GetPlayerMenu(playerid);
	if(current==aWGWeaponMenu) return wg_OnPlayerSelectedMenuRow(playerid,row);
	return 1;
}
#endif

#if defined A_WG_WPNSEL
public OnPlayerExitedMenu(playerid)
{
	new Menu:current = GetPlayerMenu(playerid);
	if(current==aWGWeaponMenu) return wg_OnPlayerExitedMenu(playerid);
	return 1;
}
#endif

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid){


	if(newinteriorid>0)
		SetPlayerWeather(playerid,0);
	else
		SetPlayerWeather(playerid,gmTemp[currentWeather]);
	SyncPlayerGameTime(playerid);

	if (gmTemp[pGMCount]+gmTemp[pAdminCount]>0)
		foreach(i)
			if(pData[i][spectating]==playerid)
				StartSpectating(i,playerid);
	
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	new pstate=GetPlayerState(playerid);

	if (pstate==PLAYER_STATE_SPECTATING) {
		if (KEY_PRESSED(KEY_CROUCH) && (pData[playerid][spectating]!=INVALID_PLAYER_ID) && (pData[playerid][adminLevel]>=LEVEL_ADMIN1)) {
			cmd_info(playerid,"");	return 0;
		} else if (KEY_PRESSED(KEY_FIRE) && (pData[playerid][spectating]!=INVALID_PLAYER_ID) && (pData[playerid][adminLevel]>=LEVEL_ADMIN1)) {
			FindNextPlayerToSpectate(playerid, true);
			return 0;
		}
	} else if (pstate == PLAYER_STATE_ONFOOT) {
		if(KEY_PRESSED(KEY_FIRE))
	    {
			pTemp[playerid][staleTime]=0;
	        pTemp[playerid][lastWeaponUsed] = GetPlayerWeapon(playerid);
		}
		if(pTemp[playerid][performingAnim])	{	// resetujemy odtwarzana animacje
					pTemp[playerid][performingAnim]=false;
					ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.0, 0, 0, 0, 0, 0, 0);
					ClearAnimations(playerid, 0);
		}
	} else if (pstate==PLAYER_STATE_PASSENGER) {
		if (pTemp[playerid][faction]==FACTION_POLICE && KEY_PRESSED(KEY_SUBMISSION) && pData[playerid][pAttraction]==A_NONE && GetVehicleModel(GetPlayerVehicleID(playerid))==497) {
			// spuszczanie sie na linie
			vehicles_SWATDrop(playerid);
			return 0;
			
		}
	} else if (pstate==PLAYER_STATE_DRIVER) {
		new vid=GetPlayerVehicleID(playerid);
		if(vid>0 && vid!=INVALID_VEHICLE_ID) {
			if(KEY_PRESSED(KEY_SUBMISSION)) {
				if (pData[playerid][pAttraction]==A_RACE) { 
					race_Napraw(playerid);
					return 0;
				} else if (pData[playerid][pAttraction]==A_DRIFT) {
					drift_Napraw(playerid);
					return 0;
				} else if (tVehicles[vid][vo_drift]) {
					vehicles_DriftSubmission(playerid,vid);
				}
			} else if (KEY_PRESSED(KEY_FIRE)) {			// turbo

				if(pData[playerid][pAttraction]==A_DERBY && GetVehicleModel(vid)==564) { // rc tiger
					vehicles_RCTiger_shoot(playerid);
					return 0;
				} 
				


				if (pData[playerid][pAttraction]==A_NONE && tVehicles[vid][vo_hasTurbo]) {
					vehicles_EngageTurbo(playerid,vid,0);
					return 0;
				}

				if (tVehicles[vid][vo_drift]) {	// specjalny efekt turbo
					SetTimerEx("vehicles_AddNitro",50+random(400),false,"i",vid);
					return 0;
				}
				

			} else if (KEY_RELEASED(KEY_FIRE)) {	// nitro
				if (vehicleHasNitro[vid]) {
					AddVehicleComponent(vid, 1010);
				}
			} else 	if (KEY_PRESSED(KEY_ANALOG_DOWN)) {
				if (vid>0 && GetVehicleModel(vid)==525)	{ // towtruck
					TowVehicle(vid);
				} else if (vid>0) {
					new engine, lights, alarm, doors, bonnet, boot, objective;
					GetVehicleParamsEx(vid, engine, lights, alarm, doors, bonnet, boot, objective);
					if (!boot && !bonnet) 
						bonnet=1;
					else if (!boot && bonnet)
						boot=1;
					else if (boot && bonnet)
						bonnet=0;
					else if (boot && !bonnet)
						boot=0;	
					SetVehicleParamsEx(vid, engine, lights, alarm,doors, bonnet, boot, objective);
	
					tVehicles[vid][vo_used]=true;
					tVehicles[vid][vo_occupied]=true;

					tVehicleUsed[vid]=true;
					return 0;
					
				}
			} else if (KEY_PRESSED(KEY_ANALOG_UP) && GetVehicleModel(vid)!=525 && GetVehicleModel(vid)!=520 && GetVehicleModel(vid)!=530) {
					new engine, lights, alarm, doors, bonnet, boot, objective;
					GetVehicleParamsEx(vid, engine, lights, alarm, doors, bonnet, boot, objective);
					if (engine) {
						engine=0;
						Msg(playerid,COLOR_INFO,"{b}Gasisz silnik{/b}. Wcisnij znowu aby odpalic.",false);
					} else {
						engine=1;
						Msg(playerid,COLOR_INFO,"{b}Uruchamiasz silnik{/b}. Wcisnij znowu aby zgasic.",false);
					}
				
					SetVehicleParamsEx(vid, engine, lights, alarm,doors, bonnet, boot, objective);

					tVehicles[vid][vo_used]=true;
					tVehicles[vid][vo_occupied]=true;

					tVehicleUsed[vid]=true;
					return 0;
				
			} else if(KEY_PRESSED(KEY_SUBMISSION)) {
					new engine, lights, alarm, doors, bonnet, boot, objective;
					GetVehicleParamsEx(vid, engine, lights, alarm, doors, bonnet, boot, objective);
					if (lights) 
						lights=0;
					else
						lights=1;
					SetVehicleParamsEx(vid, engine, lights, alarm,doors, bonnet, boot, objective);

					tVehicles[vid][vo_used]=true;
					tVehicles[vid][vo_occupied]=true;

					tVehicleUsed[vid]=true;
					return 0;
			}
		}	// vid!=0 
	} //pstate==driver
		
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	if(success)
	{
		SetTimer("SearchForNewAdmin", 50, false);
	}
	else
	{
		new
		 buffer[128];
		 
	 	static RconLoginTimes;
		static Times;

		if((GetTickCount()-RconLoginTimes) < 1000){
			Times++;
			if(Times > 2){
				Times = RconLoginTimes = 0;
				foreach(hackerid){

					if(!strcmp(ip,GetPlayerIP(hackerid),true)){
						SendClientMessage(hackerid, 0xFFFFFFFF, "SERVER: Wrong Password. Bye!");
						BanEx(hackerid,"RCON CRASH ATTEMPT!");
					}
				}
			}
		}else{
			Times = RconLoginTimes = 0;
		}

		RconLoginTimes = GetTickCount();
		
		foreach(playerid)
		{
			if(IsAdmin(playerid, LEVEL_ADMIN3))
			{
				format(buffer, sizeof buffer, TXT(playerid, 146), ip, "---");
				Msg(playerid, COLOR_INFO2, buffer);
			}
		}
	}
	
	new
	 buffer[128];
	
	format(buffer, sizeof buffer, "[Logowanie na RCON] IP: %s - haslo: %s - zalogowano: %s", ip, password, success ? ("tak") : ("nie"));
	OutputLog(LOG_SYSTEM, buffer);

	return 1;
}

public OnPlayerUpdate(playerid)
{
	if (IsPlayerNPC(playerid)) return 1;

	static
	 keys,
	 updown,
	 leftright,
	 pState,
	 wepid;

	pTemp[playerid][staleTime]=0;
	 
	GetPlayerKeys(playerid, keys, updown, leftright);

	if(pData[playerid][citySelecting] && !pTemp[playerid][holdingKey] && (leftright != 0 || keys == KEY_JUMP || keys == KEY_SECONDARY_ATTACK))
		ProcessPlayerCitySelecting(playerid, keys, leftright);
	
	if(keys != 0 || updown != 0 || leftright != 0)
		pTemp[playerid][holdingKey] = true;
	else
		pTemp[playerid][holdingKey] = false;

	pState = GetPlayerState(playerid);
	
	wepid=GetPlayerWeapon(playerid);
	if (wepid!=pTemp[playerid][lastWeaponHolded]) {
	    if (wepid>1 && (pTemp[playerid][wStrefieNODM] || pTemp[playerid][troll]) && pData[playerid][adminLevel]<LEVEL_GM && pState!=0 && pState!=9 && pState!=8 && pState!=7) {
	        SetPlayerArmedWeapon(playerid,0);
			return 0;
		}

		OnPlayerChangeWeapon(playerid,pTemp[playerid][lastWeaponHolded], wepid);
	}

	
	if (GetTickCount()%3==0) return 1;	// raz na 3 razy kontynuujemy

	if(pState == PLAYER_STATE_DRIVER || pState == PLAYER_STATE_PASSENGER)
		RefreshPlayerVehicleInfo(playerid);

	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid){
	new doors,bool:objective;
	doors=vehicleDoorState[vehicleid];

	if ((forplayerid == vehicleDoorOwner[vehicleid]) ||  (tVehicles[vehicleid][vo_private] && tVehicles[vehicleid][vo_owningPlayerId]==forplayerid ))
		doors=DOOR_OPENED;

	if (IsPlayerInAnyVehicle(forplayerid)) {
		new vid=GetPlayerVehicleID(forplayerid);
		if (vid!=vehicleid && GetVehicleModel(vid)==525 && !IsTrailerAttachedToVehicle(vid))	// towtruck
			if ((!tVehicles[vehicleid][vo_occupied] && tVehicles[vehicleid][vo_used] && !tVehicles[vehicleid][vo_private]) ||
				(tVehicles[vehicleid][vo_static] && !tVehicles[vehicleid][vo_used] && !tVehicles[vehicleid][vo_occupied] && 
				 random(10)==1) && GetVehicleFlags(vehicleid)&VF_TOWABLE==VF_TOWABLE)
				objective=true;	
	}

	SetVehicleParamsForPlayer(vehicleid, forplayerid, objective, doors);
	return 1;
}

/*public OnVehicleStreamIn(vehicleid, forplayerid)
{
	new
	 bool:_myHousesVehicle = false,
	 bool:_housesVehicle = false;

	for(new i = 0; i < MAX_HOUSES; i++)
	{
		if(vehicleid == hData[i][hVehicleID])
		{
			_housesVehicle = true;
			
			if(pData[forplayerid][loggedIn] && hData[i][hOwner] == pData[forplayerid][accountID])
			{
				_myHousesVehicle = true;
			}
			
			break;
		}
	}
	
	if((vehicleDoorState[vehicleid] == DOOR_CLOSED && forplayerid != vehicleDoorOwner[vehicleid]) || (_housesVehicle && !_myHousesVehicle))
	{
		SetVehicleParamsForPlayer(vehicleid, forplayerid, 0, DOOR_CLOSED);
	}
	else
	{
		SetVehicleParamsForPlayer(vehicleid, forplayerid, 0, DOOR_OPENED);
	}

	return 1;
}*/

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	printf("DialogResponse %d %d %d %d %s", playerid, dialogid, response, listitem, inputtext);
	if (pTemp[playerid][ept_dialogid]!=dialogid) {
		printf("Prawdopodobny hacking-attempt: otrzymano odpowiedz o innym dialogid niz oczekiwano.");
		return 1;
	}

	new
	 buffer[512];

	switch(dialogid)
	{
		case DIALOG_GANG..(DIALOG_GANG+9):
			return gangs_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		case DIALOG_TUNEMENU_MAIN..(DIALOG_TUNEMENU_MAIN+9):
			return vehicles_TunemenuResponse(playerid, dialogid, response, listitem, inputtext);
		case DIALOG_WARSZTAT..(DIALOG_WARSZTAT+9):
			return warsztat_DialogResponse(playerid, dialogid, response, listitem, inputtext);
		case DIALOG_DOMY..(DIALOG_DOMY+10):
			return domy_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		case DIALOG_POJAZDY..(DIALOG_POJAZDY+10):
			return vehicles_PojazdyResponse(playerid, dialogid, response, listitem, inputtext);
		case DIALOG_ARENASOLO_SELECT:
		{
			if (!response) return 1;
			else return solo_wybranaArena(playerid,listitem);
		}
		case DIALOG_CMDSEL:
		{
			if (!response || inputtext[0]!='/') return 1;
			new idx=1;
			new cmd[32];
			
			format(cmd,sizeof cmd,"cmd_%s", strtok(inputtext, idx));
			for (new i=0;i<strlen(cmd);i++)
				cmd[i]=tolower(cmd[i]);
			
//			new cmd[17];
//			if(sscanf(inputtext,"p< >s[16]",cmd)) return 1;
			CallLocalFunction(cmd,"d",playerid);
			return 1;
		}
		case DIALOG_WEAPON_QUICKBUY:
		{
			if (!response) return 1;
			else return PlayerQuickbuyWeapon(playerid,listitem);
		}
		case DIALOG_WEAPON_SELECT:
		{
			if(!response) return 1;
			new wepid,wname[24];
			if (!response || sscanf(inputtext,"ds[24]",wepid,wname)) CallLocalFunction(pTemp[playerid][dialogCB], "dd", playerid,-1);
			
			CallLocalFunction(pTemp[playerid][dialogCB], "dd", playerid,wepid);
			return 1;
		}
		case DIALOG_LANGUAGE:
		{
			pData[playerid][language] = response;
//			SetPlayerINIFileValue(playerid, "language", response);
			
			
			Msg(playerid, COLOR_INFO, TXT(playerid, 11));
			
//			ShowElement(playerid, TDE_HINT_CITYSELECT, true);
			ShowElement(playerid, TDE_WELCOMEBOX, true);
			InitPlayerCitySelecting(playerid);
		}

		case DIALOG_LOGIN:
		{
			if(response == BUTTON_OK)
			{
				if(pTemp[playerid][loginAttemps] >= MAX_LOGIN_ATTEMPS)
				{
					format(gstr, sizeof gstr, "%d blednych prob zalogowan na konto {b}%s{/b} z adresu IP {b}%s{/b}.", MAX_LOGIN_ATTEMPS, GetPlayerNick(playerid), GetPlayerIP(playerid));
					MSGToAdmins(COLOR_ERROR, gstr, false, LEVEL_ADMIN2);
					Msg(playerid, COLOR_ERROR, TXT(playerid, 10)); // Zbyt wiele pr�b logowa�, zosta�e� wyrzucony z serwera.
					KickPlayer(playerid);
					return 1;
				}
				new escaped_nick[MAX_PLAYER_NAME+16];
				mysql_real_escape_string(GetPlayerNick(playerid),escaped_nick);
				format(buffer, sizeof buffer, "SELECT password FROM %s WHERE nick = '%s'", gmData[DB_players], escaped_nick);
				mysql_query(buffer);
				
				mysql_store_result();
				mysql_fetch_row(buffer);
				mysql_free_result();
				
				if(strcmp(MD5_Hash(PassHash(GetPlayerNick(playerid), inputtext)), buffer, true) != 0 && strlen(buffer))
				{
					pTemp[playerid][loginAttemps]++;
					ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_INPUT, TXT(playerid, 51), TXT(playerid, 52), TXT(playerid, 53), TXT(playerid, 54));
					return Msg(playerid, COLOR_ERROR, TXT(playerid, 5)); // Nieprawid�owe has�o.
				}
			
				OnPlayerLogin(playerid);
				Msg(playerid, COLOR_INFO, TXT(playerid, 9)); // Zalogowano pomy�lnie, mi�ej gry!
				OnPlayerRequestClass(playerid, 0);

			}
			else
			{
				HidePlayerDialog(playerid);
				FlashScreen(playerid,true);	// zaslaniamy ekran (domyslnie bylo widac postac, gdzies zespawnowana)
				KickPlayer(playerid);
			}
		}
		
		case DIALOG_CONFIG_MAIN:
		{
			if(!IsAdmin(playerid)) {
			new buf[128];
			
			MSGToAdmins(COLOR_ERROR, "Prawdopodobny hacking attempt - proba obejscia autoryzacji na komende /config", true, LEVEL_ADMIN3);
			printf("Prawdopodobny hacking attempt - proba obejscia autoryzacji na komende /config");
			format(buf, sizeof buf, "Zwrocenie danych bezposrednio formularza, nick %s id %d", GetPlayerNick(playerid), playerid);
			MSGToAdmins(COLOR_ERROR, buf,true, LEVEL_ADMIN3);
			printf("%s", buf);
			return 1;
			}
			if(response == BUTTON_QUIT) return 1;
			switch(listitem)
			{
				case f_c_PCOLORS: ShowPlayerDialog(playerid, DIALOG_CONFIG_PCOLORS, DIALOG_STYLE_LIST, TXT(playerid, 95), GetDialogList(playerid, DIALOG_CONFIG_PCOLORS), TXT(playerid, 76), TXT(playerid, 77));
				case f_c_CCOLORS: ShowPlayerDialog(playerid, DIALOG_CONFIG_CCOLORS, DIALOG_STYLE_LIST, TXT(playerid, 96), GetDialogList(playerid, DIALOG_CONFIG_CCOLORS), TXT(playerid, 76), TXT(playerid, 77));
				case f_c_MAXPING: ShowPlayerDialog(playerid, DIALOG_CONFIG_MAXPING, DIALOG_STYLE_INPUT, TXT(playerid, 334), TXT(playerid, 79), TXT(playerid, 78), TXT(playerid, 77));
				case f_c_ASETTINGS: ShowPlayerDialog(playerid, DIALOG_CONFIG_ASETTINGS, DIALOG_STYLE_LIST, TXT(playerid, 337), GetDialogList(playerid, DIALOG_CONFIG_ASETTINGS), TXT(playerid, 78), TXT(playerid, 77));
				case f_c_CENSORSHIP: ShowPlayerDialog(playerid, DIALOG_CONFIG_CENSORSHIP, DIALOG_STYLE_LIST, TXT(playerid, 386), GetDialogList(playerid, DIALOG_CONFIG_CENSORSHIP), TXT(playerid, 78), TXT(playerid, 77));

				case f_c_CHATCOLORS:
				{
					if(gmData[chatColors])
					{
						gmData[chatColors] = false;
						Msg(playerid, COLOR_INFO, TXT(playerid, 401));
					}
					else
					{
						gmData[chatColors] = true;
						Msg(playerid, COLOR_INFO, TXT(playerid, 400));
					}
					
					SetConfigValueInt("chatcolors", BoolToInt(gmData[chatColors]));
					
					cmd_config(playerid);
				}
			}
		}
		
		case DIALOG_CONFIG_PCOLORS:
		{
			if(!IsAdmin(playerid)) return 1;
			if(response == BUTTON_BACK) return cmd_config(playerid);
			
			c_f_PlayerColors(playerid, listitem);
		}
		
		case DIALOG_CONFIG_CCOLORS:
		{
			if(!IsAdmin(playerid)) return 1;
			if(response == BUTTON_BACK) return cmd_config(playerid);
			
			c_f_ChatColors(playerid, listitem);
		}
		
		case DIALOG_CONFIG_MAXPING:
		{
			if(!IsAdmin(playerid)) return 1;

			if(response == BUTTON_BACK) return cmd_config(playerid);
			
			if(!IsNumeric(inputtext)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 122));
			
			new
			 _maxPing = strval(inputtext);
			
			if(_maxPing < 50 || _maxPing > 9999) return Msg(playerid, COLOR_ERROR, TXT(playerid, 335));
			
			gmData[maxPing] = _maxPing;
			
			SetConfigValueInt("max_ping", _maxPing);
			
			format(buffer, sizeof buffer, TXT(playerid, 336), _maxPing);
			Msg(playerid, COLOR_INFO, buffer);
			
			cmd_config(playerid);
		}
		
		case DIALOG_CONFIG_ASETTINGS:
		{
			if(!IsAdmin(playerid)) return 1;
			if(response == BUTTON_BACK) return cmd_config(playerid);
			
			pTemp[playerid][tmpConfigAttraction] = listitem;
			
			format(buffer, sizeof buffer, TXT(playerid, 338), aData[listitem][aName]);
			ShowPlayerDialog(playerid, DIALOG_CONFIG_ASETTINGS_LIST, DIALOG_STYLE_LIST, buffer, GetDialogList(playerid, DIALOG_CONFIG_ASETTINGS_LIST), TXT(playerid, 76), TXT(playerid, 77));
		}
		
		case DIALOG_CONFIG_CENSORSHIP:
		{
			if(!IsAdmin(playerid)) return 1;

			if(response == BUTTON_BACK) return cmd_config(playerid);
			
			switch(listitem)
			{
				case f_c_c_ENABLEDISABLE:
				{
					if(gmData[censorship]) gmData[censorship] = false;
					else gmData[censorship] = true;

					SetConfigValueInt("censorship", BoolToInt(gmData[censorship]));
					
					if(gmData[censorship]) Msg(playerid, COLOR_INFO, TXT(playerid, 390));
					else Msg(playerid, COLOR_INFO, TXT(playerid, 391));

					pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_MAIN;
					OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, f_c_CENSORSHIP, " ");
				}
				
				case f_c_c_LIST:
				{
					new
					 szList[1024],
					 File:hFile;
					
					hFile = fopen("FullServer/cenzura.ini", io_read);
					
					while(fread(hFile, buffer))
					{
						strcat(szList, buffer);
						strcat(szList, "\n");
					}
					
					fclose(hFile);
					
					strdel(szList, strlen(szList) - 1, strlen(szList));
					
					if(!strlen(szList))
					{
						strcat(szList, TXT(playerid, 392));
					}
					
					ShowPlayerDialog(playerid, DIALOG_CONFIG_CENSORSHIP_LIST, DIALOG_STYLE_LIST, TXT(playerid, 387), szList, TXT(playerid, 77), "");
				}
				
				case f_c_c_ADDWORD:
				{
					ShowPlayerDialog(playerid, DIALOG_CONFIG_CENSORSHIP_ADD, DIALOG_STYLE_INPUT, TXT(playerid, 388), TXT(playerid, 393), TXT(playerid, 78), TXT(playerid, 77));
				}
				
				case f_c_c_DELETEWORD:
				{
					new
					 szList[1024],
					 File:hFile;
					
					hFile = fopen("FullServer/cenzura.ini", io_read);
					
					while(fread(hFile, buffer))
					{
						strcat(szList, buffer);
						strcat(szList, "\n");
					}
					
					fclose(hFile);
					
					strdel(szList, strlen(szList) - 1, strlen(szList));
					
					if(!strlen(szList))
					{
						Msg(playerid, COLOR_ERROR, TXT(playerid, 394));
						SetTimerEx("NoMoreWordsDialogDelayFix", 50, false, "i", playerid);
					}
					else
					{
						ShowPlayerDialog(playerid, DIALOG_CONFIG_CENSORSHIP_DELETE, DIALOG_STYLE_LIST, TXT(playerid, 389), szList, TXT(playerid, 78), TXT(playerid, 77));
					}
				}
			}
		}
		
		case DIALOG_CONFIG_CENSORSHIP_LIST:
		{
			if(!IsAdmin(playerid)) return 1;
			pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_MAIN;
			OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, f_c_CENSORSHIP, " ");
		}
		
		case DIALOG_CONFIG_CENSORSHIP_ADD:
		{
			if(!IsAdmin(playerid)) return 1;

			if(response == BUTTON_BACK) {
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_MAIN;
				return OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, f_c_CENSORSHIP, " ");
			}
			
			if(!IsCorrectWordForCensorship(inputtext))
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 395));
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_CENSORSHIP;
				OnDialogResponse(playerid, DIALOG_CONFIG_CENSORSHIP, 1, f_c_c_ADDWORD, " ");
				
				return 1;
			}
			
			new
			 File:hFile;
			
			if(IsWordCensored(inputtext))
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 398));
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_CENSORSHIP;
				OnDialogResponse(playerid, DIALOG_CONFIG_CENSORSHIP, 1, f_c_c_ADDWORD, " ");
						
				return 1;
			}
			
			hFile = fopen("FullServer/cenzura.ini", io_append);
			
			format(buffer, sizeof buffer, "%s\n", inputtext);
			fwrite(hFile, buffer);
			
			fclose(hFile);
			
			format(buffer, sizeof buffer, TXT(playerid, 396), inputtext);
			Msg(playerid, COLOR_INFO, buffer);
			pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_CENSORSHIP;
			OnDialogResponse(playerid, DIALOG_CONFIG_CENSORSHIP, 1, f_c_c_ADDWORD, " ");
			
			LoadCensoredWords();
		}
		
		case DIALOG_CONFIG_CENSORSHIP_DELETE:
		{
			if(!IsAdmin(playerid)) return 1;
			if(response == BUTTON_BACK) {
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_MAIN;
				return OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, f_c_CENSORSHIP, " ");
			}
			
			if(!IsCorrectWordForCensorship(inputtext))
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 395));
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_CENSORSHIP;
				OnDialogResponse(playerid, DIALOG_CONFIG_CENSORSHIP, 1, f_c_c_DELETEWORD, " ");
				
				return 1;
			}
			
			new
			 File:hFile,
			 File:hFileTmp,
			 _count = 0;
					
			hFile = fopen("FullServer/cenzura.ini", io_read);
			hFileTmp = fopen("FullServer/cenzura.tmp", io_write);
					
			while(fread(hFile, buffer))
			{
				if(_count++ != listitem)
				{
					fwrite(hFileTmp, buffer);
				}
			}
			
			fclose(hFileTmp);
			fclose(hFile);
			
			hFile = fopen("FullServer/cenzura.ini", io_write);
			hFileTmp = fopen("FullServer/cenzura.tmp", io_read);
			
			while(fread(hFileTmp, buffer))
			{
				fwrite(hFile, buffer);
			}
			
			fclose(hFileTmp);
			fclose(hFile);
			fremove("FullServer/cenzura.tmp");
			
			format(buffer, sizeof buffer, TXT(playerid, 347), inputtext);
			Msg(playerid, COLOR_INFO, buffer);
			pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_CENSORSHIP;
			OnDialogResponse(playerid, DIALOG_CONFIG_CENSORSHIP, 1, f_c_c_DELETEWORD, " ");
			
			LoadCensoredWords();
		}
		
		case DIALOG_CONFIG_ASETTINGS_LIST:
		{
			if(!IsAdmin(playerid)) return 1;
		
			if(response == BUTTON_BACK) {
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_MAIN;
				return OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, f_c_ASETTINGS, " ");
			}
			
			switch(listitem)
			{
				case f_c_a_NAME:
				{
					format(buffer, sizeof buffer, TXT(playerid, 339), aData[listitem][aName]);
					ShowPlayerDialog(playerid, DIALOG_CONFIG_ASETTINGS_NAME, DIALOG_STYLE_INPUT, buffer, TXT(playerid, 79), TXT(playerid, 78), TXT(playerid, 77));
				}
				
				case f_c_a_QUEUE:
				{
					format(buffer, sizeof buffer, TXT(playerid, 340), aData[listitem][aName]);
					ShowPlayerDialog(playerid, DIALOG_CONFIG_ASETTINGS_QUEUE, DIALOG_STYLE_INPUT, buffer, TXT(playerid, 79), TXT(playerid, 78), TXT(playerid, 77));
				}
				
				case f_c_a_TIME:
				{
					format(buffer, sizeof buffer, TXT(playerid, 341), aData[listitem][aName]);
					ShowPlayerDialog(playerid, DIALOG_CONFIG_ASETTINGS_TIME, DIALOG_STYLE_INPUT, buffer, TXT(playerid, 79), TXT(playerid, 78), TXT(playerid, 77));
				}
			}
		}
		
		case DIALOG_CONFIG_ASETTINGS_NAME:
		{
			if(!IsAdmin(playerid)) return 1;
			if(response == BUTTON_BACK) {
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_ASETTINGS;
				return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS, 1, pTemp[playerid][tmpConfigAttraction], " ");
			}
			
			if(strlen(inputtext) < 1 || strlen(inputtext) > 14)
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 342)); // D�ugo�� nazwy musi si� mie�ci� w przedziale 1 - 14.
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_ASETTINGS_LIST;
				return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS_LIST, 1, f_c_a_NAME, " ");
			}
			
			if(!CheckTildes(inputtext))
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 205)); // Ilo�� tyld "~" musi by� parzysta.
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_ASETTINGS_LIST;
				return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS_LIST, 1, f_c_a_NAME, " ");
			}
			
			if(SpaceCheck(inputtext))
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 348)); // Nazwa atrakcji nie mo�e zawiera� spacji.
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_ASETTINGS_LIST;
				return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS_LIST, 1, f_c_a_NAME, " ");
			}
			
			copy(inputtext, aData[pTemp[playerid][tmpConfigAttraction]][aName]);
			SaveAttractionData(pTemp[playerid][tmpConfigAttraction]);
			
			format(buffer, sizeof buffer, TXT(playerid, 343), inputtext);
			Msg(playerid, COLOR_INFO, buffer); // Nazwa atrakcji zosta�a zmieniona na XXX.
			pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_ASETTINGS;
			OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS, 1, pTemp[playerid][tmpConfigAttraction], " ");
		}
		
		case DIALOG_CONFIG_ASETTINGS_QUEUE:
		{
			if(!IsAdmin(playerid)) return 1;
			if(response == BUTTON_BACK) {
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_ASETTINGS;
				return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS, 1, pTemp[playerid][tmpConfigAttraction], " ");
			}
			
			if(!IsNumeric(inputtext))
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 122)); // Podana warto�� musi by� numeryczna.
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_ASETTINGS_LIST;
				return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS_LIST, 1, f_c_a_QUEUE, " ");
			}
			
			new
			 _queue = strval(inputtext);
			
			if(_queue < GetAttractionMinimumQueueValue(pTemp[playerid][tmpConfigAttraction]) || _queue > GetAttractionMaximumQueueValue(pTemp[playerid][tmpConfigAttraction]))
			{
				format(buffer, sizeof buffer, TXT(playerid, 344), GetAttractionMinimumQueueValue(pTemp[playerid][tmpConfigAttraction]), GetAttractionMaximumQueueValue(pTemp[playerid][tmpConfigAttraction]));
				Msg(playerid, COLOR_ERROR, buffer); // Wielko�� kolejki nie mo�e by� mniejsza od xxx i wi�ksza ni� xxx.
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_ASETTINGS_LIST;
				return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS_LIST, 1, f_c_a_QUEUE, " ");
			}
			
			aData[pTemp[playerid][tmpConfigAttraction]][aStartPlayers] = _queue;
			SaveAttractionData(pTemp[playerid][tmpConfigAttraction]);
			
			format(buffer, sizeof buffer, TXT(playerid, 345), aData[pTemp[playerid][tmpConfigAttraction]][aName], _queue);
			Msg(playerid, COLOR_INFO, buffer); // Wielko�� kolejki XXX zosta�a zmieniona na XXX.
			pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_ASETTINGS;
			OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS, 1, pTemp[playerid][tmpConfigAttraction], " ");
		}
		
		case DIALOG_CONFIG_ASETTINGS_TIME:
		{
			if(!IsAdmin(playerid)) return 1;

			if(response == BUTTON_BACK) {
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_ASETTINGS;
				return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS, 1, pTemp[playerid][tmpConfigAttraction], " ");
			}
			
			if(!IsNumeric(inputtext))
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 122)); // Podana warto�� musi by� numeryczna.
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_ASETTINGS_LIST;
				return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS_LIST, 1, f_c_a_TIME, " ");
			}
			
			new
			 _time = strval(inputtext);
			
			if(_time < 1 || _time > 120)
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 397)); // Warto�� czasu musi si� mie�ci� w przedziale 1 - 120 (sekund).
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_ASETTINGS_LIST;
				return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS_LIST, 1, f_c_a_TIME, " ");
			}
			
			aData[pTemp[playerid][tmpConfigAttraction]][aStartingTime] = _time;
			SaveAttractionData(pTemp[playerid][tmpConfigAttraction]);
			
			format(buffer, sizeof buffer, TXT(playerid, 346), aData[pTemp[playerid][tmpConfigAttraction]][aName], _time);
			Msg(playerid, COLOR_INFO, buffer); // Czas startu XXX zosta� zmieniony na XXX.
			pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_ASETTINGS;
			OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS, 1, pTemp[playerid][tmpConfigAttraction], " ");
		}
		
		case DIALOG_CONFIG_PCOLORS_SET:
		{
			if(!IsAdmin(playerid)) return 1;
			if(response == BUTTON_BACK) {
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_MAIN;
				return OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, 0, " ");
			}
			
			if(!IsHex(inputtext))
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 14)); // Nieprawid�owa warto��, wprowad� kod koloru w systemie szesnastkowym RGB, np. 44FF2C.
				c_f_PlayerColors(playerid, pTemp[playerid][lastDialog]);
				return 1;
			}
			
			switch(pTemp[playerid][lastDialogItem])
			{
				case 0: gmData[color_normalUser] = HexToInt(inputtext);
				case 1: gmData[color_fsUser] = HexToInt(inputtext);
				case 2: gmData[color_vipUser] = HexToInt(inputtext);
				case 3: gmData[color_gmUser] = HexToInt(inputtext);
				case 4: gmData[color_lvl1User] = HexToInt(inputtext);
				case 5: gmData[color_lvl2User] = HexToInt(inputtext);
				case 6: gmData[color_adminUser] = HexToInt(inputtext);
			}
			
			SaveConfig();
			foreach(i) SetPlayerProperColor(i);
			
			format(buffer, sizeof buffer, TXT(playerid, 15), inputtext, inputtext);
			Msg(playerid, COLOR_INFO, buffer); // Kolor zosta� zmieniony na %s.
			pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_MAIN;
			OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, 0, " ");
		}
		
		case DIALOG_CONFIG_CCOLORS_SET:
		{
			if(!IsAdmin(playerid)) return 1;
		
			if(response == BUTTON_BACK) {
				pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_MAIN;
				return OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, 1, " ");
			}
			
			if(!IsHex(inputtext))
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 14)); // Nieprawid�owa warto��, wprowad� kod koloru w systemie szesnastkowym RGB, np. 44FF2C.
				c_f_ChatColors(playerid, pTemp[playerid][lastDialog]);
				return 1;
			}
			
			switch(pTemp[playerid][lastDialogItem])
			{
				// Normal
				case 0: gmData[color_chatInfo] = HexToInt(inputtext);
				case 1: gmData[color_chatInfo2] = HexToInt(inputtext);
				case 2: gmData[color_chatInfo3] = HexToInt(inputtext);
				case 3: gmData[color_chatError] = HexToInt(inputtext);
				case 4: gmData[color_joinInfo] = HexToInt(inputtext);
				case 5: gmData[color_leaveInfo] = HexToInt(inputtext);
				case 6: gmData[color_chatGM] = HexToInt(inputtext);
				case 7: gmData[color_chatAdmin] = HexToInt(inputtext);
				case 8: gmData[color_chatAdmin3] = HexToInt(inputtext);
				case 9: gmData[color_chatPM] = HexToInt(inputtext);
				case 10: gmData[color_chatIC] = HexToInt(inputtext);
				case 11: gmData[color_chatME] = HexToInt(inputtext);
				case 12: gmData[color_vipSay] = HexToInt(inputtext);
				case 13: gmData[color_chatVip] = HexToInt(inputtext);
				
				// Highlight
				case 14: gmData[color_chatInfo_HL] = HexToInt(inputtext);
				case 15: gmData[color_chatInfo2_HL] = HexToInt(inputtext);
				case 16: gmData[color_chatInfo3_HL] = HexToInt(inputtext);
				case 17: gmData[color_chatError_HL] = HexToInt(inputtext);
				case 18: gmData[color_joinInfo_HL] = HexToInt(inputtext);
				case 19: gmData[color_leaveInfo_HL] = HexToInt(inputtext);
				case 20: gmData[color_chatGM_HL] = HexToInt(inputtext);
				case 21: gmData[color_chatAdmin_HL] = HexToInt(inputtext);
				case 22: gmData[color_chatAdmin3_HL] = HexToInt(inputtext);
				case 23: gmData[color_chatPM_HL] = HexToInt(inputtext);
				case 24: gmData[color_chatIC_HL] = HexToInt(inputtext);
				case 25: gmData[color_chatME_HL] = HexToInt(inputtext);
				case 26: gmData[color_vipSay_HL] = HexToInt(inputtext);
				case 27: gmData[color_chatVip_HL] = HexToInt(inputtext);
			}
			
			SaveConfig();
			
			format(buffer, sizeof buffer, TXT(playerid, 15), inputtext, inputtext);
			Msg(playerid, COLOR_INFO, buffer); // Kolor zosta� zmieniony na %s.
			pTemp[playerid][ept_dialogid]=DIALOG_CONFIG_MAIN;
			OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, 1, " ");
		}
		
		case DIALOG_NICK_CHANGE:
		{
			if(response == BUTTON_QUIT) return 1;
			
			if(pTemp[playerid][loginAttemps] >= MAX_LOGIN_ATTEMPS)
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 59)); // Zbyt wiele pr�b potwierdzenia has�a, zosta�e� wyrzucony z serwera.
				KickPlayer(playerid);
				
				return 1;
			}
			
			if(strcmp(MD5_Hash(PassHash(GetPlayerNick(playerid), inputtext)), GetPlayerAccountData(playerid, "password"), true) != 0)
			{
				pTemp[playerid][loginAttemps]++;
				ShowPlayerDialog(playerid, DIALOG_NICK_CHANGE, DIALOG_STYLE_INPUT, TXT(playerid, 55), TXT(playerid, 56), TXT(playerid, 57), TXT(playerid, 58));
				return Msg(playerid, COLOR_ERROR, TXT(playerid, 5)); // Nieprawid�owe has�o.
			}
			
			new
			 oldNick[24];
			
			copy(GetPlayerNick(playerid), oldNick);
			
/*			if(IsFS(playerid) && strlen(pTemp[playerid][newNick]) <= 16)		// co to kurwa jest
			{
				strins(pTemp[playerid][newNick], "[FS]", 0);
			}
			else if(pData[playerid][gang] != NO_GANG && (strlen(gData[pData[playerid][gang]][name]) + strlen(pTemp[playerid][newNick])) <= 16)
			{
				format(buffer, sizeof buffer, "[%s]", gData[pData[playerid][gang]][name]);
				strins(pTemp[playerid][newNick], buffer, 0);
			}*/
			
			strcat(pTemp[playerid][newNick], pTemp[playerid][newNick]);
			SetPlayerName(playerid, pTemp[playerid][newNick]);
//			TextDrawSetString(pTextDraw[PTD_STAT_NICK][playerid], pTemp[playerid][newNick]);
//			AdjustNickTextDraw(playerid, strlen(pTemp[playerid][newNick]));
			
			SetPlayerAccountDataString(playerid, "nick", pTemp[playerid][newNick]);
			SetPlayerAccountDataString(playerid, "password", MD5_Hash(PassHash(pTemp[playerid][newNick], inputtext)));
			SetPlayerAccountDataString(playerid, "next_nick_change", "NOW() + INTERVAL 1 DAY", true);
			
			format(buffer, sizeof buffer, TXT(playerid, 18), pTemp[playerid][newNick]);
			Msg(playerid, COLOR_INFO, buffer); // Tw�j nick zosta� zmieniony na "xxx". Nast�pna zmiana b�dzie mo�liwa po 24 godzinach.
			
			foreach(i)
			{
				if(i != playerid)
				{
					format(buffer, sizeof buffer, TXT(i, 19), oldNick, pTemp[playerid][newNick]);
					Msg(i, COLOR_INFO, buffer); // "xxx" zmieni� sw�j nick na "xxx".
				}
			}
			format(buffer,sizeof buffer, "Zmiana nicku, accountid: %d, playerid:%d starynick:%s nowynick:%s ip:%s", pData[playerid][accountID], playerid, oldNick, pTemp[playerid][newNick], GetPlayerIP(playerid));
			OutputLog(LOG_PLAYERS,buffer);
			return 1;

		}
		
		case DIALOG_PASSWORD_CHANGE:
		{
			if(response == BUTTON_QUIT) return 1;
			
			if(pTemp[playerid][loginAttemps] >= MAX_LOGIN_ATTEMPS)
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 59)); // Zbyt wiele pr�b potwierdzenia has�a, zosta�e� wyrzucony z serwera.
				KickPlayer(playerid);
				
				return 1;
			}
				 
			if(strcmp(MD5_Hash(PassHash(GetPlayerNick(playerid), inputtext)), GetPlayerAccountData(playerid, "password"), true) != 0)
			{
				pTemp[playerid][loginAttemps]++;
				ShowPlayerDialog(playerid, DIALOG_PASSWORD_CHANGE, DIALOG_STYLE_INPUT, TXT(playerid, 349), TXT(playerid, 350), TXT(playerid, 57), TXT(playerid, 58));
				return Msg(playerid, COLOR_ERROR, TXT(playerid, 5)); // Nieprawid�owe has�o.
			}
			
			SetPlayerAccountDataString(playerid, "password", MD5_Hash(PassHash(GetPlayerNick(playerid), pTemp[playerid][newPassword])));
			
			format(buffer, sizeof buffer, TXT(playerid, 351), pTemp[playerid][newPassword]);
			Msg(playerid, COLOR_INFO, buffer); // Twoje has�o zosta�o zmienione na "xxx". Dobrze je zapami�taj!
			format(buffer,sizeof buffer, "Zmiana hasla, accountid: %d, playerid:%d loginattemps:%d ip:%s", pData[playerid][accountID], playerid, pTemp[playerid][loginAttemps], GetPlayerIP(playerid));
			OutputLog(LOG_PLAYERS,buffer);
			return 1;
		}
		
		case DIALOG_BAN_CONFIRM:
		{
			if(!IsAdmin(playerid)) return 1;

			if(response == BUTTON_QUIT) return 1;
			
			new
			 targetplayerid = 	pTemp[playerid][tmpTargetPlayerID],
			 banTime = 			pTemp[playerid][tmpBanTime],
			 period = 			pTemp[playerid][tmpPeriod],
			 reason[128], escaped_reason[140],
			 banAccountID,
			 szBanAccountName[24];

			copy(pTemp[playerid][tmpReason], reason);
			mysql_real_escape_string(reason,escaped_reason);
			
			if(targetplayerid == -1)
			{
				banAccountID = GetAccountID(pTemp[playerid][tmpBanAccountName]);
				copy(pTemp[playerid][tmpBanAccountName], szBanAccountName);
			}
			else
			{
				banAccountID = pData[targetplayerid][accountID];
				copy(GetPlayerProperName(targetplayerid), szBanAccountName);
			}
			
			format(buffer, sizeof buffer, "INSERT INTO %s (player_banned, player_given, date_created, date_end, reason) VALUES (%i, %i, NOW(), NOW() + INTERVAL %i %s, '%s')",
				gmData[DB_bans], 
				banAccountID,
				pData[playerid][accountID],
				banTime,
				GetMySQLNameOfPeriod(period),
				escaped_reason
			);
			
			mysql_query(buffer);
			
			if(targetplayerid != -1)
			{
				SetPlayerAccountDataString(targetplayerid, "ban_count", "ban_count + 1", true);
			}
			else
			{
				format(buffer, sizeof buffer, "UPDATE %s SET ban_count = ban_count + 1 WHERE id = %i", gmData[DB_players], banAccountID);
				mysql_query(buffer);
			}
			
			SetServerStatString("ban_count", "value + 1", true);
			
			format(buffer, sizeof buffer, "Gracz {ffffff}%s{ff0000} zostal zbanowany na {ff9090}%i %s.", szBanAccountName, banTime, GetPeriodName(playerid, period, banTime));
			SendClientMessageToAll(0xff0000ff, buffer);
			format(buffer, sizeof buffer, "Powod: {ff3030}%s", reason);
			SendClientMessageToAll(0xff0000ff, buffer);

			if (pData[playerid][adminLevel]!=LEVEL_ADMINHIDDEN && targetplayerid!=-1) {
				format(buffer, sizeof buffer, "Zbanowal%s {ff3030}%s", SkinKobiecy(GetPlayerSkin(playerid))?("a"):(""), GetPlayerNick(playerid));
				MSGToAdmins(COLOR_INFO2, buffer, true, LEVEL_GM);
				SendClientMessage(targetplayerid, 0xFF0000FF, buffer);
			}


			if(targetplayerid != -1)
			{
				SendClientMessage(targetplayerid,-1, " ");
				Msg(targetplayerid,COLOR_INFO,"Jesli uwazasz ze ten ban jest niesluszny, badz tez bedziesz staral sie o wczesniejsze jego zdjecie", false);
				Msg(targetplayerid,COLOR_INFO,"to koniecznie zrob {b}screenshot{/b} wciskajac teraz klawisz F8, nastepnie odwiedz nasze forum", false);
				Msg(targetplayerid,COLOR_INFO,"pod adresem {b}WWW.FULLSERVER.EU{/b} i zloz tam podanie o odbanowanie.");
				if (Audio_IsClientConnected(targetplayerid))
					Audio_Play(targetplayerid,AUDIOID_BAN, false, false, true);
				KickPlayer(targetplayerid);
			}
		}
		
		case DIALOG_BANIP_CONFIRM:
		{
			if(!IsAdmin(playerid)) return 1;

			if(response == BUTTON_QUIT) return 1;
			
			new
			 banTime = 			pTemp[playerid][tmpBanTime],
			 period = 			pTemp[playerid][tmpPeriod],
			 reason[128],
			 szIP[16];
			
			copy(pTemp[playerid][tmpReason], reason);
			copy(pTemp[playerid][tmpBanIP], szIP);
			
			for(new i = 0; i < strlen(szIP); i++)
			{
				if(szIP[i] == '*') szIP[i] = '%';
			}
			new escaped_reason[128];
			mysql_real_escape_string(reason,escaped_reason);
			format(buffer, sizeof buffer, "INSERT INTO %s (ip, player_given, date_created, date_end, reason) VALUES ('%s', %i, NOW(), NOW() + INTERVAL %i %s, '%s')",
				gmData[DB_ipbans], 
				szIP,
				pData[playerid][accountID],
				banTime,
				GetMySQLNameOfPeriod(period),
				escaped_reason
			);
			
			mysql_query(buffer);
			
			SetServerStatString("ban_count", "value + 1", true);
			
			format(buffer, sizeof buffer, TXT(playerid, 189), pTemp[playerid][tmpBanIP], banTime, GetPeriodName(playerid, period, banTime), reason);
			Msg(playerid, COLOR_INFO, buffer); // Adres IP "xxx" zosta� zbanowany na xxx xxx. Pow�d: xxx
		}
		
		case DIALOG_UNBAN_CONFIRM:
		{
			if(!IsAdmin(playerid)) return 1;

			if(response == BUTTON_QUIT) return 1;
			
			format(buffer, sizeof buffer, "DELETE FROM %s WHERE player_banned = %i",
				gmData[DB_bans], 
				GetAccountID(pTemp[playerid][tmpBanAccountName])
			);
			
			mysql_query(buffer);
			
			format(buffer, sizeof buffer, TXT(playerid, 184), pTemp[playerid][tmpBanAccountName]);
			Msg(playerid, COLOR_INFO, buffer); // Konto "xxx" zosta�o odbanowane.
		}
		
		case DIALOG_UNBANIP_CONFIRM:
		{
			if(!IsAdmin(playerid)) return 1;
			if(response == BUTTON_QUIT) return 1;
			
			new
			 szIP[16];
			
			copy(pTemp[playerid][tmpBanIP], szIP);
			
			for(new i = 0; i < strlen(szIP); i++)
			{
				if(szIP[i] == '*') szIP[i] = '%';
			}
			
			format(buffer, sizeof buffer, "DELETE FROM %s WHERE ip = '%s'",
				gmData[DB_ipbans], 
				szIP
			);
			
			mysql_query(buffer);
			
			format(buffer, sizeof buffer, TXT(playerid, 195), pTemp[playerid][tmpBanIP]);
			Msg(playerid, COLOR_INFO, buffer); // Adres IP "xxx" zosta� usuni�ty z listy zbanowanych.
		}
		
		case DIALOG_HELP_MAIN:
		{
			if(response == BUTTON_QUIT) return 1;
			
			switch(listitem)
	        {
	            case f_h_RULES: 	ShowPlayerDialog(playerid, DIALOG_HELP_RULES, DIALOG_STYLE_MSGBOX, TXT(playerid, 80), GetDialogContent(playerid, DIALOG_HELP_RULES), TXT(playerid, 54), TXT(playerid, 77));
				case f_h_RESPECT:	ShowPlayerDialog(playerid, DIALOG_HELP_RESPECT, DIALOG_STYLE_MSGBOX, TXT(playerid, 81), GetDialogContent(playerid, DIALOG_HELP_RESPECT), TXT(playerid, 54), TXT(playerid, 77));
				case f_h_STARS:		ShowPlayerDialog(playerid, DIALOG_HELP_STARS, DIALOG_STYLE_MSGBOX, TXT(playerid, 82), GetDialogContent(playerid, DIALOG_HELP_STARS), TXT(playerid, 54), TXT(playerid, 77));
				case f_h_VIP:		ShowPlayerDialog(playerid, DIALOG_HELP_VIP, DIALOG_STYLE_MSGBOX, TXT(playerid, 83), GetDialogContent(playerid, DIALOG_HELP_VIP), TXT(playerid, 54), TXT(playerid, 77));
				case f_h_AUTHOR:	ShowPlayerDialog(playerid, DIALOG_HELP_AUTHOR, DIALOG_STYLE_MSGBOX, TXT(playerid, 84), GetDialogContent(playerid, DIALOG_HELP_AUTHOR), TXT(playerid, 54), TXT(playerid, 77));
				case f_h_CMD:		ShowPlayerDialog(playerid, DIALOG_HELP_CMD, DIALOG_STYLE_LIST, TXT(playerid, 85), GetDialogList(playerid, DIALOG_HELP_CMD), TXT(playerid, 76), TXT(playerid, 77));
			}
		}
		
		case DIALOG_HELP_RULES..DIALOG_HELP_AUTHOR:
		{
			if(response == 1) return 1;
			else cmd_pomoc(playerid);
		}
		
		case DIALOG_HELP_CMD:
		{
			if(response == BUTTON_BACK) return cmd_pomoc(playerid);
			
			switch(listitem)
			{
				case f_h_CMD_GENERAL:		ShowPlayerDialog(playerid, DIALOG_HELP_CMD_GENERAL, DIALOG_STYLE_MSGBOX, TXT(playerid, 86), GetDialogContent(playerid, DIALOG_HELP_CMD_GENERAL), TXT(playerid, 54), TXT(playerid, 77));
				case f_h_CMD_ATRACTIONS:	ShowPlayerDialog(playerid, DIALOG_HELP_CMD_ATRACTIONS, DIALOG_STYLE_MSGBOX, TXT(playerid, 87), GetDialogContent(playerid, DIALOG_HELP_CMD_ATRACTIONS), TXT(playerid, 54), TXT(playerid, 77));
				case f_h_CMD_ACCOUNT:		ShowPlayerDialog(playerid, DIALOG_HELP_CMD_ACCOUNT, DIALOG_STYLE_MSGBOX, TXT(playerid, 88), GetDialogContent(playerid, DIALOG_HELP_CMD_ACCOUNT), TXT(playerid, 54), TXT(playerid, 77));
				case f_h_CMD_HOUSES:		ShowPlayerDialog(playerid, DIALOG_HELP_CMD_HOUSES, DIALOG_STYLE_MSGBOX, TXT(playerid, 89), GetDialogContent(playerid, DIALOG_HELP_CMD_HOUSES), TXT(playerid, 54), TXT(playerid, 77));
				case f_h_CMD_RESPECT:		return cmd_rcmd(playerid);// ShowPlayerDialog(playerid, DIALOG_HELP_CMD_RESPECT, DIALOG_STYLE_MSGBOX, TXT(playerid, 90), GetDialogContent(playerid, DIALOG_HELP_CMD_RESPECT), TXT(playerid, 54), TXT(playerid, 77));
				case f_h_CMD_TELEPORTS:		ShowPlayerDialog(playerid, DIALOG_HELP_CMD_TELEPORTS, DIALOG_STYLE_MSGBOX, TXT(playerid, 91), GetDialogContent(playerid, DIALOG_HELP_CMD_TELEPORTS), TXT(playerid, 54), TXT(playerid, 77));
				case f_h_CMD_ANIMATIONS:	return cmd_anims(playerid);//ShowPlayerDialog(playerid, DIALOG_HELP_CMD_ANIMATIONS, DIALOG_STYLE_MSGBOX, TXT(playerid, 92), GetDialogContent(playerid, DIALOG_HELP_CMD_ANIMATIONS), TXT(playerid, 54), TXT(playerid, 77));
				case f_h_CMD_VIP:			ShowPlayerDialog(playerid, DIALOG_HELP_CMD_VIP, DIALOG_STYLE_MSGBOX, TXT(playerid, 93), GetDialogContent(playerid, DIALOG_HELP_CMD_VIP), TXT(playerid, 54), TXT(playerid, 77));
//				case f_h_CMD_ADMIN:			ShowPlayerDialog(playerid, DIALOG_HELP_CMD_ADMIN, DIALOG_STYLE_MSGBOX, TXT(playerid, 94), GetDialogContent(playerid, DIALOG_HELP_CMD_ADMIN), TXT(playerid, 54), TXT(playerid, 77));
			}
		}
		
		case DIALOG_HELP_CMD_GENERAL..DIALOG_HELP_CMD_ADMIN:
		{
			if(response == 1) return 1;
			else {
				pTemp[playerid][ept_dialogid]=DIALOG_HELP_MAIN;
				OnDialogResponse(playerid, DIALOG_HELP_MAIN, BUTTON_NEXT, f_h_CMD, "");
			}
		}
		
		case DIALOG_BANK_MAIN:
		{
			if(response == BUTTON_QUIT) return 1;
			
			switch(listitem)
	        {
	            case f_b_BALANCE:
				{
					format(buffer, sizeof buffer, TXT(playerid, 117), StringToInt(GetPlayerAccountData(playerid, "bank_money")));
					ShowPlayerDialog(playerid, DIALOG_BANK_BALANCE, DIALOG_STYLE_MSGBOX, TXT(playerid, 111), buffer, TXT(playerid, 54), TXT(playerid, 77));
				}
				case f_b_DEPOSIT: ShowPlayerDialog(playerid, DIALOG_BANK_DEPOSIT_INPUT, DIALOG_STYLE_INPUT, TXT(playerid, 112), TXT(playerid, 118), TXT(playerid, 119), TXT(playerid, 77));
				case f_b_WITHDRAW: ShowPlayerDialog(playerid, DIALOG_BANK_WITHDRAW_INPUT, DIALOG_STYLE_INPUT, TXT(playerid, 113), TXT(playerid, 120), TXT(playerid, 119), TXT(playerid, 77));
				case f_b_TRANSFER: ShowPlayerDialog(playerid, DIALOG_BANK_TRANSFER_STEP1, DIALOG_STYLE_INPUT, TXT(playerid, 115), TXT(playerid, 121), TXT(playerid, 119), TXT(playerid, 77));
			}
		}
		
		case DIALOG_BANK_BALANCE:
		{
			if(response == 1) return 1;
			else cmd_bankomat(playerid);
		}
		
		case DIALOG_BANK_DEPOSIT_INPUT:
		{
			if(response == BUTTON_BACK) return cmd_bankomat(playerid);
			
			if(!IsNumeric(inputtext))
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 122));
				pTemp[playerid][ept_dialogid]=DIALOG_BANK_MAIN;
				return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_DEPOSIT, "");
			}
			
			new
			 amount = StringToInt(inputtext);
			
			if(amount < 0)
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 123));
				pTemp[playerid][ept_dialogid]=DIALOG_BANK_MAIN;
				return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_DEPOSIT, "");
			}
			
			if(amount > GetPlayerMoney(playerid))
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 124));
				pTemp[playerid][ept_dialogid]=DIALOG_BANK_MAIN;
				return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_DEPOSIT, "");
			}
			
			SetPlayerAccountDataInt(playerid, "bank_money", StringToInt(GetPlayerAccountData(playerid, "bank_money")) + amount);
			GivePlayerMoney(playerid, -amount);
			
			format(buffer, sizeof buffer, TXT(playerid, 125), amount);
			ShowPlayerDialog(playerid, DIALOG_BANK_DEPOSIT_INFO, DIALOG_STYLE_MSGBOX, TXT(playerid, 112), buffer, TXT(playerid, 54), TXT(playerid, 77));
		}
		
		case DIALOG_BANK_DEPOSIT_INFO:
		{
			if(response == 1) return 1;
			else cmd_bankomat(playerid);
		}
		
		case DIALOG_BANK_WITHDRAW_INPUT:
		{
			if(response == BUTTON_BACK) return cmd_bankomat(playerid);
			
			if(!IsNumeric(inputtext))
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 122));
				pTemp[playerid][ept_dialogid]=DIALOG_BANK_MAIN;
				return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_WITHDRAW, "");
			}
			
			new
			 amount = StringToInt(inputtext);
			
			if(amount < 0)
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 123));
				pTemp[playerid][ept_dialogid]=DIALOG_BANK_MAIN;
				return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_WITHDRAW, "");
			}
			
			new
			 balance = StringToInt(GetPlayerAccountData(playerid, "bank_money"));
			
			if(amount > balance)
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 126));
				pTemp[playerid][ept_dialogid]=DIALOG_BANK_MAIN;
				return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_WITHDRAW, "");
			}
			
			SetPlayerAccountDataInt(playerid, "bank_money", balance - amount);
			GivePlayerMoney(playerid, amount);
			
			format(buffer, sizeof buffer, TXT(playerid, 127), amount);
			ShowPlayerDialog(playerid, DIALOG_BANK_DEPOSIT_INFO, DIALOG_STYLE_MSGBOX, TXT(playerid, 112), buffer, TXT(playerid, 54), TXT(playerid, 77));
		}
		
		case DIALOG_BANK_WITHDRAW_INFO:
		{
			if(response == 1) return 1;
			else cmd_bankomat(playerid);
		}
		
		case DIALOG_BANK_TRANSFER_STEP1:
		{
			if(response == BUTTON_BACK) return cmd_bankomat(playerid);
			
			if(strlen(inputtext) < 3 || strlen(inputtext) > 24 || !IsNickCorrect(inputtext))
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 128));
				pTemp[playerid][ept_dialogid]=DIALOG_BANK_MAIN;
				return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_TRANSFER, "");
			}
			
			if(!AccountExists(inputtext))
			{
				format(buffer, sizeof buffer, TXT(playerid, 129), inputtext);
				Msg(playerid, COLOR_ERROR, buffer);
				pTemp[playerid][ept_dialogid]=DIALOG_BANK_MAIN;
				return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_TRANSFER, "");
			}
			
			copy(inputtext, pTemp[playerid][bankTransferAName]);
			pTemp[playerid][bankTransferAID] = GetAccountID(inputtext);
	
			ShowPlayerDialog(playerid, DIALOG_BANK_TRANSFER_STEP2, DIALOG_STYLE_INPUT, TXT(playerid, 116), TXT(playerid, 130), TXT(playerid, 119), TXT(playerid, 77));
		}
		
		case DIALOG_BANK_TRANSFER_STEP2:
		{
			if(response == BUTTON_BACK) {
				pTemp[playerid][ept_dialogid]=DIALOG_BANK_MAIN;
				return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_TRANSFER, "");
			}
		
			if(!IsNumeric(inputtext))
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 122));
				pTemp[playerid][ept_dialogid]=DIALOG_BANK_TRANSFER_STEP1;
				return OnDialogResponse(playerid, DIALOG_BANK_TRANSFER_STEP1, BUTTON_NEXT, 0, pTemp[playerid][bankTransferAName]);
			}
			
			new
			 amount = StringToInt(inputtext);
			
			if(amount < 0)
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 123));
				pTemp[playerid][ept_dialogid]=DIALOG_BANK_TRANSFER_STEP1;
				return OnDialogResponse(playerid, DIALOG_BANK_TRANSFER_STEP1, BUTTON_NEXT, 0, pTemp[playerid][bankTransferAName]);
			}
			
			new
			 balance = StringToInt(GetPlayerAccountData(playerid, "bank_money"));
			
			if(amount > balance)
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 126));
				pTemp[playerid][ept_dialogid]=DIALOG_BANK_TRANSFER_STEP1;
				return OnDialogResponse(playerid, DIALOG_BANK_TRANSFER_STEP1, BUTTON_NEXT, 0, pTemp[playerid][bankTransferAName]);
			}
			
			pTemp[playerid][bankTransferAmount] = amount;
			
			format(buffer, sizeof buffer, TXT(playerid, 131), amount, pTemp[playerid][bankTransferAName]);
			ShowPlayerDialog(playerid, DIALOG_BANK_TRANSFER_STEP3, DIALOG_STYLE_MSGBOX, TXT(playerid, 114), buffer, TXT(playerid, 62), TXT(playerid, 63));
		}
		
		case DIALOG_BANK_TRANSFER_STEP3:
		{
			if(response == BUTTON_NO) return cmd_bankomat(playerid);
			
			SetPlayerAccountDataInt(pTemp[playerid][bankTransferAID], "bank_money", StringToInt(GetPlayerAccountData(pTemp[playerid][bankTransferAID], "bank_money", true)) + pTemp[playerid][bankTransferAmount], true);
			SetPlayerAccountDataInt(playerid, "bank_money", StringToInt(GetPlayerAccountData(playerid, "bank_money")) - pTemp[playerid][bankTransferAmount]);
			
			format(buffer, sizeof buffer, TXT(playerid, 132), pTemp[playerid][bankTransferAmount], pTemp[playerid][bankTransferAName]);
			ShowPlayerDialog(playerid, DIALOG_BANK_TRANSFER_INFO, DIALOG_STYLE_MSGBOX, TXT(playerid, 114), buffer, TXT(playerid, 54), TXT(playerid, 77));
		}
		
		case DIALOG_BANK_TRANSFER_INFO:
		{
			if(response == 1) return 1;
			else cmd_bankomat(playerid);
		}
		
		case DIALOG_KICK_CONFIRM:
		{
			if(!IsAdmin(playerid)) return 1;
			if(response == BUTTON_QUIT) return 1;
			
			new
			 targetplayerid = pTemp[playerid][tmpTargetPlayerID],
			 reason[128];
			
			SetPlayerAccountDataString(targetplayerid, "kick_count", "kick_count + 1", true);
			SetServerStatString("kick_count", "value + 1", true);
			
			copy(pTemp[playerid][tmpReason], reason);

			return KickPlayerWithReason(targetplayerid, playerid, reason);
			
/*			format(buffer, sizeof buffer, TXT(playerid, 152), GetPlayerNick(targetplayerid), reason);
			Msg(playerid, COLOR_INFO, buffer); // Gracz "xxx" zosta� wyrzucony z serwera, pow�d: xxx

			format(buffer, sizeof buffer, TXT(targetplayerid, 153), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
			Msg(targetplayerid, COLOR_INFO2, buffer); // Zosta�e� wyrzucony z serwera przez admina "xxx".

			format(buffer, sizeof buffer, TXT(targetplayerid, 50), reason);
			Msg(targetplayerid, COLOR_INFO2, buffer); // Pow�d: xxx
			
			foreach(i)
			{
				if(i == targetplayerid || i == playerid) continue;
				
				format(buffer, sizeof buffer, TXT(i, 232), GetPlayerNick(targetplayerid), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), reason);
				Msg(i, COLOR_INFO2, buffer); // Gracz "xxx" zosta� wyrzucony z serwera przez admina "xxx" z powodu: xxx
			}
			if (Audio_IsClientConnected(targetplayerid))
				Audio_Play(targetplayerid,AUDIOID_KICK, false, false, true);
			
			KickPlayer(targetplayerid);*/
		}
		
		case DIALOG_STATS_MAIN:
		{
			if(response == BUTTON_QUIT) return 1;
			
			switch(listitem)
	        {
				case f_s_PLAYERSTATS:
				{
					new
					 timePlayed = StringToInt(GetPlayerAccountData(playerid, "session")) + ((GetTickCount() - pTemp[playerid][lastSessionSaveTick]) / 1000),
					 period;
					
					GetOptimalTimeUnit(timePlayed, period);
					
					format(buffer, sizeof buffer, GetDialogContent(playerid, DIALOG_STATS_PLAYER),
					 GetPlayerAccountData(playerid, "datetime_registered"),
					 pData[playerid][respect],
					 pData[playerid][kills],
					 pData[playerid][teamkills],
					 pData[playerid][deaths],
					 pData[playerid][suicides],
					 GetPlayerAccountData(playerid, "kick_count"),
					 GetPlayerAccountData(playerid, "ban_count"),
					 pData[playerid][averagePing],
					 timePlayed,
					 GetPeriodName2(playerid, period, timePlayed)
					);
					
					ShowPlayerDialog(playerid, DIALOG_STATS_PLAYER, DIALOG_STYLE_MSGBOX, TXT(playerid, 155), buffer, TXT(playerid, 54), TXT(playerid, 77));
				}
				
				case f_s_SERVERSTATS:
				{
					new
					 registeredPlayers;
					
					format(buffer, sizeof buffer, "SELECT count(id) FROM %s", gmData[DB_players]);
					mysql_query(buffer);
					mysql_store_result();
					if (mysql_fetch_row(buffer))
						registeredPlayers = StringToInt(buffer);
					if (mysql_result_stored()) mysql_free_result();
				
					format(buffer, sizeof buffer, GetDialogContent(playerid, DIALOG_STATS_SERVER),
					 GetServerStat("most_online"),
					 GetServerStat("most_online_date"),
					 registeredPlayers,
					 GetServerStat("join_count"),
					 GetServerStat("kill_count"),
					 GetServerStat("death_count"),
					 GetServerStat("kick_count"),
					 GetServerStat("ban_count")
					);
					
					ShowPlayerDialog(playerid, DIALOG_STATS_SERVER, DIALOG_STYLE_MSGBOX, TXT(playerid, 156), buffer, TXT(playerid, 54), TXT(playerid, 77));
				}
				
				case f_s_GANGSTATS:
				{
					gangs_ShowGangsList(playerid);
				}
			}
		}
		case DIALOG_STYLWALKI: {
			if (!response) return 1;
			switch(listitem)
			{
			    case 0:
			    {
					SetPlayerFightingStyle(playerid,4);
					Msg(playerid, COLOR_INFO, "Wybra�e�(a�) {b}normalny{/b} styl walki.");
				}
				case 1:
				{
					SetPlayerFightingStyle(playerid,6);
					Msg(playerid, COLOR_INFO, "Wybra�e�(a�) styl walki {b}karate{/b}.");
				}
				case 2:
				{
					SetPlayerFightingStyle(playerid,5);
					Msg(playerid, COLOR_INFO, "Wybra�e�(a�) styl walki {b}boxera{/b}.");
				}
				case 3:
				{
					SetPlayerFightingStyle(playerid,7);
					Msg(playerid, COLOR_INFO, "Wybra�e�(a�) styl walki {b}gangstera{/b}.");
				}
				case 4:
				{
					if (random(2)==1)
						SetPlayerFightingStyle(playerid,15);
					else
						SetPlayerFightingStyle(playerid,26);
					Msg(playerid, COLOR_INFO, "Wybra�e�(a�) styl walki {b}pijanej malpy{/b}.");
				}
			}
			return 1;
		}

		case DIALOG_STATS_GANG_LIST:
		{
			if(response == BUTTON_BACK) return cmd_staty(playerid);
/*			
			format(buffer, sizeof buffer, "{A9C4E4}%s: {FFFFFF}%s \t\t {A9C4E4}%s: {FFFFFF}%s\n{A9C4E4}%s: {FFFFFF}%s\n{A9C4E4}%s: {FFFFFF}%s\n{A9C4E4}%s: {FFFFFF}%s\n",
				TXT(playerid, 234),
				GetGangData(pTemp[playerid][statsGangListID][listitem], "name"),
				TXT(playerid, 235),
				GetGangData(pTemp[playerid][statsGangListID][listitem], "tag"),
				TXT(playerid, 236),
				GetGangOwner(pTemp[playerid][statsGangListID][listitem]),
				TXT(playerid, 237),
				GetGangData(pTemp[playerid][statsGangListID][listitem], "respect"),
				
				TXT(playerid, 238),
				GetGangData(pTemp[playerid][statsGangListID][listitem], "DATE(datetime_created)")
			);
			
			new
			 szTitle[32];
			
			format(szTitle, sizeof szTitle, TXT(playerid, 239), GetGangData(pTemp[playerid][statsGangListID][listitem], "tag"));
			
			ShowPlayerDialog(playerid, DIALOG_STATS_GANG_INFO, DIALOG_STYLE_MSGBOX, szTitle, buffer, TXT(playerid, 54), TXT(playerid, 77));*/
			return gangs_ShowGangDetails(playerid,pTemp[playerid][statsGangListID][listitem]);
		}
		
		case DIALOG_STATS_PLAYER, DIALOG_STATS_SERVER:
		{
			if(response == 1) return 1;
			else cmd_staty(playerid);
		}
		
		case DIALOG_STATS_GANG_INFO:
		{
			pTemp[playerid][ept_dialogid]=DIALOG_STATS_MAIN;
			if(response == BUTTON_BACK) OnDialogResponse(playerid, DIALOG_STATS_MAIN, BUTTON_NEXT, f_s_GANGSTATS, "");
		}
		
		case DIALOG_TOP_MAIN:
		{
			if(response == BUTTON_QUIT) return 1;
			
			switch(listitem)
	        {
				case f_t_RESPECT:
				{
					new
					 szTopList[512],
					 szNick[24],
					 szRespect[12],
					 i = 1;
					
					format(buffer, sizeof buffer, "SELECT nick, respect FROM %s ORDER BY `respect` DESC LIMIT 10", gmData[DB_players]);
					mysql_query(buffer);
					mysql_store_result();
					
					while(mysql_fetch_row_data())
					{
						mysql_fetch_field("nick", szNick);
						mysql_fetch_field("respect", szRespect);
						
						format(buffer, sizeof buffer, "{A9C4E4}%2i. %6s {EFE58D}%s", i, szRespect, szNick);
						strcat(szTopList, buffer);
						
						if(i < 10) strcat(szTopList, "\n");
						i++;
					}
					
					if (mysql_result_stored()) mysql_free_result();
					
					ShowPlayerDialog(playerid, DIALOG_TOP_RESPECT, DIALOG_STYLE_MSGBOX, TXT(playerid, 158), szTopList, TXT(playerid, 54), TXT(playerid, 77));
				}
				
				case f_t_SKILL:
				{
					new
					 szTopList[512],
					 szNick[24],
					 szSkill[12],
					 i = 1;
					
					format(buffer, sizeof buffer, "SELECT nick, skill FROM %s ORDER BY skill DESC LIMIT 10", gmData[DB_players]);
					mysql_query(buffer);
					mysql_store_result();
					
					while(mysql_fetch_row_data())
					{
						mysql_fetch_field("nick", szNick);
						mysql_fetch_field("skill", szSkill);
						
						format(buffer, sizeof buffer, "{A9C4E4}%2i. {EFE58D}%s  {FFFFFF}%s", i, szNick, szSkill);
						strcat(szTopList, buffer);
						
						if(i < 10) strcat(szTopList, "\n");
						i++;
					}
					
					if (mysql_result_stored()) mysql_free_result();
					
					ShowPlayerDialog(playerid, DIALOG_TOP_RESPECT, DIALOG_STYLE_MSGBOX, "Top lista > Skill", szTopList, TXT(playerid, 54), TXT(playerid, 77));
				}
				
				case f_t_FRAGS:
				{
					new
					 szTopList[512],
					 szNick[24],
					 szKills[12],
					 i = 1;
					
					format(buffer, sizeof buffer, "SELECT nick, kill_count FROM %s ORDER BY `kill_count` DESC LIMIT 10", gmData[DB_players]);
					mysql_query(buffer);
					mysql_store_result();
					
					while(mysql_fetch_row_data())
					{
						mysql_fetch_field("nick", szNick);
						mysql_fetch_field("kill_count", szKills);
						
						format(buffer, sizeof buffer, "{A9C4E4}%2i. %6s {FFFFFF}%s", i, szKills, szNick);
						strcat(szTopList, buffer);
						
						if(i < 10) strcat(szTopList, "\n");
						i++;
					}
					
					if (mysql_result_stored()) mysql_free_result();
					
					ShowPlayerDialog(playerid, DIALOG_TOP_FRAGS, DIALOG_STYLE_MSGBOX, TXT(playerid, 159), szTopList, TXT(playerid, 54), TXT(playerid, 77));
				}
				
				case f_t_MONEY:
				{
					new
					 szTopList[512],
					 szNick[24],
					 szMoney[12],
					 szWallet[12],
					 i = 1;
					
					format(buffer, sizeof buffer, "SELECT nick, bank_money, wallet_money FROM %s ORDER BY `bank_money`+`wallet_money` DESC LIMIT 10", gmData[DB_players]);
					mysql_query(buffer);
					mysql_store_result();
					
					while(mysql_fetch_row_data())
					{
						mysql_fetch_field("nick", szNick);
						mysql_fetch_field("bank_money", szMoney);
						mysql_fetch_field("wallet_money", szWallet);
						
						format(buffer, sizeof buffer, "{A9C4E4}%2i. %06d {FFFFFF}%s", i, StringToInt(szMoney) + StringToInt(szWallet), szNick);
						strcat(szTopList, buffer);
						
						if(i < 10) strcat(szTopList, "\n");
						i++;
					}
					
					if (mysql_result_stored()) mysql_free_result();
					
					ShowPlayerDialog(playerid, DIALOG_TOP_MONEY, DIALOG_STYLE_MSGBOX, TXT(playerid, 199), szTopList, TXT(playerid, 54), TXT(playerid, 77));
				}
				
				case f_t_PLAYEDTIME:
				{
					new
					 szTopList[512],
					 szNick[24],
					 _session,
					 _period,
					 i = 1;
					
					format(buffer, sizeof buffer, "SELECT nick, session FROM %s ORDER BY `session` DESC LIMIT 10", gmData[DB_players]);
					mysql_query(buffer);
					mysql_store_result();
					
					while(mysql_fetch_row_data())
					{
						mysql_fetch_field("nick", szNick);
						mysql_fetch_field("session", buffer);
						_session = StringToInt(buffer);
						
						GetOptimalTimeUnit(_session, _period);
						
						format(buffer, sizeof buffer, "{A9C4E4}%2i. %i %s {EFE58D}%s", i, _session, GetPeriodName2(playerid, _period, _session),  szNick);
						strcat(szTopList, buffer);
						
						if(i < 10) strcat(szTopList, "\n");
						i++;
					}
					
					if (mysql_result_stored()) mysql_free_result();
					
					ShowPlayerDialog(playerid, DIALOG_TOP_TIMEPLAYED, DIALOG_STYLE_MSGBOX, TXT(playerid, 160), szTopList, TXT(playerid, 54), TXT(playerid, 77));
				}
				
				case f_t_GANGS:
				{
					new
					 szTopList[512],
					 szGangName[8],
					 szGangRespect[12],
					 i = 1;
					
					format(buffer, sizeof buffer, "SELECT tag, respect FROM %s ORDER BY respect DESC LIMIT 10", gmData[DB_gangs]);
					mysql_query(buffer);
					mysql_store_result();
					
					while(mysql_fetch_row_data())
					{
						mysql_fetch_field("tag", szGangName);
						mysql_fetch_field("respect", szGangRespect);
						
						format(buffer, sizeof buffer, "{A9C4E4}%2i. %5s {EFE58D}%s", i, szGangRespect, szGangName);
						strcat(szTopList, buffer);
						
						if(i < 10) strcat(szTopList, "\n");
						i++;
					}
					
					if (mysql_result_stored()) mysql_free_result();
					
					ShowPlayerDialog(playerid, DIALOG_TOP_GANGS, DIALOG_STYLE_MSGBOX, TXT(playerid, 240), szTopList, TXT(playerid, 54), TXT(playerid, 77));
				}
			}
		}
		
		case DIALOG_TOP_RESPECT..DIALOG_TOP_TIMEPLAYED, DIALOG_TOP_MONEY, DIALOG_TOP_GANGS:
		{
			if(response == 1) return 1;
			else cmd_top(playerid);
		}
		
		case DIALOG_HUD:
		{
			if(response == BUTTON_QUIT) return 1;
			if (listitem==0) {
				for(new i=0; i<MAX_HUD_ELEMENTS; i++) {
					pData[playerid][hudSetting][i] = !pData[playerid][hudSetting][i];
					ShowPlayerHudElement(playerid, i, pData[playerid][hudSetting][i]);

				}
				Msg(playerid,COLOR_INFO,"Wszystkie ustawienia HUD przelaczone.");
			} else {
				listitem--;

				pData[playerid][hudSetting][listitem] = !pData[playerid][hudSetting][listitem];
				ShowPlayerHudElement(playerid, listitem, pData[playerid][hudSetting][listitem]);
			
				if(pData[playerid][hudSetting][listitem])
					format(buffer, sizeof buffer, TXT(playerid, 331), (pData[playerid][language] == LANG_POLISH) ? PL_fNames_hud[listitem] : EN_fNames_hud[listitem]);
				else
					format(buffer, sizeof buffer, TXT(playerid, 332), (pData[playerid][language] == LANG_POLISH) ? PL_fNames_hud[listitem] : EN_fNames_hud[listitem]);
				Msg(playerid, COLOR_INFO, buffer);
			}

			return cmd_hud(playerid);
		}
		
		case DIALOG_REPORTS:
		{
			if(response == BUTTON_QUIT) return 1;
			
			new
			 reportIndex = pTemp[playerid][reportSync][listitem];
			
			pTemp[playerid][reportTmpPlayerId] = gReports[reportIndex][rPlayerId];
			
			format(buffer, sizeof buffer, "Gracz: %s\nID: %i\nPow�d: %s\nZlozone przez: %s", gReports[reportIndex][rPlayerName], gReports[reportIndex][rPlayerId], gReports[reportIndex][rReason], gReports[reportIndex][rReportingPlayerName]);
			ShowPlayerDialog(playerid, DIALOG_REPORTS_MANAGE, DIALOG_STYLE_MSGBOX, TXT(playerid, 406), buffer, TXT(playerid, 409), TXT(playerid, 410));
		}
		
		case DIALOG_REPORTS_MANAGE:
		{
			if(!IsPlayerConnected(pTemp[playerid][reportTmpPlayerId])) return 1;
			
			if(response == 1) // Spec
			{
				if(!SpecAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 411)); // Nie mo�esz w tym momencie obserwowa� graczy.
				
				StartSpectating(playerid, pTemp[playerid][reportTmpPlayerId]);
				
				format(buffer, sizeof buffer, TXT(playerid, 107), GetPlayerNick(pTemp[playerid][reportTmpPlayerId]), pTemp[playerid][reportTmpPlayerId]);
				Msg(playerid, COLOR_INFO, buffer); // Obserwujesz gracza xxx (xxx).
			}
			else // Usu�
			{
				format(buffer, sizeof buffer, TXT(playerid, 412), GetPlayerNick(pTemp[playerid][reportTmpPlayerId]));
				Msg(playerid, COLOR_INFO, buffer); // Gracz xxx zosta� usuni�ty z listy raport�w.
			}
			
			RemovePlayerFromReportList(pTemp[playerid][reportTmpPlayerId]);
		}
	}

	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	if (pData[playerid][logonDialog])
		return 0;
	return ShowPlayerInfo(playerid,clickedplayerid);
}

public OnMysqlError(error[], errorid, MySQL:handle)
{
	if(errorid == 8) return;
	
	new
	 buffer[256];
	 
	format(buffer, sizeof buffer, "Blad MySQL: %s (ID: %i)", error, errorid);
	OutputLog(LOG_SYSTEM, buffer);
}

//OnPlayerShipBonus(playerid)
//{
//	GivePlayerMoney(playerid, 1000);
//}

public OnPlayerCommandReceived(playerid, cmdtext[])
{
	pTemp[playerid][lastPos]=-1;

	if(pData[playerid][jail]>0 && pData[playerid][adminLevel]<LEVEL_GM && pData[playerid][allowedLevel]<LEVEL_GM && strfind(cmdtext,"raport",true)!=0) {
		Msg(playerid, COLOR_ERROR, "Niestety - siedzisz w paczce.");
		return 0;
	}
	if(pTemp[playerid][cenzura] && pData[playerid][adminLevel]<4){
		Msg(playerid, COLOR_ERROR, "{b}Jestes ocenzurowany/-a{/b}! Nie mozesz wpisywac komend.");
		return 0;
	}
	if(pData[playerid][logonDialog] || pData[playerid][classSelecting]) {
		Msg(playerid,COLOR_ERROR,"Najpierw musisz dolaczyc do gry.");
		return 0;
	}



	return 1;
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
	if(pData[playerid][adminLevel]<LEVEL_ADMIN2 && (GetTickCount() - pTemp[playerid][lastMsgTick] < 1000))
	{
		pTemp[playerid][spamCount]++;
		
		if(pTemp[playerid][spamCount] >= 10)
		{
			Msg(playerid, COLOR_ERROR, TXT(playerid, 141));
			KickPlayer(playerid, true);
			
			return 1;
		}
		
		if(pTemp[playerid][spamCount] >= 2)
		{
			Msg(playerid, COLOR_ERROR, TXT(playerid, 140));
			pTemp[playerid][lastMsgTick] = GetTickCount();
			
			return 1;
		}
	}
	else
	{
		pTemp[playerid][spamCount] = 0;
	}
	
	pTemp[playerid][lastMsgTick] = GetTickCount();
	
	if(success)
	{
		new
		 buffer[255],
		 szName[24];
		
		GetPlayerName(playerid, szName, sizeof szName);
		
		format(buffer, sizeof buffer, "%s (%i): %s", szName, playerid, cmdtext);
		OutputLog(LOG_COMMANDS, buffer);
		
		return 1;
	}
	else
	{
		Msg(playerid, COLOR_ERROR, TXT(playerid, 8));
		return 1;
	}
}

OnPlayerRCONLogin(playerid)
{
	new
	 buffer[128];
	
	if (!pData[playerid][loggedIn] || pData[playerid][allowedLevel]<LEVEL_ADMIN3) {
		format(buffer,sizeof buffer,"Nieautoryzowane logowanie na admina RCON przez %s (%d)! Poziom %d/%d Wykopany.", GetPlayerNick(playerid), playerid, pData[playerid][adminLevel], pData[playerid][allowedLevel]);
		KickPlayer(playerid,false);
		foreach(i)
			if(IsAdmin(i, LEVEL_ADMIN1))
				Msg(i, COLOR_ERROR, buffer);
		OutputLog(LOG_SYSTEM, buffer);
	} else {
		pData[playerid][adminLevel]=pData[playerid][allowedLevel];
		if (pData[playerid][adminLevel]<LEVEL_ADMINHIDDEN)
			foreach(i) {
				format(buffer, sizeof buffer, TXT(i, 147), GetPlayerNick(playerid), pData[playerid][adminLevel] - 1);
				Msg(i, COLOR_INFO2, buffer, false);
				PlaySound(i, 1133);
			}
	}
	
	SetPlayerProperColor(playerid);
	UpdatePlayerNickTD(playerid);
}

OnPlayerFirstSpawn(playerid)
{
	DisablePlayerSounds(playerid);
	SetPlayerVirtualWorld(playerid,0);
	PlaySound(playerid, 1058);


	ShowElement(playerid, TDE_WIDE, false);
	ShowElement(playerid, TDE_CITYSELECTPLAYERSLS, false);
	ShowElement(playerid, TDE_CITYSELECTPLAYERSSF, false);
	ShowElement(playerid, TDE_CITYSELECTPLAYERSLV, false);
	ShowElement(playerid, TDE_CITYSELECTLS, false);
	ShowElement(playerid, TDE_CITYSELECTSF, false);
	ShowElement(playerid, TDE_CITYSELECTLV, false);
	ShowElement(playerid, TDE_WELCOMEBOX, false);

//	for(new i=0; i<MAX_HUD_ELEMENTS;i++)
//		printf("HUD %d L:%d %d: %d", playerid, pData[playerid][loggedIn], i, pData[playerid][hudSetting][i]);

			
	if(pData[playerid][hudSetting][HUD_DATE]) ShowElement(playerid, TDE_DATETIME, true);
	if(pData[playerid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(playerid, TDE_ATTRACTIONBOX, true);
	if(pData[playerid][hudSetting][HUD_STATUSBAR]) ShowElement(playerid, TDE_STATS, true);
	if(pData[playerid][hudSetting][HUD_SERVERLOGO]) ShowElement(playerid, TDE_FULLSERVERLOGO, true);
//	if(pData[playerid][hudSetting][HUD_FPS]) ShowElement(playerid, TDE_FPS, true);

	if((pData[playerid][jail]/60) > 0)
	{
		JailPlayer(playerid, 1); //floatround(pData[playerid][jail]/60));
		Msg(playerid, COLOR_INFO2, "Ostatnio uciekles z wiezienia... Ta minute musisz odsiedziec!");//TXT(playerid, 353));	// ostatnio uciekles z wiezienia
	}
	return 1;

}

public OnPlayerEnterDynamicArea(playerid,areaid){
//	printf("EnterDynamicArea %d %d", playerid, areaid);
    if (prezenty_OPEnterDynamicArea(playerid,areaid)) return 1;
	if (obiekty_OPEnterDynamicArea(playerid,areaid)) return 1;
    return 0;
}

public OnPlayerLeaveDynamicArea(playerid,areaid){
    if (prezenty_OPLeaveDynamicArea(playerid,areaid)) return 1;
	if (obiekty_OPLeaveDynamicArea(playerid,areaid)) return 1;
    return 0;
}

public OnPlayerEnterDynamicCP(playerid,checkpointid){
	if (obiekty_OPEnterDynamicCP(playerid,checkpointid)) return 1;
	if (domy_OPEnterDynamicCP(playerid,checkpointid)) return 1;
	return 0;
}

public OnMysqlQuery(resultid,spareid,MySQL:handle){
//	printf("OMQ: %d %d %d, stored: ",resultid, spareid, _:handle, mysql_result_stored());
	switch(resultid){
		case SQL_RI_BACKGROUND: {
			//mysql_store_result();
			if(mysql_result_stored())
				mysql_free_result();
			return 1;
		}
		case SQL_RI_DOMY_LISTA:
			return domy_OnMysqlQuery(resultid,spareid);
		case SQL_RI_VEHNAMES:
			return UpdateVehicleNames(resultid,spareid);
	}
	return 1;
}

#if defined USE_OPSP
public OnPlayerShootPlayer(Shooter,Target,Float:HealthLost,Float:ArmourLost)
{
    new msg[10];
    format(msg,sizeof(msg),"-%d HP",floatround(HealthLost+ArmourLost));
	UpdateDynamic3DTextLabelText(pTemp[Target][p3d_status],0xa52424ff,msg);
	pTemp[Target][lcTicks]=3;
    return 1;
}
#endif

public OnPlayerEnterDynamicRaceCP(playerid, checkpointid){
	if (aData[A_DRIFT][aState] == A_STATE_ON && drift_OPEDRCP(playerid, checkpointid)) return 1;
	else if (aData[A_RACE][aState] == A_STATE_ON && race_OPEDRCP(playerid, checkpointid)) return 1;
	return 0;
}
