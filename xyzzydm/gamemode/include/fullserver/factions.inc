#define FACTION_NONE	-1
#define	MAX_FACTIONS	16
#define FACTION_POLICE	0
#define FACTION_ARMY	1
#define FACTION_DILERZY	2

enum e_faction{
	ef_name[16],
	ef_color,
	bool:ef_open
}

new Factions[][e_faction]={
//Factions[0][ef_name]="Policja";
//Factions[0][ef_color]=0x1010FF;
//Factions[0][ef_open]=true;
	{"Policja", 0x54F0F0, true},
	{"Wojsko", 0xA60A14, true},
	{"Dilerzy", 0xFFFF10, true}

};


CMD:policja(playerid)
	return cmd_komisariat(playerid);

/*CMD:dilerzy(playerid){
//	Msg(playerid,COLOR_INFO,"Dolaczasz do dilerow - testowy kod frakcji! Zignoruj prosze niedociagniecia.");
	factions_PlayerJoin(playerid,2);
//	pTemp[playerid][faction]=1;
//	UpdatePlayerFactionName(playerid);
	return 1;
} */

OutputFactionChat(playerid, szMsg[])
{
//	new
//	 buffer[127];
	
	format(gstr, sizeof gstr, "%s %s: %s", Factions[pTemp[playerid][faction]][ef_name], GetPlayerProperName(playerid), szMsg);
	
	foreach(i)
		if (pTemp[i][faction]==pTemp[playerid][faction])
		{

			SendClientMessage(i, 0x618796FF, gstr);
			PlaySound(i, 1027);
		}
	
//	format(buffer, sizeof buffer, "[ADMIN] %s: %s", GetPlayerNick(playerid), szMsg);
//	OutputLog(LOG_OCHAT, buffer);
}

factions_PlayerJoin(playerid,factionid){
	if (pTemp[playerid][faction]==factionid) return;

	// warunki dolaczenia do frakcji
	switch (factionid){
		case FACTION_POLICE: {
			if (pData[playerid][hitman]>0) {	// hitman na graczu
				if (GetPlayerMoney(playerid)>=pData[playerid][hitman]) {
					GivePlayerMoney(playerid,-1 * pData[playerid][hitman]);
					pData[playerid][hitman]=0;
					Msg(playerid, COLOR_INFO, "Stawka za Twoja glowe zostaje automatycznie splacona.");
				} else {
					Msg(playerid, COLOR_ERROR, "Niestety, nie mozesz dolaczyc - posiadasz niesplacone mandaty/grzywne i nie stac Cie na ich splate!");
					return;
				}
			}
		}
	}

	pTemp[playerid][faction]=factionid;
	format(gstr, sizeof gstr, "_~n~_~n~_~n~%s",Factions[pTemp[playerid][faction]][ef_name]);
	GameTextForPlayer(playerid, gstr, 3000, 6);

	format(gstr, sizeof gstr, "Dolaczasz do frakcji {b}%s{/b}. Komunikacja z innymi czlonkami - {b}#tekst{/b}.", Factions[pTemp[playerid][faction]][ef_name]);
	Msg(playerid,COLOR_INFO,gstr);

	OutputFactionChat(playerid, "zglaszam sie.");
	factions_SetPlayerAttObject(playerid);
	UpdatePlayerFactionName(playerid);
}

factions_SetPlayerAttObject(playerid){
	if (pTemp[playerid][faction]==FACTION_NONE) return;
	switch(pTemp[playerid][faction]){ 	
		case FACTION_POLICE:
			SetPlayerAttachedObject( playerid, ATTACH_FACTION, 18637, 13, 0.360721, 0.000000, 0.000000, 0.000000, 0.000000, 211.167129, 1.000000, 1.000000, 1.000000 ); // PoliceShield1 - tarcza
/*			switch(random(3)) {
				case 0:
					SetPlayerAttachedObject( playerid, ATTACH_FACTION, 18642, 5, 0.128014, 0.030860, -0.044516, 350.691711, 353.134094, 67.086700, 1.000000, 1.000000, 1.000000 ); // Taser1 - taser
				case 1:
					SetPlayerAttachedObject( playerid, ATTACH_FACTION, 18641, 5, 0.128014, 0.030860, -0.044516, 350.691711, 353.134094, 67.086700, 1.000000, 1.000000, 1.000000 ); // Flashlight1 - latarka
				case 2:
	
			}*/
	}
}
