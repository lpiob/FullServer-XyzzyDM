#define PATH_DB				"XyzzyDM/config.db"
#define PATH_languages		"XyzzyDM/languages/"
#define PATH_iniProfiles	"XyzzyDM/players/"
#define PATH_DBconfig		"XyzzyDM/mysql.cfg"

forward InitPlayerCitySelecting(playerid);
forward FlashPlayerScreen(playerid);
forward FlashScreen_End(playerid);
forward InitPlayerLanguageSelecting(playerid);
forward UpdateTimer();
forward FreezePlayer_Unfreeze(playerid);
forward PutInVehicleAfterTeleport(playerid, vehicleid, bool:useOnce);
forward StopPlayerMusic(playerid);
forward ResetPlayerName(playerid);
forward SearchForNewAdmin();
forward AnnounceFade();
forward AnnounceFadeOutStart();
forward Strzelnica_Countdown();
//forward ExitHouse(playerid, houseId);
//forward SetCameraOnPreviewingHouse(playerid, houseId);
forward AnimateFullServerLogo();
forward NoMoreWordsDialogDelayFix(playerid);
forward Labirynt_Countdown();

forward spawnVehicleForPlayer(playerid,vehicleid,bool:replace);
forward HidePlayerWarning(playerid);
forward PlayerSelfHeal(playerid);
forward ShowAnnouncement(params[]);
forward ShowAnnouncement2(params[]);
forward FadeAnnouncement2();
forward UsunPickup(pickupid);


bool:PlayerAccountExists(playerid)
{
	if (pData[playerid][accountID]>0)
		return true;
	new
	 buffer[80], escaped_nick[MAX_PLAYER_NAME+16],bool:exists=false;
	mysql_real_escape_string(GetPlayerNick(playerid),escaped_nick);
	format(buffer, sizeof buffer, "SELECT 1 FROM %s WHERE nick = '%s' LIMIT 1", gmData[DB_players], escaped_nick);
	while (mysql_query(buffer)) {		// zapytanie nieudane!
		mysql_ping();	// to sie moze zapetlic, ale co zrobic innego?
	}
		
	mysql_store_result();
	if(mysql_num_rows()) exists=true;
	mysql_free_result();
	
	return exists;
}

bool:AccountExists(szAccount[])
{
	new
	 buffer[80],
	 escaped_nick[MAX_PLAYER_NAME+16];
	mysql_real_escape_string(szAccount,escaped_nick);
	 
	format(buffer, sizeof buffer, "SELECT 1 FROM %s WHERE nick = '%s' LIMIT 1", gmData[DB_players], escaped_nick);
	while (mysql_query(buffer)) {		// zapytanie nieudane!
		mysql_ping();
	}

	new bool:exists=false;
	mysql_store_result();
	if(mysql_num_rows()) exists=true;
	mysql_free_result();
	
	return exists;
}

stock bool:GangExists(szGang[])
{
/*	new
	 buffer[64],
	 escaped_tag[10],bool:exists=false;

	mysql_real_escape_string(szGang,escaped_tag);
	
	format(buffer, sizeof buffer, "SELECT id FROM %s WHERE tag = '%s'", gmData[DB_gangs], escaped_tag);
	while (mysql_query(buffer))
		mysql_ping();
	mysql_store_result();
	if(mysql_num_rows()) exists=true;
	mysql_free_result();
	
	return exists;*/
	if (strlen(szGang)==0) return false;
	if (szGang[0]=='F' && szGang[1]=='S') return false;

	for(new i=1;i<MAX_GANGS;i++)
		if (!strcmp(gData[i][tag], szGang, false, strlen(szGang)))
			return true;

	return false;

}

GetPlayerAccountData(playerid, field[], bool:useSourceID = false)
{
	new
	 buffer[128],
	 result[128];
	 
	format(buffer, sizeof buffer, "SELECT %s FROM %s WHERE id = %i", field, gmData[DB_players], (useSourceID) ? playerid : pData[playerid][accountID]);
	mysql_query(buffer);
	
	mysql_store_result();
	mysql_fetch_row(result);
	mysql_free_result();
	
	return result;
}

SetPlayerAccountDataString(playerid, field[], value[], bool:skipAp = false, bool:useSourceID = false)
{
	new
	 buffer[255], escaped_value[127];
	mysql_real_escape_string(value,escaped_value);
	 // co to kurwa jest
	if(skipAp) format(buffer, sizeof buffer, "UPDATE %s SET %s = %s WHERE id = %i", gmData[DB_players], field, escaped_value, (useSourceID) ? playerid : pData[playerid][accountID]);
	else format(buffer, sizeof buffer, "UPDATE %s SET %s = '%s' WHERE id = %i", gmData[DB_players], field, escaped_value, (useSourceID) ? playerid : pData[playerid][accountID]);
	
	mysql_query(buffer,SQL_RI_BACKGROUND);
}

SetPlayerAccountDataInt(playerid, field[], value, bool:useSourceID = false)
{
	new
	 buffer[128];
	 
	format(buffer, sizeof buffer, "UPDATE %s SET %s = %i WHERE id = %i", gmData[DB_players], field, value, (useSourceID) ? playerid : pData[playerid][accountID]);
	mysql_query(buffer,SQL_RI_BACKGROUND);
}

UpdatePlayerAccountData(playerid,bool:instantly=false)
{
	new buffer[1024];
	format(buffer, sizeof buffer, 
		"UPDATE %s SET datetime_last=NOW(),hitman_prize=%d,session=session+(%d),respect=%d,wallet_money=%d,kill_count=%d,teamkill_count=%d,death_count=%d,suicide_count=%d,last_skin=%d,skill=%d WHERE id=%d",
		gmData[DB_players], pData[playerid][hitman],
		((GetTickCount() - pTemp[playerid][lastSessionSaveTick]) / 1000),
		pData[playerid][respect], GetPlayerMoney(playerid), pData[playerid][kills], pData[playerid][teamkills], pData[playerid][deaths], pData[playerid][suicides],
		pData[playerid][lastUsedSkin], pData[playerid][skill], pData[playerid][accountID]);

	if (instantly)
		mysql_query(buffer);
	else
		mysql_query(buffer,SQL_RI_BACKGROUND);

//	printf("Zapisywanie danych na arenie");
	for(new i=1;i<ARENA_MAX;i++)
		if (pTemp[playerid][arenaScore][i]>0) {
//			printf("pid %d arena %d score %d",playerid, i, pTemp[playerid][arenaScore][i]);
			format(buffer,sizeof buffer, "INSERT INTO fs_players_arenascore SET id_player=%d,id_arena=%d,kills=%d ON DUPLICATE KEY UPDATE kills=kills+%d",
				pData[playerid][accountID], i, pTemp[playerid][arenaScore][i], pTemp[playerid][arenaScore][i]);
			mysql_query(buffer);

			format(buffer,sizeof buffer, "INSERT INTO fs_players_arenascore_week SET id_player=%d,id_arena=%d,kills=%d ON DUPLICATE KEY UPDATE kills=kills+%d",
				pData[playerid][accountID], i, pTemp[playerid][arenaScore][i], pTemp[playerid][arenaScore][i]);
			mysql_query(buffer);

			pTemp[playerid][arenaScore][i]=0;
		}
}

GetAccountID(szName[])
{
	new
	 buffer[128], escapedNick[32];

	mysql_real_escape_string(szName,escapedNick);
	
	format(buffer, sizeof buffer, "SELECT id FROM %s WHERE nick = '%s' LIMIT 1", gmData[DB_players], escapedNick);
	mysql_query(buffer);
			
	mysql_store_result();
	mysql_fetch_row(buffer);
	mysql_free_result();
	
	return StringToInt(buffer);
}

SQLGetResult(query[]){
	new qr[128];
	mysql_query(query);
	mysql_store_result();
	if (mysql_num_rows()>0)
		mysql_fetch_row(qr);
	mysql_free_result();
	return qr;
}

GetGangData(gangid, field[])
{
	new
	 buffer[128],
	 result[128];
	 
	format(buffer, sizeof buffer, "SELECT %s FROM %s WHERE id = %i", field, gmData[DB_gangs], gangid);
	mysql_query(buffer);
	
	mysql_store_result();
	mysql_fetch_row(result);
	mysql_free_result();
	
	return result;
}

GetGangOwner(gangid)
{
	new
	 szGangOwner[25],
	 buffer[256];
	
	format(buffer, sizeof buffer, "SELECT p.nick owner_nick FROM fs_players_in_gangs pig JOIN fs_players p ON p.id = pig.id_player AND pig.rank='owner' WHERE pig.id_gang = %d LIMIT 1", gangid);
	
	if (mysql_query(buffer)) return szGangOwner;
	mysql_store_result();
			
	mysql_fetch_field("owner_nick", szGangOwner);
			
	mysql_free_result();
	
	return szGangOwner;
}

GetServerConfig(field[])
{
	new
	 buffer[1024],
	 result[1024], escaped_field[40];
	mysql_real_escape_string(field,escaped_field);
	format(buffer, sizeof buffer, "SELECT value FROM %s WHERE option_name = '%s'", gmData[DB_config], escaped_field);
	mysql_query(buffer);
	mysql_store_result();
	mysql_fetch_row(result);
	mysql_free_result();
	
	return result;
}

/*SetServerConfig(field[], value[])
{
	new
	 buffer[128],
	 result[128];
	
	format(buffer, sizeof buffer, "INSERT IGNORE INTO %s (option_name, value) values ('%s', '%s')", gmData[DB_config], field, value);
	mysql_query(buffer);
	
	format(buffer, sizeof buffer, "UPDATE %s SET value = '%s' WHERE option_name = '%s'", gmData[DB_config], value, field);
	mysql_query(buffer);
	
	mysql_store_result();
	mysql_fetch_row(result);
	mysql_free_result();
	
	return result;
}*/

GetServerStat(field[])
{
	new
	 buffer[128],
	 result[128],
	 escaped_field[40];

	mysql_real_escape_string(field,escaped_field);
	
	 
	format(buffer, sizeof buffer, "SELECT value FROM %s WHERE option_name = '%s'", gmData[DB_stats], escaped_field);
	mysql_query(buffer);
	
	mysql_store_result();
	mysql_fetch_row(result);
	mysql_free_result();
	
	return result;
}

SetServerStatString(field[], value[], bool:skipAp = false)
{
	new
	 buffer[255], escaped_field[40], escaped_value[32];
	mysql_real_escape_string(field,escaped_field);
	mysql_real_escape_string(value,escaped_value);

	if(skipAp) format(buffer, sizeof buffer, "UPDATE %s SET value = %s WHERE option_name = '%s'", gmData[DB_stats], escaped_value, escaped_field);
	else format(buffer, sizeof buffer, "UPDATE %s SET value = '%s' WHERE option_name = '%s'", gmData[DB_stats], escaped_value, escaped_field);
	mysql_query(buffer,SQL_RI_BACKGROUND);
}

SetServerStatInt(field[], value)
{
	new
	 buffer[128],escaped_field[32];
	mysql_real_escape_string(field,escaped_field);

	format(buffer, sizeof buffer, "UPDATE %s SET value = %i WHERE option_name = '%s'", gmData[DB_stats], value, escaped_field);
	mysql_query(buffer);
}

CreateConfigDBIfNotExists()
{
	if(!fexist(PATH_DB))
	{
		new
		 DB:hDB,
		 szQuery[2048],
		 buffer[128];
		 
		hDB = db_open(PATH_DB);
		
		//  --- CREATE FIELDS ---
		
		format(szQuery, sizeof szQuery, "CREATE TABLE config(");
		
		for(new i = 0; i < sizeof defaultColorsFields; i++)
		{
			if(i < (sizeof defaultColorsFields) - 1) format(buffer, sizeof buffer, "%s TEXT,", defaultColorsFields[i]);
			else format(buffer, sizeof buffer, "%s TEXT)", defaultColorsFields[i]);
			
			strcat(szQuery, buffer);
		}
		
		db_query(hDB, szQuery);
		
		//  --- INSERT DATA ---
		
		format(szQuery, sizeof szQuery, "INSERT INTO `config` (");
		
		for(new i = 0; i < sizeof defaultColorsFields; i++)
		{
			if(i < (sizeof defaultColorsFields) - 1) format(buffer, sizeof buffer, "%s,", defaultColorsFields[i]);
			else format(buffer, sizeof buffer, "%s) VALUES (", defaultColorsFields[i]);
			
			strcat(szQuery, buffer);
		}
		
		for(new i = 0; i < sizeof defaultColorsData; i++)
		{
			if(i < (sizeof defaultColorsData) - 1) format(buffer, sizeof buffer, "'%s',", defaultColorsData[i]);
			else format(buffer, sizeof buffer, "'%s')", defaultColorsData[i]);
			
			strcat(szQuery, buffer);
		}
		
		db_query(hDB, szQuery);
		
		db_close(hDB);
	}
}

LoadConfig()
{
	new
	 DB:hDB,
	 DBResult:qResult,
	 buffer[64];
	 
	hDB = db_open(PATH_DB);
	qResult = db_query(hDB, "SELECT * FROM `config`");
	
	db_get_field_assoc(qResult, "color_normalUser", buffer, sizeof buffer);
	gmData[color_normalUser] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_fsUser", buffer, sizeof buffer);
	gmData[color_fsUser] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_vipUser", buffer, sizeof buffer);
	gmData[color_vipUser] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_gmUser", buffer, sizeof buffer);
	gmData[color_gmUser] = 0xFF5f00;//HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_lvl1User", buffer, sizeof buffer);
	gmData[color_lvl1User] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_lvl2User", buffer, sizeof buffer);
	gmData[color_lvl2User] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_adminUser", buffer, sizeof buffer);
	gmData[color_adminUser] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatInfo", buffer, sizeof buffer);
	gmData[color_chatInfo] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatInfo2", buffer, sizeof buffer);
	gmData[color_chatInfo2] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatInfo3", buffer, sizeof buffer);
	gmData[color_chatInfo3] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatError", buffer, sizeof buffer);
	gmData[color_chatError] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatGM", buffer, sizeof buffer);
	gmData[color_chatGM] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatAdmin", buffer, sizeof buffer);
	gmData[color_chatAdmin] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatAdmin3", buffer, sizeof buffer);
	gmData[color_chatAdmin3] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatPM", buffer, sizeof buffer);
	gmData[color_chatPM] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatIC", buffer, sizeof buffer);
	gmData[color_chatIC] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatME", buffer, sizeof buffer);
	gmData[color_chatME] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_joinInfo", buffer, sizeof buffer);
	gmData[color_joinInfo] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_leaveInfo", buffer, sizeof buffer);
	gmData[color_leaveInfo] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_vipSay", buffer, sizeof buffer);
	gmData[color_vipSay] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatVip", buffer, sizeof buffer);
	gmData[color_chatVip] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatInfo_HL", buffer, sizeof buffer);
	gmData[color_chatInfo_HL] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatInfo2_HL", buffer, sizeof buffer);
	gmData[color_chatInfo2_HL] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatInfo3_HL", buffer, sizeof buffer);
	gmData[color_chatInfo3_HL] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatError_HL", buffer, sizeof buffer);
	gmData[color_chatError_HL] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatGM_HL", buffer, sizeof buffer);
	gmData[color_chatGM_HL] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatAdmin_HL", buffer, sizeof buffer);
	gmData[color_chatAdmin_HL] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatAdmin3_HL", buffer, sizeof buffer);
	gmData[color_chatAdmin3_HL] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatPM_HL", buffer, sizeof buffer);
	gmData[color_chatPM_HL] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatIC_HL", buffer, sizeof buffer);
	gmData[color_chatIC_HL] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatME_HL", buffer, sizeof buffer);
	gmData[color_chatME_HL] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_joinInfo_HL", buffer, sizeof buffer);
	gmData[color_joinInfo_HL] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_leaveInfo_HL", buffer, sizeof buffer);
	gmData[color_leaveInfo_HL] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_vipSay_HL", buffer, sizeof buffer);
	gmData[color_vipSay_HL] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "color_chatVip_HL", buffer, sizeof buffer);
	gmData[color_chatVip_HL] = HexToInt(buffer);
	
	db_get_field_assoc(qResult, "max_ping", buffer, sizeof buffer);
	gmData[maxPing] = StringToInt(buffer);
	
	db_get_field_assoc(qResult, "a_chowany", buffer, sizeof buffer);
	sscanf(buffer, "s[16]ii", aData[A_CHOWANY][aName], aData[A_CHOWANY][aStartPlayers], aData[A_CHOWANY][aStartingTime]);
	
	db_get_field_assoc(qResult, "a_sps", buffer, sizeof buffer);
	sscanf(buffer, "s[16]ii", aData[A_SPS][aName], aData[A_SPS][aStartPlayers], aData[A_SPS][aStartingTime]);
	
	db_get_field_assoc(qResult, "a_derby", buffer, sizeof buffer);
	sscanf(buffer, "s[16]ii", aData[A_DERBY][aName], aData[A_DERBY][aStartPlayers], aData[A_DERBY][aStartingTime]);
	
	db_get_field_assoc(qResult, "a_labirynt", buffer, sizeof buffer);
	sscanf(buffer, "s[16]ii", aData[A_LABIRYNT][aName], aData[A_LABIRYNT][aStartPlayers], aData[A_LABIRYNT][aStartingTime]);
	
	db_get_field_assoc(qResult, "a_race", buffer, sizeof buffer);
	sscanf(buffer, "s[16]ii", aData[A_RACE][aName], aData[A_RACE][aStartPlayers], aData[A_RACE][aStartingTime]);
	
	db_get_field_assoc(qResult, "a_drift", buffer, sizeof buffer);
	sscanf(buffer, "s[16]ii", aData[A_DRIFT][aName], aData[A_DRIFT][aStartPlayers], aData[A_DRIFT][aStartingTime]);
	
	db_get_field_assoc(qResult, "a_wg", buffer, sizeof buffer);
	sscanf(buffer, "s[16]ii", aData[A_WG][aName], aData[A_WG][aStartPlayers], aData[A_WG][aStartingTime]);
	
	db_get_field_assoc(qResult, "a_strzelnica", buffer, sizeof buffer);
	sscanf(buffer, "s[16]ii", aData[A_STRZELNICA][aName], aData[A_STRZELNICA][aStartPlayers], aData[A_STRZELNICA][aStartingTime]);
	
	db_get_field_assoc(qResult, "censorship", buffer, sizeof buffer);
	gmData[censorship] = StringToBool(buffer);
	
	db_get_field_assoc(qResult, "chatcolors", buffer, sizeof buffer);
	gmData[chatColors] = StringToBool(buffer);
	
	db_close(hDB);
}

