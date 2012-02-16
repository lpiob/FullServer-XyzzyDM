forward GetFSAdminLeveL(playerid);
forward NotifyAdmins(msg[]);
forward Odliczanie(val);
forward MarkVehicleAsUsed(vid);

public GetFSAdminLeveL(playerid) {
	if (!pData[playerid][loggedIn] || !pData[playerid][accountID]) return 0;
	else return pData[playerid][adminLevel];
}

public NotifyAdmins(msg[]){
	MSGToAdmins(COLOR_ERROR, msg, false);
}


public Odliczanie(val){
	new str[15];
	if (val>0)
		format(str,sizeof str, "_~n~_~n~%d", val);
	else
		format(str,sizeof str, "_~n~_~n~~r~GO");

	foreach(i)
		if(pData[i][pAttraction]==A_NONE) {
			GameTextForPlayer(i,str,1000,6);
			PlayerPlaySound(i, val>0?1056:1057, val*val, val*val, val*val);
		}

	if (val>0)
		SetTimerEx("Odliczanie", 900, false, "d", val-1);

	return;
}

public MarkVehicleAsUsed(vid){
	tVehicleUsed[vid]=true;
	tVehicles[vid][vo_used]=true;
}