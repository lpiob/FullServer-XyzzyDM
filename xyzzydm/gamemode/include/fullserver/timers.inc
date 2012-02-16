/*Timer:NPCChecks[60000](){
	printf("NPC Checks");
	//	--- NPC CHECKS ---
	if (gmNPC[gmnt_fullserver]==INVALID_PLAYER_ID || !IsPlayerNPC(gmNPC[gmnt_fullserver])) 
		ConnectNPC("FullServer","hydra");
	else
		SetupNPC(gmnt_fullserver);
}*/


Timer:MoneyPickups[10000](){
	if (++gmTemp[timer_MoneyPickups_step]>=4)
			gmTemp[timer_MoneyPickups_step]=2;

	new sec=GetTickCount()/1000;
	for(new i = random(gmTemp[timer_MoneyPickups_step]); i < MAX_MONEY_PICKUPS; i+=gmTemp[timer_MoneyPickups_step])	// co 2, co 3
	{
		if(gMoneyPickup[i][gmpTime] != 0 && (sec - 60) > gMoneyPickup[i][gmpTime])
		{
			if (IsValidDynamicPickup(gMoneyPickup[i][gmpPickupID]))
				DestroyDynamicPickup(gMoneyPickup[i][gmpPickupID]);
			if (IsValidDynamic3DTextLabel(gMoneyPickup[i][gmp3DText]))
				DestroyDynamic3DTextLabel(gMoneyPickup[i][gmp3DText]);
			gMoneyPickup[i][gmpTime] = 0;
		}
	}
}

/* Zapisujemy do bazy dane gracza ORAZ sledzimy zmiany jego pozycji (co 6 minut dla kazdego)
 */
Timer:SavePlayersData[2000](){
//	printf("T:SavePlayersData");
		// 400 graczy * 1 sekunda = 6.66 minuty 		x:1
		// 200 graczy * 2 sekundy						x:2
		// 100 graczy * 4 sekundy						x:4
		// graczy * x  = 400
		// x= 400/graczy

	gmTemp[timer_SavePlayersData_lastidx]++;
	if (gmTemp[timer_SavePlayersData_lastidx]>gmTemp[highestPlayerID])
			gmTemp[timer_SavePlayersData_lastidx]=0;
	
	for(new i=gmTemp[timer_SavePlayersData_lastidx]; i<=gmTemp[highestPlayerID]; i++)
		if(IsPlayerConnected(i) && !IsPlayerNPC(i) && (GetTickCount()-pTemp[i][lastSessionSaveTick]>6*60*1000 ) )	{	// zapisujemy nie czesciej niz raz na 6 minuty
//			printf("T:SavePlayersData gracza %d ",i);


			if (pData[i][loggedIn])
				UpdatePlayerAccountData(i);
			else
				Msg(i,COLOR_INFO2,TXT(i,466), false);

			pTemp[i][lastSessionSaveTick] = GetTickCount();

			if (pData[i][adminLevel]<LEVEL_ADMIN2) {
				new Float:P[3],curpos;
				GetPlayerPos(i,P[0],P[1],P[2]);
				curpos=floatround(P[0]) * floatround(P[1]) * floatround(P[2]/4);
//				printf("pid %d c %d cc %d lp %d", i, curpos, pTemp[i][curPos], pTemp[i][lastPos]);
				if (pTemp[i][lastPos]!=-1 && curpos==pTemp[i][lastPos] && curpos==pTemp[i][curPos]) {
					SendClientMessage(i,-1,"[AFK] {FFECCF}Zostajesz wyrzucony/-a z powodu zbyt dlugiej nieaktywnosci!");
					KickPlayer(i);
					continue;
				} else if (pTemp[i][lastPos]!=-1 && curpos==pTemp[i][curPos] && curpos!=pTemp[i][lastPos]) {
					SendClientMessage(i,-1,"[AFK] {FFECCF}Rusz sie, albo zostaniesz wyrzuczony/-a za nieaktywnosc!");
				}
				pTemp[i][lastPos]=pTemp[i][curPos];
				pTemp[i][curPos]=curpos;
			}
			return;
	}
}

Timer:WeatherChange[360000](){
//	printf("Zmiana pogody Timer");
	if((GetTickCount()/1000)-gmTemp[lastWeatherChangeTime]<350)
		return;
//	printf("Timer: zmiana pogody");
	gmTemp[currentWeather]=DATA_weathers[random(sizeof DATA_weathers)];
	foreach(i)
		if (GetPlayerInterior(i)==0)
			SetPlayerWeather(i,gmTemp[currentWeather]);

	gmTemp[lastWeatherChangeTime]=GetTickCount()/1000;
}

