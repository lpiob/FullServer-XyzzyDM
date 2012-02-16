/*
animacje od cwicen:
/animbenchpress
hantle
/animfreeweights
/animmusculcar

*/

/*CMD:fish(playerid){
	MUST_BE_SPAWNED(playerid);
    if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));
	if(IsPlayerInAnyVehicle(playerid)) return Msg(playerid, COLOR_ERROR, "Musisz wyjsc z pojazdu!");
	
	minigames_fishing_Start(playerid);
	return 1;

//	ApplyAnimation(playerid, "SHOP", "SHP_Gun_Duck", 4.1, 0, 1, 1, 1, 1, 1);	// animshop 10
//	ApplyAnimation(playerid, "SHOP", "SHP_Serve_Idle", 4.1, 0, 1, 1, 1, 1, 1);	// animshop 22	ogladanie ryby
//	ApplyAnimation(playerid, "SHOP", "Smoke_RYD", 4.1, 0, 1, 1, 1, 1, 1);		// 			25	zjadanie ryby

	
}



minigames_fishing_Start(playerid){
	SetPlayerAttachedObject( playerid, 0, 18632, 5, 0.079999, 0.009999, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000, 1.000000, 1.000000 ); // FishingRod - wedka2

	if (random(2)==1)
		ApplyAnimation(playerid, "SWORD", "sword_IDLE", 4.1, 0, 1, 1, 1, 1, 1);
	else
		ApplyAnimation(playerid, "SWORD", "sword_block", 4.1, 0, 1, 1, 1, 1, 1);

	pTemp[playerid][pAttraction]=A_MINIGAME;
}

minigames_fishing_Stop(playerid){
	RemovePlayerAttachedObject( playerid, 0 );
}*/

//#include "fullserver/minigames_tiry"

minigames_Init(){
//	mg_tiry_Init();
}