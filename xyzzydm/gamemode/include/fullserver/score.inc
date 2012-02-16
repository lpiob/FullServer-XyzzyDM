// mistrz danej kategorii

forward FadePlayerAchievement(playerid,step);
forward FadePlayerScore(playerid,step,oldscore);

new
	players_achievements_ranks[MAX_PLAYERS][e_achievements];

new
 avt_shortnames[e_achievements][10]={
	"WALIZKI",
	"KILLS",
	"LONGPLAY",
	"RACEFCNT",
	"RACEDIST",
	"DERBYFAN",
	"DERBYWIN",
	"FOTOCASH",
	"DRIFTPTS"
};

avt_record(playerid,e_achievements:atype, score, artype=ART_ADD){
//	printf("Rejestrowanie rekordu typu %s - ilosc: %d", avt_shortnames[atype], score);
	if (pData[playerid][loggedIn] && pData[playerid][accountID]>0) {
		new buffer[1023];
		if (artype==ART_UPDATE)	// nadpisujemy wynik
			format(buffer,sizeof buffer,"INSERT INTO fs_players_achievements SET playerid=%d,shortname='%s',score=%d ON DUPLICATE KEY UPDATE score=%d",
				pData[playerid][accountID],avt_shortnames[atype],score,score);
		else // dodajemy do wyniku
			format(buffer,sizeof buffer,"INSERT INTO fs_players_achievements SET playerid=%d,shortname='%s',score=%d ON DUPLICATE KEY UPDATE score=score+(%d)",
				pData[playerid][accountID],avt_shortnames[atype],score,score);
//		printf("Q: %s",buffer);
		mysql_query(buffer,SQL_RI_BACKGROUND);
	}
	SetTimerEx("avt_refreshRankDesc",500,false,"dd",playerid,_:atype);
	return 1;
}

forward avt_refreshRankDesc(playerid,atype);
public avt_refreshRankDesc(playerid,atype){

//	printf("rdc %d %d",playerid,atype);
	avt_getCurrentRankDesc(playerid,e_achievements:atype,false);
}


avt_getCurrentRankDesc(playerid,e_achievements:atype,bool:quiet=false){

	if (!(pData[playerid][accountID]>0)) {
		players_achievements_ranks[playerid][atype]=0;
		return;
	}

	new buffer[1023];
	format(buffer,sizeof buffer,"SELECT a.name,ar.id,ar.rank,ar.replacename FROM fs_players_achievements pa JOIN fs_achievements a ON pa.shortname=a.shortname JOIN fs_achievements_ranks ar ON ar.shortname=pa.shortname AND ar.score<=pa.score WHERE pa.playerid=%d AND pa.shortname='%s' GROUP BY a.shortname ORDER BY ar.score ASC;",
			pData[playerid][accountID],avt_shortnames[atype]);

	mysql_query(buffer);
	mysql_store_result();
	if (!mysql_num_rows()) { mysql_free_result(); return; }	// brak wynikow
	mysql_fetch_row(buffer,"|");
	mysql_free_result();

//	printf("buffer: %s",buffer);

	new avt_name[33],avt_rankid,avt_rank[33],fullname[68],bool:replacename;
	sscanf(buffer,"p<|>s[33]ds[33]l",avt_name,avt_rankid,avt_rank,replacename);

//	printf("parsed buffer: %s %d %s %d",avt_name,avt_rankid,avt_rank,replacename);

//	printf("Current rank: %d, new rank: %d",players_achievements_ranks[playerid][atype],avt_rankid);


	if (!quiet && players_achievements_ranks[playerid][atype]!=avt_rankid) {	// zmienila sie ranga
		if (!replacename)
			format(fullname, sizeof fullname, "%s: %s", avt_name, avt_rank);
		else
			copy(avt_rank,fullname);
		ShowPlayerAchievement(playerid,fullname);

	}
	players_achievements_ranks[playerid][atype]=avt_rankid;
}

avt_getPlayerAchievements(playerid){
	// narazie nie pobieramy tylko zerujemy
	for(new i=0; i<sizeof avt_shortnames;i++)	// zoptymalizowac!
		avt_getCurrentRankDesc(playerid,e_achievements:i,true);	// po cichu, bez powiadamiania
	return;
}

avt_zeroPlayerAchievements(playerid){
	for(new i=0; i<sizeof avt_shortnames;i++)
		players_achievements_ranks[playerid][e_achievements:i]=0;
	return;
}