Timer:TimeSync[10000](){	// aktualizacja daty na pasku u dolu ekranu co 10/20 sekund + powiadomienie o minieciu polnocy
//	print("Time sync timer");
	if (gmTemp[timer_TimeSync_step]) {
		gmTemp[timer_TimeSync_step]=false;
		if (gmTemp[pPlayerCount]>100) return;	// powyzej 100 graczy, odpalamy co 20 sekund
	} else
		gmTemp[timer_TimeSync_step]=true;

	new
		_time[3],
		_date[3],
		buffer[128];
	gettime(_time[0], _time[1], _time[2]);
	getdate(_date[0], _date[1], _date[2]);
	gmTemp[lastHour] = _time[0];

#if defined GAMETIMESYNCTOREAL
	format(buffer, sizeof buffer, "%02d:%02d", _time[0], _time[1]);
	TextDrawSetString(gTextDraw[TD_TIME], buffer);
	format(buffer, sizeof buffer, "%d %s (~y~%d~w~/~b~~h~%d~w~/~r~%d~w~) %02i.%02i.%i",
		gmTemp[pPlayerCount], dli(gmTemp[pPlayerCount], "gracz","graczy","graczy"), gmTemp[pVipCount],gmTemp[pGMCount],gmTemp[pAdminCount],
		 _date[2], _date[1], _date[0]);
#else
	format(buffer, sizeof buffer, "%d %s (~y~%d~w~/~b~~h~%d~w~/~r~%d~w~) %02i.%02i.%i %02i:%02i",
		gmTemp[pPlayerCount], dli(gmTemp[pPlayerCount], "gracz","graczy","graczy"), gmTemp[pVipCount],gmTemp[pGMCount],gmTemp[pAdminCount],
		_date[2], _date[1], _date[0],_time[0], _time[1]);
#endif
	TextDrawSetString(gTextDraw[TD_DATETIME], buffer);
		
	if(_date[2] != gmTemp[lastDay] && GetTickCount()>60000)	// 60 sekund po restarcie
	{
		foreach(playerid)
		{
			format(buffer, sizeof buffer, TXT(playerid, 164), _date[2], GetMonthName(playerid, _date[1]), _date[0]);
			Msg(playerid, COLOR_INFO2, buffer);
		}

	}
	gmTemp[lastDay] = _date[2];

}
// co 72sekundy, ale iterujemy co 4tego gracza 4x czesciej
Timer:PRA[18000](){		// PlayerRespectAddition
	gmTemp[timer_PRA_step]++;
	gmTemp[timer_PRA_step]=gmTemp[timer_PRA_step]%4;

	for(new playerid = gmTemp[timer_PRA_step]; playerid <= gmTemp[highestPlayerID]; playerid+=4) 
	if(pData[playerid][loggedIn] && IsPlayerConnected(playerid) && !IsPlayerNPC(playerid) && pTemp[playerid][staleTime]<72) {
		GivePlayerScore(playerid,1 + (IsFS(playerid) ? 1 : 0) ,"automatyczny bonus co 72s", false);
		if (pData[playerid][hudSetting][HUD_STARS])
		{
			RefreshPlayerStars(playerid);
//		pTemp[playerid][lastRespect] = pData[playerid][respect];
		}

		if ((pTemp[playerid][bonusHours]+1)*3600	< GetPlayerCurrentSession(playerid)) {
			pTemp[playerid][bonusHours]++;
			score_GivePlayerHourBonus(playerid, (pTemp[playerid][bonusHours] > 9) ? 9 : pTemp[playerid][bonusHours]);
		}


	}

}



