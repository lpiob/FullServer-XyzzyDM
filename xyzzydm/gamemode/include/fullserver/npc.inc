

stock SetupNPC(e_gmNpcTypes:npc){
	if (IsPlayerConnected(gmNPC[npc]) && IsPlayerNPC(gmNPC[npc])) {
		new npcv=GetPlayerVehicleID(gmNPC[npc]);
		if (npcv && GetVehicleModel(npcv)==520)	
			return 1;	// wszystko ok!

		if (GetPlayerState(gmNPC[npc])!=PLAYER_STATE_DRIVER)
			SpawnPlayer(gmNPC[npc]);

		SetPlayerVirtualWorld(gmNPC[npc],VW_BOTS);
		npcv=CreateVehicle(520,2066.52,1062.05,11.0,0.0,0,126,600);
		tVehicles[npcv][vo_destroyIfEmpty]=true;

		SetVehicleVirtualWorld(npcv,VW_BOTS);
		PutPlayerInVehicle(gmNPC[npc],npcv,0);
		LinkVehicleToInterior(npcv,1);
		SetVehicleHealth(npcv, 1000000);
		SetVehicleParamsEx(npcv, 1, 0, 0, 1, 0, 0, 0);
	}
	return 1;
}