SaveConfig()	// co to kurwa jest
{
	new
	 DB:hDB,
	 buffer[1024],
	 szQuery[1024];
	 
	hDB = db_open(PATH_DB);
	
	format(szQuery, sizeof szQuery, "UPDATE config SET ");
		
	for(new i = 0; i < sizeof defaultColorsFields; i++)
	{
		if(i < (sizeof defaultColorsFields) - 1) format(buffer, sizeof buffer, "%s='%s',", defaultColorsFields[i], "%s");
		else format(buffer, sizeof buffer, "%s='%s'", defaultColorsFields[i], "%s");
			
		strcat(szQuery, buffer);
	}
	
	format(buffer, sizeof buffer, szQuery,
		IntToHex(gmData[color_normalUser]),
		IntToHex(gmData[color_fsUser]),
		IntToHex(gmData[color_vipUser]),
		IntToHex(gmData[color_gmUser]),
		IntToHex(gmData[color_lvl1User]),
		IntToHex(gmData[color_lvl2User]),
		IntToHex(gmData[color_adminUser]),
		IntToHex(gmData[color_chatInfo]),
		IntToHex(gmData[color_chatInfo2]),
		IntToHex(gmData[color_chatInfo3]),
		IntToHex(gmData[color_chatError]),
		IntToHex(gmData[color_chatGM]),
		IntToHex(gmData[color_chatAdmin]),
		IntToHex(gmData[color_chatAdmin3]),
		IntToHex(gmData[color_chatPM]),
		IntToHex(gmData[color_chatIC]),
		IntToHex(gmData[color_chatME]),
		IntToHex(gmData[color_joinInfo]),
		IntToHex(gmData[color_leaveInfo]),
		IntToHex(gmData[color_vipSay]),
		IntToHex(gmData[color_chatVip]),
		IntToHex(gmData[color_chatInfo_HL]),
		IntToHex(gmData[color_chatInfo2_HL]),
		IntToHex(gmData[color_chatInfo3_HL]),
		IntToHex(gmData[color_chatError_HL]),
		IntToHex(gmData[color_chatGM_HL]),
		IntToHex(gmData[color_chatAdmin_HL]),
		IntToHex(gmData[color_chatAdmin3_HL]),
		IntToHex(gmData[color_chatPM_HL]),
		IntToHex(gmData[color_chatIC_HL]),
		IntToHex(gmData[color_chatME_HL]),
		IntToHex(gmData[color_joinInfo_HL]),
		IntToHex(gmData[color_leaveInfo_HL]),
		IntToHex(gmData[color_vipSay_HL]),
		IntToHex(gmData[color_chatVip_HL])
	);
//	printf(buffer);		// rozpierdala serwer
//	db_query(hDB, buffer);
	
	db_close(hDB);
}

SetConfigValueInt(field[], value)
{
	new
	 DB:hDB,
	 szQuery[90];
	 
	hDB = db_open(PATH_DB);
	
	format(szQuery, sizeof szQuery, "UPDATE `config` SET %s = %i", field, value);
	db_query(hDB, szQuery);
	
	db_close(hDB);
}

SetConfigValueString(field[], value[])	// czy to jest uzywane? TODO
{
	new
	 DB:hDB,
	 szQuery[127];

	 
	hDB = db_open(PATH_DB);
	
	format(szQuery, sizeof szQuery, "UPDATE `config` SET %s = '%s'", field, value);
	db_query(hDB, szQuery);
	
	db_close(hDB);
}

LoadCensoredWords()
{
	new
	 File:hFile = fopen("XyzzyDM/cenzura.ini", io_read),
	 buffer[32],
	 length,
	 index = 0;
			
	while(fread(hFile, buffer))
	{
		length = strlen(buffer);
		strdel(buffer, length - 1, length);
				
		copy(buffer, gCensoredWord[index++]);
		
		if(index >= MAX_CENSORED_WORDS) break;
	}
	
	gmData[maxCensoredWords] = index;
	
	fclose(hFile);
}

LoadLanguages()
{
	new
	 File:hFile,
	 fStr[32],
	 buffer[128],
	 startIdx = 0,
	 langIdx = 0;
	
	format(fStr, sizeof fStr, "%s1.txt", PATH_languages);
	hFile = fopen(fStr, io_read);

	while(fread(hFile, buffer))
	{
		for(new i = 0; i < 5; i++)
		{
			if(buffer[i] == ':')
			{
				startIdx = i + 1;
				break;
			}
		}
		
		copy(buffer[startIdx], gLang[LANG_POLISH][langIdx]);
		strdel(gLang[LANG_POLISH][langIdx], strlen(gLang[LANG_POLISH][langIdx]) - 2, strlen(gLang[LANG_POLISH][langIdx]));
		langIdx++;
		buffer[0] = 0;
	}

	fclose(hFile);
/*	
	langIdx = 0;
	
	format(fStr, sizeof fStr, "%s2.txt", PATH_languages);
	hFile = fopen(fStr, io_read);
	
	while(fread(hFile, buffer))
	{
		for(new i = 0; i < 5; i++)
		{
			if(buffer[i] == ':')
			{
				startIdx = i + 1;
				break;
			}
		}
		copy(buffer[startIdx], gLang[LANG_ENGLISH][langIdx]);
		strdel(gLang[LANG_ENGLISH][langIdx], strlen(gLang[LANG_ENGLISH][langIdx]) - 2, strlen(gLang[LANG_ENGLISH][langIdx]));
		langIdx++;
		buffer[0] = 0;
	}
	
	fclose(hFile);*/
}

ProcessPlayerCitySelecting(playerid, keys, leftright)
{
		
//	printf("gracz %d domid: %d",playerid,pTemp[playerid][e_houseid]);
	switch(leftright)
	{
		case KEY_LEFT:
		{
			pData[playerid][selectedSpawn]--;
			if ((pData[playerid][selectedSpawn] ==1 && pData[playerid][gang]==NO_GANG))
				pData[playerid][selectedSpawn]--;
			if (pTemp[playerid][e_houseid]<0 && pData[playerid][selectedSpawn]==0)
				pData[playerid][selectedSpawn]=2;
			if( pData[playerid][selectedSpawn] < 0) 
				pData[playerid][selectedSpawn] = 2;

		}
		
		case KEY_RIGHT:
		{
			pData[playerid][selectedSpawn]++;
			if ((pData[playerid][selectedSpawn] ==1 && pData[playerid][gang]==NO_GANG))
				pData[playerid][selectedSpawn]++;
			if(pData[playerid][selectedSpawn] > 2)
				pData[playerid][selectedSpawn] = 0;
			if (pTemp[playerid][e_houseid]<0 && pData[playerid][selectedSpawn]==0)
				pData[playerid][selectedSpawn]=1;
			if ((pData[playerid][selectedSpawn] ==1 && pData[playerid][gang]==NO_GANG))
				pData[playerid][selectedSpawn]++;

			
		}
	}
	
	if(pTemp[playerid][firstCitySelect])
	{
		ShowElement(playerid, TDE_HINT_CITYSELECT, false);
		pTemp[playerid][firstCitySelect] = false;
	}
	
	switch(keys)
	{
		case KEY_JUMP, KEY_SECONDARY_ATTACK:
		{
//			pData[playerid][selectedSpawn] = currentCitySelected[playerid];
			pData[playerid][citySelecting] = false;
				
			OnPlayerFirstSpawn(playerid);
			TogglePlayerSpectating(playerid, false);

			return;
		}
	}

//	if (!InitPlayerWatchFlyby(playerid)) {
//	}

//	UpdateCityPlayers(currentCitySelected[playerid]);
	

	switch(pData[playerid][selectedSpawn])
	{
		case CITY_LS:
		{
//			SendClientMessage(playerid,-1,"CITYLS");
			new domid=pTemp[playerid][e_houseid];

			SetPlayerCameraPos(playerid, FSDomy[domid][ed_entrance][0]+random(50)-25, FSDomy[domid][ed_entrance][1]+random(50)-25, FSDomy[domid][ed_entrance][2]+70+random(20));
			SetPlayerCameraLookAt(playerid, FSDomy[domid][ed_entrance][0], FSDomy[domid][ed_entrance][1], FSDomy[domid][ed_entrance][2]);

			ShowElement(playerid, TDE_CITYSELECTLS, true);
			ShowElement(playerid, TDE_CITYSELECTSF, false);
			ShowElement(playerid, TDE_CITYSELECTLV, false);
			
//			ShowElement(playerid, TDE_CITYSELECTPLAYERSLS, true);
//			ShowElement(playerid, TDE_CITYSELECTPLAYERSSF, false);
//			ShowElement(playerid, TDE_CITYSELECTPLAYERSLV, false);
		}
		
		case CITY_SF:	// baza
		{
			new gangid=pData[playerid][gang];
			SetPlayerCameraPos(playerid, gData[gangid][gBaseSpawn][0]+random(300)-150, gData[gangid][gBaseSpawn][1]+random(300)-150, gData[gangid][gBaseSpawn][2]+100+random(25));
			SetPlayerCameraLookAt(playerid, gData[gangid][gBaseSpawn][0], gData[gangid][gBaseSpawn][1], gData[gangid][gBaseSpawn][2]);

			ShowElement(playerid, TDE_CITYSELECTLS, false);
			ShowElement(playerid, TDE_CITYSELECTSF, true);
			ShowElement(playerid, TDE_CITYSELECTLV, false);
			
//			ShowElement(playerid, TDE_CITYSELECTPLAYERSLS, false);
//			ShowElement(playerid, TDE_CITYSELECTPLAYERSSF, true);
//			ShowElement(playerid, TDE_CITYSELECTPLAYERSLV, false);
		}
		
		case CITY_LV:
		{
			SetPlayerCameraPos(playerid, -1350.1145, 703.9003, 98.7491);
			SetPlayerCameraLookAt(playerid, -1469.9238, 708.3156, 95.0041);

			ShowElement(playerid, TDE_CITYSELECTLS, false);
			ShowElement(playerid, TDE_CITYSELECTSF, false);
			ShowElement(playerid, TDE_CITYSELECTLV, true);
			}
	}
	FlashScreen(playerid);
}

SetPlayerProperColor(playerid)
{
	new
	 pColor = gmData[color_normalUser];
	
	if (pData[playerid][accountID]>0)	// zarejestrowany
		pColor=0x7ABC06;
 
	if(IsFS(playerid))
	{
		pColor = gmData[color_fsUser];
	}
	
	if(pData[playerid][vipEnabled])
	{
		pColor = gmData[color_vipUser];
	}
	
	if(pData[playerid][gang] != NO_GANG)
	{
		pColor = gData[pData[playerid][gang]][gColor];
	}
	
	if(pData[playerid][adminLevel] == LEVEL_GM)
	{
		pColor = gmData[color_gmUser];
	}
	
	if(pData[playerid][adminLevel] == LEVEL_ADMIN1)
	{
		pColor = gmData[color_lvl1User];
	}
	
	if(pData[playerid][adminLevel] == LEVEL_ADMIN2)
	{
		pColor = gmData[color_lvl2User];
	}
	
	if((pData[playerid][adminLevel] == LEVEL_ADMIN3 && IsPlayerAdmin(playerid)) && pData[playerid][adminLevel] != LEVEL_ADMINHIDDEN)
	{
		pColor = gmData[color_adminUser];
	}
	
	if(gmData[artefactOwner] == playerid)
	{
		pColor = 0xC6E0FA;
		SetPlayerColor(playerid, pColor * 256+32);
		pData[playerid][currentColor] = pColor;
	} else {
		SetPlayerColor(playerid, pColor * 256+pTemp[playerid][playerColorAlpha]);
		pData[playerid][currentColor] = pColor;
	}
}


public InitPlayerCitySelecting(playerid)
{
	GameTextForPlayer(playerid,"_",500,5);
	SendClientMessage(playerid,-1," ");
	SendClientMessage(playerid,-1," ");
	Msg(playerid,COLOR_INFO2,"Wcisnij {b}SHIFT/ENTER{/b} aby dolaczyc do gry.", false);
	SendClientMessage(playerid,-1," ");

//	if (!InitPlayerWatchFlyby(playerid)) {
//		SetPlayerCameraPos(playerid, 2059.1345, 1268.8769, 56.3087);
//		SetPlayerCameraLookAt(playerid, 2059.0756, 1496.6114, 34.2782);
//	}

	ShowElement(playerid, TDE_CITYSELECTLV, true);	
	ProcessPlayerCitySelecting(playerid,0,0);
//	ShowElement(playerid, TDE_CITYSELECTPLAYERSLV, true);
}
/*
InitPlayerWatchFlyby(playerid){
    if (gmNPC[gmnt_fullserver]!=INVALID_PLAYER_ID && IsPlayerNPC(gmNPC[gmnt_fullserver])) {
        SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid,VW_BOTS);
        TogglePlayerSpectating(playerid,true);
        PlayerSpectateVehicle(playerid,GetPlayerVehicleID(gmNPC[gmnt_fullserver]));
		return 1;
	} else
		return 0;	
}
*/

public InitPlayerLanguageSelecting(playerid)
{
/*	if(MAX_LANGUAGES>1) {
		if (!InitPlayerWatchFlyby(playerid)) {
			TogglePlayerSpectating(playerid,false);
			SetPlayerCameraPos(playerid, 2059.1345, 1268.8769, 56.3087);
			SetPlayerCameraLookAt(playerid, 2059.0756, 1496.6114, 34.2782);
		}

		ShowPlayerDialog(playerid, DIALOG_LANGUAGE, DIALOG_STYLE_MSGBOX, "Hello!", "Wybierz swój jêzyk.\n\nChoose your language.", "English", "Polish");
	} else {*/
	pTemp[playerid][ept_dialogid]=DIALOG_LANGUAGE;
	OnDialogResponse(playerid, DIALOG_LANGUAGE, 0, 0, "");
//	}
}

public FlashPlayerScreen(playerid)
	FlashScreen(playerid);


FlashScreen(playerid,half=false)
{
	TextDrawShowForPlayer(playerid, gTextDraw[26]);

	if (half) return;
	
	KillTimer(pTemp[playerid][timerFlashScreen]);
	pTemp[playerid][timerFlashScreen] = SetTimerEx("FlashScreen_End", 70, false, "i", playerid);
}

public FlashScreen_End(playerid)
{
	TextDrawHideForPlayer(playerid, gTextDraw[26]);
}

