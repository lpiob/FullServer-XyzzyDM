new Float:spawns_jail[][3]={
	{-1952.84, 6823.52, 13.65},
	{-1951.48, 6820.68, 13.65},
	{-1921.19, 6809.36, 13.65},
	{-1908.19, 6835.58, 13.65}
};

MoveToJail(playerid){
	new r=random(sizeof spawns_jail);
	pTemp[playerid][disableWeaponCheck]=true;
	ResetPlayerWeapons(playerid);
	pTemp[playerid][disableWeaponCheck]=false;
	Teleport(T_PLAYER, playerid,spawns_jail[r][0],spawns_jail[r][1],spawns_jail[r][2], random(360), 0, VW_JAIL,true);

	SetPlayerHealth(playerid, 100.0);
	SetPlayerArmour(playerid, 0.0);
	pTemp[playerid][onArena] = ARENA_NONE;
	pData[playerid][pAttraction] = A_NONE;
	pData[playerid][aChowany] = false;
	pData[playerid][aSPS] = false;
	pData[playerid][aDerby] = false;
	pData[playerid][aLabirynt] = false;
	pData[playerid][aRace] = false;
	pData[playerid][aDrift] = false;
	pData[playerid][aWG] = false;
	pData[playerid][aStrzelnica] = false;

}

JailPlayer(playerid, time)
{
	pTemp[playerid][onArena] = ARENA_NONE;
	pData[playerid][pAttraction] = A_NONE;
	pData[playerid][aChowany] = false;
	pData[playerid][aSPS] = false;
	pData[playerid][aDerby] = false;
	pData[playerid][aLabirynt] = false;
	pData[playerid][aRace] = false;
	pData[playerid][aDrift] = false;
	pData[playerid][aWG] = false;
	pData[playerid][aStrzelnica] = false;

	pTemp[playerid][disableWeaponCheck]=true;
	ResetPlayerWeapons(playerid);
	pTemp[playerid][disableWeaponCheck]=false;
	MoveToJail(playerid);


	SetPlayerHealth(playerid, 100.0);
	SetPlayerArmour(playerid, 0.0);
	
	pData[playerid][jail] = (GetTickCount() / 1000) + (time * 60);
}

UnjailPlayer(playerid)
{
	pData[playerid][jail] = -1;
	SpawnPlayer(playerid);
	GameTextForPlayer(playerid, " ", 1, 3);
}