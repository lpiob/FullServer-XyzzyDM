Labirynt_Start()
{
	new
	 aPlayers[MAX_QUEUE_LABIRYNT],
	 aMaxPlayers = 0,
	 randomSpawnIdx,
	 randomCheckpointIdx;
	
	foreach(playerid)
	{
		if(pData[playerid][aLabirynt])
		{
			if(AttractionAllowed(playerid))
			{
				aPlayers[aMaxPlayers++] = playerid;
				if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
			}
			else
			{
				pData[playerid][aLabirynt] = false;
				Msg(playerid, COLOR_ERROR, TXT(playerid, 285));
			}
		}
	}
	
	if(aMaxPlayers < 2)
	{
		foreach(playerid)
		{
			Msg(playerid, COLOR_INFO3, TXT(playerid, 272));
		}
		
		aData[A_LABIRYNT][aState] = A_STATE_OFF;
		
		return;
	}
	
	randomCheckpointIdx = random(sizeof aLabiryntCheckpoints);
	
	for(new i = 0; i < aMaxPlayers; i++)
	{
		pData[aPlayers[i]][pAttraction] = A_LABIRYNT;
		pData[aPlayers[i]][aLabirynt] = false;
		
		TogglePlayerControllable(aPlayers[i], false);
		SetPlayerArmour(aPlayers[i], 0.0);
		SetPlayerHealth(aPlayers[i], 100.0);
		ResetPlayerWeapons(aPlayers[i]);
		
		randomSpawnIdx = random(sizeof aLabiryntSpawns);
		
		Teleport(T_PLAYER, aPlayers[i], aLabiryntSpawns[randomSpawnIdx][X], aLabiryntSpawns[randomSpawnIdx][Y], aLabiryntSpawns[randomSpawnIdx][Z], aLabiryntSpawns[randomSpawnIdx][A], 0, VW_LABIRYNT);
		SetPlayerCheckpoint(aPlayers[i], aLabiryntCheckpoints[randomCheckpointIdx][X], aLabiryntCheckpoints[randomCheckpointIdx][Y], aLabiryntCheckpoints[randomCheckpointIdx][Z], 2.0);
	}
	
	gmTemp[aLabiryntPlayers] = aPlayers;
	gmTemp[aLabiryntMaxPlayers] = aMaxPlayers;
	gmTemp[aLabiryntCount] = 5;
	gmTemp[timerLabiryntCountdown] = SetTimer("Labirynt_Countdown", 700, true);
	gmTemp[aLabiryntStartTick] = GetTickCount();
}


labirynt_AttractionUpdate(tick){
	new
	 aPlayers = 0,
	 bool:aBreak = false;

	for(new i = 0; i < gmTemp[aLabiryntMaxPlayers]; i++)
	{
		if(gmTemp[aLabiryntPlayers][i] == INVALID_PLAYER_ID) continue;
			
		if(pData[gmTemp[aLabiryntPlayers][i]][pAttraction] == A_LABIRYNT && IsPlayerInCube(gmTemp[aLabiryntPlayers][i], 1549.6411, -1514.4098, 26.5469, 1445.6764, -1578.9307, 22.5469))
		{
			aPlayers++;
		}
	}
		
	if(aPlayers == 0)
	{
		aBreak = true;
		
		foreach(playerid)
			Msg(playerid, COLOR_INFO3, TXT(playerid, 424)); // Labirynt zosta³ przerwany z powodu braku graczy.
	}
	else if(tick - gmTemp[aLabiryntStartTick] >= 420000) // 7 minut
	{
		aBreak = true;
		
		foreach(playerid)
			Msg(playerid, COLOR_INFO3, TXT(playerid, 427)); // Labirynt zosta³ przerwany z powodu limitu czasu na ukoñczenie.
	}
	
	if(aBreak)
		labirynt_Cleanup();
}

labirynt_Cleanup(){
		aData[A_LABIRYNT][aState] = A_STATE_OFF;
	
		for(new i = 0; i < gmTemp[aLabiryntMaxPlayers]; i++)
		{
			if(gmTemp[aLabiryntPlayers][i] == INVALID_PLAYER_ID || GetPlayerVirtualWorld(gmTemp[aLabiryntPlayers][i])!=VW_LABIRYNT) continue;
			DisablePlayerSounds(gmTemp[aLabiryntPlayers][i]);
			DisablePlayerCheckpoint(gmTemp[aLabiryntPlayers][i]);
			pData[gmTemp[aLabiryntPlayers][i]][pAttraction] = A_NONE;
			SpawnPlayer(gmTemp[aLabiryntPlayers][i]);
		}

}
public Labirynt_Countdown()
{
	new
	 buffer[64];
	
	if(gmTemp[aLabiryntCount] <= 0)
	{
		format(buffer, sizeof buffer, "~g~GO!");
		KillTimer(gmTemp[timerLabiryntCountdown]);
	}
	else
	{
		format(buffer, sizeof buffer, "~r~%i", gmTemp[aLabiryntCount]);
	}
	
	for(new playerid = 0; playerid < gmTemp[aLabiryntMaxPlayers]; playerid++)
	{
		GameTextForPlayer(gmTemp[aLabiryntPlayers][playerid], buffer, 1000, 6);
		
		if(gmTemp[aLabiryntCount] == 0)
		{
			PlaySound(gmTemp[aLabiryntPlayers][playerid], 1057);
			TogglePlayerControllable(gmTemp[aLabiryntPlayers][playerid], true);
			PlaySound(gmTemp[aLabiryntPlayers][playerid], 1068);
		}
		else
		{
			PlaySound(gmTemp[aLabiryntPlayers][playerid], 1056);
		}
	}
	
	gmTemp[aLabiryntCount]--;
}