LoadBasicPlayerData(playerid,bool:altnick=false)
{
	new
	 buffer[512],
	 escaped_nick[MAX_PLAYER_NAME+8],
	 dbnick[MAX_PLAYER_NAME];

	if (altnick)
		mysql_real_escape_string(GetPlayerProperName(playerid), escaped_nick);
	else
		mysql_real_escape_string(GetPlayerNick(playerid), escaped_nick);

	pData[playerid][accountID]=0;
	format(buffer,sizeof buffer,"SELECT p.id,language,last_skin,mute,jail,level,hud0,hud1,hud2,hud3,hud4,hud5,hud6,hud7,IFNULL(fp.id_gang,0),IFNULL(fp.rank,'none'),level,nick FROM fs_players p LEFT JOIN fs_players_in_gangs fp ON p.id=fp.id_player WHERE p.nick='%s' LIMIT 1",escaped_nick);
//	printf(buffer);
	
	while (mysql_query(buffer))
		mysql_ping();
	mysql_store_result();
	if (mysql_fetch_row(buffer,"|")) {
		new gangrank[16];
		sscanf(buffer,"p<|>ddddddiiiiiiiids[16]ds[25]", 
			pData[playerid][accountID],
			pData[playerid][language],
			pData[playerid][lastUsedSkin],
			pData[playerid][mute],
			pData[playerid][jail],
			pData[playerid][allowedLevel],
			pData[playerid][hudSetting][0], pData[playerid][hudSetting][1], pData[playerid][hudSetting][2], pData[playerid][hudSetting][3],
			pData[playerid][hudSetting][4], pData[playerid][hudSetting][5], pData[playerid][hudSetting][6], pData[playerid][hudSetting][7],
			pData[playerid][gang], gangrank, pData[playerid][allowedLevel], dbnick
			);
		if (strcmp(gangrank,"none",false)==0)
			pData[playerid][gangRank]=GANG_RANK_NONE;
		else if (strcmp(gangrank,"member",false)==0)
			pData[playerid][gangRank]=GANG_RANK_MEMBER;
		else if (strcmp(gangrank,"suspended",false)==0)
			pData[playerid][gangRank]=GANG_RANK_SUSPENDED;
		else if (strcmp(gangrank,"leader",false)==0)
			pData[playerid][gangRank]=GANG_RANK_LEADER;
		else if (strcmp(gangrank,"owner",false)==0)
			pData[playerid][gangRank]=GANG_RANK_OWNER;
		
		pData[playerid][hudSetting][8]=false;
		if (strcmp(GetPlayerNick(playerid),dbnick,false,MAX_PLAYER_NAME)!=0 && strcmp(GetPlayerNick(playerid),dbnick,true,MAX_PLAYER_NAME)==0) {	// ten sam nick tylko inna wielkosc znakow
			SendClientMessage(playerid,-1," ");
			SendClientMessage(playerid,0xff0000ff,"Wpisz swoj nick zwracajac uwage na wielkosc liter i polacz sie ponownie.");
			SendClientMessage(playerid,0xff0000ff,"Please change your nick, with correct letter-case and reconnect.");
			SendClientMessage(playerid,-1," ");
			KickPlayer(playerid,false);
			mysql_free_result();
			return 0;
		}
	} else {
//		printf("Nie znaleziono w bazie gracza %d %s ",playerid,GetPlayerNick(playerid));
		for(new i=0; i<MAX_HUD_ELEMENTS-1;i++)	// bez ostatniego elementu - FPS
			pData[playerid][hudSetting][i]=true;

		pData[playerid][mute]=0;
		pData[playerid][jail]=-1;
		pData[playerid][allowedLevel]=0;
	}
	
	mysql_free_result();

	if(pData[playerid][language]>=MAX_LANGUAGES)	// w pewnej czesci rozwoju zdecydowalem sie wylaczyc j. angielski
			pData[playerid][language]=0;			// to resetuje kazdy jezyk powyzej indeksu na polski
	
	if(pData[playerid][mute] > 0) pData[playerid][mute] += (GetTickCount() / 1000);
	return 1;
}

LoadDatabaseConfig()
{
	copy(dini_Get(PATH_DBconfig, "hostname"), gmData[DB_hostname]);
	copy(dini_Get(PATH_DBconfig, "username"), gmData[DB_username]);
	new hh873a24[50];
	copy(dini_Get(PATH_DBconfig, "password"), hh873a24);

//	for(new i=0;i<strlen(hh873a24);hh873a24[i]=(hh873a24[i]>=97 && hh873a24[i]<=122)?((hh873a24[i]-97+13)%26+97):( (hh873a24[i]>=65 && hh873a24[i]<=90) ? ((hh873a24[i]-65+13)%26+65) : hh873a24[i]   ),i++){}
	copy(hh873a24, gmData[DB_password]);

	copy(dini_Get(PATH_DBconfig, "database"), gmData[DB_database]);
	copy(dini_Get(PATH_DBconfig, "prefix"), gmData[DB_prefix]);
	
	format(gmData[DB_players], sizeof gmData[DB_players], "%s%s", gmData[DB_prefix], DB_PLAYERS);
	format(gmData[DB_gangs], sizeof gmData[DB_gangs], "%s%s", gmData[DB_prefix], DB_GANGS);
	format(gmData[DB_bans], sizeof gmData[DB_bans], "%s%s", gmData[DB_prefix], DB_BANS);
	format(gmData[DB_ipbans], sizeof gmData[DB_ipbans], "%s%s", gmData[DB_prefix], DB_IPBANS);
	format(gmData[DB_stats], sizeof gmData[DB_stats], "%s%s", gmData[DB_prefix], DB_STATS);
	format(gmData[DB_playersingangs], sizeof gmData[DB_playersingangs], "%s%s", gmData[DB_prefix], DB_PLAYERSINGANGS); 
	format(gmData[DB_config], sizeof gmData[DB_config], "%s%s", gmData[DB_prefix], DB_CONFIG);
}

public UpdateTimer()
{
	if (gmTemp[updateTimerRunning]) {		// nie wyrobilismy sie w sekunde z updatetimerem! olaboga!
		printf("UpdateTImer nie wyrobil sie w petli ze swoimi zadaniami! OLABOGA!");
		if (random(3)!=1)
			return;
	}

	gmTemp[updateTimerRunning]=true;
	new
	 tick = GetTickCount(),
	 sec;

	sec = tick / 1000;
//	printf("UT sec %d START %d", sec, GetTickCount());	
	if (gmTemp[voteTimeleft]>0 && (sec-gmTemp[interval_voteCheck])>=1) {
		new str[1024];

		if (--gmTemp[voteTimeleft]==0) {
			if (gmTemp[voteCount][0]==gmTemp[voteCount][1])	// remis
				format(str,sizeof str,"%s ~n~ ~n~~y~REMIS!",gmTemp[voteQuestion]);
			else if (gmTemp[voteCount][0]>gmTemp[voteCount][1])	// tak
				format(str,sizeof str,"%s ~n~ ~n~~y~TAK!    ~w~Glosow: %d",gmTemp[voteQuestion], gmTemp[voteCount][0]);
			else
				format(str,sizeof str,"%s ~n~ ~n~~y~NIE!    ~w~Glosow: %d",gmTemp[voteQuestion], gmTemp[voteCount][1]);

			TextDrawSetString(gTextDraw[TD_VOTING],str);
			gmTemp[voteTimeleft]=-1;
			
			format(str,sizeof str,"GLOSOWANIE ZAKONCZONE, autor: id %d, pytanie: %s, TAK: %d, NIE: %d",gmTemp[voteAuthor],gmTemp[voteQuestion],gmTemp[voteCount][0],gmTemp[voteCount][1]);
			OutputLog(LOG_CHAT,str);

			SendClientMessageToAll(0xffffffff,"Glosowanie zakonczone.");
		} else {
//			format(str,sizeof str,"%s ~n~ ~n~~y~%2d       ~r~%d   ~w~/TAK   |   /NIE   ~r~%d",gmTemp[voteQuestion],gmTemp[voteTimeleft],gmTemp[voteCount][0],gmTemp[voteCount][1]);
//			format(str,sizeof str,"%s %s~n~~y~%2d       ~r~%d   ~y~~h~~h~/TAK   |   /NIE   ~r~%d",
			format(str,sizeof str,"%s %s~n~~y~%2d       ~g~%d   ~w~/TAK   ~r~%d   ~w~/NIE",
					gmTemp[voteQuestion],	(strlen(gmTemp[voteQuestion])<35) ? ("~n~ ") : (" "),
					gmTemp[voteTimeleft],gmTemp[voteCount][0],gmTemp[voteCount][1]);
			TextDrawSetString(gTextDraw[TD_VOTING],str);
		}	
		
		gmTemp[interval_voteCheck]=sec;
	}
	if (gmTemp[voteTimeleft]==-1 && (sec-gmTemp[interval_voteCheck])>=5) {
		TextDrawHideForAll(gTextDraw[TD_VOTING]);	
		gmTemp[interval_voteCheck]=sec;
		gmTemp[voteTimeleft]=0;
	}

	//  --- ATTRACTION UPDATE: CHOWANY ---
	if(aData[A_CHOWANY][aState] == A_STATE_ON && ((sec - gmTemp[interval_aChowanyUpdate]) >= 1))
	{
		new
			bool:aBreak=false;
		if(tick - gmTemp[aChowanyStartTick] >= 900000) // 15 minut
		{
			aBreak=true;

			foreach(playerid)
				Msg(playerid, COLOR_INFO3, "Chowany zostal przerwany z powodu limitu czasu na ukonczenie.");
		}
		if (!aBreak)
			CH_Update();
		else
			CH_Finish();

		gmTemp[interval_aChowanyUpdate] = sec;
	}

	if(aData[A_SPS][aState] == A_STATE_ON && ((sec - gmTemp[interval_aSPSUpdate]) >= 1))
	{
		new
			bool:aBreak=false;
		if(tick - gmTemp[aSPSStartTick] >= 1000*10*60) // 10 minut
		{
			aBreak=true;

			foreach(playerid)
				Msg(playerid, COLOR_INFO3, "SPS zostal przerwany z powodu limitu czasu na ukonczenie.");
		}
		if (!aBreak)
			SPS_Update();
		else
			SPS_Finish();

		gmTemp[interval_aSPSUpdate] = sec;
	}



	//	--- ATTRACTION UPDATE: DERBY ---
	if(aData[A_DERBY][aState] == A_STATE_ON && (sec - gmTemp[interval_aDerbyUpdate]) >= 5) {
		if(tick - gmTemp[aDerbyStartTick] >= 1200000) // 20 minut
		{			
			foreach(playerid)
				Msg(playerid, COLOR_INFO3, TXT(playerid, 430)); // Derby zosta³o przerwane z powodu limitu czasu na ukoñczenie.
		} else {
			Derby_Recount();
		}
		gmTemp[interval_aDerbyUpdate] = sec;
	}

	//  --- ATTRACTION UPDATE: RACE ---
	if (aData[A_RACE][aState] == A_STATE_ON && (sec - gmTemp[interval_aRaceUpdate]) >= 5){
		Race_Recount();
		gmTemp[interval_aRaceUpdate]=sec;
	}

	//  --- ATTRACTION UPDATE: RACE ---
	if (aData[A_DRIFT][aState] == A_STATE_ON && (sec - gmTemp[interval_aDriftUpdate]) >= 5){
		Drift_Recount();
		gmTemp[interval_aDriftUpdate]=sec;
	}





	//  --- ATTRACTION UPDATE: WG ---
	
	if(aData[A_WG][aState] == A_STATE_ON && (sec - gmTemp[interval_aWGUpdate]) >= 3)
	{
		new
		 bool:aBreak = false;

		if(tick - gmTemp[aWGStartTick] >= 1200000) // 20 minut
		{
			aBreak = true;
			
			foreach(playerid)
				Msg(playerid, COLOR_INFO3, TXT(playerid, 430)); // WG zosta³o przerwane z powodu limitu czasu na ukoñczenie.
		}
		
		if(!aBreak)
			WG_Update();
		
		if(aBreak)
			WG_Finish();
	
		gmTemp[interval_aWGUpdate] = sec;
	}
	
	//  --- ATTRACTION UPDATE: LABIRYNT ---
	
	if(aData[A_LABIRYNT][aState] == A_STATE_ON && (sec - gmTemp[interval_aLabiryntUpdate]) >= 8)
	{
		labirynt_AttractionUpdate(tick);
		gmTemp[interval_aLabiryntUpdate] = sec;
	}
	
	//  --- ATTRACTION UPDATE: STRZELNICA ---
	
	if(aData[A_STRZELNICA][aState] == A_STATE_ON && (sec - gmTemp[interval_aStrzelnicaUpdate]) >= 5)
	{
		new
		 aPlayers = 0,
		 bool:aBreak = false,
		 aLastPlayer,
		 buffer[128];
		
		if(!IsPlayerConnected(gmTemp[aStrzelnicaShooter]) || !IsPlayerInCube(gmTemp[aStrzelnicaShooter], 305.5774, -125.0515, 1006.8765, 269.3169, -145.3345, 1003.0625))
		{
			aBreak = true;
			
			foreach(playerid)
				Msg(playerid, COLOR_INFO3, TXT(playerid, 287)); // Strzelnica zosta³a przerwana z powodu braku graczy.
		}
		else if(tick - gmTemp[aStrzelnicaStartTick] >= 900000) // 15 minut
		{
			aBreak = true;
			
			foreach(playerid)
				Msg(playerid, COLOR_INFO3, TXT(playerid, 428)); // Strzelnica zosta³a przerwana z powodu limitu czasu na ukoñczenie.
		}
		
		if(aBreak)
		{
			aData[A_STRZELNICA][aState] = A_STATE_OFF;
		
			for(new i = 0; i < gmTemp[aStrzelnicaMaxPlayers]; i++)
			{
				if(gmTemp[aStrzelnicaPlayers][i] == INVALID_PLAYER_ID) continue;
				
				pData[gmTemp[aStrzelnicaPlayers][i]][pAttraction] = A_NONE;
				SpawnPlayer(gmTemp[aStrzelnicaPlayers][i]);
			}
		}
		else
		{
			for(new i = 0; i < gmTemp[aStrzelnicaMaxPlayers]; i++)
			{
				if(gmTemp[aStrzelnicaPlayers][i] == INVALID_PLAYER_ID || gmTemp[aStrzelnicaPlayers][i] == gmTemp[aStrzelnicaShooter]) continue;
				
				if(pData[gmTemp[aStrzelnicaPlayers][i]][pAttraction] == A_STRZELNICA && IsPlayerInCube(gmTemp[aStrzelnicaPlayers][i], 305.5774, -125.0515, 1006.8765, 269.3169, -145.3345, 1003.0625))
				{
					if (random(2)==1) SetPlayerPos(gmTemp[aStrzelnicaPlayers][i], 286.28+random(5)-2.5, -133.93+random(5)-2.5, 1004.16);
					aPlayers++;
					aLastPlayer = gmTemp[aStrzelnicaPlayers][i];
				}
			}
			
			if(aPlayers == 1)
			{
				foreach(playerid)
				{
					if(pData[playerid][pAttraction] == A_STRZELNICA) pData[playerid][pAttraction] = A_NONE;
					if(playerid == aLastPlayer) continue;
					
					format(buffer, sizeof buffer, TXT(playerid, 288), GetPlayerNick(aLastPlayer));
					Msg(playerid, COLOR_INFO3, buffer); // "xxx" wygrywa strzelnicê!
				}
				
				Msg(aLastPlayer, COLOR_INFO2, TXT(aLastPlayer, 289)); // Wygrywasz strzelnicê! +10 respekt
				
				GivePlayerScore(aLastPlayer, 10, "wygrana na strzelnicy");
				
				aData[A_STRZELNICA][aState] = A_STATE_OFF;
				
				pData[aLastPlayer][pAttraction] = A_NONE;
				pData[gmTemp[aStrzelnicaShooter]][pAttraction] = A_NONE;
				
				SpawnPlayer(aLastPlayer);
				SetPlayerHealth(gmTemp[aStrzelnicaShooter],100.0);
				SpawnPlayer(gmTemp[aStrzelnicaShooter]);
			}
			else if(aPlayers == 0)
			{
				aData[A_STRZELNICA][aState] = A_STATE_OFF;
		
				for(new i = 0; i < gmTemp[aStrzelnicaMaxPlayers]; i++)
				{
					if(gmTemp[aStrzelnicaPlayers][i] == INVALID_PLAYER_ID) continue;
					
					pData[gmTemp[aStrzelnicaPlayers][i]][pAttraction] = A_NONE;
					SpawnPlayer(gmTemp[aStrzelnicaPlayers][i]);
				}
			
				foreach(playerid)
				{
					Msg(playerid, COLOR_INFO3, TXT(playerid, 287)); // Strzelnica zosta³a przerwana z powodu braku graczy.
				}
			}
		}
	
		gmTemp[interval_aStrzelnicaUpdate] = sec;
	}

	//  --- ATTRACTION TEXTDRAW UPDATE ---
	
	if((sec - gmTemp[interval_aTextDrawUpdate]) >= 2)
	{
		
		new
		 szTitles[256],
		 szStates[256],
		 aPlayers[MAX_ATTRACTIONS],
		 buffer[128];

		foreach(playerid){
			if(pData[playerid][aChowany]) 		aPlayers[A_CHOWANY]++;
			if(pData[playerid][aSPS]) 			aPlayers[A_SPS]++;
			if(pData[playerid][aDerby]) 		aPlayers[A_DERBY]++;
			if(pData[playerid][aLabirynt]) 		aPlayers[A_LABIRYNT]++;
			if(pData[playerid][aRace]) 			aPlayers[A_RACE]++;
			if(pData[playerid][aDrift]) 		aPlayers[A_DRIFT]++;
			if(pData[playerid][aWG]) 			aPlayers[A_WG]++;
			if(pData[playerid][aStrzelnica]) 	aPlayers[A_STRZELNICA]++;
		}
		
		
		for(new i = 0; i < MAX_ATTRACTIONS; i++)
		{
			if(aData[i][aState] == A_STATE_OFF && aPlayers[i] >= aData[i][aStartPlayers])
			{
				aData[i][aState] = A_STATE_STARTING;
				aData[i][aStartTick] = GetTickCount() + (aData[i][aStartingTime] * 1000);
				format(buffer, sizeof buffer, "{b}%s{/b} rozpocznie siê za {b}%i %s{/b}.", aData[i][aName], aData[i][aStartingTime], dli(aData[i][aStartingTime],"sekunde","sekundy","sekund"));
				foreach(playerid)
				{
					Msg(playerid, COLOR_INFO3, buffer, gmTemp[pPlayerCount]>10 ? false : true);
				}
			}
			else if(aData[i][aState] == A_STATE_STARTING && aPlayers[i] < aData[i][aStartPlayers])
			{
				aData[i][aState] = A_STATE_OFF;
				
				foreach(playerid)
				{
					switch(i)
					{
						case A_CHOWANY: 	Msg(playerid, COLOR_INFO3, "Brak dostatecznej liczby graczy na {b}chowanego{/b}, zapisy ponowione.");
						case A_SPS: 		Msg(playerid, COLOR_INFO3, "Brak dostatecznej liczby graczy na {b}SPS{/b}, zapisy ponowione.");
						case A_DERBY: 		Msg(playerid, COLOR_INFO3, "Brak dostatecznej liczby graczy na {b}derby{/b}, zapisy ponowione.");
						case A_LABIRYNT: 	Msg(playerid, COLOR_INFO3, "Brak dostatecznej liczby graczy na {b}labirynt{/b}, zapisy ponowione.");
						case A_RACE: 		Msg(playerid, COLOR_INFO3, "Brak dostatecznej liczby graczy na {b}race{/b}, zapisy ponowione.");
						case A_DRIFT: 		Msg(playerid, COLOR_INFO3, "Brak dostatecznej liczby graczy na {b}Drifting{/b}, zapisy ponowione.");
						case A_WG: 			Msg(playerid, COLOR_INFO3, "Brak dostatecznej liczby graczy na {b}WG{/b}, zapisy ponowione.");
						case A_STRZELNICA: 	Msg(playerid, COLOR_INFO3, "Brak dostatecznej liczby graczy na {b}strzelnice{/b}, zapisy ponowione.");
					}
				}
			}
			else if(aData[i][aState] == A_STATE_STARTING && aData[i][aStartTick] <= GetTickCount())
			{
				aData[i][aState] = A_STATE_ON;
				
				switch(i)
				{
					case A_CHOWANY: 	Chowany_Start();
					case A_SPS: 		SPS_Start();
					case A_DERBY: 		Derby_Start();
					case A_LABIRYNT: 	Labirynt_Start();
					case A_RACE: 		Race_Start();
					case A_DRIFT: 		Drifting_Start();
					case A_WG: 			WG_Start();
					case A_STRZELNICA: 	Strzelnica_Start();
				}
				if (i==A_CHOWANY || i==A_LABIRYNT || i==A_RACE || i==A_DRIFT)
					format(buffer, sizeof buffer, "{b}%s{/b} wystartowa³! Zapisy na nastêpn¹ kolejkê rozpoczête.", aData[i][aName]);
				else if (i==A_WG || i==A_SPS)
					format(buffer, sizeof buffer, "{b}%s{/b} wystartowa³o! Zapisy na nastêpn¹ kolejkê rozpoczête.", aData[i][aName]);
				else
					format(buffer, sizeof buffer, "{b}%s{/b} wystartowa³y! Zapisy na nastêpn¹ kolejkê rozpoczête.", aData[i][aName]);

				foreach(playerid)
					Msg(playerid, COLOR_INFO3, buffer, gmTemp[pPlayerCount]>10 ? false : true);
			}
		}
		
		for(new i = 0; i < MAX_ATTRACTIONS; i++)
		{
//			if (i!=A_DRIFT)
			switch(aData[i][aState])
			{
				case A_STATE_OFF:
				{
					format(buffer, sizeof buffer, "~g~~h~%s~n~", aData[i][aName]);
					strcat(szTitles, buffer);
					
					format(buffer, sizeof buffer, "~g~~h~Zapisy (%i/%i)~n~", aPlayers[i], aData[i][aStartPlayers]);
					strcat(szStates, buffer);
				}
				case A_STATE_STARTING:
				{
					format(buffer, sizeof buffer, "~y~%s~n~", aData[i][aName]);
					strcat(szTitles, buffer);
					
					format(buffer, sizeof buffer, "~y~Startuje (%i)~n~", aPlayers[i], aData[i][aStartPlayers]);
					strcat(szStates, buffer);
				}
				case A_STATE_ON:
				{
					format(buffer, sizeof buffer, "~r~~h~~h~%s~n~", aData[i][aName]);
					strcat(szTitles, buffer);
					
					strcat(szStates, "~r~~h~~h~TRWA~n~");
				}
			}
		}
		
		new
			_ac,_acPRO;
		
		foreach(playerid) {
			if (pTemp[playerid][onArena]==ARENA_PRODM)
				_acPRO++;
			else if (pData[playerid][pAttraction]==A_ARENA || pTemp[playerid][onArena]>ARENA_NONE)
				_ac++;
			}
		

		strins(szTitles, "~g~~h~~h~/PRO~n~", 0);
		format(buffer, sizeof buffer, "~g~~h~(%i)~n~", _acPRO);
		strins(szStates, buffer, 0);

		strins(szTitles, "~g~~h~/Areny~n~", 0);
		
		format(buffer, sizeof buffer, "~g~~h~(%i)~n~", _ac+_acPRO);
		strins(szStates, buffer, 0);

		
		TextDrawSetString(gTextDraw[TD_ATTRACTION_TITLES], szTitles);
		TextDrawSetString(gTextDraw[TD_ATTRACTION_STATES], szStates);
		
		gmTemp[interval_aTextDrawUpdate] = sec;
	}

	
	
	//  --- STARS UPDATE + RESPECT ADDITION --- 

	
	//  --- JAIL ---
	
	if((sec - gmTemp[interval_jailCheck]) >= 25)
	{
		foreach(playerid)
		{
			if(pData[playerid][jail] > 0 && IsPlayerSpawned(playerid))
			{
				if(pData[playerid][jail] <= sec)
				{
					UnjailPlayer(playerid);
					Msg(playerid, COLOR_INFO, TXT(playerid, 229));
				}
				else
				{
					new
					 remainTime = ((pData[playerid][jail] - sec) / 60) + 1,
					 buffer[64];
					
					format(buffer, sizeof buffer, TXT(playerid, 220), remainTime, GetPeriodName2(playerid, 'm', remainTime));
					GameTextForPlayer(playerid, buffer, 15000, 3);
					
					if(!IsPlayerInDynamicArea(playerid,DAs[eDA_jail]) && GetPlayerCurrentSession(playerid)>30)
					{
						Msg(playerid, COLOR_INFO2, TXT(playerid, 223));
						MoveToJail(playerid);
					}
				}
			}
		}
		
		gmTemp[interval_jailCheck] = sec;
	}
	if((sec - gmTemp[interval_gangs]) >= (300+gmTemp[pPlayerCount])) {
		printf("Zapisywanie danych gangow");
		gangs_saveGangData();
		printf("Zapisywanie danych gangow zakonczone.");
		gmTemp[interval_gangs]=sec;
	}
	//  --- STATS UPDATE ---
	
	if((sec - gmTemp[interval_statsUpdate]) >= 
			gmTemp[pPlayerCount]>100 ? 7 : ( gmTemp[pPlayerCount]>50 ? 5 : 3 )
			)
	{

		new
		 _admins = 0,
		 _absadmins = 0,
		 _vips = 0,
		 _gms = 0,
		 _players = 0;

		foreach(playerid)
		{
				_players++;

				if(pData[playerid][loggedIn] && pData[playerid][vipEnabled]) _vips++;
				switch(pData[playerid][adminLevel]) {
					case LEVEL_ADMINHIDDEN:
						_absadmins++;
					case LEVEL_ADMIN1,LEVEL_ADMIN2,LEVEL_ADMIN3: {
						_admins++;
						_absadmins++;
					}
					case LEVEL_GM:
						_gms++;
				}

				if(pTemp[playerid][staleTime]<3 && pData[playerid][hudSetting][HUD_STATUSBAR])
					UpdatePlayerStatsTD(playerid);
	
		}
		gmTemp[pPlayerCount]=_players;
		gmTemp[pAdminCount]=_admins;
		gmTemp[pAbsAdminCount]=_absadmins;
		gmTemp[pGMCount]=_gms;
		gmTemp[pVipCount]=_vips;
		gmTemp[interval_statsUpdate] = sec;
	}


	//  --- ARTEFACT ---
	
	if((sec - gmTemp[interval_artefact]) >= 300)	// 5 minut
	{
		printf("Przeladowywanie domow");
		domy_Reload();
		printf("done");

/*		if(gmData[artefactOwner] != INVALID_PLAYER_ID)
			if (pTemp[gmData[artefactOwner]][staleTime]>30) {
				Msg(gmData[artefactOwner], COLOR_ERROR, "Niestety, zbyt dlugo byles/as AFK i {b}artefakt{/b} postanowil Cie opuscic!");
				DropArtefact(gmData[artefactOwner]);
			} else 	{	
				new
				 baseTime = (GetTickCount() / 1000) - pTemp[gmData[artefactOwner]][artefactStartTime];

				if (baseTime>30*60) baseTime=30*60;
				new bonus=floatround(baseTime*sqrt(gmTemp[pPlayerCount])/400);
				// pol godziny 1800*10/400
				if(pData[gmData[artefactOwner]][loggedIn])
					GivePlayerScore(gmData[artefactOwner], bonus, "Od artefaktu");
				GivePlayerMoney(gmData[artefactOwner], baseTime*gmTemp[pPlayerCount]*4);
			}*/
		
		gmTemp[interval_artefact] = sec;
	}

	if (gmTemp[performBenchmark]>0) {
		new buf[16];
		format(buf,sizeof buf, "~g~ %d~w~ms", GetTickCount()-tick);
		GameTextForAll(buf,1250,3);
		gmTemp[performBenchmark]--;
	}
	gmTemp[updateTimerRunning]=false;
}