forward ShowPlayerAchievement(playerid,reason[]);
public ShowPlayerAchievement(playerid,reason[]){
	new str[255];
	format(str,sizeof str, "Nowe osiagniecie: {b}%s{/b}", reason);
	Msg(playerid,COLOR_INFO2,str);

	if (pTemp[playerid][tdanimAchievement]) 	// juz trwa animacja wiec ta wklejamy na czacie tylko
		return 1;

	format(str, sizeof str, "Nowe osiagniecie! ~n~~y~%s",reason);

	if (Audio_IsClientConnected(playerid))
		Audio_Play(playerid,AUDIOID_ACHIEVEMENT,false,false,true);
//	else
//		PlaySound(playerid,1149);	

	pTemp[playerid][tdanimAchievement]=true;

	pTextDraw[PTD_ACHIEVEMENT][playerid] = TextDrawCreate(300.000000, 270.000000, str);
	TextDrawAlignment(pTextDraw[PTD_ACHIEVEMENT][playerid], 3);
	TextDrawBackgroundColor(pTextDraw[PTD_ACHIEVEMENT][playerid], 255);
	TextDrawFont(pTextDraw[PTD_ACHIEVEMENT][playerid], 1);
	TextDrawLetterSize(pTextDraw[PTD_ACHIEVEMENT][playerid], 0.330000, 1.100000);
	TextDrawColor(pTextDraw[PTD_ACHIEVEMENT][playerid], 0xffffff00);
	TextDrawSetOutline(pTextDraw[PTD_ACHIEVEMENT][playerid], 0);
	TextDrawSetProportional(pTextDraw[PTD_ACHIEVEMENT][playerid], 1);
	TextDrawSetShadow(pTextDraw[PTD_ACHIEVEMENT][playerid], 1);
	TextDrawUseBox(pTextDraw[PTD_ACHIEVEMENT][playerid], 1);
	TextDrawBoxColor(pTextDraw[PTD_ACHIEVEMENT][playerid], 0xFFA50000);
	TextDrawTextSize(pTextDraw[PTD_ACHIEVEMENT][playerid], 0.000000, 162.000000);

	TextDrawShowForPlayer(playerid, pTextDraw[PTD_ACHIEVEMENT][playerid]);
	
	SetTimerEx("FadePlayerAchievement",250,false,"dd",playerid,1);
	return 1;
}

public FadePlayerAchievement(playerid,step){
	// 1-4	pokazywanie
	// 5 pauza
	// 6-10 wygaszanie
	// 11 usuwanie
	new add;
	if (step==11) {
		TextDrawHideForPlayer(playerid, pTextDraw[PTD_ACHIEVEMENT][playerid]);
		TextDrawDestroy(pTextDraw[PTD_ACHIEVEMENT][playerid]);
		pTemp[playerid][tdanimAchievement]=false;
		return;
	} else if (step==5)
		add=85;
	else if (step<5)
		add=step*17;
	else
		add=(11-step)*16;
	TextDrawColor(pTextDraw[PTD_ACHIEVEMENT][playerid], 0xffffff00+(add*3));
	TextDrawBoxColor(pTextDraw[PTD_ACHIEVEMENT][playerid], 0xFFA50000+add);
	TextDrawShowForPlayer(playerid, pTextDraw[PTD_ACHIEVEMENT][playerid]);
	step++;
	SetTimerEx("FadePlayerAchievement",(step==6 ? 7500 : 120)+(step*4),false,"dd",playerid,step);
	return;
}

forward GivePlayerScoreEx(playerid,amount);
public GivePlayerScoreEx(playerid,amount)
	GivePlayerScore(playerid,amount,"Zewnetrzny skrypt", true);



