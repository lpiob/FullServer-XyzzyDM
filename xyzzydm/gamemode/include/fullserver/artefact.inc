CreateArtefact(Float:vX=FLOAT_NAN, Float:vY=FLOAT_NAN, Float:vZ=FLOAT_NAN)
{
//	if (IsValidDynamicPickup(gPickup[artefact]))
//		DestroyDynamicPickup(gPickup[artefact]);

	if (vX==vX) {
		gPickup[artefact] = CreateDynamicPickup(1276, 1, vX, vY, vZ, 0,0,-1);

	} else {
		switch(random(4)) {
			case 0: {
				SendClientMessageToAll(-1,"Artefakt pojawil sie w okolicach teleportu /statek");
				gPickup[artefact] = CreateDynamicPickup(1276, 1, 1982.1211, 1587.8715, 22.7734, 0,0,-1);
			}
			case 1: {
				SendClientMessageToAll(-1,"Artefakt pojawil sie gdzies w kanalach.");
				gPickup[artefact] = CreateDynamicPickup(1276, 1, 2462.90, 2792.94,-73.17, 0,11,-1);
			}
			case 2: {
				SendClientMessageToAll(-1,"Artefakt pojawil sie w jaskinii ");
				gPickup[artefact] = CreateDynamicPickup(1276, 1,-2531.09, 1117.34,-65.19, 0,15,-1);
			}
			case 3: {
				SendClientMessageToAll(-1,"Artefakt pojawil sie w bazie wojskowej");
				gPickup[artefact] = CreateDynamicPickup(1276, 1,-34.36, 1893.15,-39.06, 0,0,-1);
			}
		}

		gmTemp[artefactDropped]=0;
	}
}

DropArtefact(playerid)
{
	new
	 Float:pVectors[e_Vectors],
	 buffer[128];
	 
	if(IsPlayerConnected(playerid))
	{

		RemovePlayerAttachedObject(playerid, ATTACH_TMPMISC);
		
		GetPlayerPos(playerid, pVectors[X], pVectors[Y], pVectors[Z]);
		CreateArtefact(pVectors[X], pVectors[Y], pVectors[Z]);
		gmTemp[artefactDropped]=GetTickCount();
		foreach(i)
		{
			format(buffer, sizeof buffer, TXT(i, 300), GetPlayerNick(playerid));
			Msg(i, 0xC6E0FFFF, buffer);
		}
	}
	else
	{
		CreateArtefact();//1982.1211, 1587.8715, 22.7734);
		gmTemp[artefactDropped]=0;
	}
	
	gmData[artefactOwner] = INVALID_PLAYER_ID;
	if (IsPlayerConnected(playerid))
		SetPlayerProperColor(playerid);
}


artefact_SetPlayerHolding(playerid){
	if (gmData[artefactOwner] != playerid) return;
	SetPlayerAttachedObject(playerid, ATTACH_TMPMISC, 1276, 1, 0.1, -0.2, 0.0, 90.0, 88.0, 0.0);	
}

artefact_OnPlayerPickup(playerid){
	if(pData[playerid][pAttraction] == A_NONE && pData[playerid][adminLevel]<1)
	{
		new buffer[127];
		DestroyDynamicPickup(gPickup[artefact]);
			
		gmData[artefactOwner] = playerid;
		SetPlayerProperColor(playerid);
		artefact_SetPlayerHolding(playerid);
			
		pTemp[playerid][artefactStartTime] = GetTickCount() / 1000;
		format(buffer, sizeof buffer, "{FFFFFF}%s{C6E0FF} podniosl%s {FFFFFF}artefakt{C6E0FF}!", GetPlayerNick(playerid), Kobieta(playerid) ? ("a") : ("") );
		SendClientMessageToAll(-1, buffer);
	}
	return;

}