SavePlayerData(playerid)
{
	if(pData[playerid][loggedIn])
	{
		UpdatePlayerAccountData(playerid);
		pTemp[playerid][lastSessionSaveTick] = GetTickCount();
/*		SetPlayerAccountDataInt(playerid, "session", StringToInt(GetPlayerAccountData(playerid, "session")) + ((GetTickCount() - pTemp[playerid][lastSessionSaveTick]) / 1000));
		SetPlayerAccountDataInt(playerid, "respect", pData[playerid][respect]);
		SetPlayerAccountDataInt(playerid, "wallet_money", GetPlayerMoney(playerid));
		SetPlayerAccountDataInt(playerid, "kill_count", pData[playerid][kills]);
		SetPlayerAccountDataInt(playerid, "teamkill_count", pData[playerid][teamkills]);
		SetPlayerAccountDataInt(playerid, "death_count", pData[playerid][deaths]);
		SetPlayerAccountDataInt(playerid, "suicide_count", pData[playerid][suicides]);
		SetPlayerAccountDataInt(playerid, "skill", pData[playerid][skill]);*/
		

	}
}

GetDialogList(playerid, dialogid)
{
	new
	 szList[1024];
	 
	switch(dialogid)
	{ 
		case DIALOG_ARENASOLO_SELECT:
		{
			for(new i=0;i<sizeof DATA_arenysolo;i++)
			{
				format(szList,sizeof szList,"%s%s%s", szList, ((i>0)?("\n"):("")), DATA_arenysolo[i][eas_nazwa]);
			}
			return szList;
		}
		case DIALOG_WEAPON_QUICKBUY:
		{
			for(new i=0;i<sizeof quickbuyWeapons;i++)
			if (quickbuyWeapons[i][ewd_quickBuy])
				format(szList,sizeof szList,"%s%s{FCE206}%d$\t{ffffff}%s", szList, ((i>0)?("\n"):("")), quickbuyWeapons[i][ewd_baseCost],quickbuyWeapons[i][ewd_PLname]);
			return szList;
		}

		case DIALOG_CONFIG_ASETTINGS:
		{
			for(new i = 0; i < MAX_ATTRACTIONS; i++)
			{
				strcat(szList, aData[i][aName]);
					
				if(i < MAX_ATTRACTIONS)
					strcat(szList, "\n");
			}
		}
	}

	if(pData[playerid][language] == LANG_POLISH)
	{
		switch(dialogid)
		{
			case DIALOG_CONFIG_MAIN:
			{
				for(new i = 0; i < sizeof PL_fNames_config; i++)
				{
					new
					 buffer[64];
						 
					switch(i)
					{
						case 2:
						{
							format(buffer, sizeof buffer, "%s [%i]", PL_fNames_config[i], gmData[maxPing]);
							strcat(szList, buffer);
						}
						
						case 5:
						{
							format(buffer, sizeof buffer, "%s [%s]", PL_fNames_config[i], gmData[chatColors] ? ("ON") : ("OFF"));
							strcat(szList, buffer);
						}
						
						default: strcat(szList, PL_fNames_config[i]);
					}
					
					if(i < sizeof PL_fNames_config)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_CONFIG_PCOLORS:
			{
				for(new i = 0; i < sizeof PL_fNames_config_pColors; i++)
				{
					new
					 buffer[64];
					
					switch(i)
					{
						case 0: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_normalUser]), PL_fNames_config_pColors[i]);
						case 1: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_fsUser]), PL_fNames_config_pColors[i]);
						case 2: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_vipUser]), PL_fNames_config_pColors[i]);
						case 3: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_gmUser]), PL_fNames_config_pColors[i]);
						case 4: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_lvl1User]), PL_fNames_config_pColors[i]);
						case 5: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_lvl2User]), PL_fNames_config_pColors[i]);
						case 6: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_adminUser]), PL_fNames_config_pColors[i]);
					}
					
					strcat(szList, buffer);
					
					if(i < sizeof PL_fNames_config_pColors)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_CONFIG_CCOLORS:
			{
				for(new i = 0; i < sizeof PL_fNames_config_cColors; i++)
				{
					new
					 buffer[64];
					
					switch(i)
					{
						case 0: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo]), PL_fNames_config_cColors[i]);
						case 1: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo2]), PL_fNames_config_cColors[i]);
						case 2: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo3]), PL_fNames_config_cColors[i]);
						case 3: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatError]), PL_fNames_config_cColors[i]);
						case 4: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_joinInfo]), PL_fNames_config_cColors[i]);
						case 5: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_leaveInfo]), PL_fNames_config_cColors[i]);
						case 6: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatGM]), PL_fNames_config_cColors[i]);
						case 7: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatAdmin]), PL_fNames_config_cColors[i]);
						case 8: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatAdmin3]), PL_fNames_config_cColors[i]);
						case 9: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatPM]), PL_fNames_config_cColors[i]);
						case 10: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatIC]), PL_fNames_config_cColors[i]);
						case 11: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatME]), PL_fNames_config_cColors[i]);
						case 12: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_vipSay]), PL_fNames_config_cColors[i]);
						case 13: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatVip]), PL_fNames_config_cColors[i]);
						case 14: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo_HL]), PL_fNames_config_cColors[i]);
						case 15: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo2_HL]), PL_fNames_config_cColors[i]);
						case 16: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo3_HL]), PL_fNames_config_cColors[i]);
						case 17: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatError_HL]), PL_fNames_config_cColors[i]);
						case 18: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_joinInfo_HL]), PL_fNames_config_cColors[i]);
						case 19: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_leaveInfo_HL]), PL_fNames_config_cColors[i]);
						case 20: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatGM_HL]), PL_fNames_config_cColors[i]);
						case 21: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatAdmin_HL]), PL_fNames_config_cColors[i]);
						case 22: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatAdmin3_HL]), PL_fNames_config_cColors[i]);
						case 23: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatPM_HL]), PL_fNames_config_cColors[i]);
						case 24: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatIC_HL]), PL_fNames_config_cColors[i]);
						case 25: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatME_HL]), PL_fNames_config_cColors[i]);
						case 26: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatME_HL]), PL_fNames_config_cColors[i]);
						case 27: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_vipSay_HL]), PL_fNames_config_cColors[i]);
						case 28: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatVip_HL]), PL_fNames_config_cColors[i]);
					}
					
					strcat(szList, buffer);
					
					if(i < sizeof PL_fNames_config_cColors)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_HELP_MAIN:
			{
				for(new i = 0; i < sizeof PL_fNames_help; i++)
				{
					strcat(szList, PL_fNames_help[i]);
					
					if(i < sizeof PL_fNames_help)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_HELP_CMD:
			{
				for(new i = 0; i < sizeof PL_fNames_help_cmd; i++)
				{
					strcat(szList, PL_fNames_help_cmd[i]);
					
					if(i < sizeof PL_fNames_help_cmd)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_BANK_MAIN:
			{
				for(new i = 0; i < sizeof PL_fNames_bank; i++)
				{
					strcat(szList, PL_fNames_bank[i]);
					
					if(i < sizeof PL_fNames_bank)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_STATS_MAIN:
			{
				for(new i = 0; i < sizeof PL_fNames_stats; i++)
				{
					strcat(szList, PL_fNames_stats[i]);
					
					if(i < sizeof PL_fNames_stats)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_TOP_MAIN:
			{
				for(new i = 0; i < sizeof PL_fNames_top; i++)
				{
					strcat(szList, PL_fNames_top[i]);
					
					if(i < sizeof PL_fNames_top)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_HUD:
			{
				new
				 buffer[128];
				strcat(szList,"{FFD700}Przelacz wszystkie\n");

				for(new i = 0; i < sizeof PL_fNames_hud; i++)
				{
					if(pData[playerid][hudSetting][i]) format(buffer, sizeof buffer, "{7DC158}[ON]	%s", PL_fNames_hud[i]);
					else format(buffer, sizeof buffer, "{C15E58}[OFF]	%s", PL_fNames_hud[i]);
					strcat(szList, buffer);
					
					if(i < sizeof PL_fNames_hud)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_CONFIG_ASETTINGS_LIST:
			{
				for(new i = 0; i < sizeof PL_fNames_config_attraction; i++)
				{
					strcat(szList, PL_fNames_config_attraction[i]);
					
					if(i < sizeof PL_fNames_config_attraction)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_CONFIG_CENSORSHIP:
			{
				for(new i = 0; i < sizeof PL_fNames_censorship_off; i++)
				{
					strcat(szList, (gmData[censorship]) ? PL_fNames_censorship_on[i] : PL_fNames_censorship_off[i]);
					
					if(i < sizeof PL_fNames_censorship_off)
						strcat(szList, "\n");
				}
			}
		}
	}
	else if(pData[playerid][language] == LANG_ENGLISH)
	{
		switch(dialogid)
		{
			case DIALOG_CONFIG_MAIN:
			{
				for(new i = 0; i < sizeof EN_fNames_config; i++)
				{
					new
					 buffer[64];
						 
					switch(i)
					{
						case 2:
						{
							format(buffer, sizeof buffer, "%s [%i]", EN_fNames_config[i], gmData[maxPing]);
							strcat(szList, buffer);
						}
						
						case 5:
						{
							format(buffer, sizeof buffer, "%s [%s]", EN_fNames_config[i], gmData[chatColors] ? ("ON") : ("OFF"));
							strcat(szList, buffer);
						}
						
						default: strcat(szList, EN_fNames_config[i]);
					}

					if(i < sizeof EN_fNames_config)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_CONFIG_PCOLORS:
			{
				for(new i = 0; i < sizeof EN_fNames_config_pColors; i++)
				{
					new
					 buffer[64];
					
					switch(i)
					{
						case 0: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_normalUser]), EN_fNames_config_pColors[i]);
						case 1: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_fsUser]), EN_fNames_config_pColors[i]);
						case 2: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_vipUser]), EN_fNames_config_pColors[i]);
						case 3: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_gmUser]), EN_fNames_config_pColors[i]);
						case 4: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_lvl1User]), EN_fNames_config_pColors[i]);
						case 5: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_lvl2User]), EN_fNames_config_pColors[i]);
						case 6: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_adminUser]), EN_fNames_config_pColors[i]);
					}
					
					strcat(szList, buffer);
					
					if(i < sizeof EN_fNames_config_pColors)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_CONFIG_CCOLORS:
			{
				for(new i = 0; i < sizeof EN_fNames_config_cColors; i++)
				{
					new
					 buffer[64];
					
					switch(i)
					{
						case 0: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo]), EN_fNames_config_cColors[i]);
						case 1: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo2]), EN_fNames_config_cColors[i]);
						case 2: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo3]), EN_fNames_config_cColors[i]);
						case 3: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatError]), EN_fNames_config_cColors[i]);
						case 4: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_joinInfo]), EN_fNames_config_cColors[i]);
						case 5: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_leaveInfo]), EN_fNames_config_cColors[i]);
						case 6: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatGM]), EN_fNames_config_cColors[i]);
						case 7: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatAdmin]), EN_fNames_config_cColors[i]);
						case 8: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatAdmin3]), EN_fNames_config_cColors[i]);
						case 9: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatPM]), EN_fNames_config_cColors[i]);
						case 10: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatIC]), EN_fNames_config_cColors[i]);
						case 11: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatME]), EN_fNames_config_cColors[i]);
						case 12: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo_HL]), EN_fNames_config_cColors[i]);
						case 13: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo2_HL]), EN_fNames_config_cColors[i]);
						case 14: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo3_HL]), EN_fNames_config_cColors[i]);
						case 15: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatError_HL]), EN_fNames_config_cColors[i]);
						case 16: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_joinInfo_HL]), EN_fNames_config_cColors[i]);
						case 17: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_leaveInfo_HL]), EN_fNames_config_cColors[i]);
						case 18: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatGM_HL]), EN_fNames_config_cColors[i]);
						case 19: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatAdmin_HL]), EN_fNames_config_cColors[i]);
						case 20: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatAdmin3_HL]), EN_fNames_config_cColors[i]);
						case 21: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatPM_HL]), EN_fNames_config_cColors[i]);
						case 22: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatIC_HL]), EN_fNames_config_cColors[i]);
						case 23: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatME_HL]), EN_fNames_config_cColors[i]);
					}
					
					strcat(szList, buffer);
					
					if(i < sizeof EN_fNames_config_cColors)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_HELP_MAIN:
			{
				for(new i = 0; i < sizeof EN_fNames_help; i++)
				{
					strcat(szList, EN_fNames_help[i]);
					
					if(i < sizeof EN_fNames_help)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_HELP_CMD:
			{
				for(new i = 0; i < sizeof EN_fNames_help_cmd; i++)
				{
					strcat(szList, EN_fNames_help_cmd[i]);
					
					if(i < sizeof EN_fNames_help_cmd)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_BANK_MAIN:
			{
				for(new i = 0; i < sizeof EN_fNames_bank; i++)
				{
					strcat(szList, EN_fNames_bank[i]);
					
					if(i < sizeof EN_fNames_bank)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_STATS_MAIN:
			{
				for(new i = 0; i < sizeof EN_fNames_stats; i++)
				{
					strcat(szList, EN_fNames_stats[i]);
					
					if(i < sizeof EN_fNames_stats)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_TOP_MAIN:
			{
				for(new i = 0; i < sizeof EN_fNames_top; i++)
				{
					strcat(szList, EN_fNames_top[i]);
					
					if(i < sizeof EN_fNames_top)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_HUD:
			{
				new
				 buffer[128];
				 
				for(new i = 0; i < sizeof EN_fNames_hud; i++)
				{
					if(pData[playerid][hudSetting][i]) format(buffer, sizeof buffer, "{7DC158}[ON]	%s", EN_fNames_hud[i]);
					else format(buffer, sizeof buffer, "{C15E58}[OFF]	%s", EN_fNames_hud[i]);
					strcat(szList, buffer);
					
					if(i < sizeof EN_fNames_hud)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_CONFIG_ASETTINGS_LIST:
			{
				for(new i = 0; i < sizeof EN_fNames_config_attraction; i++)
				{
					strcat(szList, EN_fNames_config_attraction[i]);
					
					if(i < sizeof EN_fNames_config_attraction)
						strcat(szList, "\n");
				}
			}
			
			case DIALOG_CONFIG_CENSORSHIP:
			{
				for(new i = 0; i < sizeof EN_fNames_censorship_off; i++)
				{
					strcat(szList, (gmData[censorship]) ? EN_fNames_censorship_on[i] : EN_fNames_censorship_off[i]);
					
					if(i < sizeof EN_fNames_censorship_off)
						strcat(szList, "\n");
				}
			}
		}
	}
	
	return szList;
}

GetDialogContent(playerid, dialogid)
{
	new
	 szContent[1024];

	if(pData[playerid][language] == LANG_POLISH)
	{
		switch(dialogid)
		{
			case DIALOG_HELP_RULES:
			{
				for(new i = 0; i < sizeof PL_szHelpRules; i++)
				{
					strcat(szContent, PL_szHelpRules[i]);
					
					if(i < sizeof PL_szHelpRules)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_RESPECT:
			{
				for(new i = 0; i < sizeof PL_szHelpRespect; i++)
				{
					strcat(szContent, PL_szHelpRespect[i]);
					
					if(i < sizeof PL_szHelpRespect)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_STARS:
			{
				for(new i = 0; i < sizeof PL_szHelpStars; i++)
				{
					strcat(szContent, PL_szHelpStars[i]);
					
					if(i < sizeof PL_szHelpStars)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_VIP:
			{
				for(new i = 0; i < sizeof PL_szHelpVIP; i++)
				{
					strcat(szContent, PL_szHelpVIP[i]);
					
					if(i < sizeof PL_szHelpVIP)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_AUTHOR:
			{
				for(new i = 0; i < sizeof PL_szHelpAuthor; i++)
				{
					strcat(szContent, PL_szHelpAuthor[i]);
					
					if(i < sizeof PL_szHelpAuthor)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_CMD_GENERAL:
			{
				for(new i = 0; i < sizeof PL_szHelpCmdGeneral; i++)
				{
					strcat(szContent, PL_szHelpCmdGeneral[i]);
					
					if(i < sizeof PL_szHelpCmdGeneral)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_CMD_ATRACTIONS:
			{
				for(new i = 0; i < sizeof PL_szHelpCmdAtractions; i++)
				{
					strcat(szContent, PL_szHelpCmdAtractions[i]);
					
					if(i < sizeof PL_szHelpCmdAtractions)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_CMD_ACCOUNT:
			{
				for(new i = 0; i < sizeof PL_szHelpCmdAccount; i++)
				{
					strcat(szContent, PL_szHelpCmdAccount[i]);
					
					if(i < sizeof PL_szHelpCmdAccount)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_CMD_HOUSES:
			{
				for(new i = 0; i < sizeof PL_szHelpCmdHouses; i++)
				{
					strcat(szContent, PL_szHelpCmdHouses[i]);
					
					if(i < sizeof PL_szHelpCmdHouses)
						strcat(szContent, "\n");
				}
			}
			
/*			case DIALOG_HELP_CMD_RESPECT:
			{
				for(new i = 0; i < sizeof PL_szHelpCmdRespect; i++)
				{
					strcat(szContent, PL_szHelpCmdRespect[i]);
					
					if(i < sizeof PL_szHelpCmdRespect)
						strcat(szContent, "\n");
				}
			}*/
			
			case DIALOG_HELP_CMD_TELEPORTS:
			{
				for(new i = 0; i < sizeof PL_szHelpCmdTeleports; i++)
				{
					strcat(szContent, PL_szHelpCmdTeleports[i]);
					
					if(i < sizeof PL_szHelpCmdTeleports)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_CMD_ANIMATIONS:
			{
				for(new i = 0; i < sizeof PL_szHelpCmdAnimations; i++)
				{
					strcat(szContent, PL_szHelpCmdAnimations[i]);
					
					if(i < sizeof PL_szHelpCmdAnimations)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_CMD_VIP:
			{
				for(new i = 0; i < sizeof PL_szHelpCmdVIP; i++)
				{
					strcat(szContent, PL_szHelpCmdVIP[i]);
					
					if(i < sizeof PL_szHelpCmdVIP)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_CMD_ADMIN:
			{
				for(new i = 0; i < sizeof PL_szHelpCmdAdmin; i++)
				{
					strcat(szContent, PL_szHelpCmdAdmin[i]);
					
					if(i < sizeof PL_szHelpCmdAdmin)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_STATS_PLAYER:
			{
				for(new i = 0; i < sizeof PL_szStatsPlayer; i++)
				{
					strcat(szContent, PL_szStatsPlayer[i]);
					
					if(i < sizeof PL_szStatsPlayer)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_STATS_SERVER:
			{
				for(new i = 0; i < sizeof PL_szStatsServer; i++)
				{
					strcat(szContent, PL_szStatsServer[i]);
					
					if(i < sizeof PL_szStatsServer)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_TOP_RESPECT:
			{
				for(new i = 0; i < sizeof PL_szTopRespect; i++)
				{
					strcat(szContent, PL_szTopRespect[i]);
					
					if(i < sizeof PL_szTopRespect)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_TOP_FRAGS:
			{
				for(new i = 0; i < sizeof PL_szTopFrags; i++)
				{
					strcat(szContent, PL_szTopFrags[i]);
					
					if(i < sizeof PL_szTopFrags)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_TOP_TIMEPLAYED:
			{
				for(new i = 0; i < sizeof PL_szTopPlayedTime; i++)
				{
					strcat(szContent, PL_szTopPlayedTime[i]);
					
					if(i < sizeof PL_szTopPlayedTime)
						strcat(szContent, "\n");
				}
			}
		}
	}
	else if(pData[playerid][language] == LANG_ENGLISH)
	{
		switch(dialogid)
		{
			case DIALOG_HELP_RULES:
			{
				for(new i = 0; i < sizeof EN_szHelpRules; i++)
				{
					strcat(szContent, EN_szHelpRules[i]);
					
					if(i < sizeof EN_szHelpRules)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_RESPECT:
			{
				for(new i = 0; i < sizeof EN_szHelpRespect; i++)
				{
					strcat(szContent, EN_szHelpRespect[i]);
					
					if(i < sizeof EN_szHelpRespect)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_STARS:
			{
				for(new i = 0; i < sizeof EN_szHelpStars; i++)
				{
					strcat(szContent, EN_szHelpStars[i]);
					
					if(i < sizeof EN_szHelpStars)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_VIP:
			{
				for(new i = 0; i < sizeof EN_szHelpVIP; i++)
				{
					strcat(szContent, EN_szHelpVIP[i]);
					
					if(i < sizeof EN_szHelpVIP)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_AUTHOR:
			{
				for(new i = 0; i < sizeof EN_szHelpAuthor; i++)
				{
					strcat(szContent, EN_szHelpAuthor[i]);
					
					if(i < sizeof EN_szHelpAuthor)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_CMD_GENERAL:
			{
				for(new i = 0; i < sizeof EN_szHelpCmdGeneral; i++)
				{
					strcat(szContent, EN_szHelpCmdGeneral[i]);
					
					if(i < sizeof EN_szHelpCmdGeneral)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_CMD_ATRACTIONS:
			{
				for(new i = 0; i < sizeof EN_szHelpCmdAtractions; i++)
				{
					strcat(szContent, EN_szHelpCmdAtractions[i]);
					
					if(i < sizeof EN_szHelpCmdAtractions)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_CMD_ACCOUNT:
			{
				for(new i = 0; i < sizeof EN_szHelpCmdAccount; i++)
				{
					strcat(szContent, EN_szHelpCmdAccount[i]);
					
					if(i < sizeof EN_szHelpCmdAccount)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_CMD_HOUSES:
			{
				for(new i = 0; i < sizeof EN_szHelpCmdHouses; i++)
				{
					strcat(szContent, EN_szHelpCmdHouses[i]);
					
					if(i < sizeof EN_szHelpCmdHouses)
						strcat(szContent, "\n");
				}
			}
			
/*			case DIALOG_HELP_CMD_RESPECT:
			{
				for(new i = 0; i < sizeof EN_szHelpCmdRespect; i++)
				{
					strcat(szContent, EN_szHelpCmdRespect[i]);
					
					if(i < sizeof EN_szHelpCmdRespect)
						strcat(szContent, "\n");
				}
			}*/
			
			case DIALOG_HELP_CMD_TELEPORTS:
			{
				for(new i = 0; i < sizeof EN_szHelpCmdTeleports; i++)
				{
					strcat(szContent, EN_szHelpCmdTeleports[i]);
					
					if(i < sizeof EN_szHelpCmdTeleports)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_CMD_ANIMATIONS:
			{
				for(new i = 0; i < sizeof EN_szHelpCmdAnimations; i++)
				{
					strcat(szContent, EN_szHelpCmdAnimations[i]);
					
					if(i < sizeof EN_szHelpCmdAnimations)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_CMD_VIP:
			{
				for(new i = 0; i < sizeof EN_szHelpCmdVIP; i++)
				{
					strcat(szContent, EN_szHelpCmdVIP[i]);
					
					if(i < sizeof EN_szHelpCmdVIP)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_HELP_CMD_ADMIN:
			{
				for(new i = 0; i < sizeof EN_szHelpCmdAdmin; i++)
				{
					strcat(szContent, EN_szHelpCmdAdmin[i]);
					
					if(i < sizeof EN_szHelpCmdAdmin)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_STATS_PLAYER:
			{
				for(new i = 0; i < sizeof EN_szStatsPlayer; i++)
				{
					strcat(szContent, EN_szStatsPlayer[i]);
					
					if(i < sizeof EN_szStatsPlayer)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_STATS_SERVER:
			{
				for(new i = 0; i < sizeof EN_szStatsServer; i++)
				{
					strcat(szContent, EN_szStatsServer[i]);
					
					if(i < sizeof EN_szStatsServer)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_TOP_RESPECT:
			{
				for(new i = 0; i < sizeof EN_szTopRespect; i++)
				{
					strcat(szContent, EN_szTopRespect[i]);
					
					if(i < sizeof EN_szTopRespect)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_TOP_FRAGS:
			{
				for(new i = 0; i < sizeof EN_szTopFrags; i++)
				{
					strcat(szContent, EN_szTopFrags[i]);
					
					if(i < sizeof EN_szTopFrags)
						strcat(szContent, "\n");
				}
			}
			
			case DIALOG_TOP_TIMEPLAYED:
			{
				for(new i = 0; i < sizeof EN_szTopPlayedTime; i++)
				{
					strcat(szContent, EN_szTopPlayedTime[i]);
					
					if(i < sizeof EN_szTopPlayedTime)
						strcat(szContent, "\n");
				}
			}
		}
	}
	
	return szContent;
}

c_f_PlayerColors(playerid, listitem)
{
	ShowPlayerDialog(playerid, DIALOG_CONFIG_PCOLORS_SET, DIALOG_STYLE_INPUT, (pData[playerid][language] == LANG_POLISH) ? PL_fNames_config_pColors[listitem] : EN_fNames_config_pColors[listitem], TXT(playerid, 79), TXT(playerid, 78), TXT(playerid, 77));
	
	pTemp[playerid][lastDialog] = DIALOG_CONFIG_PCOLORS_SET;
	pTemp[playerid][lastDialogItem] = listitem;
}

c_f_ChatColors(playerid, listitem)
{
	ShowPlayerDialog(playerid, DIALOG_CONFIG_CCOLORS_SET, DIALOG_STYLE_INPUT, (pData[playerid][language] == LANG_POLISH) ? PL_fNames_config_cColors[listitem] : EN_fNames_config_cColors[listitem], TXT(playerid, 79), TXT(playerid, 78), TXT(playerid, 77));
	
	pTemp[playerid][lastDialog] = DIALOG_CONFIG_CCOLORS_SET;
	pTemp[playerid][lastDialogItem] = listitem;
}

KickPlayer(playerid, bool:info = false)
{
	TogglePlayerControllable(playerid, false);
	SetCameraBehindPlayer(playerid);
	
	ShowElement(playerid, TDE_WYBIERALKA, true);
	if(info)
		Msg(playerid, COLOR_INFO2, TXT(playerid, 142));
	
	Kick(playerid);
}

FindNextPlayerToSpectate(playerid, bool:direction){
	if (pData[playerid][spectating]==INVALID_PLAYER_ID) return;
	new foundTarget=INVALID_PLAYER_ID;
	if (direction) {	// do przodu
		for (new i=pData[playerid][spectating]+1; i<=gmTemp[highestPlayerID] && foundTarget==INVALID_PLAYER_ID; i++)
			if (IsPlayerConnected(i) && IsPlayerSpawned(i))
				foundTarget=i;
		if (foundTarget==INVALID_PLAYER_ID)	// jeszcze nie znaleziono, lecimy od 0
		for (new i=0; i<=pData[playerid][spectating] && foundTarget==INVALID_PLAYER_ID; i++)
			if (IsPlayerConnected(i) && IsPlayerSpawned(i))
				foundTarget=i;
		
			
	}
	if (foundTarget==INVALID_PLAYER_ID) // nie znaleziono, przerywamy specowanie
		StopSpectating(playerid);
	else
		StartSpectating(playerid, foundTarget);

}

StartSpectating(playerid, targetplayerid)
{
	if(pData[playerid][pAttraction] != A_NONE)
	{
		Msg(playerid, COLOR_ERROR, TXT(playerid, 286));
		return;
	}
	
	if (pData[playerid][spectating]==INVALID_PLAYER_ID) {
		GetPlayerPosition(playerid, pTemp[playerid][specPosition][0], pTemp[playerid][specPosition][1], pTemp[playerid][specPosition][2], pTemp[playerid][specPosition][3]);
		pTemp[playerid][specInterior] = GetPlayerInterior(playerid);
		pTemp[playerid][specVirtualWorld] = GetPlayerVirtualWorld(playerid);
	}
	pData[playerid][spectating] = targetplayerid;
	TogglePlayerSpectating(playerid, true);
	SetPlayerInterior(playerid,GetPlayerInterior(targetplayerid));
	SetPlayerVirtualWorld(playerid,GetPlayerVirtualWorld(targetplayerid));
	
	

	new Float:HP,Float:AR;
	GetPlayerArmour(targetplayerid, AR);
	GetPlayerHealth(targetplayerid, HP);
	if (pData[targetplayerid][pAttraction]!=A_NONE)
		format(gstr, sizeof gstr, "%s (%d), HP: %3.0f AR: %3.0f, atrakcja: %s", GetPlayerNick(targetplayerid), targetplayerid, HP, AR, aData[pData[targetplayerid][pAttraction]][aName]);
	else
		format(gstr, sizeof gstr, "%s (%d), HP: %3.0f AR: %3.0f", GetPlayerNick(targetplayerid), targetplayerid, HP, AR);

	SCM(playerid, -1, " ");
	SCM(playerid, -1, gstr);

	if(IsPlayerInAnyVehicle(targetplayerid)) {
		new vid=GetPlayerVehicleID(targetplayerid);
		if (tVehicles[vid][vo_drift])
			Msg(playerid,COLOR_INFO,"Gracz jest w pojazdu drifterskim (szybkie naprawianie/odwracanie)",false);
		PlayerSpectateVehicle(playerid, vid);
	} else
		PlayerSpectatePlayer(playerid, targetplayerid);


	format(gstr, sizeof gstr, "%d", targetplayerid);
	cmd_cwep(playerid, gstr);
}

StopSpectating(playerid)
{
	pData[playerid][spectating] = INVALID_PLAYER_ID;
	pTemp[playerid][specPosReturn] = true;
	
	TogglePlayerSpectating(playerid, false);
}

FreezePlayer(playerid, ms)
{
	TogglePlayerControllable(playerid, false);
	
	KillTimer(pTemp[playerid][timerFreezePlayer]);
	pTemp[playerid][timerFreezePlayer] = SetTimerEx("FreezePlayer_Unfreeze", ms, false, "i", playerid);
}

public FreezePlayer_Unfreeze(playerid)
{
	TogglePlayerControllable(playerid, true);
}

public PutInVehicleAfterTeleport(playerid, vehicleid, bool:useOnce)
{
	new
	 Float:pVector[e_Vectors];
	
	GetPlayerPosition(playerid, pVector[X], pVector[Y], pVector[Z], pVector[A]);
	
	new buffer[128];
	format(buffer, 128, "%f %f %f %f", pVector[X], pVector[Y], pVector[Z], pVector[A]);
	Msg(playerid, 0xFFFFFFFF, buffer);
	
	pData[playerid][lastVehicle] = CreateVehicle(vehicleid, pVector[X], pVector[Y], pVector[Z], pVector[A], random(15), random(15), 99999);
	if(useOnce) pData[playerid][destroyMyVehicle] = true;
	
	PutPlayerInVehicle(playerid, pData[playerid][lastVehicle], 0);
	TogglePlayerControllable(playerid, true);
}

PlayRandomMusic(playerid, time = 0)
{
	new musicSounds[] = {
	 1062,
	 1068,
	 1076,
	 1097,
	 1183,
	 1185,
	 1187
	};
	
	new
	 randomIdx = random(sizeof musicSounds);
	
	PlaySound(playerid, musicSounds[randomIdx]);
	
	if(time)
	{
		KillTimer(pTemp[playerid][timerStopPlayerMusic]);
		pTemp[playerid][timerStopPlayerMusic] = SetTimerEx("StopPlayerMusic", time, false, "i", playerid);
	}
}

DisablePlayerSounds(playerid)
{
	PlaySound(playerid, 1188);
}

public StopPlayerMusic(playerid)
{
	DisablePlayerSounds(playerid);
}

OutputVipChat(playerid, szMsg[])
{
	new
	 buffer[200];
	
	format(buffer, sizeof buffer, " {b} (VIP) %s:{/b} %s", GetPlayerProperName(playerid), szMsg);
	
	foreach(i)
	{
		if(pData[i][vipEnabled])
		{
			Msg(i, COLOR_VIP, buffer, true, false);
			PlaySound(i, 1027);
		}
	}
	
	format(buffer, sizeof buffer, "[VIP] %s: %s", GetPlayerNick(playerid), szMsg);
	OutputLog(LOG_OCHAT, buffer);
}

OutputModeratorChat(playerid, szMsg[])
{
	new
	 buffer[200];
	
	if (pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN)
		format(buffer, sizeof buffer, " {b}(GM) (Zdalny admin):{/b} %s", szMsg);
	else
		format(buffer, sizeof buffer, " {b}(GM) %s:{/b} %s", GetPlayerProperName(playerid), szMsg);
	
	foreach(i)
	{
		if(IsGM(i))
		{
			Msg(i, COLOR_GM, buffer, true, false);
			PlaySound(i, 1027);
		}
	}
	
	format(buffer, sizeof buffer, "[GM] %s: %s", GetPlayerNick(playerid), szMsg);
	OutputLog(LOG_OCHAT, buffer);
}

OutputAdminChat(playerid, szMsg[])
{
	new
	 buffer[200];
	
	if (pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN)
		format(buffer, sizeof buffer, " {b}(Zdalny admin):{/b} %s", szMsg);
	else
		format(buffer, sizeof buffer, " {b}(Admin) %s:{/b} %s", GetPlayerProperName(playerid), szMsg);
	
	foreach(i)
	{
		if(IsAdmin(i))
		{
			Msg(i, COLOR_ADMIN, buffer, true, false);
			PlaySound(i, 1027);
		}
	}
	
	format(buffer, sizeof buffer, "[ADMIN] %s: %s", GetPlayerNick(playerid), szMsg);
	OutputLog(LOG_OCHAT, buffer);
}

OutputAdmin3Chat(playerid, szMsg[])
{
	new
	 buffer[200];
	
	format(buffer, sizeof buffer, " {b}(Master admin) %s:{/b} %s", GetPlayerProperName(playerid), szMsg);
	
	foreach(i)
	{
		if(IsAdmin(i, LEVEL_ADMIN3))
		{
			Msg(i, COLOR_ADMIN3, buffer, true, false);
			PlaySound(i, 1145);
		}
	}
	
	format(buffer, sizeof buffer, "[MASTER ADMIN] %s: %s", GetPlayerNick(playerid), szMsg);
	OutputLog(LOG_OCHAT, buffer);
}

public ResetPlayerName(playerid)
{
	SetPlayerName(playerid, pTemp[playerid][tmpPlayerName]);
}

/*RandomizeWeather()
{
	gmTemp[currentWeather]=DATA_weathers[random(sizeof DATA_weathers)];
	foreach(i)
		if (GetPlayerInterior(i)==0)
			SetPlayerWeather(i,gmTemp[currentWeather]);
	gmTemp[lastWeatherChangeTime]=GetTickCount();
}*/

public SearchForNewAdmin()
{
	foreach(playerid)
	{
		if(!pTemp[playerid][isRcon] && IsPlayerAdmin(playerid) && pData[playerid][allowedLevel]>=LEVEL_ADMIN3)
		{
			pTemp[playerid][isRcon] = true;
//			pData[playerid][adminLevel] = pData[playerid][allowedLevel];
//			if(pData[playerid][adminLevel] < LEVEL_ADMIN3 && pData[playerid][adminLevel] != LEVEL_ADMINHIDDEN)
//			{
//				pData[playerid][adminLevel] = LEVEL_ADMIN3;
			OnPlayerRCONLogin(playerid);
//			}
				
			break;
		} else if (!pTemp[playerid][isRcon] && IsPlayerAdmin(playerid) && pData[playerid][allowedLevel]<LEVEL_ADMIN3) {
			new buffer[255];
			format(buffer,sizeof buffer,"Nieautoryzowane logowanie na admina RCON %d (dobre haslo!) przez %s (%d)! Poziom %d/%d.", pTemp[playerid][isRcon], GetPlayerNick(playerid), playerid, pData[playerid][adminLevel], pData[playerid][allowedLevel]);
			KickPlayer(playerid,false);
			foreach(i)
				if(IsAdmin(i, LEVEL_ADMIN1))
					Msg(i, COLOR_ERROR, buffer);
			OutputLog(LOG_SYSTEM, buffer);
		}
	}
}

public AnnounceFade()
{
	if(gmTemp[ann_effect_mode] == ANN_MODE_FADE_IN)			gmTemp[ann_effect_alpha] += 10;
	else if(gmTemp[ann_effect_mode] == ANN_MODE_FADE_OUT)	gmTemp[ann_effect_alpha] -= 10;

	TextDrawColor(gTextDraw[TD_ANNOUNCE], SetAlpha(0xFFFFD0FF, gmTemp[ann_effect_alpha]));
	TextDrawBackgroundColor(gTextDraw[TD_ANNOUNCE], SetAlpha(0x000000FF, gmTemp[ann_effect_alpha] / 2));
	TextDrawShowForAll(gTextDraw[TD_ANNOUNCE]);
	
	if(gmTemp[ann_effect_alpha] >= 250)
	{
		KillTimer(gmTemp[timerAnnounce]);
		gmTemp[timerAnnounceEnd] = SetTimer("AnnounceFadeOutStart", 6000, false);
		gmTemp[ann_effect_mode] = ANN_MODE_ON;
	}
	
	if(gmTemp[ann_effect_alpha] <= 0)
	{
		KillTimer(gmTemp[timerAnnounce]);
		TextDrawHideForAll(gTextDraw[TD_ANNOUNCE]);
		gmTemp[ann_effect_mode] = ANN_MODE_OFF;
		
		if(gmTemp[ann_switchToNew])
		{
			gmTemp[ann_effect_alpha] = 1;
			gmTemp[ann_effect_mode] = ANN_MODE_FADE_IN;
		
			gmTemp[timerAnnounce] = SetTimer("AnnounceFade", 70, true);
			TextDrawSetString(gTextDraw[TD_ANNOUNCE], gmTemp[ann_switchToNewText]);
			
			gmTemp[ann_switchToNew] = false;
		}
	}
}

public AnnounceFadeOutStart()
{
	gmTemp[ann_effect_mode] = ANN_MODE_FADE_OUT;
	
	gmTemp[timerAnnounce] = SetTimer("AnnounceFade", 70, true);
}

RefreshPlayerVehicleInfo(playerid)
{
	new
	 vehicleid = GetPlayerVehicleID(playerid),
//	 vComponent,
	 Float:vHealth,
//	 bool:nitroInstalled,
	 buffer[256];
	
//	vComponent = GetVehicleComponentInSlot(vehicleid, CARMODTYPE_NITRO);
	GetVehicleHealth(vehicleid, vHealth);
	
	vHealth = (vHealth - 250.0) / 7.5;
	if (vHealth>100) vHealth=100;
	
//	if(vComponent == 1008 || vComponent == 1009 || vComponent == 1010)
//		nitroInstalled = true;

	new hpstr[16];
	if (vHealth>=95) 
		hpstr="~g~]]]]]";
	else if (vHealth>=85)
		hpstr="~g~]]]]~l~]";
	else if (vHealth>=75)
		hpstr="~y~~h~]]]~l~]]";
	else if (vHealth>=50)
		hpstr="~y~]]]~l~]]";
	else if (vHealth>=30)
		hpstr="~y~]]~l~]]]";
	else if (vHealth>=10)
		hpstr="~r~]~l~]]]]";
	else 
		hpstr="~l~]]]]]";

	format(buffer, sizeof buffer, "~r~~h~%s~n~~b~~h~~h~~h~%i ~w~km/h    %s",
		vehicleName[GetVehicleModel(vehicleid) - 400],
		GetVehicleSpeed(vehicleid),hpstr);
	
/*	format(buffer, sizeof buffer, "~r~~h~%s~n~~b~~h~~h~~h~%i ~w~km/h    %i%% %s",// ~n~~b~~h~~h~~h~%s: ~w~%s ~n~~b~~h~~h~~h~Nitro: ~w~%s",
		vehicleName[GetVehicleModel(vehicleid) - 400],
//		TXT(playerid, 202),
		GetVehicleSpeed(vehicleid),
//		TXT(playerid, 203),
		floatround(vHealth),
//		TXT(playerid, 204),
		(vehicleDoorState[vehicleid] == DOOR_CLOSED) ? TXT(playerid, 446) : TXT(playerid, 445)
//		nitroInstalled ? TXT(playerid, 200) : TXT(playerid, 201)
	);*/

	TextDrawSetString(pTextDraw[PTD_VEHICLEINFO][playerid], buffer);
}

FilterText(playerid, text[], bool:antiSpam = false)
{
	//  --- Space filter ---
	
	new
	 tmpPos = 0,
	 bigCaseLetters;
	
	while(tmpPos < strlen(text))
	{
		if(text[tmpPos] >= 'A' && text[tmpPos] <= 'Z') bigCaseLetters++;
		else if(text[tmpPos] == ' ')
		{
			if(text[tmpPos + 1] == ' ')
			{
				strdel(text, tmpPos, tmpPos + 1);
				
				continue;
			}
		}
		tmpPos++;
	}
	
	//  --- Caps lock filter ---
	
	new
	 length = strlen(text);
		
	if(length>5 && ((float(length) / float(bigCaseLetters)) <= 3.3 && bigCaseLetters > 0))
		for(new i = 0; i < length; i++)
			text[i] = DownCase(text[i]);
	
	//  --- Anti spam ---
	if(antiSpam && pData[playerid][adminLevel]<LEVEL_ADMIN3)
	{
		if(GetTickCount() - pTemp[playerid][lastMsgTick] < (pData[playerid][respect]<1000?(pData[playerid][respect]<333?3000:2000):1000))
		{
			pTemp[playerid][spamCount]++;
			
			if(pTemp[playerid][spamCount] >= (pData[playerid][respect]<1000?(pData[playerid][respect]<333?5:9):12) )
			{
				format(gstr, sizeof gstr, "Gracz %s dostal%s kicka z powodu: spamowanie.", GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""));
				if (gmTemp[pPlayerCount]<20 || gmTemp[showJoins]==2)
					SendClientMessageToAll(0xA0A0A0FF, gstr);
				else
					MSGToAdmins(COLOR_INFO2, gstr, false, LEVEL_ADMIN2);

				Msg(playerid, COLOR_ERROR, TXT(playerid, 141));
				KickPlayer(playerid, true);
				return 0;
			}
			
			if(pTemp[playerid][spamCount] >= 2)
			{
				Msg(playerid, COLOR_ERROR, TXT(playerid, 140));
				pTemp[playerid][lastMsgTick] = GetTickCount();
				
				return 0;
			}
		}
		else
		{
			pTemp[playerid][spamCount] = 0;
		}
		
		pTemp[playerid][lastMsgTick] = GetTickCount();
	}
	
	
	return 1;
}





Strzelnica_Start()
{
	new
	 aPlayers[MAX_QUEUE_STRZELNICA],
	 aMaxPlayers = 0,
	 aShooter,
	 Float:aPosition[] = {283.3859, -135.1865},
	 Float:t_radius,
	 Float:t_angle,
	 t_index = 1,
	 aWeapons[] = {24, 25, 29, 30, 31, 33, 34};
	
	foreach(playerid)
	{
		if(pData[playerid][aStrzelnica])
		{
			if(AttractionAllowed(playerid) && pTemp[playerid][staleTime]<5)
			{
				aPlayers[aMaxPlayers++] = playerid;
				if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
			}
			else
			{
				pData[playerid][aStrzelnica] = false;
				Msg(playerid, COLOR_ERROR, TXT(playerid, 285));
			}
		}
	}
	
	if(aMaxPlayers < 3)
	{
		foreach(playerid)
		{
			Msg(playerid, COLOR_INFO3, TXT(playerid, 276));
		}
		
		aData[A_STRZELNICA][aState] = A_STATE_OFF;
		
		return;
	}
	
	aShooter = aPlayers[random(aMaxPlayers)];
	gmTemp[aStrzelnicaShooter] = aShooter;

	Teleport(T_PLAYER, aShooter, 300.3222, -134.0520, 1004.0625, 90.0, 7, VW_STRZELNICA);
	ResetPlayerWeapons(aShooter);
	GivePlayerWeapon(aShooter, aWeapons[random(sizeof aWeapons)], 9800);
	SetPlayerHealth(aShooter, 9999999.0);
	
//	t_radius = 2.5 + ((aMaxPlayers - 1) / 2);
	t_radius = 5;
	t_angle = 360.0 / (aMaxPlayers - 1);
	
	for(new i = 0; i < aMaxPlayers; i++)
	{
		pData[aPlayers[i]][pAttraction] = A_STRZELNICA;
		pData[aPlayers[i]][aStrzelnica] = false;
		TogglePlayerControllable(aPlayers[i], false);
		SetPlayerArmour(aPlayers[i], 0.0);
		
		if(aPlayers[i] == aShooter) continue;
		
		aPosition[0] += (t_radius * floatsin(t_angle * t_index, degrees));
		aPosition[1] += (t_radius * floatcos(t_angle * t_index, degrees));
		t_index++;
		
		Teleport(T_PLAYER, aPlayers[i], aPosition[0], aPosition[1], 1004.0625, 270.0, 7, VW_STRZELNICA);
		ResetPlayerWeapons(aPlayers[i]);
		SetPlayerHealth(aPlayers[i], 100.0);
	}
	
	gmTemp[aStrzelnicaPlayers] = aPlayers;
	gmTemp[aStrzelnicaMaxPlayers] = aMaxPlayers;
	gmTemp[aStrzelnicaCount] = 5;
	gmTemp[timerStrzelnicaCountdown] = SetTimer("Strzelnica_Countdown", 700, true);
	gmTemp[aStrzelnicaStartTick] = GetTickCount();
}

public Strzelnica_Countdown()
{
	new
	 buffer[64];
	
	if(gmTemp[aStrzelnicaCount] <= 0)
	{
		format(buffer, sizeof buffer, "~g~GO!");
		KillTimer(gmTemp[timerStrzelnicaCountdown]);
	}
	else
	{
		format(buffer, sizeof buffer, "~r~%i", gmTemp[aStrzelnicaCount]);
	}
	
	for(new playerid = 0; playerid < gmTemp[aStrzelnicaMaxPlayers]; playerid++)
	{
		GameTextForPlayer(gmTemp[aStrzelnicaPlayers][playerid], buffer, 1000, 6);
		
		if(gmTemp[aStrzelnicaCount] == 0)
		{
			PlaySound(gmTemp[aStrzelnicaPlayers][playerid], 1057);
			TogglePlayerControllable(gmTemp[aStrzelnicaPlayers][playerid], true);
		}
		else
		{
			PlaySound(gmTemp[aStrzelnicaPlayers][playerid], 1056);
		}
	}
	gmTemp[aStrzelnicaCount]--;
}


ShowPlayerHudElement(playerid, element, bool:show)
{
//	new
//	 buffer[8];

//	format(buffer, sizeof buffer, "hud%i", element);
//	SetPlayerINIFileValue(playerid, buffer, BoolToInt(show));

	switch(element)
	{
		case HUD_STATUSBAR:
		{
			ShowElement(playerid, TDE_STATS, show);
		}
		
		case HUD_ATTRACTIONBOX:
		{
			ShowElement(playerid, TDE_ATTRACTIONBOX, show);
		}
		
//		case HUD_VEHICLEBOX:
//		{
//			if(IsPlayerInAnyVehicle(playerid)) ShowElement(playerid, TDE_VEHICLEBOX, show);
//		}
		
		case HUD_ATTRACTIONNEWS:
		{
			// ShowElement(playerid, TDE_INFOBOX, show); // TODO
		}
		
		case HUD_SERVERLOGO:
		{
			ShowElement(playerid, TDE_FULLSERVERLOGO, show);
		}
//		case HUD_VOTING:	// zalaczane i wylaczane automatycznie		
//			ShowElement(playerid, TDE_VOTING, show);
		
		case HUD_DATE:
		{
			ShowElement(playerid, TDE_DATETIME, show);
		}
		
		case HUD_STARS:
		{
			if(show)
			{
				RefreshPlayerStars(playerid);
//				pTemp[playerid][lastRespect] = pData[playerid][respect];
			}
			else
			{
				for(new i = TD_STARS_START; i <= TD_STARS_END; i++)
				{
					TextDrawHideForPlayer(playerid, gTextDraw[i]);
				}
			}
		}
		
		case HUD_FPS:
		{
			if(show)
				TextDrawShowForPlayer(playerid,pTextDraw[PTD_FPS][playerid]);
			else
				TextDrawHideForPlayer(playerid,pTextDraw[PTD_FPS][playerid]);
		}
	}
}

SaveAttractionData(attractionID)
{
	new
	 buffer[64];

	format(buffer, sizeof buffer, "%s %i %i", aData[attractionID][aName], aData[attractionID][aStartPlayers], aData[attractionID][aStartingTime]);
			
	switch(attractionID)
	{
		case A_CHOWANY: 	SetConfigValueString("a_chowany", buffer);
		case A_SPS: 		SetConfigValueString("a_sps", buffer);
		case A_DERBY: 		SetConfigValueString("a_derby", buffer);
		case A_LABIRYNT: 	SetConfigValueString("a_labirynt", buffer);
		case A_RACE: 		SetConfigValueString("a_race", buffer);
		case A_DRIFT: 		SetConfigValueString("a_drift", buffer);
		case A_WG: 			SetConfigValueString("a_wg", buffer);
		case A_STRZELNICA: 	SetConfigValueString("a_strzelnica", buffer);
	}
}


public NoMoreWordsDialogDelayFix(playerid)
{
	ShowPlayerDialog(playerid, DIALOG_CONFIG_CENSORSHIP, DIALOG_STYLE_LIST, TXT(playerid, 386), GetDialogList(playerid, DIALOG_CONFIG_CENSORSHIP), TXT(playerid, 78), TXT(playerid, 77));
}

bool:IsWordCensored(szWord[])
{
	new
	 File:hFile = fopen("XyzzyDM/cenzura.ini", io_read),
	 buffer[32],
	 length;
			
	while(fread(hFile, buffer))
	{
		length = strlen(buffer);
		strdel(buffer, length - 1, length);
				
		if(strcmp(buffer, szWord, true) == 0)
		{
			return true;
		}
	}
	
	fclose(hFile);
	
	return false;
}

CensorText(text[])
{
	if(!gmData[censorship]) return;
	
	new
	 buffer[32],
	 szSource[256],
	 _start,
	 _end;
	
	copy(text, szSource);
	
	for(new i = 0; i < strlen(szSource); i++)
	{
		switch(szSource[i])
		{
			case 'ê': szSource[i] = 'e';
			case 'ó': szSource[i] = 'o';
			case '¹': szSource[i] = 'a';
			case 'œ': szSource[i] = 's';
			case '³': szSource[i] = 'l';
			case '¿', 'Ÿ': szSource[i] = 'z';
			case 'æ': szSource[i] = 'c';
			case 'ñ': szSource[i] = 'n';
			
			case 'Ê': szSource[i] = 'E';
			case 'Ó': szSource[i] = 'O';
			case '¥': szSource[i] = 'A';
			case 'Œ': szSource[i] = 'S';
			case '£': szSource[i] = 'L';
			case '¯', '': szSource[i] = 'Z';
			case 'Æ': szSource[i] = 'C';
			case 'Ñ': szSource[i] = 'N';
		}
	}

	for(new i = 0; i < gmData[maxCensoredWords]; i++)
	{
		while(FindString(szSource, gCensoredWord[i], _start, _end))
		{
			copy(gCensoredWord[i], buffer);
			
			for(new g = 1; g < strlen(buffer) - 1; g++)
			{
				buffer[g] = '*';
			}
			
			strdel(text, _start, _end);
			strins(text, buffer, _start, 256);
			
			strdel(szSource, _start, _end);
			strins(szSource, buffer, _start, 256);
		}
	}
}

FilterTextWithColors(text[])
{
	new
	 pos = -1;
	 
	while((pos = strfind(text, "[", false, pos + 1)) != -1)
	{
		new
		 c = pos + 1,
		 n = 0,
		 ch;

		while((ch = text[c]) && n != 6)
		{
			if(!('a' <= ch <= 'f' || 'A' <= ch <= 'F' || '0' <= ch <= '9')) break;
			
			++c;
			++n;
		}
		
		if(n == 6 && ch == ']')
		{
			text[pos] = '{';
			text[c] = '}';
		}
	}
	
//	print(text);
}

AddPlayerToReportList(playerid, reason[], reporting_playerid)
{
	if(pData[playerid][reported]) return;
	
	for(new i = 0; i < MAX_REPORTS; i++)
	{
		if(!gReports[i][rOccupiedIndex])
		{
			gReports[i][rPlayerId] = playerid;
			copy(GetPlayerNick(playerid), gReports[i][rPlayerName]);
			copy(reason, gReports[i][rReason]);
			gReports[i][rOccupiedIndex] = true;
			copy(GetPlayerNick(reporting_playerid), gReports[i][rReportingPlayerName]);
			
			pData[playerid][reported] = true;
			return;
//			break;
		}
	}
}

RemovePlayerFromReportList(playerid)
{
	for(new i = 0; i < MAX_REPORTS; i++)
	{
		if(gReports[i][rOccupiedIndex] && gReports[i][rPlayerId] == playerid)
		{
			gReports[i][rOccupiedIndex] = false;
			pData[gReports[i][rPlayerId]][reported] = false;
			
			break;
		}
	}
}

public HidePlayerWarning(playerid)
{
	if (IsPlayerConnected(playerid))
		TextDrawHideForPlayer(playerid, gTextDraw[TD_WARNING]);
	return 1;
	
}

UpdatePlayerStatsTD(playerid)
{
	new pPing=GetPlayerPing(playerid);
	static str[160];

	new pSessionH = GetPlayerCurrentSession(playerid) / 60;
                                            
	format(str,sizeof str,"%02dh%02dm    ~w~%dms      ", pSessionH/60, pSessionH%60, pPing);

//	format(str,sizeof str,"%65s%d",str,  pData[playerid][respect]);
//	format(str,sizeof str,"%75s%d", str, pData[playerid][skill]);

	format(str,sizeof str,"%23s%d", str, pData[playerid][respect]);
	format(str,sizeof str,"%33s%d", str, pData[playerid][skill]);

//	printf("%d:%s",playerid,str);


	TextDrawSetString(pTextDraw[PTD_STAT][playerid], str);
}

UpdatePlayerNickTD(playerid)
{
//	static str[64];

	if (IsAdmin(playerid,LEVEL_ADMIN1))
		format(gstr,sizeof gstr,"~r~%s ~w~(~y~%d~w~)",GetPlayerNick(playerid),	playerid);
	else if (IsGM(playerid))
		format(gstr,sizeof gstr,"~r~~h~%s ~w~(~y~%d~w~)",GetPlayerNick(playerid),	playerid);
	else if (pData[playerid][vipEnabled])
		format(gstr,sizeof gstr,"~y~%s ~w~(~y~%d~w~)",GetPlayerNick(playerid),	playerid);
	else
		format(gstr,sizeof gstr,"~w~%s ~w~(~y~%d~w~)",GetPlayerNick(playerid),	playerid);

	TextDrawSetString(pTextDraw[PTD_NICK][playerid], gstr);

	UpdatePlayerFactionName(playerid);
}

public PlayerSelfHeal(playerid){
	if (pData[playerid][pAttraction]!=A_NONE)
		return Msg(playerid,COLOR_ERROR,"Nie mozesz sie teraz uleczyc!");
	SetPlayerHealth(playerid,100.0);
	Msg(playerid,COLOR_INFO,"Zostales uleczony");
	return 1;
}

ShowPlayerInfo(playerid,targetid){
	new buffer[2048];
	if(!IsPlayerConnected(targetid))
		return Msg(playerid,COLOR_ERROR,"Nie znaleziono gracza!");
	format(buffer,sizeof buffer,"{d0d0d0}Nick: {ffffff}%s \n",GetPlayerNick(targetid));
	if (pData[targetid][loggedIn]) {
		switch(pData[targetid][adminLevel]) {
			case LEVEL_ADMIN3:
				strcat(buffer," {ff0000} Administrator RCON{ffffff} \n");
			case LEVEL_ADMIN2:
				 strcat(buffer," {ff0000} Administrator{ffffff} \n");
			case LEVEL_ADMIN1:
				 strcat(buffer," {ff0000} Administrator rekrut{ffffff} \n");
			case LEVEL_GM:
				 strcat(buffer," {FF6600} Moderator{ffffff} \n");
		}
		if (pData[targetid][vipEnabled])
			strcat(buffer, "{FFD700} Gracz V.I.P{ffffff} \n");

		if (pData[targetid][accountID]>0)  {
			format(buffer, sizeof buffer,"%s\r\n{d0d0d0}Respekt: {ffffff}%d\r\n{d0d0d0}Skill: {ffffff}%d\r\n", buffer, pData[targetid][respect], pData[targetid][skill]);
		} else
			strcat(buffer, "Gracz niezarejestrowany\r\n");
	}
	if (pData[targetid][gang]!=NO_GANG) {
		strcat(buffer," \n{ffffff}");
		switch (pData[targetid][gangRank]) {
			case GANG_RANK_MEMBER:
				strcat(buffer," Czlonek ");
			case GANG_RANK_OWNER:
				strcat(buffer," Zalozyciel ");
			case GANG_RANK_LEADER:
				strcat(buffer," Lider ");
			case GANG_RANK_SUSPENDED:
				strcat(buffer," Zawieszony czlonek ");
		}
		format(buffer, sizeof buffer,"%s {%06x} %s \n \n", buffer, gData[pData[targetid][gang]][gColor], gData[pData[targetid][gang]][name]);
		
	}
	format(buffer,sizeof buffer,"%s{d0d0d0}Ping sredni/biezacy:	{ffffff}%dms/%dms\r\n",buffer,pData[targetid][averagePing],GetPlayerPing(targetid));
	format(buffer,sizeof buffer,"%s{d0d0d0}Plugin audio: %s \n",buffer,	((Audio_IsClientConnected(targetid))?("{30ff30} TAK"):("{ff3030} NIE")));

	ShowPlayerDialog(playerid,DIALOG_PLAYER_INFO,DIALOG_STYLE_MSGBOX,"Informacje o graczu",buffer,"OK","");

	return 1;
}

public ShowAnnouncement(params[]){
	if(isnull(params) || !CheckTildes(params)) return 0;

	KillTimer(gmTemp[timerAnnounce]);
	KillTimer(gmTemp[timerAnnounceEnd]);
	
	if(gmTemp[ann_effect_mode] == ANN_MODE_ON || gmTemp[ann_effect_mode] == ANN_MODE_FADE_IN || gmTemp[ann_effect_mode] == ANN_MODE_FADE_OUT)
	{
		AnnounceFadeOutStart();
		gmTemp[ann_switchToNew] = true;
		copy(params, gmTemp[ann_switchToNewText]);
	}
	else
	{
		gmTemp[ann_effect_alpha] = 1;
		gmTemp[ann_effect_mode] = ANN_MODE_FADE_IN;
	
		gmTemp[timerAnnounce] = SetTimer("AnnounceFade", 70, true);
		TextDrawSetString(gTextDraw[TD_ANNOUNCE], params);
		
	}
	return 1;
}

public ShowAnnouncement2(params[]){
	if(isnull(params) || !CheckTildes(params)) return 0;
	if (gmTemp[ann2_step]>0) {	// juz jest jakis TD wyswietlany
		KillTimer(gmTemp[timerAnnounce2]);
	}

	gmTemp[ann2_step]=1;
	TextDrawSetString(gTextDraw[TD_ANN2], params);
	gmTemp[timerAnnounce2]=SetTimer("FadeAnnouncement2",150,false);	

	return 1;
}

public FadeAnnouncement2(){
	// 1,2,3, 4, 5,6,7
	if (gmTemp[ann2_step]==8) {
		gmTemp[ann2_step]=0; TextDrawHideForAll(gTextDraw[TD_ANN2]); return;
	} 
	if (gmTemp[ann2_step]<=4)
		TextDrawColor(gTextDraw[TD_ANN2],0xffffff00+(63*gmTemp[ann2_step]));
	else 
		TextDrawColor(gTextDraw[TD_ANN2],0xffffff00+(63*(8-gmTemp[ann2_step])));
	TextDrawShowForAll(gTextDraw[TD_ANN2]);

	gmTemp[ann2_step]++;
	gmTemp[timerAnnounce2]=SetTimer("FadeAnnouncement2",150+((gmTemp[ann2_step]==5)?10000:0),false);	
	return;
}

KickPlayerWithReason(targetplayerid,playerid,reason[]){
	new buffer[127];
	new buffer2[127];
//	format(buffer, sizeof buffer, TXT(playerid, 152), GetPlayerNick(targetplayerid), reason);
//	Msg(playerid, COLOR_INFO, buffer); // Gracz "xxx" zosta³ wyrzucony z serwera, powód: xxx

//	format(buffer, sizeof buffer, TXT(targetplayerid, 153), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
//	Msg(targetplayerid, COLOR_INFO2, buffer); // Zosta³eœ wyrzucony z serwera przez admina "xxx".

//	format(buffer, sizeof buffer, TXT(targetplayerid, 50), reason);
//	Msg(targetplayerid, COLOR_INFO2, buffer); // Powód: xxx
			
	new bool:kobietat=Kobieta(targetplayerid);
	new bool:in1line;
	if (pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN) {
		format(buffer, sizeof buffer, "{b}%s{/b} zostal%s wyrzucon%s z serwera: {b}%s{/b}", 
				GetPlayerNick(targetplayerid), kobietat?("a"):(""), kobietat?("a"):("y"), reason);
	} else {
		format(buffer, sizeof buffer, "{b}%s{/b} zostal%s wyrzucon%s z serwera przez admin%s {b}%s{/b}",
				GetPlayerNick(targetplayerid), kobietat?("a"):(""), kobietat?("a"):("y"), Kobieta(playerid)?("ke"):("a"), GetPlayerProperName(playerid));

	}


	if (strlen(buffer)+24+strlen(reason)<127) {
		strcat(buffer,": {b}");
		strcat(buffer,reason);
		in1line=true;
	} else {
		format(buffer2, sizeof buffer2, "Powod: {b}%s", reason);
		in1line=false;
	}
			
	foreach(i){
		Msg(i, COLOR_INFO2, buffer);
		if (!in1line)
			Msg(i, COLOR_INFO2, buffer2);
	}

	if (Audio_IsClientConnected(targetplayerid))
		Audio_Play(targetplayerid,AUDIOID_KICK, false, false, true);
			
	KickPlayer(targetplayerid);
	return 1;
}

LogConnection(playerid){
	new buffer[255],szIP[16], escaped_ip[20];
	GetPlayerIp(playerid, szIP, sizeof szIP);
	mysql_real_escape_string(szIP,escaped_ip);

	format(buffer,sizeof buffer,"UPDATE fs_players SET datetime_last=NOW(),login_count=login_count+1,ip_last='%s' WHERE id=%d",escaped_ip,pData[playerid][accountID]);
	mysql_query(buffer,SQL_RI_BACKGROUND);
	return 1;
}

FetchPlayerAccountData(playerid){

	new buffer[512];
	format(buffer,sizeof buffer,"SELECT wallet_money,IFNULL(DATEDIFF(vip,NOW()),'-1'),IFNULL(vip,'0000-00-00'),level,respect,kill_count,teamkill_count,death_count,suicide_count,skill,hitman_prize FROM fs_players WHERE id=%d LIMIT 1", pData[playerid][accountID]);
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows()>0) {
		mysql_fetch_row(buffer,"|");
		sscanf(buffer,"p<|>dds[11]dddddddd",pData[playerid][pmoney],pData[playerid][vipDaysLeft],pData[playerid][vipToDate],
					pData[playerid][allowedLevel],pData[playerid][respect],
					pData[playerid][kills],pData[playerid][teamkills],pData[playerid][deaths],pData[playerid][suicides],
					pData[playerid][skill],pData[playerid][hitman]);
		SetPlayerMoney(playerid,pData[playerid][pmoney]);
		pData[playerid][vipEnabled]=(pData[playerid][vipDaysLeft]>=0) ? (true): (false);
//		printf("Otrzymano dane vipdaysleft %d resp %d", pData[playerid][vipDaysLeft], pData[playerid][respect]);
	}
	mysql_free_result();
	return;
}

PlayerQuickbuyWeapon(playerid,listitem){
	new buf[127];
	new nlistitem=0;

	for (new i=0;(i<sizeof quickbuyWeapons && nlistitem<listitem);i++)
		if (quickbuyWeapons[i][ewd_quickBuy]) nlistitem++;
	listitem=nlistitem+1;

	if (!(quickbuyWeapons[listitem][ewd_id]>0))
		return Msg(playerid, COLOR_ERROR, "Niestety, ta bron jest obecnie niedostepna");
	if (pData[playerid][pAttraction]!=A_NONE)
		return Msg(playerid, COLOR_ERROR, "Nie mozesz obecnie kupowac broni!");
	if (GetPlayerMoney(playerid)<quickbuyWeapons[listitem][ewd_baseCost])
		return Msg(playerid, COLOR_ERROR, "Nie masz wystarczajacej ilosc gotowki!");
	SetPlayerMoney(playerid, GetPlayerMoney(playerid)-quickbuyWeapons[listitem][ewd_baseCost]);
	format(buf,sizeof buf,"Kupiles/-as {b}%s{/b} za {b}%d{/b}$.", quickbuyWeapons[listitem][ewd_PLname], quickbuyWeapons[listitem][ewd_baseCost]);
	Msg(playerid,COLOR_INFO, buf);

	if (quickbuyWeapons[listitem][ewd_id]<=15 || quickbuyWeapons[listitem][ewd_id]==49)	{ // bronie biale/spadochron
		GivePlayerWeapon(playerid,quickbuyWeapons[listitem][ewd_id],1);
		SetPlayerAmmo(playerid,quickbuyWeapons[listitem][ewd_id],1);
	} 
		else
	GivePlayerWeapon(playerid,quickbuyWeapons[listitem][ewd_id],quickbuyWeapons[listitem][ewd_baseAmmo]);

	new vid=GetPlayerVehicleID(playerid);
	if (vid!=0 && vid!=INVALID_VEHICLE_ID && tVehicles[vid][vo_drift])	// w pojezdzie drifterskim nie wolno miec broni
		SetPlayerArmedWeapon(playerid,0);
	else if (GetPlayerState(playerid)==PLAYER_STATE_PASSENGER)		// nie zmieniamy pasazerowi broni
		SetPlayerArmedWeapon(playerid,0);

	return 1;
}

public UsunPickup(pickupid){
	if (IsValidDynamicPickup(pickupid))
		DestroyDynamicPickup(pickupid);
	return;
}

RefreshConfiguration(){
	if (mysql_query("SELECT option_name,value FROM fs_config"))
		return mysql_ping();
	new buffer[1023],option_name[33],value[512];
	mysql_store_result();
	while(mysql_fetch_row(buffer,"|")) {
		if (sscanf(buffer,"p<|>s[32]s[511]",option_name,value)) continue;
		if (strcmp(option_name,"admin_password")==0) {
//			printf("Hasz adimna: %s %s %s", option_name, gmTemp[AdminPasswordHash], value); 
			copy(value, gmTemp[AdminPasswordHash]); 
//			printf("Hasz adimna: %s %s %s", option_name, gmTemp[AdminPasswordHash], value); 
		} else if (strcmp(option_name,"welcome_text")==0) {
			if(strlen(value)<1020)
				TextDrawSetString(gTextDraw[TD_WELCOMETEXT], value);
		}


	}
	mysql_free_result();
	return 1;
}

ShowWeaponSelectMenu(playerid, title[], callback[32], restricted_slot=-1, restricted_wepid=-1){
	new listitems[1024], buffer[128];
	for(new i=0; i<sizeof quickbuyWeapons; i++) {
		if (restricted_slot>=0 && quickbuyWeapons[i][ewd_slot]==restricted_slot) continue;
		if (restricted_wepid>=0 && quickbuyWeapons[i][ewd_id]==restricted_wepid) continue;
		format(buffer, sizeof buffer, "{000000}%02d {ffffff}%s", quickbuyWeapons[i][ewd_id], quickbuyWeapons[i][ewd_PLname]);
		if (i>0) strcat(listitems, "\n");
		strcat(listitems, buffer);
	}
	copy(callback, pTemp[playerid][dialogCB]);
	ShowPlayerDialog(playerid,DIALOG_WEAPON_SELECT,DIALOG_STYLE_LIST,title,listitems,"Wybierz", "Anuluj");
	return 1;
}

OnPlayerChangeWeapon(playerid,oldwepid, wepid){
	#pragma unused oldwepid
	if (!IsPlayerSpawned(playerid)) return;
	if (pTemp[playerid][disableWeaponCheck]) return;
	if (IsPlayerNPC(playerid)) return;
	if (pData[playerid][adminLevel]==LEVEL_ADMIN3) return;

	new buf[128];
	if(wepid==44 || wepid==45) {	// gogle
		format(buf, sizeof buf, "WYKRYTY WH: {b}%s{/b}(%d) ma gogle/noktowizor.", GetPlayerNick(playerid), playerid);
		MSGToAdmins(COLOR_INFO2, buf, true);
		SetPlayerArmedWeapon(playerid,0);
		RemovePlayerWeapon(playerid,wepid);
	} else if (wepid==36) {	// hs rocket launcher
		format(buf, sizeof buf, "WYKRYTY WH: {b}%s{/b}(%d) ma RPG z naprowadzaniem (niedostepne w grze!).", GetPlayerNick(playerid), playerid);
		MSGToAdmins(COLOR_INFO2, buf, true);
		SetPlayerArmedWeapon(playerid,0);
		RemovePlayerWeapon(playerid,wepid);
	} else if (!pTemp[playerid][protkill] && !gmTemp[protAll] && wepid==16 && pData[playerid][respect]<75000 && pData[playerid][adminLevel]<LEVEL_ADMIN1 && pTemp[playerid][onArena]!=ARENA_AZTEC) {	// granaty
		SetPlayerArmedWeapon(playerid,0);
		RemovePlayerWeapon(playerid,wepid);
	} else if (!pTemp[playerid][protkill] && !gmTemp[protAll] && wepid==18 && pData[playerid][respect]<40000 && pData[playerid][adminLevel]<LEVEL_ADMIN1	) {	// molotov
		SetPlayerArmedWeapon(playerid,0);
		RemovePlayerWeapon(playerid,wepid);
	} else if (!pTemp[playerid][protkill] && !gmTemp[protAll] && wepid==37 && GetPlayerInterior(playerid)!=11)	{	// flame thrower	 oprocz int kanalow	TODO dokladniejsze sprawdzenie
		SetPlayerArmedWeapon(playerid,0);
		RemovePlayerWeapon(playerid,wepid);
	} else if (!pTemp[playerid][protkill] && !gmTemp[protAll] && wepid==35 && !(IsPlayerInFullDMArea(playerid))) {	// RPG
		SetPlayerArmedWeapon(playerid,0);
		RemovePlayerWeapon(playerid,wepid);
	} else if(!pTemp[playerid][protkill] && !gmTemp[protAll] && wepid==38 && pData[playerid][adminLevel]<LEVEL_ADMIN1 && pTemp[playerid][onArena]!=ARENA_MINIGUN && pTemp[playerid][aChowanySide] != A_CHOWANY_SEARCHING) {		// minigun
		RemovePlayerWeapon(playerid,38);
	}/* else if(GetPlayerVirtualWorld(playerid)==VW_JAIL && wepid>0 && pData[playerid][adminLevel]<=LEVEL_ADMIN1) {
		SetPlayerArmedWeapon(playerid,0);
		ResetPlayerWeapons(playerid);
		format(buf, sizeof buf, "Prawdopodobny WH: {b}%s{/b}(%d) ma bron w wiezieniu %s (%d).", GetPlayerNick(playerid), playerid, weaponName[wepid], wepid);
		MSGToAdmins(COLOR_INFO2, buf, true);
	}*/
	
}

MSGToAdmins(msgcolor, msg[], bool:msgsound=false, minLevel=LEVEL_ADMIN2, maxLevel=LEVEL_ADMINHIDDEN) {
//	printf("MSGToAdmins(%d): %s", minLevel, msg);

	if (gmTemp[pAdminCount]==0) return;

	foreach(i)
		if (pData[i][adminLevel]>=minLevel && pData[i][adminLevel]<=maxLevel)
			Msg(i,msgcolor,msg, msgsound);
	return;
}

Delay:explode_After[150, ](playerid,userid,step){
	new Float:AR, Float:HP;
	GetPlayerHealth(userid,HP);
	GetPlayerArmour(userid,AR);
	format(gstr, sizeof gstr, "%03.1f HP / %03.1f AR", HP, AR);
	GameTextForPlayer(playerid, gstr, 500, 5);
	if (step>0)
		explode_After(playerid,userid,step--);
}

IncreasePlayerHitman(playerid,amount){
//	printf("iph %d %d (%d)", playerid, amount, pData[playerid][hitman]);
	if (amount<0) return;
	pData[playerid][hitman]+=amount;
//	format(gstr, sizeof gstr, "%i{006600}$", pData[playerid][hitman]);
//	UpdateDynamic3DTextLabelText(pTemp[playerid][p3d_status], COLOR_3DTEXT_HITMAN, gstr);
	UpdatePlayerFactionName(playerid);
	return;
}

UpdatePlayerFactionName(playerid){
	if (!IsPlayerConnected(playerid)) return;
    // ranga
	if (pData[playerid][adminLevel]>=LEVEL_ADMIN1 && pData[playerid][adminLevel]<=LEVEL_ADMIN3)
		copy("{ff0000}Admin", gstr);
	else if (pData[playerid][adminLevel]==LEVEL_GM)
		copy("{FD5E00}GM", gstr);
	else if (pData[playerid][vipEnabled])
		copy("{FFFF00}VIP", gstr);
	else
		copy("", gstr);


	// gang
	if (pData[playerid][gang]!=NO_GANG) {
		if (strlen(gstr)>0) strcat(gstr," {ffffff}| ");
		format(gstr, sizeof gstr, "%s{%06x}%s", gstr, gData[pData[playerid][gang]][gColor], gData[pData[playerid][gang]][name]);
	}
	

	if (pData[playerid][hitman]>0 || pTemp[playerid][faction]!=FACTION_NONE) {
		if (strlen(gstr)>0) strcat(gstr,"\n");
		if (pTemp[playerid][faction]!=FACTION_NONE)
			format(gstr, sizeof gstr, "%s{%06x}%s%s", gstr, Factions[pTemp[playerid][faction]][ef_color], Factions[pTemp[playerid][faction]][ef_name], pData[playerid][hitman]>0?(" {ffffff}| "):(""));
		if (pData[playerid][hitman]>0)
			format(gstr, sizeof gstr, "%s{b02020}%d$", gstr, pData[playerid][hitman]);
	}

	UpdateDynamic3DTextLabelText(pTemp[playerid][FactionName], 0xffffffff, gstr);
	
}