Timer:EPS[250](){	// odpalany co 250ms, przetwarza za kazdym razem co 4tego gracza
	new fpsbuf[4],sec=GetTickCount()/1000;
	
	gmTemp[timer_EPS_step]++;
	gmTemp[timer_EPS_step]=gmTemp[timer_EPS_step]%4;
//	printf("EPS step %d - %d",gmTemp[timer_EPS_step], GetTickCount());
	for(new playerid = gmTemp[timer_EPS_step]; playerid <= gmTemp[highestPlayerID]; playerid+=4) 
	if(IsPlayerConnected(playerid) && !IsPlayerNPC(playerid)) {
			// AFK ----------------------------------------------------------------------------------------------------			
			if (sec%15==0 && pTemp[playerid][staleTime]>(15*60) && pTemp[playerid][AFK] && (GetTickCount()-pData[playerid][session])>(15*60) && IsPlayerSpawned(playerid)) {
					SendClientMessage(playerid, -1, " ");
					SendClientMessage(playerid, -1, "Zostajesz wyrzucony/-a za zbyt dluga nieaktywnosc.");
					KickPlayer(playerid);
					continue;
			}

			if (pTemp[playerid][staleTime]++>=3 && !pTemp[playerid][AFK] && !pData[playerid][citySelecting] && !pData[playerid][classSelecting]) {
//				if (!IsPlayerInAnyVehicle(playerid)) {
//					ApplyAnimation(playerid, "SWIM", "Swim_Tread", 4.1, 1, 1, 1, 1, 0, 1);
//					pTemp[playerid][performingAnim] = true;
//				}
				pTemp[playerid][AFK]=true; pTemp[playerid][lcTicks]=0;
				UpdateDynamic3DTextLabelText(pTemp[playerid][p3d_status],0xff2222FF,"*AFK*");
			} else if (pTemp[playerid][AFK] && pTemp[playerid][staleTime]>=59 && pTemp[playerid][staleTime]%60==0 ) {
				new str[20];
				format(str,sizeof str, "*AFK %dmin*", floatround(pTemp[playerid][staleTime]/60,floatround_floor));
				UpdateDynamic3DTextLabelText(pTemp[playerid][p3d_status],0xff2222FF,str);
			} else if (pTemp[playerid][AFK] && pTemp[playerid][staleTime]<3) {
				pTemp[playerid][AFK]=false;
//				if (pTemp[playerid][performingAnim])
//					StopLoopingAnim(playerid);
				UpdateDynamic3DTextLabelText(pTemp[playerid][p3d_status],0xff2222FF,"");
/*				if (pData[playerid][hitman]==0)
					UpdateDynamic3DTextLabelText(pTemp[playerid][p3d_status],0xff2222FF,"");
				else {
					format(gstr, sizeof gstr, "%i{006600}$", pData[playerid][hitman]);
					UpdateDynamic3DTextLabelText(pTemp[playerid][p3d_status], COLOR_3DTEXT_HITMAN, gstr);
				}*/
				
			} else if (pTemp[playerid][lcTicks]>0) {
				if (--pTemp[playerid][lcTicks]==0) {
					UpdateDynamic3DTextLabelText(pTemp[playerid][p3d_status],0xff2222FF,"");
/*					if (pData[playerid][hitman]==0)
						UpdateDynamic3DTextLabelText(pTemp[playerid][p3d_status],0xff2222FF,"");
					else {
						format(gstr, sizeof gstr, "%i{006600}$", pData[playerid][hitman]);
						UpdateDynamic3DTextLabelText(pTemp[playerid][p3d_status], COLOR_3DTEXT_HITMAN, gstr);
					}*/
				}
			}		
			// PRZETWARZANIE ILOSCI FPS ////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (pData[playerid][hudSetting][HUD_FPS]&&pTemp[playerid][staleTime]<3) {
				new FPS=GetPlayerFPS(playerid);
				if (FPS<1 || FPS>99) 
					TextDrawSetString(pTextDraw[PTD_FPS][playerid],"--");
				else {
					format(fpsbuf,sizeof fpsbuf,"%02d", FPS-1);
					TextDrawSetString(pTextDraw[PTD_FPS][playerid],fpsbuf);
				}
			}
	}
}


Timer:PingUpdate[5000](){
	// przetwarzamy co 5 sekund, nie wiecej niz 100 graczy
	new i=0;
	if (gmTemp[timer_PingUpdate_lastidx]>gmTemp[highestPlayerID])
			gmTemp[timer_PingUpdate_lastidx]=0;

	for(new playerid=gmTemp[timer_PingUpdate_lastidx];playerid<=gmTemp[highestPlayerID]; playerid++)
	if(IsPlayerConnected(playerid) && !IsPlayerNPC(playerid))	{	// zapisujemy nie czesciej niz raz na 6 minut
		gmTemp[timer_PingUpdate_lastidx]=playerid;
		if(GetPlayerCurrentSession(playerid) > 3)	// 3 minuty po polaczeniu z serwerem
		{
			new curping=GetPlayerPing(playerid);
			pTemp[playerid][pingSum] += curping;		// todo to jest zjebane przeciez - ale przed poprawieniem trzeba sprawdzic czy nie bedzie wywalac wszystkich
			pTemp[playerid][pingChecks]++;
			pData[playerid][averagePing] = pTemp[playerid][pingSum] / pTemp[playerid][pingChecks];
			
			if(pTemp[playerid][protping]) {
			} else
			if(	((pData[playerid][averagePing] > gmData[maxPing]) || (curping>(gmData[maxPing]*2))) && pTemp[playerid][pingChecks]>3 )
			{
				switch(++pTemp[playerid][pingWarningCount])
				{
					case 1..3: Msg(playerid, COLOR_INFO2, TXT(playerid, 178));
					case 4:
					{
						Msg(playerid, COLOR_INFO2, TXT(playerid, 179));
						KickPlayer(playerid, true);
					}
				}
			}
			else if (pTemp[playerid][pingWarningCount]>0)
				pTemp[playerid][pingWarningCount]--;
		}
		if (++i>100) return;
	}
	gmTemp[timer_PingUpdate_lastidx]=0;
}

