forward GivePlayerMoneyEx(playerid,money);
public GivePlayerMoneyEx(playerid,money)
	return AC_GivePlayerMoney(playerid,money);

forward GetPlayerMoneyEx(playerid);
public GetPlayerMoneyEx(playerid)
	return AC_GetPlayerMoney(playerid);




stock AC_GivePlayerMoney(playerid, money)
{
	if (GetPlayerMoney(playerid)!=pData[playerid][pmoney]) {
		ResetPlayerMoney(playerid);
		GivePlayerMoney(playerid,pData[playerid][pmoney]);
	}
	pData[playerid][pmoney]+=money;
	
    return GivePlayerMoney(playerid, money);
}

stock AC_ResetPlayerMoney(playerid)
{
	pData[playerid][pmoney]=0;
	return ResetPlayerMoney(playerid);
}

stock AC_GetPlayerMoney(playerid)
{
	if (GetPlayerMoney(playerid)!=pData[playerid][pmoney]) {
		if (floatabs(GetPlayerMoney(playerid)-pData[playerid][pmoney])>=99999 && !pTemp[playerid][dead]) {
			new pstate=GetPlayerState(playerid);
			if ((pstate==PLAYER_STATE_ONFOOT || pstate==PLAYER_STATE_DRIVER || pstate == PLAYER_STATE_PASSENGER)) {
				new buf[128];
				format(buf, sizeof buf, "Prawdopodobny money hack: %s (%d) ma %d$ a powinien %d$", GetPlayerNick(playerid), playerid, GetPlayerMoney(playerid), pData[playerid][pmoney]);
				MSGToAdmins(COLOR_INFO2, buf, false, LEVEL_ADMIN1);
			}
			
		}
		ResetPlayerMoney(playerid);
		GivePlayerMoney(playerid,pData[playerid][pmoney]);
	}
	return pData[playerid][pmoney];
}

stock SetPlayerMoney(playerid, money)
{
    AC_ResetPlayerMoney(playerid); 
    AC_GivePlayerMoney(playerid, money);
}



#if defined _ALS_GivePlayerMoney
    #undef GivePlayerMoney
#else
    #define _ALS_GivePlayerMoney
#endif
#define GivePlayerMoney AC_GivePlayerMoney

#if defined _ALS_ResetPlayerMoney
    #undef ResetPlayerMoney
#else
    #define _ALS_ResetPlayerMoney
#endif
#define ResetPlayerMoney AC_ResetPlayerMoney

#if defined _ALS_GetPlayerMoney
    #undef GetPlayerMoney
#else
    #define _ALS_GetPlayerMoney
#endif
#define GetPlayerMoney AC_GetPlayerMoney