/// SCORE
GivePlayerScore(playerid,amount,reason[], bool:show=true){
#pragma unused reason
	if (!IsPlayerConnected(playerid) || IsPlayerNPC(playerid)) return;

	pData[playerid][respect] += amount;
	SetPlayerScore(playerid, pData[playerid][respect]);
	if (!show) return;

//	if (amount<5) return;
/*	if (Audio_IsClientConnected(playerid)) {
		if (amount>=15)
			Audio_Play(playerid,AUDIOID_BEEP4,false,false,true);
		else if (amount>=10)
			Audio_Play(playerid,AUDIOID_BEEP3,false,false,true);
		else if (amount>=5)
			Audio_Play(playerid,AUDIOID_BEEP2,false,false,true);
	} else {*/
	if (amount>=15)
		PlaySound(playerid,1137);	// sound_checkpoint_amber
	else if (amount>=10)
		PlaySound(playerid,1139);	// sound_checkpoint_red
	else if (amount>=5)
		PlaySound(playerid,1138);	// sound_checkpoint_green
//	}

	new str[5];

	if (pTemp[playerid][tdanimScore]!=0){	// juz trwa animacja naliczania score - wiec dodajemy tylko wynik i przedluzamy (jesli juz znika)
		pTemp[playerid][tdanimScore]+=amount;
		if (pTemp[playerid][tdanimScore]>0)
			format(str,sizeof str,"+%d",pTemp[playerid][tdanimScore]);
		else 
			format(str,sizeof str,"%d",pTemp[playerid][tdanimScore]);
		TextDrawSetString(pTextDraw[PTD_SCORECHANGE][playerid], str);
		return;
	}

	if (amount>0)
		format(str,sizeof str,"+%d",amount);
	else
		format(str,sizeof str,"%d",amount);

	pTemp[playerid][tdanimScore]=amount;

	pTextDraw[PTD_SCORECHANGE][playerid] = TextDrawCreate(395.000000, 410.000000, str);
	TextDrawAlignment(pTextDraw[PTD_SCORECHANGE][playerid], 2);
	TextDrawBackgroundColor(pTextDraw[PTD_SCORECHANGE][playerid], 96);
	TextDrawFont(pTextDraw[PTD_SCORECHANGE][playerid], 1);
	TextDrawLetterSize(pTextDraw[PTD_SCORECHANGE][playerid], 0.639999, 2.099999);
	TextDrawColor(pTextDraw[PTD_SCORECHANGE][playerid], (pTemp[playerid][tdanimScore]>0) ? 0x30ff30aa : 0xff3030aa);
	TextDrawSetOutline(pTextDraw[PTD_SCORECHANGE][playerid], 1);
	TextDrawSetProportional(pTextDraw[PTD_SCORECHANGE][playerid], 0);

	TextDrawShowForPlayer(playerid, pTextDraw[PTD_SCORECHANGE][playerid]);

	SetTimerEx("FadePlayerScore",250,false,"ddd",playerid,1,pTemp[playerid][tdanimScore]);
}

public FadePlayerScore(playerid,step,oldscore){
	// 1-4	pokazywanie
	// 5 pauza
	// 6-10 wygaszanie
	// 11 usuwanie
	if (pTemp[playerid][tdanimScore]!=oldscore && step>4)	// nastapila doliczenie score w momencie gdy bylo juz pokazywane, resetujemy krok animacji
		step = 4;
	
	new add;
	if (step==11) {
		TextDrawHideForPlayer(playerid, pTextDraw[PTD_SCORECHANGE][playerid]);
		pTemp[playerid][tdanimScore]=0;
		TextDrawDestroy(pTextDraw[PTD_SCORECHANGE][playerid]);
		return;
	} else if (step==5)
		add=85;
	else if (step<5)
		add=step*17;
	else
		add=(11-step)*16;
	TextDrawColor(pTextDraw[PTD_SCORECHANGE][playerid],((pTemp[playerid][tdanimScore]>0) ? 0x30ff3000 : 0xff303000)+(add*3));
	TextDrawShowForPlayer(playerid, pTextDraw[PTD_SCORECHANGE][playerid]);
	step++;
	SetTimerEx("FadePlayerScore",(step==6 ? 4500 : 60)+(step*4),false,"ddd",playerid,step,pTemp[playerid][tdanimScore]);
	return;
}

score_GivePlayerHourBonus(playerid, hour)
{
	if (hour<1) return;
	new
	 buffer[128],
	 exp, kasa;
	
	if(!IsFS(playerid))// && !pData[playerid][vipEnabled])
	{
		kasa=100000;
		exp = (hour * 25) + 75; 	// 100, 125, ...
	}
	else //if(IsFS(playerid))// && !pData[playerid][vipEnabled])
	{
		kasa=200000;
		exp = (hour * 25) + 125; 	// 150, 175, ...
	}

	format(buffer, sizeof buffer, "Dostajesz {b}%i respektu{/b} i {b}%d${/b} za nastepna godzine gry!", exp, kasa);
	Msg(playerid, COLOR_INFO, buffer, false);

	GivePlayerMoney(playerid,kasa);
	avt_record(playerid,e_longplay, hour, ART_UPDATE);
	GivePlayerScore(playerid, exp, "Bonus za czas gry");
}