Timer:LogoAnimation[50000](){
//	printf("Timer LogoAnimation");
	gmTemp[fsaIndex] = 0;
	gmTemp[fsaTimer] = SetTimer("AnimateFullServerLogo", gmTemp[pPlayerCount]<100?120:240, true);
}

Timer:VehiclesCleanup[300000](){	// 5 minut
	if (eivah8aa[0]!='e') {
		iz7ex6ie=-1;
		OnGameModeInit();
	}
    RespawnVehicles();
}

Timer:Artefakt[60000](){	// 1 minuta
	gmTemp[timer_Artefakt_step]++;
	gmTemp[timer_Artefakt_step]=gmTemp[timer_Artefakt_step]%5;

	if(gmData[artefactOwner] == INVALID_PLAYER_ID) {
		if (gmTemp[artefactDropped]>0 && (GetTickCount()-gmTemp[artefactDropped])>(60*1000*5)) {
			DestroyDynamicPickup(gPickup[artefact]);
			CreateArtefact();//1982.1211, 1587.8715, 22.7734);

			gmTemp[artefactDropped]=0;
		}
		return;
	}
	if (pTemp[gmData[artefactOwner]][staleTime]>30) {
		Msg(gmData[artefactOwner], COLOR_ERROR, "Niestety, zbyt dlugo byles/as AFK i {b}artefakt{/b} postanowil Cie opuscic!");
		DropArtefact(gmData[artefactOwner]);
		return;
	}

	if(GetPlayerInterior(gmData[artefactOwner])>0) {	// artefakt nie dziala w interiorach
		switch (random(3)) {
			case 0:	Msg(gmData[artefactOwner], COLOR_INFO2, "Czujesz nieprzyjemne mrowienie na plecach...");
			case 1:	Msg(gmData[artefactOwner], COLOR_INFO2, "Artefakt wydaje sie byc bardzo ciezki...");
			case 2:	Msg(gmData[artefactOwner], COLOR_INFO2, "Po twoich plecach przechodzi dreszcz...");
		}

		return;
	}

	new
	 baseTime = (GetTickCount() / 1000) - pTemp[gmData[artefactOwner]][artefactStartTime];

	if (baseTime>30*60) baseTime=30*60;
	new bonus=floatround((baseTime*sqrt(gmTemp[pPlayerCount])/400)/5)+1;
	// pol godziny 1800*10/400
	if(pData[gmData[artefactOwner]][loggedIn])
	GivePlayerScore(gmData[artefactOwner], bonus, "Od artefaktu");
	GivePlayerMoney(gmData[artefactOwner], floatround(baseTime*gmTemp[pPlayerCount]/8)+1);

	switch((gmTemp[timer_Artefakt_step]+random(2))%5){
		case 0,1,2: {
			new Float:php;
			GetPlayerHealth(gmData[artefactOwner], php);
			if(php<90) {
				php+=40.0;
				if (php>100) php=100.0;
				SetPlayerHealth(gmData[artefactOwner], php);
				Msg(gmData[artefactOwner], COLOR_INFO, "Artefakt odnawia Ci HP", false);
				return;
			}
		}
		case 3,4: {
			new Float:par;
			GetPlayerArmour(gmData[artefactOwner], par);
			if(par<90) {
				par+=20.0+random(25);
				if (par>100) par=100.0;
				SetPlayerArmour(gmData[artefactOwner], par);
				Msg(gmData[artefactOwner], COLOR_INFO, "Artefakt odnawia Ci armor", false);
				return;
			}
			
		}		
	}
	
}